/********** The Son of Tetris Project ************/

/***************** HIGH SCORES ********************/

#include <ctype.h>
#include <io.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sot.h"

#define MARK_CHAR 'à' /* detect unused table */
#define SPACE_STRING "                    "

struct scre
{
  char name[20];
  long scre_val;
} /* scre */;

static char score_space[TABLE_LEN * sizeof(struct scre)] = { MARK_CHAR };

static FILE *prog_file;
const struct scre *score = (void *)&score_space;

void  write_cfg_file(void)
{
  fseek(prog_file, 32L, SEEK_SET);
  fwrite(score, sizeof(struct scre), TABLE_LEN, prog_file);
  fflush(prog_file);
}     /* write_cfg_file */



void  init_score(char *prog_name)
{
  int i;
  char tmp1[40], tmp2[40];

  i = 0;
  while(prog_name[i] != '.')
    i++;
  prog_name[i] = NULL;
  strcpy(tmp1, prog_name);
  sprintf(tmp2, "%s.cfg", tmp1);
  prog_file = fopen(tmp2, "r+b");
  if(!prog_file) {
    prog_file = fopen(tmp2, "w+b");
    if(!prog_file)
      return;
    sprintf(tmp2, "HTETRIS Ver 1.00 Config File\n%c", 0x1a);
    fwrite(tmp2, 32, 1, prog_file);
  }
  else {
    fseek(prog_file, 32L, SEEK_SET);
    fread(score, sizeof(struct scre), TABLE_LEN, prog_file);
  }
  if (score_space[0] == MARK_CHAR)  /* unused score table */
  {
    for (i = 0; i < TABLE_LEN; i++)
    {
      strcpy((score + i)->name,SPACE_STRING);
      (score + i)->scre_val = 0;
    } /* for each table entry */
    write_cfg_file();
  } /* score table need initialising */
}     /* init_score */


void end_score(void)
{
  fclose(prog_file);
} /*  end_score */


void champ_val(char *name, long *score_val, int rank)
{
  strcpy(name,(score + (--rank)) -> name);
  *score_val = (score + rank) ->scre_val;
} /* champ_val */


void  update_score_table(unsigned char *name,long new_score)
{
  int rank;

  while (isspace(*name))   /* kill off non-entered names */
    name++;
  if (! (*name))
    return;
  for (rank = TABLE_LEN - 2; rank >= -1; rank--)
  {
    /* if new score exceeds current rank */
    if (((score + rank)->scre_val < new_score) && (rank >= 0))
    {
      (score + rank + 1)->scre_val = (score + rank)->scre_val;
      strcpy((score + rank + 1) -> name,(score + rank) -> name);
    } /* new_score execeeds current rank */

    else  /* copy new data into rank below */
    {
      (score + rank + 1)->scre_val = new_score;
      strcpy((score + rank + 1) -> name, name);
      break; /* all done */
    }
  } /* for */
  write_cfg_file();
} /* update_score_table */
