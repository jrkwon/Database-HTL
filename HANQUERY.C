/* --------------------------- HANQUERY.C -------------------------------*/

/*
 *  �a�a�i ������
 *
 *  �A���A�� �� �������A ��A���ᴡ �a�e ���a�e �a�q�� �{�a.
 *
 *  1. �១�i �a�a�a���� ��ѡ
 *  2. �១�a���a �a�e �a�a�� �១�wȁ�i �iЁ�� ���e�A��
 *  3. �១�i �a�a�a����ѡ
 *  4. �a �a�a�a���A ���w�a�e ���b ��á
 *
 *  ���e�១�� �a �A�� �a�a ���b�A ���e ���e ���i �១Ё���a.
 *  �e��A �a�a�� �a���e �១�a�w�a�a.
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

static int file;	              /* <�a�a�i ������>�A�� �����e �e���i �a���� ��ѡ */
static int exitflag;	          /* �q�� �t�i �a���e �������i �aå�a.  */
static int existing_record;     /* ���e�A �a�a�a �����a�e �q */
static char *rb;	              /* �Aš�a ����; �e �a���A �����a�e �� ϩ�a�A ��Ё  */
static char *hb;	              /* �Aš�a�a �����A���e�� �·��a�����e ���A ����	  */
char *sc;	                      /* �����a�e �� ϩ�a�A ���e ���� �a���a �e���a�a��
				                           ��i�E ���a�A ��Ё��e, �b ���e ����	 */
static int len; 	              /* �Aš�a�� ���� */
int *els, *tmp_els;		          /* �a�a�a�� ���a�a */
static int list[MAXELE+1];      /* �a�a�a���i�� ���a�a�i ��w�i ���i */
static int prn_list[MAXELE+1];  /* �a�a�a���i �� ��Ȃ�E �a�a�a���i�i ��w�i ���i */
struct coord_t coord[MAXELE+1]; /* �a�a�e�� ���e�i�A�� ���a�a ��w�I �����A */
int no_ele;		                  /* �១�i �a�a�a���� ���� */
static FILE *result;            /* ����wз�i ���ዡ�� �a���g�� �a���� �a�e �w���i ��Ё */
void (*browse2update[MAXUPDATEFUNC])(int, char*);

extern byte picture[];  /* in HANSCRN.C */
extern int screen_displayed;

/*
 *			 >>   �e�� ǡ	<<
 *
 *  F2	    : ������ �Aš�a�a �e�w�E �Aš�a�i �a�a�i�A ��w
 *  F3	    : �� �a�a ���b
 *  F8	    : �Aš�a�i �b�A
 *  PgUp    : ���� �Aš�a
 *  PgDn    : �a�q �Aš�a
 *  ^Home   : �a�w ��q �Aš�a
 *  ^End    : �a�w �a�� �Aš�a
 *  Ins     : �s��/���� �a��
 *  Enter   : �a�q ϩ�a�� ����
 *  Down    : �a�q ϩ�a�� ����
 *  Tab     : �a�q ϩ�a�� ����
 *  Up	    : ���� ϩ�a�� ����
 *  ShiftTab: ���� ϩ�a�� ����
 *  F10     : �A���e�a�� ����
 *  ESC     : �aá��
 */

/* �����i �a�� */
static void query_help_func(void)
{
  if(wopen(hgetmaxax()/2-18, hgetmaxay()/2-5, 36, 15)) {
    hgotoxy(1, 1);
    wtitle("��  ��  �i");
    hputs("\n  F2    : �a�a ��wҁ �� �a�a���b\n");
    hputs("  F8    : �a�a ������\n");
    hputs("  PgUp  : ���� �a�a\n");
    hputs("  PgDn  : �a�q �a�a\n");
    hputs("  ^PgUp : �a�w ��q �a�a\n");
    hputs("  ^PgDn : �a�w �a�� �a�a\n");
    hputs("  Enter : �a�q �a���� ����\n");
    hputs("  Down  : �a�q �a���� ����\n");
    hputs("  Tab   : �a�q �a���� ����\n");
    hputs("  Up    : ���� �a���� ����\n");
    hputs("  #Tab  : ���� �a���� ����\n");
    hputs("  F10   : �A���e�a�� ����\n");
    hputs("  ESC   : �aá��\n");

    pause();
    wclose();
  }
}

/*
 * �a�a�a���i �e���i ���a ������
 * ǡ�a�a�a���� ��ѡ�i Ή�� ���e ����i �������a�� �����a.
 * �a���� �a�w �᷁ �a�a ��á�� �����e�a.
 */
static void clear_record(int file_no)
{
  int i = 0;

  while (index_ele [file_no][0][i])
    protect(index_ele[file_no][0][i++], FALSE);
  clrrcd(sc, els);
  existing_record = FALSE;
}

/* �Aš�a ���᷁ ���w�i ���e����� ���A����A ���a */
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

/* �� �Aš�a�i �a�a��a ���� ���e �a�a�i �����a��, �a�� �a�a�i�A ��w */
static void rcdout(void)
{
  if (empty(sc, len) == 0)  {
    rcd_fill(sc, rb, els, file_ele[file]);
    if (existing_record)  {
      if (same() == 0)	{
	      post_notice("  �a�a�i ��w�s���a  ");
	      if(rtn_rcd(file, rb) == DBERROR) {
          disp_cmsg("  �a�a�i ��w�i �� ���s���a  ");
        }
      }
    }
    else  {
      post_notice("  ������ �a�a�i ��w�s���a  ");
      if (add_rcd(file, rb) == DBERROR)
	      dberror();
    }
    clear_record(file);
  }
}

/* �e�a ��Ȃ�A�e �a �e��ѡ�i ���a����, �᭡�A�e -1�i ���a���a. */
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
    if(yesno("�����e �� �������� �i�a�a ?", 1) == 0) { /* �� 0, �a���� 1, �᭡ 2 */
      rcdin();
    }
    else if(existing_record)  {
      if(same() == 0)  {
	      post_notice("  �a�a�i ��w�s���a  ");
	      if(rtn_rcd(file, rb) == DBERROR) {
          disp_cmsg("  �a�a�i ��w�i �� ���s���a  ");
        }
      }
    }
  }
}

static void delete_func(void)
{
  if (spaces(sc)== 0)  {
    if(yesno("�� �a�a�i �����a�a ?", 1) == 0) { /* �� 0, �a���� 1, �᭡ 2 */
      del_rcd(file);
      clear_record(file);
    }
  }
}

static bool quit_func(void)
{
  if(yesno("�b��i �aá���V�s���a ?", 1) == 0) {  /* �� 0, �a���� 1, �᭡ 2 */
    return true;
  }
  return false;
}

static void prev_func(void)
{
  rcdout();
  if(prev_rcd(file, 1, rb) == DBERROR) {
    if (first_rcd(file, 1, rb) == DBERROR)  {
      post_notice("  �a�a�a �a�a�� ���s���a  ");
      return;
    }
    post_notice("  �a�w ��q �a�a�����a  ");
  }
  rcdin();
}

static void next_func(void)
{
  rcdout();
  if (next_rcd(file, 1, rb) == DBERROR)  {
    if (last_rcd(file, 1, rb) == DBERROR)  {
      post_notice("  �a�a�a �a�a�� ���s���a  ");
      return;
    }
    post_notice("  �a�w �a���b �a�a�����a  ");
  }
  rcdin();
}

static void go_first_func(void)
{
  rcdout();
  if (first_rcd(file, 1, rb) == DBERROR)
    post_notice("  �a�a�a �a�a�� ���s���a  ");
  else
    rcdin();
}

static void go_last_func(void)
{
  rcdout();
  if (last_rcd(file, 1, rb) == DBERROR)
    post_notice("  �a�a�a �a�a�� ���s���a  ");
  else
    rcdin();
}

/*
 *  �� �q���e �a�a�� ���b�� �����A�� ���� �a�w�A�e �q�����a.
 *
 *  �a�a ���b�� �a�a �����i �១. pkey�e ���i�A(primary key)�� ���a�i,
 *  mode�e ���b/���� �wȁ�a��, mode�a ���������e pkey�a �A�a
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
    clrrcd(sc, els);	     /* ���e����A �����i �����a */
  set_trap();		     /* ǡ �a�a�a���a ���b�E ҁ ѡ�i �q�� ���� */
  while (!exitflag)  {
    term = data_entry(pkey, file); /* ���e ���b�i�A�� ���A ���b�i �h�e ���� */
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

/* �Aš�a ����a ���᷶�a? �b �������a���e �A�� ���a? ���᷶�a�e �q */
static int empty(char *b, int l)
{
  while (l--)
    if (*b && *b != ' ')
      return FALSE;
    else
       b++;
  return TRUE;
}

/* �Aš�a ����� ���A����a �a�e�� ��a. �b, �Aš�a�a �����A���e�� */
static int same(void)
{
  int ln = rlen(file);

  while (--ln)
    if (*(rb + ln) != *(hb + ln))
      break;
  return (*(rb + ln) == *(hb + ln));
}

/* ����ǡ�a ���b�A���i �� �a���i �១�i �q���i ��� */
static void set_trap(void)
{
  int i = 0;

  while (index_ele [file] [0] [i])
    i++;
  edit(index_ele [file] [0] [i-1], key_entry);
}

/* ��ǡ�a ���b�A�� �a�� �a�� �� �q���a ѡ�E�a */
static int key_entry(char *s)
{
  char key [MAXKEYLEN];
  int i;

  if (spaces(s))   /* ���b�E �a�a�a ���a�e */
    return DBOK;
  *key = '\0';
  i = 0;
  while (index_ele [file] [0] [i])  {
    protect(index_ele[file][0][i],TRUE);
    strcat(key, sc + epos(index_ele[file][0][i++], els));
  }
  if (find_rcd(file, 1, key, rb) == DBERROR)  {
    post_notice("  ������ �a�a�����a  ");
    existing_record = FALSE;
    return DBOK;
  }
  rcdin();
  tally();
  return DBOK;
}

/* �១�i �a�a�� ���a�i �������e�a. */
static void init_coord(struct coord_t crd[])
{
  register i;

  for(i = 0; i < no_ele ; i++)
    coord[i].x = crd[i].x, coord[i].y = crd[i].y;
}

/*
 * �១�i �a�a�a���� ���a�i �����i ͡���� ���i list�A �q�e�a.
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
 * query �q�� �a�w�i ���e ������!
 * �����a�e 1, ��́�a�e 0�i ���a���a
 */

static int init_query(int file_no, char *title, int data_ele[], struct coord_t crd[])
{
  int count, i;

  file = file_no;
  count = init_data_ele(data_ele);
  if (count == 0)  {		      /* ���e �a�a�a���i �១�a�V�a. */
    len = rlen(file);
    els = file_ele [file_no];
    for(i = 0; els[i]; i++)	  /*  *file_ele[]�� �a���� ���� */
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
  init_rcd(file, rb);	   /* rb�i 'file'�� �����A �a�a �����a�� �����a. */
  init_rcd(file, hb);	   /* hb�i 'file'�� �����A �a�a �����a�� �����a. */
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
 *  �a�a ���b�i ���e �q��.
 *
 *  �� �������i ���a����A ���� ���e �ŝe �a���e �i�ᖁ�e ���a�� �a�a
 *  ���w�� �a���a�w���A��e �e���� �w���� �� �a�i�� ���a���e
 *  Ή�a�� �w�����a�e ���� ���e�e �w���� �E�a.
 *
 *  �a���a�� ��З�� ���A�A�� �b�� �a���i ���� �i��
 *  �a���a���i �{�aé�� �b�� �a���i �h�e �w���i ���a�a.
 *
 *  �a����ѡ�� ���A���A�e �� �q�� ���a����A �១Ё�� �e�a.
 *
 *  *title     : �១�i �a�a ���e�� �A��
 *  data_ele[] : �១�i �a�a�a���� ���� ���i
 *  crd[]      : �᷁ �a�a�a���A ���w�a�e �a���i�� ���b��á
 */

void db_input(int file_no, char *title, int data_ele[], struct coord_t crd[])
{
  if(init_query(file_no, title, data_ele, crd)) {
    query(NULL, DBINPUT);
    close_query(file_no);
  }
}

/*
 *  �a�a �x���i ���e �q��
 *
 *  *title     : �១�i �a�a ���e�� �A��
 *  data_ele[] : �១�i �a�a�a���� ���� ���i
 *  crd[]      : �᷁ �a�a�a���A ���w�a�e �a���i�� ���b��á
 *  keyno      : �i�A ��ѡ
 *  *keyvalue  : �x�a�a�e �a�a�� ���a�i
 */

void db_search(int file_no, char *title, int data_ele[], struct coord_t crd[], int keyno, char *keyvalue)
{
  if(init_query(file_no, title, data_ele, crd)) {
    file = file_no;
    if(find_rcd(file, keyno, keyvalue, rb) != DBERROR) {
      post_notice("  �a�a�i �x�v�s���a  ");
      rcdin();
      query(keyvalue, DBUPDATE);
    }
    else {
      disperror("  �a�� �a�a�e ���e�A�a.  ");
    }
    close_query(file_no);
  }
}

/*
 *  �i�a���� -- > �a�a �����i ���e �q��
 *
 *  *title     : �១�i �a�a ���e�� �A��
 *  data_ele[] : �១�i �a�a�a���� ���� ���i
 *  crd[]      : �᷁ �a�a�a���A ���w�a�e �a���i�� ���b��á
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
      disperror("  �a�� �a�a�e ���e�A�a.  ");
    }
    close_query(file_no);
  }
}

/*
 *  �a�a �i�a���� - ���e �����e �a�q�� �{�a.
 *
 *     +-----------------------------------------------------------+
 *   1 |   �A�A ����                                               |
 *     +-----------------------------------------------------------+
 *   2 |   ���e �A��                                               |
 *     +-----------------------------------------------------------+
 *   3 |   �b �a�a�a�� ���q                                        |
 *     +-----------------------------------------------------------+
 *     |                                                           |
 *     .                                                           .
 *     .                                                           .
 *     .                                                           .
 *     +-----------------------------------------------------------+
 *  24 |   �a�a�wȁ�i �a���e�a.  �� �a�a���� �e�� �a�a��ѡ �a��..  |
 *     +-----------------------------------------------------------+
 *  25 |                                                           |
 *     +-----------------------------------------------------------+
 */

#define  LISTLINES   (20)                    /* �i�a������ �� �� */
#define  PRNLINES    (50)                    /* �e �����A ����I �� �� */
#ifndef DISP_MODE
  #define DISP_MODE
  enum { REVERSE, NORMAL } disp_mode;          /* �e�� �a�� �� �e�寡ǩ �����a */
#endif

/* �����i �a�� */
static void list_help_func(void)
{
  if(wopen(hgetmaxax()/2-18, hgetmaxay()/2-5, 36, 13)) {
    hgotoxy(1, 1);
    wtitle("Ԅ  ��  ��  �i  Ԅ");
    hputs("\n  Del   : �a�a ������\n");
    hputs("  Ԙ    : ���� �a�a\n");
    hputs("  ԙ    : �a�q �a�a\n");
    hputs("  ^Home : ���e �A�� ��\n");
    hputs("  ^End  : ���e �A�� �a��\n");
    hputs("  Enter : �� �a�a �e��\n");
    hputs("  Ԛ    : ���e�� ���e ����\n");
    hputs("  ԛ    : �E�� ���e ����\n");
    hputs("  F10   : �a���a��..\n");
    hputs("  ESC   : �a���a��..\n");
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
      post_notice("  �a�a�a �a�a�� ���s���a  ");
    }
    else {
      post_notice("  �a�w ��q �a�a�����a  ");
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
      post_notice("  �a�a�a �a�a�� ���s���a  ");
    }
    else {
      post_notice("  �a�w �a���b �a�a�����a  ");
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
    post_notice("  �a�a�a �a�a�� ���s���a  ");
  }
  return flag;
}

static int go_last_list(int file_no, int key_no, char *buf)
{
  int flag;

  flag = 1;
  if (last_rcd(file_no, key_no, buf) == DBERROR) {
    flag = 0;
    post_notice("  �a�a�a �a�a�� ���s���a  ");
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
    width = headlen(*dlist);  /* �a�a �i�� ������ �e�i�a�a�a�� ���q�� �� �� */
    if(line_width+width+1 > CARDWIDTH)         /* ���e �a�����a �ỡ�e �a�e !!! */
      break;
    hputsxy(line_width + 1, -TITLE_Y-1, hdenames[*dlist-1]);
    line_width += width + 1;
    dlist++;
  }
  hsetshadow(onoff);
}

static bool delete_list(int file_no)
{
  if(yesno("�� �a�a�i �����a�a ?", 1) == 0) { /* �� 0, �a���� 1, �᭡ 2 */
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
    if(key_no) {                      /* ������ѡ�i ����Ж�a�e */
      disp_list(y, file_ele[file_no], dlist, buf, offset, NORMAL);
      next_rcd(file_no, key_no, buf); /* �a ���� ���ᝡ */
    }               /* �a�a�a ��w�E ������ ����A �a�a �a�a�i �a���e�a. */
    else if(seqrcd(file_no, buf) != DBERROR)  {
      disp_list(y, file_ele[file_no], dlist, buf, offset, NORMAL);
    }
  }
  for(; y > (errno == D_EOF ? 2 : 1); y--)
    prev_rcd(file_no, key_no, buf);
}

static void disp_foot(void)
{
  char *str = "  Ԟ-����   ԟ-�a�q   ^Home-���|   ^End-����   ��-�e��  ԑ-Ԑ-���e����";
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
    post_notice("  �a�a�a �a�a�� ���s���a  ");
    return;
  }
  sethwindow(1, 1, hgetmaxax(), hgetmaxay());
  drawbox(1, TITLE_Y, hgetmaxax(), hgetmaxay()-2, 1);
  onoff = isreverse();
  hsetreverse(ON);
  eputchs(1, -TITLE_Y, hgetmaxax(), ' ');
  hputsxy(-(hgetmaxax()/2-strlen(title)/2), -TITLE_Y, title);
  hsetreverse(onoff);

  /* �����i �w ��� */
  sethwindow(2, TITLE_Y+2, hgetmaxax()-1, BUTTON_Y-1);
  hclrscr();
  if(key_no)                          /* ������ѡ�i ����Ж�a�e */
    first_rcd(file_no, key_no, buf);  /* �a�a�� �A�� ��q�A�� ���b */
  disp_header(dlist);                 /* �i�a�� �b �a�a�a�� ���q�i �a�� */
  disp_foot();
  isquit = offset = 0;
  disp_page(key_no, file_no, dlist, buf, offset);
  y = 1;
  do {
    errno = 0;            /*  ���A��ѡ ������ */
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
          post_notice(" �E�� �{�����a ");
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
          post_notice(" ���e�� �{�����a ");
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

/* b�e ���e ����A�� �e�� ϩ�a�� ��á  ͡����, �a�a �a���i ���e�A �b */
static void disp_field(char *b, char *msk, int el)
{
   char merge[80];
   register i = 0, gap = 0;

   if(eltype[el-1] == 'G') { /* �a�� �a���� �w�� */
     hputs("*�a��*");
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
     gap = strlen(hdenames[el-1])-strlen(merge);
     gap = gap > 0 ? gap : 0;
     hgotoxy(hwherex()+gap+1, hwherey());
   }
}

/*
 *  �a�a�i �e�� �a���e�a.
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
  while(*(dlist+offset)) {   /* �a�a�����A ���e �a�a �a�� �����e�q */
    mask_ptr = elmask[(*(dlist+offset))-1];
    width = headlen(*(dlist+offset));  /* �a�a �i�� ������ �e�i�a�a�a�� ���q�� �� �� */
    line_width += width + 1;
    if(line_width > CARDWIDTH)         /* ���e �a�����a �ỡ�e �a�e !!! */
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
 *  �i�a���� ������
 */

static void init_browse(int file_no, int data_ele[])
{
  int count, i;

  tmp_els = els;
  count = init_data_ele(data_ele);
  if (count == 0)  {		      /* ���e �a�a�a���i �១�a�V�a. */
    len = rlen(file_no);
    els = file_ele [file_no];
    for(i = 0; els[i]; i++)	  /*  *file_ele[]�� �a���� ���� */
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
 *  ���b�q���� ���e ����a ���a���g�e�a ��a�a����...
 *
 *  key_no : 0 ���e �a�a�a ��w�E ���ᝡ
 *           1 ���w�� ���a�e �a ������ѡ�A �a�a ���� ���a�� ..
 *
 *  �������� ���i �i�ᥡ�e
 *  1) �a�q���a�a 93.12.XX �� �a�q�i�i ���q ���a�� �������a !
 *  2) ���� '��'�� �a�q�i�i �����w����ѡ ���a�� �������a !
 *  �a���a�e ϩ�a�e ���a�i�e �a�q�� �{�a.
 *  . �b�� ���w �a����ѡ
 *  . �b�����e�� �A��
 *  . �b�󔁬w�� �A�e �a�a �a�� ����
 *  . ���i�a���a�a�e �i�A��ѡ
 *  . ���aš�a�a�e �a����ѡ
 *  . ���a�i ���a�i
 *  . ���a�i�A�� �ᗡ�A�� �ᗡ�a���i ���a�i �����a..
 */

char *db_browse(int file_no, char *title, int data_ele[], int key_no, int mode)
{
  char *ptr;

  hallowautoscroll(false);   /* �a���a�a�� �q�� */
  init_browse(file_no, data_ele);
  ptr = db_list(file_no, title, key_no, els, mode);
  close_browse();
  return ptr;
}

/*
 *   ���ᦁ
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
      if(retry("�a����a ��a ����a, �e�i�A�᷶�� �g�s���a", 0) != 0)
        return false;
    }
    else
      return true;
  }
}

int *get_print_list(int file_no, char *title, int data_ele[], struct coord_t crd[])
{
  int count, len, i, j;
  int ele_list[MAXELE+1] = { 0, }; /* ������ */

  count = init_data_ele(data_ele);
  if (count == 0)  {		      /* ���e �a�a�a���i �១�a�V�a. */
    len = rlen(file);
    els = file_ele [file_no];
    for(i = 0; els[i]; i++)	  /*  *file_ele[]�� �a���� ���� */
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

  /* ���e ���e�A ����I ͢�i ��Ё���a */
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
  /* �a�a �A���i �a�a */
  gap = line_width/2-strlen(title)/2;
  while(gap-- > 0)
    prn_func[mode](" ", result);
  prn_func[mode](title, result);
  prn_func[mode](mode == DATA2FILE ? "\n\n" : "\n\r\n\r", result);

  /* ���� �u�� */
  dlist = tmp;
  line_width = 0; /* ���� ���q �e�a���e ��ө �a�a�� �����i ���� �i����Ё ϩ�a */
  while(*dlist) {
    width = headlen(*dlist);  /* �a�a �i�� ������ �e�i�a�a�a�� ���q�� �� �� */
    if(line_width+width+1 >= PAPERWIDTH)         /* ���� �a�����a �ỡ�e �a�e !!! */
      break;
    while(width-- >= 0)
      prn_func[mode]("-", result);
    dlist++;
  }
  prn_func[mode](mode == DATA2FILE ? "\n" : "\n\r", result);

  /* �b �a�a�a���� ���q �a�� */
  dlist = tmp;
  line_width = 0;
  while(*dlist) {
    width = headlen(*dlist);  /* �a�a �i�� ������ �e�i�a�a�a�� ���q�� �� �� */
    if(line_width+width+1 >= PAPERWIDTH)         /* ���� �a�����a �ỡ�e �a�e !!! */
      break;
    prn_func[mode](hdenames[*dlist-1], result);
    gap = width - strlen(hdenames[*dlist-1]) + 1;
    while(gap-- > 0)
      prn_func[mode](" ", result);
    dlist++;
  }
  prn_func[mode](mode == DATA2FILE ? "\n" : "\n\r", result);

  /* �a�� �� �u�� */
  dlist = tmp;
  line_width = 0;
  while(*dlist) {
    width = headlen(*dlist);  /* �a�a �i�� ������ �e�i�a�a�a�� ���q�� �� �� */
    if(line_width+width+1 >= PAPERWIDTH)         /* ���� �a�����a �ỡ�e �a�e !!! */
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

   if(eltype[el-1] == 'G') { /* �a�� �a���� �w�� */
     prn_func[mode]("*�a��*", result);
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
  while(*dlist) {   /* �a�a�����A ���e �a�a �a�� �����e�q */
    mask_ptr = elmask[(*dlist)-1];
    width = headlen(*dlist);  /* �a�a �i�� ������ �e�i�a�a�a�� ���q�� �� �� */
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
  if(key_no)                          /* ������ѡ�i ����Ж�a�e */
    first_rcd(file_no, key_no, buf);  /* �a�a�� �A�� ��q�A�� ���b */

  do {
    print_header(dlist, title, mode);   /* �i�a�� �b �a�a�a�� ���q�i �a�� */
    for(lines = 0; errno != D_EOF && lines <= PRNLINES; lines++) {
      if(key_no) {                      /* ������ѡ�i ����Ж�a�e */
        print_list(file_ele[file_no], dlist, buf, mode);
        next_rcd(file_no, key_no, buf); /* �a ���� ���ᝡ */
      }               /* �a�a�a ��w�E ������ ����A �a�a �a�a�i �a���e�a. */
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
    "  �a���� �b�a�� ���s���a  ",
    "  �����a�� ���s���a  "
  };
  char *end_msg[] = {
    "  �a���A ��wЖ�s���a  ",
    "  ����i �a�v�s���a  "
  };
  char *err_str[] = {
    "  �e���� �ᢁ ����� �a���A ��w�i �� ���s���a  ",
    "  �e���� �ᢁ ����� �����i ���a ���s���a  "
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
      disperror("  �a���i �e�i �� ���s���a  ");
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
  wcenter(2, " �·� ");
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