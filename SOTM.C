/********** The Son of Tetris Project ************/

/************  MANIPULATION of SHAPES ***********/

#include <assert.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>


#include "sot.h"

#define DELAY_TIME 25 /* Loop, unit of time */
#define DONT_CARE 0   /* For unused parameter */
#define HOT_LEVEL 4  /* Level at which hot objects start to appear */
#define MAX_HEAT 4  /* Maximum heat */

void  or_obj(SHP_TYPE *sh, int *a_ht, int *a_wd, int orient)
/* Set object orientation */
{
  if (orient & 1)
  {
    *a_wd = sh->ht; *a_ht = sh->wd;
  } /* rotated 90/270 */
  else
  {
    *a_wd = sh->wd; *a_ht = sh->ht;
  } /* rotated 0/180 */
}     /* or_obj */



int  shp_pos(SHP_TYPE *sh, int  x_pos, int  y_pos,
             int orient, int temperature, sh_op_type fn)
{
  int        i,j, a_wd, a_ht, *map_ptr;
  unsigned  val, test_val = 0;

  or_obj(sh, &a_ht, &a_wd, orient);   /* get ht and width correct */
  map_ptr = sh->map[orient];          /* set orientation of shape */

  switch(fn)
  {
    case test:
      for (i = y_pos; i < a_ht + y_pos; i++)
        for (j = x_pos; j < a_wd + x_pos; j++, map_ptr++)
          if ((val = arena[j][i]) && *map_ptr  /* clash exists at this point */
              && (val > test_val))
            test_val = val;
      break;

    case insert:
      for (i = y_pos; i < a_ht + y_pos; i++)
        for (j = x_pos; j < a_wd + x_pos; j++, map_ptr++)
          if (*map_ptr)  /* i.e. shape exists at this point */
            if (temperature)
              arena[j][i] = sh->col | BLINK;
            else  /* cold! */
              arena[j][i] = sh->col;
      break;


    case remve:
      for (i = y_pos; i < a_ht + y_pos; i++)
        for (j = x_pos; j < a_wd + x_pos; j++, map_ptr++)
          if (*map_ptr)  /* i.e. shape exists at this point */
            arena[j][i] = CLEAR;
      break;
  } /* switch */
  return(test_val);
}     /*  shp_pos */


int    mv(drctn_type dt, OBJ_TYPE *cur_obj, int amt)
{
  int x, y,
      or,
      disp_left, disp_right,
      disp_top,disp_bot,
      wd, ht,
      display,
      result;
  SHP_TYPE *shp;

  shp = cur_obj->o_sh;
  or_obj(shp, &ht, &wd, cur_obj -> o_or);   /* get ht and width correct */

  disp_left = cur_obj->o_x;
  disp_right = cur_obj->o_x + wd - 1;
  disp_top = cur_obj->o_y;
  disp_bot = cur_obj->o_y + ht - 1;
  switch(dt)
  {
    case left:
      x = cur_obj->o_x - amt;
      y = cur_obj->o_y;
      or = cur_obj -> o_or;
      disp_left -= amt;
      break;

    case down:
      x = cur_obj->o_x;
      y = cur_obj->o_y + amt;
      or = cur_obj -> o_or;
      disp_bot += amt;
      break;

    case right:
      x = cur_obj->o_x + amt;
      y = cur_obj->o_y;
      or = cur_obj -> o_or;
      disp_right += amt;
      break;

    case anti:
      x = cur_obj->o_x ;
      y = cur_obj->o_y ;
      or = (cur_obj -> o_or + 1) & 3;
      disp_right = disp_bot = (ht > wd) ? ht : wd;
      disp_right += x; disp_bot += y;
      break;

        case clk:

    default:
      assert(FALSE);

  } /* switch */

  shp_pos(cur_obj->o_sh, cur_obj->o_x, cur_obj->o_y,
          cur_obj->o_or, DONT_CARE, remve);
  display = !(result = shp_pos(cur_obj->o_sh, x, y, or, DONT_CARE, test));

  if (!result)  /* ok to move */
  {
    cur_obj->o_x = x;
    cur_obj->o_y = y;
    cur_obj->o_or = or;
  }
  else
  if ( (cur_obj->o_temp) && (result < BORDER) ) /* object is hot */
  {
    cur_obj->o_x = x;
    cur_obj->o_y = y;
    cur_obj->o_or = or;
    cur_obj->o_temp--;     /* cool object down */
    result = 0; /* hot object has moved: force result */
    display = TRUE;
  }
  else
  if ((cur_obj->o_temp) && (dt == down))
  {
    cur_obj->o_temp = 0;
    display = TRUE; /* force display of cool object */
  }

  shp_pos(cur_obj->o_sh, cur_obj->o_x, cur_obj->o_y,
                     cur_obj->o_or, cur_obj->o_temp, insert);

  if (display)  /* display shape as moved */
  {
    p_arena(disp_left,disp_top,disp_right,disp_bot);
  }

  return(result);
} /* mv */


int    insrt(OBJ_TYPE *cur_obj,SHP_TYPE *next_sh, int level)
{ /* Set up new falling object if possible */
  SHP_TYPE *sh;
  int  result;

  #define TOP 1

  sh = cur_obj->o_sh = next_sh;
  cur_obj->o_x = (BLW/2) - (sh->wd / 2); /* in the middle */
  cur_obj->o_or = 0; /* Default orientation is upright */
  cur_obj->o_y =  TOP;
  cur_obj->o_temp = 0;  /* Cold */
  if ( (level > HOT_LEVEL) & !(rand() % 4) )
    cur_obj->o_temp = MAX_HEAT;

  result = shp_pos(cur_obj->o_sh, cur_obj->o_x, cur_obj->o_y,
                                    cur_obj->o_or,  DONT_CARE, test);
  shp_pos(cur_obj->o_sh, cur_obj->o_x, cur_obj->o_y,
                                  cur_obj->o_or,  cur_obj->o_temp, insert);
  p_arena(cur_obj->o_x,TOP,cur_obj->o_x + sh -> wd - 1,TOP +sh -> ht -1);
  return(result);
} /* insrt */



