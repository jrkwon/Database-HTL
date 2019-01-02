#include <stdio.h>
#include <string.h>
#include <bios.h>
#include "hanlib.h"
#include "hanwindw.h"
#include "sot.h"

int        arena[BLW][BLH];   /* Where everything is stored */
SHP_TYPE  *shp_lst;           /* List of shape defns */
int        no_of_shapes;      /* Total shape defns held */

void _Cdecl disperror(byte *msg)
{
  int color, width;

  width = (strlen(msg)+4 > 26) ? strlen(msg)+4 : 26;
  if( !wopen(hgetmaxax()/2-strlen(msg)/2-1, hgetmaxay()/2-1, width, 3))
    return;
  wtitle(msg);
  color = hgetcolor();
  hsetcolor(HRED);
  wcenter(2, "[ESC]Ÿi ’Ÿa­A¶a");
  while(getchext() != ESC)
    ;
  hsetcolor(color);
  wclose();
}

void _Cdecl memerror(void)
{
  disperror("‹¡´â¸w­¡ˆa ¦¹¢Ða‘A¶a");
}

void end_game(void)
{
  end_score();
  closehan();
  end_video();
}

int get_key(void)
{
  int ch;

  ch = bioskey(0);
  return (ch & 0xFF) ? (ch & 0xFF) : ((ch & 0xFF00) >> 8) +256;
}

void main(int argc, char *argv[])
{
  printf("Hangul TETRIS (HTETRIS) Ver 1.00  Copyright(C) 1992 Hanyang Univ. AI-Lab\n");
  init_video();
  /* Ðeœa Á¡‹¡ÑÁ */
  inithanlib(FIXRESMODE, HANDETECT, HAN8GD2, DEFENGFONT);
  registeregrfont(EGR1);
  registerhgrfont(HGR1);
  draw_ground();
  init_shapes();
  init_score(argv[0]);
  game();
  end_game();
}