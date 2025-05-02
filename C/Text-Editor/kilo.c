#include <unistd.h>
#include <termios.h>
#include <stdlib.h>

struct termios orig_termios;

void disableRawMode(void) {
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

void enableRawMode(void) {
  tcgetattr(STDIN_FILENO, &orig_termios);
  // called automatically when the program exits using atexit()
  atexit(disableRawMode);
  
  struct termios raw = orig_termios;
  raw.c_lflag &= ~(ECHO);
  /*
  * the ECHO feature causes each key to be printed in the terminal, in here we turn it off
  * this is the same as when typing a password after using `sudo` in the terminal
  */
  
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

int main(void) {
  enableRawMode();
  
  char c;
  while (read(STDIN_FILENO, &c, 1) == 1 && c != 'q');
  return 0;
}
