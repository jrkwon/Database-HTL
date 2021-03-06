#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <ctype.h>
#include "hanlib.h"
#include "hanpop.h"

char *main_title = ">>>   �� �e  �� ѡ   <<<";

char *main_menu[] = {
  "   1. ������ ���e��ѡ   ",
  "   2. ���e��ѡ�� ����   ",
  "   3. �{ �aá��         "
};

FILE *fp;  /* ���e��ѡ �a�� */

/*
 *  ���e��ѡ �a�a�a���A�� ��, ���i ��á ��a
 */

/*
 * ���� --> ���e��ѡ
 */
int findzipcode(FILE *fp, char *addr, char *zipcode)
{
  int i;
  unsigned char addr_buf[80];
  unsigned char *sido[] = {
   "�ᶉ��", "�w����", "���寡", "�q", "��", "���寡", "�w����","�׺���",
   "��q", "�如", "���e��", "�w�q", "�A����", "������", "�w��", NULL
  };
  long table[] = {
    0L, 56004L, 74152L, 85280L, 100516L, 114036L, 124800L, 159640L,
    173264L, 197080L, 218556L, 238732L, 274560L, 279708L, 294528L, 325780L
  };

  i = 0;
  while(sido[i]) {
    if(!strncmp(addr, sido[i], strlen(sido[i]))) {
      break;
    }
    i++;
  }
  if(sido[i]) {
    fseek(fp, (long)table[i], SEEK_SET);
    fread(addr_buf, 52, 1, fp);
    while(ftell(fp) < table[i+1]) {
      if(!strncmp(addr_buf+6, addr, strlen(addr_buf+6))) {
        addr_buf[6] = NULL;
        strcpy(zipcode, addr_buf);
        return 1;
      }
      else {
        fread(addr_buf, 52, 1, fp);
      }
    }
  }
  return 0;
}

/*
 *  ���e��ѡ --> ����
 */
int findaddr(FILE *fp, char *zipcode, char *addr)
{
  long start, end, middle;
  unsigned char buf[80];
  int i;

  fseek(fp, 0L, SEEK_END);    /*������ �{�a�� ����*/
  start = 0;		   /*�q���i ��᷁ ��q�� �{ ���e�i ����*/
  end = ftell(fp);
  middle = start+((end/52-start/52)/2)*52;

  while(start != middle && end != middle) {
    fseek(fp, middle, SEEK_SET);
    fread(buf, 52, 1, fp);
    if(!strncmp(zipcode, buf, 6)) {
       i = 52-1;
       while(isspace(buf[i]))
         i--;
       buf[i+1] = NULL;
       strcpy(addr, buf+6);
	     return 1;
    }
    else if(strncmp(zipcode, buf, 6) > 0) {
	     start = middle;
	     end = end;
	     middle = start+((end/52-start/52)/2)*52;
    }
	  else if(strncmp(zipcode, buf, 6) < 0) {
	     start = start;
	     end = middle;
	     middle = start+((end/52-start/52)/2)*52;
    }
  }
  return 0;
}

void addr2zip(void)
{
  char addr[80], zipcode[40];
  int i;

  hclrscr();
  hputsxy(20, 5, "         ԍ ������ ���e��ѡ �x�� ԍ");
  hputsxy(20, 7, "��(��) ��(��,��) ��(�s,�e) �a���e ���b�a�A�a");

  addr[0] = NULL;
  while( 1 ) {
    hgotoxy(20, 15);
    hgetln(addr, 60);
    for(i = 1; i <= 80; i++)
      hputsxy(i, 20, " ");
    if(findzipcode(fp, addr, zipcode)) {
      zipcode[6] = zipcode[5], zipcode[5] = zipcode[4], zipcode[4] = zipcode[3];
      zipcode[3] = '-', zipcode[7] = NULL;
      hprintfxy(20, 20, "ԃ ���e��ѡ�e %s�����a", zipcode);
    }
    else {
      hprintfxy(20, 20, "�a�� �����A�� ���e��ѡ�a ���a?");
    }
    hprintfxy(20, 25, "���� �i�a�a ?(Y/n)");
    if(toupper(getchext()) == 'N')
      break;
    hprintfxy(20, 25, "                  ");
  }
  hclrscr();
}

void zip2addr(void)
{
  char addr[80], zipcode[40], temp[40];
  int i;

  hclrscr();
  hputsxy(20, 5, "         ԍ ���e��ѡ�� ���� �x�� ԍ");
  hputsxy(20, 7, "���e��ѡ�i ���b�a�e �a �����i �x�a�����a ");

  zipcode[0] = temp[0] = NULL;
  while( 1 ) {
    strcpy(zipcode, temp);
    hgotoxy(20, 15);
    hgetdata(zipcode, "999-999");
    strcpy(temp, zipcode);
    zipcode[3] = zipcode[4], zipcode[4] = zipcode[5], zipcode[5] = zipcode[6];
    zipcode[6] = NULL;
    for(i = 1; i <= 80; i++)
      hputsxy(i, 20, " ");
    if(findaddr(fp, zipcode, addr)) {
      hprintfxy(20, 20, "�����e %s�����a", addr);
    }
    else {
      hprintfxy(20, 20, "�a�� ���e��ѡ�a ���� ���v�a?");
    }
    hprintfxy(20, 25, "���� �i�a�a ?(Y/n)");
    if(toupper(getchext()) == 'N')
      break;
    hprintfxy(20, 25, "                  ");
  }
  hclrscr();
}

void zip_finder(void)
{
  int ans, isquit = 0;
  void (*func[])(void) = {
    addr2zip, zip2addr
  };

  do {
    ans = popup(hgetmaxax()/2-12, 10, 3, main_menu, main_title, "123", 1);
    if(ans == -1 || ans == 2)
      isquit = 1;
    else
      func[ans]();
  } while(!isquit);
}

void main(int argc, char *argv[])
{
  char file[80];

  strcpy(file, argv[0]);
  *strrchr(file, '\\') = NULL;
  strcat(file, "\\koreazip.dat");
  fp = fopen(file, "r");
  if(!fp) {
    printf("Korean Zip File open Error\n");
    return;
  }

  inithanlib(FIXRESMODE, HANDETECT, HAN8GD2, ENGGD5);
  registeregrfont(EGR1);
  registerhgrfont(HGR1);
  hclrscr();

  zip_finder();

  fclose(fp);
}
