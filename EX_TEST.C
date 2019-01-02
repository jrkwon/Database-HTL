#include <alloc.h>
#include <ctype.h>
#include <dos.h>
#include <stdio.h>
#include "ascii.h"
#include "extkey.h"
#include "hanlib.h"
#include "hanin.h"

/*
 *  TG Graphic Character
 *
 *  212, 128 ~ 254, 127
 */




/*
 *  KS Graphic Character
 *
 *  blank is 217:49
 *
 *  217:49 ~ 126, 145 ~ 160, blank, blank, 161 ~ 254, blank
 *  218:49 ~ 126, 145 ~ 160, blank, blank, 161 ~ 254, blank
 *  ....
 *  ....
 *  222:49 ~ 126, 145 ~ 160, blank, blank, 161 ~ 254, blank
 */
int special_char(void)
{
  int x, y, bx, by, isquit, flag, ans;
  int blank[] = { 217, 49 };
  unsigned int key, int_bcode;
  unsigned char code1, code2, bcode1, bcode2;
  bool onoff;

  inithanlib(FIXRESMODE, HANDETECT, NULL,NULL);
  registerhgrfont(HGR1);
  registerkssfont(KSS1);


  isquit = bx = by = 0;
  ans = -1, flag = 1;
  code1 = 217;
  do {

    if( flag) {
      code2 = 49;
      _hputchxy(blank[0], blank[1], 1, 1);

      for(y = 0; y < 12; y++) {
        for(x = 0; x < 32; x+=2, code2++) {
          if(x == 0 && y == 0)
            x = 2;
          if(code2 == 127)
            code2 = 145;
          else if(code2 == 161) {
            _hputchxy(blank[0], blank[1], x+1, y+1);
            x+=2;
            if(x == 32)
              x = 0, y++;
            _hputchxy(blank[0], blank[1], x+1, y+1);
            x+=2;
            if(x == 32)
              x = 0, y++;
          }
          else if(code2 == 255) {
            _hputchxy(blank[0], blank[1], x+1, y+1);
            break;
          }
          _hputchxy(code1, code2, x+1, y+1);
        }
      }
    }
    if((bx == 0 && by == 0) || (bx == 30 && by == 11)
        || (bx == 30 && by == 5) || (bx == 0 && by == 6))
      bcode1 = blank[0], bcode2 = blank[1];
    else {
      bcode1 = code1, int_bcode = bx/2 + by*16 + 48;
      int_bcode = (int_bcode >= 127) ? int_bcode + 18 : int_bcode;
      bcode2 = (int_bcode > 160) ? int_bcode - 2 : int_bcode;
    }
    onoff = isreverse();
    if(!onoff)
      hsetreverse(ON);
    _hputchxy(bcode1, bcode2, bx+1, by+1);
    key = getchext();
    hsetreverse(onoff);
    _hputchxy(bcode1, bcode2, bx+1, by+1);
    flag = 0;
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
          if(by < 11)
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
        if(by < 11)
          by++;
        break;
      case PGUP :
        if(code1 > 217)
          code1--;
        else
          code1 = 222;
        flag = 1;
        break;
      case PGDN :
        if(code1 < 222)
          code1++;
        else
          code1 = 217;
        flag = 1;
        break;
      case ESC :
        isquit = 1;
        break;
      case '\r' :
        ans = ((bcode1 << 8) & 0xFF00) + (bcode2);
        isquit = 1;
        break;
    }
  } while (!isquit);
  return ans;
}

void main(void)
{
  int ans;

  ans = special_char();
  if(ans != -1)
    hprintfxy(1, 20, "%c%c", (ans >> 8) & 0x00FF, ans & 0x00FF);
  pause();
}
