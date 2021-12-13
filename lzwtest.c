#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define MAX_NODE 70000

typedef struct node {
  char c[10];
  bool registerd;
} Node;

Node node_init(const char c[]) {
  Node node;
  strcpy(node.c, c);
  node.registerd = true;
  return node;
}

int node_find(Node* node, size_t size, char* value) {
  for (int i = 0; i < size; i++) {
    printf("%s == %s\n",node[i].c,value);
    if (strcmp(node[i].c, value) == 0) {
      return i;
    }
  }
  return -1;
}

void node_append(Node* node, size_t* size, char* value) {
  if (node_find(node, *size, value) == -1) {
    if (*(size)+1 >= MAX_NODE) {
      fprintf(stderr, "要素数が増えすぎました\n");
      return;
    }
    strcpy(node[*size].c, value);
    node[(int)*size].registerd = true;
    *(size) += 1;
  }
}

int main(int argc, char* argv[]) {
  Node *n = malloc(sizeof(Node)*10000);
  n[0] = node_init("0");
  n[1] = node_init("1");
  n[2] = node_init("2");
  n[3] = node_init("3");
  n[4] = node_init("4");
  n[5] = node_init("5");
  size_t size = 6;
  node_append(n,&size,"7");
  printf("%s\n",n[5].c);
  printf("%ld\n",size);
  printf("%d\n",node_find(n,size,"7"));
  free(n);
  return EXIT_SUCCESS;
}