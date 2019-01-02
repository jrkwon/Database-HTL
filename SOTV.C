/********** The Son of Tetris Project ************/

/************ VIDEO/DISPLAY functions ***********/


#include <conio.h>
#include <dos.h>
#include <stdio.h>
#include <stdlib.h>
#include "hanlib.h"
#include "sot.h"

#define COFF 28       /* Column offset main display */
#define LOFF 3        /* Line offset */

#define SCOR_COFF 55
#define SCOR_LOFF 5
#define CRIB_COFF 3
#define CRIB_LOFF 3

#define HOT_CHAR "‘≤"
#define FILL_CHAR "˘"

static void *screen_buf;
static int x_pos, y_pos;

void init_video(void)
{
  screen_buf = (void *)malloc(80*25*2);
  gettext(1, 1, 80, 25, screen_buf);
  x_pos = wherex();
  y_pos = wherey();
}

void end_video(void)
{
  puttext(1, 1, 80, 25, screen_buf);
  gotoxy(x_pos, y_pos);
  free(screen_buf);
}

void  p_arena(int left,int top,int right,int bottom)
{
  unsigned x;

  left = (left > 0) ? left : 1;  /* Force display only within boundaries */
  right = (right < BLW-1) ? right : BLW-2;
  top = (top > 0) ? top : 1;
  bottom = (bottom < BLH-1) ? bottom : BLH-2;

  for(; top <= bottom; top++) {
    for(x = left; x <= right; x++) {
      switch(arena[x][top]) {
        case CLEAR:
          eputsxy(COFF+x*2+1, LOFF+top, (x & 1) ? " " : FILL_CHAR);
          eputsxy(COFF+x*2+2, LOFF+top, " ");
          break;
        default:
          hputsxy(COFF+x*2+1, LOFF+top, (arena[x][top] > BLINK) ? HOT_CHAR : "‘€");
          break;
      } /* switch */
    } /* for each column */
  } /* for each row */
} /* p_arena */

void      v_kill_row(int  t_row)
{
  hwindow(COFF+ 3, LOFF + 1, COFF + (BLW-1) * 2, LOFF + t_row);
  hgotoxy(1,1);
  hbackscroll();
  hwindow(1,1,80,25);
  p_arena(1,1,BLW-2,1);
}         /* v_kill_row */

static void p_surround(void)
{
  int i;

  for (i = (LOFF - 1) + 2; i < (LOFF - 1) + BLH; i++) {
    hputsxy(COFF + 1, i, "‘∫");
    hputsxy(COFF+ 2 * BLW-1, i, "‘∫");
  }
  hgotoxy(COFF + 3, (LOFF - 1) + BLH);
  for (i = 0; i < (BLW - 2); i++)
    hputs("‘Õ");
  hputsxy(COFF + 1, (LOFF - 1) + BLH, "‘»");
  hputsxy(COFF + 2 * BLW - 1, (LOFF - 1) + BLH, "‘º");
} /* p_surround  */

void draw_ground(void)
{
  char msg[] = "‘É –e¥∑îÅ–bäa â∑â¡îÅ–b ∏Â∏a…∑Ø•â∑–bâ¡ ≠QÀiï∑¥aü° –eÀi¥aü° ‘É";

  p_surround();
  hputsxy(SCOR_COFF,SCOR_LOFF-2, "–e¥∑îÅ ∏Â∏a…∑Ø•â∑–bâ¡");
  hputsxy(SCOR_COFF,SCOR_LOFF, "îe âÅ :");
  hputsxy(SCOR_COFF,SCOR_LOFF+1, "∏Ò ÆÅ :");
  hputsxy(SCOR_COFF,SCOR_LOFF+2, "∫â ÆÅ :");
  hputsxy(SCOR_COFF,SCOR_LOFF+3, "îa∑q °°¥∑ :");

  hputsxy(CRIB_COFF,CRIB_LOFF, "‘Ñ  –e¥∑ …AÀaü°Øa  ‘Ñ");
  hputsxy(CRIB_COFF,CRIB_LOFF+2, "7: ∂EΩ¢        P: ∏qåe");
  hputsxy(CRIB_COFF,CRIB_LOFF+3, "8: “A∏Â        S: ≠°ü°");
  hputsxy(CRIB_COFF,CRIB_LOFF+4, "9: µ°üeΩ¢      ESC: è{");
  hputsxy(CRIB_COFF,CRIB_LOFF+5, "Q: îa∑q°°¥∑");
  hputsxy(CRIB_COFF,CRIB_LOFF+6, "6: îeâÅµ©ü°ã°");
  hputsxy(CRIB_COFF,CRIB_LOFF+7, "SPACE: òÈ¥·õaü±");
  hputsxy(hgetmaxax()/2-strlen(msg)/2, hgetmaxay(), msg);
}   /* init_video */

void  update_score(unsigned level, long score, unsigned lines_del,
                   SHP_TYPE *next_sh, int show_shape)
{
  int x,y, *map_ptr, tmp_color;

  hprintfxy(SCOR_COFF + 8, SCOR_LOFF, "%7u",level);
  hprintfxy(SCOR_COFF + 8, SCOR_LOFF+1, "%7ld",score);
  hprintfxy(SCOR_COFF + 8, SCOR_LOFF+2, "%7d", lines_del);

  hwindow(SCOR_COFF, SCOR_LOFF + 6,SCOR_COFF + 15, SCOR_LOFF + 10);
  hclrscr();
  hwindow(1,1,80,25);
  if (show_shape) {
    map_ptr = next_sh -> map[0];
    for (y = 0; y < next_sh ->ht; y++) {
      hgotoxy(SCOR_COFF, SCOR_LOFF + 6 + y);
      for (x = 0; x < next_sh ->wd; x++,map_ptr++) {
        if (*map_ptr) {
          tmp_color = hgetcolor();
          hsetcolor(next_sh->col);
          hputs("‘€");
          hsetcolor(tmp_color);
        }
        else
          hputs("  ");
      } /* for x */
    } /* for y */
  } /* if show_shape */
}  /* update_score */
