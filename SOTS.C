/********** The Son of Tetris Project ************/

/************ SHAPE DEFINITIONS ***********/

#include <alloc.h>
#include <conio.h>
#include <time.h>        /* Need these to get random number seed */
#include <stdlib.h>

#include "sot.h"

#define  SHAPE !CLEAR

static void   rotate_shape(SHP_TYPE *cur_shape)
{
  int   i, j, wd, ht;
  int  *ptr, *ptr1, *ptr2, *ptr3;

  ptr = cur_shape ->map[0];
  ptr1 = cur_shape ->map[1];
  ptr2 = cur_shape ->map[2];
  ptr3 = cur_shape ->map[3];

  ht = cur_shape -> ht;
  wd = cur_shape -> wd;

  for (j = 0; j < ht; j++)
    for (i = 0; i < wd; i++)
      ptr1[((wd - i - 1) * ht) + j]
        = ptr2[(ht - j - 1) * wd + (wd - i - 1)]
        = ptr3[(ht - j - 1) + i * ht]
        = ptr[j * wd + i];

}  /* rotate_shape */



void  init_shapes(void)
{
  SHP_TYPE *cur_shape;
  int       orient;
  int      *ptr;
  long     now;

  no_of_shapes = 0;
  srand((unsigned)time(&now) % 37);    /* seed random number */

/* ************** Shape 1 ************** */  /* T shape */

  cur_shape = malloc(sizeof(SHP_TYPE));
  shp_lst = cur_shape; /* First time only */
  cur_shape -> ht = 3;                              /* Height */
  cur_shape -> wd = 3;                              /* Width */
  cur_shape -> col = WHITE;                           /* Colour */
  for (orient = 0; orient < 4; orient++)            /* Reserve space for maps */
    cur_shape -> map[orient] =
      calloc(cur_shape -> ht * cur_shape -> wd,sizeof(int));

  ptr = cur_shape -> map[0];                        /* Define shape */
  ptr[3] = ptr[4] = ptr[5] = ptr[7] = SHAPE;

  rotate_shape(cur_shape);                          /* Define other orientations */
  no_of_shapes++;

/* ************** Shape 2 ************** */ /* Line */

  cur_shape -> next_shp = malloc(sizeof(SHP_TYPE));
  cur_shape = cur_shape -> next_shp;                /* Link into list */
  cur_shape -> ht = 3;                              /* Height */
  cur_shape -> wd = 4;                              /* Width */
  cur_shape -> col = WHITE;                         /* Colour */
  for (orient = 0; orient < 4; orient++)            /* Reserve space for maps */
    cur_shape -> map[orient] =
      calloc(cur_shape -> ht * cur_shape -> wd,sizeof(int));

  ptr = cur_shape -> map[0];                        /* Define shape */
  ptr[4] = ptr[5] = ptr[6] = ptr[7] = SHAPE;

  rotate_shape(cur_shape);                          /* Define other orientations */
  no_of_shapes++;

/* ************** Shape 3************** */ /* Box */

  cur_shape -> next_shp = malloc(sizeof(SHP_TYPE));
  cur_shape = cur_shape -> next_shp;                /* Link into list */
  cur_shape -> ht = 2;                              /* Height */
  cur_shape -> wd = 2;                              /* Width */
  cur_shape -> col = WHITE;                          /* Colour */
  for (orient = 0; orient < 4; orient++)            /* Reserve space for maps */
    cur_shape -> map[orient] =
      calloc(cur_shape -> ht * cur_shape -> wd,sizeof(int));

  ptr = cur_shape -> map[0];                        /* Define shape */
  ptr[0] = ptr[1] = ptr[2] = ptr[3] = SHAPE;

  rotate_shape(cur_shape);                          /* Define other orientations */
  no_of_shapes++;

/* ************** Shape 4************** */ /* 'L' */

  cur_shape -> next_shp = malloc(sizeof(SHP_TYPE));
  cur_shape = cur_shape -> next_shp;                /* Link into list */
  cur_shape -> ht = 2;                              /* Height */
  cur_shape -> wd = 3;                              /* Width */
  cur_shape -> col = WHITE;                           /* Colour */
  for (orient = 0; orient < 4; orient++)            /* Reserve space for maps */
    cur_shape -> map[orient] =
      calloc(cur_shape -> ht * cur_shape -> wd,sizeof(int));

  ptr = cur_shape -> map[0];                        /* Define shape */
  ptr[0] = ptr[1] = ptr[2] = ptr[5] = SHAPE;

  rotate_shape(cur_shape);                          /* Define other orientations */
  no_of_shapes++;

/* ************** Shape 5************** */ /* Backwards 'L' */

  cur_shape -> next_shp = malloc(sizeof(SHP_TYPE));
  cur_shape = cur_shape -> next_shp;                /* Link into list */
  cur_shape -> ht = 2;                              /* Height */
  cur_shape -> wd = 3;                              /* Width */
  cur_shape -> col = WHITE;                       /* Colour */
  for (orient = 0; orient < 4; orient++)            /* Reserve space for maps */
    cur_shape -> map[orient] =
      calloc(cur_shape -> ht * cur_shape -> wd,sizeof(int));

  ptr = cur_shape -> map[0];                        /* Define shape */
  ptr[0] = ptr[1] = ptr[2] = ptr[3] = SHAPE;

  rotate_shape(cur_shape);                          /* Define other orientations */
  no_of_shapes++;

/* ************** Shape 6************** */ /* 'N' thingy */

  cur_shape -> next_shp = malloc(sizeof(SHP_TYPE));
  cur_shape = cur_shape -> next_shp;                /* Link into list */
  cur_shape -> ht = 2;                              /* Height */
  cur_shape -> wd = 3;                              /* Width */
  cur_shape -> col = WHITE;                         /* Colour */
  for (orient = 0; orient < 4; orient++)            /* Reserve space for maps */
    cur_shape -> map[orient] =
      calloc(cur_shape -> ht * cur_shape -> wd,sizeof(int));

  ptr = cur_shape -> map[0];                        /* Define shape */
  ptr[0] = ptr[1] = ptr[4] = ptr[5] = SHAPE;

  rotate_shape(cur_shape);                          /* Define other orientations */
  no_of_shapes++;

/* ************** Shape 7************* */ /* Backwards 'N' thingy */

  cur_shape -> next_shp = malloc(sizeof(SHP_TYPE));
  cur_shape = cur_shape -> next_shp;                /* Link into list */
  cur_shape -> ht = 2;                              /* Height */
  cur_shape -> wd = 3;                              /* Width */
  cur_shape -> col = WHITE;                     /* Colour */
  cur_shape -> next_shp = NULL;                     /* Seal off end of list */
  for (orient = 0; orient < 4; orient++)            /* Reserve space for maps */
    cur_shape -> map[orient] =
      calloc(cur_shape -> ht * cur_shape -> wd,sizeof(int));

  ptr = cur_shape -> map[0];                        /* Define shape */
  ptr[1] = ptr[2] = ptr[3] = ptr[4] = SHAPE;

  rotate_shape(cur_shape);                          /* Define other orientations */
  no_of_shapes++;

  cur_shape -> next_shp = NULL;                     /* Seal off end of list */
} /* init_shapes */







