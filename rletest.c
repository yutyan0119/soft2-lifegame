#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

int main() {
  int height = 40;
  int width = 70;
  int cell[height][width];
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      cell[i][j] = 0;
    }
  }

  FILE* fp;
  fp = fopen("thunderbird.rle", "r");
  char buf[300];
  int column = 0;
  int row = 0;
  while (fgets(buf, 300, fp)) {
    if (buf[0] == '#') {
      continue;
    }
    if (buf[0] == 'x') {
      int xsize;
      int ysize;
      char* tmpx = buf + 4;
      char* tmpy;
      tmpx = strtok(tmpx, ",");
      tmpy = strtok(NULL, ",");
      tmpy = tmpy + 5;
      xsize = atoi(tmpx);
      ysize = atoi(tmpy);
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
  }
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      printf("%d ",cell[i][j]);
    }
    printf("\n");
  }
}