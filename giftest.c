#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main() {
  char width = 70, height = 40;
  char num = 85;
  unsigned char gifdata[70000] = {
      'G',  'I',    'F',  '8',  '9',  'a',  width, 0x00, height, 0x00, 0x80,
      0x00, 0x00,   0xFF, 0x00, 0x00, 0xFF, 0xFF,  0xFF, 0x21,   0xFF, 0x11,
      'N',  'E',    'T',  'S',  'C',  'A',  'P',   'E',  '2',    '.',  '0',
      3,    1,      0,    0,    0,    0x21, 0xF9,  4,    4,      10,   0,
      0,    0, 0x2C, 0,    0,    0,    0,    width, 0,    height, 0,    0x00,
      2,    num,    132,  143,  169,  203,  237,   15,   163,    156,  180,
      218,  139,    179,  222,  188,  251,  15,    134,  226,    72,   150,
      230,  137,    166,  234,  202,  182,  174,   24,   196,    242,  76,
      215,  241,    139,  216,  186,  142,  31,    251,  63,     235,  25,
      128,  68,     33,   128,  8,    52,   34,    127,  202,    37,   79,
      232,  124,    246,  162,  182,  38,   53,    8,    189,    98,   167,
      90,   153,    181,  107,  12,   139,  199,   228,  178,    249,  140,
      78,   171,    215,  236,  182,  251,  13,    143,  39,     10,   0,
      0x3B};
  FILE* fp;
  fp = fopen("my1stgif.gif", "wb");
  fwrite(gifdata, sizeof(unsigned char), 143, fp);
  fclose(fp);
  return 0;
}