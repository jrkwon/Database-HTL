#include <stdio.h>
#include <string.h>
/*
 *  ���e��ѡ �a�a�a���A�� ��, ���i ��á
 *  �� ) �ᶉ���e 0�a���a�A�� ���b
 *       ���e���e ???�a���a...
 */

void main(void)
{
  FILE *fp;
  int i;
  unsigned char buf[80];
  unsigned char *sido[] = {
    "�ᶉ��", "���e��", "������", "���寡", "�׺���", "���寡", "�w����",
    "�w����", "�w�q", "�w��", "��q", "�如", "�A����", "�q", "��", NULL
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