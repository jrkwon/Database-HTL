/********** The Son of Tetris Project ************/

/************ ARENA INITIALISATION ***********/

#include "sot.h"

void  init_arena(void)
{
  int x, y;

  for (x = 0; x < BLW; x++)  /* Clear the whole lot */
    for (y = 0; y < BLH; y++)
      arena[x][y] = CLEAR;

  for (x = 0; x < BLW; x++)  /* Draw top + bottom boundaries */
  {
    arena[x][0] = BORDER;
    arena[x][BLH-1] = BORDER;
  }

  for (y = 0; y < BLH; y++)  /* Draw left + right boundaries */
  {
    arena[0][y] = BORDER;
    arena[BLW-1][y] = BORDER;
  }

}  /*  init_arena  */




