/*
 *  �e�i�a�� �a�a�i �១ �a��
 *  �� �a�a�i�A�� �a�w �e�e�� �A�e �១ �a��
 *  handbms.h
 */

#ifndef DBERROR
#define DBERROR   -1
#define DBOK       0
#endif

#ifndef TRUE
#define TRUE     1
#define FALSE    0
#endif

#include <ctype.h>
#include "typedefs.h"

extern int errno;

#include <sys\stat.h>
#include <fcntl.h>
#include <io.h>

#define OPENMODE  O_RDWR+O_BINARY
#define CMODE     S_IWRITE

#define NODE 512     	   /* B-�a�� ���a�� ����  */
#define RPTR long		     /* B-�a�� ���a��ѡ�� �a�a�a���� �a�a��ѡ�� �w
                            long�w���e 2�� 32�w���a�� �a�w */

#define MAXFILS   20	     /* �a�a�i�e�� �A�� �a�� �� */
#define MAXELE    100	     /* �e �a���e�� �A�� �a�a �a���� */
#define MAXINDEX  20       /* �a���w �A�� ���B�a ��		*/

#define MAXUPDATEFUNC  20  /* browse -> update �A�� ���� �q���� �A������ */

/* �������� ���wǡ ���� */
#define HELPKEY    F1        /* �����i */
#define MENUKEY    F10       /* �a���a */
#define SAVEKEY    F2        /* �a�a ��w || ��Ȃ�Şa */
#define UNDOKEY    F5        /* �a�a�i �������� */
#define DELETEKEY  F8        /* �a�a ������ */
#define CANCELKEY  ESC       /* �᭡   */
#define QUITKEY    ESC       /* �{���� */
#define SWITCHKEY  ESC       /* �A���e�� �e���e�e�� ���� */

/* �������� ���e ���� ���� */
#define BUTTON_GAP  (2)         /* �A���e�A ���e �b �e �a���� �១ */
#define BUTTON_CTR  (8)         /* �A���e�A ���e �e�� ���� */
#define BUTTON_Y    (24)        /* �e�i�� y���a */
#define TITLE_Y     (2)         /* �������� ���e �A���� y���a */
#define CARDWIDTH   (78)        /* ���e�A�� �A�� �췡 */

/* �i����ѡ(errno)�i ���a���� ���e �a�a�i �i�� ��ѡ */
#define D_NF	    1	       /* record not found; �Aš�a �� �x�q */
#define D_PRIOR	  2	       /* no prior record for this request;
                              �� �a���i ��Ё ��ᖅ �Aš�a ���q */
#define D_EOF	    3	       /* end of file; �a�� �{	*/
#define D_BOF	    4	       /* beginning of file; �a���� ���b  */
#define D_DUPL	  5	       /* primary key already exists; ��ǡ�a ���� ���� */
#define D_OM	    6	       /* out of memory; ���⭡ ���� */
#define D_INDXC   7        /* index corrupted; ���B�a ���� */
#define D_IOERR	  8        /* i/o error; ���b �i�� */

#define MAXKEYLEN 80	     /* ���B�a�i ���e �A�� ǡ ���� */

/* qd(�����q��)�� �a�w�A�� �a �w���i �jӡ�� ��Ё */
#define DBUPDATE  0        /* ���� �wȁ */
#define DBINPUT   1        /* ���b �wȁ */

/* browse�� �wȁ
  (1) Browse mode --> Update
  (2) Browse mode --> Ref */
#define B2UPDATE  0
#define B2REF     1

#ifndef HANSCHEMA

/* �w�wΉ�a���i ��Ё ���E �aǡ�a  */
extern char *dbfiles [];		/* �a�a�i �a�����q */
extern char *denames [];		/* �w�� �a�a�a�� ���q */
extern char *hdenames [];		/* �e�i �a�a�a�� ���q */
extern char *elmask  [];		/* �a�a�a�� �a�a�a */
extern char eltype [];			/* �a�a�a�� �w */
extern int ellen [];			  /* �a�a�a���� ���� */
extern int *file_ele [];		/* �a���A ͡�q�E �a�a�a���� ��ѡ */
extern int **index_ele [];	/* ���B�a�E �a�a�a�� ��ѡ */

extern void (*browse2update[])(int, char*);

/* �a�a�១�i ���e ���� */
extern char *rb, *sc;

/* �a�a�a���� �១ */
typedef struct fhdr	{
	RPTR first_record;
	RPTR next_record;
	int record_length;
} FHEADER;

/* ���b�h�i �a�a�a���� ���a */
struct coord_t {
  int x, y;
};

void mov_mem(), set_mem(), fatal();
void cls_file();
void build_b();
void put_char(), clear_screen(), cursor();
void error_message(), clear_notice(), post_notice();

/* ------------------ handuitl.c --------------------- */
int db_init(void);

/* ------------------ hanstr.c ---------------------- */
char *str_trim(char *str);
char *ins_left_space(char *str, int len);
char *ins_left_zero(char *str, int len);
char *ins_right_space(char *str, int len);

/* ------------------ handfile.c -----------------------*/
int file_create(char *name, int len);
int file_open(char *name);
void file_close(int fp);
RPTR new_record(int fp, char *buf);
int get_record(int fp, RPTR rcdno, char *buf);
int put_record(int fp, RPTR rcdno, char *buf);
int delete_record(int fp, RPTR rcdno);

/* ------------------- hanbtree.c ----------------------*/
int btree_init(char *ndx_name);
int btree_close(int tree);
void build_b(char *name, int len);
RPTR locate(int tree, char *k);
int deletekey(int  tree, char *x, RPTR ad);
int insertkey(int tree, char *x, RPTR ad, int unique);
RPTR nextkey(int tree);
RPTR prevkey(int tree);
RPTR firstkey(int tree);
RPTR lastkey(int tree);
void keyval(int tree, char *ky);
RPTR currkey(int tree);

/* -------------------- handbase.c ---------------------*/
void db_open(char *path, int *fl);
int add_rcd(int fileno, char *buf);
int find_rcd(int fileno, int keyno, char *keyvalue, char *buf);
int verify_rcd(int fileno, int keyno, char *keyvalue);
int first_rcd(int fileno, int keyno, char *buf);
int last_rcd(int fileno, int keyno, char *buf);
int next_rcd(int fileno, int keyno, char *buf);
int prev_rcd(int fileno, int keyno, char *buf);
int rtn_rcd(int fileno, char *buf);
int del_rcd(int fileno);
int curr_rcd(int fileno, int keyno, char *buf);
int seqrcd(int fileno, RPTR *buf);
void db_cls(void);
int rlen(int fileno);
void clrrcd(char *buf, int *els);
void init_rcd(int fileno, char *buf);
void rcd_fill(char *sourse, char *destin, int *sourcelist, int *destinlist);
int epos(int elementno, int *list);
void dberror(void);
void mov_mem(char *s, char *d, int l);
void set_mem(char *s, int l, char n) ;

void rebuild_index(int file_no, int allflag);
void build_index(char *path, int fileno);
int add_indexes(int fileno, char *buf, RPTR ad);
int filename(char *fn);
void name_cvt(char *c2, char *c1);

/*---------------------------- hanquery.c ------------------------- */
void db_input(int file_no, char *title, int data_ele[], struct coord_t crd[]);
void db_search(int file_no, char *title, int data_ele[], struct coord_t crd[], int keyno, char *keyvalue);
void db_update(int file_no, char *title, int data_ele[], struct coord_t crd[], int key, char *buf);
void db_delete(int file_no, char *title, int data_ele[], struct coord_t crd[], int keyno, char *keyvalue);
char *db_browse(int file_no, char *title, int data_ele[], int key_no, int mode);
int *get_print_list(int file_no, char *title, int data_ele[], struct coord_t crd[]);
void db_print(int file_no, char *title, int data_ele[], int key_no, int mode);

/* ---------------------------- hanscrn.c --------------------------- */
void post_notice(char *s);
void clear_scrn(void);
void init_screen(char *name, int *els, char *bfr);
void protect(int el, int tf);
void edit(int el, int (*func)());
void mark_pannel(int butt_ctr);
void display_template(char *title, int menu_flag);
bool hookhgetdata(int *keycodep);
int data_entry(char *pkey, int file);
int *get_ele_list(int ele_list[]);
char *del_comma(char *str);
char *ins_comma(char *str);
byte *pickoutdata(byte *buffer, byte *format, byte *data);
int spaces(char *c);
void tally(void);
void put_field(int el);

#endif

