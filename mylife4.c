#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>  // sleep()関数を使う
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

void init_cells(const int height, const int width, int cell[height][width],
                FILE *fp);

void print_cells(FILE *fp, int gen, const int height, const int width,
                 int cell[height][width]);

int count_adjacent_cells(int h, int w, const int height, const int width,
                         int cell[height][width]);

void update_cells(const int height, const int width, int cell[height][width]);

int getrandom(int min, int max);

int max(int a, int b);

int min(int a, int b);

void output_board(int height, int width, int cell[height][width], int gen);

typedef struct rule {
} Rule;

int main(int argc, char **argv) {
  srand(time(NULL));
  FILE *fp = stdout;
  const int height = 40;
  const int width = 70;

  int cell[height][width];
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      cell[y][x] = 0;
    }
  }

  /* ファイルを引数にとるか、ない場合はデフォルトの初期値を使う */
  if (argc > 2) {
    fprintf(stderr, "usage: %s [filename for init]\n", argv[0]);
    return EXIT_FAILURE;
  } else if (argc == 2) {
    FILE *lgfile;
    if ((lgfile = fopen(argv[1], "r")) != NULL) {
      init_cells(height, width, cell, lgfile);  // ファイルによる初期化
    } else {
      fprintf(stderr, "cannot open file %s\n", argv[1]);
      return EXIT_FAILURE;
    }
    fclose(lgfile);
  } else {
    init_cells(height, width, cell, NULL);  // デフォルトの初期値を使う
  }

  // print_cells(fp, 0, height, width, cell);  // 表示する
  // sleep(1);                                 // 1秒休止
  // fprintf(fp, "\e[%dA", height + 4);

  FILE *file;
  char *filename;
  file = fopen("lifegame.gif", "w");
  unsigned char gifdata[100000] = {
      'G',  'I',  'F',  '8',  '9',  'a',  width, 0x00, height, 0x00, 0x80, 0x00,
      0x00, 0xb0, 0xc4, 0xde, 0xFF, 0xFF, 0xFF,  0x21, 0xFF,   11,   'N',  'E',
      'T',  'S',  'C',  'A',  'P',  'E',  '2',   '.',  '0',    3,    1,    0,
      0,    0,    0x21, 0xF9, 4,    4,    10,    0,    0,      0};


  int gifindex = 46;
  /* 世代を進める*/
  for (int gen = 1; gen < 100; gen++) {
    update_cells(height, width, cell);          // セルを更新
    // print_cells(fp, gen, height, width, cell);  // 表示する
    // sleep(1);                                   // 1秒休止する
    // fprintf(fp, "\e[%dA", height + 4);
    // sprintf(filename, "gen%04d.gif", gen);
    // file = fopen(filename, "wb");
    makegif(width, height, cell, file, gifdata, &gifindex);
    printf("gen = %d\n",gen);
  }
  gifdata[gifindex] = 0x3b;
  fwrite(gifdata, sizeof(unsigned char), sizeof(unsigned char) * (gifindex + 2),
         file);
  fclose(file);
  return EXIT_SUCCESS;
}

void init_cells(const int height, const int width, int cell[height][width],
                FILE *fp) {
  //乱数を生成してそれにしたがってRIF6ファイルを出力してこれに入れる。
  size_t buffsize = 300;
  int rleflag = 0;
  if (fp == NULL) {
    FILE *pk;
    pk = fopen("random.lif", "w");
    fprintf(pk, "#Life 1.06");
    for (int i = 0; i < 300; i++) {
      fprintf(pk, "\n");
      int x = getrandom(0, 69);
      int y = getrandom(0, 39);
      fprintf(pk, "%d %d", x, y);
    }
    fclose(pk);  //ここまででファイル生成終了

    FILE *fp2;
    fp2 = fopen("random.lif", "r");  //生成したファイル利用
    char buf[buffsize];
    fgets(buf, buffsize, fp2);  // 1行飛ばす
    while (fgets(buf, buffsize, fp2)) {
      char *y;
      char *x;
      x = strtok(buf, " ");
      y = strtok(NULL, " ");
      for (int i = 0; i < strlen(y); i++) {
        if (y[i] == '\n') {
          y[i] = '\0';
        }
      }
      int X = atoi(x);
      int Y = atoi(y);
      cell[Y][X] = 1;
    }
    fclose(pk);
  } else {
    char buf[buffsize];
    int column = 0;
    int row = 0;
    while (fgets(buf, buffsize, fp)) {
      if (buf[strlen(buf) - 1] == '\n') {
        buf[strlen(buf) - 1] = '\0';
      }
      if (buf[0] == '#') {
        continue;
      } else if (buf[0] == 'x') {
        rleflag = 1;
        int xsize;
        int ysize;
        char *tmpx = buf + 4;
        char *tmpy;
        tmpx = strtok(tmpx, ",");
        tmpy = strtok(NULL, ",");
        tmpy = tmpy + 5;
        xsize = atoi(tmpx);
        ysize = atoi(tmpy);
        if (xsize >= 70 || ysize >= 40) {
          printf("This BoardSize is too large to input.Plz check your file!\n");
          break;
        }
        continue;
      } else if (rleflag) {
        if (buf[0] == '#') {
          continue;
        }
        if (buf[0] == 'x') {
          int xsize;
          int ysize;
          char *tmpx = buf + 4;
          char *tmpy;
          tmpx = strtok(tmpx, ",");
          tmpy = strtok(NULL, ",");
          tmpy = tmpy + 5;
          xsize = atoi(tmpx);
          ysize = atoi(tmpy);
          if (xsize >= 70 || ysize >= 40) {
            printf(
                "This BoardSize is too large to input.Plz check your file!\n");
            break;
          }
          char *rule;
          rule = strtok(NULL, "= ");
          if (rule == NULL) {
            continue;
          }
          printf("%s\n", rule);
          continue;
        }
        int howmany = 1;
        for (int i = 0; i < strlen(buf); i++) {
          if (buf[i] == '$') {
            column += howmany;
            howmany = 1;
            row = 0;
          } else if (buf[i] == 'o') {
            for (int j = 0; j < howmany; j++) {
              cell[column][row + j] = 1;
            }
            row += howmany;
            howmany = 1;
          } else if (buf[i] == 'b') {
            for (int j = 0; j < howmany; j++) {
              cell[column][row + j] = 0;
            }
            row += howmany;
            howmany = 1;
          } else if (buf[i] == '!') {
            break;
          } else {                        //数字のとき
            int index = strlen(buf) - 2;  //ないと思うけど最後改行対策
            for (int j = i; j < strlen(buf); j++) {
              if (buf[j] == 'b' || buf[j] == 'o' || buf[j] == '!' ||
                  buf[j] == '$') {
                index = j - 1;
                break;
              }
            }
            char num[5];
            for (int j = i; j <= index; j++) {
              num[j - i] = buf[j];
            }
            howmany = atoi(num);
          }
        }
      } else {
        char *y;
        char *x;
        x = strtok(buf, " ");
        y = strtok(NULL, " ");
        for (int i = 0; i < strlen(y); i++) {
          if (y[i] == '\n') {
            y[i] = '\0';
          }
        }
        int X = atoi(x);
        int Y = atoi(y);
        cell[Y][X] = 1;
      }
    }
  }
}

void print_cells(FILE *fp, int gen, const int height, const int width,
                 int cell[height][width]) {
  // cellのデータを使って、fp(stdout)にfprintfで出力
  int count = 0;
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      count += cell[i][j];
    }
  }
  fprintf(fp, "generation = %d\n", gen);
  fprintf(fp, "life rate = %3f %%\n", (double)count / 28);
  fprintf(fp, "+");
  for (int x = 0; x < width; x++) fprintf(fp, "-");
  fprintf(fp, "+\n");
  /* 外壁と 内側のゲーム部分 */
  for (int y = 0; y < height; y++) {
    fprintf(fp, "|");
    for (int x = 0; x < width; x++) {
      if (cell[y][x]) {
        fprintf(fp, "\e[31m#\e[0m");
      } else {
        fprintf(fp, " ");
      }
    }
    fprintf(fp, "|\n");
  }
  // 下の壁
  fprintf(fp, "+");
  for (int x = 0; x < width; x++) fprintf(fp, "-");
  fprintf(fp, "+\n");
  fflush(fp);
};

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

int getrandom(int min, int max) {
  double R = (double)rand() / RAND_MAX;
  return (int)(min + R * (max - min));
}

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

void output_board(int height, int width, int cell[height][width], int gen) {
  FILE *file;
  char *filename;
  sprintf(filename, "gen%04d.lif", gen);
  printf("%s", filename);
  file = fopen(filename, "w");
  fprintf(file, "#Life 1.06\n");
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      if (cell[i][j]) {
        fprintf(file, "%d %d\n", j, i);
      }
    }
  }
  fclose(file);
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
  // for (int i = 0; i < imageindex; i++) {
  //   printf("%d,", imageboard[i]);
  // }
  // for (int i = 0; i < imageindex; i++) {
  //   printf("%d,", bitsize[i]);
  // }
  char need[] = {0x2c, 0x00, 0x00, 0x00, 0x00, width, 0, height, 0, 0, 2};
  for (int i = 0; i < 11; i++) {
    gifdata[*gifdataindex] = need[i];
    (*gifdataindex)++;
  }

  gifdata[*gifdataindex] = num;
  (*gifdataindex)++;
  for (int i = 0; i < imageindex; i++) {
    gifdata[*(gifdataindex)] = imageboard[i];
    (*gifdataindex)++;
  }
  gifdata[*(gifdataindex)] = 0;
  (*gifdataindex)++;
  free(n);
}