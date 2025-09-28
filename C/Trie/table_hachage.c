#include "table_hachage.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// value of m
#define MAX_BUCKETS (7)

List createList(int max);

Trie createTrie(int maxNode) {
  // should add error handling

  printf("Warning: Max number of buckets allowed is %d",
         MAX_BUCKETS); // like mentioned in the homework, we need to have n/m <
                       // 0,75

  //  allocate the struct
  Trie trie = malloc(sizeof(struct _trie));
  if (trie == NULL) {
    fprintf(stderr, "createTrie: failed to allocate trie struct\n");
  }

  // allocate the two ints
  trie->maxNode = maxNode;
  trie->nextNode = 1;

  // allocate the finite nodes list
  trie->finite = (char *)calloc((size_t)maxNode, sizeof(char));
  if (trie->finite == NULL) {
    fprintf(stderr, "createTrie: failed to allocate finite");
    free(trie);
    return NULL;
  }

  // allocate the bucket array of size m and initialize to NULL
  trie->transition = (List *)calloc((size_t)MAX_BUCKETS, sizeof(List));
  if (trie->transition == NULL) {
    fprintf(stderr, "createTrie: failed to alocate transition buckets");
    free(trie);
    return NULL;
  }

  return trie;
}

int h(int state, unsigned char letter, int m) {
  // multiplicative hash that mix state and letter
  // deterministic
  // uses 64 bit
  unsigned long long key =
      ((unsigned long long)state * 1315423911ULL) ^ (unsigned long long)letter;
  return (int)(key % (unsigned long long)m);
}

static List findTransitionNode(Trie trie, int state, unsigned char letter) {
  int bucket = h(state, letter, MAX_BUCKETS);
  List cur = trie->transition[bucket];
  while (cur != NULL) {
    if (cur->startNode == state && cur->letter == letter) {
      return  cur;
    }
    cur = cur->next;
  }
  return NULL;
}

static int addOrGetTransition(Trie trie, int state, unsigned char letter) {
  List existing = findTransitionNode(trie, state, letter);
  if (existing != NULL) {
    return existing->targetNode;
  }
  
  if (trie->nextNode >= trie->maxNode) {    fprintf(stderr, "insertInTrie: reached maxNode capacity (%d)\n", trie->maxNode);    return -1;  }    // allocate a new List  List node = (List)malloc(sizeof(struct _list));  if (node == NULL) {  fprintf(stderr, "insertInTrie: failed to allocate list node\n");  return -1;  }    node->startNode = state;  node->letter = letter;  node->targetNode = trie->nextNode++;  int bucket = h(state, letter, MAX_BUCKETS);  node->next = NULL;    List head = trie->transition[bucket];  if (head == NULL) {  trie->transition[bucket] = node;  } else {  List cur = head;  while (cur->next != NULL) {  cur = cur->next;  }  cur->next = node;  }    return node->targetNode;
}

void insertInTrie(Trie trie, unsigned char *w) {
  if (trie == NULL || w == NULL) {
    return;
  }

  int state = 0;
  size_t len = strlen((const char *)w);
  for (size_t i = 0; i < len; i++) {
    unsigned char c = w[i];
    int next = addOrGetTransition(trie, state, c);
    if (next < 0) {
      return;
    }
    state = next;
  }

  if (state >= 0 && state < trie->maxNode) {
    trie->finite[state] = 1;
  }
}

int isInTrie(Trie trie, unsigned char *w) {
  if (trie == NULL || w == NULL) {
    return 0;
  }

  int state = 0;
  size_t len = strlen((const char *)w);
  for (size_t i = 0; i < len; i++) {
    unsigned char c = w[i];
    List node = findTransitionNode(trie, state, c);
    if (node == NULL) {
      return 0;
    }
    state = node->targetNode;
  }
  if (state >= 0 && state < trie->maxNode) {
    return trie->finite[state] ? 1 : 0;
  }
  return 0;
}

int main() {
  const int maxNode = 10;
  Trie trie = createTrie(maxNode);
  if (trie == NULL) {
    return 1;
  }

  insertInTrie(trie, (unsigned char *)"abc");
  insertInTrie(trie, (unsigned char *)"abd");
  insertInTrie(trie, (unsigned char *)"b");

  printf("\n");
  printf("isInTrie(\"abc\") = %d\n", isInTrie(trie, (unsigned char *)"abc"));
  printf("isInTrie(\"abd\") = %d\n", isInTrie(trie, (unsigned char *)"abd"));
  printf("isInTrie(\"ab\") = %d\n", isInTrie(trie, (unsigned char *)"ab"));
  printf("isInTrie(\"b\") = %d\n", isInTrie(trie, (unsigned char *)"b"));
  printf("isInTrie(\"c\") = %d\n", isInTrie(trie, (unsigned char *)"c"));

  // TODO: free memory (list and trie)
  return 0;
}
