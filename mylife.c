#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>  // sleep()関数を使う

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

  print_cells(fp, 0, height, width, cell);  // 表示する
  sleep(1);                                 // 1秒休止
  fprintf(fp, "\e[%dA", height + 4);

  /* 世代を進める*/
  for (int gen = 1;; gen++) {
    update_cells(height, width, cell);          // セルを更新
    print_cells(fp, gen, height, width, cell);  // 表示する
    sleep(1);                                   // 1秒休止する
    fprintf(fp, "\e[%dA", height + 4);
    if (gen % 100 == 0 && gen < 10000) {
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
  }

  return EXIT_SUCCESS;
}

void init_cells(const int height, const int width, int cell[height][width],
                FILE *fp) {
  //乱数を生成してそれにしたがってRIF6ファイルを出力してこれに入れる。
  if (fp == NULL) {
    FILE *pk;
    pk = fopen("random.lif", "w");
    fprintf(pk, "#Life 1.06");
    for (int i = 0; i < 280; i++) {
      fprintf(pk, "\n");
      int x = getrandom(0, 69);
      int y = getrandom(0, 39);
      fprintf(pk, "%d %d", x, y);
    }
    fclose(pk);  //ここまででファイル生成終了

    FILE *fp2;
    fp2 = fopen("random.lif", "r");  //生成したファイル利用
    char buf[100];
    fgets(buf, 100, fp2);  // 1行飛ばす
    while (fgets(buf, 100, fp2)) {
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
    char buf[100];
    fgets(buf, 100, fp);  // 1行目は切り捨てる
    while (fgets(buf, 100, fp)) {
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