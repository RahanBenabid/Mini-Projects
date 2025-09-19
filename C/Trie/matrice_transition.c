#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct _trie {
  int maxNode;      /* Nombre maximal de noeuds du trie int */
  int nextNode;     /* Indice du prochain noeud disponible */
  int **transition; /* matrice de transition */
  char *finite;     /* etats terminaux */
};

typedef struct _trie *Trie;

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

  return trie;
}

// later should add the destructor below

void insertInTrie(Trie trie, unsigned char *w) {
  if (strlen(w) >= trie->maxNode) {
    perror("the word length is longer than the trie max number of nodes");
    return;
  }
  int currentNode = 0;
  for (int i = 0; i < strlen(w); i++) {
    unsigned char currentCharacter = w[i];
    if (trie->transition[currentNode][currentCharacter] == -1) {
      trie->transition[currentNode][currentCharacter] = trie->nextNode;
      trie->nextNode++;
    }
    currentNode = trie->transition[currentNode][currentCharacter];
  }
  trie->finite[currentNode] = 1;
}

int isInTrie(Trie trie, unsigned char *w) {}

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
  const int maxNode = 4;
  Trie trie = createTrie(maxNode);

  char *word = "car";
  insertInTrie(trie, word);

  word = "cat";
  insertInTrie(trie, word);

  printing_the_trie(trie, maxNode);
  
  printf("\nthe list of the terminal nodes: %s", trie->finite);

  return 0;
}
