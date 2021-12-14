#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>  // sleep()関数を使う
#define MAX_NODE 10000
#define CODE_SIZE 10000

typedef struct node {
  char c[400];
  bool registerd;
} Node;

Node node_init(const char c[]) {
  Node node;
  strcpy(node.c, c);
  node.registerd = true;
  return node;
}

int node_find(Node *node, size_t size, char *value) {
  for (int i = 0; i < size; i++) {
    if (strcmp(node[i].c, value) == 0) {
      return i;
    }
  }
  return -1;
}

void node_append(Node *node, size_t *size, char *value) {
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

int return0xnum(char buf[]) {
  int num = 0;
  for (int i = 0; i < 8; i++) {
    num += buf[i] * 1 << (7 - i);
  }
  return num;
}

void makegif(const int width, const int height, const int a[][width], FILE *fp,
             unsigned char gifdata[], int *gifdataindex);


int main() {
  FILE *fp = stdout;
  const int height = 40;
  const int width = 70;
  int cell[height][width];
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      cell[y][x] = 0;
    }
  }
  FILE *file;
  char *filename;
  file = fopen("animetest.gif", "w");
  unsigned char gifdata[1000] = {
      'G',  'I',  'F',  '8',  '9',   'a',  width,  0x00, height, 0x00,
      0x80, 0x00, 0x00, 0x41, 0x69,  0xE1, 0xFF,   0xFF, 0xFF,   0x2C,
      0,    0,    0,    0,    width, 0,    height, 0,    0x00,   2};
  int gifindex = 30;
  /* 世代を進める*/
  for (int gen = 2; gen < 4; gen++) {
    for (int i = 0; i < height; i++) {
      for (int j = 0; j < width; j++) {
        if (i % gen == j % gen) {
          cell[i][j] = 1;
        } else {
          cell[i][j] = 0;
        }
      }
    }
    makegif(width, height, cell, file, gifdata, &gifindex);
  }
  gifdata[gifindex] = 0x3b;
  for (int i = 0; i < 1000; i++)
  {
      printf("%d",gifdata[i]);
  }
  
 fwrite(gifdata,sizeof(unsigned char),sizeof(unsigned char)*(gifindex+2),file);
  fclose(file);

  return EXIT_SUCCESS;
}

void makegif(const int width, const int height, const int a[][width], FILE *fp,
             unsigned char gifdata[], int *gifdataindex) {
  Node *n = malloc(sizeof(Node) * 300);
  n[0] = node_init("0");
  n[1] = node_init("1");
  n[2] = node_init("2");
  n[3] = node_init("3");
  n[4] = node_init("4");
  n[5] = node_init("5");
  size_t size = 6;
  char code[CODE_SIZE];
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      code[i * width + j] = '0' + a[i][j];
    }
  }
  char bitsize[CODE_SIZE + 2];
  int bitlength = 3;
  bitsize[0] = bitlength;
  char ans[CODE_SIZE + 2];
  size_t anssize = 0;
  ans[0] = 4;
  for (int i = 0; i < strlen(code); i++) {
    char s[1000000];
    for (int j = 0; j < 1000000; j++) {
      s[j] = '\0';
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
      bitsize[anssize + 1] = bitsize[anssize + 2] = bitlength;
      ans[anssize + 2] = 5;
      anssize++;
      break;
    } else {
      i += strlen(s) - 2;
      node_append(n, &size, s);
      s[j - 1] = '\0';
      ans[anssize + 1] = node_find(n, size, s);
      bitsize[anssize + 1] = bitlength;
      if (size - 1 >= 1 << bitlength) {
        bitlength++;
      }
      anssize++;
    }
  }
  char buf[8];
  int bufindex = 7;
  int imageboard[256];
  int imageindex = 0;
  for (int i = 0; i < anssize + 2; i++) {
    char x = ans[i];
    char y[bitsize[i]];
    for (int j = 0; j < bitsize[i]; j++) {
      if (x & 1 << (bitsize[i] - 1 - j)) {
        y[j] = 1;
      } else {
        y[j] = 0;
      }
    }
    for (int j = 0; j < bitsize[i]; j++) {
      if (bufindex < 0) {  // bufが満杯
        for (int k = 0; k < 8; k++) {
        }
        imageboard[imageindex] = return0xnum(buf);
        ++imageindex;
        bufindex = 7;
      }
      buf[bufindex] = y[bitsize[i] - 1 - j];
      bufindex--;
    }
    if (i == anssize + 1) {
      for (int j = 0; j <= bufindex; j++) {
        buf[j] = 0;
      }
      imageboard[imageindex] = return0xnum(buf);
      imageindex++;
    }
  }
  unsigned char num = imageindex;
  for (int i = 0; i < imageindex; i++)
  {
      printf("%d,",imageboard[i]);
  }
  for (int i = 0; i < imageindex; i++)
  {
      printf("%d,",bitsize[i]);
  }
  
  gifdata[*gifdataindex] = num;
  (*gifdataindex)++;
  for (int i = 0; i < imageindex + 1; i++) {
    gifdata[*(gifdataindex)] = imageboard[i];
    (*gifdataindex)++;
  }
  gifdata[*(gifdataindex)] = 0;
  (*gifdataindex)++;
  free(n);
}