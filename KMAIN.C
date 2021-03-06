/* #define  DEBUG */

/*
 *  �a��   : kmain.c
 *  ���w   : �e���� �� �q��
 *  ���B   : hantle.prj
 *  �i�a   : 93.03.16
 *  �e�e�� : �e�����b�a ��aɷ�����b�� �e�i�a�� �����b
 *  �i���i : �� Ή�a���� ���a���e �����i ��â�a�� �s���a.
 *           �ᐡ�a�e, �a�� �a�e ���a���� �����i �g�� �h�v�a���a�a.
 *           �a�a��, �� ���a���i �h�a�� ���e �a�e�A �a�w�a�a�� �A���e
 *           ���� �e�e ���a���� �i �����a�a�� ����Ё ������ �a�s���a.
 */
#include <stdio.h>
#include <dos.h>
#include <string.h>
#include <stdlib.h>
#include <process.h>
#include <conio.h>
#include "hanlib.h"
#include "hanin.h"
#include "hancolor.h"
#include "handetec.h"
#include "extkey.h"
#include "ascii.h"
#include "hanwindw.h"
#include "hanmenu.h"
#include "hanpcx.h"
#include "hanerro2.h"
#include "hancalen.h"
#include "hancalc.h"
#include "hanpop.h"
#include "khelp.h"
#include "khinput.h"
#include "kedit.h"
#include "kfile.h"
#include "kutil.h"
#include "kprn.h"
#include "kmain.h"

#define  LOGO_FILE     "HANYANG.PCX"
#define  CINE_FILE     "3L-LABOR.PCX"
#define  HANJA_FILE    "HANJA.FNT"
#define  HELP_FILE     "HANTLE.HLP"
#define  CONFIG_FILE   "HANTLE.CFG"

enum menu_no { HANTLE_MENU, FILE_MENU, EDIT_MENU, FIND_MENU, PRINT_MENU, CONFIG_MENU };

enum items_menu0 { HANTLE_ITEM, HELP_ITEM, NULL0, CALENDAR_ITEM, CALCULATOR_ITEM, NULL1, CINE_ITEM };
enum items_menu1 { NEW_ITEM, LOAD_ITEM, SAVE_ITEM, SAVEAS_ITEM, NULL2, CHDIR_ITEM, OS_ITEM, QUIT_ITEM };
enum items_menu2 { BBEGIN_ITEM, BEND_ITEM, BSHOW_ITEM, BCOPY_ITEM, BMOVE_ITEM, BDEL_ITEM, NULL3, BREAD_ITEM, BWRITE_ITEM };
enum items_menu3 { FIND_ITEM, REPLACE_ITEM, REPEAT_ITEM, NULL4, TOBBEGIN_ITEM, TOBEND_ITEM, NULL5, TOPREV_ITEM };
enum items_menu4 { ALLPRINT_ITEM, BPRINT_ITEM, NULL6, PRINTER_ITEM };
enum items_menu5 { INDENT_ITEM, TAB_ITEM, FORMFEED_ITEM, BORDER_ITEM, EXT_ITEM, NULL7, HANGUL_ITEM, ENGLISH_ITEM, NULL8, KBD_ITEM, NULL9, SAVECFG_ITEM };

struct scr_cfg_t {
  int x, y, width, height, border;
};

struct scr_cfg_t scr_cfg[2] = { {1, 2, 80, 23, 0}, {2, 3, 78, 21, 1} };
enum scr_border { NOBORDER_MODE, BORDER_MODE };
static int is_border = NOBORDER_MODE;

hanfont_t han_font[] = { HAN10MD1, HAN8GD2, HAN412GS1, HAN213GD1 };
engfont_t eng_font[] = { ENGMD1, ENGGD5, ENGMD6 };
auxfont_t aux_font[] = { ENGAUX2, ENGAUX5, ENGAUX6 };

static int menu_fcolor, menu_bcolor, fcolor, bcolor;
static int cur_han_font = 1, /* HAN8GD2 */ cur_eng_font = 1 /* ENGGD5 */;
static char logo_file[80], cine_file[80];

/*
 *  �e�i ���� �a���� ����  1.03
 *  -------------------------------------------------------------------
 *  ���i�a - HANyang gulTLE ConFiGuration File V 1.03<EOF><NULL>
 *                     42�a���a
 *  ���� �¸w�a        5�a���a
 *  ȓ �췡            4���a
 *  �i�a�a��           1���a
 *  ����ҁ ���q        1���a
 *  �e�i �i��          4���a
 *  �w�� �i��          4���a
 *  �e�i �a�e          1���a
 *  �A���� �wȁ        1���a
 *  -------------------------------------------------------------------
 *  ����               42 + 5 + 2 = 49�a���a
 */

struct cfg_t {
  unsigned char id[42];
  unsigned char def_ext[5];
  unsigned indent   : 1;
  unsigned tab_size : 4;
  unsigned formfeed : 1;
  unsigned han_font : 4;
  unsigned eng_font : 4;
  unsigned kbd_kind : 1;
  unsigned is_border: 1;
};

char config_id[] = "HANyang gulTLE ConFiGuration File V 1.03";
char config_file[80];
struct cfg_t cfg;

void initusermenu(int fcolor, int bcolor, int init_menu)
{
  nmenu = 6;         /* �a���a ���� 6 �� */

  pmenu[0].menu = "�e�i";
  pmenu[0].nitem = 7;
  pmenu[0].item[0] = "�e�i���e?";    pmenu[0].shortcut[0] = 0;
  pmenu[0].item[1] = "�����i";       pmenu[0].shortcut[1] = F1;
  pmenu[0].item[2] = "-";            pmenu[0].shortcut[2] = 0;
  pmenu[0].item[3] = "�i�b";         pmenu[0].shortcut[3] = 0;
  pmenu[0].item[4] = "���e��";       pmenu[0].shortcut[4] = 0;
  pmenu[0].item[5] = "-";            pmenu[0].shortcut[5] = 0;
  pmenu[0].item[6] = "�hӥ �a���i..";pmenu[0].shortcut[6] = 0;

  pmenu[1].menu = "��A��";
  pmenu[1].nitem = 8;
  pmenu[1].item[0] = "�� �i";        pmenu[1].shortcut[0] = ALT_N;
  pmenu[1].item[1] = "���ᵡ��";     pmenu[1].shortcut[1] = ALT_O;
  pmenu[1].item[2] = "��w�a��";     pmenu[1].shortcut[2] = ALT_S;
  pmenu[1].item[3] = "�����q�a��";   pmenu[1].shortcut[3] = ALT_V;
  pmenu[1].item[4] = "-";            pmenu[1].shortcut[4] = 0;
  pmenu[1].item[5] = "�w���a����";   pmenu[1].shortcut[5] = 0;
  pmenu[1].item[6] = "���a�a�i��";   pmenu[1].shortcut[6] = ALT_F10;
  pmenu[1].item[7] = "�{";           pmenu[1].shortcut[7] = ALT_X;

  pmenu[2].menu = "�i��";
  pmenu[2].nitem = 9;
  pmenu[2].item[0] = "�i�� ���b";    pmenu[2].shortcut[0] = CTRL_B;
  pmenu[2].item[1] = "�i�� �{";      pmenu[2].shortcut[1] = CTRL_E;
  /* CTRL_H is 8 --> ASCII 8 is BS */
  pmenu[2].item[2] = "�i�� �a��";    pmenu[2].shortcut[2] = CTRL_O;
  pmenu[2].item[3] = "�i�� ���a";    pmenu[2].shortcut[3] = CTRL_C;
  pmenu[2].item[4] = "�i�� ����";    pmenu[2].shortcut[4] = CTRL_V;
  pmenu[2].item[5] = "�i�� ������";  pmenu[2].shortcut[5] = CTRL_D;
  pmenu[2].item[6] = "-";            pmenu[2].shortcut[6] = 0;
  pmenu[2].item[7] = "�i�� ����";    pmenu[2].shortcut[7] = CTRL_R;
  pmenu[2].item[8] = "�i�� �a��";    pmenu[2].shortcut[8] = CTRL_W;

  pmenu[3].menu = "�x��";
  pmenu[3].nitem = 8;
  pmenu[3].item[0] = "�x��";         pmenu[3].shortcut[0] = ALT_F;
  pmenu[3].item[1] = "�a����";       pmenu[3].shortcut[1] = ALT_A;
  pmenu[3].item[2] = "�a�� �x��";    pmenu[3].shortcut[2] = ALT_L;
  pmenu[3].item[3] = "-";            pmenu[3].shortcut[3] = 0;
  pmenu[3].item[4] = "�i�� ��q�a��";pmenu[3].shortcut[4] = ALT_B;
  pmenu[3].item[5] = "�i�� �{�a��";  pmenu[3].shortcut[5] = ALT_E;
  pmenu[3].item[6] = "-";            pmenu[3].shortcut[6] = 0;
  pmenu[3].item[7] = "���� ��á��";  pmenu[3].shortcut[7] = CTRL_Z;

  pmenu[4].menu = "�b";
  pmenu[4].nitem = 4;
  pmenu[4].item[0] = "���A ����";    pmenu[4].shortcut[0] = ALT_P;
  pmenu[4].item[1] = "�i�� ����";    pmenu[4].shortcut[1] = CTRL_A;
  pmenu[4].item[2] = "-";            pmenu[4].shortcut[2] = 0;
  pmenu[4].item[3] = "���� ���";    pmenu[4].shortcut[3] = CTRL_P;

  pmenu[5].menu = "��Ȃ�a�w";
  pmenu[5].nitem = 12;
  pmenu[5].item[0] = (indent_mode) ? "����a��" : "�i�a�a��" ;
  pmenu[5].shortcut[0] = CTRL_I;
  pmenu[5].item[1] = "ȓ�a�� �a����";pmenu[5].shortcut[1] = 0;
  pmenu[5].item[2] = (formfeed_mode) ? "����ҁ �e���q" : "����ҁ ���q  " ;
  pmenu[5].shortcut[2] = 0;
  pmenu[5].item[3] = (is_border) ? "�A���� �e�a��" : "�A���� �a��  " ;
  pmenu[5].shortcut[3] = 0;
  pmenu[5].item[4] = "�¸w�a ���";  pmenu[5].shortcut[4] = 0;
  pmenu[5].item[5] = "-";            pmenu[5].shortcut[5] = 0;
  pmenu[5].item[6] = "�e�i �i��";    pmenu[5].shortcut[6] = F7;
  pmenu[5].item[7] = "�w�� �i��";    pmenu[5].shortcut[7] = F8;
  pmenu[5].item[8] = "-";            pmenu[5].shortcut[8] = 0;
  pmenu[5].item[9] = "�e�i �a�e";    pmenu[5].shortcut[9] = ALT_K;
  pmenu[5].item[10] = "-";           pmenu[5].shortcut[10] = 0;
  pmenu[5].item[11] = "�ŉw ��w";   pmenu[5].shortcut[11] = 0;

  definemenu(fcolor, bcolor, init_menu, 2);
}


/*
 *  �a�a���� �A�᷁ ������a Ѕ�i��
 */
int handler(void)
{
  hardretn(-1);
  return YES;
}

void init_sys_color(void)
{
  int isHGC;

  isHGC = _detecthgc();
  if(!isHGC) {
    fcolor = WHITE, bcolor = BLUE;
    menu_fcolor = WHITE, menu_bcolor = BLUE;
  }
  else {
    fcolor = WHITE, bcolor = BLACK;
    menu_fcolor = BLACK, menu_bcolor = WHITE;
  }
}

void disp_copy_right_msg(void)
{
  bool threed, rev, under;

  threed = isthreed();
  rev = isreverse();
  under = isunder();
  hsetthreed(true);
  hsetreverse(true);
  hsetunder(true);
  hputsxy(-60, -1, "(��) �e�i�a�� 1993");
  hsetthreed(threed);
  hsetreverse(rev);
  hsetunder(under);
}

/*
 *  menu support function
 */
void about_hantle(void)
{
  if(wopen(21, 9, 40, 10)) {
    wcenter(2, "Ԑ  �e�� �i�i 1.03 �e  ԑ");
    view_pcx_file(11*8, 2*16, logo_file, bcolor, fcolor);
    wait_getkey(5);
    wclose();
  }
}

/*
 *  �w�� '�hӥ �a���i �i�a'�i ���e.
 *  �a ���i�១ �w��嬗���� 'L'�� ���b�a�e �a�w �a�q�a�� �i
 *  �A�a���i ���ᥡ�e �w�e�� �����a.
 *
 *  Love(�a�w), Liberty(�a�A), Labor(����)
 *
 */
void cine_view(void)
{
  if(wopen(10, 4, 60, 19)) {
    wtitle("ٺ       �hӥ �a���i �i�a       ԑ");
    view_pcx_file(4, 4, cine_file, fcolor, bcolor);
    wait_getkey(5);
    wclose();
  }
}

void dosshell(int fcolor, int bcolor)
{
  char *comspec;

  hrestorecrtmode();
  comspec = (char*)getenv("COMSPEC");
  cprintf("         ����������������������������������������������������ͻ\n\r");
  cprintf("         �                   Hantle Dos Shell                 �\n\r");
  cprintf("         ����������������������������������������������������Ķ\n\r");
  cprintf("         �                                                    �\n\r");
  cprintf("         �       To return Hantle, type 'EXIT'. 1992 Kwon     �\n\r");
  cprintf("         �                                                    �\n\r");
  cprintf("         ����������������������������������������������������ͼ\n\r\n\r");

  putenv("PROMPT=$_Type EXIT to Return to Hantle$_(Hantle)$p$g");
  if(spawnl(P_WAIT, comspec, comspec, NULL) == -1) {
    cprintf("\n\r\n\r%s missing or Memory insufficent.   Press any key.... \n\r\n\r");
    getxch();
  }
  hsetgraphmode();
  hsetcolor(fcolor);
  hsetbkcolor(bcolor);
  drawmenubar();
  disp_copy_right_msg();
  disp_status();
  draw_border(scr_cfg[is_border].x, scr_cfg[is_border].y,
              scr_cfg[is_border].width, scr_cfg[is_border].height, scr_cfg[is_border].border);
  user_task = TASK_DISPLAYPAGE;
}

void change_hangul_font(void)
{
  char *title = "�e�i �i�� �a����";
  char *menu[] = {
    " 1.  �a�w�A     ",
    " 2.  �A���A     ",
    " 3.  �e�w���A   ",
    " 4.  �A���A     "
  };
  int ans;

  ans = popup(hgetmaxax()/2-strlen(menu[0])/2, 10, 4, menu, title, "1234", 1);
  if(ans == -1)
     return;
  registerhanfont(han_font[ans]);
  cur_han_font = ans;
  drawmenubar();
  disp_copy_right_msg();
  disp_status();
  user_task = TASK_DISPLAYPAGE;
}

void change_english_font(void)
{
  char *title = "�w�� �i�� �a����";
  char *menu[] = {
    " 1.  �a�w�A     ",
    " 2.  ���e�A     ",
    " 3.  �e �a�w�A  ",
    " 4.  �A���A     ",
    " 5.  �����A     ",
    " 6.  ���i���A   "
  };
  int ans;

  ans = popup(hgetmaxax()/2-strlen(menu[0])/2, 10, 6, menu, title, "123456", 1);
  if(ans == -1)
     return;
  if(ans >=0 && ans <= 2) {
    registerauxfont(NULL);
    registerengfont(eng_font[ans]);
  }
  else
    registerauxfont(aux_font[ans-3]);
  cur_eng_font = ans;
  drawmenubar();
  disp_copy_right_msg();
  disp_status();
  user_task = TASK_DISPLAYPAGE;
}

void change_ext_name(void)
{
  char ext_name[45];

  strcpy(ext_name, file_ext);
  if(get_ext_name(ext_name) == YES_ANS) {
    strcpy(file_ext, ext_name);
  }
}

void read_config(char *cfg_file)
{
  FILE *fcfg;

  fcfg = fopen(cfg_file, "rb");
  if(fcfg) {
    fread(&cfg, sizeof(struct cfg_t), 1, fcfg);

    indent_mode = cfg.indent;
    set_tabsize(cfg.tab_size);
    formfeed_mode = cfg.formfeed;
    is_border = cfg.is_border;
    strcpy(file_ext, cfg.def_ext);
    cur_han_font = cfg.han_font;
    registerhanfont(han_font[cur_han_font]);
    cur_eng_font = cfg.eng_font;
    if(cur_eng_font >=0 && cur_eng_font <= 2) {
      registerauxfont(NULL);
      registerengfont(eng_font[cur_eng_font]);
    }
    else
      registerauxfont(aux_font[cur_eng_font-3]);

    HanKbdKind = cfg.kbd_kind;
    if(HanKbdKind== HANKBD2)
      hsethan2board(1);   /* for halla */
    else
      hsethan390board(1);    /* for halla */
    fclose(fcfg);
  }
}

void write_config(char *cfg_file, char *cfg_id)
{
  FILE *fcfg;

  fcfg = fopen(cfg_file, "wb");
  if(fcfg) {
    sprintf(cfg.id, "%s%c", cfg_id, 0x1A);

    cfg.indent = indent_mode;
    cfg.tab_size = get_tabsize();
    cfg.formfeed = formfeed_mode;
    cfg.is_border = is_border;
    strcpy(cfg.def_ext, file_ext);
    cfg.han_font = cur_han_font;
    cfg.eng_font = cur_eng_font;
    cfg.kbd_kind = HanKbdKind;

    fwrite(&cfg, sizeof(struct cfg_t), 1, fcfg);
    fclose(fcfg);
    disp_cmsg_sec(" �ŉw �a���i ��wЖ�s���a ", 5);
  }
  else {
    disperror(" �ŉw �a���i ��w�i ���a ���A�a ");
  }
}

void save_config_file(void)
{
  write_config(config_file, config_id);
}

void hantle_menu(void)
{
  switch(getcuritem()) {
    case HANTLE_ITEM :
      about_hantle();
      break;
    case HELP_ITEM :
      help_manager(help_file);
      break;
    case CALENDAR_ITEM :
      calendar();
      break;
    case CALCULATOR_ITEM :
      calculator();
      break;
    case CINE_ITEM :
      cine_view();
      break;
  }
}

void file_menu(void)
{
  switch(getcuritem()) {
    case NEW_ITEM :
      new_file();
      break;
    case LOAD_ITEM :
      load();
      break;
    case SAVE_ITEM :
      save();
      break;
    case SAVEAS_ITEM :
      save_as();
      break;
    case CHDIR_ITEM :
      change_dir();
      break;
    case OS_ITEM :
      dosshell(fcolor, bcolor);
      break;
    case QUIT_ITEM :
      all_jobs_close = true;
      break;
  }
}

void block_menu(void)
{
  switch(getcuritem()) {
    case BBEGIN_ITEM :
      block_decision( &b_b_ptr);
      break;
    case BEND_ITEM :
      block_decision( &b_k_ptr );
      break;
    case BSHOW_ITEM :
      block_display = !block_display;
      user_task = TASK_DISPLAYPAGE;
      break;
    case BCOPY_ITEM :
      block_copy();
      break;
    case BMOVE_ITEM :
      block_move();
      break;
    case BDEL_ITEM :
      block_delete();
      break;
    case BREAD_ITEM :
      block_read();
      break;
    case BWRITE_ITEM :
      block_write();
      break;
  }
}

void find_menu(void)
{
  switch(getcuritem()) {
    case FIND_ITEM :
      find_condition(NO); /* replace NO */
      break;
    case REPLACE_ITEM :
      find_condition(YES); /* replace YES */
      break;
    case REPEAT_ITEM :
      break;
    case TOBBEGIN_ITEM :
      to_pointer(b_b_ptr);
      break;
    case TOBEND_ITEM :
      to_pointer(b_k_ptr);
      break;
    case TOPREV_ITEM :
      to_pointer(last_pos);
      break;
  }
}

void print_menu(void)
{
  switch(getcuritem()) {
    case ALLPRINT_ITEM :
      data_print(memory, eofm);
      break;
    case BPRINT_ITEM :
      data_print(b_b_ptr, b_k_ptr);
      break;
    case PRINTER_ITEM :
      break;
  }
}

void config_menu(void)
{
  unsigned char num[45];
  int tab_size;

  switch(getcuritem()) {
    case INDENT_ITEM :
      beep();
      indent_mode = !(indent_mode);
      pmenu[5].item[0] = (indent_mode) ? "����a��" : "�i�a�a��";
      disp_indent_status();
      break;
    case TAB_ITEM :
      tab_size = get_tabsize();
      itoa(tab_size, num, 10);
      if(get_number_to_set_tab(num) == YES_ANS) {
        tab_size = atoi(num);
        if(tab_size >= 0 && tab_size <= 8) {
          set_tabsize(tab_size);
        }
        else {
          disperror(" ȓ �췡�e 0 �= 8 ���ᴡ Ё�a ");
        }
      }
      break;
    case FORMFEED_ITEM :
      beep();
      formfeed_mode = !formfeed_mode;
      pmenu[5].item[2] = (formfeed_mode) ? "����ҁ �e���q" : "����ҁ ���q  ";
      break;
    case BORDER_ITEM :
      beep();
      is_border = !is_border;
      pmenu[5].item[3] = (is_border) ? "�A���� �e�a��" : "�A���� �a��  ";
      set_max_width_height(scr_cfg[is_border].width, scr_cfg[is_border].height);
      draw_border(scr_cfg[is_border].x, scr_cfg[is_border].y,
                  scr_cfg[is_border].width, scr_cfg[is_border].height, scr_cfg[is_border].border);
      user_task = TASK_DISPLAYPAGE;
      break;
    case EXT_ITEM :
      change_ext_name();
      break;
    case HANGUL_ITEM :
      change_hangul_font();
      break;
    case ENGLISH_ITEM :
      change_english_font();
      break;
    case KBD_ITEM :
      beep();
      if(HanKbdKind == HANKBD2) {
        hsethan390board(1);   /* for halla */
        HanKbdKind = HANKBD3;                /* for user(=JaeRock) define */
      }
      else {
        hsethan2board(1);         /* for halla */
        HanKbdKind = HANKBD2;                 /* for user(=JaeRock) define */
      }
      disp_kbd_status();
      break;
    case SAVECFG_ITEM :
      save_config_file();
      break;
  }
}

void main_menu(int key)
{
  if(getitem(key)) {
    switch(getcurmenu()) {
      case  HANTLE_MENU :
        hantle_menu();
        break;
      case  FILE_MENU :
        file_menu();
        break;
      case  EDIT_MENU :
        block_menu();
        break;
      case  FIND_MENU :
        find_menu();
        break;
      case  PRINT_MENU :
        print_menu();
        break;
      case  CONFIG_MENU :
        config_menu();
        break;
    } /* switch */
  } /* if */
}

void main(int argc, unsigned char *argv[])
{
  char edit_file[80], hanja_file[80], help_file[80];
  char init_dir[80];   /* �a���a���� ��q �����E ���� */
  char end_msg[]
    = " +     Hanyang Gultle-[Hantle] Release 1.03 (C) Copyright 1993 Hantle-Ary     + ";
  int ans;

  harderr(handler);

  inithanlib(FIXRESMODE, HANDETECT, HAN8GD2, ENGGD5);
  strcpy(init_dir, argv[0]);
  truncate_filename(init_dir);

  sprintf(hanja_file, "%s%s", init_dir, HANJA_FILE);
  sprintf(help_file, "%s%s", init_dir, HELP_FILE);
  sprintf(logo_file, "%s%s", init_dir, LOGO_FILE);
  sprintf(cine_file, "%s%s", init_dir, CINE_FILE);
  sprintf(config_file, "%s%s", init_dir, CONFIG_FILE);

#ifndef DEBUG
  registeregrfont(EGR1);
  registerhgrfont(HGR1);
  registerkssfont(KSS1);
  ans = loadhanjafont(hanja_file);
  hanja_font_exist = (ans == 0) ? true : false;
#endif
  read_config(config_file);
  init_sys_color();
  inithwindow(fcolor, bcolor);
  initusermenu(menu_fcolor, menu_bcolor, 1);
  menu_process = (*main_menu);
  strcpy(edit_file, (argc > 1) ? strupr(argv[1]) : "");

  hclrscr();
  drawmenubar();
  disp_copy_right_msg();
#ifdef DEBUG
  if(!init_editor(edit_file, help_file, 1000L, HanKbdKind, true))
#endif
  if(!init_editor(edit_file, help_file, 0L, HanKbdKind, true))
    return;

  editor(scr_cfg[is_border].x, scr_cfg[is_border].y,
         scr_cfg[is_border].width, scr_cfg[is_border].height, scr_cfg[is_border].border);
  closehan();
  textcolor(WHITE);
  textbackground(BLUE);
  cprintf(end_msg);
}

/*
 *  �a�� kedit.c �� �{
 */

