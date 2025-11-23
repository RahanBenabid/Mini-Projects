#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "data.h"

int matchNum = 0;

int *bon_pref_calc(char *pattern) {
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

int *meil_pref_calc(char *pattern) {

  int *bon_pref = bon_pref_calc(pattern);

  int patternLength = (int)strlen(pattern);

  printf("\n");

  int *meil_pref = calloc(patternLength + 1, sizeof(int));

  meil_pref[0] = -1;
  meil_pref[patternLength] = bon_pref[patternLength];

  for (int i = 1; i < patternLength; i++) {
    if (pattern[i] != pattern[bon_pref[i]]) {
      meil_pref[i] = bon_pref[i];
    } else {
      meil_pref[i] = meil_pref[bon_pref[i]];
    }
  }

  free(bon_pref);
  return meil_pref;
}

int *search(char *text, char *pattern) {
  int matchCount = 1;
  int *match = calloc(matchCount, sizeof(int));

  int textLength = (int)strlen(text);
  int patternLength = (int)strlen(pattern);

  int *meil_pref = meil_pref_calc(pattern);

  int j = -1;
  int i = -1;

  while (i < textLength) {
    if (text[i] == pattern[j]) {
      j++;
      i++;
      if (j == patternLength) {
        // match
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
      j = meil_pref[j];
    }

    if (j == -1) {
      i++;
      j = 0;
    }
  }

  free(meil_pref);
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

  return 0;
}
