/*
 *      File SL.C         Screen Locker
 *
 *      '92. 6. 5. �q
 *
 *      ���e ��ѡ�� �a�e �a�q�� �q���� �a���� ���e���i �a�w�a�e ��
 *      �b�����e ���e �a���A
 *
 *      �e�e�� : �� �� �b
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dos.h>
#include <bios.h>
#include <alloc.h>

/*
 *   hgettext�� hputtext�q���e TEXTEMUL�� �e�� ���� �A�� ���a�e ���a�A��
 *   �g�q�A ����
 */

#include "ascii.h"
#include "extkey.h"
#include "hanlib.h"
#include "hanin.h"

#define MAXSTARS    60
#define MAXPASS     43

/* ��ЗЁ�� �i �b�� ���A */

enum {
  BLINK = -1, /*  �i�� �e�b��       */
  HELPME,     /*  �����i       F1   */
  PASSWORD,   /*  �qѡ ���b         */
  CHANGEPASS, /*  �qѡ �a����  F2   */
  QUIT        /*  �a���a�� �i     */
};

typedef struct {
  int x, y;       /* �i�� �e�b�� x, y���a */
  int color;      /* �e�b�� �i�� ���i     */
} STAR;

STAR stars[MAXSTARS];

int task = BLINK;      /* �b�� �e�� */
int maxwidth, maxheight, maxcolors;
byte passwd[MAXPASS+1];
int pos = 0;           /* ���w �A�A�� �a�a�a ��w�A�� ���e ��á */
byte far *_scrbuf;

struct {
  int x1, y1, x2, y2;
} window;

/*
 *   �a�� �e�e�e �w�� �q���i
 */

int openwindow(int x1, int y1, int x2, int y2)
{
  unsigned long size;

  size = htextsize(x1, y1, x2, y2);
  _scrbuf = farmalloc(size);
  if(_scrbuf == NULL) return 0;
  hgettext(x1, y1, x2, y2, _scrbuf);
  hwindow(x1, y1, x2, y2);
  hclrscr();
  window.x1 = x1, window.y1 = y1, window.x2 = x2, window.y2 = y2;
  return 1;
}

void closewindow(void)
{
  int x1, y1, x2, y2;

  farfree(_scrbuf);
  hwindow(1, 1, -80, -25);
  x1 = window.x1, y1 = window.y1, x2 = window.x2, y2 = window.y2;
  hputtext(x1, y1, x2, y2, _scrbuf);
}

/*
 *    �a���a���� ���a ����
 */

void encoder(void)
{
  int i = 0;

  for(i = 0; passwd[i]; i++)
    passwd[i] = passwd[i] + i - 25;
}

void decoder(void)
{
  int i = 0;

  for(i = 0; i < passwd[i]; i++)
    passwd[i] = passwd[i] - i + 25;
}

void displayhelp(void)
{
  int isquit = 0, cnt = 0;

  if(!openwindow(18, 8, -48, -11)) return;
  hsetcolor(HLIGHTGREEN);
  hputsxy(1,  1, "����������������������������������������������Կ");
  hputsxy(1,  2, "Գ��Գ      �i�A �� �a���A  �� �� �i          Գ");
  hputsxy(1,  3, "����������������������������������������������Դ");
  hputsxy(1,  4, "Գ                                            Գ");
  hputsxy(1,  5, "Գ          F1 - ���q �� �����i               Գ");
  hputsxy(1,  6, "Գ                                            Գ");
  hputsxy(1,  7, "Գ          F2 - �qѡ �a����                  Գ");
  hputsxy(1,  8, "Գ                                            Գ");
  hputsxy(1,  9, "����������������������������������������������Դ");
  hputsxy(1, 10, "Գ  �e�����b�a ��aɷ�����b�� �������w�e����  Գ");
  hputsxy(1, 11, "������������������������������������������������");
  do {
    if(keypressed()) {
      switch(getkey()) {
        case F1 :
          task = HELPME;
          break;
        case F2 :
          task = CHANGEPASS;
          break;
        default :
          task = BLINK;
          break;
      }
      isquit = 1;
    }
    else {
      if(cnt++ > 600) {
        task = BLINK;
        isquit = 1;
        break;
      }
      delay(10);
    }
  } while(!isquit);
  closewindow();
}

void bell(void)
{
  sound(500);
  delay(250);
  nosound();
}

void password(void)
{
  int isquit = 0, cnt = 0, key;
  char pass[80] = "";

  if(!openwindow(18, 8, -48, -5)) return;
  hsetcolor(HLIGHTGREEN);
  hputsxy(1,  1, "����������������������������������������������Կ");
  hputsxy(1,  2, "Գ��Գ      �qѡ�i ���� ���A�a .....          Գ");
  hputsxy(1,  3, "����������������������������������������������Դ");
  hputsxy(1,  4, "Գ                                            Գ");
  hputsxy(1,  5, "������������������������������������������������");
  hgotoxy(4, 4);
  hclearkeybuf();
  do {
    switch(key = getchext()) {
      case ESC :
        task = BLINK;
        isquit = 1;
        break;
      case CR  :
        pass[cnt] = '\0';     /* ���b �h�e ���i ���a�i�� �e�e�a. */
        if( !strcmp(pass, passwd))
          task = QUIT, isquit = 1;
        else {
          task = BLINK, isquit = 1;
          bell();
        }
        break;
      default :
        hputs("*");
        pass[cnt++] = key;
        break;
    }
    if(cnt == MAXPASS) {
      task = BLINK;
      isquit = 1;
    }
  } while( !isquit);
  closewindow();
}

/*
 *   �qѡ�i �a���a.
 *   �a�� �qѡ�i ���a���a.  �᭡ ���e NULL�i ���a ��
 */

char *changepassword(void)
{
  int isquit = 0, cnt = 0, key;
  char pass[80] = "";

  if(!openwindow(18, 8, -48, -5)) return NULL;
  hsetcolor(HLIGHTGREEN);
  hputsxy(1,  1, "����������������������������������������������Կ");
  hputsxy(1,  2, "Գ��Գ      �qѡ�i �a�� ���A�a .....          Գ");
  hputsxy(1,  3, "����������������������������������������������Դ");
  hputsxy(1,  4, "Գ                                            Գ");
  hputsxy(1,  5, "������������������������������������������������");
  hgotoxy(4, 4);
  hclearkeybuf();
  do {
    switch(key = getchext()) {
      case ESC :
        task = BLINK;
        pass[0] = NULL;
        isquit = 1;
        break;
      case CR  :
        pass[cnt] = '\0';
        task = BLINK, isquit = 1;
        break;
      default :
        hputs("*");
        pass[cnt++] = key;
        break;
    }
    if(cnt == MAXPASS) {
      task = BLINK;
      isquit = 1;
    }
  } while( !isquit);
  closewindow();
  if(pass[0] != NULL)
    strcpy(passwd, pass);
  return (key == ESC) ? 0 : passwd;
}

/*
 *    �qѡ�i �qѡ �a���A �� ���e�a .
 */

void writepassword(void)
{
  char title[] = "Screen Locker Password File V1.00";
  char buf[MAXPASS+1];
  FILE *fp;

  fp = fopen("C:\\PASSWORD.HTL", "wb");     /* ���� �qѡ �a���i �e�e�a. */
  sprintf(buf, "%s%c", title, 0x1A);
  fwrite(buf, strlen(title)+1, 1, fp);
  encoder();                                /* �qѡ�i �e�eӡ �qѡ�� �e�a. */
  fwrite(passwd, MAXPASS, 1, fp);           /* �qѡ�i �a���A �� ���e�a. */
  fclose(fp);
  decoder();
}


int initpassword(void)
{
  char title[] = "Screen Locker Password File V1.00";
  char buf[MAXPASS+1];
  FILE *fp;

  fp = fopen("C:\\PASSWORD.HTL", "rb");         /* �qѡ �a���i �e�a. */
  if(!fp) {                                     /* �qѡ �a���� ���a�e */
    fp = fopen("C:\\PASSWORD.HTL", "wb");       /* ���� �qѡ �a���i �e�e�a. */
    sprintf(buf, "%s%c", title, 0x1A);
    fwrite(buf, strlen(title)+1, 1, fp);
    if(changepassword()) {                      /* �qѡ�i ���� ���b�h�e�a */
      encoder();                                /* �qѡ�i �e�eӡ �qѡ�� �e�a. */
      fwrite(passwd, MAXPASS, 1, fp);           /* �qѡ�i �a���A �� ���e�a. */
      fclose(fp);
      decoder();
      return 1;
    }
    else
      return 0;
  }
  else {
    fseek(fp, (long)(strlen(title)+1), SEEK_SET);
    fread(passwd, MAXPASS, 1, fp);
    fclose(fp);
    decoder();
    return 1;
  }
}

/*
 *  x, y��á�A color�� ���i�� �i�i �e�b���a.
 */

void blinkstar(int x, int y, int color)
{
  char blinkch[] = { 7, 4, 15, ' ' };
  char star[2];
  int i;

  hsetcolor(color);
  for(i = 0; i < 4; i++) {
    sprintf(star, "%c%c", blinkch[i], '\0');
    eputsxy(x, y, star);
    delay(250);
  }
}

/*
 *  �A�A �a�a�� �i�i ��w�e�a.
 */

void qstore(void)
{
  int x, y, color;

  if(pos == MAXSTARS)
    pos = 0;

  x = 1 + random(maxwidth);		      /* ���b�ᝡ x��á�i �i����ǥ�a. */
  y = 1 + random(maxheight);        /* ���b�ᝡ y��á�i �i����ǥ�a. */
  color = 1 + random(maxcolors);    /* ���b�ᝡ ���i�i �i����ǥ�a.  */
  stars[pos].x = x, stars[pos].y = y, stars[pos].color = color;

  hsetcolor(stars[pos].color);
  eputsxy(stars[pos].x, stars[pos].y, ".");

  pos++;
}

/*
 *  �A�A�� �a�a�� �i�i �ᐅ�a.
 */

void qretrieve(void)
{
  static int dq_pos = MAXSTARS;

  dq_pos++;
  if(dq_pos >= MAXSTARS)
    dq_pos = 0;
  if(dq_pos == pos) {
    dq_pos--;
    return;
  }

  blinkstar(stars[dq_pos+1].x, stars[dq_pos+1].y, stars[dq_pos+1].color);
}

/*
 *  �A�A�� �a�w �����E �i�i ������(�e�b�a�A �e �a�q)
 *  �� �i�i �i�����a ���e�A �a�a ���a.
 */
void displaystar(void)
{
  int i = 3;

  while(i--)
    qretrieve();
  i = 3;
  while(i--)
    qstore();
}

/*
 *  �a���I �A������ �i�� ���a�� ���i ���b�ᝡ �i����ǥ�a�q
 *  ���w �A�A ���e�a.
 */
void initstar(void)
{
  int i, seed = 1968;    /* �a�� �� �e�� */

  srand(seed);			     /* ���b�� �q���i seed�t�i �a���� */

  maxwidth  = hgetmaxax();
  maxheight = hgetmaxay();
  maxcolors = hgetmaxcolor();

  for(i = 0; i < MAXSTARS; i++) {  /* MAXSTAR���� �i�i ���w �A�A ���� ���e�A �a�� */
    qstore();
  }
  /*
   * �����A �A�e ���w �A�a �a�b�a�A �A�e�A displaystar()�q���A��e
   * ���w �A�e�A�� �a�w �����E �i�i �a�a ������ �a�� �a�a ���b�ᝡ ���a�i
   * �i�����a �a�� ���w �A�e�A ���e�a. �b, �����A �A�e �� ���w �A�e ��A�a
   * �a�b �a ���e �wȁ�a �E�a.
   */
}


void main(void)
{
  int isquit = 0, cnt = 0;

  /* �e�i ���a�Q ������ */

  inithanlib(FIXRESMODE, HANDETECT, HAN412GS1, ENGGD5);
  registerhgrfont(HGR1);  /* �e�i �a��Ϣ �i�� �w�� */
  displayhelp();
  initstar();             /* �a���I �i�i�i ������ �e�a. */
  if(!initpassword())     /* �qѡ �������A ��́�a�e ��З�i ͡���e�a. */
    return;
  do {
    if(keypressed()) {
      switch(getkey()) {
        case F1 :
          task = HELPME;
          break;
        case F2 :
          task = CHANGEPASS;
          break;
        case ESC :
          task = BLINK;
          break;
        default :
          task = PASSWORD;
          break;
      }
    }
    switch(task) {
      case BLINK :
        displaystar();
        break;
      case HELPME :
        displayhelp();
        break;
      case PASSWORD :
        password();
        break;
      case CHANGEPASS :
        password();
        if(task != QUIT)
          bell();
        else {
          changepassword();
          writepassword();
        }
        break;
      case QUIT :
        isquit = 1;
        break;
    }
  } while ( !isquit);
  closehan();
  puts(" � Screen Locker Ver 1.00   Hanyang Univ. AI Lab. Kwon. J. R .. For Hard Disk");
}


 