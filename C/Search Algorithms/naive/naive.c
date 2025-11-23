#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "data.h"

int matchNum = 0;

int *search(char *text, char *pattern) {
  int textLength = (int)strlen(text);
  int patternLength = (int)strlen(pattern);

  int *match = (int *)calloc(1, sizeof(int));
  if (match == NULL) {
    printf("Memory not allocated for out\n");
    exit(0);
  }

  for (int i = 0; i <= textLength - patternLength; i++) {
    int j;
    for (j = 0; j < patternLength; j++) {
      if (pattern[j] != text[i + j]) {
        break;
      }
    }

    if (j == patternLength) {
      int *temp = match;
      match = (int *)realloc(match, (matchNum + 1) * sizeof(int));
      if (!match) {
        printf("Allocation failed\n");
        match = temp;
      } else {
        match[matchNum] = i;
        matchNum++;
      }
    }
  }
  return match;
}

int main(void) {
  int *match = search(text, pattern);

  for (int k = 0; k < matchNum; k++) {
    printf("match @ index: %d\n", match[k]);
    for (int s = match[k]; s < match[k] + (int)strlen(pattern); s++) {
      printf("%c", text[s]);
    }
    printf("\n\n");
  }

  free(match);
  return 0;
}
