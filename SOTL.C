/********** The Son of Tetris Project ************/

/************  get LEVEL value etc. ***********/

#include <conio.h>
#include <ctype.h>
#include <stdio.h>
#include <bios.h>
#include "hanlib.h"
#include "hanin.h"
#include "hanwindw.h"
#include "hanerro2.h"
#include "sot.h"

#define LEVEL_COL 19
#define LEVEL_ROW 14
#define LEVEL_WIDTH 46
#define LEVEL_DEPTH 3

#define AG_COL 19
#define AG_ROW 18
#define AG_WIDTH 44
#define AG_DEPTH 3

#define CH_COL 19
#define CH_ROW 7
#define CH_WIDTH 44
#define CH_DEPTH 8

#define PG_COL 21
#define PG_ROW 10
#define PG_WIDTH 40
#define PG_DEPTH 6

#define NN_COL 19
#define NN_ROW 10
#define NN_WIDTH 44
#define NN_DEPTH 6

void sting(void)
{
	char *mus="t80o4mll64c#mnl16deco3l8al16bmsl8g\
		   mll64c#mnl16deco2l8al16bl8msgmn\
		   o2mll64c#mnl16deco1l8al16baa-\
		   msl8gmnp4o3l8go2l16mldmnd#\
		   mlo2l16emsl8o3cmlo2l16eo3msl8c\
		   mno2l16eo3l4c.l16o4cdd#\
		   ecdl8el16o3bo4msl8d\
		   mncl16o3mlego4msl8cmlo2l16dmnd#\
		   mlo2l16emsl8o3cmlo2l16eo3msl8c\
		   mlo2l16eo3l4c.l16mlamng\
		   f#ao4cl8el16dco3a\
		   o4l4d.o1l16mldmnd#\
		   mlo2l16emsl8o3cmlo2l16eo3msl8c\
		   mno2l16eo3l4c.l16o4cdd#\
		   ecdl8el16o3bo4msl8d\
		   mncl16o3mlego4msl8cmnl16cd\
		   ecdl8el16cdc\
		   ecdl8el16cdc\
		   ecdl8el16o3bo4l8msdmn\
		   l5mlc.mnl16o3eff#\
		   o3l8msgmnl16al8gl16eff#\
		   msl8gmnl16al8gl16mseco2g\
		   mll16abo3cdedcd\
		   mlo2gmlo3efgagemnf\
		   l8msgmnl16al8gl16eff#\
		   l8msgmnl16al8gl16gaa#\
		   bl8bl16l8bl16af#d\
		   l5mlg.mnl16eff#\
		   msl8gmnl16al8gl16eff#\
		   l8msgmnl16al8gl16mleco3mng\
		   mlabo4cdedcd\
		   msl8mlcl16egmso5co4mngf#g\
		   mso5l8co4mnl16ao5l8cl16o4ao5co4a\
		   go5cel8gl16eco4g\
		   msl8ao5cmnl16el8d\
		   l4c..l16o4eff#\
		   msl8gmnl16al8gl16eff#\
		   l8gl16al8gl16mseco3g\
		   mnabo4cdedcd\
		   o3go4efgagef\
		   msl8gmnl16al8gl16eff#\
		   l8gl16al8gl16gaa#\
		   bl8bbl16af#d\
		   mll64gagagagagagagagagl16mneff#\
		   l8msgl16al8gl16eff#\
		   l8gl16al8gl16mseco3g\
		   mnabo4cdedcd\
		   msl8cl16mlego5mnco4gf#g\
		   o5msl8co4mnl16amlo5l8cl16o4ao5\
		   co4ago5cel8gl16eco4g\
		   l8msao5cmnl16el8dl16mlc\
		   l4cl8c";

	play(mus);
}

int get_level(void)
{
  unsigned result = 5;
  int char_in;

  if(!wopen(LEVEL_COL, LEVEL_ROW, LEVEL_WIDTH, LEVEL_DEPTH)) {
    memerror();
  }
  hputsxy(2, 2, " ´á“a ”e‰µA¬á ¯¡¸bÐiŒa¶a ? (0-9) [5] >  \n");

  while(1) {
    char_in = get_key();
    if(isdigit(char_in))
      break;
    if (char_in == ESC) {
      wclose();
      return -1;
    }
    else {
      char_in = '5';
      break;
    }
  }
  wclose();
  result = char_in - '0';
  return(result);
} /* get_level */

void  get_new_name(unsigned char *name)
{
  if(!wopen(NN_COL, NN_ROW, NN_WIDTH, NN_DEPTH)) {
    memerror();
  }
  hputs("\n\n\n ·¡Ÿq·i ý´áº­A¶a > ");
  wcenter(2, "Â‚ÐaÐs“¡”a. 5 —w´eµA —i´ö¯s“¡”a.\n");

  hgotoxy(22, 4);
  sting();
  strnset(name, ' ', 19);
  hgetln(name, 19);
  wclose();
}  /* get_new_name */



unsigned  get_another_go(long new_score) /* Dialogue between games */
{
  auto unsigned result = TRUE;
  unsigned char name[20];
  long score;
  int rank;

  champ_val(name, &score, TABLE_LEN);  /* get current bottom of table */

  if (score < new_score) {
    while(bioskey(1))
      bioskey(0);
    get_new_name(name);
    update_score_table(name, new_score);
  }

  if(!wopen(CH_COL, CH_ROW, CH_WIDTH, CH_DEPTH)) {
    memerror();
  }
  wcenter(1, "ÉAËaŸ¡¯a ¸i Ða“e ¬aœq—i");

  for (rank = 1; rank <= TABLE_LEN; rank++) {
    champ_val(name, &score, rank);
    hgotoxy(4,rank+2);
    if (score)
      hprintf("%d. %-20s  %8ld", rank, name, score);
    else
      break;
  } /* for */

  if(!wopen(AG_COL, AG_ROW, AG_WIDTH, AG_DEPTH)) {
    memerror();
    wclose();
  }
  hputs("\n ÉAËaŸ¡¯aŸi ‰­¢ Ða¯¡‰V¯s“¡Œa ? (Y/N)");

  if(toupper(get_key()) == 'N') {
    result = FALSE;
  }
  wclose();
  wclose();
  return(result);
} /* get_another_go */

unsigned pause_game(void)
{
  int result = FALSE;

  if(!wopen(PG_COL, PG_ROW, PG_WIDTH, PG_DEPTH)) {
    memerror();
  }
  hputs("\n    ¸qŒe .....\n\n");
  hputs("    ‰­¢Ðaa¡e ´a¢ˆáa ’Ÿa¯¡‰¡, \n");
  hputs("    {a¡e ESCŸi ’Ÿa­A¶a.");
  if(get_key() == ESC)
    result = TRUE;
  wclose();
  return(result);
} /* pause_game */

