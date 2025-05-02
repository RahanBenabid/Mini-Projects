/*** includes ***/

#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <stdlib.h>
#include <sys/types.h>

/*** data ***/
struct termios orig_termios;

/*** terminal ***/

/*
* prints an error message then exists the program
*
* most c library functions that fail will set the global errno variable to indicate what the error was
*
* so when the other functions return -1 in failure, we call this function
*/
void die (const char *s) {
  perror(s);
  exit(1);
}

void disableRawMode(void) {
  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1) {
    die("tcsetattr");
  }
  // the TSCAFLUSH discards any unread uput before applying the changes to the terminal
}

// gets us fully into raw mode
void enableRawMode(void) {
  if (tcgetattr(STDIN_FILENO, &orig_termios) == -1) die("tcgetattr");
  
  /* called automatically when the program exits using atexit()
  * this is to make sure we store the original terminal attributes of the user
  */
  atexit(disableRawMode);
  
  struct termios raw = orig_termios;
  raw.c_iflag &= ~(BRKINT | INPCK | ISTRIP | ICRNL | IXON);
  /*
  * disable ctrl-s and ctrl-q
  * this comes from termios.hm the I stands for input flag, XON comes from names of the two control characters ctrl-s and ctrl-q produce
  *
  * ICRNL diables ctrl-m, I stands for input flag, CT stands for carriage return, NL stands for new line
  *
  * the other flags aren't really important but more for safety
  */
  
  
  raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
  /*
  * the ECHO feature causes each key to be printed in the terminal, in here we turn it off
  * this is the same as when typing a password after using `sudo` in the terminal
  *
  * we also turn off canonical mode, to be able to read input byte-by-byte, now the program quits as soon as you press `q`
  *
  * to disable ctrl-c and ctrl-z and ctrl-y we use ISIG
  *
  * IEXTEN  diables ctrl-v and fixes ctrl-o
  */
  
  raw.c_oflag &= ~(OPOST);
  
  raw.c_cflag |= (CS8);
  /*
  * this ain't a flag but more of of mask with multiple bits, that we set using the bitwise OR, it sets the character size to be 8 bits per byte, which is already the case for most systems
  */
  
  raw.c_cc[VMIN] = 0;
  /*
  * this sets the min number of bytes of input needed before read() can return
  * when we set it to 0, read() returns as soon as there is any input to be read
  */
  
  raw.c_cc[VTIME] = 1;
  /*
  * read() returns as soon as there is any input to be read
  */
  
  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) die("tcsetattr");
}

/*** init ***/

int main(void) {
  enableRawMode();
  
  while (1) {
    char c = '\0';
    if (read(STDIN_FILENO, &c, 1) == -1 && errno != EAGAIN) die("read");
    // test whether a character is control, so we don't print it on the screen, not just the ctrl character, but all the 0-31 ASCII codes
    if (iscntrl(c)) {
      printf("%d\r\n", c);
    } else {
      printf("%d ('%c')\r\n", c, c);
    }
    if (c == 'q') break;
  }
  
  return 0;
}
