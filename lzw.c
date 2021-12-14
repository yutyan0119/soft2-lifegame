#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define MAX_NODE 70000
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

int return0xnum(char buf[]) {
  int num = 0;
  for (int i = 0; i < 8; i++) {
    num += buf[i] * 1 << (7 - i);
  }
  return num;
}

int main(int argc, char* argv[]) {
  Node* n = malloc(sizeof(Node) * 300);
  n[0] = node_init("0");
  n[1] = node_init("1");
  n[2] = node_init("2");
  n[3] = node_init("3");
  n[4] = node_init("4");
  n[5] = node_init("5");
  size_t size = 6;
  char code[CODE_SIZE];
  int width = 70;
  int height = 40;
  FILE* fp1;
  fp1 = fopen("test.dat", "w");
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      if (height / 3 <= i && i < height * 2 / 3 && width / 3 <= j &&
          j < 2 * width / 3) {
        code[i * width + j] = '1';
      } else {
        code[i * width + j] = '0';
      }
      fprintf(fp1, "%c", code[i * width + j]);
    }
    fprintf(fp1, "\n");
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
    fprintf(fp1, "i=%d\n", i);
    for (int j = 0; j < height; j++) {
      for (int k = 0; k < width; k++) {
        fprintf(fp1, "%c", code[j * width + k]);
      }
      fprintf(fp1, "\n");
    }

    int j = 0;
    s[0] = code[i];
    while (node_find(n, size, s) != -1) {
      if (i + j == strlen(code)) {
        break;
      }
      s[j] = code[i + j];
      // printf("i+j = %d, %d\n", i + j, code[i + j]);
      // printf("%c",s[j]);
      j++;
    }
    // printf("\n");
    // printf("j = %d\n", j);
    if (i + j == strlen(code) && node_find(n, size, s) != -1) {
      ans[anssize + 1] = node_find(n, size, s);
      bitsize[anssize + 1] = bitsize[anssize + 2] = bitlength;
      ans[anssize + 2] = 5;
      anssize++;
      break;
    } else {
      i += strlen(s) - 2;
      // printf("i = %d,%s\n", i, s);
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

  for (int i = 0; i < anssize + 2; i++) {
    printf("%d,", ans[i]);
  }
  printf("\n");
  for (int i = 0; i < anssize + 2; i++) {
    printf("%d,", bitsize[i]);
  }
  printf("\n");
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
  FILE* fp;
  fp = fopen("giftest.dat", "w");
  for (int i = 0; i < imageindex; i++) {
    fprintf(fp, "%d,", imageboard[i]);
  }
  fprintf(fp, "\n%d", imageindex);
  fclose(fp);
  // for (int i = 0; i < size; i++) {
  //   printf("%d %s\n", i, n[i].c);
  // }
  fclose(fp1);
  free(n);
  char num = imageindex;
  unsigned char gifdata[70000] = {
      'G',  'I',  'F',   '8',  '9',    'a',  width, 0x00, height, 0x00, 0x80,
      0x00, 0x00, 0x41,  0x69, 0xE1,   0xFF, 0xFF,  0xFF, 0x2C,   0,    0,
      0,    0,    width, 0,    height, 0,    0x00,  2,    num};
  for (int i = 0; i < imageindex; i++) {
    gifdata[31 + i] = imageboard[i];
  }
  gifdata[31 + imageindex] = 0;
  gifdata[32 + imageindex] = 0x3B;
  fp = fopen("my1stgif.gif", "wb");
  fwrite(gifdata, sizeof(unsigned char), sizeof(gifdata) / sizeof(gifdata[0]),
         fp);
  fclose(fp);
  return EXIT_SUCCESS;
}