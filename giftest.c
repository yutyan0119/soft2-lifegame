#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main() {
  char width = 3, height = 3;
  char num = 4;
  unsigned char gifdata[70000] = {
      'G',  'I',  'F',   '8',  '9',    'a',  width, 0x00, height, 0x00, 0x91,
      0x00, 0x00, 0xb0,  0xc4, 0xde,   0xFF, 0xFF,  0xFF, 0xff,   0xc0, 0xcb,
      0xff, 0xff, 0xff,  0x21, 0xFF,   11,   'N',   'E',  'T',    'S',  'C',
      'A',  'P',  'E',   '2',  '.',    '0',  3,     1,    0,      0,    0,
      0x21, 0xF9, 4,     4,    10,     0,    0,     0,    0x2C,   0,    0,
      0,    0,    width, 0,    height, 0,    0x00,  2,    4,      0x84, 0x03,
      0x81, 0x51, 0x00,  0x2C, 0,      0,    0,     0,    width,  0,    height,
      0,    0x00, 2,     4,    0x44,   0x74, 0x09,  0x03, 0,      0x3B};
  FILE* fp;
  fp = fopen("my2ndgif.gif", "wb");
  fwrite(gifdata, sizeof(unsigned char), sizeof(gifdata) / sizeof(gifdata[0]),
         fp);
  fclose(fp);
  return 0;
}
