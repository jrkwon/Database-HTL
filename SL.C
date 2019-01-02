/*
 *      File SL.C         Screen Locker
 *
 *      '92. 6. 5. ‹q
 *
 *      ÑÁ¡e ¥¡Ñ¡µÁ ”aŸe ¬aœq·¡ Ğq¦¡ ¸a¯¥· ‰¬e‹¡Ÿi ¬a¶wĞa“e ˆé
 *       b‹¡¶áĞe ÑÁ¡e ¸a¢‰®A
 *
 *       e—e·¡ : Š¥ ¸ œb
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dos.h>
#include <bios.h>
#include <alloc.h>

/*
 *   hgettextµÁ hputtextĞq®“e TEXTEMUL·¡ Ğe¤å ¸÷· –A´á ·¶·a¡e Ÿ·Ça–A»¡
 *   ´g·qµA º·
 */

#include "ascii.h"
#include "extkey.h"
#include "hanlib.h"
#include "hanin.h"

#define MAXSTARS    60
#define MAXPASS     43

/* ®Ğ—Ğ´¡ Ği ¸b´ó· ¹·ŸA */

enum {
  BLINK = -1, /*  ¥i·¡ ¤e¼b·±       */
  HELPME,     /*  •¡¶‘ i       F1   */
  PASSWORD,   /*  ´qÑ¡ ·³b         */
  CHANGEPASS, /*  ´qÑ¡ ¤a‹¡  F2   */
  QUIT        /*  Ïa¡‹aœ‘ ÈiÂ‰     */
};

typedef struct {
  int x, y;       /* ¥i·¡ ¤e¼b·© x, y¹ÁÎa */
  int color;      /* ¤e¼b·© ¥i· ¬‚Œi     */
} STAR;

STAR stars[MAXSTARS];

int task = BLINK;      /* ¸b´ó ¥e® */
int maxwidth, maxheight, maxcolors;
byte passwd[MAXPASS+1];
int pos = 0;           /* ¶¥Ñw ÇAµA¬á ¸aaˆa ¸á¸w–A´á ·¶“e ¶áÃ¡ */
byte far *_scrbuf;

struct {
  int x1, y1, x2, y2;
} window;

/*
 *   ´aº ˆe”eĞe Àw¢… Ğq®—i
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
 *    Ïa¡‹aœ‘· º¶a ¦¦…
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
  hputsxy(1,  1, "ÔÚÔÄÔÂÔÄÔÄÔÄÔÄÔÄÔÄÔÄÔÄÔÄÔÄÔÄÔÄÔÄÔÄÔÄÔÄÔÄÔÄÔÄÔÄÔ¿");
  hputsxy(1,  2, "Ô³ÔÛÔ³      µi®A Ôù ¸a¢‰®A  •¡ ¶‘  i          Ô³");
  hputsxy(1,  3, "ÔÃÔÄÔÁÔÄÔÄÔÄÔÄÔÄÔÄÔÄÔÄÔÄÔÄÔÄÔÄÔÄÔÄÔÄÔÄÔÄÔÄÔÄÔÄÔ´");
  hputsxy(1,  4, "Ô³                                            Ô³");
  hputsxy(1,  5, "Ô³          F1 - »¡‹q ·¡ •¡¶‘ i               Ô³");
  hputsxy(1,  6, "Ô³                                            Ô³");
  hputsxy(1,  7, "Ô³          F2 - ´qÑ¡ ¤a‹¡                  Ô³");
  hputsxy(1,  8, "Ô³                                            Ô³");
  hputsxy(1,  9, "ÔÃÔÄÔÄÔÄÔÄÔÄÔÄÔÄÔÄÔÄÔÄÔÄÔÄÔÄÔÄÔÄÔÄÔÄÔÄÔÄÔÄÔÄÔÄÔ´");
  hputsxy(1, 10, "Ô³  Ğe´·”ĞbŠa ¸å¸aÉ·¯¥‰·Ğb‰Á ·¥‰·»¡“wµeŠ¯©  Ô³");
  hputsxy(1, 11, "ÔÀÔÄÔÄÔÄÔÄÔÄÔÄÔÄÔÄÔÄÔÄÔÄÔÄÔÄÔÄÔÄÔÄÔÄÔÄÔÄÔÄÔÄÔÄÔÙ");
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
  hputsxy(1,  1, "ÔÚÔÄÔÂÔÄÔÄÔÄÔÄÔÄÔÄÔÄÔÄÔÄÔÄÔÄÔÄÔÄÔÄÔÄÔÄÔÄÔÄÔÄÔÄÔ¿");
  hputsxy(1,  2, "Ô³ÔÛÔ³      ´qÑ¡Ÿi ı´á º­A¶a .....          Ô³");
  hputsxy(1,  3, "ÔÃÔÄÔÁÔÄÔÄÔÄÔÄÔÄÔÄÔÄÔÄÔÄÔÄÔÄÔÄÔÄÔÄÔÄÔÄÔÄÔÄÔÄÔÄÔ´");
  hputsxy(1,  4, "Ô³                                            Ô³");
  hputsxy(1,  5, "ÔÀÔÄÔÄÔÄÔÄÔÄÔÄÔÄÔÄÔÄÔÄÔÄÔÄÔÄÔÄÔÄÔÄÔÄÔÄÔÄÔÄÔÄÔÄÔÙ");
  hgotoxy(4, 4);
  hclearkeybuf();
  do {
    switch(key = getchext()) {
      case ESC :
        task = BLINK;
        isquit = 1;
        break;
      case CR  :
        pass[cnt] = '\0';     /* ·³b ¤h·e ˆõ·i ¢…¸aµi¡  e—e”a. */
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
 *   ´qÑ¡Ÿi ¤a…”a.
 *   ¤aå ´qÑ¡Ÿi •©aº…”a.  Âá­¡ ˜“e NULL·i •©a º‘
 */

char *changepassword(void)
{
  int isquit = 0, cnt = 0, key;
  char pass[80] = "";

  if(!openwindow(18, 8, -48, -5)) return NULL;
  hsetcolor(HLIGHTGREEN);
  hputsxy(1,  1, "ÔÚÔÄÔÂÔÄÔÄÔÄÔÄÔÄÔÄÔÄÔÄÔÄÔÄÔÄÔÄÔÄÔÄÔÄÔÄÔÄÔÄÔÄÔÄÔ¿");
  hputsxy(1,  2, "Ô³ÔÛÔ³      ´qÑ¡Ÿi ¤a¡ º­A¶a .....          Ô³");
  hputsxy(1,  3, "ÔÃÔÄÔÁÔÄÔÄÔÄÔÄÔÄÔÄÔÄÔÄÔÄÔÄÔÄÔÄÔÄÔÄÔÄÔÄÔÄÔÄÔÄÔÄÔ´");
  hputsxy(1,  4, "Ô³                                            Ô³");
  hputsxy(1,  5, "ÔÀÔÄÔÄÔÄÔÄÔÄÔÄÔÄÔÄÔÄÔÄÔÄÔÄÔÄÔÄÔÄÔÄÔÄÔÄÔÄÔÄÔÄÔÄÔÙ");
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
 *    ´qÑ¡Ÿi ´qÑ¡ Ìa·©µA °á ı“e”a .
 */

void writepassword(void)
{
  char title[] = "Screen Locker Password File V1.00";
  char buf[MAXPASS+1];
  FILE *fp;

  fp = fopen("C:\\PASSWORD.HTL", "wb");     /* ¬¡ ´qÑ¡ Ìa·©·i  e—e”a. */
  sprintf(buf, "%s%c", title, 0x1A);
  fwrite(buf, strlen(title)+1, 1, fp);
  encoder();                                /* ´qÑ¡Ÿi ˆe”eÓ¡ ´qÑ¡ÑÁ Ğe”a. */
  fwrite(passwd, MAXPASS, 1, fp);           /* ´qÑ¡Ÿi Ìa·©µA °á ı“e”a. */
  fclose(fp);
  decoder();
}


int initpassword(void)
{
  char title[] = "Screen Locker Password File V1.00";
  char buf[MAXPASS+1];
  FILE *fp;

  fp = fopen("C:\\PASSWORD.HTL", "rb");         /* ´qÑ¡ Ìa·©·i µe”a. */
  if(!fp) {                                     /* ´qÑ¡ Ìa·©·¡ ´ô·a¡e */
    fp = fopen("C:\\PASSWORD.HTL", "wb");       /* ¬¡ ´qÑ¡ Ìa·©·i  e—e”a. */
    sprintf(buf, "%s%c", title, 0x1A);
    fwrite(buf, strlen(title)+1, 1, fp);
    if(changepassword()) {                      /* ´qÑ¡Ÿi ¬¡ ·³b¤h“e”a */
      encoder();                                /* ´qÑ¡Ÿi ˆe”eÓ¡ ´qÑ¡ÑÁ Ğe”a. */
      fwrite(passwd, MAXPASS, 1, fp);           /* ´qÑ¡Ÿi Ìa·©µA °á ı“e”a. */
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
 *  x, y¶áÃ¡µA color· ¬‚Œi¡ ¥i·i ¤e¼b·¥”a.
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
 *  ÇAµA Ğaa· ¥i·i ¸á¸wĞe”a.
 */

void qstore(void)
{
  int x, y, color;

  if(pos == MAXSTARS)
    pos = 0;

  x = 1 + random(maxwidth);		      /* ¢¸b¶á¡ x¶áÃ¡Ÿi ¤i¬—¯¡Ç¥”a. */
  y = 1 + random(maxheight);        /* ¢¸b¶á¡ y¶áÃ¡Ÿi ¤i¬—¯¡Ç¥”a. */
  color = 1 + random(maxcolors);    /* ¢¸b¶á¡ ¬‚Œi·i ¤i¬—¯¡Ç¥”a.  */
  stars[pos].x = x, stars[pos].y = y, stars[pos].color = color;

  hsetcolor(stars[pos].color);
  eputsxy(stars[pos].x, stars[pos].y, ".");

  pos++;
}

/*
 *  ÇAµA¬á Ğaa· ¥i·i Œá…”a.
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
 *  ÇAµA¬á ˆa¸w µ¡œ–E ¥i·i »¡¶‰¡(¤e¼bĞa‰A Ğe ”a·q)
 *  ¬ ¥i·i ¤i¬—¯¡Åa ÑÁ¡eµA aÈa …”a.
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
 *  Îa¯¡–I ÂA”ˆ· ¥i· ¹ÁÎaµÁ ¬‚·i ¢¸b¶á¡ ¤i¬—¯¡Ç¥”a·q
 *  ¶¥Ñw ÇAµA ı“e”a.
 */
void initstar(void)
{
  int i, seed = 1968;    /* a· Â‰¬— ‘e•¡ */

  srand(seed);			     /* ¢¸b¶á Ğq®Ÿi seedˆt·i ˆa»¡‰¡ */

  maxwidth  = hgetmaxax();
  maxheight = hgetmaxay();
  maxcolors = hgetmaxcolor();

  for(i = 0; i < MAXSTARS; i++) {  /* MAXSTARˆ· ¥i·i ¶¥Ñw ÇAµA ı‰¡ ÑÁ¡eµA Îa¯¡ */
    qstore();
  }
  /*
   * ·¡œı‰A –A¡e ¶¥Ñw ÇAˆa ˆa—bÀa‰A –A“e•A displaystar()Ğq®µA¬á“e
   * ¶¥Ñw ÇA´eµA¬á ˆa¸w µ¡œ–E ¥i·i Ğaa »¡¶‰¡ ”a¯¡ Ğaa ¢¸b¶á¡ ¹ÁÎaŸi
   * ¤i¬—¯¡Åa ”a¯¡ ¶¥Ñw ÇA´eµA ı“e”a. »b, ·¡œı‰A –A¡e ·¡ ¶¥Ñw ÇA“e ´å¹Aa
   * ˆa—b Àa ·¶“e ¬wÈˆa –E”a.
   */
}


void main(void)
{
  int isquit = 0, cnt = 0;

  /* Ğe‹i ¯¡¯aÉQ Á¡‹¡ÑÁ */

  inithanlib(FIXRESMODE, HANDETECT, HAN412GS1, ENGGD5);
  registerhgrfont(HGR1);  /* Ğe‹i ‹aœÏ¢ ‹i© —w¢ */
  displayhelp();
  initstar();             /* Îa¯¡–I ¥i—i·i Á¡‹¡ÑÁ Ğe”a. */
  if(!initpassword())     /* ´qÑ¡ Á¡‹¡ÑÁµA ¯©ÌĞa¡e ¯©Ğ—·i Í¡‹¡Ğe”a. */
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
  puts(" ş Screen Locker Ver 1.00   Hanyang Univ. AI Lab. Kwon. J. R .. For Hard Disk");
}


 