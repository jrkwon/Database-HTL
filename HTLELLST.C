/* ----------------------- htlellst.c ---------------------- */

/*
 *  ·³b–E ·¡Ÿq· ¡¢¢·a¡¦Èá ¸aža¶a­¡ ¡¢¢(¸÷®¤µi)·i  e—e”a
 *  ¸aža¶a­¡ ¡¢¢·i  e—a“e •A ¬÷‰·Ða¡e OKŸi •©aº‰¡,  e´¢
 *  ¸aža¶a­¡ ¬a¸åµA ´ô“e ·¡Ÿq·i ·³bÐa¡e ERRORŸi •©aº…”a.
 */

#include <stdio.h>
#include "htldbms.h"

/*
  count  :   ¡¢¢µA ·¶“e ·¡Ÿq· ˆ•®
  names  :   ¸aža¶a­¡ ¢…¸aµi· ¡¢¢
  list   :   ‰i‰Á ¸aža¶a­¡ ¤åÑ¡Ÿi ”q·i ¸÷®¤µi
*/
int ellist(int count, char *names[], int *list)
{
  char elname [31];
  char msg[] = "¸aža¶a­¡“e ´ô´á¶a";
  char msg2[50];
  int el, el1;
  extern void name_cvt();

  for (el = 0; el < count; el++)  {
    for (el1 = 0; ; el1++)  {
      if (denames [el1] == (char *) 0)  {
        sprintf(msg2, "%s %s", elname, msg);
        disperror(msg2);
        return ERROR;
      }
      name_cvt(elname, names[el]);
      if (strcmp(elname, denames [el1]) == 0)
        break;
    }
    *list++ = el1 + 1;
  }
  *list = 0;
  return OK;
}

