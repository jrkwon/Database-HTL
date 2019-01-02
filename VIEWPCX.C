#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <alloc.h>
#include <bios.h>
#include "hanlib.h"

typedef struct {
  char manuf;     /* Ðw¬w 10 (0x0a) */
  char ver;       /*                */
  char encod;     /* RLE ·¡¡e 1     */
  char bitpx;     /* Ðe¸ñ·i Îa¯¡Ða“e §¡Ëa ® */
  int  x1;        /* ‹aŸ±· ¹ÁÎa    */
  int  y1;
  int  x2;
  int  y2;
  int  hres;      /* Ð¬w•¡         */
  int  vres;
  char palette[48];
  char reserved;     /* ‹aŸ±·i ¤h·e §¡—¡µ¡ ¡¡—a ? */
  char nplanes;   /* ¬a¶w–E ÏiA·¥® */
  int  bplin;     /* Ðeœa·¥”w ¤a·¡Ëa ® */
  int  palinfo;   /* 1=COLOR, 0=GREY */
  int  shres;     /* ¯aÄá Ð¬w•¡?*/
  int  svres;
  char xtra[54];  /* ¬a¶w ´g·q */
} PCXHDR;    /* PCX Ñw¯¢ ÑÁ·©· ÑA”á */

static void writepcxline(int x, int y, char *s, int n, int fcolor, int bcolor)
{
  char mask[8] = { 0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01 };
  int i, j;

  for(i = 0; i < n-2; i++, s++) {
    if(x+i >= hgetmaxx())
      return;
    for(j = 0; j < 8; j++)
      hputpixel((x+i)*8+j, y, (*s & mask[j]) ? fcolor : bcolor);
  }
}

static int readpcxline(char *s, FILE *fpt, int bytes)
{
	int c,i;
	int n=0;

	do {
		c=(fgetc(fpt) & 0xff);
		if ((c&0xc0)==0xc0) {
			i=c&0x3f;
			c=fgetc(fpt);
      while (i--) s[n++]=c;
		}
		else s[n++]=c;
	} while (n<bytes);
	return n;
}

static bool pcx_view(int x, int y, FILE *fp, int fcolor, int bcolor)
{
  PCXHDR hdr;
  unsigned bytes, height;
  int i, n;
  char *bits;

  fread((void *)&hdr, 128, 1, fp);
  if(hdr.manuf != 0x0a /* &&    ..  */)
    return false;
  x /= 8;
  bytes = hdr.bplin > hgetmaxpx() ? hgetmaxpx() : hdr.bplin;
  height = hdr.y2-hdr.y1+1 > hgetmaxpy() ? hgetmaxpy() : hdr.y2-hdr.y1+1;

  bits = (char *)malloc((size_t)bytes);
  if(!bits)
    return false;
  for(i = y; i < y+height; i++) {
    n = readpcxline(bits, fp, bytes);
    writepcxline(x, i, bits, n, fcolor, bcolor);
  }
  free(bits);
  return true;
}

bool view_pcx_file(int x, int y, char *file_name, int fcolor, int bcolor)
{
  FILE *fp;

  fp = fopen(file_name, "rb");
  if(!fp)
    return false;
  pcx_view(x, y, fp, fcolor, bcolor);
  fclose(fp);
  return true;
}

void main(int argc, char* argv[])
{
  inithanlib(FIXRESMODE, HANDETECT, DEFHANFONT, DEFENGFONT);

  if(argc == 1)
    return;
  view_pcx_file(0, 0, argv[1], WHITE, BLACK);
  bioskey(0);
}

