/********** The Son of Tetris Project ************/

/************  GAME ***********/

#include <conio.h>
#include <dos.h>
#include <stdlib.h>
#include <bios.h>
#include "hanin.h"
#include "sot.h"

#define MAX_LEVEL 9
#define ACT_LEVEL 3  /* level at which extra shapes start appearing */
#define MIN_SHAPES 7 /* minimum number of shapes */
#define ADD_SHAPE MIN_SHAPES - ACT_LEVEL + 1
#define MAX_SHAPE 20 /* maximum number of shapes allowed before upping level */

#define DELAY_TIME 120 /* Loop, unit of time */

int      mv(drctn_type dt, OBJ_TYPE *cur_obj, int amt);   /* move object */
int      insrt(OBJ_TYPE *cur_obj,SHP_TYPE *next_sh);      /* insert object */

static  SHP_TYPE *next_sh;  /* next shape to appear */
static  int level;    /* Level at which game is being played */
static  long      score;
static  unsigned  lines_del;/* Lines deleted */
static  int       sound_on = TRUE; /* Controls use of sound FX */
static  int       show_shape = TRUE; /* show next shape */
static  unsigned  shape_count;

static void get_next_shape(void)
{
  unsigned new_shape_no, choice_of_shapes;

  if (level < ACT_LEVEL)
    choice_of_shapes = MIN_SHAPES;
  else
    choice_of_shapes = (no_of_shapes < level + ADD_SHAPE) ?
                                       no_of_shapes : level + ADD_SHAPE;
  new_shape_no = (rand() % choice_of_shapes);
  for (next_sh = shp_lst; new_shape_no; new_shape_no--)
    next_sh = next_sh -> next_shp;
}    /* get_next_shape */

static void level_fx(void)
{
  int i;

  if (sound_on) {
    for (i = 1000; i < 3000; i+= 50) {
      sound(i); delay(10);
    }
    nosound();
  }
} /* do_blip */

static void do_blip(void)
{
  int i;

  if(sound_on) {
    for(i = 0; i < 10; i++) {
      sound(2000+i*500);
      delay(25);
    }
    nosound();
  }
}

static void end_noise(void)
{
  int i;

  if (!sound_on)
    return;
  for (i = 4000; i > 2000; i-= 50) {
    sound(i); delay(50);
  }
  nosound();
} /* end_noise */

static void   kill_row(int t_row)
{
  int x,y;

  for (y = t_row; y > 1; y--)
    for (x = 1; x < BLW - 1; x++)
      arena[x][y] = arena[x][y-1];
  for (x = 1; x < BLW - 1; x++)
      arena[x][1] = CLEAR;
  v_kill_row(t_row);
  p_arena(1,1,BLW-1,t_row);
}      /* kill_row */

static int   check_full_row(void)
{
  int y,x;
  int clear_found, result;

  result = FALSE;
  for (y = BLH - 2; y > 1; y--)
  {
    clear_found = FALSE;
    for (x = 1; x <= BLW -2; x++)
      if (arena[x][y] == CLEAR)
      {
        clear_found = TRUE;
        break;
      }
    if (clear_found)
      continue;
    lines_del++;  result = TRUE;
    kill_row(y);
    y++;
  }
  return(result);
} /* check_full_row */

static int handle_keys(OBJ_TYPE *cur_obj)
{
  int  i;           /* Index to Number of keys allowed per loop */

  static unsigned delay_period[MAX_LEVEL + 1] =
                {80, 66, 54, 44, 36, 26, 22, 18, 16, 14 };

  for (i = 0; i < NO_OF_KEY_PRESSES; i++)
  {
    if(bioskey(1))
      switch(get_key())
      {
        case HOME:              /* Go Left */
        case '7':
          mv(left, cur_obj, HORIZ_MOVE);
          break;


        case UP:                /* Rotate */
        case '8':
          mv(anti, cur_obj, HORIZ_MOVE);
          break;


        case PGUP:             /* Go right */
        case '9':
          mv(right, cur_obj, HORIZ_MOVE);
          break;

        case LEFT:             /* Drop */
        case '4':
        case ' ':
          while (!mv(down,cur_obj, VERT_MOVE));
          break;


        case RIGHT:             /* Level up */
        case '6':
          if (level < MAX_LEVEL)
          {
            level++;
            level_fx();
            shape_count  = 0;
            update_score(level, score, lines_del, next_sh, show_shape);
          }
          break;

        case 'p':                /* Pause game */
        case 'P':
          if (pause_game())
            return TRUE;

        case 'q':                /* Toggle next shape flag */
        case 'Q':
          show_shape = !show_shape;
          update_score(level, score, lines_del, next_sh, show_shape);
          break;

        case ESC:                /* Quit game */
          end_game();
          exit(1);
          break;


        case 's':
        case 'S':
          sound_on = !sound_on;
          break;

        default:
          /* Do nothing */
          break;
      } /* switch */
    else
      delay(delay_period[level]);  /* delay after action */
  } /* for */
  return FALSE;
}  /* handle_keys */

void   game()
{
  OBJ_TYPE *cur_obj;
  unsigned local_score;

  do
  {
    init_arena();
    p_arena(0,0,(BLW-1),(BLH-1));   /*  display blank arena */
    level = get_level();
    if(level == -1)
      return;
    score = 0L; lines_del = 0;
    cur_obj = (OBJ_TYPE *)malloc(sizeof(OBJ_TYPE));
    shape_count = 0;
    get_next_shape(); /* get first shape */
    while (!insrt(cur_obj,next_sh))
    {
      get_next_shape();
      update_score(level, score, lines_del, next_sh, show_shape);
      if(handle_keys(cur_obj))
        return;

      for (local_score = 24 + 3*level; !mv(down,cur_obj, VERT_MOVE); local_score--)
        handle_keys(cur_obj);

      if (check_full_row())
        do_blip();
      score += local_score;
      if (((lines_del/10) > level) && (level < MAX_LEVEL))
      {
        level++;
        level_fx();
        shape_count = 0;
      }
      if (!(++shape_count % MAX_SHAPE) && (level < MAX_LEVEL)) {
        level++;
        level_fx();
      }
    } /* while can insert */
    update_score(level, score, lines_del, next_sh, FALSE);
    end_noise();
    while (bioskey(1))   /* Empty keyboard buffer */
      bioskey(0);
  }
  while (get_another_go(score));   /* Dialogue with player between games */
  free(cur_obj);
} /* game */



