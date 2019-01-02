#include <alloc.h>
#include <ctype.h>
#include <dos.h>
#include <stdio.h>
#include "ascii.h"
#include "extkey.h"
#include "hanlib.h"
#include "hanin.h"

void main(void)
{
    int x, y;
    int blank[] = { 217, 49 };
    unsigned char code1, code2;

    inithanlib(FIXRESMODE, HANDETECT, NULL,NULL);
    registerhgrfont(HGR1);
    registerkssfont(KSS1);

    code1 = 217, code2 = 49;
    for(y = 0; y < 12; y++) {
      for(x = 0; x < 32; x+=2, code2++) {
        if(code2 == 127)
          code2 = 145;
        else if(code2 == 161) {
          _hputchxy(blank[0], blank[1], x+1, y+1);
          x+=2;
          if(x == 32)
            x = 0, y++;
          _hputchxy(blank[0], blank[1], x+1, y+1);
        }
        _hputchxy(code1, code2, x+1, y+1);
      }
    }
    pause();
}
