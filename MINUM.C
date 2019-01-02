#include <mem.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dos.h>
#include <ctype.h>
#include <alloc.h>
#include <process.h>
#include "hanlib.h"
#include "hancolor.h"
#include "hanmenu.h"
#include "hanwindw.h"
#include "ascii.h"
#include "extkey.h"
#include "hanin.h"
#include "handbms.h"
#include "hanerro2.h"
#include "min-stru.h"
#include "hanprint.h"
#include "hanpop.h"
#include "hanzip.h"
#include "handdd.h"
#include "hancalen.h"
#include "hancalc.h"

#define  MAXTRY   3    /* ¥q—° Ø°ï° “UÆÅ */

enum menu_no { UTIL_MENU, PROF_MENU, GROUP_MENU, MEDIA_MENU, SUPPORT_MENU, CAMP_MENU, CONFIG_MENU };

enum items_menu0 { ABOUT_MINUM, CALENDAR, CALCULATOR, NULL0, OSSHELL, QUIT };
enum items_menu1 { PROF_FIND, PROF_BROWSE, PROF_INPUT, PROF_DELETE, PROF_PRINT };
enum items_menu2 { GROUP_FIND, GROUP_BROWSE, GROUP_INPUT, GROUP_DELETE, GROUP_PRINT };
enum items_menu3 { MEDIA_FIND, MEDIA_BROWSE, MEDIA_INPUT, MEDIA_DELETE, MEDIA_PRINT };
enum items_menu4 { SUPPORT_FIND, SUPPORT_BROWSE, SUPPORT_INPUT, SUPPORT_DELETE, SUPPORT_PRINT };
enum items_menu5 { CAMP_FIND, CAMP_BROWSE, CAMP_INPUT, CAMP_DELETE, CAMP_PRINT };
enum items_menu6 { USER_REGISTER, PASSWD_REGISTER, NULL1, ABOUT_HANTLE };

/*
 *  äaÆÅ        : PROFessor
 *  îe¡A“A∂•    : GROUP
 *  ¥Âù•ã°â≈    : mass MEDIA
 *  “Å∂•“A∂•    : SUPPORTer
 *  ƒëœa¿qàa∏a  : CAMP
 */

int fl[] = { PROF, GROUP, MEDIA, SUPPORT, CAMP, -1 };
int ele[] = { 0, -1 };

struct coord_t prof_crd[] = {
  { 18, 3 }, { 56, 3 }, { 18, 5 }, { 56, 5 }, { 22, 7 }, { 18, 9 },
  { 22,11 }, { 61,11 }, { 22,13 }, { 18,15 }, { 22,17 }, { 61,17 },
  { 18,19 }
};

struct coord_t group_crd[] = {
  { 18, 4 }, { 18, 6 }, { 18,10 }, { 18,12 }, { 18,14 }, { 62,14 },
  { 18,18 }
};

struct coord_t media_crd[] = {
  { 18, 3 }, { 61, 3 }, { 18, 5 }, { 18, 9 }, { 18,11 }, { 18,13 },
  { 61,13 }, { 61,15 }, { 18,19 }
};

struct coord_t support_crd[] = {
  { 18, 4 }, { 63, 4 }, { 18, 6 }, { 63, 6 }, { 18,10 }, { 18,12 },
  { 18,14 }, { 63,14 }, { 18,18 }
};

struct coord_t camp_crd[] = {
  { 18, 4 }, { 54, 4 }, { 18, 6 }, { 54, 6 }, { 18, 8 }, { 18,11 },
  { 18,13 }, { 18,15 }, { 54,15 }, { 18,18 }
};

static char name[40];
bool redisplay_flag = true;

int fcolor, bcolor, menu_fcolor, menu_bcolor;

extern int *elist;

void initusermenu(int fcolor, int bcolor, int lcolor);
int util_menu(void);
void prof_menu(void);
void group_menu(void);
void media_menu(void);
void support_menu(void);
void camp_menu(void);
void config_menu(void);
int main_loop(void);
void background(void);
void about_minum(void);
void about_hantle(void);
void init_prof_edit(void);
void init_group_edit(void);
void init_media_edit(void);
void init_support_edit(void);
void init_camp_edit(void);

void initusermenu(int fcolor, int bcolor, int init_menu)
{
  nmenu = 7;         /* ¿aü±Œa àïÆÅ 6 àÅ */

  pmenu[0].menu = "£•∑qµe";
  pmenu[0].nitem = 6;
  pmenu[0].item[0] = "£•π¢∑q¥bµeäÅ“A";  pmenu[0].shortcut[0] = 0;
  pmenu[0].item[1] = "îiùb";            pmenu[0].shortcut[1] = ALT_D;
  pmenu[0].item[2] = "âÅ¨eã°";          pmenu[0].shortcut[2] = ALT_A;
  pmenu[0].item[3] = "-";               pmenu[0].shortcut[3] = 0;
  pmenu[0].item[4] = "ï°Øa°wùw¥·";      pmenu[0].shortcut[4] = ALT_F10;
  pmenu[0].item[5] = "è{";              pmenu[0].shortcut[5] = ALT_X;

  pmenu[1].menu = "äa ÆÅ";
  pmenu[1].nitem = 5;
  pmenu[1].item[0] = "∏aûa ¿xã°";      pmenu[1].shortcut[0] = ALT_P;
  pmenu[1].item[1] = "∏aûa •°ã°";      pmenu[1].shortcut[1] = 0;
  pmenu[1].item[2] = "∏aûa ê˝ã°";      pmenu[1].shortcut[2] = 0;
  pmenu[1].item[3] = "∏aûa ª°∂Åã°";    pmenu[1].shortcut[3] = 0;
  pmenu[1].item[4] = "∏aûa ¬âùb";      pmenu[1].shortcut[4] = 0;

  pmenu[2].menu = "îe ¡A";
  pmenu[2].nitem = 5;
  pmenu[2].item[0] = "∏aûa ¿xã°";      pmenu[2].shortcut[0] = ALT_G;
  pmenu[2].item[1] = "∏aûa •°ã°";      pmenu[2].shortcut[1] = 0;
  pmenu[2].item[2] = "∏aûa ê˝ã°";      pmenu[2].shortcut[2] = 0;
  pmenu[2].item[3] = "∏aûa ª°∂Åã°";    pmenu[2].shortcut[3] = 0;
  pmenu[2].item[4] = "∏aûa ¬âùb";      pmenu[2].shortcut[4] = 0;

  pmenu[3].menu = "¥Âù•ã°â≈";
  pmenu[3].nitem = 5;
  pmenu[3].item[0] = "∏aûa ¿xã°";      pmenu[3].shortcut[0] = ALT_M;
  pmenu[3].item[1] = "∏aûa •°ã°";      pmenu[3].shortcut[1] = 0;
  pmenu[3].item[2] = "∏aûa ê˝ã°";      pmenu[3].shortcut[2] = 0;
  pmenu[3].item[3] = "∏aûa ª°∂Åã°";    pmenu[3].shortcut[3] = 0;
  pmenu[3].item[4] = "∏aûa ¬âùb";      pmenu[3].shortcut[4] = 0;


  pmenu[4].menu = "“Å∂•“A∂•";
  pmenu[4].nitem = 5;
  pmenu[4].item[0] = "∏aûa ¿xã°";      pmenu[4].shortcut[0] = ALT_S;
  pmenu[4].item[1] = "∏aûa •°ã°";      pmenu[4].shortcut[1] = 0;
  pmenu[4].item[2] = "∏aûa ê˝ã°";      pmenu[4].shortcut[2] = 0;
  pmenu[4].item[3] = "∏aûa ª°∂Åã°";    pmenu[4].shortcut[3] = 0;
  pmenu[4].item[4] = "∏aûa ¬âùb";      pmenu[4].shortcut[4] = 0;

  pmenu[5].menu = "ƒëœa ¿qàa∏a";
  pmenu[5].nitem = 5;
  pmenu[5].item[0] = "∏aûa ¿xã°";      pmenu[5].shortcut[0] = ALT_C;
  pmenu[5].item[1] = "∏aûa •°ã°";      pmenu[5].shortcut[1] = 0;
  pmenu[5].item[2] = "∏aûa ê˝ã°";      pmenu[5].shortcut[2] = 0;
  pmenu[5].item[3] = "∏aûa ª°∂Åã°";    pmenu[5].shortcut[3] = 0;
  pmenu[5].item[4] = "∏aûa ¬âùb";      pmenu[5].shortcut[4] = 0;

  pmenu[6].menu = "ã°»a";
  pmenu[6].nitem = 4;
  pmenu[6].item[0] = "¨a∂w∏a ówù¢";    pmenu[6].shortcut[0] = ALT_U;
  pmenu[6].item[1] = "¥q—° ówù¢";      pmenu[6].shortcut[1] = 0;
  pmenu[6].item[2] = "-";              pmenu[6].shortcut[2] = 0;
  pmenu[6].item[3] = "–eÀi¥aü°";       pmenu[6].shortcut[3] = 0;
  

  definemenu(fcolor, bcolor, init_menu, 1);
}

void dosshell(void)
{
  char *comspec;

  closehan();
  comspec = (char*)getenv("COMSPEC");
  printf("         …ÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕª\n");
  printf("         ∫                    Minum Dos Shell                 ∫\n");
  printf("         «ƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒ∂\n");
  printf("         ∫                                                    ∫\n");
  printf("         ∫      To return Minum,   type 'EXIT'. 1993 Kwon     ∫\n");
  printf("         ∫                                                    ∫\n");
  printf("         »ÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕº\n\n");

  putenv("PROMPT=Type EXIT to Return to Minum$_(Minum)$p$g");
  if(spawnl(P_WAIT, comspec, comspec, NULL) == -1) {
    printf("\n\n%s missing or Memory insufficent.   Press any key.... \n\n");
    getchext();
  }
  inithanlib(FIXRESMODE, HANDETECT, HAN8GD2, ENGGD5);
  registeregrfont(EGR1);
  registerhgrfont(HGR1);
  initusermenu(menu_fcolor, menu_bcolor, 1);
  inithwindow(fcolor, bcolor);
  background();
}

int util_menu()
{
  int isquit = FALSE;

  redisplay_flag = false;

  switch(getcuritem()) {
    case ABOUT_MINUM :
      about_minum();
      break;
    case CALENDAR :
      calendar();
      break;
    case CALCULATOR :
      calculator();
      break;
    case OSSHELL :
      dosshell();
      break;
    case QUIT :
      if(yesno(" ∏˜†i è{êâåa∂a ? ", 1) == 0)
        isquit = TRUE;
      break;
  }
  return isquit;
}

char *direct_title =
  "‘Ñ   ¬â ùb §w –∑    ‘Ñ";
char *direct_menu[] = {
  "  1. œa ü• »· ù°..    ",
  "  2. Ãa ∑© ù°..       "
};

/*
 *  äa ÆÅ
 */
void prof_menu(void)
{
  int ans, dir;
  char *title = "‘Ñ    ∑• ≠· —w »Å    ‘Ñ";
  char *menu[] = {
    " 1. ª≥ ∫Å≠°üi •°â°¨· —w»Åù°     ",
    " 2. ª≥ ∫Å≠°üi Õeª°•∑ Å —w»Åù°   ",
    " 3. ª¢∏w ∫Å≠°üi •°â°¨· —w»Åù°   ",
    " 4. ª¢∏w ∫Å≠°üi Õeª°•∑ Å —w»Åù° "
  };
  int home[] = {
    PROF_NAME, PROF_ZIP1, PROF_ADDR1, PROF_AREA1, PROF_TEL1, 0
  };
  int job[] = {
    PROF_NAME, PROF_ZIP2, PROF_ADDR2, PROF_AREA2, PROF_TEL2, 0
  };

  redisplay_flag = false;
  switch(getcuritem()) {
    case PROF_FIND :
      ans = win_hgetdata(10, " ‘Ñ ¿x∑i ¨aúq∑Å ∑°üq∑i ∑≥ùb–a≠A∂a ", "XXXXXXXXXX", name);
      if(ans) {
        init_prof_edit();
        db_search(PROF, "‘ê   äa   ÆÅ   “A   ∂•     ∏a   ûa   •°   ã°   ‘ë", ele, prof_crd, 1, name);
        redisplay_flag = true;
      }
      break;
    case PROF_BROWSE :
      init_prof_edit();
      db_browse(PROF, "‘ê   äa   ÆÅ   “A   ∂•     ∏a   ûa   •°   ã°   ‘ë", ele, 1, B2UPDATE);
      redisplay_flag = true;
      break;
    case PROF_INPUT :
      init_prof_edit();
      db_input(PROF, "‘ê   äa   ÆÅ   “A   ∂•     ∏a   ûa   ∑≥   ùb   ‘ë", ele, prof_crd);
      redisplay_flag = true;
      break;
    case PROF_DELETE :
      ans = win_hgetdata(10, " ‘Ñ ª°∂â ¨aúq∑Å ∑°üq∑i ∑≥ùb–a≠A∂a ", "XXXXXXXXXX", name);
      if(ans) {
        init_prof_edit();
        db_delete(PROF, "‘ê   äa   ÆÅ   “A   ∂•     ∏a   ûa   ª°   ∂Å  ã°   ‘ë", ele, prof_crd, 1, name);
        redisplay_flag = true;
      }
      break;
    case PROF_PRINT :
      ans = popup(hgetmaxax()/2-strlen(menu[0])/2, 10, 4, menu, title, "1234", 1);
      if(ans == -1)
         return;
      dir = popup(hgetmaxax()/2-strlen(direct_menu[0])/2, 10, 2, direct_menu, direct_title, "12", 1);
      if(dir == -1)
         return;
      if(ans == 0)
        db_print(PROF, ">>   äa   ÆÅ   “A   ∂•    ª≥   ∫Å   ≠°     <<", home, 1, (dir == 0) ? DATA2PRN : DATA2FILE);
      else if(ans == 2)
        db_print(PROF, ">>   äa   ÆÅ   “A   ∂•    ª¢   ∏w   ∫Å   ≠°   <<", job, 1, (dir == 0) ? DATA2PRN : DATA2FILE);
      break;
  } /* switch */
}

void update_prof(int file_no, char *buf)
{
  db_update(file_no, "‘ê   äa   ÆÅ   “A   ∂•    ∏a   ûa   â°   √°   ã°   ‘ë", ele, prof_crd, 1, buf);
}

/*
 *  îe ¡A
 */
void group_find_by_name(void)
{
  int ans;

  ans = win_hgetdata(20, " ‘Ñ ¿x∑i îe¡A∑Å ∑°üq∑i ∑≥ùb–a≠A∂a ", "XXXXXXXXXXXXXXXXXXXX", name);
  if(ans) {
    init_group_edit();
    db_search(GROUP, "‘ê   îe   ¡A   “A   ∂•     ∏a   ûa   •°   ã°   ‘ë", ele, group_crd, 1, name);
    redisplay_flag = true;
  }
}

void group_find_by_presi(void)
{
  int ans;

  ans = win_hgetdata(10, " ‘Ñ ¿x∑i îe¡A∑Å îÅŒa∑°üq∑i ∑≥ùb–a≠A∂a ", "XXXXXXXXXX", name);
  if(ans) {
    init_group_edit();
    db_search(GROUP, "‘ê   îe   ¡A   “A   ∂•     ∏a   ûa   •°   ã°   ‘ë", ele, group_crd, 2, name);
    redisplay_flag = true;
  }
}

void group_menu(void)
{
  int ans, dir;
  char *find_title = "‘Ñ  ¿x ã° ¨Â »Ç  ‘Ñ";
  char *find_menu[] = {
    "  1. îe¡A ∑°üq∑aù°  ",
    "  2. îÅŒa ∑°üq∑aù°  "
  };
  char *browse_title = "‘Ñ  •° ã° ÆÖ ¨·  ‘Ñ";
  char *browse_menu[] = {
    "  1. îe¡A ∑°üq ÆÖ¨·ù°  ",
    "  2. îÅŒa ∑°üq ÆÖ¨·ù°  "
  };
  char *print_title = "‘Ñ   ∑• ≠· —w »Å   ‘Ñ";
  char *print_menu[] = {
    "  1. •°â°¨· —w»Åù°     ",
    "  2. Õeª°•∑ Å —w»Åù°   "
  };
  void (*find_func[])(void) = {
    group_find_by_name, group_find_by_presi
  };
  int addr[] = { GROUP_NAME, /* GROUP_PRESI,*/ GROUP_ZIP, GROUP_ADDR, GROUP_AREA, GROUP_TEL, 0 };

    redisplay_flag = false;
  switch(getcuritem()) {
    case GROUP_FIND :
      ans = popup(hgetmaxax()/2-strlen(find_menu[0])/2, 10, 2, find_menu, find_title, "12", 1);
      if(ans == -1)
         return;
      find_func[ans]();
      break;
    case GROUP_BROWSE :
      ans = popup(hgetmaxax()/2-strlen(browse_menu[0])/2, 10, 2, browse_menu, browse_title, "12", 1);
      if(ans == -1)
         return;
      init_group_edit();
      db_browse(GROUP, "‘ê   îe   ¡A   “A   ∂•     ∏a   ûa   •°   ã°   ‘ë", ele, ans+1, B2UPDATE);
      redisplay_flag = true;
      break;
    case GROUP_INPUT :
      init_group_edit();
      db_input(GROUP, "‘ê   îe   ¡A   “A   ∂•     ∏a   ûa   ∑≥   ùb   ‘ë", ele, group_crd);
      redisplay_flag = true;
      break;
    case GROUP_DELETE :
      ans = win_hgetdata(10, " ‘Ñ ª°∂â îe¡A ∑°üq∑i ∑≥ùb–a≠A∂a ", "XXXXXXXXXX", name);
      if(ans) {
        init_group_edit();
        db_delete(GROUP, "‘ê   îe   ¡A   “A   ∂•     ∏a   ûa   ª°   ∂Å   ã°   ‘ë", ele, group_crd, 1, name);
        redisplay_flag = true;
      }
      break;
    case GROUP_PRINT :
      ans = popup(hgetmaxax()/2-strlen(print_menu[0])/2, 10, 2, print_menu, print_title, "12", 1);
      if(ans == -1)
         return;
      dir = popup(hgetmaxax()/2-strlen(direct_menu[0])/2, 10, 2, direct_menu, direct_title, "12", 1);
      if(dir == -1)
         return;
      if(ans == 0)
        db_print(GROUP, ">>   îe   ¡A   “A   ∂•    ∫Å   ≠°     <<", addr, 1, (dir == 0) ? DATA2PRN : DATA2FILE);
/*      else if(ans == 1) */

      break;
  } /* switch */
}

void update_group(int file_no, char *buf)
{
  db_update(file_no, "‘ê   îe   ¡A   “A   ∂•    ∏a   ûa   â°   √°   ã°   ‘ë", ele, group_crd, 1, buf);
}

/*
 *  ¥Â ù• ã° â≈
 */
void media_find_by_name(void)
{
  int ans;

  ans = win_hgetdata(20, " ‘Ñ ¿x∑i ¥Âù•ã°â≈∑Å ∑°üq∑i ∑≥ùb–a≠A∂a ", "XXXXXXXXXXXXXXXXXXXX", name);
  if(ans) {
    init_media_edit();
    db_search(MEDIA, "‘ê   ¥Â   ù•   ã°   â≈     ∏a   ûa   •°   ã°   ‘ë", ele, media_crd, 1, name);
    redisplay_flag = true;
  }
}

void media_find_by_part(void)
{
  int ans;

  ans = win_hgetdata(10, " ‘Ñ ¿x∑i îqîw∏a∑Å îÅŒa∑°üq∑i ∑≥ùb–a≠A∂a ", "XXXXXXXXXX", name);
  if(ans) {
    init_media_edit();
    db_search(MEDIA, "‘ê   ¥Â   ù•   ã°   â≈     ∏a   ûa   •°   ã°   ‘ë", ele, media_crd, 2, name);
    redisplay_flag = true;
  }
}

void media_menu(void)
{
  int ans, dir;
  char *find_title = "‘Ñ  ¿x ã° ¨Â »Ç  ‘Ñ";
  char *find_menu[] = {
    "  1. ¥Âù•ã°â≈ ∑°üq∑aù°  ",
    "  2. îqîw∏a ∑°üq∑aù°    "
  };
  char *browse_title = "‘Ñ  •° ã° ÆÖ ¨·  ‘Ñ";
  char *browse_menu[] = {
    "  1. ¥Âù•ã°â≈ ∑°üq ÆÖ¨·ù°  ",
    "  2. îqîw∏a ∑°üq ÆÖ¨·ù°    "
  };
  char *print_title = "‘Ñ   ∑• ≠· —w »Å   ‘Ñ";
  char *print_menu[] = {
    "  1. •°â°¨· —w»Åù°     ",
    "  2. Õeª°•∑ Å —w»Åù°   "
  };
  void (*find_func[])(void) = {
    media_find_by_name, media_find_by_part
  };
  int addr[] = {
    MEDIA_NAME, /* MEDIA_PART, MEDIA_PERSON, */ MEDIA_ZIP, MEDIA_ADDR,
    MEDIA_AREA, MEDIA_TEL, /* MEDIA_FAX, */ 0
  };

  redisplay_flag = false;
  switch(getcuritem()) {
    case MEDIA_FIND :
      ans = popup(hgetmaxax()/2-strlen(find_menu[0])/2, 10, 2, find_menu, find_title, "12", 1);
      if(ans == -1)
         return;
      find_func[ans]();
      break;
    case MEDIA_BROWSE :
      ans = popup(hgetmaxax()/2-strlen(browse_menu[0])/2, 10, 2, browse_menu, browse_title, "12", 1);
      if(ans == -1)
         return;
      init_media_edit();
      db_browse(MEDIA, "‘ê   ¥Â   ù•   ã°   â≈     ∏a   ûa   •°   ã°   ‘ë", ele, ans+1, B2UPDATE);
      redisplay_flag = true;
      break;
    case MEDIA_INPUT :
      init_media_edit();
      db_input(MEDIA, "‘ê   ¥Â   ù•   ã°   â≈     ∏a   ûa   ∑≥   ùb   ‘ë", ele, media_crd);
      redisplay_flag = true;
      break;
    case MEDIA_DELETE :
      ans = win_hgetdata(20, " ‘Ñ ª°∂â ¥Âù•ã°â≈∑Å ∑°üq∑i ∑≥ùb–a≠A∂a ", "XXXXXXXXXXXXXXXXXXXX", name);
      if(ans) {
        init_media_edit();
        db_delete(MEDIA, "‘ê   ¥Â   ù•   ã°   â≈     ∏a   ûa   ª°   ∂Å   ã°   ‘ë", ele, media_crd, 1, name);
        redisplay_flag = true;
      }
      break;
    case MEDIA_PRINT :
      ans = popup(hgetmaxax()/2-strlen(print_menu[0])/2, 10, 2, print_menu, print_title, "12", 1);
      if(ans == -1)
         return;
      dir = popup(hgetmaxax()/2-strlen(direct_menu[0])/2, 10, 2, direct_menu, direct_title, "12", 1);
      if(dir == -1)
         return;
      if(ans == 0)
        db_print(MEDIA, ">>   ¥Â   ù•   ã°   â≈    ∫Å   ≠°     <<", addr, 1, (dir == 0) ? DATA2PRN : DATA2FILE);
/*      else if(ans == 1) */

      break;
  } /* switch */
}

void update_media(int file_no, char *buf)
{
  db_update(file_no, "‘ê   ¥Â   ù•   ã°   â≈    ∏a   ûa   â°   √°   ã°   ‘ë", ele, media_crd, 1, buf);
}

void support_find_by_name(void)
{
  int ans;

  ans = win_hgetdata(20, " ‘Ñ ¿x∑i “Å∂•“A∂•∑Å ∑°üq∑i ∑≥ùb–a≠A∂a ", "XXXXXXXXXXXXXXXXXXXX", name);
  if(ans) {
    init_support_edit();
    db_search(SUPPORT, "‘ê   “Å   ∂•   “A   ∂•     ∏a   ûa   •°   ã°   ‘ë", ele, support_crd, 1, name);
    redisplay_flag = true;
  }
}

void support_find_by_invi(void)
{
  int ans;

  ans = win_hgetdata(10, " ‘Ñ ¿x∑i ≠°àÅ–e ¨aúq ∑°üq∑i ∑≥ùb–a≠A∂a ", "XXXXXXXXXX", name);
  if(ans) {
    init_support_edit();
    db_search(SUPPORT, "‘ê   “Å   ∂•   “A   ∂•     ∏a   ûa   •°   ã°   ‘ë", ele, support_crd, 2, name);
    redisplay_flag = true;
  }
}

void support_menu(void)
{
  int ans, dir;
  char *find_title = "‘Ñ  ¿x ã° ¨Â »Ç  ‘Ñ";
  char *find_menu[] = {
    "  1. “Å∂•“A∂• ∑°üq∑aù°       ",
    "  2. ≠°àÅ–e ¨aúq ∑°üq∑aù°    "
  };
  char *browse_title = "‘Ñ  •° ã° ÆÖ ¨·  ‘Ñ";
  char *browse_menu[] = {
    "  1. “Å∂•“A∂• ∑°üq ÆÖ¨·ù°       ",
    "  2. ≠°àÅ–e ¨aúq ∑°üq ÆÖ¨·ù°    "
  };
  char *print_title = "‘Ñ   ∑• ≠· —w »Å   ‘Ñ";
  char *print_menu[] = {
    "  1. •°â°¨· —w»Åù°     ",
    "  2. Õeª°•∑ Å —w»Åù°   "
  };
  void (*find_func[])(void) = {
    support_find_by_name, support_find_by_invi
  };
  int addr[] = {
    SUPPORT_NAME, /* SUPPORT_PART, */ SUPPORT_ZIP, SUPPORT_ADDR,
    SUPPORT_AREA, SUPPORT_TEL, 0
  };

  redisplay_flag = false;
  switch(getcuritem()) {
    case SUPPORT_FIND :
      ans = popup(hgetmaxax()/2-strlen(find_menu[0])/2, 10, 2, find_menu, find_title, "12", 1);
      if(ans == -1)
         return;
      find_func[ans]();
      break;
    case SUPPORT_BROWSE :
      ans = popup(hgetmaxax()/2-strlen(browse_menu[0])/2, 10, 2, browse_menu, browse_title, "12", 1);
      if(ans == -1)
         return;
      init_support_edit();
      db_browse(SUPPORT, "‘ê   “Å   ∂•   “A   ∂•     ∏a   ûa   •°   ã°   ‘ë", ele, ans+1, B2UPDATE);
      redisplay_flag = true;
      break;
    case SUPPORT_INPUT :
      init_support_edit();
      db_input(SUPPORT, "‘ê   “Å   ∂•   “A   ∂•     ∏a   ûa   ∑≥   ùb   ‘ë", ele, support_crd);
      redisplay_flag = true;
      break;
    case SUPPORT_DELETE :
      ans = win_hgetdata(20, " ‘Ñ ª°∂â “Å∂•“A∂•∑Å ∑°üq∑i ∑≥ùb–a≠A∂a ", "XXXXXXXXXXXXXXXXXXXX", name);
      if(ans) {
        init_support_edit();
        db_delete(SUPPORT, "‘ê   “Å   ∂•   “A   ∂•     ∏a   ûa   ª°   ∂Å   ã°   ‘ë", ele, support_crd, 1, name);
        redisplay_flag = true;
      }
      break;
    case SUPPORT_PRINT :
      ans = popup(hgetmaxax()/2-strlen(print_menu[0])/2, 10, 2, print_menu, print_title, "12", 1);
      if(ans == -1)
         return;
      dir = popup(hgetmaxax()/2-strlen(direct_menu[0])/2, 10, 2, direct_menu, direct_title, "12", 1);
      if(dir == -1)
         return;
      if(ans == 0)
        db_print(SUPPORT, ">>   “Å   ∂•   “A   ∂•    ∫Å   ≠°     <<", addr, 1, (dir == 0) ? DATA2PRN : DATA2FILE);
/*      else if(ans == 1) */

      break;
  } /* switch */
}

void update_support(int file_no, char *buf)
{
  db_update(file_no, "‘ê   “Å   ∂•   “A   ∂•    ∏a   ûa   â°   √°   ã°   ‘ë", ele, support_crd, 1, buf);
}

/* ¢Ö∏aµi süi µ°üeΩ¢ ∏˜ùi(ß•ƒe∑e â∑§Ç∑aù° ¿Å∂Öîa */
static void right_justify(char *s)
{
  int len;

  len = strlen(s);
  while (*s == ' ' && len)   {
    len--;
  *s++ = ' ';
  }
  if (len)
    while (*(s+(len-1)) == ' ')     {
      mov_mem(s, s+1, len-1);
      *s = ' ';
    }
}

/*
 * ƒëœa ¿qàa∏a  ; îa∫ó «° ¿·ü°µA ∫Å∑Å!
 */

void camp_find(void)
{
  unsigned char key[MAXKEYLEN], num[4];
  int ans;

  key[0] = num[0] = NULL;
  ans = win_hgetdata(10, " ‘Ñ ¿x∑i ¿qàa∏a∑Å ∑°üq∑i ∑≥ùb–a≠A∂a ", "XXXXXXXXXX", name);
  if(ans) {
    strcpy(key, name);
    ans = win_hgetdata(3, " ‘Ñ ãa ¨aúq∑Å ¿qàa “A §Â—°üi ∑≥ùb–a≠A∂a ", "###", num);
    if(ans) {
      right_justify(num);
      strcat(key, num);
      init_camp_edit();
      db_search(CAMP, "‘ê   ƒë   œa   ¿q   àa   ∏a     ∏a   ûa   •°   ã°   ‘ë", ele, camp_crd, 1, key);
      redisplay_flag = true;
    }
  }
}

void camp_delete(void)
{
  unsigned char key[MAXKEYLEN], num[4];
  int ans;

  key[0] = num[0] = NULL;
  ans = win_hgetdata(10, " ‘Ñ ª°∂â ¿qàa∏a∑Å ∑°üq∑i ∑≥ùb–a≠A∂a ", "XXXXXXXXXX", name);
  if(ans) {
    strcpy(key, name);
    ans = win_hgetdata(3, " ‘Ñ ãa ¨aúq∑Å ¿qàa “A §Â—°üi ∑≥ùb–a≠A∂a ", "###", num);
    if(ans) {
      right_justify(num);
      strcat(key, num);
      init_camp_edit();
      db_delete(CAMP, "‘ê   ƒë   œa   ¿q   àa   ∏a     ∏a   ûa   ª°   ∂Å   ã°   ‘ë", ele, camp_crd, 1, key);
      redisplay_flag = true;
    }
  }
}

void camp_menu(void)
{
  int ans, dir;
  char *print_title = "‘Ñ   ∑• ≠· —w »Å   ‘Ñ";
  char *print_menu[] = {
    "  1. •°â°¨· —w»Åù°     ",
    "  2. Õeª°•∑ Å —w»Åù°   "
  };
  int addr[] = { CAMP_NAME, /* CAMP_NO, */ CAMP_ZIP, CAMP_ADDR, CAMP_AREA, CAMP_TEL, 0 };

  redisplay_flag = false;
  switch(getcuritem()) {
    case CAMP_FIND :
      camp_find();
      break;
    case CAMP_BROWSE :
      init_camp_edit();
      db_browse(CAMP, "‘ê   ƒë   œa   ¿q   àa   ∏a     ∏a   ûa   •°   ã°   ‘ë", ele, 1, B2UPDATE);
      break;
    case CAMP_INPUT :
      init_camp_edit();
      db_input(CAMP, "‘ê   ƒë   œa   ¿q   àa   ∏a     ∏a   ûa   ∑≥   ùb   ‘ë", ele, camp_crd);
      break;
    case CAMP_DELETE :
      camp_delete();
      break;
    case CAMP_PRINT :
      ans = popup(hgetmaxax()/2-strlen(print_menu[0])/2, 10, 2, print_menu, print_title, "12", 1);
      if(ans == -1)
         return;
      dir = popup(hgetmaxax()/2-strlen(direct_menu[0])/2, 10, 2, direct_menu, direct_title, "12", 1);
      if(dir == -1)
         return;
      if(ans == 0)
        db_print(CAMP, ">>   ƒë   œa   ¿q   àa   ∏a    ∫Å   ≠°     <<", addr, 1, (dir == 0) ? DATA2PRN : DATA2FILE);
/*      else if(ans == 1) */

      break;
  } /* switch */
}

void update_camp(int file_no, char *buf)
{
  db_update(file_no, "‘ê   ƒë   œa   ¿q   àa   ∏a    ∏a   ûa   â°   √°   ã°   ‘ë", ele, camp_crd, 1, buf);
}

void init_update_func(void)
{
  browse2update[PROF]  = update_prof;
  browse2update[GROUP] = update_group;
  browse2update[MEDIA] = update_media;
  browse2update[SUPPORT] = update_support;
  browse2update[CAMP] = update_camp;
}

#define  CFGFILE      "MINUMYON.CFG"
#define  CFGID        "Minumyon Config File 0.94"
#define  MAXCYPERKEY  31

/*
 *  äÅ¨˜ Ãa∑©∑Å äÅπ°  1.00
 *  Ø¢•i∏a - 32§a∑°Àa
 *  Minumyon Config File 0.94<EOF>
 *  ∑°üq - 32§a∑°Àa
 *  ¥q—° - 32§a∑°Àa
 *
 *  Ãa∑©µA ≥i òÅ†e ¥q—°—¡–aâ° °A°°ü° ¨wµAìe ¥ÂπAêa ¥q—°àa Œâü• ¨w»Åù° ∑Aª°.
 */
struct cfg_t {
  unsigned char id[32];
  unsigned char name[32];
  unsigned char passwd[32];
} cfg;

unsigned char cyperkey[MAXCYPERKEY] = {
  0xf2, 0x13, 0xe3, 0x45, 0xfe, 0x12, 0x54, 0x99, 0xeb, 0x77,
  0x0c, 0xff, 0xc0, 0x34, 0x32, 0x50, 0x22, 0x90, 0x06, 0x7f,
  0x30, 0x29, 0x03, 0xaa, 0xab, 0xae, 0x73, 0x50, 0x69, 0xab,
  0x11
};

void cyper(unsigned char *code)
{
  int cnt;

  for(cnt = 0; *code; cnt++, code++)
    *code ^= cyperkey[cnt % MAXCYPERKEY];
}

void cyper_cfg(void)
{
  cyper(cfg.name);
}

bool check_passwd(void)
{
  unsigned char str[32];

  str[0] = NULL;
  if(win_hgets(31, " ¥q—° —¬∑• ", str)) {
    return strcmp(str, cfg.passwd) ? false : true;
  }
  return false;
}

void ang_ang(void)
{
  int i;

  for (i = 4000; i > 2000; i-= 50) {
    sound(i);
    delay(50);
  }
  nosound();
}

void fail_sound(void)
{
  int i;

  for (i = 1000; i < 3000; i+= 50) {
    sound(i);
    delay(10);
  }
  nosound();
}

bool init_cfg(void)
{
  FILE *fcfg;
  char siren[] = "    ê·ìe íÅäÅê°? ï°ñÇ∑°ª°?  ãaúÅ... ∑a¥a ∑a¥a  ï°ñÇ∑°¥°!    ";
  int cnt;

  fcfg = fopen(CFGFILE, "rb");
  if(fcfg) {
    fread(&cfg, sizeof(struct cfg_t), 1, fcfg);
    cyper_cfg();
    cyper(cfg.passwd);
    if(!isspace(cfg.passwd[0]) && cfg.passwd[0]) {
      for(cnt = 0; cnt < MAXTRY; cnt++) {
        if(check_passwd())
          break;
        fail_sound();
      }
      if(cnt == MAXTRY) {
        hputsxy(hgetmaxax()/2-strlen(siren)/2, hgetmaxay()/2, siren);
        ang_ang();
        getchext();
        fclose(fcfg);
        return false;
      }
    }
    fclose(fcfg);
  }
  return true;
}

void user_register(void)
{
  unsigned char str[40];
  FILE *fcfg;
  int i;

  str[0] = NULL;
  fcfg = fopen(CFGFILE, "rb");
  if(fcfg) {
    fread(&cfg, sizeof(struct cfg_t), 1, fcfg);
    cyper_cfg();
    fclose(fcfg);
  }

  fcfg = fopen(CFGFILE, "wb");
  if(!fcfg) {
    disperror(" ¨a∂w∏aüi ówù¢–i ÆÅ ¥ÙØsì°îa ");
    return;
  }
  sprintf(cfg.id, "%s%c", CFGID, 0x1A);

  strcpy(str, cfg.name);
  if(win_hgets(31, " ‘ ¨a∂w∏aüi ówù¢–a≠A∂a ‘ ", str)) {
    for(i = strlen(str)-1; i > 0; i--) {
      if(str[i] == ' ')
        str[i] = NULL;
      else
        break;
    }
    strcpy(cfg.name, str);
    redisplay_flag = true;
  }

  cyper_cfg();
  fwrite(&cfg, sizeof(struct cfg_t), 1, fcfg);
  cyper_cfg();
  fclose(fcfg);
}

void passwd_register(void)
{
  unsigned char str[40];
  FILE *fcfg;

  str[0] = NULL;
  fcfg = fopen(CFGFILE, "rb");
  if(fcfg) {
    fread(&cfg, sizeof(struct cfg_t), 1, fcfg);
    cyper_cfg();
    cyper(cfg.passwd);
    if(!isspace(cfg.passwd[0]) && cfg.passwd[0]) {
      if(!check_passwd()) {
        disperror(" ¥q—°àa Àiü≥ì°îa ");
        fclose(fcfg);
        return;
      }
    }
    fclose(fcfg);
  }
  fcfg = fopen(CFGFILE, "wb");
  if(!fcfg) {
    disperror(" ¥q—°üi ówù¢–i ÆÅ ¥ÙØsì°îa ");
    return;
  }
  sprintf(cfg.id, "%s%c", CFGID, 0x1A);
  str[0] = NULL;
  if(win_hgets(31, "‘ ¥q—° ówù¢ ‘", str)) {
    strcpy(cfg.passwd, str);
  }
  cyper_cfg();
  cyper(cfg.passwd);
  fwrite(&cfg, sizeof(struct cfg_t), 1, fcfg);
  cyper_cfg();
  cyper(cfg.passwd);
  fclose(fcfg);
}

void config_menu()
{
  redisplay_flag = false;
  switch(getcuritem()) {
    case USER_REGISTER :
      user_register();
      break;
    case PASSWD_REGISTER :
      passwd_register();
      break;
    case ABOUT_HANTLE :
      about_hantle();
  }
}

int main_loop(void)
{
  int isquit = FALSE, key = F10;

  drawmenubar();
  if(getitem(key)) {
    switch(getcurmenu()) {
      case  UTIL_MENU :
        isquit = util_menu();
        break;
      case  PROF_MENU :
        prof_menu();
        break;
      case  GROUP_MENU :
        group_menu();
        break;
      case  MEDIA_MENU :
        media_menu();
        break;
      case  SUPPORT_MENU :
        support_menu();
        break;
      case  CAMP_MENU :
        camp_menu();
        break;
      case  CONFIG_MENU :
        config_menu();
        break;
    } /* switch */
  } /* if */
  else {
    if(yesno("∏˜†i è{êâåa∂a ?", 1) == 0)
      isquit = true;
  }
  if(!isquit && redisplay_flag)
    background();
  return isquit;
}

void background(void)
{
  bool onoff;

  hclrscr();
  hputsxy(-4,  -9, "‘±‘±‘±‘±‘±‘±‘±‘±  ‘±‘±      ‘±‘±‘±‘±‘±‘±‘±‘±‘±        ‘±‘±‘±‘±‘±      ‘±‘±");
  hputsxy(-4, -10, "‘±‘±        ‘±‘±  ‘±‘±    ‘±‘±‘±          ‘±‘±‘±    ‘±‘±‘±  ‘±‘±‘±  ‘±‘±‘±");
  hputsxy(-4, -11, "‘±‘±        ‘±‘±  ‘±‘±    ‘±‘±              ‘±‘±    ‘±‘±      ‘±‘±    ‘±‘±");
  hputsxy(-4, -12, "‘±‘±        ‘±‘±  ‘±‘±    ‘±‘±‘±          ‘±‘±‘±    ‘±‘±      ‘±‘±    ‘±‘±");
  hputsxy(-4, -13, "‘±‘±        ‘±‘±  ‘±‘±      ‘±‘±‘±‘±‘±‘±‘±‘±‘±      ‘±‘±‘±  ‘±‘±‘±  ‘±‘±‘±");
  hputsxy(-4, -14, "‘±‘±‘±‘±‘±‘±‘±‘±  ‘±‘±                                ‘±‘±‘±‘±‘±      ‘±‘±");
  hputsxy(-4, -15, "                          ‘±‘±‘±‘±‘±‘±‘±‘±‘±‘±‘±                          ");
  hputsxy(-4, -16, "                                                                          ");
  hputsxy(-4, -17, "‘±‘±                      ‘±‘±‘±‘±‘±‘±‘±‘±‘±‘±‘±    ‘±‘±                  ");
  hputsxy(-4, -18, "‘±‘±                      ‘±‘±              ‘±‘±    ‘±‘±                  ");
  hputsxy(-4, -19, "‘±‘±‘±‘±‘±‘±‘±‘±‘±‘±‘±    ‘±‘±‘±‘±‘±‘±‘±‘±‘±‘±‘±    ‘±‘±‘±‘±‘±‘±‘±‘±‘±‘±‘±");

  hputsxy(-4, -21, "‘±‘±‘±‘±‘±‘±‘±‘±‘±‘±‘±‘±‘±‘±‘±‘±‘±‘±‘±‘±‘±‘±‘±‘±‘±‘±‘±‘±‘±‘±‘±‘±‘±‘±‘±‘±‘±");

  onoff = isreverse();
  hsetreverse(ON);
  eputchs(-1, -hgetmaxax(), 80, ' ');
  hputsxy(-(hgetmaxax()-20), -hgetmaxay(), "–e Ài ¥a ü° 1993");
  eputchs(-1, -1, 80, ' ');
  hprintfxy(-2, -25, "‘É  %s  ‘É", cfg.name);
  hsetreverse(onoff);
}

void about_minum(void)
{
  if(wopen(21, 9, 40, 8)) {
    wcenter(2, "‘   £•π¢∑q¥bµeäÅ“A   ‘");
    wcenter(4, "£•π¢∑q¥bµeäÅ“A(£•∑qµe)ìe");
    wcenter(5, "¥·º·äÅ ∏·º·äÅ ¥·º·äÅ");
    wcenter(6, "¥·º·äÅ ∏·º·äÅ ¥·º·äÅ");
    wcenter(7, "¥·º·äÅ ∏·º·äÅ ¥·º·äÅ");
    wait_getkey(5);
    wclose();
  }
}

void about_hantle(void)
{
  if(wopen(21, 9, 40, 8)) {
    wcenter(2, "‘  –e Ài ¥a ü°  ‘");
    wcenter(4, "–e¥∑îÅ–bäa ∏Â∏a…∑Ø•â∑–bâ¡");
    wcenter(5, "≠QÀiï∑¥aü°");
    wcenter(7, "ä• ∏Å úb & ã± —e ÆÅ");
    wait_getkey(5);
    wclose();
  }
}

/*
 * äaÆÅ∏aûaµA¨· ∂ÅÕe§Â—°1ù° ∫Å≠°1¿xã°
 */
int prof_zip1_addr1(void)
{
  unsigned char *zipcode, *ptr;
  unsigned char addr[MAXKEYLEN];
  int len, addrlen;

  ptr = sc + epos(PROF_ADDR1, file_ele[PROF]);
  if(!spaces(ptr))
    return DBOK;
  ptr = sc + epos(PROF_ZIP1, file_ele[PROF]);
  if(spaces(ptr))
    return DBOK;
  zipcode = sc + epos(PROF_ZIP1, file_ele[PROF]);
  if(findaddr(zipcode, addr)) {
    len = strlen(sc+epos(PROF_ADDR1, file_ele[PROF]));
    addrlen = strlen(addr);
    while(addrlen < len)
      addr[addrlen++] = ' ';
    addr[addrlen] = NULL;
    strcpy(sc+epos(PROF_ADDR1, file_ele[PROF]), addr);
    put_field(PROF_ADDR1);
  }
  return DBOK;
}

/*
 * äaÆÅ∏aûaµA¨· ∂ÅÕe§Â—°2ù° ∫Å≠°2¿xã°
 */
int prof_zip2_addr2(void)
{
  unsigned char *zipcode, *ptr;
  unsigned char addr[MAXKEYLEN];
  int len, addrlen;

  ptr = sc + epos(PROF_ADDR2, file_ele[PROF]);
  if(!spaces(ptr))
    return DBOK;
  ptr = sc + epos(PROF_ZIP2, file_ele[PROF]);
  if(spaces(ptr))
    return DBOK;
  zipcode = sc + epos(PROF_ZIP2, file_ele[PROF]);
  if(findaddr(zipcode, addr)) {
    len = strlen(sc+epos(PROF_ADDR2, file_ele[PROF]));
    addrlen = strlen(addr);
    while(addrlen < len)
      addr[addrlen++] = ' ';
    addr[addrlen] = NULL;
    strcpy(sc+epos(PROF_ADDR2, file_ele[PROF]), addr);
    put_field(PROF_ADDR2);
  }
  return DBOK;
}

/*
 * äaÆÅ∏aûaµA¨· ∫Å≠°1ù° ∂ÅÕe§Â—°1 ¿xã°
 */
int prof_addr1_zip1(void)
{
  unsigned char zipcode[MAXKEYLEN];
  unsigned char addr[MAXKEYLEN], *ptr;
  int len, ziplen;

  ptr = sc + epos(PROF_ZIP1, file_ele[PROF]);
  if(!spaces(ptr))
    return DBOK;
  ptr = sc + epos(PROF_ADDR1, file_ele[PROF]);
  if(spaces(ptr))
    return DBOK;
  strcpy(addr, sc + epos(PROF_ADDR1, file_ele[PROF]));
  len = strlen(addr);
  while(isspace(addr[len-1])) {
    addr[len-1] = NULL;
    len--;
  }
  if(findzipcode(addr, zipcode)) {
    len = strlen(sc+epos(PROF_ZIP1, file_ele[PROF]));
    ziplen = strlen(zipcode);
    while(ziplen < len)
      zipcode[ziplen++] = ' ';
    zipcode[ziplen] = NULL;
    strcpy(sc+epos(PROF_ZIP1, file_ele[PROF]), zipcode);
    put_field(PROF_ZIP1);
  }
  return DBOK;
}

/*
 * äaÆÅ∏aûaµA¨· ∫Å≠°1ù° ª°µb§Â—°1 ¿xã°
 */
int prof_addr1_ddd1(void)
{
  unsigned char *ptr, *addr;
  unsigned char ddd[MAXKEYLEN], tmp[MAXKEYLEN];

  ptr = sc + epos(PROF_AREA1, file_ele[PROF]);
  if(!spaces(ptr))
    return DBOK;
  addr = sc + epos(PROF_ADDR1, file_ele[PROF]);
  if(spaces(addr))
    return DBOK;
  if(addr2ddd(addr, ddd)) {
    sprintf(tmp, "%4s", ddd);
    strcpy(ptr, tmp);
    put_field(PROF_AREA1);
  }
  return DBOK;
}

/*
 * äaÆÅ∏aûaµA¨· ∫Å≠°2ù° ∂ÅÕe§Â—°2 ¿xã°
 */
int prof_addr2_zip2(void)
{
  unsigned char zipcode[MAXKEYLEN];
  unsigned char addr[MAXKEYLEN], *ptr;
  int len, ziplen;

  ptr = sc + epos(PROF_ZIP2, file_ele[PROF]);
  if(!spaces(ptr))
    return DBOK;
  ptr = sc + epos(PROF_ADDR2, file_ele[PROF]);
  if(spaces(ptr))
    return DBOK;
  strcpy(addr, sc + epos(PROF_ADDR2, file_ele[PROF]));
  len = strlen(addr);
  while(isspace(addr[len-1])) {
    addr[len-1] = NULL;
    len--;
  }
  if(findzipcode(addr, zipcode)) {
    len = strlen(sc+epos(PROF_ZIP2, file_ele[PROF]));
    ziplen = strlen(zipcode);
    while(ziplen < len)
      zipcode[ziplen++] = ' ';
    zipcode[ziplen] = NULL;
    strcpy(sc+epos(PROF_ZIP2, file_ele[PROF]), zipcode);
    put_field(PROF_ZIP2);
  }
  return DBOK;
}

/*
 * äaÆÅ∏aûaµA¨· ∫Å≠°2ù° ª°µb§Â—°2 ¿xã°
 */
int prof_addr2_ddd2(void)
{
  unsigned char *ptr, *addr;
  unsigned char ddd[MAXKEYLEN], tmp[MAXKEYLEN];

  ptr = sc + epos(PROF_AREA2, file_ele[PROF]);
  if(!spaces(ptr))
    return DBOK;
  addr = sc + epos(PROF_ADDR2, file_ele[PROF]);
  if(spaces(addr))
    return DBOK;
  if(addr2ddd(addr, ddd)) {
    sprintf(tmp, "%4s", ddd);
    strcpy(ptr, tmp);
    put_field(PROF_AREA2);
  }
  return DBOK;
}

/*
 * îe¡A∏aûaµA¨· ∂ÅÕe§Â—°ù° ∫Å≠°¿xã°
 */
int group_zip_addr(void)
{
  unsigned char *zipcode, *ptr;
  unsigned char addr[MAXKEYLEN];
  int len, addrlen;

  ptr = sc + epos(GROUP_ADDR, file_ele[GROUP]);
  if(!spaces(ptr))
    return DBOK;
  ptr = sc + epos(GROUP_ZIP, file_ele[GROUP]);
  if(spaces(ptr))
    return DBOK;
  zipcode = sc + epos(GROUP_ZIP, file_ele[GROUP]);
  if(findaddr(zipcode, addr)) {
    len = strlen(sc+epos(GROUP_ADDR, file_ele[GROUP]));
    addrlen = strlen(addr);
    while(addrlen < len)
      addr[addrlen++] = ' ';
    addr[addrlen] = NULL;
    strcpy(sc+epos(GROUP_ADDR, file_ele[GROUP]), addr);
    put_field(GROUP_ADDR);
  }
  return DBOK;
}

/*
 * îe¡A∏aûaµA¨· ∫Å≠°ù° ∂ÅÕe§Â—° ¿xã°
 */
int group_addr_zip(void)
{
  unsigned char zipcode[MAXKEYLEN];
  unsigned char addr[MAXKEYLEN], *ptr;
  int len, ziplen;

  ptr = sc + epos(GROUP_ZIP, file_ele[GROUP]);
  if(!spaces(ptr))
    return DBOK;
  ptr = sc + epos(GROUP_ADDR, file_ele[GROUP]);
  if(spaces(ptr))
    return DBOK;
  strcpy(addr, sc + epos(GROUP_ADDR, file_ele[GROUP]));
  len = strlen(addr);
  while(isspace(addr[len-1])) {
    addr[len-1] = NULL;
    len--;
  }
  if(findzipcode(addr, zipcode)) {
    len = strlen(sc+epos(GROUP_ZIP, file_ele[GROUP]));
    ziplen = strlen(zipcode);
    while(ziplen < len)
      zipcode[ziplen++] = ' ';
    zipcode[ziplen] = NULL;
    strcpy(sc+epos(GROUP_ZIP, file_ele[GROUP]), zipcode);
    put_field(GROUP_ZIP);
  }
  return DBOK;
}

/*
 * îe¡A∏aûaµA¨· ∫Å≠°ù° ª°µb§Â—° ¿xã°
 */
int group_addr_ddd(void)
{
  unsigned char *ptr, *addr;
  unsigned char ddd[MAXKEYLEN], tmp[MAXKEYLEN];

  ptr = sc + epos(GROUP_AREA, file_ele[GROUP]);
  if(!spaces(ptr))
    return DBOK;
  addr = sc + epos(GROUP_ADDR, file_ele[GROUP]);
  if(spaces(addr))
    return DBOK;
  if(addr2ddd(addr, ddd)) {
    sprintf(tmp, "%4s", ddd);
    strcpy(ptr, tmp);
    put_field(GROUP_AREA);
  }
  return DBOK;
}

/*
 * ¥Âù•ã°â≈∏aûaµA¨· ∂ÅÕe§Â—°ù° ∫Å≠°¿xã°
 */
int media_zip_addr(void)
{
  unsigned char *zipcode, *ptr;
  unsigned char addr[MAXKEYLEN];
  int len, addrlen;

  ptr = sc + epos(MEDIA_ADDR, file_ele[MEDIA]);
  if(!spaces(ptr))
    return DBOK;
  ptr = sc + epos(MEDIA_ZIP, file_ele[MEDIA]);
  if(spaces(ptr))
    return DBOK;
  zipcode = sc + epos(MEDIA_ZIP, file_ele[MEDIA]);
  if(findaddr(zipcode, addr)) {
    len = strlen(sc+epos(MEDIA_ADDR, file_ele[MEDIA]));
    addrlen = strlen(addr);
    while(addrlen < len)
      addr[addrlen++] = ' ';
    addr[addrlen] = NULL;
    strcpy(sc+epos(MEDIA_ADDR, file_ele[MEDIA]), addr);
    put_field(MEDIA_ADDR);
  }
  return DBOK;
}

/*
 * ¥Âù•ã°â≈∏aûaµA¨· ∫Å≠°ù° ∂ÅÕe§Â—° ¿xã°
 */
int media_addr_zip(void)
{
  unsigned char zipcode[MAXKEYLEN];
  unsigned char addr[MAXKEYLEN], *ptr;
  int len, ziplen;

  ptr = sc + epos(MEDIA_ZIP, file_ele[MEDIA]);
  if(!spaces(ptr))
    return DBOK;
  ptr = sc + epos(MEDIA_ADDR, file_ele[MEDIA]);
  if(spaces(ptr))
    return DBOK;
  strcpy(addr, sc + epos(MEDIA_ADDR, file_ele[MEDIA]));
  len = strlen(addr);
  while(isspace(addr[len-1])) {
    addr[len-1] = NULL;
    len--;
  }
  if(findzipcode(addr, zipcode)) {
    len = strlen(sc+epos(MEDIA_ZIP, file_ele[MEDIA]));
    ziplen = strlen(zipcode);
    while(ziplen < len)
      zipcode[ziplen++] = ' ';
    zipcode[ziplen] = NULL;
    strcpy(sc+epos(MEDIA_ZIP, file_ele[MEDIA]), zipcode);
    put_field(MEDIA_ZIP);
  }
  return DBOK;
}

/*
 * ¥Âù•ã°â≈∏aûaµA¨· ∫Å≠°ù° ª°µb§Â—° ¿xã°
 */
int media_addr_ddd(void)
{
  unsigned char *ptr, *addr;
  unsigned char ddd[MAXKEYLEN], tmp[MAXKEYLEN];

  ptr = sc + epos(MEDIA_AREA, file_ele[MEDIA]);
  if(!spaces(ptr))
    return DBOK;
  addr = sc + epos(MEDIA_ADDR, file_ele[MEDIA]);
  if(spaces(addr))
    return DBOK;
  if(addr2ddd(addr, ddd)) {
    sprintf(tmp, "%4s", ddd);
    strcpy(ptr, tmp);
    put_field(MEDIA_AREA);
  }
  return DBOK;
}

/*
 * “Å∂•“A∂•∏aûaµA¨· ∂ÅÕe§Â—°ù° ∫Å≠°¿xã°
 */
int support_zip_addr(void)
{
  unsigned char *zipcode, *ptr;
  unsigned char addr[MAXKEYLEN];
  int len, addrlen;

  ptr = sc + epos(SUPPORT_ADDR, file_ele[SUPPORT]);
  if(!spaces(ptr))
    return DBOK;
  ptr = sc + epos(SUPPORT_ZIP, file_ele[SUPPORT]);
  if(spaces(ptr))
    return DBOK;
  zipcode = sc + epos(SUPPORT_ZIP, file_ele[SUPPORT]);
  if(findaddr(zipcode, addr)) {
    len = strlen(sc+epos(SUPPORT_ADDR, file_ele[SUPPORT]));
    addrlen = strlen(addr);
    while(addrlen < len)
      addr[addrlen++] = ' ';
    addr[addrlen] = NULL;
    strcpy(sc+epos(SUPPORT_ADDR, file_ele[SUPPORT]), addr);
    put_field(SUPPORT_ADDR);
  }
  return DBOK;
}

/*
 * “Å∂•“A∂•∏aûaµA¨· ∫Å≠°ù° ∂ÅÕe§Â—° ¿xã°
 */
int support_addr_zip(void)
{
  unsigned char zipcode[MAXKEYLEN];
  unsigned char addr[MAXKEYLEN], *ptr;
  int len, ziplen;

  ptr = sc + epos(SUPPORT_ZIP, file_ele[SUPPORT]);
  if(!spaces(ptr))
    return DBOK;
  ptr = sc + epos(SUPPORT_ADDR, file_ele[SUPPORT]);
  if(spaces(ptr))
    return DBOK;
  strcpy(addr, sc + epos(SUPPORT_ADDR, file_ele[SUPPORT]));
  len = strlen(addr);
  while(isspace(addr[len-1])) {
    addr[len-1] = NULL;
    len--;
  }
  if(findzipcode(addr, zipcode)) {
    len = strlen(sc+epos(SUPPORT_ZIP, file_ele[SUPPORT]));
    ziplen = strlen(zipcode);
    while(ziplen < len)
      zipcode[ziplen++] = ' ';
    zipcode[ziplen] = NULL;
    strcpy(sc+epos(SUPPORT_ZIP, file_ele[SUPPORT]), zipcode);
    put_field(SUPPORT_ZIP);
  }
  return DBOK;
}

/*
 * “Å∂•“A∂•∏aûaµA¨· ∫Å≠°ù° ª°µb§Â—° ¿xã°
 */
int support_addr_ddd(void)
{
  unsigned char *ptr, *addr;
  unsigned char ddd[MAXKEYLEN], tmp[MAXKEYLEN];

  ptr = sc + epos(SUPPORT_AREA, file_ele[SUPPORT]);
  if(!spaces(ptr))
    return DBOK;
  addr = sc + epos(SUPPORT_ADDR, file_ele[SUPPORT]);
  if(spaces(addr))
    return DBOK;
  if(addr2ddd(addr, ddd)) {
    sprintf(tmp, "%4s", ddd);
    strcpy(ptr, tmp);
    put_field(SUPPORT_AREA);
  }
  return DBOK;
}

/*
 * ƒëœa¿qàa∏a∏aûaµA¨· ∂ÅÕe§Â—°ù° ∫Å≠°¿xã°
 */
int camp_zip_addr(void)
{
  unsigned char *zipcode, *ptr;
  unsigned char addr[MAXKEYLEN];
  int len, addrlen;

  ptr = sc + epos(CAMP_ADDR, file_ele[CAMP]);
  if(!spaces(ptr))
    return DBOK;
  ptr = sc + epos(CAMP_ZIP, file_ele[CAMP]);
  if(spaces(ptr))
    return DBOK;
  zipcode = sc + epos(CAMP_ZIP, file_ele[CAMP]);
  if(findaddr(zipcode, addr)) {
    len = strlen(sc+epos(CAMP_ADDR, file_ele[CAMP]));
    addrlen = strlen(addr);
    while(addrlen < len)
      addr[addrlen++] = ' ';
    addr[addrlen] = NULL;
    strcpy(sc+epos(CAMP_ADDR, file_ele[CAMP]), addr);
    put_field(CAMP_ADDR);
  }
  return DBOK;
}

/*
 * ƒëœa¿qàa∏a∏aûaµA¨· ∫Å≠°ù° ∂ÅÕe§Â—° ¿xã°
 */
int camp_addr_zip(void)
{
  unsigned char zipcode[MAXKEYLEN];
  unsigned char addr[MAXKEYLEN], *ptr;
  int len, ziplen;

  ptr = sc + epos(CAMP_ZIP, file_ele[CAMP]);
  if(!spaces(ptr))
    return DBOK;
  ptr = sc + epos(CAMP_ADDR, file_ele[CAMP]);
  if(spaces(ptr))
    return DBOK;
  strcpy(addr, sc + epos(CAMP_ADDR, file_ele[CAMP]));
  len = strlen(addr);
  while(isspace(addr[len-1])) {
    addr[len-1] = NULL;
    len--;
  }
  if(findzipcode(addr, zipcode)) {
    len = strlen(sc+epos(CAMP_ZIP, file_ele[CAMP]));
    ziplen = strlen(zipcode);
    while(ziplen < len)
      zipcode[ziplen++] = ' ';
    zipcode[ziplen] = NULL;
    strcpy(sc+epos(CAMP_ZIP, file_ele[CAMP]), zipcode);
    put_field(CAMP_ZIP);
  }
  return DBOK;
}

/*
 * ƒëœa¿qàa∏a∏aûaµA¨· ∫Å≠°ù° ª°µb§Â—° ¿xã°
 */
int camp_addr_ddd(void)
{
  unsigned char *ptr, *addr;
  unsigned char ddd[MAXKEYLEN], tmp[MAXKEYLEN];

  ptr = sc + epos(CAMP_AREA, file_ele[CAMP]);
  if(!spaces(ptr))
    return DBOK;
  addr = sc + epos(CAMP_ADDR, file_ele[CAMP]);
  if(spaces(addr))
    return DBOK;
  if(addr2ddd(addr, ddd)) {
    sprintf(tmp, "%4s", ddd);
    strcpy(ptr, tmp);
    put_field(CAMP_AREA);
  }
  return DBOK;
}

void init_prof_edit(void)
{
  elist = file_ele[PROF];
  edit(PROF_ZIP1, prof_zip1_addr1);
  edit(PROF_ADDR1, prof_addr1_zip1);
  edit(PROF_AREA1, prof_addr1_ddd1);
  edit(PROF_ZIP2, prof_zip2_addr2);
  edit(PROF_ADDR2, prof_addr2_zip2);
  edit(PROF_AREA2, prof_addr2_ddd2);
}

void init_group_edit(void)
{
  elist = file_ele[GROUP];
  edit(GROUP_ZIP, group_zip_addr);
  edit(GROUP_ADDR, group_addr_zip);
  edit(GROUP_AREA, group_addr_ddd);
}

void init_media_edit(void)
{
  elist = file_ele[MEDIA];
  edit(MEDIA_ZIP, media_zip_addr);
  edit(MEDIA_ADDR, media_addr_zip);
  edit(MEDIA_AREA, media_addr_ddd);
}

void init_support_edit(void)
{
  elist = file_ele[SUPPORT];
  edit(SUPPORT_ZIP, support_zip_addr);
  edit(SUPPORT_ADDR, support_addr_zip);
  edit(SUPPORT_AREA, support_addr_ddd);
}

void init_camp_edit(void)
{
  elist = file_ele[CAMP];
  edit(CAMP_ZIP, camp_zip_addr);
  edit(CAMP_ADDR, camp_addr_zip);
  edit(CAMP_AREA, camp_addr_ddd);
}


void end_msg(void)
{
  unsigned char msg[] = "+ Minumyon Ver 1.00    Copyright(C) 1993  Kwon Jae Rock & Kim Hyun Soo";

  printf("%s\n", msg);
}

void init_sys_color(void)
{
  int isHGC;

  isHGC = _detecthgc();
  if(!isHGC) {
    fcolor = WHITE, bcolor = LIGHTBLUE;
    menu_fcolor = WHITE, menu_bcolor = BLUE;
  }
  else {
    fcolor = WHITE, bcolor = BLACK;
    menu_fcolor = BLACK, menu_bcolor = WHITE;
  }
}

char screen_buf[80*25*2];
int tmp_x, tmp_y;

void init_scrn(void)
{
  int i;

  cputs("\n\r                    >>>    Welcom to Minumyon World    <<<\n\r\n\r");
  tmp_x = wherex();
  tmp_y = wherey();
  gettext(1, 1, 80, 25, screen_buf);
  for(i = 0; i < 80*25*2; i+=2)
    screen_buf[i+1] ^= 0x77;
  puttext(1, 1, 80, 25, screen_buf);

  for(i = 0; i < 25; i++) {
    gotoxy(1, 25);
    textbackground(BLACK);
    cputs("\n\r");
    delay(70);
  }
}

void end_scrn(void)
{
  int i;

  for(i = 1; i <= 25; i++) {
    delay(70);
    puttext(1, 1, 80, i, screen_buf+80*25*2-80*2*i);
  }
  for(i = 0; i < 80*25*2; i+=2)
    screen_buf[i+1] ^= 0x77;
  puttext(1, 1, 80, 25, screen_buf);
  gotoxy(tmp_x, tmp_y);
}

void main(void)
{
  int isquit;

  init_scrn();
  inithanlib(FIXRESMODE, HANDETECT, HAN8GD2, ENGGD5);
  hsetbufcursor(FRAMECURSOR, FRAMECURSOR, DEFAULTCOLOR);
  registeregrfont(EGR1);
  registerhgrfont(HGR1);
  initusermenu(menu_fcolor, menu_bcolor, 1);
  inithwindow(fcolor, bcolor);

  init_update_func();

  if(!init_cfg()) {
    return;
  }

  db_open("", fl);
  background();
  about_minum();

  do {
    isquit = main_loop();
  } while(!isquit);

  db_cls();
  closehan();
  end_scrn();
  end_msg();
}


