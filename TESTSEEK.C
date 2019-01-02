#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <ctype.h>
#include "hanlib.h>
#include "hanpop.h"

char *nain_tltle = ">>>   秮 蚭  ゅ 选   <<<";

char *main_menu[] = {
  "   1. 簛潯 秮蚭ゅ选   ",
  "   2. 秮蚭ゅ选潯 簛   ",
  "   3. 弡 燼谩嫛         "
};

FILE *fp;  /* 秮蚭ゅ选 蘟珐 */

/*
 *  秮蚭ゅ选 竌瀉蘟珐礎 , 暋 夺谩 堮琣
 */

/*
 * 簛 --> 秮蚭ゅ选
 */
int findzipcode(FILE *fp, char *addr, char *zipcode)
{
  int i;
  unsigned char addr_buf[80];
  unsigned char *sido[] = {
   "秹", "坵顶暋", "攣稿", "聴恞", "聴", "伐厘", "墂嫛暋","壸簛",
   "稿恞", "稿", "琫", "墂恞", "笰簛暋", "攣妬", "墂", NULL
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
      if(!strncmp(addr_buf+6, addr, strlen(addr))) {
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
 *  秮蚭ゅ选 --> 簛
 */
int findaddr(FILE *fp, char *zipcode, char *addr)
{
  long start, end, middle;
  unsigned char buf[80];
  int i;

  fseek(fp, 0L, SEEK_END);    /*蚜珐穪 弡穉潯 贰暦*/
  start = 0;		   /*萹瑐衖 ゑ夺穪 泪穛壛 弡 簵坋穒 弧各*/
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

  addr[0] = NULL;
  hputsxy(5, 5, "         詬 簛潯 秮蚭ゅ选 纗嫛 詬");
  hputsxy(5, 7, "(暋) 妬(,妳) 暦(穝,) 宎弧爀 烦漛衋瑼禷");

  while( 1 ) {
    hgotoxy(5, 15);
    hgetln(addr, 60);
    for(i = 0; i < 80; i++)
      hputsxy(5+i, 20, " ");
    if(findzipcode(fp, addr, zipcode)) {
      zipcode[6] = zipcode[5], zipcode[5] = zipcode[4], zipcode[4] = zipcode[3];
      zipcode[3] = '-', zipcode[7] = NULL;
      hprintf(5, 20, "%s穪 秮蚭ゅ选揺 %s烦摗攁", addr, zipcode);
    }
    else {
      hprintf(5, 20, "媋滃 簛礎暋 秮蚭ゅ选坅 范恆?");
    }
    hprintf(5, 25, "墎 衖宎禷 ?(Y/n)");
    if(toupper(getchext()) == 'N')
      break;
  }
}

void zip2addr(void)
{
  char addr[80], zipcode[40], temp[40];
  int i;

  zipcode[0] = NULL;
  hputsxy(5, 5, "         詬 秮蚭ゅ选潯 簛 纗嫛 詬");
  hputsxy(5, 7, "秮蚭ゅ选焛 烦漛衋 媋 簛焛 纗碼簛弧禷 ");

  while( 1 ) {
    hgotoxy(5, 15);
    hgetdata(zipcode, '999-999');
    strcpy(temp, zipcode);
    zipcode[3] = zipcode[4], zipcode[4] = zipcode[5], zipcode[5] = zipcode[6];
    zipcode[6] = NULL;
    for(i = 0; i < 80; i++)
      hputsxy(5+i, 20, " ");
    if(findaddr(fp, zipcode, addr)) {
      hprintf(5, 20, "秮蚭ゅ选 %s伐 壍積 %s烦摗攁", zipcode, addr);
    }
    else {
      hprintf(5, 20, "媋滃 秮蚭ゅ选坅 瑏潯 瑮塿恆?");
    }
    hprintf(5, 25, "墎 衖宎禷 ?(Y/n)");
    if(toupper(getchext()) == 'N')
      break;
  }
}

void zip_finder(void)
{
  int ans, isquit = 0;
  void (*func[])(void) = {
    addr2zip, zip2addr
  };

  do {
    ans = popup(hgetmaxax()/2-12, 10, 5, main_menu, menu_title, "123", 1);
    if(ans == -1 || ans == 2)
      isquit = 1;
    else
      func[ans]();
  } while(!isquit);
}

void main(void)
{
  char zipcode[80], addr[80];
  int ans;

  fp = fopen("koreazip.dat", "r");
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