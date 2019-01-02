/* --------------------------- HANQUERY.C -------------------------------*/

/*
 *  ∏aûaÀi ª©∑Å¶Å
 *
 *  ∂A¶ÅµA¨· ∑° ª©∑Å¶ÅµA àÂëA∫Å¥·¥° –aìe ∑•∏aìe îa∑qâ¡ à{îa.
 *
 *  1. ¿·ü°–i ∏aûaÃa∑©∑Å §Â—°
 *  2. ¿·ü°–aâ°∏a –aìe ∏aûa∑Å ¿·ü°¨w»Åüi †i–Å∫â —¡°eπA°¢
 *  3. ¿·ü°–i ∏aûa∂a≠°§Â—°
 *  4. ãa ∏aûa∂a≠°µA îÅ∑w–aìe ∑≥ùb ∂·√°
 *
 *  —¡°e¿·ü°µ¡ ãa ∂A∑Å ∏aûa ∑≥ùbµA îÅ–e °°óe àı∑i ¿·ü°–Å∫Öîa.
 *  –e§ÂµA –aêa∑Å Ãa∑©†e ¿·ü°àaìw–aîa.
 */

#include <stdio.h>
#include <stdlib.h>
#include <alloc.h>
#include <string.h>
#include "hanout.h"
#include "ascii.h"
#include "extkey.h"
#include "hancoord.h"
#include "hanwindw.h"
#include "hanerro2.h"
#include "hanprint.h"
#include "handbms.h"

static void query_help_func(void);
/* ** static int control_pannel(void); ** */
static void save_func(void);
static void undo_func(void);
static void delete_func(void);
static bool quit_func(void);
static void prev_func(void);
static void next_func(void);
static void go_first_func(void);
static void go_last_func(void);
static void query(char *pkey, int mode);
static void clear_record(int file_no);
static void rcdin(void);
static void rcdout(void);
static int empty(char *b, int l);
static int same(void);
static void set_trap(void);
static int key_entry(char *s);
static void init_coord(struct coord_t crd[]);
static int init_data_ele(int data_ele[]);
static int init_query(int file_no, char *title, int data_ele[], struct coord_t crd[]);
static void close_query(int file_no);
static void list_help_func(void);
static void disp_list(int y, int *filelist, int *dlist, char *buf, int offset, int mode);
static void disp_header(int *dlist);
static void disp_foot(void);
static void disp_page(int key_no, int file_no, int *dlist, char *buf, int offset);
static char *db_list(int file_no, char *title, int key_no, int *dlist, int mode);
static void disp_field(char *b, char *msk, int el);
static void init_browse(int file_no, int data_ele[]);

static int file;	              /* <∏aûaÀi ∏˜∑Å¶Å>µA¨· ∏˜∑Å–e Õeª≥–i Ãa∑©∑Å §Â—° */
static int exitflag;	          /* ¿q∑Å àt∑i àaª°°e ª©∑Å¶Åüi †a√•îa.  */
static int existing_record;     /* —¡°eµA ∏aûaàa π•∏Å–a°e ¿q */
static char *rb;	              /* ùA≈°óa §·Ã·; –e Ãa∑©µA π•∏Å–aìe ∏Â œ©óaµA îÅ–Å  */
static char *hb;	              /* ùA≈°óaàa àóØ•ñA¥ˆìeª° —¬∑•–aã°∂·–e •°∑A §·Ã·	  */
char *sc;	                      /* π•∏Å–aìe ∏Â œ©óaµA îÅ–e àı∑° ¥aì°úa Õeª≥–aùaâ°
				                           ∏ÂîiñE ∑•∏aµA îÅ–Å¨·†e, ªb —¡°e §·Ã·	 */
static int len; 	              /* ùA≈°óa∑Å ã©∑° */
int *els, *tmp_els;		          /* ∏aûa∂a≠° ü°ØaÀa */
static int list[MAXELE+1];      /* ∏aûa∂a≠°ói∑Å ü°ØaÀaüi ∏·∏w–i §Åµi */
static int prn_list[MAXELE+1];  /* ∏aûa∂a≠°ói ∫ó ¨Â»ÇñE ∏aûa∂a≠°ói∑i ∏·∏w–i §Åµi */
struct coord_t coord[MAXELE+1]; /* ∏aûaÕeª≥ —¡°eÀiµA¨· π¡Œaàa ∏·∏wñI äÅπ°¡A */
int no_ele;		                  /* ¿·ü°–i ∏aûa∂a≠°∑Å àïÆÅ */
static FILE *result;            /* ∑•≠·§w–∑∑i ∑•≠·ã°ù° –aª°¥gâ° Ãa∑©ù° –aìe âw∂Åüi ∂·–Å */
void (*browse2update[MAXUPDATEFUNC])(int, char*);

extern byte picture[];  /* in HANSCRN.C */
extern int screen_displayed;

/*
 *			 >>   Õeª≥ «°	<<
 *
 *  F2	    : ¨Åù°∂Ö ùA≈°óaêa •eâwñE ùA≈°óaüi ∏aûaÀiµA ∏·∏w
 *  F3	    : ¨Å ∏aûa ∑≥ùb
 *  F8	    : ùA≈°óaüi ¨bπA
 *  PgUp    : ∑°∏Â ùA≈°óa
 *  PgDn    : îa∑q ùA≈°óa
 *  ^Home   : àa∏w ¿·∑q ùA≈°óa
 *  ^End    : àa∏w êa∫ó ùA≈°óa
 *  Ins     : ¨s∑≥/ÆÅ∏˜ §aéë
 *  Enter   : îa∑q œ©óaù° ∑°ï∑
 *  Down    : îa∑q œ©óaù° ∑°ï∑
 *  Tab     : îa∑q œ©óaù° ∑°ï∑
 *  Up	    : ∑°∏Â œ©óaù° ∑°ï∑
 *  ShiftTab: ∑°∏Â œ©óaù° ∑°ï∑
 *  F10     : πA¥·Ãe∑aù° ∑°ï∑
 *  ESC     : †a√°ã°
 */

/* ï°∂ë†i ŒaØ° */
static void query_help_func(void)
{
  if(wopen(hgetmaxax()/2-18, hgetmaxay()/2-5, 36, 15)) {
    hgotoxy(1, 1);
    wtitle("ï°  ∂ë  †i");
    hputs("\n  F2    : ∏aûa ∏·∏w“Å ¨Å ∏aûa∑≥ùb\n");
    hputs("  F8    : ∏aûa ª°∂Åã°\n");
    hputs("  PgUp  : ∑°∏Â ∏aûa\n");
    hputs("  PgDn  : îa∑q ∏aûa\n");
    hputs("  ^PgUp : àa∏w ¿·∑q ∏aûa\n");
    hputs("  ^PgDn : àa∏w êa∫ó ∏aûa\n");
    hputs("  Enter : îa∑q ∂a≠°ù° ∑°ï∑\n");
    hputs("  Down  : îa∑q ∂a≠°ù° ∑°ï∑\n");
    hputs("  Tab   : îa∑q ∂a≠°ù° ∑°ï∑\n");
    hputs("  Up    : ∑°∏Â ∂a≠°ù° ∑°ï∑\n");
    hputs("  #Tab  : ∑°∏Â ∂a≠°ù° ∑°ï∑\n");
    hputs("  F10   : πA¥·Ãe∑aù° ∑°ï∑\n");
    hputs("  ESC   : †a√°ã°\n");

    pause();
    wclose();
  }
}

/*
 * ∏aûa∂a≠°üi Õeª≥–i π¡Œa ¡°ã°—¡
 * «°∏aûa∂a≠°∑Å •°—°üi Œââ° —¡°e §·Ã·üi â∑§Ç¢Ö∏aù° ¿Å∂Öîa.
 * ãaü°â° àa∏w ñ·∑Å ∏aûa ∂·√°ù° ∑°ï∑–eîa.
 */
static void clear_record(int file_no)
{
  int i = 0;

  while (index_ele [file_no][0][i])
    protect(index_ele[file_no][0][i++], FALSE);
  clrrcd(sc, els);
  existing_record = FALSE;
}

/* ùA≈°óa §·Ã·∑Å êÅ∂w∑i —¡°e§·Ã·µ¡ •°∑A§·Ã·µA •¢¨a */
static void rcdin(void)
{
  int i = 0;

  if (empty(rb, rlen(file)) == 0)  {
    rcd_fill(rb, sc, file_ele [file], els);
    mov_mem(rb, hb, rlen(file));
    existing_record = TRUE;
    while (index_ele [file] [0] [i])
      protect(index_ele[file][0][i++],TRUE);
  }
}

/* ¨Å ùA≈°óaüi ¬Åàa–aà·êa ∑°£° ∑∂ìe ∏aûaüi àóØ•–aâ°, îaØ° ∏aûaÀiµA ∏·∏w */
static void rcdout(void)
{
  if (empty(sc, len) == 0)  {
    rcd_fill(sc, rb, els, file_ele[file]);
    if (existing_record)  {
      if (same() == 0)	{
	      post_notice("  ∏aûaüi ∏·∏w–sì°îa  ");
	      if(rtn_rcd(file, rb) == DBERROR) {
          disp_cmsg("  ∏aûaüi ∏·∏w–i ÆÅ ¥ÙØsì°îa  ");
        }
      }
    }
    else  {
      post_notice("  ¨Åù°∂Ö ∏aûaüi ∏·∏w–sì°îa  ");
      if (add_rcd(file, rb) == DBERROR)
	      dberror();
    }
    clear_record(file);
  }
}

/* îe¬Åàa ¨Â»ÇñA°e ãa îe¬Å§Â—°üi ï©ùa∫Åâ°, ¬·≠°ñA°e -1∑i ï©ùa∫Öîa. */
/* *******
static int control_pannel(void)
{
  int isquit = FALSE, key, ans = 0;
  static int buttn_ptr = 0;

  do {
    mark_pannel(buttn_ptr);
    key = getchext();
    mark_pannel(buttn_ptr);
    switch(key) {
      case LEFTARROW : case SHIFT_TAB :
	      if(buttn_ptr == 0) buttn_ptr = BUTTON_CTR -1;
	      else		   buttn_ptr--;
	      break;
      case RIGHTARROW : case '\t' : case ' ' :
	      if(buttn_ptr == (BUTTON_CTR-1)) buttn_ptr = 0;
	      else		   buttn_ptr++;
	      break;
      case ESC : case UPARROW : case DOWNARROW :
	      isquit = TRUE;
	      ans = -1;
        break;
      case '\r' :
	      isquit = TRUE;
	      break;
    }
  } while(!isquit);
  return (ans == -1) ? ans : buttn_ptr;
}
****** */

static void save_func(void)
{
  rcdout();
}

static void undo_func(void)
{
  if(spaces(rb)== 0)  {
    if(yesno("ÆÅ∏˜–e àÈ ∂•úÅîÅù° –iåa∂a ?", 1) == 0) { /* µÅ 0, ¥aì°µ° 1, ¬·≠° 2 */
      rcdin();
    }
    else if(existing_record)  {
      if(same() == 0)  {
	      post_notice("  ∏aûaüi ∏·∏w–sì°îa  ");
	      if(rtn_rcd(file, rb) == DBERROR) {
          disp_cmsg("  ∏aûaüi ∏·∏w–i ÆÅ ¥ÙØsì°îa  ");
        }
      }
    }
  }
}

static void delete_func(void)
{
  if (spaces(sc)== 0)  {
    if(yesno("∑° ∏aûaüi ª°∂âåa∂a ?", 1) == 0) { /* µÅ 0, ¥aì°µ° 1, ¬·≠° 2 */
      del_rcd(file);
      clear_record(file);
    }
  }
}

static bool quit_func(void)
{
  if(yesno("∏b¥Û∑i †a√°Ø°âVØsì°åa ?", 1) == 0) {  /* µÅ 0, ¥aì°µ° 1, ¬·≠° 2 */
    return true;
  }
  return false;
}

static void prev_func(void)
{
  rcdout();
  if(prev_rcd(file, 1, rb) == DBERROR) {
    if (first_rcd(file, 1, rb) == DBERROR)  {
      post_notice("  ∏aûaàa –aêaï° ¥ÙØsì°îa  ");
      return;
    }
    post_notice("  àa∏w ¿·∑q ∏aûa∑≥ì°îa  ");
  }
  rcdin();
}

static void next_func(void)
{
  rcdout();
  if (next_rcd(file, 1, rb) == DBERROR)  {
    if (last_rcd(file, 1, rb) == DBERROR)  {
      post_notice("  ∏aûaàa –aêaï° ¥ÙØsì°îa  ");
      return;
    }
    post_notice("  àa∏w †aª°†b ∏aûa∑≥ì°îa  ");
  }
  rcdin();
}

static void go_first_func(void)
{
  rcdout();
  if (first_rcd(file, 1, rb) == DBERROR)
    post_notice("  ∏aûaàa –aêaï° ¥ÙØsì°îa  ");
  else
    rcdin();
}

static void go_last_func(void)
{
  rcdout();
  if (last_rcd(file, 1, rb) == DBERROR)
    post_notice("  ∏aûaàa –aêaï° ¥ÙØsì°îa  ");
  else
    rcdin();
}

/*
 *  ∑° –qÆÅìe ∏aûa∑Å ∑≥ùbâ¡ ÆÅ∏˜µA¨· °°ñÅ ¨a∂wñAìe –qÆÅ∑°îa.
 *
 *  ∏aûa ∑≥ùbâ¡ ∏aûa ÆÅ∏˜∑i ¿·ü°. pkeyìe ∫ÅµiÆA(primary key)∑Å ¢Ö∏aµi,
 *  modeìe ∑≥ùb/ÆÅ∏˜ ¨w»ÅŒaØ°, modeàa ÆÅ∏˜∑©òÅ†e pkeyàa ∑A“a
 */
static void query(char *pkey, int mode)
{
  int term = 0, buttn_ptr;
/* *******
void (*control_func[8])() =
    { save_func, undo_func, prev_func, next_func,
      go_first_func, go_last_func, delete_func, quit_func };
******* */

  screen_displayed = exitflag = FALSE;

  sethwindow(2, TITLE_Y+2, hgetmaxax()-1, BUTTON_Y-1);
  if(mode == DBINPUT)
    clrrcd(sc, els);	     /* —¡°e§·Ã·µA â∑§Ç∑i ¿Å∂Öîa */
  set_trap();		     /* «° ∏aûa∂a≠°àa ∑≥ùbñE “Å —°¬â–i –qÆÅ ª°∏˜ */
  while (!exitflag)  {
    term = data_entry(pkey, file); /* —¡°e ∑≥ùbÀiµA¨· Ø©πA ∑≥ùb∑i §hìe ¶Å¶Ö */
/* ********
    if(term == SWITCHKEY) {
      buttn_ptr = control_pannel();
      if(buttn_ptr != -1)
	      control_func[buttn_ptr]();
      if(exitflag)
        term = QUITKEY;
    }
    else {
****** */
      switch (term)  {
	      case HELPKEY :
	        query_help_func();
	        break;
        case SAVEKEY :
	        save_func();
	        break;
        case CTRL_PGUP :
	        go_first_func();
	        break;
        case CTRL_PGDN :
	        go_last_func();
	        break;
        case PGUP:
	        prev_func();
	        break;
        case PGDN:
	        next_func();
	        break;
        case DELETEKEY :
	        delete_func();
	        break;
        case MENUKEY : case QUITKEY :
          exitflag = quit_func();
          if(exitflag)
            rcdout();
	        break;
        case UNDOKEY :
          undo_func();
          break;
      } /* switch */
/*    } */ /* else */
  } /* while */
  sethwindow(1, 1, hgetmaxax(), hgetmaxay());
}

/* ùA≈°óa §·Ã·àa ß°¥·∑∂êa? ªb â∑§Ç¢Ö∏aù°†e ñA¥· ∑∂êa? ß°¥·∑∂∑a°e ¿q */
static int empty(char *b, int l)
{
  while (l--)
    if (*b && *b != ' ')
      return FALSE;
    else
       b++;
  return TRUE;
}

/* ùA≈°óa §·Ã·µ¡ •°∑A§·Ã·àa îaüeª° àÒ¨a. ªb, ùA≈°óaàa àóØ•ñA¥ˆìeª° */
static int same(void)
{
  int ln = rlen(file);

  while (--ln)
    if (*(rb + ln) != *(hb + ln))
      break;
  return (*(rb + ln) == *(hb + ln));
}

/* ã°••«°àa ∑≥ùbñA¥ˆ∑i òÅ ãaàı∑i ¿·ü°–i –qÆÅüi ¨È∏˜ */
static void set_trap(void)
{
  int i = 0;

  while (index_ele [file] [0] [i])
    i++;
  edit(index_ele [file] [0] [i-1], key_entry);
}

/* ∫Å«°àa ∑≥ùbñAâ° êa¨· §aù° ∑° –qÆÅàa —°¬âñEîa */
static int key_entry(char *s)
{
  char key [MAXKEYLEN];
  int i;

  if (spaces(s))   /* ∑≥ùbñE ∏aûaàa ¥Ùîa°e */
    return DBOK;
  *key = '\0';
  i = 0;
  while (index_ele [file] [0] [i])  {
    protect(index_ele[file][0][i],TRUE);
    strcat(key, sc + epos(index_ele[file][0][i++], els));
  }
  if (find_rcd(file, 1, key, rb) == DBERROR)  {
    post_notice("  ¨Åù°∂Ö ∏aûa∑≥ì°îa  ");
    existing_record = FALSE;
    return DBOK;
  }
  rcdin();
  tally();
  return DBOK;
}

/* ¿·ü°–i ∏aûa∑Å π¡Œaüi ¡°ã°—¡–eîa. */
static void init_coord(struct coord_t crd[])
{
  register i;

  for(i = 0; i < no_ele ; i++)
    coord[i].x = crd[i].x, coord[i].y = crd[i].y;
}

/*
 * ¿·ü°–i ∏aûa∂a≠°∑Å ¢Ö∏aµi °¢ù¢∑i Õ°∑•»· §Åµi listµA îqìeîa.
 */
static int init_data_ele(int data_ele[])
{
  int i;

  if( ! data_ele[0])
    return 0;
  for(i = 0; data_ele[i]; i++)
    list[i] = data_ele[i];
  return i;
}

/*
 * query –qÆÅ ¨a∂w∑i ∂·–e ¡°ã°—¡!
 * ¨˜â∑–a°e 1, Ø©ÃÅ–a°e 0∑i ï©ùa∫Öîa
 */

static int init_query(int file_no, char *title, int data_ele[], struct coord_t crd[])
{
  int count, i;

  file = file_no;
  count = init_data_ele(data_ele);
  if (count == 0)  {		      /* °°óe ∏aûa∂a≠°üi ¿·ü°–aâVîa. */
    len = rlen(file);
    els = file_ele [file_no];
    for(i = 0; els[i]; i++)	  /*  *file_ele[]∑Å ∂a≠°∑Å àïÆÅ */
      ;
    no_ele = i;
  }
  else {
    len = epos(0, data_ele);
    els = data_ele;
    no_ele = count;
  }
  sc = (char *)farmalloc(len);	      /* screen buffer */
  rb = (char *)farmalloc(rlen(file));  /* record buffer */
  hb = (char *)farmalloc(rlen(file));  /* hold buffer	*/
  if (sc == (char *) 0 || rb == (char *) 0 ||  hb == (char *) 0)  {
    memerror();
    return FALSE;
  }
  init_rcd(file, rb);	   /* rbüi 'file'∑Å äÅπ°µA òaúa â∑§Ç∑aù° ¿Å∂Öîa. */
  init_rcd(file, hb);	   /* hbüi 'file'∑Å äÅπ°µA òaúa â∑§Ç∑aù° ¿Å∂Öîa. */
  init_screen(title, els, sc);
  init_coord(crd);
  return TRUE;
}

static void close_query(int file_no)
{
  clear_record(file_no);
  clear_edit();
  farfree(hb);
  farfree(rb);
  farfree(sc);
}

/*
 *  ∏aûa ∑≥ùb∑i ∂·–e –qÆÅ.
 *
 *  ∑° ª©∑Å¶Åüi ¶Åüaã°∏ÂµA ∑°£° °°óe â≈ùe Ãa∑©∑e µi¥·ñÅìe àı∑aù° –a∏a
 *  âÅ√w∏‚ ¿aü±Œa§wØ¢µA¨·ìe —e∏Å∑Å §wØ¢∑° î· êa∑iª° °°üaª°†e
 *  Œâîa∂Ö §wØ¢∑°úa°e †Å∂Å â•úe–e §wØ¢∑° ñEîa.
 *
 *  œaù°ãaúë Ø©–ó∑Å ••¡AµA¨· ∏b¥Û Ãa∑©∑i °°ñÅ µiâ°
 *  œaù°ãaúë∑i è{†a√©òÅ ∏b¥Û Ãa∑©∑i îhìe §wØ¢∑i ¬·–a∏a.
 *
 *  Ãa∑©§Â—°∑Å µ°üA¢ÖπAìe ∑° –qÆÅ ¶Åüaã°∏ÂµA ¿·ü°–Å¥° –eîa.
 *
 *  *title     : ¿·ü°–i ∏aûa —¡°e∑Å πA°¢
 *  data_ele[] : ¿·ü°–i ∏aûa∂a≠°∑Å ∏˜ÆÅ §Åµi
 *  crd[]      : ∂·∑Å ∏aûa∂a≠°µA îÅ∑w–aìe ∂a≠°ói∑Å ∑≥ùb∂·√°
 */

void db_input(int file_no, char *title, int data_ele[], struct coord_t crd[])
{
  if(init_query(file_no, title, data_ele, crd)) {
    query(NULL, DBINPUT);
    close_query(file_no);
  }
}

/*
 *  ∏aûa ¿xã°üi ∂·–e –qÆÅ
 *
 *  *title     : ¿·ü°–i ∏aûa —¡°e∑Å πA°¢
 *  data_ele[] : ¿·ü°–i ∏aûa∂a≠°∑Å ∏˜ÆÅ §Åµi
 *  crd[]      : ∂·∑Å ∏aûa∂a≠°µA îÅ∑w–aìe ∂a≠°ói∑Å ∑≥ùb∂·√°
 *  keyno      : µiÆA §Â—°
 *  *keyvalue  : ¿x∑aùaìe ∏aûa∑Å ¢Ö∏aµi
 */

void db_search(int file_no, char *title, int data_ele[], struct coord_t crd[], int keyno, char *keyvalue)
{
  if(init_query(file_no, title, data_ele, crd)) {
    file = file_no;
    if(find_rcd(file, keyno, keyvalue, rb) != DBERROR) {
      post_notice("  ∏aûaüi ¿x¥vØsì°îa  ");
      rcdin();
      query(keyvalue, DBUPDATE);
    }
    else {
      disperror("  ãaúÂ ∏aûaìe ¥ÙìeïA∂a.  ");
    }
    close_query(file_no);
  }
}

/*
 *  Õi¡a•°ã° -- > ∏aûa ÆÅ∏˜∑i ∂·–e –qÆÅ
 *
 *  *title     : ¿·ü°–i ∏aûa —¡°e∑Å πA°¢
 *  data_ele[] : ¿·ü°–i ∏aûa∂a≠°∑Å ∏˜ÆÅ §Åµi
 *  crd[]      : ∂·∑Å ∏aûa∂a≠°µA îÅ∑w–aìe ∂a≠°ói∑Å ∑≥ùb∂·√°
 */

void db_update(int file_no, char *title, int data_ele[], struct coord_t crd[], int key, char *buf)
{
  char keyvalue[MAXKEYLEN];
  int tmp;

  if(init_query(file_no, title, data_ele, crd)) {
    file = file_no;
    memcpy(rb, buf, rlen(file_no));
    rcdin();
    tmp = epos(index_ele[file_no][0][key-1], els);
    strcpy(keyvalue, sc+tmp);
    query(keyvalue, DBUPDATE);
    close_query(file_no);
  }
}

void db_delete(int file_no, char *title, int data_ele[], struct coord_t crd[], int keyno, char *keyvalue)
{
  if(init_query(file_no, title, data_ele, crd)) {
    file = file_no;
    if(find_rcd(file, keyno, keyvalue, rb) != DBERROR) {
      rcdin();
      display_template(title, 1);
      tally();
      delete_func();
      sethwindow(1, 1, hgetmaxax(), hgetmaxay());
    }
    else {
      disperror("  ãaúÂ ∏aûaìe ¥ÙìeïA∂a.  ");
    }
    close_query(file_no);
  }
}

/*
 *  ∏aûa Õi¡a•°ã° - —¡°e äÅ¨˜∑e îa∑qâ¡ à{îa.
 *
 *     +-----------------------------------------------------------+
 *   1 |   °AìA ¶Å¶Ö                                               |
 *     +-----------------------------------------------------------+
 *   2 |   —¡°e πA°¢                                               |
 *     +-----------------------------------------------------------+
 *   3 |   àb ∏aûa∂a≠° ∑°üq                                        |
 *     +-----------------------------------------------------------+
 *     |                                                           |
 *     .                                                           .
 *     .                                                           .
 *     .                                                           .
 *     +-----------------------------------------------------------+
 *  24 |   ∏aûa¨w»Åüi ŒaØ°–eîa.  ¡∑ ∏aûaÆÅµ¡ —e∏Å ∏aûa§Â—° òa∂·..  |
 *     +-----------------------------------------------------------+
 *  25 |                                                           |
 *     +-----------------------------------------------------------+
 */

#define  LISTLINES   (20)                    /* Õi¡a•°ã°∑Å ∫â ÆÅ */
#define  PRNLINES    (50)                    /* –e π∑∑°µA ∑•≠·ñI ∫â ÆÅ */
#ifndef DISP_MODE
  #define DISP_MODE
  enum { REVERSE, NORMAL } disp_mode;          /* –e∫â ŒaØ° òÅ §e∏ÂØ°«© àı∑•àa */
#endif

/* ï°∂ë†i ŒaØ° */
static void list_help_func(void)
{
  if(wopen(hgetmaxax()/2-18, hgetmaxay()/2-5, 36, 13)) {
    hgotoxy(1, 1);
    wtitle("‘Ñ  ï°  ∂ë  †i  ‘Ñ");
    hputs("\n  Del   : ∏aûa ª°∂Åã°\n");
    hputs("  ‘ò    : ∑°∏Â ∏aûa\n");
    hputs("  ‘ô    : îa∑q ∏aûa\n");
    hputs("  ^Home : —¡°e πA∑© ∂·\n");
    hputs("  ^End  : —¡°e πA∑© ¥aúÅ\n");
    hputs("  Enter : ∑° ∏aûa Õeª≥\n");
    hputs("  ‘ö    : µ°üeΩ¢ —¡°e ¶Å¶Ö\n");
    hputs("  ‘õ    : ∂EΩ¢ —¡°e ¶Å¶Ö\n");
    hputs("  F10   : ¿aü±Œaù°..\n");
    hputs("  ESC   : ¿aü±Œaù°..\n");
    pause();
    wclose();
  }
}

static int prev_list(int file_no, int key_no, char *buf)
{
  int flag;

  flag = 1;
  if(prev_rcd(file_no, key_no, buf) == DBERROR) {
    flag = 0;
    if (first_rcd(file_no, key_no, buf) == DBERROR)  {
      post_notice("  ∏aûaàa –aêaï° ¥ÙØsì°îa  ");
    }
    else {
      post_notice("  àa∏w ¿·∑q ∏aûa∑≥ì°îa  ");
    }
  }
  return flag;
}

static int next_list(int file_no, int key_no, char *buf)
{
  int flag;

  flag = 1;
  if (next_rcd(file_no, key_no, buf) == DBERROR)  {
    flag = 0;
    if (last_rcd(file_no, key_no, buf) == DBERROR)  {
      post_notice("  ∏aûaàa –aêaï° ¥ÙØsì°îa  ");
    }
    else {
      post_notice("  àa∏w †aª°†b ∏aûa∑≥ì°îa  ");
    }
  }
  return flag;
}

static int go_first_list(int file_no, int key_no, char *buf)
{
  int flag;

  flag = 0;
  if (first_rcd(file_no, key_no, buf) == DBERROR) {
    flag = 0;
    post_notice("  ∏aûaàa –aêaï° ¥ÙØsì°îa  ");
  }
  return flag;
}

static int go_last_list(int file_no, int key_no, char *buf)
{
  int flag;

  flag = 1;
  if (last_rcd(file_no, key_no, buf) == DBERROR) {
    flag = 0;
    post_notice("  ∏aûaàa –aêaï° ¥ÙØsì°îa  ");
  }
  return flag;
}

static void disp_header(int *dlist)
{
  int onoff, width, line_width;

  onoff = isshadow();
  hsetshadow(ON);
  eputchs(-1, -TITLE_Y-1, hgetmaxax(), ' ');
  line_width = 0;
  while(*dlist) {
    width = headlen(*dlist);  /* ∏aûa »i∑Å ã©∑°µ¡ –eãi∏aûa∂a≠° ∑°üq∫ó ã• àı */
    if(line_width+width+1 > CARDWIDTH)         /* —¡°e «aã°•°îa ƒ·ª°°e ãa†e !!! */
      break;
    hputsxy(line_width + 1, -TITLE_Y-1, hdenames[*dlist-1]);
    line_width += width + 1;
    dlist++;
  }
  hsetshadow(onoff);
}

static bool delete_list(int file_no)
{
  if(yesno("∑° ∏aûaüi ª°∂âåa∂a ?", 1) == 0) { /* µÅ 0, ¥aì°µ° 1, ¬·≠° 2 */
    del_rcd(file_no);
    return true;
  }
  return false;
}

static void disp_page(int key_no, int file_no, int *dlist, char *buf, int offset)
{
  int y;

  hclrscr();
  for(y = 1; errno != D_EOF && y <= LISTLINES; y++) {
    hgotoxy(1, y);
    if(key_no) {                      /* ¨Ç∑•§Â—°üi ª°∏˜–ñîa°e */
      disp_list(y, file_ele[file_no], dlist, buf, offset, NORMAL);
      next_rcd(file_no, key_no, buf); /* ãa ¨Ç∑• ÆÖ¨·ù° */
    }               /* ∏aûaàa ∏·∏wñE ¢âü°∏‚ ÆÖ¨·µA òaúa ∏aûaüi ŒaØ°–eîa. */
    else if(seqrcd(file_no, buf) != DBERROR)  {
      disp_list(y, file_ele[file_no], dlist, buf, offset, NORMAL);
    }
  }
  for(; y > (errno == D_EOF ? 2 : 1); y--)
    prev_rcd(file_no, key_no, buf);
}

static void disp_foot(void)
{
  char *str = "  ‘û-∑°∏Â   ‘ü-îa∑q   ^Home-†Ö¥|   ^End-†Öñ·   ‘˙-Õeª≥  ‘ë-‘ê-—¡°eπ¡∂Å";
  bool onoff;

  onoff = isshadow();
  hsetshadow(ON);
  hputsxy(-1, -BUTTON_Y, str);
  eputchs(-(strlen(str)+1), -BUTTON_Y, hgetmaxax()-strlen(str), ' ');
  hsetshadow(onoff);
  onoff = isreverse();
  hsetreverse(ON);
  eputchs(-1, -hgetmaxay(), hgetmaxax(), ' ');
  hsetreverse(onoff);
}

static char *db_list(int file_no, char *title, int key_no, int *dlist, int mode)
{
  char *buf;
  int onoff, isquit, y, ch, offset, tmpy;

  if(!browse2update[file_no])
    return NULL;
  buf = (char *)farmalloc(rlen(file_no));
  if(!buf) {
    memerror();
    return;
  }

  errno = 0;

  if(first_rcd(file_no, 1, buf) == DBERROR)  {
    post_notice("  ∏aûaàa –aêaï° ¥ÙØsì°îa  ");
    return;
  }
  sethwindow(1, 1, hgetmaxax(), hgetmaxay());
  drawbox(1, TITLE_Y, hgetmaxax(), hgetmaxay()-2, 1);
  onoff = isreverse();
  hsetreverse(ON);
  eputchs(1, -TITLE_Y, hgetmaxax(), ' ');
  hputsxy(-(hgetmaxax()/2-strlen(title)/2), -TITLE_Y, title);
  hsetreverse(onoff);

  /* êÅ¶Åüi ¿w ¨È∏˜ */
  sethwindow(2, TITLE_Y+2, hgetmaxax()-1, BUTTON_Y-1);
  hclrscr();
  if(key_no)                          /* ¨Ç∑•§Â—°üi ª°∏˜–ñîa°e */
    first_rcd(file_no, key_no, buf);  /* ∏aûa∑Å πA∑© ¿·∑qµA¨· Ø°∏b */
  disp_header(dlist);                 /* Õi¡a•© àb ∏aûa∂a≠° ∑°üq∑i ŒaØ° */
  disp_foot();
  isquit = offset = 0;
  disp_page(key_no, file_no, dlist, buf, offset);
  y = 1;
  do {
    errno = 0;            /*  µ°üA§Â—° ¡°ã°—¡ */
    disp_list(y, file_ele[file_no], dlist, buf, offset, REVERSE);
    ch = getchext();
    disp_list(y, file_ele[file_no], dlist, buf, offset, NORMAL);
    switch(ch) {
      case '\r' :
        if(mode == B2UPDATE) {
          (browse2update[file_no])(file_no, buf);
          sethwindow(2, TITLE_Y+2, hgetmaxax()-1, BUTTON_Y-1);
          disp_header(dlist+offset);
          disp_foot();
          for(tmpy = y; y > 1; y--)
            prev_rcd(file_no, key_no, buf);
          if(errno)
            next_rcd(file_no, key_no, buf);
          errno = 0;
          disp_page(key_no, file_no, dlist, buf, offset);
          for(errno = 0; y < tmpy; y++)
            next_rcd(file_no, key_no, buf);
          if(errno && y > 1)
            y--;
        }
        else {
          return buf + epos(index_ele[file_no][0][0], file_ele[file_no]);
        }
        break;
      case CTRL_HOME :
        for(; y > 1; y--)
          prev_rcd(file_no, key_no, buf);
        break;
      case CTRL_END :
        for(errno = 0; errno != D_EOF && y < LISTLINES; y++)
          next_rcd(file_no, key_no, buf);
        if(errno == D_EOF && y)
          y--;
        break;
      case UPARROW :
        if(y > 1) {
          if(prev_list(file_no, key_no, buf))
            y--;
        }
        else {
          if(prev_list(file_no, key_no, buf))
            hbackscroll();
        }
        break;
      case DOWNARROW :
        if(y < LISTLINES) {
          if(next_list(file_no, key_no, buf))
            y++;
        }
        else {
          if(next_list(file_no, key_no, buf))
            hscroll();
        }
        break;
      case PGUP :
        for(; y > 0; y--) {
          if(!prev_list(file_no, key_no, buf))
            break;
        }
        if(y == 0) {
          for(y = LISTLINES; y > 1; y--) {
            if(!prev_list(file_no, key_no, buf))
              break;
          }
          errno = 0, y = 1;
          disp_page(key_no, file_no, dlist, buf, offset);
        }
        break;
      case PGDN :
        for(; y <= LISTLINES; y++) {
          if(!next_list(file_no, key_no, buf))
            break;
        }
        if(y == LISTLINES+1) {
          errno = 0, y = 1;
          disp_page(key_no, file_no, dlist, buf, offset);
        }
        break;
      case CTRL_PGUP :
        for(; y > 0; y--) {
          if(!prev_list(file_no, key_no, buf))
            break;
        }
        if(y == 0) {
          go_first_list(file_no, key_no, buf);
          errno = 0, y = 1;
          disp_page(key_no, file_no, dlist, buf, offset);
        }
        break;
      case CTRL_PGDN :
        for(; y < LISTLINES; y++) {
          if(!next_list(file_no, key_no, buf))
            break;
        }
        if(y == LISTLINES) {
          go_last_list(file_no, key_no, buf);
          y = 1, errno = 0;
          disp_page(key_no, file_no, dlist, buf, offset);
        }
        break;
      case LEFTARROW :
        if(offset) {
          offset--;
          disp_header(dlist+offset);
          for(tmpy = y; y > 1; y--)
            prev_rcd(file_no, key_no, buf);
          disp_page(key_no, file_no, dlist, buf, offset);
          for(; y < tmpy; y++)
            next_rcd(file_no, key_no, buf);
        }
        else {
          post_notice(" ∂EΩ¢ è{∑≥ì°îa ");
        }
        break;
      case RIGHTARROW :
        if(*(dlist+offset)) {
          offset++;
          disp_header(dlist+offset);
          for(tmpy = y; y > 1; y--)
            prev_rcd(file_no, key_no, buf);
          disp_page(key_no, file_no, dlist, buf, offset);
          for(; y < tmpy; y++)
            next_rcd(file_no, key_no, buf);
        }
        else {
          post_notice(" µ°üeΩ¢ è{∑≥ì°îa ");
        }
        break;
      case DELETEKEY :
        if(delete_list(file_no)) {
          for(tmpy = y-1; y > 1; y--)
            prev_rcd(file_no, key_no, buf);
          next_rcd(file_no, key_no, buf);
          disp_page(key_no, file_no, dlist, buf, offset);
          for(errno = 0; errno != D_EOF && y <= tmpy; y++)
            next_rcd(file_no, key_no, buf);
          if(errno == D_EOF && y)
            y--;
        }
        break;
      case HELPKEY :
        list_help_func();
        break;
      case QUITKEY : case MENUKEY:
        isquit = quit_func();
        break;
    }
  } while(!isquit);
  sethwindow(1, 1, hgetmaxax(), hgetmaxay());
  farfree(buf);
  return NULL;
}

int headlen(int el)
{
  return strlen(elmask[el-1]) < strlen(hdenames[el-1]) ? strlen(hdenames[el-1]) : strlen(elmask[el-1]);
}

/* bìe —¡°e §·Ã·µA¨· —e∏Å œ©óa∑Å ∂·√°  Õ°∑•»·, ∏aûa ∂a≠°üi —¡°eµA ¬âùb */
static void disp_field(char *b, char *msk, int el)
{
   char merge[80];
   register i = 0, gap = 0;

   if(eltype[el-1] == 'G') { /* ãaü± Ãa∑©∑Å âw∂Å */
     hputs("*ãaü±*");
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
     gap = strlen(hdenames[el-1])-strlen(merge);
     gap = gap > 0 ? gap : 0;
     hgotoxy(hwherex()+gap+1, hwherey());
   }
}

/*
 *  ∏aûaüi –e∫â ŒaØ°–eîa.
 */

static void disp_list(int y, int *filelist, int *dlist, char *buf, int offset, int mode)
{
  char *line, *cur_ptr, *mask_ptr;
  int width, line_width, onoff;

  line_width = 0;
  line = (char *)farmalloc(epos(0, dlist));
  if(!line) {
    memerror();
    return;
  }
  if(mode == REVERSE) {
    onoff = isreverse();
    hsetreverse(ON);
  }
  hgotoxy(1, y);
  rcd_fill(buf, line, filelist, dlist);
  cur_ptr = line;
  cur_ptr += epos(*(dlist+offset), dlist);
  while(*(dlist+offset)) {   /* ∏aûa°¢ù¢µA ∑∂ìe ∏aûa ∂a≠° àïÆÅ†e«q */
    mask_ptr = elmask[(*(dlist+offset))-1];
    width = headlen(*(dlist+offset));  /* ∏aûa »i∑Å ã©∑°µ¡ –eãi∏aûa∂a≠° ∑°üq∫ó ã• àı */
    line_width += width + 1;
    if(line_width > CARDWIDTH)         /* —¡°e «aã°•°îa ƒ·ª°°e ãa†e !!! */
      break;
    disp_field(cur_ptr, mask_ptr, (*(dlist+offset)));
    dlist++;
    cur_ptr += strlen(cur_ptr)+1;
  }
  if(mode == REVERSE) {
    hsetreverse(onoff);
  }
  farfree(line);
}

/*
 *  Õi¡a•°ã° ¡°ã°—¡
 */

static void init_browse(int file_no, int data_ele[])
{
  int count, i;

  tmp_els = els;
  count = init_data_ele(data_ele);
  if (count == 0)  {		      /* °°óe ∏aûa∂a≠°üi ¿·ü°–aâVîa. */
    len = rlen(file_no);
    els = file_ele [file_no];
    for(i = 0; els[i]; i++)	  /*  *file_ele[]∑Å ∂a≠°∑Å àïÆÅ */
      ;
    no_ele = i;
  }
  else {
    len = epos(0, data_ele);
    els = data_ele;
    no_ele = count;
  }
}

static void close_browse(void)
{
  els = tmp_els;
}

/*
 *  ∑≥ùb–qÆÅµ¡ —¡°e §·Ã·àa ¬óï©–aª°¥gìeàa àÒ¨a–aï°ù¢...
 *
 *  key_no : 0 ∑°°e ∏aûaàa ∏·∏wñE ÆÖ¨·ù°
 *           1 ∑°¨w∑Å ÆÅúa°e ãa ¨Ç∑•§Â—°µA òaúa ¨Ç∑• ÆÖ∑aù° ..
 *
 *  ª©∑Å¢Ö∑Å µÅüi ói¥·•°°e
 *  1) †aàq∑©∏aàa 93.12.XX ∑• ¨aúqói∑i ∑°üq ÆÖ∑aù° •°â°Øºîa !
 *  2) ¨˜∑° 'ä•'∑• ¨aúqói∑i ∫Å£•ówù¢§Â—° ÆÖ∑aù° •°â°Øºîa !
 *  ãaú˝îa°e œ©∂a–e ∑•∏aói∑e îa∑qâ¡ à{îa.
 *  . ∏b¥Û îÅ¨w Ãa∑©§Â—°
 *  . ∏b¥Û—¡°e∑Å πA°¢
 *  . ∏b¥ÛîÅ¨w∑° ñAìe ∏aûa ∂a≠° °¢ù¢
 *  . ∏˜ùi–aâ°∏a–aìe µiÆA§Â—°
 *  . ß°äa≈°∏a–aìe ∂a≠°§Â—°
 *  . ß°äa–i ¢Ö∏aµi
 *  . ¢Ö∏aµiµA¨· ¥·ó°µA¨· ¥·ó°åaª°üi ß°äa–i àı∑•àa..
 */

char *db_browse(int file_no, char *title, int data_ele[], int key_no, int mode)
{
  char *ptr;

  hallowautoscroll(false);   /* ∏aï∑Øa«aù© ãqª° */
  init_browse(file_no, data_ele);
  ptr = db_list(file_no, title, key_no, els, mode);
  close_browse();
  return ptr;
}

/*
 *   ∑•≠·¶Å
 */

int (*prn_func[2])() = { lputs, fputs };

static bool get_width(int *dlist)
{
  int width, line_width;

  line_width = 0;
  while(*dlist) {
    width = headlen(*dlist++);
    if(line_width+width+1 >= PAPERWIDTH)
      return false;
    line_width += width + 1;
  }
  return true;
}

bool check_printer(void)
{
  while( 1 ) {
    if(!isOKprn()) {
      if(retry("œaü•»·àa å·πa ∑∂à·êa, µeâiñA¥·∑∂ª° ¥gØsì°îa", 0) != 0)
        return false;
    }
    else
      return true;
  }
}

int *get_print_list(int file_no, char *title, int data_ele[], struct coord_t crd[])
{
  int count, len, i, j;
  int ele_list[MAXELE+1] = { 0, }; /* ¡°ã°—¡ */

  count = init_data_ele(data_ele);
  if (count == 0)  {		      /* °°óe ∏aûa∂a≠°üi ¿·ü°–aâVîa. */
    len = rlen(file);
    els = file_ele [file_no];
    for(i = 0; els[i]; i++)	  /*  *file_ele[]∑Å ∂a≠°∑Å àïÆÅ */
      ;
    no_ele = i;
  }
  else {
    len = epos(0, data_ele);
    els = data_ele;
    no_ele = count;
  }
  sc = (char *)farmalloc(len);	      /* screen buffer */
  init_screen(title, els, sc);
  init_coord(crd);

  display_template(title, 0);

  if(!get_ele_list(ele_list)) {
    sethwindow(1, 1, hgetmaxax(), hgetmaxay());
    return NULL;
  }
  for(i = 0, j = 0; ele_list[j] != -1 && j < MAXELE; j++) {
    if(ele_list[j])
      prn_list[i++] = ele_list[j];
  }
  if( !i )
    prn_list[0] = 0, prn_list[1] = -1;
  sethwindow(1, 1, hgetmaxax(), hgetmaxay());
  farfree(sc);
  return prn_list;
}

static void print_header(int *dlist, char *title, int mode)
{
  int width, line_width, gap;
  int *tmp;

  /* ∑©îe —¡°eµA ∑•≠·ñI Õ¢∑i äÅ–Å•°∏a */
  tmp = dlist;
  line_width = 0;
  while(*dlist) {
    width = headlen(*dlist++);
    if(line_width+width+1 >= PAPERWIDTH)
      break;
    line_width += width + 1;
  }

  if(mode == DATA2PRN && !check_printer())
    return;
  /* ∏aûa πA°¢∑i ≥a∏a */
  gap = line_width/2-strlen(title)/2;
  while(gap-- > 0)
    prn_func[mode](" ", result);
  prn_func[mode](title, result);
  prn_func[mode](mode == DATA2FILE ? "\n\n" : "\n\r\n\r", result);

  /* ∂ı∫â ãuã° */
  dlist = tmp;
  line_width = 0; /* ∑°àÂ ª°ãq ¥e≥aª°†e ø¢”© ∏aûa∑Å ã©∑°üi £°ü° ¥iã°∂·–Å œ©∂a */
  while(*dlist) {
    width = headlen(*dlist);  /* ∏aûa »i∑Å ã©∑°µ¡ –eãi∏aûa∂a≠° ∑°üq∫ó ã• àı */
    if(line_width+width+1 >= PAPERWIDTH)         /* π∑∑° «aã°•°îa ƒ·ª°°e ãa†e !!! */
      break;
    while(width-- >= 0)
      prn_func[mode]("-", result);
    dlist++;
  }
  prn_func[mode](mode == DATA2FILE ? "\n" : "\n\r", result);

  /* àb ∏aûa∂a≠°∑Å ∑°üq ≥aã° */
  dlist = tmp;
  line_width = 0;
  while(*dlist) {
    width = headlen(*dlist);  /* ∏aûa »i∑Å ã©∑°µ¡ –eãi∏aûa∂a≠° ∑°üq∫ó ã• àı */
    if(line_width+width+1 >= PAPERWIDTH)         /* π∑∑° «aã°•°îa ƒ·ª°°e ãa†e !!! */
      break;
    prn_func[mode](hdenames[*dlist-1], result);
    gap = width - strlen(hdenames[*dlist-1]) + 1;
    while(gap-- > 0)
      prn_func[mode](" ", result);
    dlist++;
  }
  prn_func[mode](mode == DATA2FILE ? "\n" : "\n\r", result);

  /* ¥aúÅ ∫â ãuã° */
  dlist = tmp;
  line_width = 0;
  while(*dlist) {
    width = headlen(*dlist);  /* ∏aûa »i∑Å ã©∑°µ¡ –eãi∏aûa∂a≠° ∑°üq∫ó ã• àı */
    if(line_width+width+1 >= PAPERWIDTH)         /* π∑∑° «aã°•°îa ƒ·ª°°e ãa†e !!! */
      break;
    while(width-- >= 0)
      prn_func[mode]("-", result);
    dlist++;
  }
  prn_func[mode](mode == DATA2FILE ? "\n" : "\n\r", result);
}

static void print_field(char *b, char *msk, int el, int mode)
{
   char merge[80];
   register i = 0, gap = 0;

   if(eltype[el-1] == 'G') { /* ãaü± Ãa∑©∑Å âw∂Å */
     prn_func[mode]("*ãaü±*", result);
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
     if(mode == DATA2PRN && !check_printer())
       return;
     prn_func[mode](merge, result);
     gap = strlen(hdenames[el-1])-strlen(merge) + 1;
     gap = gap >= 0 ? gap : 1;
     while(gap-- > 0)
     prn_func[mode](" ", result);
   }
}

static void print_list(int *filelist, int *dlist, char *buf, int mode)
{
  char *line, *cur_ptr, *mask_ptr;
  int width, line_width;

  line_width = 0;
  line = (char *)farmalloc(epos(0, dlist));
  if(!line) {
    memerror();
    return;
  }
  rcd_fill(buf, line, filelist, dlist);
  cur_ptr = line;
  cur_ptr += epos(*dlist, dlist);
  while(*dlist) {   /* ∏aûa°¢ù¢µA ∑∂ìe ∏aûa ∂a≠° àïÆÅ†e«q */
    mask_ptr = elmask[(*dlist)-1];
    width = headlen(*dlist);  /* ∏aûa »i∑Å ã©∑°µ¡ –eãi∏aûa∂a≠° ∑°üq∫ó ã• àı */
    line_width += width + 1;
    if(line_width >= PAPERWIDTH)
      break;
    print_field(cur_ptr, mask_ptr, (*dlist), mode);
    dlist++;
    cur_ptr += strlen(cur_ptr)+1;
  }
  if(mode == DATA2PRN && !check_printer())
    return;
  prn_func[mode](mode == DATA2FILE ? "\n" : "\n\r", result);
  farfree(line);
}

static void print_all_list(int file_no, char *title, int key_no, int *dlist, int mode)
{
  int lines;
  char *buf;

  buf = (char *)farmalloc(rlen(file_no));
  if(!buf) {
    memerror();
    return;
  }
  errno = 0;
  if(key_no)                          /* ¨Ç∑•§Â—°üi ª°∏˜–ñîa°e */
    first_rcd(file_no, key_no, buf);  /* ∏aûa∑Å πA∑© ¿·∑qµA¨· Ø°∏b */

  do {
    print_header(dlist, title, mode);   /* Õi¡a•© àb ∏aûa∂a≠° ∑°üq∑i ŒaØ° */
    for(lines = 0; errno != D_EOF && lines <= PRNLINES; lines++) {
      if(key_no) {                      /* ¨Ç∑•§Â—°üi ª°∏˜–ñîa°e */
        print_list(file_ele[file_no], dlist, buf, mode);
        next_rcd(file_no, key_no, buf); /* ãa ¨Ç∑• ÆÖ¨·ù° */
      }               /* ∏aûaàa ∏·∏wñE ¢âü°∏‚ ÆÖ¨·µA òaúa ∏aûaüi ŒaØ°–eîa. */
      else if(seqrcd(file_no, buf) != DBERROR)  {
        print_list(file_ele[file_no], dlist, buf, mode);
      }
    }
    prn_func[mode]((mode == DATA2PRN ) ? "\f" : "\n\n", result);
  } while(errno != D_EOF);
  farfree(buf);
}

void db_print(int file_no, char *title, int data_ele[], int key_no, int mode)
{
  char *outfile, *msg;
  char *msg_str[] = {
    "  Ãa∑©ù° ¬âùb–aâ° ∑∂Øsì°îa  ",
    "  ∑•≠·–aâ° ∑∂Øsì°îa  "
  };
  char *end_msg[] = {
    "  Ãa∑©µA ∏·∏w–ñØsì°îa  ",
    "  ∑•≠·üi †a¡vØsì°îa  "
  };
  char *err_str[] = {
    "  –e∫â∑° ê·¢Å ã©¥·¨· Ãa∑©µA ∏·∏w–i ÆÅ ¥ÙØsì°îa  ",
    "  –e∫â∑° ê·¢Å ã©¥·¨· ∑•≠·–i ÆÅàa ¥ÙØsì°îa  "
  };
  int width;
  bool onoff;

  if(mode == DATA2FILE) {
    outfile = (char *)malloc(15);
    if(outfile == NULL) {
      memerror();
      return;
    }
    sprintf(outfile, "%s.TXT", dbfiles[file_no]);
    result = fopen(outfile, "wt");
    free(outfile);
    if(result == NULL) {
      disperror("  Ãa∑©∑i †eói ÆÅ ¥ÙØsì°îa  ");
      return;
    }
  }
  else {
    if(!check_printer())
      return;
  }
  init_browse(file_no, data_ele);
  msg = msg_str[(mode == DATA2FILE) ? 0 : 1];
  width = (strlen(msg)+4 > 10) ? strlen(msg)+4 : 10;
  if( !wopen(hgetmaxax()/2-strlen(msg)/2-1, hgetmaxay()/2-1, width, 3)) {
    if(mode == DATA2FILE)
      fclose(result);
    return;
  }
  wtitle(msg);
  onoff = isreverse();
  hsetreverse(ON);
  wcenter(2, " —¬∑• ");
  hsetreverse(onoff);
  if(get_width(els))
    print_all_list(file_no, title, key_no, els, mode);
  else {
    fclose(result);
    wclose();
    disperror(err_str[(mode == DATA2FILE) ? 0 : 1]);
    return;
  }
  fclose(result);
  wclose();
  msg = end_msg[(mode == DATA2FILE) ? 0 : 1];
  disp_cmsg_sec(msg, 1);
}
