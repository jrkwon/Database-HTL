/*
 *  ÐeËi´aŸ¡ ¸ažaËi  áŸ¡ Ìa·©
 *  ·¡ ¸ažaËiµA¬á ˆa¸w ‹eˆe·¡ –A“e  áŸ¡ Ìa·©
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

#define NODE 512     	   /* B-ËaŸ¡ ‘¡—a· ‹©·¡  */
#define RPTR long		     /* B-ËaŸ¡ ‘¡—a¤åÑ¡µÁ ¸ažaÌa·©· ¸aža¤åÑ¡· Ñw
                            longÑw·¡¡e 2· 32¯wˆŒa»¡ ˆa“w */

#define MAXFILS   20	     /* ¸ažaËi´e· ÂA” Ìa·© ® */
#define MAXELE    100	     /* Ðe Ìa·©´e· ÂA” ¸aža ¶a­¡® */
#define MAXINDEX  20       /* Ìa·©”w ÂA” ·¥•B¯a ®		*/

#define MAXUPDATEFUNC  20  /* browse -> update µA¬á ¦‰Ÿ© Ðq®· ÂA”ˆ•® */

/* »©·¦· ‹¡“wÇ¡ ¸÷· */
#define HELPKEY    F1        /* •¡¶‘ i */
#define MENUKEY    F10       /* ÀaŸ±Îa */
#define SAVEKEY    F2        /* ¸aža ¸á¸w || ¬åÈ‚µÅža */
#define UNDOKEY    F5        /* ¸ažaŸi ¶¥œ”¡ */
#define DELETEKEY  F8        /* ¸aža »¡¶‹¡ */
#define CANCELKEY  ESC       /* Âá­¡   */
#define QUITKEY    ESC       /* {‹¡ */
#define SWITCHKEY  ESC       /* ¹A´áÌe‰Á Íe»³Ìeˆe· ¸åÑÅ */

/* »©·¦· ÑÁ¡e Š¬÷ ¸÷· */
#define BUTTON_GAP  (2)         /* ¹A´áÌeµA ·¶“e ˆb ”eÂ ¬a·¡· ˆáŸ¡ */
#define BUTTON_CTR  (8)         /* ¹A´áÌeµA ·¶“e ”eÂ· ˆ•® */
#define BUTTON_Y    (24)        /* ”eÂ—i· y¹ÁÎa */
#define TITLE_Y     (2)         /* »©·¦· ÑÁ¡e ¹A¡¢· y¹ÁÎa */
#define CARDWIDTH   (78)        /* ÑÁ¡eµA¬á ÂA” ì·¡ */

/* ¸i¡µ¤åÑ¡(errno)Ÿi •©aº‹¡ ¶áÐe ¸ažaËi ¸i¡µ ¦Ñ¡ */
#define D_NF	    1	       /* record not found; AÅ¡—a ¡µ Àx·q */
#define D_PRIOR	  2	       /* no prior record for this request;
                              ·¡ ¶aŠŸi ¶áÐ ´è´á–… AÅ¡—a ´ô·q */
#define D_EOF	    3	       /* end of file; Ìa·© {	*/
#define D_BOF	    4	       /* beginning of file; Ìa·©· ¯¡¸b  */
#define D_DUPL	  5	       /* primary key already exists; ºÇ¡ˆa ·¡£¡ ¹¥¸ */
#define D_OM	    6	       /* out of memory; ‹¡´â­¡ ¦¹¢ */
#define D_INDXC   7        /* index corrupted; ·¥•B¯a Œ»± */
#define D_IOERR	  8        /* i/o error; ·³Â‰b ¸i¡µ */

#define MAXKEYLEN 80	     /* ·¥•B¯aŸi ¶áÐe ÂA” Ç¡ ‹©·¡ */

/* qd(»©·Ðq®)· ¬a¶wµA¬á ‹a ¶w•¡Ÿi ¤jÓ¡‹¡ ¶áÐ */
#define DBUPDATE  0        /* ®¸÷ ¬wÈ */
#define DBINPUT   1        /* ·³b ¬wÈ */

/* browse· ¬wÈ
  (1) Browse mode --> Update
  (2) Browse mode --> Ref */
#define B2UPDATE  0
#define B2REF     1

#ifndef HANSCHEMA

/* ·w¶wÎ‰‹aŸ±·i ¶áÐ ŠÂ‚–E ¯aÇ¡ a  */
extern char *dbfiles [];		/* ¸ažaËi Ìa·©·¡Ÿq */
extern char *denames [];		/* µw¢… ¸aža¶a­¡ ·¡Ÿq */
extern char *hdenames [];		/* Ðe‹i ¸aža¶a­¡ ·¡Ÿq */
extern char *elmask  [];		/* ¸aža¶a­¡  a¯aÇa */
extern char eltype [];			/* ¸aža¶a­¡ Ñw */
extern int ellen [];			  /* ¸aža¶a­¡· ‹©·¡ */
extern int *file_ele [];		/* Ìa·©µA Í¡Ðq–E ¸aža¶a­¡· ¤åÑ¡ */
extern int **index_ele [];	/* ·¥•B¯a–E ¸aža¶a­¡ ¤åÑ¡ */

extern void (*browse2update[])(int, char*);

/* ¸ažaÀáŸ¡Ÿi ¶áÐe ¤áÌá */
extern char *rb, *sc;

/* ¸ažaÌa·©·  áŸ¡ */
typedef struct fhdr	{
	RPTR first_record;
	RPTR next_record;
	int record_length;
} FHEADER;

/* ·³b¤h·i ¸aža¶a­¡· ¹ÁÎa */
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