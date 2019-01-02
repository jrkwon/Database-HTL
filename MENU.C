#include <mem.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "hanlib.h"
#include "hanmenu.h"
#include "ascii.h"
#include "hanin.h"
#include "handbms.h"
#include "hanerror2.h"
#include "hanojark.h"
#include "hanpop.h"

#define  setcurstatus(status)   (curstatus = (status))
#define  getcurstatus()         (curstatus)

enum status { STUDENT_STATUS, GRADUATE_STATUS };
enum menu_no { UTIL_MENU, DOMAIN_MENU, VIEW_MENU, SEARCH_MENU, UPDATE_MENU, OUTPUT_MENU, INPUT_MENU, CONFIG_MENU };

enum items_menu0 { ABOUT_OJARK, CALL_HELP, NULL0, CALENDAR, CALCULATOR, OSSHELL, QUIT };
enum items_menu1 { STUDENT_DOMAIN, GRADUATE_DOMAIN };
enum items_menu3 { SER_BY_NUM, SER_BY_NAME };
enum items_menu5 { STUDENT_CARD, PART_CARD, DUTY_CARD, GRADUATE_CARD, LAB_CARD, COMPANYCARD };
enum items_menu7 { PHONE_TYPE, DIALING, SET_PRN, NULL7, ABOUT_HANTLE };

static int curstatus = STUDENT_STATUS;

int fl[] = { STUDENTS, GRADUATES, PARTS, DUTIES, COMPANIES, LABS, -1 };
int ele[] = { 0, -1 };
struct coord_t student_crd[] = {
  { 10, 2 }, { 44, 2 }, { 66, 2 }, { 18, 4 }, { 48, 4 }, { 66, 4 },
  { 14, 6 }, { 44, 6 }, { 64, 6 }, { 14, 8 }, { 48, 8 }, { 14,10 },
  { 48,10 }, { 14,12 }, { 46,12 }, { 18,14 }, { 48,14 }, { 67,14 },
  { 10,16 }, { 18,18 }, { 48,18 }, { 67,18 }, { 10,20 }
};
struct coord_t graduate_crd[] = {
  { 10, 2 }, { 44, 2 }, { 66, 2 }, { 18, 4 }, { 48, 4 }, { 66, 4 },
  { 10, 6 }, { 44, 6 }, { 72, 6 }, { 14, 8 }, { 48, 8 }, { 14,10 },
  { 48,10 }, { 14,12 }, { 48,12 }, { 18,14 }, { 48,14 }, { 67,14 },
  { 10,16 }, { 18,18 }, { 48,18 }, { 67,18 }, { 10,20 }
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

void initusermenu(int fcolor, int bcolor, int lcolor)
{
  nmenu = 8;         /* ¿aü±Œa àïÆÅ 7 àÅ */

  pmenu[0].menu = "‘";
  pmenu[0].nitem = 7;
  pmenu[0].item[0] = "µ°∏bäa.... ";  pmenu[0].shortcut[0] = 0;
  pmenu[0].item[1] = "ï°∂ë†i";       pmenu[0].shortcut[1] = F1KEY;
  pmenu[0].item[2] = "-";            pmenu[0].shortcut[2] = 0;
  pmenu[0].item[3] = "îiùb";         pmenu[0].shortcut[3] = 0;
  pmenu[0].item[4] = "âÅ¨eã°";       pmenu[0].shortcut[4] = 0;
  pmenu[0].item[5] = "ï°Øa°wùw¥·";   pmenu[0].shortcut[5] = ALTF0KEY;
  pmenu[0].item[6] = "è{";           pmenu[0].shortcut[6] = ALTXKEY;

  pmenu[1].menu = "∏b¥Û∏Â—≈";
  pmenu[1].nitem = 3;
  pmenu[1].item[0] = "–b¶Å¨ó µwµb";  pmenu[1].shortcut[0] = CTRLSKEY;
  pmenu[1].item[1] = "-";            pmenu[1].shortcut[1] = CTRLSKEY;
  pmenu[1].item[2] = "îÅ–b∂•¨ó µwµb";pmenu[1].shortcut[2] = CTRLGKEY;

  pmenu[2].menu = "•°ã°";
  pmenu[2].nitem = 9;
  pmenu[2].item[0] = "–b§Â•iù°...";   pmenu[2].shortcut[0] = ALTNKEY;
  pmenu[2].item[1] = "–bëe•iù°...";   pmenu[2].shortcut[1] = ALTOKEY;
  pmenu[2].item[2] = "ã°ÆÅ•iù°...";   pmenu[2].shortcut[2] = ALTSKEY;
  pmenu[2].item[3] = "â¡∏˜•iù°...";   pmenu[2].shortcut[3] = ALTVKEY;
  pmenu[2].item[4] = "ª°µb•iù°...";   pmenu[2].shortcut[4] = 0;
  pmenu[2].item[5] = "µeäÅØ©•iù°..."; pmenu[2].shortcut[5] = 0;
  pmenu[2].item[6] = "¶Å¨·•iù°...";   pmenu[2].shortcut[6] = 0;
  pmenu[2].item[7] = "¨e–b•iù°...";   pmenu[2].shortcut[7] = 0;
  pmenu[2].item[8] = "¨ó∑©•iù°...";   pmenu[2].shortcut[8] = 0;

  pmenu[3].menu = "¿xã°";
  pmenu[3].nitem = 2;
  pmenu[3].item[0] = "–b§Â∑aù°";      pmenu[3].shortcut[0] = CTRLFKEY;
  pmenu[3].item[1] = "∑°üq∑aù°";      pmenu[3].shortcut[1] = CTRLAKEY;

  pmenu[4].menu = "â°√°ã°";
  pmenu[4].nitem = 2;
  pmenu[4].item[0] = "–b§Â∑aù°";      pmenu[4].shortcut[0] = ALTPKEY;
  pmenu[4].item[1] = "∑°üq∑aù°";      pmenu[4].shortcut[1] = CTRLPKEY;

  pmenu[5].menu = "ø¢ã°";
  pmenu[5].nitem = 4;
  pmenu[5].item[0] = "–bëe•i ∫Å≠°ù¢";   pmenu[5].shortcut[0] = 0;
  pmenu[5].item[1] = "–bëe•i Ø•¨wƒaóa"; pmenu[5].shortcut[1] = 0;
  pmenu[5].item[2] = "Õeª°•∑ Å..";      pmenu[5].shortcut[2] = CTRLZKEY;
  pmenu[5].item[3] = "µeäÅØ©•i ∫Å≠°ù¢"; pmenu[5].shortcut[3] = 0;

  pmenu[6].menu = "ê˝ã°";
  pmenu[6].nitem = 5;
  pmenu[6].item[0] = "–b¶Å¨ó Ø•¨w";   pmenu[6].shortcut[0] = ALTQKEY;
  pmenu[6].item[1] = "–b“A¶Å¨·";      pmenu[6].shortcut[1] = 0;
  pmenu[6].item[2] = "îÅ–b∂•¨ó Ø•¨w"; pmenu[6].shortcut[2] = 0;
  pmenu[6].item[3] = "µeäÅØ©";        pmenu[6].shortcut[3] = 0;
  pmenu[6].item[4] = "¨e–b“A¨a";      pmenu[6].shortcut[4] = 0;

  pmenu[7].menu = "∏˜–aã°";
  pmenu[7].nitem = 5;
  pmenu[7].item[0] = "∏Â—¡§wØ¢";      pmenu[7].shortcut[0] = ALTMKEY;
  pmenu[7].item[1] = "∏Â—¡à·ìe §w§Û"; pmenu[7].shortcut[1] = 0;
  pmenu[7].item[2] = "∑•≠·ã° ª°∏˜";   pmenu[7].shortcut[2] = 0;
  pmenu[7].item[3] = "-";             pmenu[7].shortcut[3] = 0;
  pmenu[7].item[4] = "–eÀi¥aü°";        pmenu[7].shortcut[4] = 0;

  definemenu(fcolor, bcolor, lcolor);
}

void setstatus(int status)
{
  setcurstatus(status);
  switch(status) {
    case STUDENT_STATUS :
      pmenu[1].itemable[0] = FALSE;
      pmenu[1].itemable[2] = TRUE;

      pmenu[2].itemable[1] = TRUE;

      pmenu[2].itemable[2] = FALSE;
      pmenu[2].itemable[3] = FALSE;
      pmenu[2].itemable[5] = FALSE;

      pmenu[2].itemable[6] = TRUE;

      pmenu[5].itemable[0] = TRUE;
      pmenu[5].itemable[1] = TRUE;

      pmenu[5].itemable[3] = FALSE;

      pmenu[6].itemable[0] = TRUE;
      pmenu[6].itemable[1] = TRUE;
      pmenu[6].itemable[2] = FALSE;
      pmenu[6].itemable[3] = FALSE;
      break;
    case GRADUATE_STATUS :
      pmenu[1].itemable[0] = TRUE;
      pmenu[1].itemable[2] = FALSE;

      pmenu[2].itemable[1] = FALSE;

      pmenu[2].itemable[2] = TRUE;
      pmenu[2].itemable[3] = TRUE;
      pmenu[2].itemable[5] = TRUE;

      pmenu[2].itemable[6] = FALSE;

      pmenu[5].itemable[0] = FALSE;
      pmenu[5].itemable[1] = FALSE;

      pmenu[5].itemable[3] = TRUE;

      pmenu[6].itemable[0] = FALSE;
      pmenu[6].itemable[1] = FALSE;
      pmenu[6].itemable[2] = TRUE;
      pmenu[6].itemable[3] = TRUE;
      break;
  } /* switch */
  drawmenubar();
}

int util_menu()
{
  int isquit = FALSE;

  switch(getcuritem()) {

    case QUIT :
      isquit = TRUE;
      break;
  }
  return isquit;
}

void domain_menu(void)
{
  switch(getcuritem()) {
    case STUDENT_DOMAIN :
      setstatus(STUDENT_DOMAIN);
      break;
    case GRADUATE_DOMAIN :
      setstatus(GRADUATE_DOMAIN);
      break;
  }
}

void view_menu()
{
  disp_cmsg_sec("¥aª¢ ª°∂•–aª° °µ–ñìeïA∂a. ∫A≠∑!", 3);
}

/*
 *  –b¶Å¨ó ∑°üq∑aù° ¿xã°
 */
void student_search_by_name(void)
{
  int ans;

  ans = win_hgetdata(10, " ¿x∑i ¨aúq∑Å ∑°üq∑i ∑≥ùb–a≠A∂a ", "XXXXXXXXXX", str);
  if(ans) {
    db_search(STUDENTS, "–b  ¶Å  ¨ó    Ø•  ¨w  •°  ã°", ele, student_crd, 2, str);
  }
}

/*
 *  îÅ–b∂•¨ó ∑°üq∑aù° ¿xã°
 */
void graduate_search_by_name(void)
{
  int ans;

  ans = win_hgetdata(10, " ¿x∑i ¨aúq∑Å ∑°üq∑i ∑≥ùb–a≠A∂a ", "XXXXXXXXXX", str);
  if(ans) {
    db_search(GRADUATES, "îÅ  –b  ∂•  ¨ó    Ø•  ¨w  •°  ã°", ele, graduate_crd, 2, str);
  }
}

/*
 *  –b¶Å¨ó –b§Â∑aù° ¿xã°
 */
void student_search_by_number(void)
{
  int ans;

  ans = win_hgetdata(8, " ¿x∑i ¨aúq∑Å –b§Â∑i ∑≥ùb–a≠A∂a ", "99999999", str);
  if(ans) {
    db_search(STUDENTS, "–b  ¶Å  ¨ó    Ø•  ¨w  •°  ã°", ele, student_crd, 1, str);
  }
}

/*
 *  îÅ–b∂•¨ó –b§Â∑aù° ¿xã°
 */
void graduate_search_by_number(void)
{
  int ans;

  ans = win_hgetdata(8, " ¿x∑i ¨aúq∑Å –b§Â∑i ∑≥ùb–a≠A∂a ", "99999999", str);
  if(ans) {
    db_search(GRADUATES, "îÅ  –b  ∂•  ¨ó    Ø•  ¨w  •°  ã°", ele, graduate_crd, 1, str);
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
        students_search_by_name();
      else
        graduate_search_by_name();
      break;
  }
}

/*
 *  –b¶Å¨ó ∑°üq∑aù° â°√°ã°
 */
void student_update_by_name(void)
{
  int ans;

  ans = win_hgetdata(10, " â°√© ¨aúq∑Å ∑°üq∑i ∑≥ùb–a≠A∂a ", "XXXXXXXXXX", str);
  if(ans) {
    db_search(STUDENTS, "–b  ¶Å  ¨ó    Ø•  ¨w  â°  √°  ã°", ele, student_crd, 2, str);
  }
}

/*
 *  îÅ–b∂•¨ó ∑°üq∑aù° â°√°ã°
 */
void graduate_update_by_name(void)
{
  int ans;

  ans = win_hgetdata(10, " â°√© ¨aúq∑Å ∑°üq∑i ∑≥ùb–a≠A∂a ", "XXXXXXXXXX", str);
  if(ans) {
    db_search(GRADUATES, "îÅ  –b  ∂•  ¨ó    Ø•  ¨w  â°  √°  ã°", ele, graduate_crd, 2, str);
  }
}

/*
 *  –b¶Å¨ó –b§Â∑aù° â°√°ã°
 */
void student_update_by_number(void)
{
  int ans;

  ans = win_hgetdata(8, " â°√© ¨aúq∑Å –b§Â∑i ∑≥ùb–a≠A∂a ", "99999999", str);
  if(ans) {
    db_search(STUDENTS, "–b  ¶Å  ¨ó    Ø•  ¨w  â°  √°  ã°", ele, student_crd, 1, str);
  }
}

/*
 *  îÅ–b∂•¨ó –b§Â∑aù° â°√°ã°
 */
void graduate_update_by_number(void)
{
  int ans;

  ans = win_hgetdata(8, " â°√© ¨aúq∑Å –b§Â∑i ∑≥ùb–a≠A∂a ", "99999999", str);
  if(ans) {
    db_search(GRADUATES, "îÅ  –b  ∂•  ¨ó    Ø•  ¨w  â°  √°  ã°", ele, graduate_crd, 1, str);
  }
}

void update_menu()
{
  switch(getcuritem()) {
    case UPDATE_BY_NUM :
      if(getcurstatus() == STUDENT_STATUS)
        student_update_by_number();
      else
        graduate_update_by_number();
      break;
    case UPDATE_BY_NAME :
      if(getcurstatus() == STUDENT_STATUS)
        students_update_by_name();
      else
        graduate_update_by_name();
      break;
  }
}

void output_menu()
{
  disp_cmsg_sec("¥aª¢ ª°∂•–aª° °µ–ñìeïA∂a. ∫A≠∑!", 3);
}

void input_menu(void)
{
  switch(getcuritem()) {
    case STUDENT_CARD :
      db_input(STUDENTS, "–b  ¶Å  ¨ó    Ø•  ¨w  ∑≥  ùb", ele, customer_crd);
      break;
    case PART_CARD :
      db_input(PARTS, "â¡  êÅ  ¶Å  ¨·    ∏a  ûa  ∑≥  ùb", ele, part_crd);
      break;
    case DUTY_CARD :
      db_input(DUTIES, "â¡  êÅ  ª¢  ¿Ç    ∏a  ûa  ∑≥  ùb", ele, duty_crd);
      break;
    case GRADUATE_CARD :
      db_input(GRADUATES, "îÅ  –b  ∂•  ¨ó    Ø•  ¨w  ∑≥  ùb", ele, graduate_crd);
      break;
    case LAB_CARD :
      db_input(LABS, "µe  äÅ  Ø©    ∏a  ûa  ∑≥  ùb", ele, lab_crd);
      break;
    case COMPANYCARD :
      db_input(COMPANIES, "¨e  –b  “A  ¨a    ∏a  ûa  ∑≥  ùb", ele, _crd);
      break;
  } /* switch */
}

void config_menu()
{
  switch(getcuritem()) {

    case ABOUT_HANTLE :
      about_hantle();
      break;
  }
}

int main_loop(void)
{
  int isquit = FALSE, key = F0KEY;

  setstatus(getcurstatus());
  if(getitem(key)) {
    switch(getcurmenu()) {
      case  UTIL_MENU :
        isquit = util_menu();
        break;
      case  DOMAIN_MENU :
        domain_menu();
        break;
      case  VIEW_MENU :
        view_menu();
        break;
      case  SEARCH_MENU :
        search_menu();
        break;
      case  UPDATE_MENU :
        update_menu();
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
  return isquit;
}

void background(void)
{
  bool onoff, onoff2;

  hclrscr();
  onoff = isreverse();
  onoff2 = isunder();
  hsetreverse(ON);
  eputchs(-1, -hgetmaxax(), 80, ' ');
  hputsxy(-(hgetmaxax()-5), -hgetmaxay(), "1992");
  hsetunder(ON);
  eputchs(-1, -1, 80, ' ');
  hputsxy(-49, -1, "‘ –e¥∑îÅ–bäa ∏Â∏a…∑Ø•â∑–bâ¡ ‘");
  hsetreverse(onoff);
  hsetunder(onoff2);
}

void about_ojark(void)
{
  if(wopen(21, 7, 40, 8)) {
    wcenter(2, "‘  µ°  ∏b  äa  ‘");
    wcenter(4, "âe∂Åµ¡ ª¢ëaàa †eêaï°ù¢");
    wcenter(5, "ï°µ¡∫Åìe µ°∏bäa¿·úÒ");
    wcenter(6, "–b¨óâ¡ –b¨ó“Aüi µeâi–Å∫ìì°îa");
    wcenter(7, "∑• â∑ ª° ìw µe äÅ Ø© (1992)");
    wait_getkey(5);
    wclose();
  }
}


void about_hantle(void)
{
  if(wopen(21, 7, 40, 8)) {
    wcenter(2, "‘  –e Ài ¥a ü°  ‘");
    wcenter(4, "ãº∑°∑∂ìe ∏Â¢Öã°Æâ∑i ∑¢”°â°");
    wcenter(5, "Õ¢ êÏ∑e ∏aã°âÅ§iù°");
    wcenter(6, "â¡–bã°Æâ∏aù°¨·∑Å ≠°°w∑i îa–aúa");
    wcenter(7, "â∑‘áï©‘á∑°");
    wait_getkey(5);
    wclose();
  }
}

void main(void)
{
  char str[11] = "";
  int isquit;

  initusermenu(YELLOW, BLUE, WHITE);
  inithanlib(FIXRESMODE, HANDETECT, HAN8GD2, ENGGD5);
  registeregrfont(EGR1);
  registerhgrfont(HGR1);
  initusermenu(YELLOW, BLUE, WHITE);
  initinput();

  db_open("", fl);
  background();
  about_ojark();
  init_screen(CUSTOMER, file_ele[CUSTOMER], sc);
  init_edit();

  do {
    isquit = main_loop();
  } while(!isquit);
  db_cls();
}
