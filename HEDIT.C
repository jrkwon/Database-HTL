#include "hanlib.h"
#include "handetect.h"
#include "hancoord.h"
#include "hanwindw.h"
#include "ascii.h"
#include "extkey.h"
#include "hanin.h"
#include "hanerro2.h"

#include "hanedit.h"

#include "khinput.h"

static int fcolor, bcolor;
void init_sys_color(void)
{
  int isHGC;

  isHGC = _detecthgc();
  if(!isHGC) {
    fcolor = WHITE, bcolor = LIGHTBLUE;
  }
  else {
    fcolor = WHITE, bcolor = BLACK;
  }
}

void main()
{
  inithanlib(FIXRESMODE, HANDETECT, HAN8GD2, ENGGD5);
  registeregrfont(EGR1);
  registerhgrfont(HGR1);
  registerkssfont(KSS1);
  loadhanjafont("hanja.fnt");

  init_sys_color();
  inithwindow(fcolor, bcolor);

  hclrscr();
  if(!init_editor("UNTITLED.TXT", "HANTLE.HLP", 0L, HANKBD2, true))
    return;
  if(!editor(2, 3, 78, 21, 1)) {
    hputs("¹ÁÎa »¡¸÷ ¸i¡µ!!");
    pause();
  }
}

