/*
 *      PCXVIEW.C
 *      �b�� ϡ���B�a �a�� ����
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
  char manuf;     /* �w�w 10 (0x0a) */
  char hard;      /* ��e ���� ?    */
  char encod;     /* RLE ���e 1     */
  char bitpx;     /* �e��i �a���a�e ���a �� */
  int  x1;        /* �a���� ���a    */
  int  y1;
  int  x2;
  int  y2;
  int  hres;      /* Ё�w��         */
  int  vres;
  char clrma[48]; /* �i�A�a ����   */
  char vmode;     /* ????????????? */
  char nplanes;   /* �a�w�E �i�A���� */
  int  bplin;     /* �e�a���w �a���a �� */
  int  palinfo;   /* 1=COLOR, 0=GREY */
  int  shres;     /* �aā�� Ё�w��?*/
  int  svres;
  char xtra[54];  /* �a�w �g�q */
} PCXHEADER;    /* PCX �w�� ������ �A�� */

unsigned char far *addr[480];
int offx, offy;  /* x .. byte,   y ... dot  */

void set_mode(int );
void set_plane(int no);

/* PCX �១�i �e�e�a */
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

/* �i�A���i �a�� */
void set_plane(int no)
{
  static int table[4] = {1, 2, 4, 8};

  outp(0x3ce, 2);
  outp(0x3cf, table[no]);
}

/* ������ ���A �e �a���a�i  �� ���q
 * x - byte�e��,  y - dot�e��          */
void putbyte(unsigned xb, unsigned yd, char data)
{
  char mask[8] = { 0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01 };
  int i;
  
  for(i = 0; i < 8; i++)
    hputpixel(xb*8+i, yd, char & mask[i]);
}

/* ������ ���A �e �a���a�i (XOR��) �� ���q
 * x - byte�e��,  y - dot�e��               */
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
  driver = _detectgraph();       /* �a��Ϣ �a�a�i �·��e�a. */

  xs = xx = (header.x1)/8;  /* ���b�� X ���a�i �a�a���e �����i ���e */
  yy = header.y1 + 18;      /* ���b�� Y ���a�i �a�a���e �����i ���e */
  plane = 0;                /* �i�A���i �a�a���e �e���i ������ */
  xb = header.bplin;

  maxx = 640, maxy = 400;
  while((ch = getc(fp)) != EOF && flag == 0){
    if((ch & 0xc0) == 0xc0){            /* �a���a���a ? */
      rc = ch & 0x3f;       /* �a���a ���e( �A�w�� 2���a �A��) */
      data = getc(fp);      /* �a�q�� �A���a */
    }
    else {                             /* �a���e */
      rc = 1;               /* �a���a = 1 */
      data = ch;            /* ���e �t�� �A���a */
    }

    while( rc-- > 0){     /* �a���a�e�q �e�� */
      if((xx - xs + 1) >= xb){
      /* ��q���� �e�a���w�� �a���a���e�q �a��Ж�a? */
        if(++plane == header.nplanes){
        /* �a���A�� �a�w�E �i�A���A ���� �a��Ж�a?*/
          plane = 0; /* �i�A���i �a�� ���� */
          if(++yy > header.y2){
           /*�a���� �A�� Y�a ��a�e �{�����A��*/
             flag = 1;
             break;
           }
         }
         xx -= xb; /* X ���a�� �����i �a�� ��q�a�� */
         if( driver != HERCMONO )set_plane(plane);
         /* ���a�w�a�a �a���e �i�A���i �a���� �a�q �A���� ���� ���� �a�� */
      }
      if(xx < (maxx)/8 && yy < (maxy-2))   /* ���e�i ����a�� �g�a�e */
        putbyte(xx++, yy, data);   /* �A���a �a�� */
      else
        xx++;                  /* �a���e ���a�e �w�a */
      /* �q��:X ���a �w�a �e�� : �a���a
         Y ���a �w�a �e�� : Ϣ�I   */
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

