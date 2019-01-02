/*
 *  Ãa∑©   : nmain.c
 *  êÅ∂w   : êi∏‚∑°(∑©ã°∏w) ∫Å –qÆÅ
 *  ã°“B   : nal.prj
 *  êiºa   : 93.03.16
 *  †eóe∑° : –e¥∑îÅ–bäa ∏Â∏a…∑Ø•â∑–bâ¡ –eÀi¥aü° ä•∏Åúb
 *  ¥iü±ãi : ∑° Œâãaü±∑Å £ªãaü±∑e â∑àÅüi ∂•√¢∑aù° –sì°îa.
 *           µ·ê°–a°e, êaï° îaüe £ªãaü±∑Å ï°∂ë∑i †g∑° §h¥v∑aì°åa∂a.
 *           òaúa¨·, ∑° £ªãaü±∑i §h∑aØ• ¶Ö∑e îaüeïA ¨a∂w–a≠aï° ñAª°†e
 *           ¨Åù° †eóe £ªãaü±ï° ∏i ∏˜ü°–a≠a¨· â∑àÅ–Å ∫ÅØ°ã° §aúsì°îa.
 */
#include <dos.h>
#include <string.h>
#include "hanlib.h"
#include "hancolor.h"
#include "handetec.h"
#include "hanmenu.h"
#include "hanerro2.h"
#include "hanwindw.h"
#include "ascii.h"
#include "extkey.h"
#include "kinput.h"
#include "kedit.h"
#include "kfile.h"
#include "nindex.h"

#define   HELP_FILE   "NAL.HLP"

/* ************************************************************************** *
 *                             ∂A¶Å •eÆÅói                                    *
 * ************************************************************************** */

static int menu_fcolor, menu_bcolor, fcolor, bcolor;
char edit_file[80];


/* ************************************************************************** *
 *                           ¿aü±Œa ¿·ü° ¶Å¶Ö                                 *
 * ************************************************************************** */

/*
 *  ∫Å ¿aü±Œa
 */
enum MENU_T   { HANTLE_T, DIARY_T, REGIST_T, ADDR_T, CONFIG_T, TOOL_T };

/*
 *  àb ¿aü±Œa∑Å –w°¢
 */
enum HANTLE_T { DIARY, HELP, NULL0, CALENDAR, CALCULATOR, OSSHELL, QUIT };
enum DIARY_T  { WRITE, VIEW, FIND, PRINT, NULL1, EDIT, DELETE, NULL2, ALBEM };
enum REGIST_T { KIND, PASSWD, NULL3, USER };
enum ADDR_T   { AINPUT, AEDIT, ADELETE, AFIND, APRINT };
enum CONFIG_T { HFONT, EFONT, NULL4, PPORT, MPORT, TKIND, NULL5, CSAVE };
enum TOOL_T   { SCAN };


/*
 *  îe¬Ç«° ∏˜∑Å
 */
int shortcuts[] = {
  0, F1, 0, ALT_C, 0, 0,  ALT_F10, ALT_X,
  ALT_I, ALT_V, ALT_S, ALT_P, 0, 0, 0, ALT_A,
  ALT_K, 0, 0, 0,
  CTRL_I, CTRL_E, CTRL_D, CTRL_S, CTRL_P,
  F7, F8, 0, 0, 0, 0, 0, 0,
 0
};

/*
 *  ¿aü±Œa ¡°ã°—¡
 */
void initusermenu(int fcolor, int bcolor)
{
  char *menus[] = { "ŸT", "∑©ã°", "∑©ã°¨È∏˜", "∫Å≠°ù¢", "—≈âw¨È∏˜", "ï°äÅ" };
  char item_num[] = { 7, 9, 4, 5, 8, 1 };
  char *items[] = {
    "–eÀi∑©ã°∏w", "ï°∂ë†i", "-", "îiùb", "âÅ¨eã°", "ï°Øa êaói∑°", "è{",
    "∑©ã°≥aã°", "∑©ã°•°ã°", "∑©ã°¿xã°", "∑©ã°∑•≠·", "-", "∑©ã°â°√°ã°", "∑©ã°ª°∂Åã°", "-", "¨aª•¿Û •°ã°",
    "∑©ã°π∑üA", "¥q—°ówù¢", "-", "¨a∂w∏aówù¢",
    "¨Å∏aûaê˝ã°", "∏aûaÕeª≥", "∏aûaª°∂Åã°", "∏aûa¿xã°", "∏aûa∑•≠·",
    "–eãiãiç©", "µw¢Öãiç©", "-", "œaü•»·Õ°Àa", "°°ïQÕ°Àa", "∏Â—¡π∑üA", "-", "—≈âw∏·∏w",
    "ØaƒÅê·"
  };
  register i, j, sum;

  nmenu = 6;         /* ¿aü±Œa àïÆÅ 6àÅ */
  for(i = 0, sum = 0; i < nmenu; i++) {
    pmenu[i].menu = menus[i];
    pmenu[i].nitem = item_num[i];
    for(j = 0; j < item_num[i]; j++, sum++) {
      pmenu[i].item[j] = items[sum];
      pmenu[i].shortcut[j] = shortcuts[sum];
    }
  }
  definemenu(fcolor, bcolor, 1, 2);
}

/* ************************************************************************** *
 *                           ¿aü±ŒaµA¨· ¶âü°ìe –qÆÅ                           *
 * ************************************************************************** */

void diary_item(void)
{
}

void help_item(void)
{
}

void calendar_item(void)
{
}

void calculator_item(void)
{
}

void osshell_item(void)
{
}

bool quit_item(void)
{
  return true;
}

/*
 *
 */
bool hantle_menu(void)
{
  bool ans;

  ans = false;
  switch(getcuritem()) {
    case DIARY :
      diary_item();
      break;
    case HELP :
      help_item();
      break;
    case CALENDAR :
      calendar_item();
      break;
    case CALCULATOR :
      calculator_item();
      break;
    case OSSHELL :
      osshell_item();
      break;
    case QUIT :
      ans = quit_item();
      break;
  }
  return ans;
}

struct date_title_t head;

void write_item(void)
{
  drawbox(1, 2, 80, 3, true);

  /* ∑©ã°≥i êi∏a ∑≥ùb */
  hgotoxy(3, 3);
  get_date_title(&head);

  if(!init_editor(edit_file, HELP_FILE, 10000L, HANKBD2, true))
    return;
  drawbox(1, 4, 80, 21, false);
  eputsxy(1, 4, "√");
  eputsxy(80, 4, "¥");
  editor(2, 5, 78, 19 , 0);
}

void view_item(void)
{
}

void find_item(void)
{
}

void print_item(void)
{
}

void edit_item(void)
{
}

void delete_item(void)
{
}

void albem_item(void)
{
}

/*
 *
 */
void diary_menu(void)
{
  switch(getcuritem()) {
    case WRITE :
      write_item();
      break;
    case VIEW :
      view_item();
      break;
    case FIND :
      find_item();
      break;
    case PRINT :
      print_item();
      break;
    case EDIT :
      edit_item();
      break;
    case DELETE :
      delete_item();
      break;
    case ALBEM :
      albem_item();
      break;
  }
}

void kind_item(void)
{
}

void passwd_item(void)
{
}

void user_item(void)
{
}

/*
 *
 */
void regist_menu(void)
{
  switch(getcuritem()) {
    case KIND :
      kind_item();
      break;
    case PASSWD :
      passwd_item();
      break;
    case USER :
      user_item();
      break;
  }
}

void ainput_item(void)
{
}

void aedit_item(void)
{
}

void adelete_item(void)
{
}

void afind_item(void)
{
}

void aprint_item(void)
{
}

/*
 *
 */
void addr_menu(void)
{
  switch(getcuritem()) {
    case AINPUT :
      ainput_item();
      break;
    case AEDIT :
      aedit_item();
      break;
    case ADELETE :
      adelete_item();
      break;
    case AFIND :
      afind_item();
      break;
    case APRINT :
      aprint_item();
      break;
  }
}

void hfont_item(void)
{
}

void efont_item(void)
{
}

void pport_item(void)
{
}

void mport_item(void)
{
}

void tkind_item(void)
{
}

void csave_item(void)
{
}

/*
 *
 */
void config_menu(void)
{
  switch(getcuritem()) {
    case HFONT :
      hfont_item();
      break;
    case EFONT :
      efont_item();
      break;
    case PPORT :
      pport_item();
      break;
    case MPORT :
      mport_item();
      break;
    case TKIND :
      tkind_item();
      break;
    case CSAVE :
      csave_item();
      break;
  }
}

void scan_item(void)
{
}

/*
 *
 */
void tool_menu(void)
{
  switch(getcuritem()) {
    case SCAN :
      scan_item();
      break;
  }
}

/*
 *  œaù°ãaúë∑Å ∫Å ûÅœa
 */
bool main_loop(void)
{
  bool isquit = false;

  if(getitem(F10)) {             /* ¿aü±Œaêa îe¬Ç«° ¿·ü° ¶Å¶Ö */
    switch(getcurmenu()) {           /* F10∑°êa îe¬Ç«°àa íâü°°e   */
      case  HANTLE_T :
        isquit = hantle_menu();
        break;
      case  DIARY_T :
        diary_menu();
        break;
      case REGIST_T :
        regist_menu();
        break;
      case ADDR_T :
        addr_menu();
        break;
      case CONFIG_T :
        config_menu();
        break;
      case TOOL_T :
        tool_menu();
        break;
    } /* switch */
  } /* if */
  else {
    memerror();
    isquit = true;
  }
  return isquit;
}

/* ************************************************************************** *
 *                               °yàaª° ï°äÅ –qÆÅ                             *
 * ************************************************************************** */

/*
 *  –aóa∂¡¥· µAú·∑Å ∑•»·úÛÀa –Öóiú·
 */
int handler(void)
{
  hardretn(-1);
  return YES;
}

/*
 *  —¡°e¨Çåi ¡°ã°—¡
 */
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
  hsetcolor(fcolor);
  hsetbkcolor(bcolor);
}

/* ************************************************************************** *
 *                                 ∫Å –q ÆÅ                                   *
 * ************************************************************************** */

void main(int argc, unsigned char *argv[])
{
  bool isquit = false;

  harderr(handler);

  inithanlib(FIXRESMODE, HANDETECT, HAN8GD2, ENGGD5);
  registeregrfont(EGR1);
  registerhgrfont(HGR1);
/*  registerkssfont(KSS1);
  loadhanjafont("hanja.fnt"); */

  init_sys_color();
  initusermenu(menu_fcolor, menu_bcolor);

  if(argc > 1)
    strcpy(edit_file, strupr(argv[1]));
  else
    strcpy(edit_file, "");;
  hclrscr();

  drawmenubar();
  do {
    isquit = main_loop();          /* œaù°ãaúë∑Å ∫Å ¶Å¶Ö */
  } while(!isquit);

}

/*
 *  Ãa∑© nmain.c ∑Å è{
 */
