/*
 *  �a�a�i�A ϩ�a�e ���� �q���i
 *
 *  HANDUTIL.C
 *
 *  92. 7. 10
 *
 */

#include <stdio.h>
#include <string.h>
#include "hanout.h"
#include "hanwindw.h"
#include "ascii.h"
#include "handbms.h"

/*
 *  �a�a�i�i ��������ǥ�a.  �a�a�a���� �����a���i�i ���� �e�i�a
 *  ���� ���� �a�a�i�e ���� �������a.
 */

void dbinit(void)
{
  int f = 0, ans;
  char fname [13], msg[80], str[] = "�a�� 12345678.123�i �������a�e ��..";

  ans = disp_cmsg("���e �a�a�a���i �������s���a");
  if(ans == ESC)
    return;

  wopen(hgetmaxax()/2-strlen(str)-1, hgetmaxay()-2, -(strlen(str)+4), -3);
  while (dbfiles [f])  {
    sprintf(fname, "%.8s.dat", dbfiles [f]);
    file_create(fname, rlen(f));
    sprintf(msg, "�a�� %12s�i �������a�e ��..", strupr(fname));
    wcenter(2, msg);
     build_index("", f);
    f++;
  }
  wclose();
}

/*
 *  ���b�E ���q�� �����a������ �a�a�a�� ����(�������i)�i �e�e�a
 *  �a�a�a�� �����i �e�a�e �A �����a�e OK�i ���a����, �e��
 *  �a�a�a�� �a��A ���e ���q�i ���b�a�e ERROR�i ���a���a.
 */

/*
 * count  :   �����A ���e ���q�� ����
 * names  :   �a�a�a�� ���a�i�� ����
 * list   :   �i�� �a�a�a�� ��ѡ�i �q�i �������i
 */
int ellist(int count, char *names[], int *list)
{
  char elname [31];
  char msg[] = "�a�a�a���e ����a";
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


