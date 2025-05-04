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

#define CTRL_KEY(k) ((k) & 0x1f)

/*** data ***/

struct termios orig_termios;

struct editorConfig {
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
char editorReadKey(void) {
  int nread;
  char c;
  while ((nread = read(STDIN_FILENO, &c, 1)) != 1) {
    if (nread == -1 && errno != EAGAIN)
      die("read");
  }
  return c;
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
    abAppend(ab, "~", 1);

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
  abAppend(&ab, "\x1b[2J", 4);
  // put the cursor back at the top of the screen
  abAppend(&ab, "\x1b[H", 3);

  editorDrawRows(&ab);

  abAppend(&ab, "\x1b[H", 3);

  write(STDOUT_FILENO, ab.b, ab.len);
  abFree(&ab);
}

/*** input ***/

/*
 * waits for a keypress then processes it later...
 *
 * we will map various ctrl key combinations or special keys to different editor
 * functions
 */
void editorProcessKeypress(void) {
  char c = editorReadKey();

  switch (c) {
  case CTRL_KEY('q'):
    write(STDOUT_FILENO, "\x1b[2J", 4);
    write(STDOUT_FILENO, "\x1b[H", 3);
    exit(0);
    break;
  }
}

/*** init ***/

/*
 * initialize all the fields in the E struct
 */
void initEditor(void) {
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
