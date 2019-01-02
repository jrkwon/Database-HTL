#include <stdio.h>
#include <ctype.h>
#include <string.h>
/*
 *  ----------------------------
 *  ¶¥œ ¸aža· Š¹¡
 *  ----------------------------
 *  ¶Íe¤åÑ¡   : 7 (333-333)
 *  ¯¡(•¡)     : 8
 *  Š(¯¡,Š…)  : 8
 *  a á»¡     : 28
 *  ----------------------------
 *             : 52
 */

/*
 *  ----------------------------
 *   e—i´á»© ¸aža· Š¹¡
 *  ----------------------------
 *  ¶Íe¤åÑ¡   : 6 (333333)
 *  ¯¡(•¡)     : 8 + (‰·¤‚) = 9
 *  Š(¯¡,Š…)  : 8 + (‰·¤‚) = 9
 *  a á»¡     : 28
 *  ----------------------------
 *             : 52
 */

void convert_new_zip(FILE *fold, FILE *fnew)
{
  unsigned char zip[8], si[9], gu[9], dong[30], tmpbuf[80], *ptr;
  static unsigned char buf[80];

  while(!feof(fold)) {
    fscanf(fold,"%7s%8s%8s%29s", zip, si, gu, dong);
    /* zipµA¬á '-'Ÿi ¨…”a */
    zip[3] = zip[4], zip[4] = zip[5], zip[5] = zip[6], zip[6] = NULL;

    /*
     * si, gu, dongµA¬á ‰·¤‚·i ´ô´‰¡,
     * ËbÓ¡ dongµA¬á '('·¡Ò“e ¸iœa¤áŸ¥”a
     */
    ptr = strchr(dong, '(');
    if(ptr)
      *ptr = NULL;

    sprintf(tmpbuf, "%s %s %s", si, gu, dong);
    if(strlen(dong) > 28) {
      printf("28¸a¥¡”a Çeˆá ·¶”a %s\n", dong);
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