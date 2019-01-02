#include <stdio.h>
#include <string.h>
/*
 *  ¶Íe¤åÑ¡ ¸ažaÌa·©µA¬á ¯¡, •¡¥i ¶áÃ¡
 *  µ ) ¬á¶‰¯¡“e 0¤a·¡ËaµA¬á ¯¡¸b
 *       ¦¬e¯¡“e ???¤a·¡Ëa...
 */

void main(void)
{
  FILE *fp;
  int i;
  unsigned char buf[80];
  unsigned char *sido[] = {
    "¬á¶‰¯¡", "¦¬e¯¡", "”Š¯¡", "·¥Àå¯¡", "‰×º¯¡", "”¸å¯¡", "ˆw¶¥•¡",
    "‰w‹¡•¡", "‰wq", "‰w¦‚", "¸åq", "¸å¦‚", "¹Aº•¡", "Â—q", "Â—¦‚", NULL
  };

  fp = fopen("koreazip.dat", "r");
  if(!fp) {
    printf("File open error\n");
    return;
  }
  i = 0;
  while(!feof(fp)) {
    if(sido[i] == NULL)
      break;
    fread(buf, 52, 1, fp);
    buf[52] = NULL;
    if(!strncmp(buf+6, sido[i], strlen(sido[i]))) {
      printf("%s : %ld\n", buf, ftell(fp)-52L);
      fseek(fp, 0L, SEEK_SET);
      i++;
    }
  }
  fclose(fp);
}