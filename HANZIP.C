/*
 *  ¶Íe¤åÑ¡ ¸ažaÌa·©µA¬á ¯¡, •¡¥i ¶áÃ¡ ˆñ¬a
 */

#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <ctype.h>
#include "hanzip.h"

/*
 * º­¡ --> ¶Íe¤åÑ¡
 */
int findzipcode(char *addr, char *zipcode)
{
  FILE *fp;
  int i;
  unsigned char addr_buf[80];
  unsigned char *sido[] = {
   "¬á¶‰¯¡", "ˆw¶¥•¡", "”¸å¯¡", "Â—q", "Â—¦‚", "·¥Àå¯¡", "‰w‹¡•¡","‰×º¯¡",
   "¸åq", "¸å¦‚", "¦¬e¯¡", "‰wq", "¹Aº•¡", "”Š¯¡", "‰w¦‚", NULL
  };
  long table[] = {
    0L, 56004L, 74152L, 85280L, 100516L, 114036L, 124800L, 159640L,
    173264L, 197080L, 218556L, 238732L, 274560L, 279708L, 294528L, 325780L
  };

  fp = fopen("koreazip.dat", "rb");
  if(!fp)
    return 0;
  i = 0;
  while(sido[i]) {
    if(!strncmp(addr, sido[i], strlen(sido[i]))) {
      break;
    }
    i++;
  }
  if(sido[i]) {
    fseek(fp, (long)table[i], SEEK_SET);
    fread(addr_buf, 52, 1, fp);
    while(ftell(fp) < table[i+1]) {
      if(!strncmp(addr_buf+6, addr, strlen(addr_buf+6))) {
        addr_buf[6] = NULL;
        strcpy(zipcode, addr_buf);
        fclose(fp);
        return 1;
      }
      else {
        fread(addr_buf, 52, 1, fp);
      }
    }
  }
  fclose(fp);
  return 0;
}

/*
 *  ¶Íe¤åÑ¡ --> º­¡
 */
int findaddr(char *zipcode, char *addr)
{
  long start, end, middle;
  unsigned char buf[80];
  FILE *fp;
  int i;

  fp = fopen("koreazip.dat", "rb");
  if(!fp)
    return 0;
  fseek(fp, 0L, SEEK_END);    /*ÑÁ·©· {·a¡ ·¡•·*/
  start = 0;		   /*Èq¬‚Ði ¤ñ¶á· Àá·q‰Á { º—ˆe·i »¡¸÷*/
  end = ftell(fp);
  middle = start+((end/52-start/52)/2)*52;

  while(start != middle && end != middle) {
    fseek(fp, middle, SEEK_SET);
    fread(buf, 52, 1, fp);
    if(!strncmp(zipcode, buf, 6)) {
       i = 52-1;
       while(isspace(buf[i]))
         i--;
       buf[i+1] = NULL;
       strcpy(addr, buf+6);
	     return 1;
    }
    else if(strncmp(zipcode, buf, 6) > 0) {
	     start = middle;
	     end = end;
	     middle = start+((end/52-start/52)/2)*52;
    }
	  else if(strncmp(zipcode, buf, 6) < 0) {
	     start = start;
	     end = middle;
	     middle = start+((end/52-start/52)/2)*52;
    }
  }
  fclose(fp);
  return 0;
}

