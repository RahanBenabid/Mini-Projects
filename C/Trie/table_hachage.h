struct _list {
  int startNode,        /* etat de depart de la transition */
      targetNode;       /* cible de la transition */
  unsigned char letter; /* etiquette de la transition */
  struct _list *next;   /* maillon suivant */
};

typedef struct _list *List;

struct _trie {
  int maxNode;      /* Numbre max de noeuds du trie */
  int nextNode;     /* Indice du prochain noeud disponible */
  List *transition; /* listes d'adjacence */
  char *finite;     /* etats terminaux */
};

typedef struct _trie *Trie;

Trie createTrie(int maxNode);

void insertInTrie(Trie trie, unsigned char *w);

int isInTrie(Trie trie, unsigned char *w);
