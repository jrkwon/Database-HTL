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
  unsigned char zip[8], si[30], gu[30], dong1[30], dong2[30], tmpbuf[80], *ptr;
  static unsigned char buf[80];
  int cnt;

  cnt = 0;
  while(!feof(fold)) {
    printf("Rec no. %d\r", cnt++);
    fread(buf, 62, 1, fold);
    sscanf(buf,"%7s%s%s%s%s", zip, si, gu, dong1, dong2);

    /* zipµA¬á '-'Ÿi ¨…”a */
    zip[3] = zip[4], zip[4] = zip[5], zip[5] = zip[6], zip[6] = NULL;

    /*
     * si, gu, dongµA¬á ‰·¤‚·i ´ô´‰¡,
     * ËbÓ¡ dongµA¬á '('·¡Ò“e ¸iœa¤áŸ¥”a
     */
    ptr = strchr(dong1, '(');
    if(ptr)
      *ptr = NULL;
    ptr = strchr(dong2, '(');
    if(ptr)
      *ptr = NULL;

    if(dong2[0])
      sprintf(tmpbuf, "%s %s %s %s", si, gu, dong1, dong2);
    else
      sprintf(tmpbuf, "%s %s %s", si, gu, dong1);
    dong2[0] = NULL;
    if(strlen(dong1) > 28) {
      printf("28¸a¥¡”a Çeˆá ·¶”a %s\n", dong1);
      return;
    }
    if(strlen(dong2) > 28) {
      printf("28¸a¥¡”a Çeˆá ·¶”a %s\n", dong2);
      return;
    }

    sprintf(buf, "%6s%s", zip, tmpbuf);
    fwrite(buf, 52, 1, fnew);
  }
}

void main(void)
{
  FILE *fold, *fnew;

  fold = fopen("koreazip.dbf", "rb");
  fnew = fopen("koreazip.dat", "wb");
  if(!fold || !fnew) {
    printf("File open Error\n");
    return;
  }

  printf("+ File Converter for DBF Zip File -> Ojarkyo Zip Data File\n");
  fseek(fold, 194L,SEEK_SET);
  convert_new_zip(fold, fnew);

  fclose(fold);
  fclose(fnew);
  printf("Complete.....\n");
}


