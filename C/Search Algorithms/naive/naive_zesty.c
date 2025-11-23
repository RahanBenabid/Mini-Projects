#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "data.h"

int matchNum = 0;

int *search(char *text, char *pattern) {
  // int matchNum = 0;
  int *match = (int *)calloc(0, sizeof(int));
  int m = 0;

  if (match == NULL) {
    printf("Memory not allocated for out\n");
    exit(0);
  }

  for (int i = 0; i < (int)strlen(text); i++) {
    if (text[i] == pattern[m]) {
      m++;
    } else {
      m = 0;
    }

    // last letter of pattern matched
    if (m == (int)strlen(pattern)) {
      // add the value i (index of match) to match
      int *temp = match;
      match = (int *)realloc(match, (matchNum + 1) * sizeof(int));
      if (!match) {
        printf("Allocation failed\n");
        match = temp;
      } else {
        match[matchNum] = i - (int)strlen(pattern);
        matchNum++;
      }
    }
  }

  return match;
}

int main(void) {
  int *match = search(text, pattern);

  for (int k = 0; k < matchNum; k++) {
    printf("\n\n %d\n", match[k]);
    for (int s = match[k]; s < match[k] + (int)strlen(pattern) + 1; s++) {
      printf("%c", text[s]);
    }
  }

  free(match);
  return 0;
}
