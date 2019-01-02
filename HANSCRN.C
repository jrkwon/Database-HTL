/* -------------------------- HANSCRN.C ------------------------- */

/*
 *   ª©∑Å¶Å —¡°e∑i ãaùa•°°e îa∑qâ¡ à{îa.
 *
 *   +------------------------------------------------------+
 *   |   °AìA ¶Å¶Ö                                          |  <- •°…∑
 *   +------------------------------------------------------+
 *   |                 —¡°e πA°¢                            |  <- §e∏Â
 *   +------------------------------------------------------+
 *   |                                                      |
 *   | ∑° êÅ¶Åìe ¿w∑i ¨È∏˜–Å¨· π¡Œaª°∏˜∑i ÆÛâA–eîa.         |
 *   |                                                      |
 *   .                                                      .
 *   .                                                      .
 *   .                                                      .
 *   +------------------------------------------------------+
 *   |   îe¬Å ¶Å¶Ö                                          |  <- §e∏Â
 *   +------------------------------------------------------+
 *   |  ∑≥ùb¨w»Å ŒaØ° - –e/µw, 2§ÈØ¢3§ÈØ¢ ówów              |  <- •°…∑
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

int cur_col = 0, cur_row = 0;    /* —e∏Å ƒ·¨·∑Å ∂·√° */
int screen_displayed;            /* —¡°eµA Ài∑° ŒaØ° ñA¥ˆìeª° */
extern struct coord_t coord[MAXELE];    /* àb ∏aûa ∂a≠°∑Å ∑≥ùbπ¡Œa */

struct {
  int prot;           /* œ©óa∑Å •°—° µa¶Å - ON/OFF  */
  int (*edits)();     /* àb ∂a≠°(œ©óa)∑≥ùb “Å Ø©–ó –i –qÆÅ */
} sb [MAXELE];

int *elist;           /* ∂a≠° ü°ØaÀa */
char *bf;             /* —¡°e §·Ã· Õ°∑•»· */
char *tname;          /* ∑≥ùb —¡°e∑Å πA°¢ */
char *prim_key;       /* ∫Å«°üi •¢¨a–ÅñÖîa */

byte picture[] = "#CcAa9HhEeKkLlYy!xX*";  /* ∏aûa∑≥ùbµA ≥a∑°ìe ¥∑Ø¢ */
/* ****
 char *but_str[] = {
  "  ∏·∏w  ", "  ¬·≠°  ", "  ¥|  ", "  ñ·  ", "  †Ö ¥|  ", "  †Ö ñ·  ", "  ª°∂ë  ", "  †a√±  " };
***** */
char query_help_str[] = "  PgUp-¥|   PgDn-ñ·   ^PgUp-àa∏w ¥|   ^PgDn-àa∏w ñ·   F5-¬·≠°  ESC-è{êÅã° ";

void (*_disp_card)(void);  /* ã°•• —¡°eµA ¨a∂w∏aàa ¬Åàa–i à· */

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

/* ª°Ø°¢Ö∑i ŒaØ°–eîa */
void post_notice(char *s)
{
  disp_cmsg_sec(s, 1);
}

void clear_scrn(void)
{
  screen_displayed = FALSE;
  hclrscr();
}

/* —¡°e ¿·ü°üi ∂·–e ¡°ã°—¡ */
void init_screen(char *name, int *els, char *bfr)
{
  tname = name;
  elist = els;
  bf = bfr;
}

/* àb œ©óaµA •°—° ç°ü°Œaüi ¶õ∑°à·êa ôEîa */
void protect(int el, int tf)
{
  sb[elp(el)].prot = tf;
}

/* àb ∂a≠°(el)àa ∑≥ùbñE “Å ¶âü°ìe –qÆÅüi ¨È∏˜–eîa */
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

/* Øa«°†aµA¨· ∏˜∑Å–e ∏aûa∂a≠°§Â—°µA îÅ–Å,
 * ∂a≠°ü°ØaÀaµA ∏ÂîiñE ∏aûa∂a≠°∑Å ¨wîÅ §Â—°
 * Ø©πA ∏aûa ∂a≠°§Â—°üi ∫Å°e ∂a≠° ü°ØaÀaµA¨· °y§ÂºÅ∑•ª° ï©ùa∫Öîa.
 * ãaúÂ ∏aûa∂a≠°àa ¥Ùìe âw∂Å µ°üAŒaØ°üi –aª°¥g∑a£aù° π°Ø±!
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

/* —¡°eµA ∑≥ùb Ài∑i ãaùa∫Öîa. */
void display_template(char *title, int menu_flag)
{
   int i, el, ct; /* sum; */
   char detag[80], *cp1, *cp2;
   bool onoff;

   ct = no_flds();
   sethwindow(1, 1, hgetmaxax(), hgetmaxay());
   /* ëA°° ãaü°ã° */
   drawbox(1, TITLE_Y, hgetmaxax(), hgetmaxay()-2, 1);

   onoff = isreverse();
   hsetreverse(ON);
   eputchs(1, -TITLE_Y, hgetmaxax(), ' ');
   hputsxy(-(hgetmaxax()/2-strlen(title)/2), -TITLE_Y, title);
   hsetreverse(onoff);

   /* êÅ¶Åüi ¿w ¨È∏˜ */
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
     data_coord(el);  /* ∏aûa∂a≠° el∑Å ∑≥ùbπ¡Œa àt∑i cur_colâ¡ cur_rowµA ¨È∏˜ */
     hputsxy(cur_col, cur_row, detag); /* —¡°e ¥∑Ø¢∑i ¬âùb */
     cur_col -= (strlen(hdenames[el-1])+1);
     onoff = isbold();
     hsetbold(ON);
     hputsxy(cur_col, cur_row, hdenames[el-1]); /* –eãi ∏aûa∂a≠° ¬âùb */
     hsetbold(onoff);
   }
   screen_displayed = TRUE;
}

/* —¡°e ∑≥ùbÀiµA¨· ∏aûaüi ∑≥ùb§hìeîa */
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

/* —¡°e ∑≥ùbÀiµA¨· œ©óa∑Å àïÆÅüi âÅ¨e */
static int no_flds(void)
{
  int ct = 0;

  while (*(elist + ct))
    ct++;
  return ct;
}

/* ∏aûa ∂a≠°∑Å ∑≥ùb π¡Œa */
static void data_coord(int el)
{
  cur_col = coord[elp(el)].x;
  cur_row = coord[elp(el)].y;
  hgotoxy(cur_col, cur_row);
}

/* 3∏aü°†aîa ÆÒŒaüi ê˝ìeîa. */
char *ins_comma(char *str)
{
  char tmp[80], tmp2[80];
  int i, j, len, tlen, comma, cnt;

  if(strchr(str, ','))
    return str;

  tlen = strlen(str);  /* ∂•úÅ ã©∑° */
  /* ∑©îe â∑§Ç πAà· */
  for(i = 0, j = 0; i < tlen; i++)
    if(str[i] != ' ')
      tmp[j++] = str[i];
  tmp[j] = '\0';

  len = strlen(tmp);
  /* ÆÒŒa ê˝ã° */
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

/* ÆÒŒaüi πAà·–eîa. */

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
  hputsxy(-1, -hgetmaxay(), hangulmode ? " –eãi ∑≥ùb‘≥" : " µw¢Ö ∑≥ùb‘≥");
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
  hputsxy(-13, -hgetmaxay(), insertmode ? "¨s∑≥ §wØ¢‘≥" : "ÆÅ∏˜ §wØ¢‘≥");
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
 –eãi ∑≥ùb‘≥¨s∑≥ §wØ¢‘≥ 2§ÈØ¢ ‘≥  F1 ï°∂ë†i ‘≥ F2 ∏·∏w ‘≥ F8 ª°∂ë ‘≥ F10 ¿aü±Œa
 *
 */
static void put_status_line(void)
{
  hputsxy(-1, -hgetmaxay(), _hangulmode ? " –eãi ∑≥ùb‘≥" : " µw¢Ö ∑≥ùb‘≥");
  hputsxy(-13, -hgetmaxay(), _insertmode ? "¨s∑≥ §wØ¢‘≥" : "ÆÅ∏˜ §wØ¢‘≥");
  hputsxy(-24, -hgetmaxay(), ishan2board() ? " 2§ÈØ¢ ‘≥" : " 3§ÈØ¢ ‘≥");
  hputsxy(-33, -hgetmaxay(), "  F1 ï°∂ë†i ‘≥");
  hputsxy(-47, -hgetmaxay(), " F2 ∏·∏w ‘≥");
  hputsxy(-58, -hgetmaxay(), " F8 ª°∂ë ‘≥");
  hputsxy(-69, -hgetmaxay(), " F10 ¿aü±Œa ");
}
/*
 *  Ø©πA ∑≥ùb∑i §hìe ¶Å¶Ö
 *
 *  —e∏Å –eãi úa∑°ßaú·ü°µA †xï°ù¢ ∏aûa—w∑i îaØ° ∏˜ü°–a°e îa∑qâ¡ à{îa
 *
 *  H : ∑≥ùb “Å ¥a¢ÅúÂ ¿·ü°üi ¥g∑eîa
 *  D : êiºa (∑≥ùb “Å êiºaàa ëe.∂©.∑© ÆÖ¨·ù° ñA¥· ∑∂ìeª° àÒ¨a
 *  N : Æï∏a ∑≥ùb “Å µ°üeΩ¢ ∏˜ùi, ∂EΩ¢ ¶Å¶ÖµAìe â∑§Ç∑i ¿Å∂ë
 *  Z : Æï∏a ∑≥ùb “Å µ°üeΩ¢ ∏˜ùi, ∂EΩ¢ ¶Å¶ÖµA 0∑i ¿Å∂ë
 *  M : ∑≥ùb§hª° ¥gâ° ãaê∑ àÂê·öÂîa...
 *  C : Æï∏a ∑≥ùb “Å µ°üeΩ¢ ∏˜ùi, ∂EΩ¢¶Å¶ÖµA â∑§Ç ¿Å∂Åâ°, 3∏aü° †aîa ÆÒŒa(,) ¨s∑≥
 *      - ãq¥Ç ŒaØ°µA ∫Åù° ≥eîa. ∏aûa «aã°ìe ÆÒŒaåaª° â°ùa–Å¥°–eîa.
 *  G : ãaü± Ãa∑© ∑≥ùb, ØaƒÅê·êa Ãa∑©µA¨· ∑≥ùb§h∑q. –aª°†e ! ¥aª¢ ª°∂• ¥eñQ.
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
        hputsxy(-24, -hgetmaxay(), " 3§ÈØ¢ ‘≥");
        hsethan390board(issplitmode());
      } else {
        hputsxy(-24, -hgetmaxay(), " 2§ÈØ¢ ‘≥");
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
      *  ãaü±∑≥ùb(ØaƒÅê· ô°ìe Ãa∑©) ¶Å¶Ö
      *  ãaü±Ãa∑©∑Å ∑°üq∑e ª°ûa Ãa∑©∑Å ∫Å«°üi ∑°∂w–eîa.
      *  òaúa¨· ãaü±∑° ói¥·àaìe Ãa∑©∑Å âw∂Å π°–s«°∑Å ¨a∂w∑e ¶âàaìw–aîa.
      */
   }
   else if(type == 'M') {
     return _lastkey;
   }
   else {  /* ãa ∑°∂A∑Å ∏aûa∑≥ùb */
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
     /* Æï∏a ∑≥ùbâ¡ â≈ùeñE –w°¢∑e ÆÅ∏˜¨w»Åù° */
     if(type == 'C' || type == 'D' || type == 'Z' || type == 'N')
       _insertmode = false;
     onoff = /*isunder(); */ isreverse();
     /* hsetunder(ON);     */ hsetreverse(ON);
     hgetdata(tmpbuf, msk);
     /* hsetunder(onoff); */ hsetreverse(onoff);
     _insertmode = tmpins;
     c = _lastkey;

     /* buffµA¨· ∏˜—w(picture)πAà· */
     pickoutdata(tmpbuf, msk, tmpbuf2);
     while(tmpbuf[0] && len > i) {
       temp[i] = tmpbuf2[i];
       i++;
     }
     _allowcanceldelline = allowcanceldelline, _restoreline = restoreline;
     _cuttail = cuttail, _allowfirstdelline = allowfirstdelline;
     if (c != ESC) { /* ∑≥ùb âiâ¡àa ¬·≠°ñAª° ¥g¥vîa°e */
        switch(type) { /* ∏aûa∂a≠°∑Å —wµA òaúa âiâ¡üi àÒ¨a */
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

/* —¡°eµA¨· ∏aûa∂a≠°üi ¨Â»Ç§hã°∂·–Å */
static int onoff_element(int el)
{
  bool onoff;
  int ch;

  data_coord(el);
  cur_col -= (strlen(hdenames[el-1])+1);
  onoff = isreverse();
  hsetreverse(ON);
  hputsxy(cur_col, cur_row, hdenames[el-1]); /* –eãi ∏aûa∂a≠° ¬âùb */
  hsetreverse(onoff);
  ch = getchext();
  hputsxy(cur_col, cur_row, hdenames[el-1]); /* –eãi ∏aûa∂a≠° ¬âùb */
  return ch;
}

/* ∏aûa∂a≠°üi ¨Â»Ç–aìeïA ≥eîaëA. */
int *get_ele_list(int ele_list[])
{
  int field_ctr, exitcode, el, line_width, width;
  int field_ptr = 0, done = FALSE;
  bool onoff;

  line_width = 0;
  onoff = isshadow();
  hsetshadow(ON);
  eputchs(-1, -BUTTON_Y, hgetmaxax(), ' ');
  hputsxy(-1, -BUTTON_Y, "  [¨a∑°õÅàÅ]-¨Â»Ç∏Â—≈   ‘˙-¨Â»Ç∏Â—≈   ‘ë-‘ê îa∑q–w°¢   F2-∑•≠·/∏·∏w   ESC-¬·≠°");
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
       case ESC : /* ¬·≠° */
         return NULL;
       case SAVEKEY : /* ¨Â»Ç µ≈ûa */
         done = TRUE;
         ele_list[field_ctr] = -1;
         break;
       case ' ' :
         if(!ele_list[field_ptr]) {
           width = headlen(el);
           if(line_width+width+1 >= PAPERWIDTH) {
             disperror("  ∑°ú˝âA ñA°e –e∫â∑° ê·¢Å ã©¥·∂a  ");
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
         eputsxy(cur_col, cur_row, ele_list[field_ptr] ? "˚" : " ");
         break;
       case '\r' :
         if(!ele_list[field_ptr]) {
           width = headlen(el);
           if(line_width+width+1 >= PAPERWIDTH) {
             disperror("  ∑°ú˝âA ñA°e –e∫â∑° ê·¢Å ã©¥·∂a  ");
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
         eputsxy(cur_col, cur_row, ele_list[field_ptr] ? "˚" : " ");
         if(field_ptr+1 == field_ctr)
           field_ptr = 0;
         else
           field_ptr++;
         break;
     }    /* switch */
  }   /* while */
  return ele_list;
}

/* ∂EΩ¢ ∏˜ùi–aâ° µ°üeΩ¢∑Å ß•ƒe∑e â∑§Ç∑aù° ¿Å∂Öîa */
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

/* ¢Ö∏aµi süi µ°üeΩ¢ ∏˜ùi(ß•ƒe∑e 0∑aù° ¿Å∂Öîa */
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

/* ¢Ö∏aµi c¥eµA â∑§Ç†e ói¥·∑∂∑a°e ¿q àt∑i ï©ùa∫Öîa */
int spaces(char *c)
{
   while (*c == ' ')
     c++;
   return !*c;
}

/* ¢Ö∏aµi sàa ëe∂©∑©ù° ñA¥·¨· àbàb∑Å àt∑° ∏‚îw–eàa  */
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
  post_notice("êiºaàa ∏i°µñA¥ˆ¥·∂a, ëe.∂©.∑© ÆÖ¨·ù° ñA¥·¥°–Å∂a");
  return DBERROR;
}

/* —¡°e∑Å °°óe œ©óaµA ∏aûa ∂a≠° àt ¬âùb */
void tally(void)
{
   int *els = elist;

   sethwindow(2, TITLE_Y+1, hgetmaxax()-1, BUTTON_Y-1);
   while (*els)
     put_field(*els++);
}

/* ∏aûa ∂a≠°üi —¡°e †aØa«aµ¡ –qçA —¡°eµA ¬âùb–eîa */
void put_field(int el)
{
   data_coord(el);
   disp_element(bf + epos(el, elist), elmask[el - 1], el);
}

/* bìe —¡°e §·Ã·µA¨· —e∏Å œ©óa∑Å ∂·√°  Õ°∑•»·, ∏aûa ∂a≠°üi —¡°eµA ¬âùb */
static void disp_element(char *b, char *msk, int el)
{
   char merge[80];
   register i = 0;

   if(eltype[el-1] == 'G') { /* ãaü± Ãa∑©∑Å âw∂Å */
     /*
      * prime_keyüi Ãa∑©∑°üq∑aù° –aìe PCXÃa∑©∑i ∑™¥·¨·
      * ∏˜–Åª• π¡Œaµ¡ «aã°ù° —¡°eµA ¬âùb–eîa.
      * òaúa¨· ∏aûa∂a≠° ∫óµA ãaü±∑° ∑∂ìe ∏aûaÃa∑©∑Å âw∂ÅµAìe
      * ∫Å«°üi 8∏a ∑°êÅù° –aìeâA πΩâVª°?
      */
     /* put_pcx(prim_key); */ /* PCXÃa∑© •°ã° */
   }
   else { /* ãa ∑°∂A∑Å âw∂Å */
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
     hputs(merge);   /* —¡°e»i∑i Õ°–q–e ∏aûa∂a≠°∑Å ¢Ö∏aµi */
   }
}


