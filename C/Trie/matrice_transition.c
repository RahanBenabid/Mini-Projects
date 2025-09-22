#include "matrice_transition.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Trie createTrie(int maxNode) {
  Trie trie = malloc(sizeof(struct _trie));

  trie->maxNode = maxNode;
  trie->nextNode = 1;
  trie->transition = malloc(sizeof(int *) * maxNode);
  if (!trie->transition) {
    return NULL;
  }

  // go through each line, and in each one allocate for the
  for (int i = 0; i < maxNode; i++) {
    trie->transition[i] = malloc(sizeof(int) * 256);
  }
  trie->finite = malloc(sizeof(char) * maxNode);
  if (!trie->finite) {
    return NULL;
  }

  // initialize the martix with -1 in each cell
  for (int i = 0; i < maxNode; i++) {
    for (int j = 0; j < 256; j++) {
      trie->transition[i][j] = -1;
    }
  }

  for (int i = 0; i < maxNode; i++) {
    trie->finite[i] = 0;
  }

  return trie;
}

// later should add the destructor below

void insertInTrie(Trie trie, unsigned char *w) {
  int currentNode = 0;
  size_t n = strlen((char*) w);
  for (int i = 0; i < n; i++) {
    unsigned char currentCharacter = w[i];
    if (trie->transition[currentNode][currentCharacter] == -1) {
      // during an insertion, we check if we're passing the limit
      if (trie->nextNode >= trie->maxNode) {
        perror("Trie is full, cannot insert more nodes");
        return;
      }
      trie->transition[currentNode][currentCharacter] = trie->nextNode;
      trie->nextNode++;
    }
    currentNode = trie->transition[currentNode][currentCharacter];
  }
  // should be only true for terminal nodes, but i'm keeping it as is for now
  trie->finite[currentNode] = 1;
}

int isInTrie(Trie trie, unsigned char *w) {
  int currentNode = 0;
  size_t n = strlen((char*) w);
  for (int i = 0; i < n; i++) {
    unsigned char currentCharacter = w[i];
    if (trie->transition[currentNode][currentCharacter] != -1) {
      currentNode = trie->transition[currentNode][currentCharacter];
    } else {
      return 0;
    }
  }
  // might only return if it's terminal, not thinking about it for now
  return 1;
}

// ignore this function for now
void printing_the_trie(Trie trie, int maxNode) {
  for (int i = 0; i < maxNode; i++) {
    // go to a new line for the node i
    printf("\nnode: %d\n", i);
    for (int j = 0; j < 256; j++) {
      // print the content of the line
      printf("  %d,", trie->transition[i][j]);
    }
  }
}

int main(int argc, char *argv[]) {
  const int maxNode = 5;
  Trie trie = createTrie(maxNode);

  char *word = "car";
  insertInTrie(trie, word);

  word = "cat";
  insertInTrie(trie, word);

  printf("\nterminal nodes: ");
  for (int i = 0; i < maxNode; i++) {
    if (trie->finite[i]) {
      printf("%d ", i);
    }
  }
  printf("\n");

  int searchResult;

  searchResult = isInTrie(trie, "car");
  printf("searching %s: %d", "car", searchResult);

  searchResult = isInTrie(trie, "yol");
  printf("\nsearching %s: %d\n", "yol", searchResult);

  return 0;
}
