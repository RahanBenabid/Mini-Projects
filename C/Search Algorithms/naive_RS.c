#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "data.h"

int matchNum = 0;

int *search(char *text, char *pattern) {
  int *match = (int *)calloc(1, sizeof(int));
  int textLength = (int) strlen(text);
  int patternLength = (int) strlen(pattern);
  
  if (match == NULL) {
    printf("Memory not allocated for out\n");
    exit(0);
  }

  for (int i = 0; i <= textLength - patternLength; i++) {
    if (text[i] != pattern[0]) continue; // fast loop
    
    int j;
    for (j = 0; j < patternLength; j++) {
      if (pattern[j] != text[i+j]) {
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
    printf("\n\n %d\n", match[k]);
    for (int s = match[k]; s < match[k] + (int)strlen(pattern); s++) {
      printf("%c", text[s]);
    }
  }


  free(match);
  return 0;
}
