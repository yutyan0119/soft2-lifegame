#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>  // sleep()関数を使う
#define MAX_NODE 10000
#define CODE_SIZE 50000

int max(int a, int b) {
  if (a > b) {
    return a;
  } else {
    return b;
  }
}

int min(int a, int b) {
  if (a > b) {
    return b;
  } else {
    return a;
  }
}

int count_adjacent_cells(int h, int w, const int height, const int width,
                         int cell[height][width]) {
  // cellの(h,w)周辺の数をfor文でカウントする
  int count = 0;
  int miny = max(0, h - 1);
  int maxy = min(height - 1, h + 1);
  int minx = max(0, w - 1);
  int maxx = min(width - 1, w + 1);
  for (int i = miny; i <= maxy; i++) {
    for (int j = minx; j <= maxx; j++) {
      count += cell[i][j];
    }
  }
  count -= cell[h][w];
  return count;
};

void update_cells(const int height, const int width, int cell[height][width]) {
  int newcell[height][width];
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      int count = count_adjacent_cells(i, j, height, width, cell);
      if (cell[i][j]) {
        if (count == 2 || count == 3) {
          newcell[i][j] = 1;
        } else {
          newcell[i][j] = 0;
        }
      } else {
        if (count == 3) {
          newcell[i][j] = 1;
        } else {
          newcell[i][j] = 0;
        }
      }
    }
  }
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      cell[i][j] = newcell[i][j];
    }
  }
};

typedef struct node {
  char c[1000];
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
  // int cell[height][width];
  // for (int y = 0; y < height; y++) {
  //   for (int x = 0; x < width; x++) {
  //     cell[y][x] = 0;
  //   }
  // }
  FILE *testdata;
  int k = 0;
  testdata = fopen("testdata.txt", "r");
  int cell[height][width];
  char buf[70];
  while (fgets(buf, 80, testdata) != NULL) {
    // printf("%s\n",buf);
    for (int i = 0; i < width; i++) {
      cell[k][i] = buf[i] - '0';
    }
    k++;
    // printf("%d",k);
  }
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      // printf("%d",cell[i][j]);
    }
    // printf("\n");
  }

  FILE *file;
  char *filename;
  file = fopen("animetest.gif", "w");
  unsigned char gifdata[1000000] = {
      'G',  'I',  'F',  '8',  '9',  'a',  width * 3, 0x00, height * 3, 0x00,
      0x80, 0x00, 0x00, 0xb0, 0xc4, 0xde, 0xFF,      0xFF, 0xFF,       0x21,
      0xFF, 11,   'N',  'E',  'T',  'S',  'C',       'A',  'P',        'E',
      '2',  '.',  '0',  3,    1,    0,    0,         0,    0x21,       0xF9,
      4,    4,    10,   0,    0,    0};
  int gifindex = 46;
  /* 世代を進める*/
  for (int gen = 2; gen < 10; gen++) {
    makegif(width, height, cell, file, gifdata, &gifindex);
    update_cells(height, width, cell);
    printf("gen = %d\n", gen);
    printf("%d\n", gifindex);
  }
  gifdata[gifindex] = 0x3b;
  // for (int i = 0; i < 1000; i++) {
  //   printf("%d", gifdata[i]);
  // }

  fwrite(gifdata, sizeof(unsigned char), sizeof(unsigned char) * (gifindex + 2),
         file);
  fclose(file);

  return EXIT_SUCCESS;
}

void makegif(const int width, const int height, const int a[][width], FILE *fp,
             unsigned char gifdata[], int *gifdataindex) {
  Node *n = malloc(sizeof(Node) * 10000);
  n[0] = node_init("0");
  n[1] = node_init("1");
  n[2] = node_init("2");
  n[3] = node_init("3");
  n[4] = node_init("4");
  n[5] = node_init("5");
  size_t size = 6;
  unsigned char *code = malloc(sizeof(unsigned char) * 100000);
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      if (a[i][j] == 1) {
        for (int k = 0; k < 3; k++) {
          for (int l = 0; l < 3; l++) {
            code[i * width * 3 * 3 + k * width * 3 + j * 3 + l] = '1';
            // printf("1を%dに入れる\n", (i * 3 + k) * width * 3 + j * 3 + l);
          }
        }
      } else {
        for (int k = 0; k < 3; k++) {
          for (int l = 0; l < 3; l++) {
            code[i * width * 3 * 3 + k * width * 3 + j * 3 + l] = '0';
            // printf("0を%dに入れる\n",i*width*3*3+k*width*3+j*3+l);
          }
        }
      }
    }
  }
  // for (int i = 0; i < height * 3; i++) {
  //   for (int j = 0; j < width * 3; j++) {
  //     printf("%c", code[i * width * 3 + j]);
  //   }
  //   printf("\n");
  // }

  unsigned char bitsize[CODE_SIZE + 2];
  int bitlength = 3;
  bitsize[0] = bitlength;
  int ans[CODE_SIZE + 2];
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
      // printf("size = %ld, bitlength = %d ,%d =
      // %d,%s\n",size,bitlength,node_find(n,size,s),ans[anssize+1],s);
      bitsize[anssize + 1] = bitlength;
      if (size - 1 >= 1 << bitlength) {
        bitlength++;
      }
      anssize++;
    }
  }
  // for (int i = 0; i < anssize + 2; i++) {
  //   printf("%d, ", ans[i]);
  // }
  // printf("\n");
  // printf("maxbitlength = %d, anssize = %d\n", bitlength, anssize);
  char buf[8];
  int bufindex = 7;
  int imageboard[500];
  int imageindex = 0;
  for (int i = 0; i < anssize + 2; i++) {
    // printf("bitsize = %d\n",bitsize[i]);
    int x = ans[i];
    char y[bitsize[i]];
    // printf("%d = ",x);
    for (int j = 0; j < bitsize[i]; j++) {
      if (x & 1 << (bitsize[i] - 1 - j)) {
        y[j] = 1;
      } else {
        y[j] = 0;
      }
      // printf("%d", y[j]);
    }
    // printf("\n");
    for (int j = 0; j < bitsize[i]; j++) {
      if (bufindex < 0) {  // bufが満杯
        // printf("bitset");
        // for (int k = 0; k < 8; k++) {
        //   printf("%d", buf[k]);
        // }
        // printf("\n");
        imageboard[imageindex] = return0xnum(buf);
        ++imageindex;
        bufindex = 7;
      }
      buf[bufindex] = y[bitsize[i] - 1 - j];
      // printf("%d",buf[bufindex]);
      bufindex--;
    }
    if (i == anssize + 1) {
      for (int j = 0; j <= bufindex; j++) {
        buf[j] = 0;
      }
      // printf("lastbitset");
      // for (int j = 0; j < 8; j++) {
      //   printf("%d", buf[j]);
      // }
      // printf("\n");
      imageboard[imageindex] = return0xnum(buf);
      imageindex++;
    }
  }
  int num = imageindex;
  // printf("num = %d\n", num);
  // printf("imageに入れる配列長%d\n", num + 12);
  // for (int i = 0; i < imageindex; i++) {
  //   printf("%d,", imageboard[i]);
  // }
  // printf("\n");
  // for (int i = 0; i < imageindex; i++) {
  //   printf("%d,", bitsize[i]);
  // }
  // printf("\n");
  char need[] = {0x2c, 0x00,       0x00, 0x00, 0x00, width * 3,
                 0,    height * 3, 0,    0,    2};
  for (int i = 0; i < 11; i++) {
    gifdata[*gifdataindex] = need[i];
    (*gifdataindex)++;
  }
  int imageboardindex = 0;
  printf("gifdataindex = %d,num = %d\n", *gifdataindex, num);
  while (num >= 256) {
    gifdata[*gifdataindex] = 255;
    (*gifdataindex)++;
    for (int j = 0; j < 255; j++) {
      gifdata[*(gifdataindex)] = imageboard[imageboardindex];
      // printf("j = %d, gifdataindex = %d, imageboardindex =
      // %d\n",j,*gifdataindex,imageboardindex);
      *(gifdataindex) += 1;
      imageboardindex++;
    }
    num -= 255;
  }
  gifdata[*gifdataindex] = num;
  // printf("kokomadekita!\n");
  *(gifdataindex) += 1;
  for (int j = 0; j < num; j++) {
    gifdata[*(gifdataindex)] = imageboard[imageboardindex];
    *(gifdataindex) += 1;
    imageboardindex++;
  }
  gifdata[*(gifdataindex)] = 0;
  (*gifdataindex)++;
  free(n);
  free(code);
}