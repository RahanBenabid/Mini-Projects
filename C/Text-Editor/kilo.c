/*** includes ***/

#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <termios.h>
#include <unistd.h>

/*** defines ***/

#define KILO_VERSION "0.0.1"

#define CTRL_KEY(k) ((k) & 0x1f)

/*
 * we set a representation for arrow keys that doesn't conflict with other kays
 * like wasd, we give them a large `int` value, which will be out of range of a
 * `char` representation
 *
 * by setting the first const to 1000, the others will get incremented values of
 * 1001 -> 1003
 */
enum editorKey {
  ARROW_LEFT = 1000,
  ARROW_RIGHT,
  ARROW_UP,
  ARROW_DOWN,
  DEL_KEY,
  HOME_KEY,
  END_KEY,
  PAGE_UP,
  PAGE_DOWN
};

/*** data ***/

struct termios orig_termios;

struct editorConfig {
  // coordinates of the cursor
  int cx, cy;
  int screenrows;
  int screencols;
  struct termios orig_termios;
};

// global variable containing our editor state
struct editorConfig E;

/*** terminal ***/

/*
 * prints an error message then exists the program
 *
 * most c library functions that fail will set the global errno variable to
 * indicate what the error was
 *
 * so when the other functions return -1 in failure, we call this function
 */
void die(const char *s) {
  // this for a clean exit in case of an error, same thing in the ctrl+q below
  write(STDOUT_FILENO, "\x1b[2J", 4);
  write(STDOUT_FILENO, "\x1b[H", 3);

  perror(s);
  exit(1);
}

void disableRawMode(void) {
  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &E.orig_termios) == -1) {
    die("tcsetattr");
  }
  /*
   * the TSCAFLUSH discards any unread uput
   * before applying the changes to the terminal
   */
}

// gets us fully into raw mode
void enableRawMode(void) {
  if (tcgetattr(STDIN_FILENO, &E.orig_termios) == -1)
    die("tcgetattr");

  /* called automatically when the program exits using `atexit()` this is to
   * make sure we store the original terminal attributes of the user
   */
  atexit(disableRawMode);

  struct termios raw = E.orig_termios;
  raw.c_iflag &= ~(BRKINT | INPCK | ISTRIP | ICRNL | IXON);
  /*
   * disable ctrl-s and ctrl-q
   * this comes from termios.hm the I stands for input flag XON comes from names
   * of the two control characters ctrl-s and ctrl-q produce
   *
   * ICRNL diables ctrl-m, I stands for input flag
   * CT stands for carriage return
   * NL stands for new line
   *
   * the other flags aren't really important but more for safety
   */

  raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
  /*
   * the ECHO feature causes each key to be printed in the terminal in here we
   * turn it off this is the same as when typing a password after using `sudo`
   * in the terminal
   *
   * we also turn off canonical mode, to be able to read input byte-by-byte now
   * the program quits as soon as you press `q`
   *
   * to disable ctrl-c and ctrl-z and ctrl-y we use ISIG
   *
   * IEXTEN  diables ctrl-v and fixes ctrl-o
   */

  raw.c_oflag &= ~(OPOST);

  raw.c_cflag |= (CS8);
  /*
   * this ain't a flag but more of of mask with multiple bits that we set using
   * the bitwise OR, it sets the character size to be 8 bits per byte, which is
   * already the case for most systems
   */

  raw.c_cc[VMIN] = 0;
  /*
   * this sets the min number of bytes of input needed before read() can return
   * when we set it to 0, read() returns as soon as there is any input to be
   * read
   */

  raw.c_cc[VTIME] = 1;
  /*
   * read() returns as soon as there is any input to be read
   */

  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1)
    die("tcsetattr");
}

/*
 * wait for one keypress then return it later
 */
int editorReadKey(void) {
  int nread;
  char c;
  while ((nread = read(STDIN_FILENO, &c, 1)) != 1) {
    if (nread == -1 && errno != EAGAIN)
      die("read");
  }
  if (c == '\x1b') {
    char seq[3];
    if (read(STDIN_FILENO, &seq[0], 1) != 1)
      return '\x1b';
    if (read(STDIN_FILENO, &seq[1], 1) != 1)
      return '\x1b';

    if (seq[0] == '[') {
      if (seq[1] >= '0' && seq[1] <= '9') {
        if (read(STDIN_FILENO, &seq[2], 1) != 1)
          return '\x1b';
        if (seq[2] == '~') {
          switch (seq[1]) {
          case '1':
            return HOME_KEY;
          case '3':
            return DEL_KEY;
          case '4':
            return END_KEY;
          case '5':
            return PAGE_UP;
          case '6':
            return PAGE_DOWN;
          case '7':
            return HOME_KEY;
          case '8':
            return END_KEY;
          }
        }
      } else {
        switch (seq[1]) {
        case 'A':
          return ARROW_UP;
        case 'B':
          return ARROW_DOWN;
        case 'C':
          return ARROW_RIGHT;
        case 'D':
          return ARROW_LEFT;
        case 'H':
          return HOME_KEY;
        case 'F':
          return END_KEY;
        }
      }
    } else if (seq[0] == 'O') {
      switch (seq[1]) {
      case 'H':
        return HOME_KEY;
      case 'F':
        return END_KEY;
      }
    }

    return '\x1b';
  } else {
    return c;
  }
}

int getCursorPosition(int *rows, int *cols) {
  char buf[32];
  unsigned int i = 0;

  /*
   * ask the terminal for the cursor position
   *
   * the terminal will repond with an escape sequence such as \x1b[24;80R, where
   * 24 is the row number and 80 is the column number
   */
  if (write(STDOUT_FILENO, "\x1b[6n", 4) != 4)
    return -1;

  while (i < sizeof(buf) - 1) {
    if (read(STDIN_FILENO, &buf[i], 1) != 1)
      break;
    if (buf[i] == 'R')
      break;
    i++;
  }
  buf[i] = '\0';

  /*
   * parse the two number using sscanf()
   * we make sure it responded with an escape sequence
   * we pass a pointer to the third character to skip '\x1b['
   * then we parse to integers seperated by a `;` and put them into rows and
   * cols
   */
  if (buf[0] != '\x1b' || buf[1] != '[')
    return -1;
  if (sscanf(&buf[2], "%d;%d", rows, cols) != 2)
    return -1;

  return 0;
}

int getWindowSize(int *rows, int *cols) {
  struct winsize ws;

  /*
   * error return with the code -1  and also the values aren't 0 so it doesn't
   * create an error  on success, we pass the values back by setting the int
   * references that were passed to the function
   */
  if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0) {
    if (write(STDOUT_FILENO, "\x1b[999C\x1b[999B", 12) != 12)
      return -1;
    return getCursorPosition(rows, cols);
  } else {
    *cols = ws.ws_col;
    *rows = ws.ws_row;
    return 0;
  }
}

/*** append buffer ***/

/*
 * create a dynamic string that supports appending this consists of a pointer to
 * our buffer in memory and a lengthABUF_INIT represents an empty buffer, it
 * acts as a contructor
 */
struct abuf {
  char *b;
  int len;
};

#define ABUF_INIT {NULL, 0}

/*
 * we use realloc to make sure enough memory to available to hold the new string
 *
 * reaclloc will give us a block of mem the size of the current string plusthe
 * new string we are just appending
 *
 * so it will either extend the current size of the block of memory or will take
 * care of freeing the current block and allocating a new one elsewhere that is
 * big enough for the new string
 *
 * then we use a memcpy to copy the string s after the end of the current data
 * in the buffer
 *
 * we then update the pointer and length of the abuf using the new values
 */
void abAppend(struct abuf *ab, const char *s, int len) {
  char *new = realloc(ab->b, ab->len + len);

  if (new == NULL)
    return;
  memcpy(&new[ab->len], s, len);
  ab->b = new;
  ab->len += len;
}

// acts like a destructor to deallocate the dyn-mem used by abuf
void abFree(struct abuf *ab) { free(ab->b); }

/*** output ***/

void editorDrawRows(struct abuf *ab) {
  // draw the edges using the E struct for something dynamic
  for (int y = 0; y < E.screenrows; y++) {
    if (y == E.screenrows / 3) {
      char welcome[80];

      // snprintf is better for formatting buffers for later use
      int welcomelen = snprintf(welcome, sizeof(welcome),
                                "Kilo editor -- version %s", KILO_VERSION);
      if (welcomelen > E.screencols)
        welcomelen = E.screencols;
      int padding = (E.screencols - welcomelen) / 2;
      if (padding) {
        abAppend(ab, "~", 1);
        padding--;
      }
      while (padding--)
        abAppend(ab, " ", 1);
      abAppend(ab, welcome, welcomelen);
    } else {
      abAppend(ab, "~", 1);
    }

    // clear each line
    abAppend(ab, "\x1b[K", 3);
    if (y < E.screenrows - 1) {
      abAppend(ab, "\r\n", 2);
    }
  }
}

void editorRefreshScreen(void) {
  /*
   * writes an escape sequence into the terminalwhich instruct the terminal to
   * do various things like coloring text or moving the cursor around or
   * clearing parts of the screen
   */

  struct abuf ab = ABUF_INIT;

  /*
   * there could be a possible flicker effect when trying to draw the scene, the
   * cursor might appear in the middle of the screen for a split second while
   * the terminal is drawing to the screen so we hide the cursor before
   * refereshing the screen then display it again
   * the `h` and `l` commands are for set and reset mode
   */
  abAppend(&ab, "\x1b[?25l", 6);
  // put the cursor back at the top of the screen
  abAppend(&ab, "\x1b[H", 3);

  editorDrawRows(&ab);

  // move the cursor to the stored position
  char buf[32];
  snprintf(buf, sizeof(buf), "\x1b[%d;%dH", E.cy + 1, E.cx + 1);
  abAppend(&ab, buf, strlen(buf));

  // re-diplay the cursor
  abAppend(&ab, "\x1b[?25h", 6);

  // write the buffer's contents our to standard output & free memory
  write(STDOUT_FILENO, ab.b, ab.len);
  abFree(&ab);
}

/*** input ***/

void editorMoveCursor(int key) {
  switch (key) {
  case ARROW_LEFT:
    if (E.cx != 0) {
      E.cx--;
    }
    break;
  case ARROW_RIGHT:
    if (E.cx != E.screencols - 1) {
      E.cx++;
    }
    break;
  case ARROW_UP:
    if (E.cy != 0) {
      E.cy--;
    }
    break;
  case ARROW_DOWN:
    if (E.cy != E.screenrows - 1) {
      E.cy++;
    }
    break;
  }
}

/*
 * waits for a keypress then processes it later...
 *
 * we will map various ctrl key combinations or special keys to different editor
 * functions
 */
void editorProcessKeypress(void) {
  int c = editorReadKey();

  switch (c) {
  case CTRL_KEY('q'):
    write(STDOUT_FILENO, "\x1b[2J", 4);
    write(STDOUT_FILENO, "\x1b[H", 3);
    exit(0);
    break;

  case HOME_KEY:
    E.cx = 0;
    break;

  case END_KEY:
    E.cx = E.screencols - 1;
    break;

  case PAGE_UP:
  case PAGE_DOWN: {
    int times = E.screenrows;
    while (times--)
      editorMoveCursor(c == PAGE_UP ? ARROW_UP : ARROW_DOWN);
  }

  case ARROW_UP:
  case ARROW_DOWN:
  case ARROW_LEFT:
  case ARROW_RIGHT:
    editorMoveCursor(c);
    break;
  }
}

/*** init ***/

/*
 * initialize all the fields in the E struct
 */
void initEditor(void) {
  E.cx = 0;
  E.cy = 0;
  if (getWindowSize(&E.screenrows, &E.screencols) == -1)
    die("getWindowSize");
}

int main(void) {
  enableRawMode();
  initEditor();

  while (1) {
    editorRefreshScreen();
    editorProcessKeypress();
  }

  return 0;
}

// source https://viewsourcecode.org/snaptoken/kilo/03.rawInputAndOutput.html
