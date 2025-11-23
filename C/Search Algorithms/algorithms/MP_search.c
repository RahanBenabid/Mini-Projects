#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "data.h"

int matchNum = 0;

int *preprocessing(char *pattern) {
  const int patternLength = strlen(pattern);
  int *bon_pref = calloc(strlen(pattern) + 1, sizeof(char));

  int i = 1;
  int j = 0;

  while (i < patternLength) {
    if (pattern[i] == pattern[j]) {
      j++;
      bon_pref[i] = j;
      i++;
    } else {
      if (j != 0) {
        j = bon_pref[j - 1];
      } else {
        bon_pref[i] = 0;
        i++;
      }
    }
  }

  for (int k = patternLength; k > 0; k--) {
    bon_pref[k] = bon_pref[k - 1];
  }
  bon_pref[0] = -1;

  return bon_pref;
}

int *search(char *text, char *pattern) {
  int matchCount = 1;
  int *match = calloc(matchCount, sizeof(int));

  int textLength = (int)strlen(text);
  int patternLength = (int)strlen(pattern);

  int *bon_pref = preprocessing(pattern);

  int j = -1;
  int i = -1;

  while (i < textLength) {
    if (text[i] == pattern[j]) {
      j++;
      i++;
      if (j == patternLength) {
        int matchIndex = i - j;
        int *temp = match;
        match = (int *)realloc(match, (matchNum + 1) * sizeof(int));
        if (!match) {
          printf("Allocation failed\n");
          match = temp;
        } else {
          match[matchNum] = matchIndex;
          matchNum++;
        }
      }
    } else {
      j = bon_pref[j];
    }

    if (j == -1) {
      i++;
      j = 0;
    }
  }

  free(bon_pref);
  return match;
}

void test(void) {
  int *bon_pref = preprocessing(pattern);

  for (int i = 0; i < (int)strlen(pattern) + 1; i++) {
    printf("%d | ", bon_pref[i]);
  }
  printf("\n ");
  for (int i = 0; i < (int)strlen(pattern); i++) {
    printf("%c | ", pattern[i]);
  }
}

int main(void) {
  // test();

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
