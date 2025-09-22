#ifndef MATRICE_TRANSITION_
#define MATRICE_TRANSITION_

struct _trie {
  int maxNode;      /* Nombre maximal de noeuds du trie int */
  int nextNode;     /* Indice du prochain noeud disponible */
  int **transition; /* matrice de transition */
  char *finite;     /* etats terminaux */
};

typedef struct _trie *Trie;

Trie createTrie(int maxNode);
void insertInTrie(Trie trie, unsigned char *w);
int isInTrie(Trie trie, unsigned char *w);

// other stuff
void printing_the_trie(Trie trie, int maxNode);
void freeTrie(Trie trie);

#endif