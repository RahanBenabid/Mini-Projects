/*
 * like seen in the TP, this one will help us not allocate a table with huge
 * amounts of zeros in it, it's more efficient
  */
  #include <stdlib.h>

struct _list {
  int startNode, targetNode;
  unsigned char letter;
  struct _list *next;
};

typedef struct _list List;

struct _trie {
  int maxNode;
  int nextNode;
  List *transition;
  char *finite;
};

typedef struct _trie *Trie;

Trie createTrie(int maxNode) {
  Trie trie;
  int* ligne = malloc(256 * sizeof(int));
  trie->maxNode = maxNode;
  trie->nextNode = 0;
  trie->transition = malloc(maxNode * sizeof(ligne));
  
  return trie;
}

void insertInTrie(Trie trie, unsigned char *w) {}

int isInTrie(Trie trie, unsigned char *w) {}

int main(int argc, char *argv[]) { return 0; }
