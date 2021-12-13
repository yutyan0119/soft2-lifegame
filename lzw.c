#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define MAX_NODE 70000
#define CODE_SIZE 100

typedef struct node {
  char c[100];
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
    if (strcmp(node[i].c, value) == 0) {
      return i;
    }
  }
  return -1;
}

void node_append(Node* node, size_t* size, char* value) {
  if (node_find(node, *size, value) == -1) {
    if (*(size) + 1 >= MAX_NODE) {
      fprintf(stderr, "要素数が増えすぎました\n");
      return;
    }
    strcpy(node[*size].c, value);
    node[(int)*size].registerd = true;
    *(size) += 1;
  }
}

int main(int argc, char* argv[]) {
  Node n[2000];
  n[0] = node_init("0");
  n[1] = node_init("1");
  n[2] = node_init("2");
  n[3] = node_init("3");
  n[4] = node_init("4");
  n[5] = node_init("5");
  size_t size = 6;
  char code[CODE_SIZE] = "010101010";
  char ans[CODE_SIZE + 2];
  size_t anssize = 0;
  ans[0] = 4;
  for (int i = 0; i < strlen(code); i++) {
    char s[1000000];
    for (int i = 0; i < 1000000; i++) {
      s[i] = '\0';
    }

    int j = 0;
    s[0] = code[i];
    while (node_find(n, size, s) != -1) {
      if (i + j == strlen(code)) {
        break;
      }
      s[j] = code[i + j];
      j++;
    }
    if (i + j == strlen(code) && node_find(n, size, s) != -1) {
      ans[anssize + 1] = node_find(n, size, s);
      ans[anssize + 2] = 5;
      anssize++;
      break;
    } else {
      i += strlen(s) - 2;
      node_append(n, &size, s);
      s[j - 1] = '\0';
      ans[anssize + 1] = node_find(n, size, s);
      anssize++;
    }
  }
  for (int i = 0; i < anssize + 2; i++) {
    printf("%d,", ans[i]);
  }
  printf("\n");
  return EXIT_SUCCESS;
}