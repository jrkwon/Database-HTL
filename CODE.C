#include <stdio.h>

void main(void)
{
  FILE *fp;
  unsigned char buf[81];
  int i;

  fp = fopen("code.dat", "r");
  if(!fp) {
    printf("open error\n");
    return;
  }
  do {
    fgets(buf, 80, fp);
    if(feof(fp))
      break;
    printf("%s%d\n", buf, buf[0]);
    for(i = 0; buf[i] != '\n'; i+=2)
      printf("%3d ", buf[i+1]);
    printf("\n\n");
  } while (1);
  fclose(fp);
}