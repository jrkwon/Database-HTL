/*
 *      PCXVIEW.C
 *      Ób¤‚ Ï¡³¡µB¯a Ìa·© ¥¡‹¡
 */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <dos.h>
#include <time.h>
#include <bios.h>
#include "ascii.h"
#include "hanlib.h"
#include "handetect.h"

typedef struct {
  char manuf;     /* Ðw¬w 10 (0x0a) */
  char hard;      /* ¤á¹e ¸÷¥¡ ?    */
  char encod;     /* RLE ·¡¡e 1     */
  char bitpx;     /* Ðe¸ñ·i Îa¯¡Ða“e §¡Ëa ® */
  int  x1;        /* ‹aŸ±· ¹ÁÎa    */
  int  y1;
  int  x2;
  int  y2;
  int  hres;      /* Ð¬w•¡         */
  int  vres;
  char clrma[48]; /* ÌiAËa ¸÷¥¡   */
  char vmode;     /* ????????????? */
  char nplanes;   /* ¬a¶w–E ÏiA·¥® */
  int  bplin;     /* Ðeœa·¥”w ¤a·¡Ëa ® */
  int  palinfo;   /* 1=COLOR, 0=GREY */
  int  shres;     /* ¯aÄá Ð¬w•¡?*/
  int  svres;
  char xtra[54];  /* ¬a¶w ´g·q */
} PCXHEADER;    /* PCX Ñw¯¢ ÑÁ·©· ÑA”á */

unsigned char far *addr[480];
int offx, offy;  /* x .. byte,   y ... dot  */

void set_mode(int );
void set_plane(int no);

/* PCX  áŸ¡Ÿi  e—e”a */
void set_pcxheader(PCXHEADER *hdr)
{
  hdr->manuf = 10;
  hdr->hard = 2;
  hdr->encod = 1;
  hdr->bitpx = 1;
  hdr->x1 = hdr->y1 = 0;
  hdr->x2 = 14*8 -1;     /*  <----  */
  hdr->y2 = 7*16 -1;     /*  <----  */
  hdr->hres = 14*8;
  hdr->vres = 7*16;
  hdr->vmode = 0;
  hdr->nplanes = 1;
  hdr->bplin = (14*8 + 7)/8;
}

/* ÏiA·¥·i ¤aŽ‘ */
void set_plane(int no)
{
  static int table[4] = {1, 2, 4, 8};

  outp(0x3ce, 2);
  outp(0x3cf, table[no]);
}

/* §¡—¡µ¡ œ‘µA Ðe ¤a·¡ËaŸi  °á ý·q
 * x - byte”e¶á,  y - dot”e¶á          */
void putbyte(unsigned xb, unsigned yd, char data)
{
  char mask[8] = { 0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01 };
  int i;
  
  for(i = 0; i < 8; i++)
    hputpixel(xb*8+i, yd, char & mask[i]);
}

/* §¡—¡µ¡ œ‘µA Ðe ¤a·¡ËaŸi (XOR¡) °á ý·q
 * x - byte”e¶á,  y - dot”e¶á               */
void xor_putbyte(unsigned xb, unsigned yd, char data)
{
  unsigned char far *ptr = addr[yd]+xb;

  *ptr ^= data;
}

void pcx_view(FILE *fp)
{
  register unsigned xx, yy, plane, i;
  int ch, rc, data, flag=0, xb, xs, driver, maxx, maxy;
  PCXHEADER header;

  fread((void *)&header, 128, 1, fp);
  driver = _detectgraph();       /* ‹aœÏ¢ Äa—aŸi ÑÂ·¥Ðe”a. */

  xs = xx = (header.x1)/8;  /* ¯¡¸b¸ñ· X ¹ÁÎaŸi aÈa“e º­¡Ÿi ‰¬e */
  yy = header.y1 + 18;      /* ¯¡¸b¸ñ· Y ¹ÁÎaŸi aÈa“e º­¡Ÿi ‰¬e */
  plane = 0;                /* ÏiA·¥·i aÈa“e ¥e®Ÿi Á¡‹¡ÑÁ */
  xb = header.bplin;

  maxx = 640, maxy = 400;
  while((ch = getc(fp)) != EOF && flag == 0){
    if((ch & 0xc0) == 0xc0){            /* Äa¶…Ëa·¥ˆa ? */
      rc = ch & 0x3f;       /* Äa¶…Ëa ‰¬e( ÂA¬w¶á 2§¡Ëa ¹Aˆá) */
      data = getc(fp);      /* ”a·q·¡ •A·¡Èa */
    }
    else {                             /* ´a“¡¡e */
      rc = 1;               /* Äa¶…Ëa = 1 */
      data = ch;            /* ·ª·e ˆt·¡ •A·¡Èa */
    }

    while( rc-- > 0){     /* Äa¶…Ëa eÇq ¤e¥¢ */
      if((xx - xs + 1) >= xb){
      /* Àá·q¦Èá Ðeœa·¥”w· ¤a·¡Ëa® eÇq Îa¯¡Ð–a? */
        if(++plane == header.nplanes){
        /* ‹aŸ±µA¬á ¬a¶w–E ÏiA·¥µA ¡¡– Îa¯¡Ð–a?*/
          plane = 0; /* ÏiA·¥·i ”a¯¡ •©Ÿ± */
          if(++yy > header.y2){
           /*‹aŸ±· ÂA” Yˆa ñ·a¡e {´¡–A»¡*/
             flag = 1;
             break;
           }
         }
         xx -= xb; /* X ¹ÁÎa· º­¡Ÿi ”a¯¡ Àá·q·a¡ */
         if( driver != HERCMONO )set_plane(plane);
         /* ÀåÐa¸w¬aˆa ´a“¡¡e ÏiA·¥·i ¤aŽ¡¬á ”a·q •A·¡Èá ¦Èá ‰­¢ Îa¯¡ */
      }
      if(xx < (maxx)/8 && yy < (maxy-2))   /* ÑÁ¡e·i ¤õ´áa»¡ ´g·a¡e */
        putbyte(xx++, yy, data);   /* •A·¡Èa Îa¯¡ */
      else
        xx++;                  /* ´a“¡¡e ¹ÁÎa e »wˆa */
      /* Àq‰¡:X ¹ÁÎa »wˆa ”e¶á : ¤a·¡Ëa
         Y ¹ÁÎa »wˆa ”e¶á : Ï¢­I   */
    }
  }
}

void view_file(char *file_name)
{
  FILE *fp;

  fp = fopen(file_name, "rb");
  if(!fp)
    return;
  pcx_view(fp);
  fclose(fp);
}

