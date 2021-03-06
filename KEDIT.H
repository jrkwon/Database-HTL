/*
 *  �a��   : kedit.h
 *  ���w   : �e���� ���A�A ͡�q�A�ᴡ �a�e ����
 *  ���B   : hantle.prj, nal.prj
 *  �i�a   : 93.03.16
 *  �e�e�� : �e�����b�a ��aɷ�����b�� �e�i�a�� �����b
 *  �i���i : �� Ή�a���� ���a���e �����i ��â�a�� �s���a.
 *           �ᐡ�a�e, �a�� �a�e ���a���� �����i �g�� �h�v�a���a�a.
 *           �a�a��, �� ���a���i �h�a�� ���e �a�e�A �a�w�a�a�� �A���e
 *           ���� �e�e ���a���� �i �����a�a�� ����Ё ������ �a�s���a.
 */

#include "typedefs.h"

#ifndef _KEDIT_H
#define _KEDIT_H

/*
 * �a�� �b�w�A�� �a�� �� ���e �a���� �A�� ��
 */
#define  MAX_FILES          (400)          /* �a���w�a�A�� �a�� �� ���e �a�� �� */
#define  MAXLINEBUF         (255)          /* �e���� �A�� �e���a�w ���� */
#define  DEFAULTBUFSIZE     (57200L)       /* ���A �e���w���� �A���a��  */
#define  MAXBLOCKSIZE       (2000)         /* �i��a�� �a�� �� ���e �A���a�� */

/*
 *  �e���w�� �a���e �A�� (5 x 3), �A�� (80 x 23)
 */
#define  MAX_SCR_COL    80
#define  MAX_SCR_LINE   23
#define  MIN_SCR_COL     5
#define  MIN_SCR_LINE    3

#define  HE_2LARGE  36   /* �a���� �ᢁ ��a */
#define  HE_NOFIND  37   /* �a�� ���a�i�e ���s���a */
#define  HE_ILFILE  38   /* �a���� �����a��a �i���E ���q�����a */
#define  HE_NOEXIST 39   /* �� ���w ���s���a */

enum yesnocan_t { YES_ANS, NO_ANS, CANCEL_ANS };
enum yesno_t { NO, YES };
enum loadmode_t { INIT_LOAD, BLOCK_LOAD };
enum display_mode { REVERSE, NORMAL };

/*
 *  ǡ ���b �����a�� �a�� ���a�ַi ��, ���� Ё�� �i ��
 *  �b, ǡ ���b�� ���i �� ��� ���i �i �����a.
 */
#define  TASK_DISPLAYPAGE      0x0100    /* �� ���e �a�� �b */
#define  TASK_SCROLLUPPAGE     0x0040    /* ���A �ᝡ ���e �a�a�� */
#define  TASK_SCROLLUP         0x0020    /* �e�� ��� �a�� �����e �ᝡ �e�e �a�a�� */
#define  TASK_SCROLLDOWN       0x0010    /* �e�� ��� �a�� �����e �a���� �e�e �a�a�� */
#define  TASK_DISPLAYUPLINE    0x0008    /* �e�� �ᝡ ����Ё�� �a ���i �a�� �a�� */
#define  TASK_DISPLAYDOWNLINE  0x0004    /* �e�� �a���� ����Ё�� �a ���i �a�� �a�� */
#define  TASK_DISPLAYBOTLINE   0x0002    /* �� �a���� �a�� */
#define  TASK_DISPLAYLINE      0x0001    /* �e�� �� �a�� �a�� */


extern unsigned char *memory;                   /* ���b�i �� �e���i �e�q�� ���e�i �i�w�h�e�a */
extern unsigned char *line_start;               /* ��a �e ���� ���b ͡���� */
extern unsigned char *current;                  /* �e�� �e�� ���� ��á */
extern unsigned char *last_pos;                 /* �a�� ���� ��á */
extern unsigned char *b_b_ptr;                  /* �i�ⷁ ���b ͡���� */
extern unsigned char *b_k_ptr;                  /* �i�ⷁ �{ ͡���� */
extern unsigned char *eofm;                     /* �a���� �a���b */
extern unsigned char work_file[80];             /* �b�� �a���� ���q */
extern unsigned char block_file[80];            /* �i��a�� ��w/���ᵡ�� �a���� ���q */
extern unsigned char help_file[80];
extern unsigned char write_file[80];
extern unsigned char line_buffer[MAXLINEBUF+1]; /* �e�� �e�� ���� ���� ���� �b�� �w�� */

extern bool insert_mode, indent_mode, saved, block_display;
extern unsigned int curx, cury, user_task, max_line, base_y, base_x, block_size;
extern long maxeditbuf;

/*
 *  �a�q�e �e������ ���wǡ(��w, ���ᵡ��)�A ���e �����t���a.
 *  �e, �e�w ɡ�iǡ�e ������a�� SHIFT-SPACE����, ���A�A �a�e ǡ�� �w���i �� ���a.
 */
extern int save_file_key       ;   /* Save : ��w�a�� */
extern int print_file_key      ;   /* Print : �����a�� */
extern int help_key            ;   /* �����i ǡ */
extern int han_toggle_key      ;   /* �e/�w �a��ǡ */
extern int hanja_conv_key      ;   /* �e�i->�e�aǡ */
extern int kdb_toggle_key      ;   /* 3�鯢/2�鯢 �a��ǡ */
extern int ks_graph_key        ;   /* KS 2�a���a �a�����a ���b */
extern int tg_graph_key        ;   /* TG 2�a���a �a�����a ���b */
extern int menu_key            ;   /* pulldown menu */
extern int quit_key            ;   /* ��w�i ������ ����, �e���i �{���� */
extern char file_ext[]         ;   /* file extension */
extern char init_file_name[]   ;   /* ���� �a�� ���q - �¸w�a ���� */

extern bool hanja_font_exist;      /* �e�a �a���� �����a�e��.. */
extern bool all_jobs_close;        /* ���e �b�� ���a �A���e�� ... */

extern void (*menu_process)(int key);         /* for the main menu process */

void block_decision(unsigned char **block_ptr);
void block_copy(void);
void block_move(void);
void block_delete(void);
void block_write(void);
void block_read(void);
void to_pointer(unsigned char *destin);
void find_condition(int replace);
void writef(int sx, int ex, int y, unsigned char *msg, int mode);
void errorf(void);
bool set_tabsize(int size);
void set_max_width_height(int width, int height);
void draw_border(int x, int y, int width, int height, int border);
int  get_tabsize(void);
void init_editor_variable(void);
bool init_editor(char *filename, char *help_filename, long bufsize, int kdb_kind, bool han_eng);
bool editor(int x, int y, int width, int height, int border);
unsigned int line_count(unsigned char *start, unsigned char *finish);
void disp_kbd_status(void);
void disp_indent_status(void);
void disp_file_status(void);
void disp_status(void);

#endif

/*
 *  �a�� kedit.h �� �{
 */
