#include "hanlib.h"
#include "ascii.h"
#include "extkey.h"
#include "hanwindw.h"
#include "hanerro2.h"

/*
 *  TG Graphic Character
 *
 *  212:128 ~ 212:255
 */
static unsigned int get_tg_graphic_char(void)
{
  int x, y, isquit;
  unsigned int key, ans;
  int code1, code2;
  static int bx = 0, by = 0;
  static unsigned int bcode1 = 212, bcode2 = 218;
  bool onoff;

  ans = -1, isquit = 0;

  if(wopen(hgetmaxax()/2-16, hgetmaxay()/2-4, 32, 8))
    wtitle("ÙÃ TG ‹aŸ± ¢…¸a ·³b ÙÃ");
  else
    return -1;

  code1 = 212, code2 = 128;
  for(y = 0; y < 8; y++) {
    for(x = 0; x < 32; x+=2, code2++) {
      _hputchxy(code1, code2, x+1, y+1);
    }
  }

  do {
    bcode1 = code1, bcode2 = bx/2 + by*16 + 128;
    onoff = isreverse();
    if(!onoff)
      hsetreverse(ON);
    _hputchxy(bcode1, bcode2, bx+1, by+1);
    key = getchext();
    hsetreverse(onoff);
    _hputchxy(bcode1, bcode2, bx+1, by+1);
    switch(key) {
      case LEFTARROW :
        if(bx == 0) {
          if(by > 0)
            by--, bx = 30;
        }
        else
          bx-=2;
        break;
      case RIGHTARROW :
        if(bx == 30) {
          if(by < 7)
            by++, bx = 0;
        }
        else
          bx+=2;
        break;
      case UPARROW :
        if(by > 0)
          by--;
        break;
      case DOWNARROW :
        if(by < 7)
          by++;
        break;
      case ESC :
        isquit = 1;
        break;
      case '\r' :
        ans = ((bcode1 << 8) & 0xFF00) + (bcode2);
        isquit = 1;
        break;
    }
  } while(!isquit);
  wclose();
  return ans;
}


void main(void)
{
  int ans;

  inithanlib(FIXRESMODE, HANDETECT, DEFHANFONT, DEFENGFONT);
  registerhgrfont(HGR1);
  registeregrfont(EGR1);

  ans = get_tg_graphic_char();
  if(ans != -1)
    _hputchxy((ans >> 8) & 0x00FF, ans & 0x00FF, 1, 1);
  pause();
  ans = get_tg_graphic_char();
  if(ans != -1)
    _hputchxy((ans >> 8) & 0x00FF, ans & 0x00FF, 1, 1);
  pause();
}

