/*
 *  ¸ažaËiµA Ï©¶aÐe ¦­¢ Ðq®—i
 *
 *  HANDUTIL.C
 *
 *  92. 7. 10
 *
 */

#include <stdio.h>
#include <string.h>
#include "hanout.h"
#include "hancoord.h"
#include "hanwindw.h"
#include "ascii.h"
#include "handbms.h"

/*
 *  ¸ažaËi·i Á¡‹¡ÑÁ¯¡Ç¥”a.  ¸ažaÌa·©‰Á ¬‚·¥Ìa·©—i·i ¬¡  e—i¡a
 *  ·¡£¡ ·¶”å ¸aža—i·e ¡¡– »¡¶¡»¥”a.
 */

int db_init(void)
{
  int f = 0, ans;
  char fname [13], msg[80], str[] = "Ìa·© 12345678.123·i Á¡‹¡ÑÁÐa“e º—..";

  ans = disp_cmsg(" ¡¡—e ¸ažaÌa·©·i ¬¡  e—s“¡”a. ´a¢ˆáa ’Ÿa­A¶a ");
  if(ans == ESC)
    return DBERROR;

  while (dbfiles [f])  {
    sprintf(fname, "%.8s.dat", dbfiles [f]);
    if(file_create(fname, rlen(f)) == DBERROR) {
      disperror(" Ìa·©·i  e—i ® ´ô¯s“¡”a. ");
      return DBERROR;
    }
    sprintf(msg, " Ìa·© %sŸi Á¡‹¡ÑÁÐa“e º—..", strupr(fname));
    disp_cmsg_sec(msg, 1);
    build_index("", f);
    f++;
  }
  return DBOK;
}

void beep(void)
{
  sound(100);
  delay(1);
  sound(500);
  delay(50);
  nosound();
}

/*  ·³b–E ·¡Ÿq· ¡¢¢·a¡¦Èá ¸aža¶a­¡ ¡¢¢(¸÷®¤µi)·i  e—e”a
 *  ¸aža¶a­¡ ¡¢¢·i  e—a“e •A ¬÷‰·Ða¡e OKŸi •©aº‰¡,  e´¢
 *  ¸aža¶a­¡ ¬a¸åµA ´ô“e ·¡Ÿq·i ·³bÐa¡e ERRORŸi •©aº…”a.
 */

/*
 * count  :   ¡¢¢µA ·¶“e ·¡Ÿq· ˆ•®
 * names  :   ¸aža¶a­¡ ¢…¸aµi· ¡¢¢
 * list   :   ‰i‰Á ¸aža¶a­¡ ¤åÑ¡Ÿi ”q·i ¸÷®¤µi
 */
/*int ellist(int count, char *names[], int *list)
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
        return DBERROR;
      }
      name_cvt(elname, names[el]);
      if (strcmp(elname, denames [el1]) == 0)
        break;
    }
    *list++ = el1 + 1;
  }
  *list = 0;
  return DBOK;
}
  */

