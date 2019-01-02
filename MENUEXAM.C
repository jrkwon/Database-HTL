#include <mem.h>
#include "htlout.h"
#include "htlwin.h"
#include "htlmenu.h"
#include "htlkey.h"
#include "htldbms.h"

#define  setcurstatus(status)   (curstatus = (status))
#define  getcurstatus()         (curstatus)

enum status { ST_STATUS, GR_STATUS };
enum menu_no { UTIL_MENU, DOMAIN_MENU, VIEW_MENU, SEARCH_MENU, UPDATE_MENU, OUTPUT_MENU, INPUT_MENU, CONFIG_MENU };

enum items_menu0 { ABOUT_HANTLE, CALL_HELP, NULL0, CALENDAR, CALCULATOR, OSSHELL, QUIT };
enum items_menu1 { ST_DOMAIN, GR_DOMAIN };
enum items_menu3 { SER_BY_NUM, SER_BY_NAME };
enum items_menu5 { ST_CARD, PART_CARD, GR_CARD, LAB_CARD, COM_CARD };
enum items_menu7 { PHONE_TYPE, DIALING, SET_PRN, NULL7, INIT_DB };

static int curstatus = ST_STATUS;

void initusermenu(int fcolor, int bcolor, int lcolor)
{
	nmenu = 8;         /* ¿aü±Œa àïÆÅ 7 àÅ */

	pmenu[0].menu = "‘á";
	pmenu[0].nitem = 7;
  pmenu[0].item[0] = "–eÀi¥aü°...";  pmenu[0].shortcut[0] = 0;
	pmenu[0].item[1] = "ï°∂ë†i";       pmenu[0].shortcut[1] = F1KEY;
	pmenu[0].item[2] = "-";            pmenu[0].shortcut[2] = 0;
	pmenu[0].item[3] = "îiùb";         pmenu[0].shortcut[3] = 0;
	pmenu[0].item[4] = "âÅ¨eã°";       pmenu[0].shortcut[4] = 0;
	pmenu[0].item[5] = "ï°Øa°wùw¥·";   pmenu[0].shortcut[5] = ALTF0KEY;
	pmenu[0].item[6] = "è{";           pmenu[0].shortcut[6] = ALTXKEY;

  pmenu[1].menu = "∏b¥Û∏Â—≈";
  pmenu[1].nitem = 2;
  pmenu[1].item[0] = "–b¶Å¨ó µwµb";  pmenu[1].shortcut[0] = CTRLSKEY;
  pmenu[1].item[1] = "îÅ–b∂•¨ó µwµb";pmenu[1].shortcut[1] = CTRLGKEY;

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
	pmenu[7].item[4] = "¡°ã°—¡";        pmenu[7].shortcut[4] = 0;

	definemenu(fcolor, bcolor, lcolor);
}

void setstatus(int status)
{
  setcurstatus(status);
  switch(status) {
    case ST_STATUS :
      pmenu[1].itemable[0] = FALSE;
      pmenu[1].itemable[1] = TRUE;

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
    case GR_STATUS :
      pmenu[1].itemable[0] = TRUE;
      pmenu[1].itemable[1] = FALSE;

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
    case ST_DOMAIN :
      setstatus(ST_DOMAIN);
      break;
    case GR_DOMAIN :
      setstatus(GR_DOMAIN);
      break;
  }
}

void view_menu()
{}

void search_menu(void)
{
  switch(getcuritem()) {
    case SER_BY_NUM :
      if(getcurstatus() == ST_STATUS)
        search_by_num("STUDENTS", "–b¶Å¨ó Ø•¨w ∑≥ùb");
      else
        search_by_num("GRADUATES", "îÅ–b∂•¨ó Ø•¨w ∑≥ùb");
      break;
    case SER_BY_NAME :
      if(getcurstatus() == ST_STATUS)
        search_by_name("STUDENTS", "–b¶Å¨ó Ø•¨w ∑≥ùb");
      else
        search_by_name("GRADUATES", "îÅ–b∂•¨ó Ø•¨w ∑≥ùb");
      break;
  }
}

void update_menu()
{}

void output_menu()
{}

void input_menu(void)
{
  switch(getcuritem()) {
    case ST_CARD :
      qd("STUDENTS", "–b¶Å¨ó Ø•¨w ∑≥ùb", "", DBINPUT);
      break;
    case PART_CARD :
      qd("PARTS", "–b¶Å –b“A ∑≥ùb", "", DBINPUT);
      break;
    case GR_CARD :
      qd("GRADUATES", "îÅ–b∂•¨ó Ø•¨w ∑≥ùb", "", DBINPUT);
      break;
    case LAB_CARD :
      qd("LABS", "µeäÅØ© ¨w—◊ ∑≥ùb", "", DBINPUT);
      break;
    case COM_CARD :
      qd("COMPANYS", "¨e–b ∏w–b¨ó “A¨a ∑≥ùb", "", DBINPUT);
      break;
  } /* switch */
}

void config_menu()
{
  switch(getcuritem()) {

    case INIT_DB :
      dbinit();
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

void main(void)
{
  int isquit;

  inithantle(YELLOW, BLUE, WHITE);
  initusermenu(YELLOW, BLUE, WHITE);
  do {
    isquit = main_loop();
  } while(!isquit);
  closehantle();
}
