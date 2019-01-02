#include <stdio.h>
#include "hanlib.h"
#include "hanerro2.h"
#include "hanwindw.h"
#include "ascii.h"
#include "extkey.h"

/*
 *  •¡¶‘ i Ìa·©· Š¹¡
 *
 *  Ðeº‰· ‹©·¡“e ÂA” 60Äe
 *  Ðe ÑÁ¡eµA¬á º‰· ®“e 15º‰
 *  ÂA” ¡e· ®“e 10¡e
 */

#define  MAXLEN     60+1
#define  MAXLINE    15
#define  MAXPAGE    10

FILE *fhelp;
char help_buf[MAXLINE*MAXPAGE][MAXLEN+1];
int max_page_no;

bool load_help_file(char *filename)
{
  int i, j, flag;

  flag = 1;
  fhelp = fopen(filename, "r");
  if(!fhelp) {
    disperror("•¡¶‘ i Ìa·©·¡ ´ô¯s“¡”a");
    return false;
  }
  for(i = 0; i < MAXPAGE && flag; i++) {
    for(j = 0; j < MAXLINE; j++) {
      fgets(help_buf[i*MAXLINE+j], MAXLEN, fhelp);
      if(feof(fhelp)) {
        flag = 0;
        break;
      }
    }
  }
  max_page_no = i > 0 ? i-1 : i;
  fclose(fhelp);
  return true;
}

void disp_help(int page)
{
  int i;

  hclrscr();
  for(i = 0; i < MAXLINE; i++)
    wputs(i+1, help_buf[i+page*MAXLINE]);
}

void help_manager(char *filename)
{
  static int page;
  int isquit, flag;

  isquit = 0, flag = 1;
  if(!wopen(10, 7, MAXLEN+2, MAXLINE))
    return;

  wtitle("ÐeËi Íe»³‹¡ •¡¶‘ i");

  if(load_help_file(filename)) {
    do {
      if(flag)
        disp_help(page);
      switch(getchext()) {
        case LEFTARROW : case UPARROW : case PGUP :
          if(page)
            page--, flag = 1;
          else
            flag = 0;
          break;
        case RIGHTARROW : case DOWNARROW : case PGDN :
          if(page < max_page_no)
            page++, flag = 1;
          else
            flag = 0;
          break;
        case ESC :
          isquit = 1;
          break;
        default :
          flag = 0;
          break;
      }
    } while( !isquit);
  }
  wclose();
}

void main (void)
{
  inithanlib(FIXRESMODE, HANDETECT, HAN8GD2, DEFENGFONT);
  registeregrfont(EGR1);
  registerhgrfont(HGR1);
  registerkssfont(KSS1);


  hputs("12345678901234567890123456789012345678901234567890123456789012345678901234567890");
  help_manager("hantle.hlp");
  pause();
}