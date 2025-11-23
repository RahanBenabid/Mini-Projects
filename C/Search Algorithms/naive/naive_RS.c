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
    printf("Memory not allocated for the match table\n");
    exit(0);
  }

  // append the pattern to the end of the text
  char * mutableText = malloc((textLength + patternLength + 1) * sizeof(char));
  if (mutableText == NULL) {
    printf("Could not allocate memory for mutableText\n");
    exit(0);
  }

  strcpy(mutableText, text);
  for (int i = 0; i < patternLength; i++) {
    mutableText[textLength + i] = pattern[i];
  }
  
  for (int j = 0; j < textLength; j++) {
    int k = 0;
    while (mutableText[j + k] == pattern[k]) {
      k++;
    }
    if (k == (int) strlen(pattern)) {
      int *temp = match;
      match = (int *)realloc(match, (matchNum + 1) * sizeof(int));
      if (!match) {
        printf("Allocation failed\n");
        match = temp;
      } else {
        match[matchNum] = j;
        matchNum++;
      }
    }
  }
  free(mutableText);
  
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
