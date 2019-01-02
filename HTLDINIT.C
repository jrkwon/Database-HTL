/* ------------------------- htldinit.c -------------------------- */

/*   ¸ažaËi·i Á¡‹¡ÑÁ¯¡Ç¥”a.  ¸ažaÌa·©‰Á ¬‚·¥Ìa·©—i·i ¬¡  e—i¡a
    ·¡£¡ ·¶”å ¸aža—i·e ¡¡– »¡¶¡»¥”a.                           */

#include <stdio.h>
#include <string.h>
#include "htlout.h"
#include "htlwin.h"
#include "htlkey.h"
#include "htldbms.h"

void dbinit(void)
{
  int f = 0;
  char fname [13], msg[80], str[] = "Ìa·© 12345678.123·i Á¡‹¡ÑÁÐa“e º—..";

  if(wopen(getmaxix()/2-strlen(str)/2-1, getmaxiy()/2-1, strlen(str)+2, 1, L1T1R1B1, BLUE, WHITE, GREEN) != SUCCESS)
    return;
  wcenter(0, "¡¡—e ¸ažaÌa·©·i Á¡‹¡ÑÁÐs“¡”a");
  if(getkey() == ESCKEY) {
    wclose();
    return;
  }
  while (dbfiles [f])  {
    sprintf(fname, "%.8s.dat", dbfiles [f]);
    file_create(fname, rlen(f));
    sprintf(msg, "Ìa·© %12sŸi Á¡‹¡ÑÁÐa“e º—..", strupr(fname));
    wcenter(0, msg);
     build_index("", f);
    f++;
  }
  wclose();
}
