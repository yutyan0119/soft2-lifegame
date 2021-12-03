#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

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

int getrandom(int min, int max);

void print_cells(FILE* fp, int gen, const int height, const int width, int cell[height][width]) {
  // cellのデータを使って、fp(stdout)にfprintfで出力
  int count = 0;
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      count += cell[i][j];
    }
  }
  fprintf(fp, "generation = %d\n", gen);
  fprintf(fp, "life rate = %f %%\n", (double)count / 28);
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

int count_adjacent_cells(int h, int w, const int height, const int width,int cell[height][width]) {
  // cellの[h,w]周辺の数をfor文でカウントする
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
      if (cell[i][j] == 1) {
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

int main(int argc, char* argv[]) {
  int height = 40;
  int width = 70;
  int cell[height][width];
  for (int i = 0; i < 40; i++) {
    for (int j = 0; j < 70; j++) {
      cell[i][j] = 0;
    }
  }
  FILE* fp;
  fp = fopen("test.lif", "w");
  fprintf(fp, "#Life 1.06");
  srand(time(NULL));
  for (int i = 0; i < 290; i++) {
    fprintf(fp, "\n");
    int x = getrandom(0, 69);
    int y = getrandom(0, 39);
    fprintf(fp, "%d %d", x, y);
  }
  fclose(fp);
  FILE* fp3;
  fp3 = fopen("test.lif", "r");
  char buf[100];
  fgets(buf, 100, fp3);
  printf("%s", buf);
  while (fgets(buf, 100, fp3)) {
    char* y;
    char* x;
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
  fclose(fp3);
  FILE* fp2 = stdout;
  for (int gen = 0;; gen++) {
    print_cells(fp2, gen, height, width, cell);  // 表示する
    update_cells(height, width, cell);          // セルを更新
    fprintf(fp2, "\e[%dA",height + 4); 
    sleep(1);                                   // 1秒休止する
  }
  return 0;
}

int getrandom(int min, int max) {
  double R = (double)rand() / RAND_MAX;
  return (int)(min + R * (max - min));
}