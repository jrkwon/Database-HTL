/*
 *  –eúa 2.3∑aù° †eóaìe Õeª≥ã° –qÆÅ
 *  1993.01.01µA Ø°∏b–Å¨· 1993.03.31µA µ≈¨˜–aîa.
 */

/*
 *  Õeª≥ã° –qÆÅµA îÅ–e µaú·àaª° ¨óàbói
 *
 *  ∂Å¨Â, init_editor()¶Å¶ÖµA¨·ìe
 *  Õeª≥–i Ãa∑©∑°üqâ¡ Õeª≥«aã°µ¡ –eãi∏aÃe∑Å π∑üA ¡°ã°—¡.
 *
 *  Õeª≥µA ¨a∂wñAìe «°ói∑e °°ñÅ »·•°-Õeª≥ã°∑Å àıâ¡ ã°••∏‚∑aù° à{∑aêa
 *  ∏·∏w–aã°, ¶âú·µ°ã°, –eµw§aéÅã°, –e∏a§aéÅã° «°ów∑e ª°∏˜–i ÆÅ ∑∂ï°ù¢ –eîa.
 */

/*
 *  –eúa œaù°µA¨·ìe ã°∫Ö π¡Œaàa (1, 1)∑°ª°†e
 *  ∑° Õeª≥ã° œaù°ãaúëµA¨·ìe Õe∑Å¨w ã°∫Öπ¡Œaüi (0, 0)∑aù° –eîa.
 *  µaã°¨·, π¡Œaüi êa»aêÅìe °°óe •eÆÅìe (0, 0)∑° ã°∫Ö∑°£aù° –eúa∑Å –qÆÅüi
 *  ≥i òÅìe –w¨w π¡Œaàt∑i êa»aêÅìe •eÆÅµA 1∑i î·–Å∫Öîa.
 */

/*
 *  curx, curyìe —e∏Å —¡°eµA •°∑°ìe ¿w ¥eµA¨·∑Å ∂·√°.
 *  base_x, base_yìe ∏Â¡A Õeª≥ °A°°ü°µA¨· —e∏Å ¿w∑Å ¨wîÅ ∂·√°
 *
 *      +---------------------------------------------+
 *      |                    ^                        |
 *      |                    | base_y                 |<--- Õeª≥–aâ° ∑∂ìe Ãa∑©êÅ∂w
 *      |                    v                        |
 *      |         +------------------------+          |
 *      |         |          ^             |          |
 *      |<------->|          | cury        |<-------------- —e∏Å —¡°eµA êa»aêe ¶Å¶Ö
 *      | base_x  |          v             |          |
 *      |         |<-------->+             |          |
 *      |         |  curx                  |          |
 *      |         +------------------------+          |
 *      |                                             |
 *      +---------------------------------------------+
 *       \                                           /
 *         + .........    MAXLINEBUF   ........... +
 */

/*
 *  Õeª≥ °A°°ü°∑Å äÅπ°ìe ∏Â¡Aàa –aêa∑Å ã• ¢Ö∏aµi∑°úaâ° ¨óàb–aâ° πA∑© †aª°†bµA
 *  êÈ(NULL)¢Ö∏aüi ê˝ìeîa. ∑° êÈ∑i àaü°«°ìe Õ°∑•»·àa §aù° eofm∑°îa.
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <dos.h>
#include <mem.h>
#include <alloc.h>
#include <ctype.h>
#include <io.h>
#include <dir.h>
#include "hanlib.h"
#include "hancoord.h"
#include "hanwindw.h"
#include "ascii.h"
#include "extkey.h"
#include "hanin.h"
#include "hanerro2.h"
#include "hanedit.h"
#include "hanhanja.h"
#include "khinput.h"
#include "hanhelp.h"
/*
 *  ∑°üq∑i ª°∏˜–aª° ¥g¥v∑i òÅ∑Å Ãa∑©∑°üq
 */
#define  INIT_FILE   "UNTITLED.TXT"

/*
 * Ãa∑© ¬âùb¿wµA¨· îaûâ ÆÅ ∑∂ìe Ãa∑©∑Å ¬AîÅ ÆÅ
 */
#define  MAX_FILES   400

#define MAXLINEBUF         (160)          /* –e∫â∑Å ¬AîÅ Õeª≥àaìw ã©∑° */
#define DEFAULTBUFSIZE     (57200L)       /* ∏Â¡A Õeª≥∂wú∑∑Å ¬AîÅ«aã°  */
#define MAXBLOCKSIZE       (2000)         /* ßiú‚∑aù° îaûâ ÆÅ ∑∂ìe ¬AîÅ«aã° */

/*
 *  Õeª≥¿w∑Å «aã°ìe ¬A≠° (5 x 3), ¬AîÅ (80 x 23)
 */
#define  MAX_SCR_COL    80
#define  MAX_SCR_LINE   23
#define  MIN_SCR_COL     5
#define  MIN_SCR_LINE    3

enum yesno_t { YES_ANS, NO_ANS, CANCEL_ANS };
enum loadmode_t { INIT_LOAD, BLOCK_LOAD };

static unsigned char *pre_ptr(unsigned char *ptr, int count);
static unsigned char *next_ptr(unsigned char *ptr, int count);
static unsigned int linewidth(unsigned char *ptr);
static void writef(int sx, int ex, int y, unsigned char *msg, int mode);
static void down_line(int delta);
static void insert_line(int y);
static void delete_line(int y);
static void user_func(int kbd_check);
static unsigned int get_key(int x, int y, int *state);
static unsigned int get_key2(int indicator);
static bool insert(unsigned int size, unsigned char content);
static bool insert_ptr(unsigned int size, unsigned char content,  unsigned char *ptr);
static unsigned int line_count(unsigned char *start, unsigned char *finish);
static unsigned int block_line_count(unsigned char *start, unsigned char *finish);
static unsigned int delete(unsigned int size);
static void disp_kbd_status(void);
static void disp_coord_status(void);
static void disp_ins_status(void);
static void disp_ins_status(void);
static void disp_file_status(void);
static void disp_status(void);
static int in_block(unsigned char *ptr);
static unsigned char *one_line(unsigned char *start, int y);
static void display_page(int kbd_check);
static void hor_locate(unsigned int pos);
static void ver_locate(unsigned int line_no,unsigned char *destin);
static void to_pointer(unsigned char *destin);
static void left(void);
static int is_eol(unsigned char *ptr);
static int is_eof(unsigned char *ptr);
static void han_right(void);
static void right(void);
static void del(void);
static void english(int ch);
static void hanja(void);
static unsigned int get_ks_graphic_char(void);
static void ks_graphic_char(void);
static unsigned int get_tg_graphic_char(void);
static void tg_graphic_char(void);
static void hangul(void);
static void up_down(int delta);
static void del_eol(void);
static void ins_return(void);
static void errorf( void );
static int compare(unsigned char *st1);
static int find( void );
static int change_line(int mem_size, int buf_size, unsigned char *buf_ptr);
static void find_replace( void );
static void find_string( int repeat_last_find );
static void  find_condition(void);
static int in_word(unsigned char *ch);
static void word_left(void);
static void word_right( void );
static int file_or_dir_exist(char *fname);
static bool save_file(unsigned char *start, unsigned char *finish, unsigned char *filename);
static void block_decision( unsigned char **block_ptr);
static void block_copy( void);
static void line_after_block_move( void );
static void block_move(void);
static void block_delete( void );
static void block_write(void);
static void block_read(void);
static void restore_line( void );
static void ctrl_k(int key);
static void ctrl_q(int key);
static int get_filename_to_read(unsigned char *name);
static int get_filename_to_write( unsigned char *name);
static int ask_save(void);
static unsigned int ctrl_z_delete(unsigned index);
static unsigned int load_file(char *filename, int load_mode);
static int save(void);
static int search( char ch, char fn[][13], int start, int all);
static int get_response(char filename[][13], int count, int dir_count);
static char *get_modir(char *dir);
static int display_name(int topi, int count, char filename[][13]);
static char *split_filename(char *path);
static char *files(char *name);

/*
 *  «° ∑≥ùb ¶Å¶Öåaª° îaØ° ï©¥aµ÷∑i òÅ, ∂Å¨Â –Å¥° –i ∑©
 *  ªb, «° ∑≥ùb∑° ¥Ù∑i òÅ ¥·òÂ ∑©∑i –i àı∑•àa.
 */
#define  TASK_DISPLAYPAGE      0x0100    /* ∏Â —¡°e îaØ° ¬âùb */
#define  TASK_SCROLLUPPAGE     0x0040    /* ∏Â¡A ∂·ù° —¡°e Øa«aù© */
#define  TASK_SCROLLUP         0x0020    /* —e∏Å ƒ·¨· ¥aúÅ ¶Å¶Ö†e ∂·ù° –eƒe Øa«aù© */
#define  TASK_SCROLLDOWN       0x0010    /* —e∏Å ƒ·¨· ¥aúÅ ¶Å¶Ö†e ¥aúÅù° –eƒe Øa«aù© */
#define  TASK_DISPLAYUPLINE    0x0008    /* –e∫â ∂·ù° ∑°ï∑–Å¨· ãa ∫â∑i îaØ° ŒaØ° */
#define  TASK_DISPLAYDOWNLINE  0x0004    /* –e∫â ¥aúÅù° ∑°ï∑–Å¨· ãa ∫â∑i îaØ° ŒaØ° */
#define  TASK_DISPLAYBOTLINE   0x0002    /* †Ö ¥aúÅ∫â ŒaØ° */
#define  TASK_DISPLAYLINE      0x0001    /* —e∏Å ∫â îaØ° ŒaØ° */

extern int errno;

enum yesno_t { NO, YES };

static unsigned char *memory;                   /* Ø°∏b–i òÅ Õeª≥–i †e«q∑Å â∑àe∑i –iîw§hìeîa */
static unsigned char *line_start;               /* ¥·ìa –e ∫â∑Å Ø°∏b Õ°∑•»· */
static unsigned char *current;                  /* —e∏Å Õeª≥ ∫ó∑• ∂·√° */
static unsigned char *last_pos;                 /* §aù° ∑°∏Â∑Å ∂·√° */
static unsigned char *b_b_ptr;                  /* ßiú‚∑Å Ø°∏b Õ°∑•»· */
static unsigned char *b_k_ptr;                  /* ßiú‚∑Å è{ Õ°∑•»· */
static unsigned char *eofm;                     /* Ãa∑©∑Å †aª°†b */
static unsigned char work_file[80];             /* ∏b¥Û Ãa∑©∑Å ∑°üq */
static unsigned char block_file[80];            /* ßiú‚∑aù° ∏·∏w/∑™¥·µ°ã° Ãa∑©∑Å ∑°üq */
static unsigned char help_file[80];             /* ï°∂ë†i Ãa∑© */
static unsigned char line_buffer[MAXLINEBUF+1]; /* —e∏Å Õeª≥ ∫ó∑• ∫â∑Å ∑±Ø° ∏b¥Û ∏w≠° */

static unsigned int maxwidth, maxheight;
static long maxeditbuf;

static bool insert_mode = true, indent_mode = true, saved = true, block_display = true;
static unsigned int curx, cury, user_task, max_line, base_y, base_x, block_size;

/*
 *  îa∑q∑e Õeª≥ã°∑Å ã°ìw«°(∏·∏w, ¶âú·µ°ã°)µA îÅ–e êÅ∏˜àt∑°îa.
 *  îe, –eµw …°ãi«°ìe ã°••∏‚∑aù° SHIFT-SPACE∑°â°, ∑°∂AµA îaüe «°ï° ówù¢–i ÆÅ ∑∂îa.
 */
int save_file_key       = ALT_S;   /* Save : ∏·∏w–aã° */
int help_key            = F1;      /* ï°∂ë†i «° */
int han_toggle_key      = F2;      /* –e/µw §aéë«° */
int hanja_conv_key      = F9;      /* –eãi->–e∏a«° */
int kbd_toggle_key      = ALT_K;   /* 3§ÈØ¢/2§ÈØ¢ §aéë«° */
int ks_graph_key        = F6;      /* KS 2§a∑°Àa ãaü±¢Ö∏a ∑≥ùb */
int tg_graph_key        = F7;      /* TG 2§a∑°Àa ãaü±¢Ö∏a ∑≥ùb */
int quit_key            = ALT_X;   /* ∏·∏w–i àı∑•ª° ¢àâ°, Õeª≥∑i è{êÅã° */

static unsigned char *pre_ptr(unsigned char *ptr, int count)
{
 int line = 0;

 while(line <= count && ptr > memory)
   if(*(--ptr) == LF && *(ptr-1) == CR) line++;
 return((line <= count) ? memory : ptr+1);
}

static unsigned char *next_ptr(unsigned char *ptr, int count)
{
 int line = 0;

 while(line < count && ptr < eofm) {
   if(*ptr++ == CR && *ptr == LF) {
     ++ptr, ++line;
   }
 }
 return(ptr);
}

/*
 * —e∏Å Õ°∑•»·µA¨· ãa ∫â è{åaª°∑Å ã©∑°
 */
static unsigned int linewidth(unsigned char *ptr)
{
  unsigned int size = 0;

  while(eofm > ptr && !(*ptr++ == CR && *ptr == LF))
    ++size;
  return(size);
}

enum display_mode { REVERSE, NORMAL };

/*
 *  msg ¢Ö∏aµi∑i (sx, y)π¡ŒaµA ¬âùb–añA exåaª°∑Å êa†·ª° ¶Å¶Ö∑e â∑§Ç∑aù° ¿Å∂Öîa
 */
static void writef(int sx, int ex, int y, unsigned char *msg, int mode)
{
  bool onoff;

  if(mode == REVERSE) {
    onoff = isreverse();
    hsetreverse(ON);
  }
  hputsxy(-(sx+1), -(y+1), msg);
  sx += strlen(msg);

  while(sx++ <= ex)
    eputsxy(-(sx), -(y+1), " ");
  if(mode == REVERSE)
    hsetreverse(onoff);
}

/*
 *  delta†e«q ¥aúÅ ∫âù°
 */
static void down_line(int delta)
{
  if(base_y + cury + delta <= max_line)
    one_line(next_ptr(line_start, delta), cury + delta);
}

/*
 *  –e∫â ¨s∑≥
 */
static void insert_line(int y)
{
  hbackscrollxy(1, y+1, -maxwidth, -(maxheight-y), _CW->linespace);
}

/*
 *  –e∫â ª°∂Åã°
 */
static void delete_line(int y)
{
  hscrollxy(1, y+1, -maxwidth, -(maxheight-y), _CW->linespace);
}

/*
 *  ∏aÃe∑≥ùb∑° ¥Ù∑i òÅ –aìe ∑©...
 */
static void user_func(int kbd_check)
{
  if(user_task & TASK_DISPLAYPAGE)
    display_page(kbd_check);
  else {
    if(user_task & TASK_SCROLLUPPAGE)
      delete_line(0);
    else if(user_task & TASK_SCROLLUP)
      delete_line(cury);
    else if(user_task & TASK_SCROLLDOWN)
      insert_line(cury);
    if((user_task & TASK_DISPLAYUPLINE) && cury)
      one_line(pre_ptr(line_start, 1), cury-1);
    if(user_task & TASK_DISPLAYDOWNLINE)
      down_line(1);
    else if(user_task & TASK_DISPLAYBOTLINE)
      down_line(maxheight-cury-1);
    if(user_task & TASK_DISPLAYLINE)
      one_line(line_start, cury);
    user_task = 0;
 }
}

static unsigned int get_key(int x, int y, int *state)
{
  int bch, key;
  bool onoff;

  if(user_task && !keypressed())
    user_func(true);
  if(block_display && in_block(current)) {
    onoff = isreverse();
    hsetreverse(!onoff);
  }
  if(*current & 0x80)
    bch = (*current << 8) | *(current+1);
  else
    bch = *current;
  *state = GetHanCh(x, y, &key, bch);
  if(block_display && in_block(current)) {
    hsetreverse(onoff);
  }
  return key;
}

/*
 *  ñÅàÅ∑Å «°∑Å π°–s∑aù° ∑°ûÅ¥·ª• àıµA¨· ãa ñÅ§ÂºÅ àı∑i ∑≥ùb§hìeîa.
 *  Ctrl-KB, Ctrl-KK, ...
 */
static unsigned int get_key2(int indicator)
{
  byte far *buf;
  long bufsize;
  bool onoff;
  int key;

  bufsize = htextsize(1, 1, -3, -1);
  buf = farmalloc(bufsize);
  if(!buf) {
    memerror();
    return ESC;
  }
  hgettext(1, 1, -3, -1, buf);
  onoff = isreverse();
  hsetreverse(ON);
  hprintfxy(1, 1, "^%c ", (char)indicator);
  key = getchext();
  hprintfxy(3, 1, "%c", key);
  if(('A' <= key && key <= 'Z') || ('a' <= key && key <= 'z'))
    key &= 31;
  hputtext(1, 1, -3, -1, buf);
  hsetreverse(onoff);
  farfree(buf);
  return(key);
}

static bool insert(unsigned int size, unsigned char content)
{
  int i;

  if(maxeditbuf-(unsigned int)(eofm-memory) < size)
    return false;
  movmem(current, current+size, (unsigned int)(eofm-current+1));
  for(i = 0; i < size; i++)
    *(current+i) = content;
  if(*current == CR) {
    *(current+1) = LF,  ++max_line;
  }
  else
    user_task |= TASK_DISPLAYLINE;
  eofm += size;
  saved = false;
  if(last_pos > current)
    last_pos += size;
  if(block_size) {
    if(b_b_ptr > current)
      b_b_ptr += size;
    if(b_k_ptr > current)
      b_k_ptr += size;
    block_size = (unsigned int)(b_k_ptr - b_b_ptr);
  }
  return true;
}

static bool insert_ptr(unsigned int size, unsigned char content,  unsigned char *ptr)
{
  int i;

  if(maxeditbuf-(unsigned int)(eofm-memory) < size)
    return false;
  movmem(ptr, ptr+size, (unsigned int)(eofm-ptr+1));
  for(i = 0; i < size; i++)
    *(ptr+i) = content;
  if(*ptr == CR) {
    *(ptr+1) = LF,  ++max_line;
  }
  else
    user_task |= TASK_DISPLAYLINE;
  eofm += size;
  if(last_pos > ptr)
    last_pos += size;
  if(block_size) {
    if(b_b_ptr > ptr)
      b_b_ptr += size;
    if(b_k_ptr > ptr)
      b_k_ptr += size;
    block_size = (unsigned int)(b_k_ptr - b_b_ptr);
  }
  return true;
}

/*
 *  startµA¨· finishåaª° °y ∫â?
 */
static unsigned int line_count(unsigned char *start, unsigned char *finish)
{
  unsigned int line = 0;

  while( finish > start)
    if(*start++ == CR && *start == LF)
      ++line;
  return(line);
}

/*
 */
static unsigned int block_line_count(unsigned char *start, unsigned char *finish)
{
  if((unsigned int)(finish-start) < (unsigned int)(eofm-(finish-start)))
    return(line_count(start, finish));
  else
    return(max_line-line_count(memory, start)-line_count(finish, eofm));
}

/*
 *  —e∏Å Õ°∑•»·µA¨· size†e«q ª°∂Öîa.
 */
static unsigned int delete(unsigned int size)
{
  unsigned char *del_e;
  unsigned int line = 0;

  if(eofm == current || size == 0)
    return false;
  del_e = current + size;
  if((line = block_line_count(current, del_e)) != 0)
    max_line -= line;
  else
    user_task |= TASK_DISPLAYLINE;
  movmem(del_e, current, (unsigned int)(eofm-del_e+1));
  eofm -= size;
  saved = false;
  if(last_pos > current)
    last_pos = (last_pos > del_e) ? last_pos-size : current;
  if(block_size) {
    if(current<=b_b_ptr && b_k_ptr <= del_e) {
      b_b_ptr = b_k_ptr = current;
    }
    else {
      if(del_e<= b_b_ptr) {
        b_b_ptr -= size, b_k_ptr -= size;
      }
      else if(current <= b_b_ptr) {
        b_b_ptr=current, b_k_ptr -= size;
      }
      else if(del_e <= b_k_ptr)
        b_k_ptr -= size;
      else if(current<=b_k_ptr)
        b_k_ptr = current;
    }
    block_size = (unsigned int)(b_k_ptr - b_b_ptr);
  }
  return true;
}

/*
 *  ¨w»Å∫â
 *           1         2         3         4         5         6         7         8
 *  12345678901234567890123456789012345678901234567890123456789012345678901234567890
 *   –eãi 2§ÈØ¢ |0000∫â 000ƒe |  ¨s∑≥ |  óiµa≥aã° |  FILENAME.EXT
 */

static void disp_kbd_status(void)
{
  unsigned char kbd_msg[40];

  strcpy(kbd_msg, (HanKbdKind == HANKBD2) ? "–eãi 2§ÈØ¢" : "–eãi 3§ÈØ¢");
  writef(1, 1+strlen(kbd_msg)-1, hgetmaxax()-1, HanKbdState ? kbd_msg : "µw¢Ö  £°äÇ", REVERSE);
}

static void disp_coord_status(void)
{
  unsigned char msg[40];

  sprintf(msg, "%4u∫â %3uƒe", base_y+cury+1, base_x+curx+1);
  writef(13, 13+strlen(msg)-1, hgetmaxax()-1, msg, REVERSE);
}

static void disp_ins_status(void)
{
  writef(29, 29+strlen("¨s∑≥")-1, hgetmaxax()-1, insert_mode ? "¨s∑≥" : "ÆÅ∏˜", REVERSE);
}

static void disp_indent_status(void)
{
  writef(37, 37+strlen("óiµa≥aã°")-1, hgetmaxax()-1, indent_mode ? "óiµa≥aã°" : "êÅ¥·≥aã°", REVERSE);
}

static void disp_file_status(void)
{
  writef(49, 49+8+3+1-1, hgetmaxax()-1, work_file, REVERSE);
}

static void disp_status(void)
{
  unsigned char msg[80], kbd_msg[40];

  strcpy(kbd_msg, (HanKbdKind == HANKBD2) ? "–eãi 2§ÈØ¢" : "–eãi 3§ÈØ¢");
  sprintf(msg, " %s‘≥%4u∫â %3uƒe ‘≥ %s ‘≥ %s ‘≥ %s",
      HanKbdState ? kbd_msg : "µw¢Ö  £°äÇ",
      base_y+cury+1, base_x+curx+1,
      (insert_mode) ? "¨s∑≥" : "ÆÅ∏˜",
      (indent_mode) ? "óiµa≥aã°" : "êÅ¥·≥aã°", work_file);
  writef(0, hgetmaxax()-1, hgetmaxay()-1, msg, REVERSE);
}

static int in_block(unsigned char *ptr)
{
  return(b_b_ptr <= ptr && ptr < b_k_ptr);
}

static unsigned char *one_line(unsigned char *start, int y)
{
  unsigned char *ptr = start;
  int pos = 0, limit, block_inst, tmp= 0;
  unsigned int size;
  bool onoff;

  size = linewidth(start);
  block_inst = block_size && block_display;
  hgotoxy(1, y+1);
  if(base_x < size) {
    ptr += base_x;
    limit = (base_x+maxwidth-1 >= size) ? size-base_x : maxwidth;
    for( ;pos < limit; ++pos) {
      if(block_inst && b_b_ptr <= ptr && ptr < b_k_ptr) {
        onoff = isreverse();
        hsetreverse(ON);
        if(*ptr == '\t') {
          *ptr = ' ';
          insert_ptr(_CW->tabsize-((pos) % _CW->tabsize)-1, ' ', ptr);
          limit += _CW->tabsize-((pos) % _CW->tabsize);
          tmp += _CW->tabsize-((pos) % _CW->tabsize)-1;
        }
        else {
          if(*ptr & 0x80) {
            _hputchxy(*ptr, *(ptr+1), hwherex(), hwherey());
            hgotoxy(hwherex()+2, hwherey());
            ptr += 2;
            pos++;
          }
          else {
            if(is_eol(ptr)) {
              pos++;
              ptr += 2;
            }
            else {
              _eputchxy(*ptr++, hwherex(), hwherey());
              hgotoxy(hwherex()+1, hwherey());
            }
          }
        }
        hsetreverse(onoff);
      }
      else {
        if(*ptr == '\t') {
          *ptr = ' ';
          insert_ptr(_CW->tabsize-((pos) % _CW->tabsize)-1, ' ', ptr);
          limit += _CW->tabsize-((pos) % _CW->tabsize);
          tmp += _CW->tabsize-((pos) % _CW->tabsize)-1;
        }
        else {
          if(*ptr & 0x80) {
            _hputchxy(*ptr, *(ptr+1), hwherex(), hwherey());
            hgotoxy(hwherex()+2, hwherey());
            ptr += 2;
            pos++;
          }
          else {
            if(is_eol(ptr)) {
              ptr += 2; pos++;
            }
            else {
              _eputchxy(*ptr++, hwherex(), hwherey());
              hgotoxy(hwherex()+1, hwherey());
            }
          }
        }
      }
    }
  }
  while(pos++ < maxwidth) {
    if(block_inst && in_block(start+size)) {
      onoff = isreverse();
      hsetreverse(ON);
      _eputch(' ');
      hsetreverse(onoff);
    }
    else {
      _eputch(' ');
    }
  }
  start += (size+2)+tmp;
  hputch(-1);  /* hputch buffer clear */
  return start;
}

static void display_page(int kbd_check)
{
  unsigned char *ptr;
  int y, last;

  user_task = TASK_DISPLAYPAGE;
  one_line(line_start, cury);
  if(base_y+maxheight-2 < max_line)
    last = maxheight-1;
  else {
    last = max_line - base_y;
    hclrscrxy(1, last+1, -maxwidth, -(maxheight-last));
  }
  ptr = pre_ptr(line_start, cury);
  for(y = 0; y <= last; y++) {
    if(kbd_check && keypressed())
      return;
    ptr = one_line(ptr, y);
  }
  user_task = 0;
}

static void hor_locate(unsigned int pos)
{
  if (pos < base_x || base_x+maxwidth-1 <= pos) {
    curx = (pos < base_x) ? 0 : maxwidth-2;
    base_x = pos - curx;
    user_task = TASK_DISPLAYPAGE;
  }
  else curx = pos - base_x;
  last_pos = current;
  current = line_start + pos;
  disp_coord_status();
}

static void ver_locate(unsigned int line_no,unsigned char *destin)
{
  if(line_no < base_y || base_y+maxheight-2 < line_no) {
    if(cury == 0 && line_no+1 == base_y) {
      user_task = TASK_DISPLAYPAGE;
    }
    else if(cury == maxheight-2 && base_y+maxheight-1 == line_no) {
      user_task = TASK_DISPLAYPAGE;
    }
    else
      user_task = TASK_DISPLAYPAGE;
    if(line_no < cury && line_no < base_y)
      cury = line_no;
    base_y = line_no - cury;
  }
  else
    cury = line_no - base_y;
  strncpy(line_buffer, line_start, MAXLINEBUF);
  hor_locate((unsigned int)(destin-line_start));
}

static void to_pointer(unsigned char *destin)
{
  unsigned int line_no;

  line_no = base_y + cury;
  if(destin <= current) {
    if((unsigned int)(destin-memory) < (unsigned int)(current-destin))
      line_no = line_count(memory, destin);
    else
      line_no -= line_count(destin, current);
  }
  else {
    if((unsigned int)(destin-current) < (unsigned int)(eofm-destin))
      line_no += line_count(current, destin);
    else
      line_no = max_line - line_count(destin, eofm);
  }
  line_start = pre_ptr(destin, 0);
  ver_locate(line_no,destin);
}

static void left(void)
{
  if(memory < current) {
    if(current == line_start)
      to_pointer(current-2);
    else {
      if(ishangul2nd(line_start, curx+base_x-1))
        hor_locate(base_x+curx-2);
      else
        hor_locate(base_x+curx-1);
    }
  }
}

static int is_eol(unsigned char *ptr)
{
  return (*ptr==CR && *(ptr+1)==LF);
}

static int is_eof(unsigned char *ptr)
{
  return (*ptr == 0x1a || *ptr == 0 || ptr == eofm);
}

static void han_right(void)
{
  if(current < eofm) {
    hor_locate(base_x+curx+2);
  }
}

static void right(void)
{
  if(current < eofm) {
    if(is_eol(current))
      to_pointer(current+2);
    else {
      if(ishangul1st(line_start, curx+base_x))
        han_right();
      else
        hor_locate(base_x+curx+1);
    }
  }
}

static void del(void)
{
  if(is_eol(current)) {
    if(delete(2))
      user_task |= TASK_SCROLLUP+TASK_DISPLAYLINE+TASK_DISPLAYBOTLINE;
  }
  else {
    if(ishangul1st(line_start, curx+base_x))
      delete(2);
    else
      delete(1);
  }
}

static void english(int ch)
{
  if(insert_mode || is_eol(current) || is_eof(current)) {
    if(insert(1, ' ') == false)
      return;
  }
  if(eofm >= current) {
    if(ishangul1st(line_start, curx+base_x))
      *(current+1) = ' ';
    *current = (unsigned char)ch;
    user_task |= TASK_DISPLAYLINE;
    saved = false;
    right();
  }
}

static void hanja(void)
{
  unsigned int hangulcode, hanjacode;

  if(isalnum(*current) || is_eol(current) || is_eof(current))
    return;
  if(eofm >= current) {
    if(ishangul1st(line_start, curx+base_x)) {
      hangulcode = ((*current) << 8) + *(current+1);
      hanjacode = findhanja(hgetmaxax()/2 - 14, 8, *current, *(current+1));
      if(hangulcode != hanjacode) {  /* –e∏aàa ¨Â»Ç ñA¥ˆ∑q */
        *current = (hanjacode & 0xFF00) >> 8;
        *(current+1) = hanjacode & 0x00FF;
        user_task |= TASK_DISPLAYLINE;
        saved = false;
        han_right();
      }
    }
  }
}

/*
 *  KS Graphic Character
 *
 *  blank is 217:49
 *
 *  217:49 ~ 126, 145 ~ 160, blank, blank, 161 ~ 254, blank
 *  218:49 ~ 126, 145 ~ 160, blank, blank, 161 ~ 254, blank
 *  ....
 *  ....
 *  222:49 ~ 126, 145 ~ 160, blank, blank, 161 ~ 254, blank
 */
static unsigned int get_ks_graphic_char(void)
{
  int x, y, isquit, flag;
  int blank[] = { 217, 49 };
  unsigned int key, int_bcode, ans;
  unsigned char bcode1, bcode2;
  static int bx = 0, by = 0;
  static unsigned int code1 = 217, code2;
  bool onoff;

  isquit = ans = 0;
  flag = 1;

  if(wopen(hgetmaxax()/2-16, hgetmaxay()/2-4, 32, 12))
    wtitle("Ÿ√ Ÿﬁ ãaü± ¢Ö∏a ∑≥ùb Ÿ√");
  else
    return 0;
  do {
    ans = 0;
    if( flag) {
      code2 = 49;
      _hputchxy(blank[0], blank[1], 1, 1);

      for(y = 0; y < 12; y++) {
        for(x = 0; x < 32; x+=2, code2++) {
          if(x == 0 && y == 0)
            x = 2;
          if(code2 == 127)
            code2 = 145;
          else if(code2 == 161) {
            _hputchxy(blank[0], blank[1], x+1, y+1);
            x+=2;
            if(x == 32)
              x = 0, y++;
            _hputchxy(blank[0], blank[1], x+1, y+1);
            x+=2;
            if(x == 32)
              x = 0, y++;
          }
          else if(code2 == 255) {
            _hputchxy(blank[0], blank[1], x+1, y+1);
            break;
          }
          _hputchxy(code1, code2, x+1, y+1);
        }
      }
    }
    if((bx == 0 && by == 0) || (bx == 30 && by == 11)
        || (bx == 30 && by == 5) || (bx == 0 && by == 6))
      bcode1 = blank[0], bcode2 = blank[1];
    else {
      bcode1 = code1, int_bcode = bx/2 + by*16 + 48;
      int_bcode = (int_bcode >= 127) ? int_bcode + 18 : int_bcode;
      bcode2 = (int_bcode > 160) ? int_bcode - 2 : int_bcode;
    }
    onoff = isreverse();
    if(!onoff)
      hsetreverse(ON);
    _hputchxy(bcode1, bcode2, bx+1, by+1);
    key = getchext();
    hsetreverse(onoff);
    _hputchxy(bcode1, bcode2, bx+1, by+1);
    flag = 0;
    switch(key) {
      case LEFTARROW :
        if(bx == 0) {
          if(by > 0)
            by--, bx = 30;
        }
        else
          bx-=2;
        break;
      case RIGHTARROW :
        if(bx == 30) {
          if(by < 11)
            by++, bx = 0;
        }
        else
          bx+=2;
        break;
      case UPARROW :
        if(by > 0)
          by--;
        break;
      case DOWNARROW :
        if(by < 11)
          by++;
        break;
      case PGUP :
        if(code1 > 217)
          code1--;
        else
          code1 = 222;
        flag = 1;
        break;
      case PGDN :
        if(code1 < 222)
          code1++;
        else
          code1 = 217;
        flag = 1;
        break;
      case ESC :
        isquit = 1;
        break;
      case '\r' :
        ans = ((bcode1 << 8) & 0xFF00) + (bcode2);
        isquit = 1;
        break;
    }
  } while (!isquit);
  wclose();
  return ans;
}

static void ks_graphic_char(void)
{
  unsigned int ans, flag;

  flag = 0;
  if(insert_mode || is_eol(current) || is_eof(current)) {
    if(insert(2, ' ') == false)
      return;
    flag = 1;
  }
  if(eofm >= current) {
    if((ans = get_ks_graphic_char()) != 0) {
      if(ishangul1st(line_start, curx+base_x+1))
        *(current+2) = ' ';
      *current = (ans & 0xFF00) >> 8;
      *(current+1) = ans & 0x00FF;
      user_task |= TASK_DISPLAYLINE;
      saved = false;
      han_right();
    }
    else if(flag) {
      delete(2);
    }
  }
}

/*
 *  TG Graphic Character
 *
 *  212:128 ~ 212:255
 */
static unsigned int get_tg_graphic_char(void)
{
  int x, y, isquit;
  unsigned int key, ans;
  int code1, code2;
  static int bx = 0, by = 0;
  static unsigned int bcode1 = 212, bcode2 = 218;
  bool onoff;

  ans = isquit = 0;

  if(wopen(hgetmaxax()/2-16, hgetmaxay()/2-4, 32, 8))
    wtitle("Ÿ√ ‘Ø ãaü± ¢Ö∏a ∑≥ùb Ÿ√");
  else
    return 0;

  code1 = 212, code2 = 128;
  for(y = 0; y < 8; y++) {
    for(x = 0; x < 32; x+=2, code2++) {
      _hputchxy(code1, code2, x+1, y+1);
    }
  }

  do {
    bcode1 = code1, bcode2 = bx/2 + by*16 + 128;
    onoff = isreverse();
    if(!onoff)
      hsetreverse(ON);
    _hputchxy(bcode1, bcode2, bx+1, by+1);
    key = getchext();
    hsetreverse(onoff);
    _hputchxy(bcode1, bcode2, bx+1, by+1);
    switch(key) {
      case LEFTARROW :
        if(bx == 0) {
          if(by > 0)
            by--, bx = 30;
        }
        else
          bx-=2;
        break;
      case RIGHTARROW :
        if(bx == 30) {
          if(by < 7)
            by++, bx = 0;
        }
        else
          bx+=2;
        break;
      case UPARROW :
        if(by > 0)
          by--;
        break;
      case DOWNARROW :
        if(by < 7)
          by++;
        break;
      case ESC :
        isquit = 1;
        break;
      case '\r' :
        ans = ((bcode1 << 8) & 0xFF00) + (bcode2);
        isquit = 1;
        break;
    }
  } while(!isquit);
  wclose();
  return ans;
}

static void tg_graphic_char(void)
{
  unsigned int ans, flag;

  flag = 0;
  if(insert_mode || is_eol(current) || is_eof(current)) {
    if(insert(2, ' ') == false)
      return;
    flag = 1;
  }
  if(eofm >= current) {
    if((ans = get_tg_graphic_char()) != 0) {
      if(ishangul1st(line_start, curx+base_x+1))
        *(current+2) = ' ';
      *current = (ans & 0xFF00) >> 8;
      *(current+1) = ans & 0x00FF;
      user_task |= TASK_DISPLAYLINE;
      saved = false;
      han_right();
    }
    else if(flag) {
      delete(2);
    }
  }
}

static void hangul(void)
{
  if(insert_mode || is_eol(current) || is_eof(current)) {
    if(insert(2, ' ') == false)
      return;
  }
  if(eofm >= current) {
    if(ishangul1st(line_start, curx+base_x+1)) {
      *(current+2) = ' ';
      user_task |= TASK_DISPLAYLINE;
    }
    saved = false;
  }
}

static void up_down(int delta)
{
  unsigned char *destin;
  unsigned int line_no, size;

  if(delta <= 0) {
    delta = -delta;
    line_no = (delta > base_y+cury) ? 0 : base_y+cury-delta;
    line_start = pre_ptr(line_start, delta);
  }
  else {
    line_no = base_y+cury;
    if(delta > max_line-line_no)
      delta = max_line-line_no;
    line_no += delta;
    line_start = next_ptr(line_start, delta);
  }
  size = linewidth(line_start);
  destin = line_start + ((base_x+curx > size) ? size : base_x+curx);
  if(ishangul2nd(line_start, (base_x+curx > size) ? size : base_x+curx))
    destin--;
  ver_locate(line_no,destin);
}

static void del_eol(void)
{
  delete(linewidth(current));
}

static void ins_return(void)
{
  unsigned int pos = 0;

  if(indent_mode) {
    pos = strspn(line_start, " ");
    if(pos >= base_x+curx)
      pos = base_x;
  }
  if(insert(pos+2, CR)) {
    user_task |= TASK_SCROLLDOWN+TASK_DISPLAYLINE+TASK_DISPLAYUPLINE;
    memset(current+2, ' ', pos);
    to_pointer(current+pos+2);
  }
}

static void errorf( void )
{
  static unsigned char *errmsg[] = {
    "Ãa∑©∑° ê·¢Å ƒ·∂a",
    "ãaúÂ ¢Ö∏aµi∑° ¥ÙØsì°îa",
    "Ãa∑©∑° ∑°£° π•∏Å–aà·êa ∏i°µñE ∑°üq∑≥ì°îa",
    "î· ∑°¨w ¥ÙØsì°îa"
  };

  disperror((errno>=36) ? errmsg[errno-36] : hsys_errlist[errno]);
  errno = 0;
}

/*
 *  ¿xã° ¶Å¶Ö
 */
static unsigned char first_key = 0xFF, keys[41], object[41], option[41];
static unsigned char replace_str[41], sense = 1, f_count = 1, word_f = NO;
static unsigned char replace = NO, len = 0;

static int compare(unsigned char *st1)
{
  int count = 1, same = YES;
  unsigned char *st2 = object, *key = keys;

  while( count++ < len )
    if( (*++st1 - *++st2) & *++key ) {
      same = NO; break;
    }
  return (same);
}

static int find( void )
{
  unsigned char *pt = current;
  int found = 0, left, right, success;
  unsigned int line = base_y+cury;

  do {
    success = NO;
      while(*pt) {
        if(*pt == LF)
          line += sense;
        else if( !((*pt - *object)&first_key ) && compare(pt) ) {
          left = right = YES;
        if( word_f ) {
          left = (*(pt-1) <= ' ' );
          right = (*(pt+len ) <= ' ');
        }
        if(left && right) {
          success = YES;
          ++found;
          break;
        }
      }
      pt+=sense;
    }
    if( success == NO || found == f_count )
      break;
    pt+=sense;
  } while( YES );
  success = (found== f_count);
  if(success) {
    line_start = pre_ptr( pt ,0 );
    ver_locate( line , pt);
  }
  else {
    errno = 37;
    errorf();
  }
  return (success);
}

static int change_line(int mem_size, int buf_size, unsigned char *buf_ptr)
{
  int success;

  success = ( buf_size < mem_size ) ? delete( (unsigned int) (mem_size-buf_size) ) :
            insert( (unsigned int) (buf_size-mem_size) , ' ');
  if(success)
    strncpy(current, buf_ptr, buf_size);
  return ( success );
}

static void find_replace( void )
{
  #define  ASK         1
  #define  NOT_ASK     2
  int repeat , key = 0 , replace_len , delta, mode;
  bool onoff;

  if(strchr(option, 'N'))
    replace = NOT_ASK;
  if(strchr(option, 'G')) {
    sense = 1;
    f_count = 1;
    repeat = YES;
    to_pointer(memory);
    user_task = TASK_DISPLAYPAGE;
  }
  else
    repeat = NO;
  replace_len = strlen(replace_str);
  while(find()) {
    switch( replace ) {
      case ASK :
        delta = len ;
        writef(0, 16, hgetmaxay()-1, "[§aéâåa∂a? (Y/N)]", NORMAL);
        mode = HanKbdState, HanKbdState = false;
        onoff = isreverse();
        hsetreverse(ON);
        hprintfxy(curx, cury, "%s", key);
        hsetreverse(onoff);
        key = toupper(getchext());
        HanKbdState = mode;
        disp_status();
        if( key != 'Y' )
          break;
      case NOT_ASK :
        delta = replace_len;
        if(change_line(len, replace_len, replace_str) == NO)
          repeat = NO;
        else if(replace == ASK)
          user_func(YES);
    }
    if(keypressed()) key = getchext();
    if(repeat==NO || key==ESC) break;
    to_pointer(current+delta);
  }
}

static void find_string( int repeat_last_find )
{
  if(*object) {
    if( sense == 1 ) {
      if( repeat_last_find)
        right();
    }
    else if( repeat_last_find || current == eofm)
      left();
    if(replace)
      find_replace();
    else
      find();
  }
}

static void  find_condition(void)
{
  unsigned char *op; int i;

  if(win_hgets(40, "¿x∑i ¢Ö∏aµi∑e ?", object))  {
    if(replace && !win_hgets(40, "§aéâ ¢Ö∏aµi∑e ?", replace_str))
      return;
    strcuttail(object);
    strcuttail(replace_str);
    if(win_hgetdata(40, "¨Â»Ç¨a–w (G B U W N Æï∏a)", "EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE", option)) {
      strcuttail(option);
      strupr( option );
      if(strchr( option, 'U') ) {
        first_key = 0xFF;
        strupr( object );
      }
      else
        first_key = 0xFF;
      len = (unsigned char)strlen(object);
      for(i = 0; i < len; i++ )
        keys[i] = (object[i] == 1) ? 0 : first_key;
      first_key = keys[0];
      f_count = 1;
      for(op = option; *op; ++op)
        if( '1' <= *op && *op <='9')
           f_count = *op -'0';
      sense = (strchr(option, 'B')) ? -1 :1 ;
      word_f = (strchr(option , 'W')) ? YES :NO;
      find_string(NO);
    }
  }
}

static int in_word(unsigned char *ch)
{
  return (strchr(" \r\n<>,:.()[]^'*+-/$[", *ch) == NULL);
}

static void word_left(void)
{
  unsigned char *ptr =  current;

  while(memory < ptr && !in_word(ptr-1))
    --ptr;
  while(memory < ptr &&  in_word(ptr-1))
    --ptr;
  to_pointer(ptr);
}

static void word_right( void )
{
  unsigned char *ptr = current;

  while(eofm > ptr && in_word(ptr))
    ++ptr;
  while(eofm > ptr && !in_word(ptr))
    ++ptr;
  to_pointer(ptr);
}

static int file_or_dir_exist(char *fname)
{
  return (access(fname, 0) == 0);
}

static bool save_file(unsigned char *start, unsigned char *finish, unsigned char *filename)
{
  unsigned char back_up_name[80];
  unsigned char *f_n_pt = filename, *b_n_pt = back_up_name;
  int handle;
  bool success = true;

  while( *f_n_pt && *f_n_pt !=  '.')
    *b_n_pt++ = *f_n_pt++;

  strcpy(b_n_pt , ".BAK");
  if(file_or_dir_exist(filename)) {
    if(file_or_dir_exist(back_up_name)) {
      if(strcmp(filename, back_up_name) == 0) {
        unlink( back_up_name);
      }
      rename(filename, back_up_name);
    }
  }
  errno = 0;
  if((handle = creat(filename, 0x0180)) != -1) {
    /* ∂wú∑∑° ¶Åπ¢–a°e ¥·ò‚–aª°.. */
    _write(handle, start, (unsigned int)(finish-start));
    close(handle);
  }
  if(errno) {
    errorf();
    success = false;
  }
  return success;
}

static void block_decision( unsigned char **block_ptr)
{
  *block_ptr = current;
  block_size = (b_b_ptr >= b_k_ptr) ? 0 : (unsigned int)( b_k_ptr-b_b_ptr);
  block_display = YES;
  user_task = TASK_DISPLAYPAGE;
}

static void block_copy( void)
{
  if( !in_block( current ) && insert( block_size,' ') ) {
    max_line += block_line_count( b_b_ptr,b_k_ptr) ;
    movmem( b_b_ptr, current , block_size);
    b_b_ptr = current;
    b_k_ptr = b_b_ptr+ block_size;
    user_task = TASK_DISPLAYPAGE;
  }
}

static void line_after_block_move( void )
{
  unsigned char *homep;
  homep = pre_ptr( line_start, cury );
  if( homep <= b_b_ptr )
    cury  -=  line_count( b_b_ptr , b_k_ptr );
  else if ( homep <= b_k_ptr ) {
    base_y  -= line_count( b_b_ptr , homep );
    cury = line_count( b_k_ptr , current);
  }
  else base_y -= line_count(b_b_ptr , b_k_ptr);
  saved = NO;
}

static void block_move(void)
{
  unsigned char buffer[2001], *start, *mid, *endp;
  unsigned int count, len, size;

  if( in_block( current ) ) return ;
  if( b_k_ptr <= current ) {
    start =  b_b_ptr; mid = b_k_ptr; endp =current;
    line_after_block_move();
    b_b_ptr += (current - b_k_ptr);
  }
  else {
    start = current;
    mid = b_b_ptr;
    endp = b_k_ptr;
    b_b_ptr = current;
  }
  len = ( unsigned int)(endp - start );
  count = (unsigned int) (mid - start );
  while( count > 0) {
    size = ( count >= 2000 )? 2000 :count ;
    count -= size;
    strncpy( buffer, start, size );
    movmem(start + size, start, len - size );
    strncpy(endp - size, buffer, size );
  }
  b_k_ptr = b_b_ptr + block_size;
  line_start = pre_ptr( b_b_ptr, 0);
  hor_locate((unsigned int)(b_b_ptr-line_start));
  user_task = TASK_DISPLAYPAGE;
  saved = NO;
}

static void block_delete( void )
{
  unsigned char *temp;

  if( in_block( current ) )
    to_pointer( b_b_ptr );
  else if( b_k_ptr  <= current ) {
    line_after_block_move();
    current -= block_size;
  }
  temp = current;
  current = b_b_ptr;
  delete( block_size );
  current = temp ;
  user_task = TASK_DISPLAYPAGE;
  line_start = pre_ptr( current , 0 );
  hor_locate( ( unsigned int) ( current - line_start) );
}

static void block_write(void)
{
  char temp_name[80];

  if(b_k_ptr > b_b_ptr) {
    strcpy(temp_name, block_file);
    if(get_filename_to_write(temp_name) != CANCEL_ANS) {
      if(files(temp_name) != NULL) {
        save_file(b_b_ptr, b_k_ptr, temp_name);
      }
      strcpy(block_file, temp_name);
    }
  }
}

static void block_read(void)
{
  char temp_name[80];
  char *temp_b_ptr = b_b_ptr;
  unsigned temp_size = block_size;

  strcpy(temp_name, block_file);
  if(get_filename_to_read(temp_name) == YES_ANS) {
    if(files(temp_name) != NULL) {
      b_b_ptr = current;
      block_size = load_file(temp_name, BLOCK_LOAD);
      if(block_size) {
        b_k_ptr = current+block_size;
        user_task = TASK_DISPLAYPAGE;
        block_display = YES;
        saved = NO;
      }
      else {
        b_b_ptr = temp_b_ptr;
        block_size = temp_size;
      }
    }
    strcpy(block_file, temp_name);
  }
}

static void restore_line( void )
{
  unsigned char *pt = line_buffer;
  unsigned int buf_size = 0;

  hor_locate(0);
  while(buf_size < MAXLINEBUF && *pt && !is_eol(pt)) {
    ++buf_size;
    ++pt;
  }
  change_line(linewidth(current), buf_size, line_buffer);
}

static void ctrl_k(int key)
{
  if(strchr("CVMWY", (unsigned char) key +'@' ) \
      && ( block_size ==0 || block_display == NO) )
    return;
  switch( key ) {
    case  2:    /* B */
      block_decision( &b_b_ptr);
      break;
    case 11:    /* K */
      block_decision( &b_k_ptr );
      break;
    case 8 :    /* H */
      block_display = !block_display;
      user_task = TASK_DISPLAYPAGE;
      break;
    case 3 :    /* C */
      block_copy();
      break;
    case 22 :   /* V */
      block_move();
      break;
    case 25 :   /* Y */
      block_delete();
      break;
    case 18 :   /* R */
      block_read();
      break;
    case 23 :   /* W */
      block_write();
      break;
  }
}

static void ctrl_q(int key)
{
  switch( key ) {
    case 2 :    /* B */
      to_pointer(b_b_ptr);
      break;
    case 11:    /* K */
      to_pointer(b_k_ptr);
      break;
    case 16:    /* P */
      to_pointer(last_pos);
      break;
    case 12:    /* L */
      restore_line();
      break;
    case 6:     /* F */
      replace = NO;
      find_condition();
      break;
    case 1:     /* A */
      replace = YES;
      find_condition();
      break;
    case 9:     /* I */
      indent_mode = !(indent_mode);
      disp_indent_status();
      break;
    case 25:    /* Y */
      del_eol();
      break;
  }
}

static int get_filename_to_read(unsigned char *name)
{
  if(win_hgetdata(40, " ∑™¥·ói∑© Ãa∑©∑°üq∑e ?", "EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE", name)) {
    strcuttail(name);
    strupr(name);
    return YES_ANS;
  }
  return CANCEL_ANS;
}

static int get_filename_to_write( unsigned char *name)
{
  if(win_hgetdata(40, " ∏·∏w–i  —¡∑©∑°üq∑e ?", "EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE", name)) {
    strcuttail(name);
    strupr(name);
    return YES_ANS;
  }
  return CANCEL_ANS;
}

/*
 *  ∏·∏w–i àı∑•ª° —¬∑•
 *  µÅ : 0, ¥aì°µ° : 1, ¬·≠° : 2
 */
static int ask_save(void)
{
  unsigned char msg[40];
  int ans;

  if(!saved && memory!=eofm ) {
    sprintf(msg,"%süi ∏·∏w–iåa∂a?",split_filename(work_file));
    ans = yesno(msg, YES_ANS);
    if(ans == YES_ANS)
      save();
  }
  return ans;
}

static unsigned int ctrl_z_delete(unsigned index)
{
  if((*(current+index-1)) == 0x1a) {
    movmem(current+index, current+index-1, (size_t )(eofm-current+1));
    index -= 1;
  }
  return index;
}

static unsigned int load_file(char *filename, int load_mode)
{
  int handle;
  long     l_size;
  unsigned size;

  size = 0;
  errno = 0;
  user_task = TASK_DISPLAYPAGE;

  if((handle = _open(filename, 0x8001)) != -1) {
    l_size =filelength(handle);
    if(load_mode == INIT_LOAD) {
      if(ask_save() != CANCEL_ANS) {
        if(l_size > maxeditbuf) {
          if(yesno("°A°°ü°àa ¶Åπ¢–sì°îa. ñı¶Å¶Ö∑i ∏aüiåa∂a?", 0) == YES_ANS)
            l_size = maxeditbuf;
          else
            l_size = -1L;
        }
        /* init_editor(filename, maxeditbuf, HanKbdState); */
      }
      else
        l_size = -1L;
    }
    else {
      if(l_size > (maxeditbuf-(long)(eofm-memory))) {
        memerror();
        l_size = -1L;
      }
    }
    if(l_size != -1L) {
      size = (size_t)(l_size);
      movmem(current, current+size, (size_t)(eofm-current)+1);
      if(_read(handle, current, size)== -1)
        movmem(current+size, current,(size_t)(eofm-current)+1);
      else {
        size = ctrl_z_delete(size);
        max_line += line_count(current, current+size);
        eofm += size;
        strncpy(line_buffer, line_start, MAXLINEBUF);
      }
    }
    _close(handle);
  }
  else
    errno = 38; /* file not found */
  if(errno)
    errorf();
  return size;
}

static int save(void)
{
  char temp_name[80];

  if(memory != eofm) {
    if(!strcmp(work_file, INIT_FILE)) {
      strcpy(temp_name, INIT_FILE);
      if(get_filename_to_write(temp_name) == YES_ANS)
        strcpy(work_file, temp_name);
/*      disp_file_status();*/
    }
    if(save_file(memory, eofm, work_file)) {
      saved = YES;
      return YES;
    }
    else
      return NO;
  }
  else
    return YES; /* length 0 error */
}

static int search( char ch, char fn[][13], int start, int all)
{
  int i = start+1;

  if(i < all) {
    while( i < all) {
      if((ch == fn[i][0]))
        return i;
      else
        i++;
    }
  }
  else
    start = all;
  i = 0;
  while(i < start) {
    if(ch == fn[i][0])
      return i;
    else
      i++;
  }
  return -1;
}

static int get_response(char filename[][13], int count, int dir_count)
{
  unsigned   ch;
  int   select_done;
  int i, topi, last, temp;
  unsigned ox, oy, barx, bary;
  bool onoff;

  select_done = i = topi = 0, barx = bary = 1;
  last = display_name(0, count, filename);
  while(!select_done) {
    ox = barx, oy = bary;
    onoff = isreverse();
    if(!onoff)
      hsetreverse(ON);
    hprintfxy(ox, oy, " %-12s ", filename[topi+i]);
    hsetreverse(onoff);
    ch = getchext();
    hprintfxy(ox, oy, " %-12s ", filename[topi+i]);
    switch(ch) {
      case LEFTARROW :
        i--;
        if(i < 0) {
          if(topi-4 <0 ) {
            i = 0;
          }
          else {
            topi -= 4;
            i = 3;
            last = display_name(topi, count, filename);
          }
        }
        break;
      case RIGHTARROW :
        i++;
        if(i > last) {
          if(last == 35) {
            if(topi+36 < count) {
              topi += 4;
              i = 32;
              last = display_name(topi, count, filename);
            }
            else {
              i = last;
            }
          }
          else
            i = last;
        }
        break;
      case  UPARROW :
        if(i-4 < 0) {
          if(topi-4 >= 0) {
            topi -= 4;
            last = display_name(topi, count, filename);
          }
        }
        else
          i -=4;
        break;
      case  DOWNARROW :
        if(i+4 > last) {
          if(topi+36 <count) {
            topi +=4;
            last = display_name(topi, count, filename);
            if(i>last) i-=4;
          }
        }
        else
          i += 4;
        break;
      case  HOMEKEY :
        if(topi != 0) {
          topi=0;
          last = display_name(topi, count, filename);
        }
        i = 0;
        break;
      case  ENDKEY  :
        if(topi+36 < count) {
          i = 31+(count & 0x03);
          topi = count-(i+1);
          last = display_name(topi, count, filename);
        }
        else
          i = last;
        break;
      case  PGUP :
        if(topi != 0) {
          topi -= 36;
          if(topi < 0)
            topi = 0;
          last = display_name(topi, count, filename);
        }
        else
          i = i & 0x03;
        break;
      case  PGDN :
        temp = topi+36;
        if(temp < count) {
          if((temp+(i&0x03)) < count)
            topi = temp;
          else
            topi += 32;
          last = display_name(topi, count, filename);
          if(i > last)
          i = last-((last-i)&0x03);
        }
        else
          i = last-((last-i)&0x03);
        break;
      case  CR   :
        select_done = 1;
        break;
      case  ESC  :
        return -1;
      default    :
        temp = search(toupper(ch), filename, topi+i, count-dir_count);
        if(temp >= 0) {
          if(topi <= temp && topi+36 > temp)
            i = (temp-topi)%36;
          else {
            topi = temp & 0xfffc;
            i = temp & 0x0003;
            last = display_name(topi, count, filename);
          }
        }
    }
    barx = (i&0x0003)*15+1;
    bary = (i/4) +1;
  }/* while */
  return topi+i;
}

/*
 *  °° ó°ùB…°ü°(..)üi äÅ–eîa.
 */
static char *get_modir(char *dir)
{
  int index;

  if(*dir) {
    index=(strlen(dir))-2;
    while(index >= 0 && dir[index] != '\\' && dir[index] != ':')
      index--;
    dir[index+1] = NULL;
  }
  return dir;
}

static int display_name(int topi, int count, char filename[][13])
{
  unsigned index, last, x, y;

  last = (count-topi) > 35 ? 35 : count-topi-1;

  for(index = 0; index < 36; index++) {
    x = (index & 0x0003)*15+1;
    y = (index / 4) +1;
    if(index > last)
      hprintfxy(x, y, " %-12s ", " ");
    else
      hprintfxy(x, y, " %-12s ", filename[topi+index]);
  }
  return last;
}

static char *split_filename(char *path)
{
  int index;

  if(*path) {
    index = (strlen(path))-1;
    while(index >= 0 && path[index] != '\\' && path[index] != ':')
      index--;
    return path+index+1;
  }
  return NULL;
}

char temp_file[66];

static char *files(char *name)
{
  struct ffblk ffblk;
  int  index, dir_count, count, response, select_done = NO;
  char path_name[66], search_name[13], drive[3], dir[66], file[9], ext[5];
  char filename[MAX_FILES][13], otherdir[20][13];

  while(*name) {
    if((*name)==' ')
      name++;
    else
      break;
  }
  fnsplit(name, drive, dir, file, ext);
  if((*drive) == NULL)
    sprintf(drive, "%c:", getdisk()+'A');
  if((*dir) == NULL) {
    if(getcurdir(drive[0]-'A'+1, path_name) == -1) {
      errorf();
      return NULL;
    }
    sprintf(dir, "\\%s%c", path_name, (*path_name) ? '\\' : '' );
  }
  else
    strcat(dir, (dir[strlen(dir)-1]=='\\') ? "" : "\\");
  sprintf(search_name, "%s%s", (*file) ? file : "*",ext);

  if(strchr(search_name,'*') || strchr(search_name,'?') ) {
    if(wopen(11, 10, 59, 9)) {
      do {
        sprintf(temp_file, "%s%s%s", drive, dir, "*");
        dir_count=0;
        errno = 0;
        findfirst(temp_file, &ffblk, FA_DIREC);
        while(errno == 0 && dir_count < 20) {
          if(ffblk.ff_attrib==16)
            sprintf(otherdir[dir_count++], "%s\\", ffblk.ff_name);
          findnext(&ffblk);
        }
        sprintf(temp_file, "%s%s%s", drive, dir, search_name);
        wtitle(temp_file);
        count = 0;
        errno = 0;
        findfirst(temp_file, &ffblk, 0x00);
        while(errno == 0 && count < MAX_FILES) {
          sprintf(filename[count++], "%s", ffblk.ff_name);
          findnext(&ffblk);
        }
        if(count > 0)
          qsort(filename,count,13,strcmp);
        if(dir_count)
          qsort(otherdir, dir_count, 13, strcmp);
        index=0;
        response=0;
        if(dir_count && otherdir[0][0]=='.') {
          index = 2, dir_count--, response = 1;
        }
        while (count < MAX_FILES && index < dir_count+response) {
          sprintf(filename[count++],"%s",otherdir[index++]);
        }
        if(response)
          sprintf(filename[count++], "%s", otherdir[0]);
        if(count > 0) {
          response = get_response(filename, count, dir_count);
          if(response != -1)
            sprintf(name, "%s%s%s", drive, dir, filename[response]);
          else {
            wclose();
            return NULL;
          }
        }
        else {
          errno=38;
          errorf();
          wclose();
          return NULL;
        }
        if(strchr(filename[response],'\\') == NULL)
          select_done = YES;
        else {
          if(filename[response][0]=='.')
            strcpy(dir, get_modir(dir));
          else
            strcat(dir, filename[response]);
        }
      }while( !select_done );
      wclose();
      return name;
    }
    else {
      return NULL;
    }
  }
  else
    sprintf(temp_file, "%s%s%s", drive, dir, search_name);
  return temp_file;
}

bool set_tabsize(int size)
{
  if(size > 0 && size <= 8) {
    _CW->tabsize = size;
    return true;
  }
  return false;
}

int get_tabsize(void)
{
  return _CW->tabsize;
}

/*
 *  Õeª≥ã°µA ≥a∑°ìe àb •eÆÅ ¡°ã°—¡
 *  filename : Õeª≥–i Ãa∑©∑°üq, NULL∑°°e "UNTITLED.TXT"úaìe ∑°üq∑i ∫Öîa.
 *  bufsize  : ¬AîÅ Õeª≥«aã°, 0∑°°e DEFAULTBUFSIZEüi ¨È∏˜–eîa.
 *  kbd_kind : 3§ÈØ¢; HANKBD3   2§ÈØ¢; HANKBD2
 *  hangul   : –eãi¨w»Åù° Ø°∏b; true   µw¢Ö¨w»Åù° Ø°∏b; false
 */
bool init_editor(char *filename, char *help_file_name, long bufsize, int kbd_kind, bool hangul)
{
  if(bufsize == 0L)
    bufsize = DEFAULTBUFSIZE;
  maxeditbuf = bufsize;
  if((memory = (unsigned char *)farmalloc(bufsize+2)) == NULL) {
    memerror();
    return false;
  }
  *memory++ = 0;    /* Õeª≥§·Ã·∑Å ¿·∑qµA NULL∑i ê˝¥·∫Öîa */

  line_start = current = last_pos = b_b_ptr = b_k_ptr = eofm = memory;
  *memory = *line_buffer = curx = cury = 0;
  *replace_str = *object = *option = *block_file = 0;
  max_line = base_y = base_x = block_size = 0;
  saved = block_display = true;
  user_task = TASK_DISPLAYPAGE;

  if(filename[0] == NULL)
    strcpy(work_file, INIT_FILE);
  else {
    strcpy(work_file, filename);
    if(!load_file(work_file, INIT_LOAD))
      return false;
  }

  HanKbdKind = kbd_kind;
  HanKbdState = hangul;
  strcpy(help_file, help_file_name);

  return true;
}

/*
 *  Õeª≥¿w∑Å «aã°ìe ¬A≠° (5 x 3), ¬AîÅ (80 x 23)
 */

bool editor(int x, int y, int width, int height, int border)
{
  int maxx, maxy, tmp_tabsize;
  int key, state, tmp_state, done;

  tmp_state = done = 0;

  maxx = hgetmaxax();
  maxy = hgetmaxay();
  /*
   *  ∏‚îw–aâA π¡Œaàa ª°∏˜ñA¥ˆìeª° àÒ¨a–eîa.
   */
  if((x < (border ? 1 : 0)) || (x > (border ? maxx-MIN_SCR_COL : maxx-MIN_SCR_COL+1)) ||\
     (y < (border ? 1 : 0)) || (y > (border ? maxy-MIN_SCR_LINE : maxy-MIN_SCR_LINE+1)) ||\
     (width < MIN_SCR_COL) || (width > (border ? MAX_SCR_COL-2 : MAX_SCR_COL)) ||\
     (height < MIN_SCR_LINE) || (height > (border ? MAX_SCR_LINE-2 : MAX_SCR_LINE))) {
    return false;
  }

  maxwidth = width, maxheight = height;

  if(border)
    drawbox(x-1, y-1, (width+2), (height+2), 1);

  disp_status();
  sethwindow(x, y, -width, -height);

  do {
    key = get_key(curx, cury, &state);
    /*
     *  ¨a∂w∏a ∏˜∑Å «°üi ¿·ü°
     */
    if(key == hanja_conv_key) {
      if(tmp_state == HR_HANEND)
        left();
      tmp_state = 0;
      hanja();
    }
    else if(key == ks_graph_key) {
      ks_graphic_char();
    }
    else if(key == tg_graph_key) {
      tg_graphic_char();
    }
    else if(key == kbd_toggle_key && HanKbdState) {
      HanKbdKind = (HanKbdKind == HANKBD2) ? HANKBD3 : HANKBD2;
      disp_kbd_status();
    }
    else if(key == quit_key) {
      if(ask_save() != CANCEL_ANS)
        done = 1;
    }
    else if(key == han_toggle_key) {
      HanKbdState = ! HanKbdState;
      disp_kbd_status();
    }
    else if(key == save_file_key) {
      save();
    }
    else if(key == help_key) {
      help_manager(help_file);
    }
    else {
      if(key != hanja_conv_key)
        tmp_state = 0;
      switch(key) {
        case CTRL_K :
          ctrl_k(get_key2('K'));
          break;
        case CTRL_Q :
          ctrl_q(get_key2('Q'));
          break;
        case CTRL_P:  /* πA¥·¢Ö∏a ∑≥ùb */
          key = get_key2('P');
          if( (key <' ') && key && \
              (strchr("\r\n\x1A", (unsigned char) key) == NULL))
            english(key);
          break;
        case LEFTARROW :
          left();
          break;
        case RIGHTARROW :
          right();
          break;
        case CTRL_LEFT :
          word_left();
          break;
        case CTRL_RIGHT :
          word_right();
          break;
        case UPARROW :
          up_down(-1);
          break;
        case DOWNARROW :
          up_down(1);
          break;
        case PGUP :
          up_down(-maxheight);
          break;
        case PGDN :
          up_down(maxheight);
          break;
        case HOMEKEY :
          hor_locate(0);
          break;
        case ENDKEY :
          hor_locate((unsigned int)linewidth(line_start));
          break;
        case CTRL_HOME :
          up_down(-cury) ;
          break;
        case CTRL_END :
          up_down(maxheight-2-cury);
          break;
        case CTRL_PGUP :
          to_pointer(memory) ;
          break;
        case CTRL_PGDN :
          to_pointer(eofm) ;
          break;
        case CTRL_L :
          find_string(YES);
          break;
        case CR :
          if(insert_mode)
            ins_return();
          else {
            hor_locate(0);
            up_down(1);
          }
          break;
        case CTRL_N :
          if(insert(2, CR))
            user_task = TASK_DISPLAYPAGE;
          break;
        case INSKEY :
          insert_mode = !insert_mode;
          disp_ins_status();
          break;
        case BS :
          if(state == HR_NOHAN) {
            delete(2);
            break;
          }
          if(current == memory)
            break;
          left();
          if(user_task > 1)
            user_func(true);
        case DELKEY :
          del();
          break;
        case CTRL_Y :
          hor_locate(0);
          if(base_y+cury == max_line)
            del_eol();
          else if(delete(linewidth(current)+2))
            user_task = TASK_DISPLAYPAGE;
          break;
        case LSHIFT_SPACE : case RSHIFT_SPACE :
          HanKbdState = ! HanKbdState;
          disp_kbd_status();
          break;
        default :
          if(state == HR_ASC && ' ' <= key && key < 128) {
            if(!insert_mode && (*current & 0x80))
              *(current + 1) = ' ';
            english(key);
          }
          else if(state == HR_ASC  && key == '\t') {
            if(insert_mode)
              insert(_CW->tabsize-((curx+base_x) % _CW->tabsize), ' ');
            tmp_tabsize = _CW->tabsize-((curx+base_x) % _CW->tabsize);
            while(tmp_tabsize--)
              right();
          }
          else if(state == HR_HANST) { /* –eãi ∑≥ùb∑° Ø°∏b */
            hangul();
          }
          else if(state == HR_HANIN || state == HR_HANBS) {
            *current = (key >> 8) & 0xFF;
            *(current + 1) = key & 0xFF;
          }
          else if(state == HR_HANEND) {
            *current = (key >> 8) & 0xFF;
            *(current + 1) = key & 0xFF;
            right();
            tmp_state = state;
          }
      } /* switch */
    } /* else */
  }while( !done );
  releasehwindow();
  return true;
}

