/*
 *  �a���a��  : �e�i�a���a�១
 *  ���� ���q : HANMENU.H
 *  �e�e ��   : �� �� �b
 *  �i�a      : 1992. 8. 16
 *  ���� ���w : �A�A �ŝe �១ ����
 */

#if !defined(__HANMENU)
#define __HANMENU

#if __STDC__
#define _Cdecl
#else
#define _Cdecl cdecl
#endif

#define  MAXMENU    10   /* �A�� �a���a ���� */
#define  MAXITEM    16   /* �e �a���a�A�� �A�� �w�� ���� */
#define  MAXLENGTH  20   /* �e �a���a ���q�� �A������ ? */

#define  getcurmenu()   (themenu)
#define  getcuritem()   (theitem)

typedef struct {
	unsigned char	*menu;                    /* �a���a ���q */
	unsigned char nitem;                    /* �w�� ���� */
	unsigned char	*item[MAXITEM];           /* �w�� ���q */
	unsigned int  menuable;                 /* �a���a �a�w �a�� */
	unsigned int  itemable[MAXITEM];        /* �w�� �a�w �a�� */
	unsigned int  shortcut[MAXITEM];        /* �e �i�A */
	char  shortcutchar[MAXITEM][4];         /* �a���a�A �a�a�i �e �i�A�� ���a�i */
	unsigned int  top, left, width, height; /* �a���a�� ���a */
	unsigned int  menuwidth;                /* �a���a�� �췡 */
  unsigned int  itemwidth;                /* �w���� �췡 */
} HMENU;

extern int themenu, theitem, nmenu;
extern HMENU pmenu[MAXMENU];

void _Cdecl definemenu(char fcolor, char bcolor, int init_menu, int gap);
void _Cdecl drawmenubar(void);
int _Cdecl getitem(unsigned int keycode);

#endif

/* HANMENU.H ������ �{ */