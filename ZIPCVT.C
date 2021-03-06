#include <stdio.h>
#include <ctype.h>
#include <string.h>
/*
 *  ----------------------------
 *  ���� �a�a�� ����
 *  ----------------------------
 *  ���e��ѡ   : 7 (333-333)
 *  ��(��)     : 8
 *  ��(��,��)  : 8
 *  �a�ỡ     : 28
 *  ----------------------------
 *             : 52
 */

/*
 *  ----------------------------
 *  �e�i�ứ �a�a�� ����
 *  ----------------------------
 *  ���e��ѡ   : 6 (333333)
 *  ��(��)     : 8 + (����) = 9
 *  ��(��,��)  : 8 + (����) = 9
 *  �a�ỡ     : 28
 *  ----------------------------
 *             : 52
 */

void convert_new_zip(FILE *fold, FILE *fnew)
{
  unsigned char zip[8], si[9], gu[9], dong[30], tmpbuf[80], *ptr;
  static unsigned char buf[80];

  while(!feof(fold)) {
    fscanf(fold,"%7s%8s%8s%29s", zip, si, gu, dong);
    /* zip�A�� '-'�i ���a */
    zip[3] = zip[4], zip[4] = zip[5], zip[5] = zip[6], zip[6] = NULL;

    /*
     * si, gu, dong�A�� �����i ������,
     * �bӡ dong�A�� '('��ҁ�e �i�a�៥�a
     */
    ptr = strchr(dong, '(');
    if(ptr)
      *ptr = NULL;

    sprintf(tmpbuf, "%s %s %s", si, gu, dong);
    if(strlen(dong) > 28) {
      printf("28�a���a �e�� ���a %s\n", dong);
      return;
    }

    sprintf(buf, "%6s%s", zip, tmpbuf);
    fwrite(buf, 52, 1, fnew);
  }
}

void main(void)
{
  FILE *fold, *fnew;

  fold = fopen("zip.dat", "r");
  fnew = fopen("koreazip.dat", "w");
  if(!fold || !fnew) {
    printf("File open Error\n");
    return;
  }

  convert_new_zip(fold, fnew);

  fclose(fold);
  fclose(fnew);
}
