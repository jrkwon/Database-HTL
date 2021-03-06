/* -------------------------- HANSCRN.C ------------------------- */

/*
 *   ������ ���e�i �a�a���e �a�q�� �{�a.
 *
 *   +------------------------------------------------------+
 *   |   �A�A ����                                          |  <- ��ɷ
 *   +------------------------------------------------------+
 *   |                 ���e �A��                            |  <- �e��
 *   +------------------------------------------------------+
 *   |                                                      |
 *   | �� �����e �w�i ���Ё�� ���a�����i ��A�e�a.         |
 *   |                                                      |
 *   .                                                      .
 *   .                                                      .
 *   .                                                      .
 *   +------------------------------------------------------+
 *   |   �e ����                                          |  <- �e��
 *   +------------------------------------------------------+
 *   |  ���b�wȁ �a�� - �e/�w, 2�鯢3�鯢 �w�w              |  <- ��ɷ
 *   +------------------------------------------------------+
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dos.h>
#include "hanout.h"
#include "hanin.h"
#include "hancolor.h"
#include "hancoord.h"
#include "hanwindw.h"
#include "hanerro2.h"
#include "handbms.h"
#include "hanprint.h"
#include "extkey.h"
#include "ascii.h"

int cur_col = 0, cur_row = 0;    /* �e�� ��᷁ ��á */
int screen_displayed;            /* ���e�A �i�� �a�� �A���e�� */
extern struct coord_t coord[MAXELE];    /* �b �a�a �a���� ���b���a */

struct {
  int prot;           /* ϩ�a�� ��ѡ �a�� - ON/OFF  */
  int (*edits)();     /* �b �a��(ϩ�a)���b ҁ ��З �i �q�� */
} sb [MAXELE];

int *elist;           /* �a�� ���a�a */
char *bf;             /* ���e ���� ͡���� */
char *tname;          /* ���b ���e�� �A�� */
char *prim_key;       /* ��ǡ�i ���aЁ���a */

byte picture[] = "#CcAa9HhEeKkLlYy!xX*";  /* �a�a���b�A �a���e ���� */
/* ****
 char *but_str[] = {
  "  ��w  ", "  �᭡  ", "  �|  ", "  ��  ", "  �� �|  ", "  �� ��  ", "  ����  ", "  �añ  " };
***** */
char query_help_str[] = "  PgUp-�|   PgDn-��   ^PgUp-�a�w �|   ^PgDn-�a�w ��   F5-�᭡  ESC-�{���� ";

void (*_disp_card)(void);  /* ���� ���e�A �a�w�a�a �a�i �� */

static int elp(int el);
static int no_flds(void);
static void data_coord(int el);
static void hookhangulmodetoggle(bool hangulmode);
static void hookinsertmodetoggle(bool insertmode);
static void hookmistyping(void);
static void init_input(void);
static void close_input(void);
static void put_status_line(void);
static int read_element(char type, char *msk, char *bfr, int el);
static void left_justify(char *s);
static void right_justify(char *s);
static void right_justify_zero_fill(char *s);
static int validate_date(char *s);
static void disp_element(char *b, char *msk, int el);

/* �������i �a���e�a */
void post_notice(char *s)
{
  disp_cmsg_sec(s, 1);
}

void clear_scrn(void)
{
  screen_displayed = FALSE;
  hclrscr();
}

/* ���e �១�i ���e ������ */
void init_screen(char *name, int *els, char *bfr)
{
  tname = name;
  elist = els;
  bf = bfr;
}

/* �b ϩ�a�A ��ѡ �����a�i ������a �E�a */
void protect(int el, int tf)
{
  sb[elp(el)].prot = tf;
}

/* �b �a��(el)�a ���b�E ҁ �����e �q���i ����e�a */
void edit(int el, int (*func)())
{
  sb[elp(el)].edits = func;
}

void clear_edit(void)
{
  int i;

  for(i = 0; i < MAXELE; i++)
    sb[i].edits = NULL;
}

/* �aǡ�a�A�� �����e �a�a�a����ѡ�A ��Ё,
 * �a�����a�a�A ��i�E �a�a�a���� �w�� ��ѡ
 * ���A �a�a �a����ѡ�i ���e �a�� ���a�a�A�� �y�弁���� ���a���a.
 * �a�� �a�a�a���a ���e �w�� ���A�a���i �a���g�a�a�� ����!
 */
static int elp(int el)
{
   int i;

   for (i = 0; *(elist + i); i++)
     if (el == *(elist + i))
       break;
     return i;
}

/* *************
void mark_pannel(int butt_ctr)
{
  static int flag = FALSE;
  int sum = BUTTON_GAP, ctr = butt_ctr;
  char ch1[2], ch2[2];
  bool onoff, onoff2;

  onoff = isreverse();
  onoff2 = isshadow();
  if(flag)
    hsetshadow(ON);
  else
    hsetreverse(ON);
  while(ctr--)
    sum += strlen(but_str[ctr])+BUTTON_GAP;
  ch1[1] = ch2[1] = '\0';
  ch1[0] = 16, ch2[0] = 17;
  hputsxy(-sum, -BUTTON_Y, but_str[butt_ctr]);
  eputsxy(-sum, -BUTTON_Y, flag ? " " : ch1);
  eputsxy(-(sum+strlen(but_str[butt_ctr])-1), -BUTTON_Y, flag ? " " : ch2);
  if(flag)
    hsetshadow(onoff2);
  else
    hsetreverse(onoff);
  flag = !flag;
}
***************** */

/* ���e�A ���b �i�i �a�a���a. */
void display_template(char *title, int menu_flag)
{
   int i, el, ct; /* sum; */
   char detag[80], *cp1, *cp2;
   bool onoff;

   ct = no_flds();
   sethwindow(1, 1, hgetmaxax(), hgetmaxay());
   /* �A�� �a���� */
   drawbox(1, TITLE_Y, hgetmaxax(), hgetmaxay()-2, 1);

   onoff = isreverse();
   hsetreverse(ON);
   eputchs(1, -TITLE_Y, hgetmaxax(), ' ');
   hputsxy(-(hgetmaxax()/2-strlen(title)/2), -TITLE_Y, title);
   hsetreverse(onoff);

   /* �����i �w ��� */
   sethwindow(2, TITLE_Y+1, hgetmaxax()-1, BUTTON_Y-1);
   screen_displayed = false;

  if(menu_flag) {
     onoff = isshadow();
     hsetshadow(ON);
     hputsxy(-1, -BUTTON_Y, query_help_str);
     eputchs(-strlen(query_help_str)-1, -BUTTON_Y, hgetmaxax()-strlen(query_help_str), ' ');
     hsetshadow(onoff);
  }
/* ************
   if(menu_flag) {
     onoff = isshadow();
     hsetshadow(ON);
     eputchs(-1, -BUTTON_Y, hgetmaxax(), ' ');
     for(sum = BUTTON_GAP, i = 0; i < 8; i++) {
       hputsxy(-sum, -BUTTON_Y, but_str[i]);
       sum += strlen(but_str[i]) + BUTTON_GAP;
     }
     hsetshadow(onoff);
   }
************* */
   if(_disp_card)
     (*_disp_card)();
   for(i = 0; i < ct; i++) {
     el = *(elist + i);
     cp1 = elmask[el-1];
     cp2 = detag;
     while(*cp1 && cp2 < detag + sizeof detag - 1) {
       if(*cp1 & 0x80)
         *cp2++ = *cp1++, *cp2++ = *cp1++;
       else {
         *cp2++ = strchr(picture, *cp1) ?  ' ' : *cp1;
         cp1++;
       }
     }
     *cp2 = '\0';
     data_coord(el);  /* �a�a�a�� el�� ���b���a �t�i cur_col�� cur_row�A ��� */
     hputsxy(cur_col, cur_row, detag); /* ���e �����i �b */
     cur_col -= (strlen(hdenames[el-1])+1);
     onoff = isbold();
     hsetbold(ON);
     hputsxy(cur_col, cur_row, hdenames[el-1]); /* �e�i �a�a�a�� �b */
     hsetbold(onoff);
   }
   screen_displayed = TRUE;
}

/* ���e ���b�i�A�� �a�a�i ���b�h�e�a */
int data_entry(char *pkey, int file)
{
  int (*validfunct)();
  int field_ctr, exitcode, el, index_ctr = 0;
  int field_ptr = 0, done = FALSE, isvalid;
  char *bfptr;

  prim_key = pkey;
  if (screen_displayed == FALSE)
    display_template(tname, 1);
  tally();
  field_ctr = no_flds();
  while(index_ele[file][0][index_ctr])
    index_ctr++;

  while (done == FALSE) {
     bfptr = bf + epos(elist[field_ptr], elist);
     el = *(elist + field_ptr);
     validfunct = sb[field_ptr].edits;
     if (sb[field_ptr].prot == FALSE) {
       exitcode = read_element(eltype[el-1], elmask[el-1], bfptr, el);
       isvalid = (exitcode != CANCELKEY && validfunct) ? (*validfunct)(bfptr,exitcode) : DBOK;
     }
     else {
       exitcode = DOWNARROW;
       isvalid = DBOK;
     }
     if (isvalid == DBOK) {
       switch (exitcode)  {
         case '\r' :
           if(field_ptr < index_ctr && spaces(bfptr))
             break;
           if(field_ptr+1 == field_ctr)
             done = TRUE, exitcode = SAVEKEY;
           else
             field_ptr++;
           break;
         case DOWNARROW : case '\t' :
           if(field_ptr < index_ctr && spaces(bfptr))
             break;
           else if(field_ptr+1 == field_ctr)
             field_ptr = index_ctr;
           else
             field_ptr++;
           break;
         case UPARROW : case SHIFT_TAB :
           if(field_ptr == index_ctr)
             field_ptr = field_ctr-1;
           else if(field_ptr)
             field_ptr--;
           break;
         case SWITCHKEY :
           done = TRUE;
           break;
         default:
           done = hookhgetdata(&exitcode);
           exitcode = _lastkey;
       }    /* switch */
     }   /* if */
  }   /* while */
  return (exitcode);
}

/* ���e ���b�i�A�� ϩ�a�� �����i ���e */
static int no_flds(void)
{
  int ct = 0;

  while (*(elist + ct))
    ct++;
  return ct;
}

/* �a�a �a���� ���b ���a */
static void data_coord(int el)
{
  cur_col = coord[elp(el)].x;
  cur_row = coord[elp(el)].y;
  hgotoxy(cur_col, cur_row);
}

/* 3�a���a�a ���a�i ���e�a. */
char *ins_comma(char *str)
{
  char tmp[80], tmp2[80];
  int i, j, len, tlen, comma, cnt;

  if(strchr(str, ','))
    return str;

  tlen = strlen(str);  /* ���� ���� */
  /* ���e ���� �A�� */
  for(i = 0, j = 0; i < tlen; i++)
    if(str[i] != ' ')
      tmp[j++] = str[i];
  tmp[j] = '\0';

  len = strlen(tmp);
  /* ���a ���� */
  comma = len/3;
  if((!(len % 3)) && len)
    comma--;
  for(i = len-1, j = tlen-1, cnt = 1; j >= 0; j--, cnt++) {
    if((!(cnt % 4)) && cnt && (i >= 0))
      tmp2[j] = ',';
    else {
     if(i < 0) {
        while(j)
          tmp2[j--] = ' ';
        tmp2[j] = ' ';
        break;
      }
      else {
        tmp2[j] = tmp[i];
        i--;
      }
    }
  }
  tmp2[tlen] = '\0';
  strcpy(str, tmp2);
  return str;
}

/* ���a�i �A���e�a. */

char *del_comma(char *str)
{
  char temp[80], *tmp, *tmp2, temp2[80], cnt=0;

  if(!strchr(str, ','))
    return str;
  tmp = temp;
  tmp2 = str;
  while(*tmp2) {
    if(*tmp2 != ',')
      *tmp++ = *tmp2++;
    else
      tmp2++, cnt++;
  }
  *tmp = '\0';
  tmp2 = temp2;
  while(cnt--)
    *tmp2++ = ' ';
  cnt = 0;
  while(temp[cnt])
    *tmp2++ = temp[cnt++];
  *tmp2 = '\0';
  strcpy(str, temp2);
  return str;
}

static void sound_effect(void)
{
  int i;

  for(i = 0; i < 3; i++) {
    sound(300+i*3);
    delay(8);
  }
  for(i = 0; i < 3; i++) {
    sound(300+i*3);
    delay(8);
  }
  nosound();
}

static void hookhangulmodetoggle(bool hangulmode)
{
  int onoff;

  onoff = isreverse();
  hsetreverse(ON);
  hputsxy(-1, -hgetmaxay(), hangulmode ? " �e�i ���bԳ" : " �w�� ���bԳ");
  hsetreverse(onoff);
  if(_hangulmode)
    hsetbufcursor(FRAMECURSOR, FRAMECURSOR, DEFAULTCOLOR);
  else
    hsetbufcursor(UNDERCURSOR, UNDERCURSOR, DEFAULTCOLOR);
  sound_effect();
}

static void hookinsertmodetoggle(bool insertmode)
{
  int onoff;

  onoff = isreverse();
  hsetreverse(ON);
  hputsxy(-13, -hgetmaxay(), insertmode ? "�s�� �w��Գ" : "���� �w��Գ");
  hsetreverse(onoff);
  sound(600);
  delay(100);
  nosound();
}

static void hookmistyping(void)
{
  sound(600);
  delay(100);
  nosound();
}

static void init_input(void)
{
  _hookhangulmodetoggle = hookhangulmodetoggle;
  _hookinsertmodetoggle = hookinsertmodetoggle;
  _hookmistyping = hookmistyping;
/*  _hookhgetdata = hookhgetdata; */
}

static void close_input(void)
{
  _hookhangulmodetoggle = NULL;
  _hookinsertmodetoggle = NULL;
  _hookmistyping = NULL;
  _hookhgetdata = NULL;
}

/*
 *
 �e�i ���bԳ�s�� �w��Գ 2�鯢 Գ  F1 �����i Գ F2 ��w Գ F8 ���� Գ F10 �a���a
 *
 */
static void put_status_line(void)
{
  hputsxy(-1, -hgetmaxay(), _hangulmode ? " �e�i ���bԳ" : " �w�� ���bԳ");
  hputsxy(-13, -hgetmaxay(), _insertmode ? "�s�� �w��Գ" : "���� �w��Գ");
  hputsxy(-24, -hgetmaxay(), ishan2board() ? " 2�鯢 Գ" : " 3�鯢 Գ");
  hputsxy(-33, -hgetmaxay(), "  F1 �����i Գ");
  hputsxy(-47, -hgetmaxay(), " F2 ��w Գ");
  hputsxy(-58, -hgetmaxay(), " F8 ���� Գ");
  hputsxy(-69, -hgetmaxay(), " F10 �a���a ");
}
/*
 *  ���A ���b�i �h�e ����
 *
 *  �e�� �e�i �a���a�១�A �x���� �a�a�w�i �a�� �����a�e �a�q�� �{�a
 *
 *  H : ���b ҁ �a���� �១�i �g�e�a
 *  D : �i�a (���b ҁ �i�a�a �e.��.�� ���ᝡ �A�� ���e�� ��a
 *  N : ���a ���b ҁ ���e�� ���i, �E�� �����A�e �����i ����
 *  Z : ���a ���b ҁ ���e�� ���i, �E�� �����A 0�i ����
 *  M : ���b�h�� �g�� �a�� ����a...
 *  C : ���a ���b ҁ ���e�� ���i, �E�������A ���� ������, 3�a�� �a�a ���a(,) �s��
 *      - �q�� �a���A ���� �e�a. �a�a �a���e ���a�a�� ���aЁ���e�a.
 *  G : �a�� �a�� ���b, �aā��a �a���A�� ���b�h�q. �a���e ! �a�� ���� �e�Q.
 */

bool hookhgetdata(int *keycodep)
{
  bool ans, onoff;

  onoff = isreverse();
  hsetreverse(ON);
  if (keycodep == NULL) {  /* Initialization before key input */
    put_status_line();
    init_input();
    hsetreverse(onoff);
    return false;
  }
  switch (*keycodep) {
    case ESC : case DOWNARROW : case '\t' : case F1 : case UPARROW :
    case PGUP:  case PGDN: case CTRL_PGUP : case CTRL_PGDN : case F5 :
    case F2  :  case F8  : case F10 :  case SHIFT_TAB :
      *keycodep = CR;  /* If *keycodep = CR or ESC, quit hgetline */
      close_input();
      ans = true;
      break;
    case ALT_K:
      if (ishan2board()) {
        hputsxy(-24, -hgetmaxay(), " 3�鯢 Գ");
        hsethan390board(issplitmode());
      } else {
        hputsxy(-24, -hgetmaxay(), " 2�鯢 Գ");
        hsethan2board(issplitmode());
      }
      ans  = false;
      break;
    default :
      ans =  false;
      break;
  }
  hsetreverse(onoff);
  return ans;
}

static int read_element(char type, char *msk, char *bfr, int el)
{
   char temp[80], tmpbuf[80] = "", tmpbuf2[80] = "";
   int c, len, i = 0, allowcanceldelline, restoreline, cuttail, allowfirstdelline;
   int tmpins;
   bool onoff;

   if(type == 'G') {
     /*
      *  �a�����b(�aā�� ���e �a��) ����
      *  �a���a���� ���q�e ���a �a���� ��ǡ�i ���w�e�a.
      *  �a�a�� �a���� �i��a�e �a���� �w�� ���sǡ�� �a�w�e ���a�w�a�a.
      */
   }
   else if(type == 'M') {
     return _lastkey;
   }
   else {  /* �a ���A�� �a�a���b */
     sethwindow(2, TITLE_Y+1, hgetmaxax()-1, BUTTON_Y-1);
     data_coord(el);
     len = strlen(bfr);
     strcpy(temp, bfr);
     strcpy(tmpbuf, bfr);
     _hookhgetdata = hookhgetdata;
     allowcanceldelline = _allowcanceldelline, restoreline = _restoreline;
     cuttail = _cuttail, allowfirstdelline = _allowfirstdelline;
     _allowcanceldelline = _restoreline = _cuttail = _allowfirstdelline = false;
     putindata(tmpbuf, msk, tmpbuf2);
     strcpy(tmpbuf, tmpbuf2);
     if(type == 'C') {
       del_comma(tmpbuf);
       left_justify(tmpbuf);
     }
     tmpins = _insertmode;
     /* ���a ���b�� �ŝe�E �w���e �����wȁ�� */
     if(type == 'C' || type == 'D' || type == 'Z' || type == 'N')
       _insertmode = false;
     onoff = /*isunder(); */ isreverse();
     /* hsetunder(ON);     */ hsetreverse(ON);
     hgetdata(tmpbuf, msk);
     /* hsetunder(onoff); */ hsetreverse(onoff);
     _insertmode = tmpins;
     c = _lastkey;

     /* buff�A�� ���w(picture)�A�� */
     pickoutdata(tmpbuf, msk, tmpbuf2);
     while(tmpbuf[0] && len > i) {
       temp[i] = tmpbuf2[i];
       i++;
     }
     _allowcanceldelline = allowcanceldelline, _restoreline = restoreline;
     _cuttail = cuttail, _allowfirstdelline = allowfirstdelline;
     if (c != ESC) { /* ���b �i���a �᭡�A�� �g�v�a�e */
        switch(type) { /* �a�a�a���� �w�A �a�a �i���i ��a */
          case 'D' :
            validate_date(temp);
            break;
          case 'Z' :
            right_justify_zero_fill(temp);
            break;
          case 'N' :
            right_justify(temp);
            break;
          case 'C' :
            ins_comma(temp);
            break;
        } /* switch */
     } /* if */
     strcpy(bfr, temp);
     sethwindow(2, TITLE_Y+1, hgetmaxax()-1, BUTTON_Y-1);
     data_coord(el);
     disp_element(bfr,msk,el);
     if(hookhgetdata(&c))
       return _lastkey;
   }
   return (c);
}

/* ���e�A�� �a�a�a���i ��Ȃ�h����Ё */
static int onoff_element(int el)
{
  bool onoff;
  int ch;

  data_coord(el);
  cur_col -= (strlen(hdenames[el-1])+1);
  onoff = isreverse();
  hsetreverse(ON);
  hputsxy(cur_col, cur_row, hdenames[el-1]); /* �e�i �a�a�a�� �b */
  hsetreverse(onoff);
  ch = getchext();
  hputsxy(cur_col, cur_row, hdenames[el-1]); /* �e�i �a�a�a�� �b */
  return ch;
}

/* �a�a�a���i ��Ȃ�a�e�A �e�a�A. */
int *get_ele_list(int ele_list[])
{
  int field_ctr, exitcode, el, line_width, width;
  int field_ptr = 0, done = FALSE;
  bool onoff;

  line_width = 0;
  onoff = isshadow();
  hsetshadow(ON);
  eputchs(-1, -BUTTON_Y, hgetmaxax(), ' ');
  hputsxy(-1, -BUTTON_Y, "  [�a������]-��Ȃ����   ��-��Ȃ����   ԑ-Ԑ �a�q�w��   F2-����/��w   ESC-�᭡");
  hsetshadow(onoff);
  field_ctr = no_flds();
  while (done == FALSE) {
     el = *(elist + field_ptr);
     exitcode = onoff_element(el);
     switch (exitcode)  {
       case DOWNARROW : case '\t' : case LEFTARROW :
         if(field_ptr+1 == field_ctr)
           field_ptr = 0;
         else
           field_ptr++;
         break;
       case UPARROW : case SHIFT_TAB : case RIGHTARROW :
         if(field_ptr)
           field_ptr--;
         else
           field_ptr = field_ctr-1;
         break;
       case ESC : /* �᭡ */
         return NULL;
       case SAVEKEY : /* ��Ȃ �Şa */
         done = TRUE;
         ele_list[field_ctr] = -1;
         break;
       case ' ' :
         if(!ele_list[field_ptr]) {
           width = headlen(el);
           if(line_width+width+1 >= PAPERWIDTH) {
             disperror("  �����A �A�e �e���� �ᢁ ����a  ");
             break;
           }
           ele_list[field_ptr] = el;
           line_width += width + 1;
         }
         else {
           ele_list[field_ptr] = 0;
           line_width -= width - 1;
         }
         data_coord(el);
         eputsxy(cur_col, cur_row, ele_list[field_ptr] ? "�" : " ");
         break;
       case '\r' :
         if(!ele_list[field_ptr]) {
           width = headlen(el);
           if(line_width+width+1 >= PAPERWIDTH) {
             disperror("  �����A �A�e �e���� �ᢁ ����a  ");
             break;
           }
           ele_list[field_ptr] = el;
           line_width += width + 1;
         }
         else {
           ele_list[field_ptr] = 0;
           line_width -= width - 1;
         }
         data_coord(el);
         eputsxy(cur_col, cur_row, ele_list[field_ptr] ? "�" : " ");
         if(field_ptr+1 == field_ctr)
           field_ptr = 0;
         else
           field_ptr++;
         break;
     }    /* switch */
  }   /* while */
  return ele_list;
}

/* �E�� ���i�a�� ���e���� ���e�e �����a�� �����a */
static void left_justify(char *s)
{
  int len, cnt = 0, cnt2;
  char tmp[40], *str;

  len = strlen(s);
  str = s;
  while(*str) {
    if(*str == ' ')
      str++;
    else
      tmp[cnt++] = *str++;
  }
  cnt2 = len - cnt;
  while(cnt2--)
    tmp[cnt++] = ' ';
  tmp[cnt] = '\0';
  strcpy(s, tmp);
}

/* ���a�i s�i ���e�� ���i(���e�e �����a�� �����a */
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

/* ���a�i s�i ���e�� ���i(���e�e 0�a�� �����a */
static void right_justify_zero_fill(char *s)
{
  int len;

  if (spaces(s))
    return;
  len = strlen(s);
    while (*(s + len - 1) == ' ')   {
  mov_mem(s, s + 1, len-1);
    *s = '0';
  }
}

/* ���a�i c�e�A �����e �i�᷶�a�e �q �t�i ���a���a */
int spaces(char *c)
{
   while (*c == ' ')
     c++;
   return !*c;
}

/* ���a�i s�a �e������ �A��� �b�b�� �t�� ��w�e�a  */
static int validate_date(char *s)
{
  static int days [] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
  char date [7];
  int mo, yr;

  strncpy(date, s, 6);
  date[6] = '\0';
  if (spaces(date))
    return DBOK;
  *(date + 4) = '\0';
  mo = atoi(date+2);
  *(date + 2) = '\0';
  yr = atoi(date);
  days[1] = (yr % 4)  ? 28 : 29;
  if (mo && mo < 13 && atoi(s+4) && atoi(s+4) <= days [mo - 1])
    return DBOK;
  post_notice("�i�a�a �i���A����a, �e.��.�� ���ᝡ �A�ᴡЁ�a");
  return DBERROR;
}

/* ���e�� ���e ϩ�a�A �a�a �a�� �t �b */
void tally(void)
{
   int *els = elist;

   sethwindow(2, TITLE_Y+1, hgetmaxax()-1, BUTTON_Y-1);
   while (*els)
     put_field(*els++);
}

/* �a�a �a���i ���e �a�a�a�� �q�A ���e�A �b�e�a */
void put_field(int el)
{
   data_coord(el);
   disp_element(bf + epos(el, elist), elmask[el - 1], el);
}

/* b�e ���e ����A�� �e�� ϩ�a�� ��á  ͡����, �a�a �a���i ���e�A �b */
static void disp_element(char *b, char *msk, int el)
{
   char merge[80];
   register i = 0;

   if(eltype[el-1] == 'G') { /* �a�� �a���� �w�� */
     /*
      * prime_key�i �a�����q�a�� �a�e PCX�a���i �����
      * ��Ё�� ���a�� �a���� ���e�A �b�e�a.
      * �a�a�� �a�a�a�� ���A �a���� ���e �a�a�a���� �w���A�e
      * ��ǡ�i 8�a ������ �a�e�A ���V��?
      */
     /* put_pcx(prim_key); */ /* PCX�a�� ���� */
   }
   else { /* �a ���A�� �w�� */
     while (*msk)    {
       if(*msk & 0x80) {
         merge[i++] = *msk++;
         merge[i++] = *msk++;
       }
       else {
         merge[i] = strchr(picture, *msk) ? *b++ : *msk;
         msk++, i++;
       }
     }
     merge[i] = '\0';
     hputs(merge);   /* ���e�i�i ͡�q�e �a�a�a���� ���a�i */
   }
}


