/* ----------------------- htlellst.c ---------------------- */

/*
 *  ���b�E ���q�� �����a������ �a�a�a�� ����(�������i)�i �e�e�a
 *  �a�a�a�� �����i �e�a�e �A �����a�e OK�i ���a����, �e��
 *  �a�a�a�� �a��A ���e ���q�i ���b�a�e ERROR�i ���a���a.
 */

#include <stdio.h>
#include "htldbms.h"

/*
  count  :   �����A ���e ���q�� ����
  names  :   �a�a�a�� ���a�i�� ����
  list   :   �i�� �a�a�a�� ��ѡ�i �q�i �������i
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