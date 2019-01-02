#include <mem.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <alloc.h>
#include <process.h>
#include <conio.h>
#include "hanlib.h"
#include "hanmenu.h"
#include "hanwindw.h"
#include "ascii.h"
#include "extkey.h"
#include "hanin.h"
#include "handbms.h"
#include "hanerro2.h"
#include "hanojark.h"
#include "hanprint.h"
#include "hanpop.h"
#include "hanzip.h"
#include "hanpcx.h"

#define  OJARKPCX   "hanyang.pcx"

#define  setcurstatus(status)   (curstatus = (status))
#define  getcurstatus()         (curstatus)

enum status { STUDENT_STATUS, GRADUATE_STATUS };
enum menu_no { UTIL_MENU, DOMAIN_MENU, BROWSE_MENU, SEARCH_MENU, OUTPUT_MENU, INPUT_MENU, CONFIG_MENU };

enum items_menu0 { ABOUT_OJARK, ABOUT_HANTLE, NULL0, CALENDAR, CALCULATOR, OSSHELL, QUIT };
enum items_menu1 { STUDENT_DOMAIN, NULL5, GRADUATE_DOMAIN };
enum items_menu2 { BROWSE_BY_NUM, BROWSE_BY_NAME, BROWSE_LAB, BROWSE_PART, BROWSE_DUTY, BROWSE_COMPANY };
enum items_menu3 { SEARCH_BY_NUM, SEARCH_BY_NAME };
enum items_menu4 { PRINT_BY_NUM, PRINT_BY_NAME, PRINT_LAB, PRINT_PART, PRINT_DUTY, PRINT_COMPANY };
enum items_menu5 { STUDENT_CARD, PART_CARD, DUTY_CARD, ST_ARRANGE, GRADUATE_CARD, LAB_CARD, GR_ARRANGE, COMPANY_CARD };
enum items_menu6 { USER_REGISTER, PASSWD_REGISTER };

static int curstatus = STUDENT_STATUS;

int fl[] = { STUDENTS, GRADUATES, PARTS, DUTIES, COMPANIES, LABS, -1 };
int ele[] = { 0, -1 };
struct coord_t student_crd[] = {
  { 10, 2 }, { 44, 2 }, { 66, 2 }, { 18, 4 }, { 48, 4 }, { 66, 4 },
  { 14, 6 }, { 44, 6 }, { 64, 6 }, { 14, 8 }, { 48, 8 }, { 14,10 },
  { 48,10 }, { 14,12 }, { 46,12 }, { 18,14 }, { 10,16 }, { 48,14 },
  { 67,14 }, { 18,18 }, { 10,20 }, { 48,18 }, { 67,18 }
};
struct coord_t graduate_crd[] = {
  { 10, 2 }, { 44, 2 }, { 66, 2 }, { 18, 4 }, { 48, 4 }, { 66, 4 },
  { 10, 6 }, { 44, 6 }, { 72, 6 }, { 14, 8 }, { 48, 8 }, { 14,10 },
  { 48,10 }, { 12,12 }, { 46,12 }, { 18,14 }, { 10,16 }, { 48,14 },
  { 67,14 }, { 18,18 }, { 10,20 }, { 48,18 }, { 67,18 }
};
struct coord_t part_crd[] = {
  { 33, 8 }, { 33,10 }
};
struct coord_t duty_crd[] = {
  { 33, 8 }, { 33,10 }
};
struct coord_t company_crd[] = {
  { 33, 8 }, { 33,10 }
};
struct coord_t lab_crd[] = {
  { 21, 8 }, { 47, 8 }, { 21, 10 }
};
static char name[40], number[40];
static int fcolor, bcolor, menu_fcolor, menu_bcolor;
extern int *elist;

void initusermenu(int fcolor, int bcolor, int lcolor);
void setstatus(int status);
int util_menu(void);
void domain_menu(void);
void browse_menu(void);
void student_search_by_name(void);
void graduate_search_by_name(void);
void student_search_by_number(void);
void graduate_search_by_number(void);
void search_menu(void);
void student_update_by_name(void);
void graduate_update_by_name(void);
void student_update_by_number(void);
void graduate_update_by_number(void);
void update_menu(void);
void output_menu(void);
void input_menu(void);
void config_menu(void);
int main_loop(void);
void background(void);
void about_ojark(void);
void about_hantle(void);
void init_st_edit(void);
void init_gr_edit(void);

void initusermenu(int fcolor, int bcolor, int init_menu)
{
  nmenu = 7;         /* ¿aü±Œa àïÆÅ 6 àÅ */

  pmenu[0].menu = "‘";
  pmenu[0].nitem = 7;
  pmenu[0].item[0] = "µ°∏bäa.... ";  pmenu[0].shortcut[0] = 0;
  pmenu[0].item[1] = "–eÀi¥aü°";     pmenu[0].shortcut[1] = F1;
  pmenu[0].item[2] = "-";            pmenu[0].shortcut[2] = 0;
  pmenu[0].item[3] = "îiùb";         pmenu[0].shortcut[3] = 0;
  pmenu[0].item[4] = "âÅ¨eã°";       pmenu[0].shortcut[4] = 0;
  pmenu[0].item[5] = "ï°Øa°wùw¥·";   pmenu[0].shortcut[5] = ALT_F10;
  pmenu[0].item[6] = "è{";           pmenu[0].shortcut[6] = ALT_X;

  pmenu[1].menu = "–b¶Å‘ùîÅ–b∂•";
  pmenu[1].nitem = 3;
  pmenu[1].item[0] = "–b¶Åµwµb∑aù°";  pmenu[1].shortcut[0] = CTRL_S;
  pmenu[1].item[1] = "-";             pmenu[1].shortcut[1] = 0;
  pmenu[1].item[2] = "îÅ–b∂•µwµb∑aù°";pmenu[1].shortcut[2] = CTRL_G;

  pmenu[2].menu = "•°ã°";
  pmenu[2].nitem = 6;
  pmenu[2].item[0] = "–b§ÂÆÖ¨·ù°";    pmenu[2].shortcut[0] = ALT_N;
  pmenu[2].item[1] = "∑°üqÆÖ¨·ù°";    pmenu[2].shortcut[1] = ALT_O;
  pmenu[2].item[2] = "µeäÅØ©";        pmenu[2].shortcut[2] = 0;
  pmenu[2].item[3] = "–b“A¶Å¨·";      pmenu[2].shortcut[3] = 0;
  pmenu[2].item[4] = "â¡êÅ ª¢¿Ç";     pmenu[2].shortcut[4] = 0;
  pmenu[2].item[5] = "¨e–b“A¨a";      pmenu[2].shortcut[5] = 0;

  pmenu[3].menu = "¿xã°";
  pmenu[3].nitem = 2;
  pmenu[3].item[0] = "–b§Â∑aù°";      pmenu[3].shortcut[0] = ALT_F;
  pmenu[3].item[1] = "∑°üq∑aù°";      pmenu[3].shortcut[1] = ALT_A;

  pmenu[4].menu = "ø¢ã°";
  pmenu[4].nitem = 6;
  pmenu[4].item[0] = "–b§ÂÆÖ¨·ù°";    pmenu[4].shortcut[0] = ALT_P;
  pmenu[4].item[1] = "∑°üqÆÖ¨·ù°";    pmenu[4].shortcut[1] = ALT_R;
  pmenu[4].item[2] = "µeäÅØ©";        pmenu[4].shortcut[2] = ALT_L;
  pmenu[4].item[3] = "–b“A¶Å¨·";      pmenu[4].shortcut[3] = 0;
  pmenu[4].item[4] = "â¡êÅ ª¢¿Ç";     pmenu[4].shortcut[4] = 0;
  pmenu[4].item[5] = "¨e–b“A¨a";      pmenu[4].shortcut[5] = 0;

  pmenu[5].menu = "ê˝ã°";
  pmenu[5].nitem = 8;
  pmenu[5].item[0] = "–b¶Å¨ó Ø•¨w";   pmenu[5].shortcut[0] = ALT_S;
  pmenu[5].item[1] = "–b“A¶Å¨·";      pmenu[5].shortcut[1] = 0;
  pmenu[5].item[2] = "â¡êÅ ª¢¿Ç";     pmenu[5].shortcut[2] = 0;
  pmenu[5].item[3] = "–bëe π°∏˜";     pmenu[5].shortcut[3] = 0;
  pmenu[5].item[4] = "îÅ–b∂•¨ó Ø•¨w"; pmenu[5].shortcut[4] = ALT_G;
  pmenu[5].item[5] = "µeäÅØ©";        pmenu[5].shortcut[5] = 0;
  pmenu[5].item[6] = "ã°ÆÅ π°∏˜";     pmenu[5].shortcut[6] = 0;
  pmenu[5].item[7] = "¨e–b“A¨a";      pmenu[5].shortcut[7] = 0;

  pmenu[6].menu = "ã°»a";
  pmenu[6].nitem = 2;
  pmenu[6].item[0] = "¨a∂w∏a ówù¢";   pmenu[6].shortcut[0] = ALT_U;
  pmenu[6].item[1] = "¥q—° ówù¢";     pmenu[6].shortcut[1] = ALT_D;

  definemenu(fcolor, bcolor, init_menu, 2);
}

void setstatus(int status)
{
  setcurstatus(status);
  switch(status) {
    case STUDENT_STATUS :
      pmenu[1].itemable[0] = FALSE;
      pmenu[1].itemable[2] = TRUE;

      pmenu[2].itemable[3] = TRUE;
      pmenu[2].itemable[4] = TRUE;

      pmenu[2].itemable[2] = FALSE;

      pmenu[4].itemable[2] = FALSE;
      pmenu[4].itemable[3] = TRUE;
      pmenu[4].itemable[4] = TRUE;

      pmenu[5].itemable[0] = TRUE;
      pmenu[5].itemable[1] = TRUE;
      pmenu[5].itemable[2] = TRUE;
      pmenu[5].itemable[3] = TRUE;

      pmenu[5].itemable[4] = FALSE;
      pmenu[5].itemable[5] = FALSE;
      pmenu[5].itemable[6] = FALSE;
      break;
    case GRADUATE_STATUS :
      pmenu[1].itemable[0] = TRUE;
      pmenu[1].itemable[2] = FALSE;

      pmenu[2].itemable[3] = FALSE;
      pmenu[2].itemable[4] = FALSE;

      pmenu[2].itemable[2] = TRUE;

      pmenu[4].itemable[2] = TRUE;
      pmenu[4].itemable[3] = FALSE;
      pmenu[4].itemable[4] = FALSE;

      pmenu[5].itemable[0] = FALSE;
      pmenu[5].itemable[1] = FALSE;
      pmenu[5].itemable[2] = FALSE;
      pmenu[5].itemable[3] = FALSE;

      pmenu[5].itemable[4] = TRUE;
      pmenu[5].itemable[5] = TRUE;
      pmenu[5].itemable[6] = TRUE;
      break;
  } /* switch */
  drawmenubar();
}

void dosshell(void)
{
  char *comspec;

  closehan();
  comspec = (char*)getenv("COMSPEC");
  cprintf("         …ÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕª\n\r");
  cprintf("         ∫                  Ojarkyo Dos Shell                 ∫\n\r");
  cprintf("         «ƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒ∂\n\r");
  cprintf("         ∫                                                    ∫\n\r");
  cprintf("         ∫      To return Ojarkyo, type 'EXIT'. 1992 Kwon     ∫\n\r");
  cprintf("         ∫                                                    ∫\n\r");
  cprintf("         »ÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕº\n\r\n\r");

  putenv("PROMPT=Type EXIT to Return to Ojarkyo$_$_(Ojarkyo)$p$g");
  if(spawnl(P_WAIT, comspec, comspec, NULL) == -1) {
    cprintf("\n\r\n\r%s missing or Memory insufficent.   Press any key.... \n\r\n\r");
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

  switch(getcuritem()) {
    case ABOUT_OJARK :
      about_ojark();
      break;
    case ABOUT_HANTLE :
      about_hantle();
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

void domain_menu(void)
{
  switch(getcuritem()) {
    case STUDENT_DOMAIN :
      setstatus(STUDENT_STATUS);
      break;
    case GRADUATE_DOMAIN :
      setstatus(GRADUATE_STATUS);
      break;
  }
}

void browse_menu()
{
  switch(getcuritem()) {
    case BROWSE_BY_NUM :
      if(getcurstatus() == STUDENT_DOMAIN) {
        init_st_edit();
        db_browse(STUDENTS, "‘ê  –b  ¶Å  ¨ó    ∏a  ûa  •°  ã°  ‘ë", ele, 1, B2UPDATE);
      }
      else {
        init_gr_edit();
        db_browse(GRADUATES, "‘ê  îÅ  –b  ∂•  ¨ó    ∏a  ûa  •°  ã°  ‘ë", ele, 1, B2UPDATE);
      }
      break;
    case BROWSE_BY_NAME :
      if(getcurstatus() == STUDENT_DOMAIN) {
        init_st_edit();
        db_browse(STUDENTS, "‘ê  –b  ¶Å  ¨ó    ∏a  ûa  •°  ã°  ‘ë", ele, 2, B2UPDATE);
      }
      else {
        init_gr_edit();
        db_browse(GRADUATES, "‘ê  îÅ  –b  ∂•  ¨ó    ∏a  ûa  •°  ã°  ‘ë", ele, 2, B2UPDATE);
      }
      break;
    case BROWSE_LAB :
      db_browse(LABS, "‘ê  îÅ  –b  ∂•   µe  äÅ  Ø©   ∏a  ûa  •°  ã°  ‘ë", ele, 1, B2UPDATE);
      break;
    case BROWSE_PART :
      db_browse(PARTS, "‘ê  –b  “A  ¶Å  ¨·   µe  äÅ  Ø©   ∏a  ûa  •°  ã°  ‘ë", ele, 1, B2UPDATE);
      break;
    case BROWSE_DUTY :
      db_browse(DUTIES, "‘ê  â¡  êÅ  ª¢  ¿Ç   ∏a  ûa  •°  ã°  ‘ë", ele, 1, B2UPDATE);
      break;
    case BROWSE_COMPANY :
      db_browse(COMPANIES, "‘ê  ¨e  –b  “A  ¨a   µe  äÅ  Ø©   ∏a  ûa  •°  ã°  ‘ë", ele, 1, B2UPDATE);
      break;
  }
}

void update_student(int file_no, char *buf)
{
  init_st_edit();
  db_update(file_no, "‘ê  –b  ¶Å  ¨ó    Ø•  ¨w  â°  √°  ã°  ‘ë", ele, student_crd, 1, buf);
}

void update_graduate(int file_no, char *buf)
{
  init_gr_edit();
  db_update(file_no, "‘ê  îÅ  –b  ∂•  ¨ó    Ø•  ¨w  â°  √°  ã°  ‘ë", ele, graduate_crd, 1, buf);
}

void update_lab(int file_no, char *buf)
{
  db_update(file_no, "‘ê  µe  äÅ  Ø©  ∏a  ûa   â°  √°  ã°  ‘ë", ele, lab_crd, 1, buf);
}

void update_duty(int file_no, char *buf)
{
  db_update(file_no, "‘ê  â¡  êÅ  ª¢  ¿Ç  ∏a  ûa   â°  √°  ã°  ‘ë", ele, duty_crd, 1, buf);
}

void update_company(int file_no, char *buf)
{
  db_update(file_no, "‘ê  ¨e  –b  “A  ¨a  ∏a  ûa   â°  √°  ã°  ‘ë", ele, company_crd, 1, buf);
}

void update_part(int file_no, char *buf)
{
  db_update(file_no, "‘ê  –b  “A  ¶Å  ¨·  ∏a  ûa   â°  √°  ã°  ‘ë", ele, part_crd, 1, buf);
}

void init_update_func(void)
{
  browse2update[STUDENTS] = update_student;
  browse2update[GRADUATES] = update_graduate;
  browse2update[PARTS] = update_part;
  browse2update[DUTIES] = update_duty;
  browse2update[LABS] = update_lab;
  browse2update[COMPANIES] = update_company;
}

/*
 *  –b¶Å¨ó ∑°üq∑aù° ¿xã°
 */
void student_search_by_name(void)
{
  int ans;

  ans = win_hgetdata(10, " ‘Ñ ¿x∑i ¨aúq∑Å ∑°üq∑i ∑≥ùb–a≠A∂a ", "XXXXXXXXXX", name);
  if(ans) {
    init_st_edit();
    db_search(STUDENTS, "‘ê  –b  ¶Å  ¨ó    Ø•  ¨w  •°  ã°  ‘ë", ele, student_crd, 2, name);
  }
}

/*
 *  îÅ–b∂•¨ó ∑°üq∑aù° ¿xã°
 */
void graduate_search_by_name(void)
{
  int ans;

  ans = win_hgetdata(10, " ‘Ñ ¿x∑i ¨aúq∑Å ∑°üq∑i ∑≥ùb–a≠A∂a ", "XXXXXXXXXX", name);
  if(ans) {
    init_gr_edit();
    db_search(GRADUATES, "‘ê  îÅ  –b  ∂•  ¨ó    Ø•  ¨w  •°  ã°  ‘ë", ele, graduate_crd, 2, name);
  }
}

/*
 *  –b¶Å¨ó –b§Â∑aù° ¿xã°
 */
void student_search_by_number(void)
{
  int ans;

  ans = win_hgetdata(8, " ‘Ñ ¿x∑i ¨aúq∑Å –b§Â∑i ∑≥ùb–a≠A∂a ", "99999999", number);
  if(ans) {
    init_st_edit();
    db_search(STUDENTS, "‘ê  –b  ¶Å  ¨ó    Ø•  ¨w  •°  ã°  ‘ë", ele, student_crd, 1, number);
  }
}

/*
 *  îÅ–b∂•¨ó –b§Â∑aù° ¿xã°
 */
void graduate_search_by_number(void)
{
  int ans;

  ans = win_hgetdata(8, " ‘Ñ ¿x∑i ¨aúq∑Å –b§Â∑i ∑≥ùb–a≠A∂a ", "99999999", number);
  if(ans) {
    init_gr_edit();
    db_search(GRADUATES, "‘ê  îÅ  –b  ∂•  ¨ó    Ø•  ¨w  •°  ã°  ‘ë", ele, graduate_crd, 1, number);
  }
}

void search_menu(void)
{
  switch(getcuritem()) {
    case SEARCH_BY_NUM :
      if(getcurstatus() == STUDENT_STATUS)
        student_search_by_number();
      else
        graduate_search_by_number();
      break;
    case SEARCH_BY_NAME :
      if(getcurstatus() == STUDENT_STATUS)
        student_search_by_name();
      else
        graduate_search_by_name();
      break;
  }
}

void print_by_st_num(int ans)
{
  int *ele_ptr;

  if(ans == 0 || ans == 2) {
    ele_ptr = get_print_list(STUDENTS, "‘ê  ∑•  ≠·  –i  –w  °¢   â°  üa  ã°  ‘ë", ele, student_crd);
    if(ele_ptr)
      db_print(STUDENTS, ">>  –b  ¶Å  ¨ó   °¢  ù¢  <<", ele_ptr, 1, (ans == 0) ? DATA2PRN : DATA2FILE);
  }
  else if(ans == 1 || ans == 3)
    db_print(STUDENTS, ">>  –b  ¶Å  ¨ó   °¢  ù¢  <<", ele, 1, (ans == 1) ? DATA2PRN : DATA2FILE);
}

void print_by_st_name(int ans)
{
  int *ele_ptr;

  if(ans == 0 || ans == 2) {
    ele_ptr = get_print_list(STUDENTS, "‘ê  ∑•  ≠·  –i  –w  °¢   â°  üa  ã°  ‘ë", ele, student_crd);
    if(ele_ptr)
      db_print(STUDENTS, ">>  –b  ¶Å  ¨ó   °¢  ù¢  <<", ele_ptr, 2, (ans == 0) ? DATA2PRN : DATA2FILE);
  }
  else if(ans == 1 || ans == 3)
    db_print(STUDENTS, ">>  –b  ¶Å  ¨ó   °¢  ù¢  <<", ele, 2, (ans == 1) ? DATA2PRN : DATA2FILE);
}

void print_by_gr_num(int ans)
{
  int *ele_ptr;

  if(ans == 0 || ans == 2) {
    ele_ptr = get_print_list(GRADUATES, "‘ê  ∑•  ≠·  –i  –w  °¢   â°  üa  ã°  ‘ë", ele, graduate_crd);
    if(ele_ptr)
      db_print(GRADUATES, ">>  îÅ  –b  ∂•  ¨ó   °¢  ù¢  <<", ele_ptr, 1, (ans == 0) ? DATA2PRN : DATA2FILE);
  }
  else if(ans == 1 || ans == 3)
    db_print(GRADUATES, ">>  îÅ  –b  ∂•  ¨ó   °¢  ù¢  <<", ele, 1, (ans == 1) ? DATA2PRN : DATA2FILE);
}

void print_by_gr_name(int ans)
{
  int *ele_ptr;

  if(ans == 0 || ans == 2) {
    ele_ptr = get_print_list(GRADUATES, "‘ê  ∑•  ≠·  –i  –w  °¢   â°  üa  ã°  ‘ë", ele, graduate_crd);
    if(ele_ptr)
      db_print(GRADUATES, ">>  îÅ  –b  ∂•  ¨ó   °¢  ù¢  <<", ele_ptr, 2, (ans == 0) ? DATA2PRN : DATA2FILE);
  }
  else if(ans == 1 || ans == 3)
    db_print(GRADUATES, ">>  îÅ  –b  ∂•  ¨ó   °¢  ù¢  <<", ele, 2, (ans == 1) ? DATA2PRN : DATA2FILE);
}

void print_lab(int ans)
{
  int *ele_ptr;

  if(ans == 0 || ans == 2) {
    ele_ptr = get_print_list(LABS, "‘ê  ∑•  ≠·  –i  –w  °¢   â°  üa  ã°  ‘ë", ele, lab_crd);
    if(ele_ptr)
      db_print(LABS, ">>  µe  äÅ  Ø©   °¢  ù¢  <<", ele_ptr, 1, (ans == 0) ? DATA2PRN : DATA2FILE);
  }
  else if(ans == 1 || ans == 3)
    db_print(LABS, ">>  µe  äÅ  Ø©   °¢  ù¢  <<", ele, 1, (ans == 1) ? DATA2PRN : DATA2FILE);
}

void print_part(int ans)
{
  int *ele_ptr;

  if(ans == 0 || ans == 2) {
    ele_ptr = get_print_list(PARTS, "‘ê  ∑•  ≠·  –i  –w  °¢   â°  üa  ã°  ‘ë", ele, part_crd);
    if(ele_ptr)
      db_print(PARTS, ">>  â¡  êÅ  ¶Å  ¨·   °¢  ù¢  <<", ele_ptr, 1, (ans == 0) ? DATA2PRN : DATA2FILE);
  }
  else if(ans == 1 || ans == 3)
    db_print(PARTS, ">>  â¡  êÅ  ¶Å  ¨·   °¢  ù¢  <<", ele, 1, (ans == 1) ? DATA2PRN : DATA2FILE);
}

void print_duty(int ans)
{
  int *ele_ptr;

  if(ans == 0 || ans == 2) {
    ele_ptr = get_print_list(DUTIES, "‘ê  ∑•  ≠·  –i  –w  °¢   â°  üa  ã°  ‘ë", ele, duty_crd);
    if(ele_ptr)
      db_print(DUTIES, ">>  â¡  êÅ  ª¢  ¿Ç   °¢  ù¢  <<", ele_ptr, 1, (ans == 0) ? DATA2PRN : DATA2FILE);
  }
  else if(ans == 1 || ans == 3)
    db_print(DUTIES, ">>  â¡  êÅ  ª¢  ¿Ç   °¢  ù¢  <<", ele, 1, (ans == 1) ? DATA2PRN : DATA2FILE);
}

void print_company(int ans)
{
  int *ele_ptr;

  if(ans == 0 || ans == 2) {
    ele_ptr = get_print_list(COMPANIES, "‘ê  ∑•  ≠·  –i  –w  °¢   â°  üa  ã°  ‘ë", ele, company_crd);
    if(ele_ptr)
      db_print(COMPANIES, ">>  ¨e  –b  “A  ¨a   °¢  ù¢  <<", ele_ptr, 1, (ans == 0) ? DATA2PRN : DATA2FILE);
  }
  else if(ans == 1 || ans == 3)
    db_print(COMPANIES, ">>  ¨e  –b  “A  ¨a   °¢  ù¢  <<", ele, 1, (ans == 1) ? DATA2PRN : DATA2FILE);
}

void output_menu()
{
  char *title = "‘Ñ ∑•≠· π°àÂ  ‘Ñ";
  char *menu[] = {
    " 1. π°àÂ¶Åù° ∑•≠·       ",
    " 2. °°ñÅ ∑•≠·           ",
    " 3. Ãa∑©ù° π°àÂ¶Å ∏·∏w  ",
    " 4. Ãa∑©ù° °°ñÅ ∏·∏w    "
  };
  int ans;

  ans = popup(hgetmaxax()/2-strlen(menu[0])/2, 10, 4, menu, title, "1234", 1);
  if(ans == -1)
     return;
  switch(getcuritem()) {
    case PRINT_BY_NUM :
      if(getcurstatus() == STUDENT_STATUS)
        print_by_st_num(ans);
      else
        print_by_gr_num(ans);
      break;
    case PRINT_BY_NAME :
      if(getcurstatus() == STUDENT_STATUS)
        print_by_st_name(ans);
      else
        print_by_gr_name(ans);
      break;
    case PRINT_LAB :
      print_lab(ans);
      break;
    case PRINT_PART :
      print_part(ans);
      break;
    case PRINT_DUTY :
      print_duty(ans);
      break;
    case PRINT_COMPANY :
      print_company(ans);
      break;
  }
}

/*
 * –bëe π°∏˜
 *
 * –bëe∑e 1 ~ 4–bëe∑° ∑∂îa.
 * –e –bëe≥¢ µ©ü© òÅ 4–bëeói∑e ∏aï∑∑aù° "∏Å–bµa¶Å"–w°¢µA ∏Å–b --> π©¥Û∑aù°.
 * –e –bëe≥¢ êÅü© òÅ 1–bëeói∑e 0–bëe∑aù° §aéÖîa. µ·ê°–a°e ∏i°µ π°∏˜ñE âw∂Å
 * ∂•¨w •¢äÅ–aã°∂·–Å¨·∑°îa.
 */
void st_grade_arrange(void)
{
  char *title = "‘Ñ –bëe π°∏˜ ‘Ñ";
  char *menu[] = {
    " 1. –e –bëe≥¢ µ©ü°ã°     ",
    " 2. –e –bëe≥¢ êÅü°ã°     ",
  };
  char *msg[] = {
    "–e –bëe≥¢ µ©ü°â° ∑∂Øsì°îa",
    "–e –bëe≥¢ êÅü°â° ∑∂Øsì°îa"
  };
  int ans, tmp;
  char *buf, *ptr1, *ptr2;

  ans = popup(hgetmaxax()/2-strlen(menu[0])/2, 10, 2, menu, title, "12", 1);
  if(ans == -1)
     return;
  if(yesno(" ∏Â¡A ∏aûaàa àóØ•ñSì°îa. —¬Ø©–sì°åa? ", 1) != 0)
    return;
  if( !wopen(hgetmaxax()/2-(strlen(msg[0])+6)/2-1, hgetmaxay()/2-1, strlen(msg[0])+6, 1))
    return;
  errno = 0;
  buf = (char *)farmalloc(rlen(STUDENTS));
  if(!buf) {
    wclose();
    memerror();
    return;
  }
  first_rcd(STUDENTS, 1, buf);
  if(ans == 0) {
    wtitle(msg[0]);
    while(errno != D_EOF) {
      ptr1 = buf + epos(ST_GRADE, file_ele[STUDENTS]);
      if(!spaces(ptr1)) {
        tmp = atoi(ptr1);
        if(tmp >= 0 && tmp <= 4) {
          tmp++;
          if(tmp == 5) {
            ptr2 =  buf + epos(ST_ABSENCE, file_ele[STUDENTS]);
            if(*ptr2 == '0') { /* ∏Å–b∫ó∑• âw∂Å†e */
              *ptr2 = '2'; /* π©¥Û */
              *ptr1 = ' '; /* –bëe∑i ª°∂Öîa */
            }
          }
          else {
            *ptr1 = tmp+'0';
          } /* else */
          rtn_rcd(STUDENTS, buf);
        } /* if */
      } /* if */
      next_rcd(STUDENTS, 1, buf);
    } /* while */
  } /* if */
  else {
    wtitle(msg[1]);
    while(errno != D_EOF) {
      ptr1 = buf + epos(ST_GRADE, file_ele[STUDENTS]);
      if(!spaces(ptr1)) {
        tmp = atoi(ptr1);
        if(tmp >= 1 && tmp <= 4) {
          ptr2 =  buf + epos(ST_ABSENCE, file_ele[STUDENTS]);
          if(*ptr2 == '0') { /* ∏Å–b∫ó∑• âw∂Å†e */
            tmp--;
            *ptr1 = tmp+'0';
            rtn_rcd(STUDENTS, buf);
          }
        } /* if */
      } /* if */
      next_rcd(STUDENTS, 1, buf);
    } /* while */
  }
  farfree(buf);
  wclose();
  disp_cmsg_sec("  –bëe π°∏˜∑i †x¬Å¥ˆØsì°îa  ", 1);
}

/*
 * ã°ÆÅ π°∏˜
 *
 * ¨‚¨a ã°ÆÅìe 1 ~ 4ã°àa ∑∂îaâ° ••îa. ¥¢àe∑Å µÅ∂Aìe ÆÅ∏b¥ÛµA ∑Åπ•–aã° §aúeîa
 * 1ã°ÆÅ ≥¢ µ©ü© òÅ ¨‚¨a 4ã°ói∑e ∏aï∑∑aù° ∏Å–bµA¨· π©¥Û∑aù° •eâw.
 * §b¨aã°ÆÅìe 99àa ñI òÅåaª° âÅ≠¢ ªwàa.
 * 1ã°ÆÅ ≥¢ êÅü© òÅ 1ã°ói∑e 0ã°ù° §aéÖîa. ∏i°µ π°∏˜ñE âw∂Å, ∂•¨w•¢äÅüi ∂·–Å.
 */
void gr_grade_arrange(void)
{
  char *title = "‘Ñ ã°ÆÅ π°∏˜ ‘Ñ";
  char *menu[] = {
    " 1. –e ã°ÆÅ≥¢ µ©ü°ã°     ",
    " 2. –e ã°ÆÅ≥¢ êÅü°ã°     ",
  };
  char *msg[] = {
    "–e ã°ÆÅ≥¢ µ©ü°â° ∑∂Øsì°îa",
    "–e ã°ÆÅ≥¢ êÅü°â° ∑∂Øsì°îa"
  };
  int ans, tmp;
  char *buf, *ptr1, *ptr2;

  ans = popup(hgetmaxax()/2-strlen(menu[0])/2, 10, 2, menu, title, "12", 1);
  if(ans == -1)
     return;
  if(yesno(" ∏Â¡A ∏aûaàa àóØ•ñSì°îa. —¬Ø©–sì°åa? ", 1) != 0)
    return;
  if( !wopen(hgetmaxax()/2-(strlen(msg[0])+6)/2-1, hgetmaxay()/2-1, strlen(msg[0])+6, 1))
    return;
  errno = 0;
  buf = (char *)farmalloc(rlen(GRADUATES));
  if(!buf) {
    wclose();
    memerror();
    return;
  }
  first_rcd(GRADUATES, 1, buf);
  if(ans == 0) {
    wtitle(msg[0]);
    while(errno != D_EOF) {
      ptr1 = buf + epos(GR_GRADE, file_ele[GRADUATES]);
      if(!spaces(ptr1)) {
        tmp = atoi(ptr1);
        if(tmp >= 0 && tmp <= 4) {
          tmp++;
          if(tmp == 5) {
            ptr2 =  buf + epos(GR_COURSE, file_ele[GRADUATES]);
            if(*ptr2 == '1') {  /* ¨‚¨a */
              ptr2 = buf + epos(GR_ABSENCE, file_ele[GRADUATES]);
              if(*ptr2 == '0') { /* ∏Å–b */
                *ptr1 = ' ', *(ptr1+1) = ' '; /* ã°ÆÅìe ª°∂Åâ° */
                *ptr2 = '2'; /* π©¥Û */
              }
            }
            else {
              *ptr1 = tmp/10 ? (+'0') : ' ', *(ptr1+1) = tmp%10+'0';
            }
          }
          else {
            *ptr1 = tmp/10 ? (+'0') : ' ', *(ptr1+1) = tmp%10+'0';
          } /* else */
          rtn_rcd(GRADUATES, buf);
        } /* if */
      } /* if */
      next_rcd(GRADUATES, 1, buf);
    } /* while */
  } /* if */
  else {
    wtitle(msg[1]);
    while(errno != D_EOF) {
      ptr1 = buf + epos(GR_GRADE, file_ele[GRADUATES]);
      if(!spaces(ptr1)) {
        tmp = atoi(ptr1);
        if(tmp >= 1) {
          ptr2 = buf + epos(GR_ABSENCE, file_ele[GRADUATES]);
          if(*ptr2 == '0') { /* ∏Å–b */
            tmp--;
            *ptr1 = tmp/10 ? (+'0') : ' ', *(ptr1+1) = tmp%10+'0';
            rtn_rcd(GRADUATES, buf);
          }
        } /* if */
      } /* if */
      next_rcd(GRADUATES, 1, buf);
    } /* while */
  }
  farfree(buf);
  wclose();
  disp_cmsg_sec("  ã°ÆÅ π°∏˜∑i †x¬Å¥ˆØsì°îa  ", 1);
}

void input_menu(void)
{
  switch(getcuritem()) {
    case STUDENT_CARD :
      init_st_edit();
      db_input(STUDENTS, "–b  ¶Å  ¨ó    Ø•  ¨w  ∑≥  ùb", ele, student_crd);
      break;
    case PART_CARD :
      db_input(PARTS, "â¡  êÅ  ¶Å  ¨·    ∏a  ûa  ∑≥  ùb", ele, part_crd);
      break;
    case DUTY_CARD :
      db_input(DUTIES, "â¡  êÅ  ª¢  ¿Ç    ∏a  ûa  ∑≥  ùb", ele, duty_crd);
      break;
    case ST_ARRANGE :
      st_grade_arrange();
      break;
    case GRADUATE_CARD :
      init_gr_edit();
      db_input(GRADUATES, "îÅ  –b  ∂•  ¨ó    Ø•  ¨w  ∑≥  ùb", ele, graduate_crd);
      break;
    case LAB_CARD :
      db_input(LABS, "µe  äÅ  Ø©    ∏a  ûa  ∑≥  ùb", ele, lab_crd);
      break;
    case GR_ARRANGE :
      gr_grade_arrange();
      break;
    case COMPANY_CARD :
      db_input(COMPANIES, "¨e  –b  “A  ¨a    ∏a  ûa  ∑≥  ùb", ele, company_crd);
      break;
  } /* switch */
}

void hookhgetln(int *keycodep)
{
  if (keycodep == NULL) {  /* Initialization before key input */
    return;
  }
  switch (*keycodep) {
    case DOWNARROW : case '\t' : case UPARROW :
      *keycodep = CR;  /* If *keycodep = CR or ESC, quit hgetline */
      return;
  }
  return;
}

#define  CFGFILE      "OJARKYO.CFG"
#define  CFGID        "Ojarkyo Config File 1.00"
#define  MAXCYPERKEY  31

/*
 *  äÅ¨˜ Ãa∑©∑Å äÅπ°  1.00
 *  Ø¢•i∏a - 32§a∑°Àa
 *  Ojarkyo Config File 1.00<EOF>
 *  –bäa - 32§a∑°Àa
 *  –bâ¡ - 32§a∑°Àa
 *  ¥q—° - 32§a∑°Àa
 *
 *  Ãa∑©µA ≥i òÅ†e ¥q—°—¡–aâ° °A°°ü° ¨wµAìe ¥ÂπAêa ¥q—°àa Œâü• ¨w»Åù° ∑Aª°.
 */
struct cfg_t {
  unsigned char id[32];
  unsigned char college[32];
  unsigned char dep[32];
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
  cyper(cfg.college);
  cyper(cfg.dep);
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

bool init_cfg(void)
{
  FILE *fcfg;

  fcfg = fopen(CFGFILE, "rb");
  if(fcfg) {
    fread(&cfg, sizeof(struct cfg_t), 1, fcfg);
    cyper_cfg();
    cyper(cfg.passwd);
    if(!spaces(cfg.passwd[0]) && cfg.passwd[0]) {
      if(!check_passwd()) {
        disperror(" ¥q—°àa Àiü≥ì°îa ");
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
  int line;
  int allowcanceldelline, restoreline, cuttail, allowfirstdelline;
  unsigned char str[40];
  FILE *fcfg;

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

  if(!wopen(hgetmaxax()/2-20, hgetmaxay()/2-2, 40, 4))
    return;
  wtitle("‘ ¨a∂w∏a ówù¢ ‘");
  hputsxy(2, 2, "–b äa");
  hputsxy(2, 3, "–b â¡");
  hputsxy(8, 2, cfg.college);
  hputsxy(8, 3, cfg.dep);
  allowcanceldelline = _allowcanceldelline, restoreline = _restoreline;
  cuttail = _cuttail, allowfirstdelline = _allowfirstdelline;
  _hookhgetln = hookhgetln;
  _cuttail = false, _allowcanceldelline = _restoreline = true;
  line = 0;
  while ( 1 ) {
    hgotoxy(8, 2+line);
    strcpy(str, line ? cfg.dep : cfg.college);
    hgetln(str, 31);
    if(_lastkey == '\r') {
      strcpy(line ? cfg.dep : cfg.college, str);
      if(line)
        break;
      else
        line = 1;
    }
    else if(_lastkey == ESC)
      break;
    else {
      strcpy(line ? cfg.dep : cfg.college, str);
      line = line ? 0 : 1;
    }
  } /* while */

  _allowcanceldelline = allowcanceldelline, _restoreline = restoreline;
  _cuttail = cuttail, _allowfirstdelline = allowfirstdelline;
  _hookhgetln = NULL;
  wclose();
  cyper_cfg();
  fwrite(&cfg, sizeof(struct cfg_t), 1, fcfg);
  cyper_cfg();
  fclose(fcfg);
}

void passwd_register(void)
{
  unsigned char str[40];
  FILE *fcfg;

  fcfg = fopen(CFGFILE, "rb");
  if(fcfg) {
    fread(&cfg, sizeof(struct cfg_t), 1, fcfg);
    cyper_cfg();
    cyper(cfg.passwd);
    if(!spaces(cfg.passwd[0]) && cfg.passwd[0]) {
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
  switch(getcuritem()) {
    case USER_REGISTER :
      user_register();
      break;
    case PASSWD_REGISTER :
      passwd_register();
      break;
  }
}

int main_loop(void)
{
  int isquit = FALSE, key = F10;

  setstatus(getcurstatus());
  if(getitem(key)) {
    switch(getcurmenu()) {
      case  UTIL_MENU :
        isquit = util_menu();
        break;
      case  DOMAIN_MENU :
        domain_menu();
        break;
      case  BROWSE_MENU :
        browse_menu();
        break;
      case  SEARCH_MENU :
        search_menu();
        break;
      case  OUTPUT_MENU :
        output_menu();
        break;
      case  INPUT_MENU :
        input_menu();
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
  if(isquit == false)
    background();
  return isquit;
}

void background(void)
{
  bool onoff;

  hclrscr();
  onoff = isreverse();
  hsetreverse(ON);
  eputchs(-1, -hgetmaxax(), 80, ' ');
  hputsxy(-(hgetmaxax()-14), -hgetmaxay(),
           getcurstatus() == STUDENT_STATUS ? "‘ê –b¶Å¨ó ‘ë" : "‘ê îÅ–b∂• ‘ë");
  eputchs(-1, -1, 80, ' ');
  hprintfxy(-2, -25, "‘ %s %s ‘", cfg.college, cfg.dep);
  hsetreverse(onoff);
}

void about_ojark(void)
{
  if(wopen(21, 9, 40, 8)) {
    wcenter(2, "‘  µ°  ∏b  äa  1.00 Ãe ‘");
    wcenter(4, "âe∂Åµ¡ ª¢ëaàa †eêaï°ù¢");
    wcenter(5, "ï°µ¡∫Åìe µ°∏bäa¿·úÒ");
    wcenter(6, "–b¨óâ¡ –b¨ó“Aüi µeâi–Å∫ìì°îa");
    wcenter(7, "–e ‘á Ài ‘á ¥a ‘á ü°");
    wait_getkey(5);
    wclose();
  }
}


void about_hantle(void)
{
  if(wopen(21, 9, 40, 8)) {
    wcenter(2, "‘  –e Ài ¥a ü°  ‘");
    view_pcx_file(11*8, 2*16, OJARKPCX, bcolor, fcolor);
    wait_getkey(5);
    wclose();
  }
}

/*
 * –b¶Å¨ó∏aûaµA¨· ∂ÅÕe§Â—°1ù° ∫Å≠°1¿xã°
 */
int st_zip1_addr1(void)
{
  unsigned char *zipcode, *ptr;
  unsigned char addr[MAXKEYLEN];
  int len, addrlen;

  ptr = sc + epos(ST_ADDR1, file_ele[STUDENTS]);
  if(!spaces(ptr))
    return DBOK;
  ptr = sc + epos(ST_ZIP1, file_ele[STUDENTS]);
  if(spaces(ptr))
    return DBOK;
  zipcode = sc + epos(ST_ZIP1, file_ele[STUDENTS]);
  if(findaddr(zipcode, addr)) {
    len = strlen(sc+epos(ST_ADDR1, file_ele[STUDENTS]));
    addrlen = strlen(addr);
    while(addrlen < len)
      addr[addrlen++] = ' ';
    addr[addrlen] = NULL;
    strcpy(sc+epos(ST_ADDR1, file_ele[STUDENTS]), addr);
    put_field(ST_ADDR1);
  }
  return DBOK;
}

/*
 * –b¶Å¨ó∏aûaµA¨· ∂ÅÕe§Â—°2ù° ∫Å≠°2¿xã°
 */
int st_zip2_addr2(void)
{
  unsigned char *zipcode, *ptr;
  unsigned char addr[MAXKEYLEN];
  int len, addrlen;

  ptr = sc + epos(ST_ADDR2, file_ele[STUDENTS]);
  if(!spaces(ptr))
    return DBOK;
  ptr = sc + epos(ST_ZIP2, file_ele[STUDENTS]);
  if(spaces(ptr))
    return DBOK;
  zipcode = sc + epos(ST_ZIP2, file_ele[STUDENTS]);
  if(findaddr(zipcode, addr)) {
    len = strlen(sc+epos(ST_ADDR2, file_ele[STUDENTS]));
    addrlen = strlen(addr);
    while(addrlen < len)
      addr[addrlen++] = ' ';
    addr[addrlen] = NULL;
    strcpy(sc+epos(ST_ADDR2, file_ele[STUDENTS]), addr);
    put_field(ST_ADDR2);
  }
  return DBOK;
}

/*
 * –b¶Å¨ó∏aûaµA¨· ∫Å≠°1ù° ∂ÅÕe§Â—°1 ¿xã°
 */
int st_addr1_zip1(void)
{
  unsigned char zipcode[MAXKEYLEN];
  unsigned char addr[MAXKEYLEN], *ptr;
  int len, ziplen;

  ptr = sc + epos(ST_ZIP1, file_ele[STUDENTS]);
  if(!spaces(ptr))
    return DBOK;
  ptr = sc + epos(ST_ADDR1, file_ele[STUDENTS]);
  if(spaces(ptr))
    return DBOK;
  strcpy(addr, sc + epos(ST_ADDR1, file_ele[STUDENTS]));
  len = strlen(addr);
  while(isspace(addr[len-1])) {
    addr[len-1] = NULL;
    len--;
  }
  if(findzipcode(addr, zipcode)) {
    len = strlen(sc+epos(ST_ZIP1, file_ele[STUDENTS]));
    ziplen = strlen(zipcode);
    while(ziplen < len)
      zipcode[ziplen++] = ' ';
    zipcode[ziplen] = NULL;
    strcpy(sc+epos(ST_ZIP1, file_ele[STUDENTS]), zipcode);
    put_field(ST_ZIP1);
  }
  return DBOK;
}

/*
 * –b¶Å¨ó∏aûaµA¨· ∫Å≠°1ù° ª°µb§Â—°1 ¿xã°
 */
int st_addr1_ddd1(void)
{
  unsigned char *ptr, *addr;
  unsigned char ddd[MAXKEYLEN], tmp[MAXKEYLEN];

  ptr = sc + epos(ST_AREA1, file_ele[STUDENTS]);
  if(!spaces(ptr))
    return DBOK;
  addr = sc + epos(ST_ADDR1, file_ele[STUDENTS]);
  if(spaces(addr))
    return DBOK;
  if(addr2ddd(addr, ddd)) {
    sprintf(tmp, "%4s", ddd);
    strcpy(ptr, tmp);
    put_field(ST_AREA1);
  }
  return DBOK;
}

/*
 * –b¶Å¨ó∏aûaµA¨· ∫Å≠°2ù° ∂ÅÕe§Â—°2 ¿xã°
 */
int st_addr2_zip2(void)
{
  unsigned char zipcode[MAXKEYLEN];
  unsigned char addr[MAXKEYLEN], *ptr;
  int len, ziplen;

  ptr = sc + epos(ST_ZIP2, file_ele[STUDENTS]);
  if(!spaces(ptr))
    return DBOK;
  ptr = sc + epos(ST_ADDR2, file_ele[STUDENTS]);
  if(spaces(ptr))
    return DBOK;
  strcpy(addr, sc + epos(ST_ADDR2, file_ele[STUDENTS]));
  len = strlen(addr);
  while(isspace(addr[len-1])) {
    addr[len-1] = NULL;
    len--;
  }
  if(findzipcode(addr, zipcode)) {
    len = strlen(sc+epos(ST_ZIP2, file_ele[STUDENTS]));
    ziplen = strlen(zipcode);
    while(ziplen < len)
      zipcode[ziplen++] = ' ';
    zipcode[ziplen] = NULL;
    strcpy(sc+epos(ST_ZIP2, file_ele[STUDENTS]), zipcode);
    put_field(ST_ZIP2);
  }
  return DBOK;
}

/*
 * –b¶Å¨ó∏aûaµA¨· ∫Å≠°2ù° ª°µb§Â—°2 ¿xã°
 */
int st_addr2_ddd2(void)
{
  unsigned char *ptr, *addr;
  unsigned char ddd[MAXKEYLEN], tmp[MAXKEYLEN];

  ptr = sc + epos(ST_AREA2, file_ele[STUDENTS]);
  if(!spaces(ptr))
    return DBOK;
  addr = sc + epos(ST_ADDR2, file_ele[STUDENTS]);
  if(spaces(addr))
    return DBOK;
  if(addr2ddd(addr, ddd)) {
    sprintf(tmp, "%4s", ddd);
    strcpy(ptr, tmp);
    put_field(ST_AREA2);
  }
  return DBOK;
}

/*
 * îÅ–b∂•¨ó∏aûaµA¨· ∂ÅÕe§Â—°1ù° ∫Å≠°1¿xã°
 */
int gr_zip1_addr1(void)
{
  unsigned char *zipcode, *ptr;
  unsigned char addr[MAXKEYLEN];
  int len, addrlen;

  ptr = sc + epos(GR_ADDR1, file_ele[GRADUATES]);
  if(!spaces(ptr))
    return DBOK;
  ptr = sc + epos(GR_ZIP1, file_ele[GRADUATES]);
  if(spaces(ptr))
    return DBOK;
  zipcode = sc + epos(GR_ZIP1, file_ele[GRADUATES]);
  if(findaddr(zipcode, addr)) {
    len = strlen(sc+epos(GR_ADDR1, file_ele[GRADUATES]));
    addrlen = strlen(addr);
    while(addrlen < len)
      addr[addrlen++] = ' ';
    addr[addrlen] = NULL;
    strcpy(sc+epos(GR_ADDR1, file_ele[GRADUATES]), addr);
    put_field(GR_ADDR1);
  }
  return DBOK;
}

/*
 * îÅ–b∂•¨ó∏aûaµA¨· ∂ÅÕe§Â—°2ù° ∫Å≠°2¿xã°
 */
int gr_zip2_addr2(void)
{
  unsigned char *zipcode, *ptr;
  unsigned char addr[MAXKEYLEN];
  int len, addrlen;

  ptr = sc + epos(GR_ADDR2, file_ele[GRADUATES]);
  if(!spaces(ptr))
    return DBOK;
  ptr = sc + epos(GR_ZIP2, file_ele[GRADUATES]);
  if(spaces(ptr))
    return DBOK;
  zipcode = sc + epos(GR_ZIP2, file_ele[GRADUATES]);
  if(findaddr(zipcode, addr)) {
    len = strlen(sc+epos(GR_ADDR2, file_ele[GRADUATES]));
    addrlen = strlen(addr);
    while(addrlen < len)
      addr[addrlen++] = ' ';
    addr[addrlen] = NULL;
    strcpy(sc+epos(GR_ADDR2, file_ele[GRADUATES]), addr);
    put_field(GR_ADDR2);
  }
  return DBOK;
}

/*
 * îÅ–b∂•¨ó∏aûaµA¨· ∫Å≠°1ù° ∂ÅÕe§Â—°1 ¿xã°
 */
int gr_addr1_zip1(void)
{
  unsigned char zipcode[MAXKEYLEN];
  unsigned char addr[MAXKEYLEN], *ptr;
  int len, ziplen;

  ptr = sc + epos(GR_ZIP1, file_ele[GRADUATES]);
  if(!spaces(ptr))
    return DBOK;
  ptr = sc + epos(GR_ADDR1, file_ele[GRADUATES]);
  if(spaces(ptr))
    return DBOK;
  strcpy(addr, sc + epos(GR_ADDR1, file_ele[GRADUATES]));
  len = strlen(addr);
  while(isspace(addr[len-1])) {
    addr[len-1] = NULL;
    len--;
  }
  if(findzipcode(addr, zipcode)) {
    len = strlen(sc+epos(GR_ZIP1, file_ele[GRADUATES]));
    ziplen = strlen(zipcode);
    while(ziplen < len)
      zipcode[ziplen++] = ' ';
    zipcode[ziplen] = NULL;
    strcpy(sc+epos(GR_ZIP1, file_ele[GRADUATES]), zipcode);
    put_field(GR_ZIP1);
  }
  return DBOK;
}

/*
 * îÅ–b∂•¨ó∏aûaµA¨· ∫Å≠°1ù° ª°µb§Â—°1 ¿xã°
 */
int gr_addr1_ddd1(void)
{
  unsigned char *ptr, *addr;
  unsigned char ddd[MAXKEYLEN], tmp[MAXKEYLEN];

  ptr = sc + epos(GR_AREA1, file_ele[GRADUATES]);
  if(!spaces(ptr))
    return DBOK;
  addr = sc + epos(GR_ADDR1, file_ele[GRADUATES]);
  if(spaces(addr))
    return DBOK;
  if(addr2ddd(addr, ddd)) {
    sprintf(tmp, "%4s", ddd);
    strcpy(ptr, tmp);
    put_field(GR_AREA1);
  }
  return DBOK;
}

/*
 * îÅ–b∂•¨ó∏aûaµA¨· ∫Å≠°2ù° ∂ÅÕe§Â—°2 ¿xã°
 */
int gr_addr2_zip2(void)
{
  unsigned char zipcode[MAXKEYLEN];
  unsigned char addr[MAXKEYLEN], *ptr;
  int len, ziplen;

  ptr = sc + epos(GR_ZIP2, file_ele[GRADUATES]);
  if(!spaces(ptr))
    return DBOK;
  ptr = sc + epos(GR_ADDR2, file_ele[GRADUATES]);
  if(spaces(ptr))
    return DBOK;
  strcpy(addr, sc + epos(GR_ADDR2, file_ele[GRADUATES]));
  len = strlen(addr);
  while(isspace(addr[len-1])) {
    addr[len-1] = NULL;
    len--;
  }
  if(findzipcode(addr, zipcode)) {
    len = strlen(sc+epos(GR_ZIP2, file_ele[GRADUATES]));
    ziplen = strlen(zipcode);
    while(ziplen < len)
      zipcode[ziplen++] = ' ';
    zipcode[ziplen] = NULL;
    strcpy(sc+epos(GR_ZIP2, file_ele[GRADUATES]), zipcode);
    put_field(GR_ZIP2);
  }
  return DBOK;
}

/*
 * îÅ–b∂•¨ó∏aûaµA¨· ∫Å≠°2ù° ª°µb§Â—°2 ¿xã°
 */
int gr_addr2_ddd2(void)
{
  unsigned char *ptr, *addr;
  unsigned char ddd[MAXKEYLEN], tmp[MAXKEYLEN];

  ptr = sc + epos(GR_AREA2, file_ele[GRADUATES]);
  if(!spaces(ptr))
    return DBOK;
  addr = sc + epos(GR_ADDR2, file_ele[GRADUATES]);
  if(spaces(addr))
    return DBOK;
  if(addr2ddd(addr, ddd)) {
    sprintf(tmp, "%4s", ddd);
    strcpy(ptr, tmp);
    put_field(GR_AREA2);
  }
  return DBOK;
}

/* —¡°e§·Ã· scµ¡ elistµ¡ äó–s∑° †xâ°, rbàa file_eleµ¡ äó–s∑° †xìeïA... */
int ref_st_part(void)
{
  char *buf, *key, *tmp;

  buf = (char *)farmalloc(rlen(PARTS));
  key = (sc + epos(PART_NO, file_ele[STUDENTS]));
  if(first_rcd(PARTS, 1, buf) == DBERROR) {
    disp_cmsg("  ¶Å¨·∏aûaàa –aêaï° ¥ÙØsì°îa. †Â∏· ¶Å¨·∏aûaüi ∑≥ùb–a≠A∂a  ");
  }
  else if(find_rcd(PARTS, 1, key, buf) == DBERROR) {
    disp_cmsg("  ¶Å¨·§Â—°àa Àiü≥ì°îa  ");
    if(yesno("¶Å¨·§Â—°üi ¿qπ°–aØ°âVØsì°åa?", 1) == 0) {
      tmp = db_browse(PARTS, "¶Å ¨· §Â —°  ¿q π° –a ã°", ele, 1, B2REF);
      if(tmp)
        strcpy(sc+epos(PART_NO, file_ele[STUDENTS]), tmp);
      display_template("", 1);
      tally();
    }
  }
  farfree(buf);
  return DBOK;
}

int ref_st_duty(void)
{
  char *buf, *key, *tmp;

  buf = (char *)farmalloc(rlen(DUTIES));
  key = (sc + epos(DUTY_NO, file_ele[STUDENTS]));
  if(first_rcd(DUTIES, 1, buf) == DBERROR) {
    disp_cmsg("  ª¢¿Ç∏aûaàa –aêaï° ¥ÙØsì°îa. †Â∏· ª¢¿Ç∏aûaüi ∑≥ùb–a≠A∂a  ");
  }
  else if(find_rcd(DUTIES, 1, key, buf) == DBERROR) {
    disp_cmsg("  ª¢¿Ç§Â—°àa Àiü≥ì°îa  ");
    if(yesno("ª¢¿Ç§Â—°üi ¿qπ°–aØ°âVØsì°åa?", 1) == 0) {
      tmp = db_browse(DUTIES, "ª¢ ¿Ç §Â —°  ¿q π° –a ã°", ele, 1, B2REF);
      if(tmp)
        strcpy(sc+epos(DUTY_NO, file_ele[STUDENTS]), tmp);
      display_template("", 1);
      tally();
    }
  }
  farfree(buf);
  return DBOK;
}

int ref_st_company(void)
{
  char *buf, *key, *tmp;

  buf = (char *)farmalloc(rlen(COMPANIES));
  key = (sc + epos(COMPANY_NO, file_ele[STUDENTS]));
  if(first_rcd(COMPANIES, 1, buf) == DBERROR) {
    disp_cmsg("  “A¨a∏aûaàa –aêaï° ¥ÙØsì°îa. †Â∏· “A¨a∏aûaüi ∑≥ùb–a≠A∂a  ");
  }
  else if(find_rcd(COMPANIES, 1, key, buf) == DBERROR) {
    disp_cmsg("  “A¨a§Â—°àa Àiü≥ì°îa  ");
    if(yesno("“A¨a§Â—°üi ¿qπ°–aØ°âVØsì°åa?", 1) == 0) {
      tmp = db_browse(COMPANIES, "“A ¨a §Â —°  ¿q π° –a ã°", ele, 1, B2REF);
      if(tmp)
        strcpy(sc+epos(COMPANY_NO, file_ele[STUDENTS]), tmp);
      display_template("", 1);
      tally();
    }
  }
  farfree(buf);
  return DBOK;
}

void init_st_edit(void)
{
  elist = file_ele[STUDENTS];
  edit(PART_NO, ref_st_part);
  edit(DUTY_NO, ref_st_duty);
  edit(COMPANY_NO, ref_st_company);
  edit(ST_ZIP1, st_zip1_addr1);
  edit(ST_ADDR1, st_addr1_zip1);
  edit(ST_AREA1, st_addr1_ddd1);
  edit(ST_ZIP2, st_zip2_addr2);
  edit(ST_ADDR2, st_addr2_zip2);
  edit(ST_AREA2, st_addr2_ddd2);
}

int ref_gr_lab(void)
{
  char *buf, *key, *tmp;

  buf = (char *)farmalloc(rlen(LABS));
  key = (sc + epos(LAB_NO, file_ele[GRADUATES]));
  if(first_rcd(LABS, 1, buf) == DBERROR) {
    disp_cmsg("  µeäÅØ©∏aûaàa –aêaï° ¥ÙØsì°îa. †Â∏· µeäÅØ©∏aûaüi ∑≥ùb–a≠A∂a  ");
  }
  else if(find_rcd(LABS, 1, key, buf) == DBERROR) {
    disp_cmsg("  µeäÅØ©§Â—°àa Àiü≥ì°îa");
    if(yesno("µeäÅØ©§Â—°üi ¿qπ°–aØ°âVØsì°åa?", 1) == 0) {
      tmp = db_browse(LABS, "µe äÅ Ø© §Â —°  ¿q π° –a ã°", ele, 1, B2REF);
      if(tmp)
        strcpy(sc+epos(LAB_NO, file_ele[GRADUATES]), tmp);
      display_template("", 1);
      tally();
    }
  }
  farfree(buf);
  return DBOK;
}

int ref_gr_company(void)
{
  char *buf, *key, *tmp;

  buf = (char *)farmalloc(rlen(COMPANIES));
  key = (sc + epos(COMPANY_NO, file_ele[GRADUATES]));
  if(first_rcd(COMPANIES, 1, buf) == DBERROR) {
    disp_cmsg("  “A¨a∏aûaàa –aêaï° ¥ÙØsì°îa. †Â∏· “A¨a∏aûaüi ∑≥ùb–a≠A∂a  ");
  }
  else if(find_rcd(COMPANIES, 1, key, buf) == DBERROR) {
    disp_cmsg("  “A¨a§Â—°àa Àiü≥ì°îa");
    if(yesno("“A¨a§Â—°üi ¿qπ°–aØ°âVØsì°åa?", 1) == 0) {
      tmp = db_browse(COMPANIES, "“A ¨a §Â —°  ¿q π° –a ã°", ele, 1, B2REF);
      if(tmp)
        strcpy(sc+epos(COMPANY_NO, file_ele[GRADUATES]), tmp);
      display_template("", 1);
      tally();
    }
  }
  farfree(buf);
  return DBOK;
}

void init_gr_edit(void)
{
  elist = file_ele[GRADUATES];
  edit(LAB_NO, ref_gr_lab);
  edit(COMPANY_NO, ref_gr_company);
  edit(GR_ZIP1, gr_zip1_addr1);
  edit(GR_ADDR1, gr_addr1_zip1);
  edit(GR_AREA1, gr_addr1_ddd1);
  edit(GR_ZIP2, gr_zip2_addr2);
  edit(GR_ADDR2, gr_addr2_zip2);
  edit(GR_AREA2, gr_addr2_ddd2);
}

void end_msg(void)
{
  unsigned char msg[] = "+ Ojarkyo Ver 1.00    Copyright(C) 1993  Hantrary : Dongary for Computopia";

  cprintf("%s\n\r", msg);
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

  cputs("\n\r                    >>>    Welcom to Ojarkyo World    <<<\n\r\n\r");
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
  init_sys_color();
  inithanlib(FIXRESMODE, HANDETECT, HAN8GD2, ENGGD5);
/*  hsetbufcursor(SHADOWCURSOR, SHADOWCURSOR, DEFAULTCOLOR); */
  registeregrfont(EGR1);
  registerhgrfont(HGR1);
  initusermenu(menu_fcolor, menu_bcolor, 1);
  inithwindow(fcolor, bcolor);

  init_update_func();

  if(!init_cfg())
    return;

  db_open("", fl);
  background();
  about_ojark();

  do {
    isquit = main_loop();
  } while(!isquit);
  db_cls();
  closehan();
  end_scrn();
  end_msg();
}

