#include <stdio.h>
#include <stdarg.h>
#include <bios.h>
#include "hanlib.h"
#include "hanerro2.h"

int prn_port = 0;   /* 0 = lpt1, 1 = lpt2 ... */
int prn_OK = true;

bool isOKprn(void)
{
  int res;

  res = biosprint(2, '*', prn_port) & 0x29;
  prn_OK = (res) ? false : true;
  return prn_OK;
}

bool lputc(unsigned char c)
{
  int res;

  while( 1 ) {
    if(!isOKprn()) {
      if(retry("ÏaŸ¥Èáˆa Œá¹a ·¶ˆáa, µe‰i–A´á·¶»¡ ´g¯s“¡”a", 0) == 0)
        break;
      else
        return false;
    }
    else
      break;
  }
  while( 1 ) {
    res = biosprint(2, c, prn_port) & 0x29;
    if(res) {
      while( 1 ) {
        if(retry("·¥­áÐi ®ˆa ´ô´á¶a", 0) == 0)
          break;
        else
          return false;
      }
    }
    if(res == 0) {  /* ¬÷‰· */
      biosprint(0, c, prn_port);
      return true;
    }
  }
}

bool lputs(unsigned char *s)
{
  if(!prn_OK)
    return false;
  while( 1 ) {
    if(!isOKprn()) {
      if(retry("ÏaŸ¥Èáˆa Œá¹a ·¶ˆáa, µe‰i–A´á·¶»¡ ´g¯s“¡”a", 0) == 0)
        break;
      else
        return false;
    }
    else
      break;
  }
  while(*s) {
    if(!lputc(*s++))
      return false;
  }
  return true;
}

void lprintf(va_list arg_list, ...)
{
  va_list arg_ptr;
  char *format;
  char prtbuf[300];

  va_start(arg_ptr, arg_list);
  format = arg_list;
  vsprintf(prtbuf, format, arg_ptr);
  lputs(prtbuf);
}
