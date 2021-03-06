/*
 *  �a���a��  : �e�i�a���a�១
 *  ���� ���q : HANMENU.C
 *  �e�e ��   : �� �� �b
 *  �i�a      : 1992. 8. 16
 *  ���� ���w : �i�a���� �a���a �ŝe ����
 */

#include <stdio.h>
#include <alloc.h>
#include <ctype.h>
#include <string.h>
#include "hanmenu.h"
#include "hanwindw.h"
#include "hancolor.h"
#include "hanin.h"
#include "ascii.h"
#include "extkey.h"


/*
 *  �a���a(menu)�e �a���wз, �w��(item)�e �A�� �wз
 */
int nmenu;                      /* �a���a ���� */
int themenu, theitem;           /* ��Ȃ�E �a���a�� �w����ѡ */
static int curmenu;             /* �e�� �a���a */
static int curitem[MAXMENU];    /* �e�� �w�� */
static int ispulldown;          /* �i�a�����a���a(pull-down menu)�a �a���A�᷶�a? */
static char menuflag, itemflag; /* �a���a�� �w�� �e��i ���e �i���a */
static unsigned char menufcolor, menubcolor;

HMENU pmenu[MAXMENU];

static int _Cdecl maxitemlength(int curmenu_num);
void _Cdecl definemenu(char fcolor, char bcolor, int init_menu, int gap);
void _Cdecl drawmenubar(void);
static void _Cdecl highlightmenu(void);
static void _Cdecl highlightitem(void);
/*static bool _Cdecl selectmenu(int menunum);*/
/*static bool _Cdecl selectitem(int itemnum);*/
static void _Cdecl erasemenubox(void);
static bool _Cdecl findshortcut(unsigned int keycode);
static bool _Cdecl drawmenubox(void);
static bool _Cdecl beforemenu(void);
static bool _Cdecl nextmenu(void);
static void _Cdecl beforeitem(void);
static void _Cdecl nextitem(void);
static void _Cdecl firstitem(void);
static void _Cdecl lastitem(void);
int _Cdecl getitem(unsigned int keycode);

/*
 * �e �a���a�A�� �a�w �� �w���� �����i ���e�a
 */
static int _Cdecl maxitemlength(int curmenu_num)
{
	int i, max, length;

	max = 0;
	for(i = 0; i < pmenu[curmenu_num].nitem; i++)
		if((length = strlen(pmenu[curmenu_num].item[i])) > max)
			max = length;
	return max;
}

/*
 *  �i�a���� �a���a�i �����e�a
 *  fcolor    : �i�a��
 *  bcolor    : ���w��
 *  init_menu : �����wȁ�� �A�A��ѡ (�E���A�ᦁ�� 0, 1, 2...)
 *  gap       : �A�A�a�� �e�b
 */
void _Cdecl definemenu(char fcolor, char bcolor, int init_menu, int gap)
{
  char altkey[] = {
    81, 87, 69, 82, 84, 89, 85, 73, 79, 80, 0, 0, 0, 0,
    65, 83, 68, 70, 71, 72, 74, 75, 76,  0, 0, 0, 0, 0,
    90, 88, 67, 86, 66, 78, 77
  };
	register i, j;
  char tmp_fcolor, tmp_bcolor;

  tmp_fcolor = hgetcolor();
  tmp_bcolor = hgetbkcolor();
  hsetcolor(fcolor);
  hsetbkcolor(bcolor);
  menufcolor = fcolor, menubcolor = bcolor;
  menuflag = itemflag = ON;
  themenu = theitem = -1;   /* ��Ȃ�E �a���a�� �w����ѡ ������ */
	for(i = 0; i < nmenu; i++) {
		pmenu[i].menuable = true;
		for(j = 0; j < pmenu[i].nitem; j++)
			pmenu[i].itemable[j] = (!strcmp(pmenu[i].item[j], "-")) ? false : true;
		pmenu[i].menuwidth = strlen(pmenu[i].menu)+2;
		pmenu[i].itemwidth = maxitemlength(i)+3+2;
    /* �a���a �w�A�� ���A �i�a�a �ỡ�e ������ ���a */
  	pmenu[i].left = 2;
		for(j = 0; j < i; j++)
			pmenu[i].left += pmenu[j].menuwidth+gap;
		pmenu[i].top = 3;
		pmenu[i].width = pmenu[i].itemwidth;
		pmenu[i].height = pmenu[i].nitem;
	} /* for i */
	curmenu = init_menu;
	for(i = 0; i < nmenu; i++)
		curitem[i] = 0; /* ���e �w����ѡ ������ */
	for(i = 0; i < nmenu; i++) {
		for(j = 0; j < pmenu[i].nitem; j++) {
			if(pmenu[i].shortcut[j] == 0) { /* �e�i�A ���q */
				sprintf(pmenu[i].shortcutchar[j], "%c%c", ' ', 0);
      }
      else {  /* �e�i�A �i�a �e�i�� */
				if(pmenu[i].shortcut[j] < 27) { /* �e�i�A�a Ctrl���s���e */
					sprintf(pmenu[i].shortcutchar[j], "%c%c%c",
                  '^', pmenu[i].shortcut[j]+64, 0);
				}	else if(pmenu[i].shortcut[j] < 97) { /* �e�i�A�a �w���a */
					sprintf(pmenu[i].shortcutchar[j], "%c%c", pmenu[i].shortcut[j], 0);
				}	else if(pmenu[i].shortcut[j] >= F1 && pmenu[i].shortcut[j] < F10) {
          /* F1 .. F9 �a�� */
					sprintf(pmenu[i].shortcutchar[j], "%c%c%c",
                  'F', ((pmenu[i].shortcut[j]-F1) >> 8)+'1', 0);
        } else if(pmenu[i].shortcut[j] == F10) { /* F10 ���e */
					sprintf(pmenu[i].shortcutchar[j], "%c%c%c", 'F', '0', 0);
        } else if(pmenu[i].shortcut[j] >= CTRL_F1 && pmenu[i].shortcut[j] < CTRL_F10) {
          /* ^F1 .. ^F9 �a�� */
					sprintf(pmenu[i].shortcutchar[j], "%c%c%c%c",
                  '^', 'F', ((pmenu[i].shortcut[j]-CTRL_F1) >> 8) +'1', 0);
        } else if(pmenu[i].shortcut[j] == CTRL_F10) { /* ^F10 ���e */
					sprintf(pmenu[i].shortcutchar[j], "%c%c%c%c", '^', 'F', '0', 0);
        } else if(pmenu[i].shortcut[j] >= ALT_F1 && pmenu[i].shortcut[j] < ALT_F10) {
          /* @F1 .. @F9 �a�� */
					sprintf(pmenu[i].shortcutchar[j], "%c%c%c%c",
                  '@', 'F', ((pmenu[i].shortcut[j]-ALT_F1) >> 8) +'1', 0);
        } else if(pmenu[i].shortcut[j] == ALT_F10) { /* @F10 ���e */
					sprintf(pmenu[i].shortcutchar[j], "%c%c%c%c", '@', 'F', '0', 0);
        } else if(pmenu[i].shortcut[j] >= ALT_1 && pmenu[i].shortcut[j] < ALT_0) {
          /* @1 .. @9 �a�� */
					sprintf(pmenu[i].shortcutchar[j], "%c%c%c",
                  '@', ((pmenu[i].shortcut[j]-ALT_1)>>8) +'1', 0);
        } else if(pmenu[i].shortcut[j] == ALT_0) { /* @0 ���e */
					sprintf(pmenu[i].shortcutchar[j], "%c%c%c", '@', '0', 0);
        } else { /* ALT ���s �i�A */
          sprintf(pmenu[i].shortcutchar[j], "%c%c%c", '@', ' ', 0);
					pmenu[i].shortcutchar[j][1]=altkey[(pmenu[i].shortcut[j]-0x1000)>>8];
				} /* else */
			} /* else */
		} /* for j */
	} /* for i */
  hsetcolor(tmp_fcolor);
  hsetbkcolor(tmp_bcolor);
}

/*
 *  �i�a���� �a���a�� �a���wз�����i �a���a
 */
void _Cdecl drawmenubar(void)
{
	register i, j;
  bool onoff, onoff2;
  char fcolor, bcolor;

  fcolor = hgetcolor();
  bcolor = hgetbkcolor();
  hsetcolor(menufcolor);
  hsetbkcolor(menubcolor);
  onoff = isreverse(); onoff2 = isunder();
  hsetreverse(ON);     hsetunder(ON);
  eputchs(-1, -1, 80, ' ');
	if(!(pmenu[curmenu].menuable)) /* �a���a �a�w�a���A �a�a */
		for(i = nmenu-1; i >= 0; i--)  /* �a���a ������ѡ ���� */
			if(pmenu[i].menuable)
				curmenu = i;
	for(i = 0; i < nmenu; i++) /* �b �a���a�� �w�� �a�w�a���A �a�a */
		if(!(pmenu[i].itemable[curitem[i]])) /* �w����ѡ ������ */
			for(j = pmenu[i].nitem-1; j >= 0; j--)
				if(pmenu[i].itemable[j] == true)
					curitem[i] = j;
	for(i = 0; i < nmenu; i++)
    hprintfxy(-pmenu[i].left, -1, " %s ", pmenu[i].menu);
  hsetreverse(onoff);  hsetunder(onoff2);
  menuflag = OFF;
  hsetcolor(fcolor);
  hsetbkcolor(bcolor);
}

/*
 *  �a���a�����i �e��
 */
static void _Cdecl highlightmenu(void)
{
  bool onoff, onoff2;

  onoff = isreverse(); onoff2 = isunder();
  if(menuflag) {
    hsetreverse(ON); hsetunder(ON);
  }
	hprintfxy(-pmenu[curmenu].left, -1, " %s ", pmenu[curmenu].menu);
  if(menuflag) {
    hsetreverse(onoff); hsetunder(onoff2);
  }
  menuflag = !menuflag;
}

/*
 *  �w�� �����i �e��
 */
static void _Cdecl highlightitem(void)
{
  register limit;
  int offset, onoff;

  onoff = isreverse();
  if(itemflag)
    hsetreverse(ON);
	hprintfxy(1, curitem[curmenu]+1, " %s", pmenu[curmenu].item[curitem[curmenu]]);
  offset = strlen(pmenu[curmenu].item[curitem[curmenu]])+1;
  limit = pmenu[curmenu].width-3-offset;
  eputchs(offset+1, curitem[curmenu]+1, limit, ' ');
  hprintfxy(pmenu[curmenu].width-2, curitem[curmenu]+1, "%-3s", pmenu[curmenu].shortcutchar[curitem[curmenu]]);
  if(itemflag)
    hsetreverse(onoff);
  itemflag = !itemflag;
}

/*
 *  ��ѡǡ�i ����� �A�A�i ��Ȃ.. �a�e�i ���w�a�e�A �a�w�a�� ��Ё��a�����a�e
 *  �a���a�� �A�A�i ��Ȃ�a�e �w���i ��Ё �e�i�� ���v�q
 *  �E���A�ᦁ�� 1, 2, 3, ...
 */
/*static bool _Cdecl selectmenu(int menunum)
{
  if(!pmenu[curmenu].menuable || !(menunum >= 0 && menunum <= 9) || !(menunum < nmenu))
    return false;
  erasemenubox();
  curmenu = menunum;
  return drawmenubox() ? true : false;
} */

/*
 *  �A�A�� �w���i ��Ȃ�i �e�a.
 *  �� �q���e �a���a�� �a���a�i ��Ȃ�a�e �w���i ��Ё�� �e�i���a
 */
/*static int _Cdecl selectitem(int itemnum)
{
  if(!pmenu[curmenu].itemable[itemnum] || !(itemnum < pmenu[curmenu].nitem))
    return false;
  highlightitem();
  themenu = curmenu;
  theitem = curitem[curmenu] = itemnum;
  highlightitem();
  return true;
} */

/* �i�� �w���w�a�i �����a */
static void _Cdecl erasemenubox(void)
{
  highlightitem();
	wclose();
	highlightmenu();
}

/* keycode�a �e�a�e�a�� ����A�� ���e�� ��a */
static bool _Cdecl findshortcut(unsigned int keycode)
{
	register i, j;

  keycode = ((keycode > 256) ? keycode : toupper(keycode));
	for(i = 0; i < nmenu; i++) {
		if(pmenu[i].menuable) {
			for(j = 0; j < pmenu[i].nitem; j++) {
				if(keycode == pmenu[i].shortcut[j] && pmenu[i].itemable[j]) {
					if(ispulldown)
						erasemenubox();
					curmenu = themenu = i;
					curitem[i] = theitem = j;
					return true; /* �e�i�A �x�v�a */
				} /* if keycode */
			} /* for j */
		} /* if */
	} /* for i */
	return false; /* �e�i�A ���x�v�a */
}

/* �w�� �w�a�i �a���a */
static bool _Cdecl drawmenubox(void)
{
	register i;
  int left, top, width, height;
  int limit, offset, onoff;

  left = pmenu[curmenu].left, top = pmenu[curmenu].top;
  width = pmenu[curmenu].width, height = pmenu[curmenu].height;

	highlightmenu();
	if(wopen(left, top, width, height) == 0)
    return false;
	for(i = 0; i < pmenu[curmenu].nitem; i++) {
		if(!strcmp(pmenu[curmenu].item[i], "-")) {
      eputchs(1, i+1, width, '�');
		} else {
			if(!(pmenu[curmenu].itemable[i])) {
        onoff = isfaint();
        hsetfaint(ON);
			  hprintfxy(1, i+1, " %s", pmenu[curmenu].item[i]);
        offset = strlen(pmenu[curmenu].item[i])+1;
        limit = width-2-offset;
        eputchs(offset+1, i+1, limit, ' ');
			  hprintfxy(width-2, i+1, "%-3s", pmenu[curmenu].shortcutchar[i]);
        hsetfaint(onoff);
			} else {
			  hprintfxy(1, i+1, " %s", pmenu[curmenu].item[i]);
        offset = strlen(pmenu[curmenu].item[i])+1;
        limit = width-2-offset;
        eputchs(offset+1, i+1, limit, ' ');
			  hprintfxy(width-2, i+1, "%-3s", pmenu[curmenu].shortcutchar[i]);
      } /* �e�� else */
		} /* �a�{�� else */
	} /* for */
	highlightitem();
  return true;
}

static bool _Cdecl beforemenu(void)
{
	erasemenubox();
	do {
		curmenu--;
		if(curmenu < 0)
			curmenu = nmenu-1;
	} while(!pmenu[curmenu].menuable);
	return drawmenubox() ? true : false;
}

static bool _Cdecl nextmenu(void)
{
	erasemenubox();
	do {
		curmenu++;
		if(curmenu >= nmenu)
			curmenu = 0;
	} while(!pmenu[curmenu].menuable);
	return drawmenubox() ? true : false;
}

static void _Cdecl beforeitem(void)
{
	highlightitem();
	do {
		curitem[curmenu]--;
		if(curitem[curmenu] < 0)
			curitem[curmenu] = pmenu[curmenu].nitem-1;
	} while(!pmenu[curmenu].itemable[curitem[curmenu]]);
	highlightitem();
}

static void _Cdecl nextitem(void)
{
	highlightitem();
	do {
		curitem[curmenu]++;
		if(curitem[curmenu] >= pmenu[curmenu].nitem)
			curitem[curmenu] = 0;
	} while(!pmenu[curmenu].itemable[curitem[curmenu]]);
	highlightitem();
}

static void _Cdecl firstitem(void)
{
	register i;

	highlightitem();
	i = 0;
	while(!pmenu[curmenu].itemable[i])
		i++;
	curitem[curmenu] = i;
	highlightitem();
}

static void _Cdecl lastitem(void)
{
	register i;

	highlightitem();
	i = pmenu[curmenu].nitem-1;
	while(!pmenu[curmenu].itemable[i])
		i--;
	curitem[curmenu] = i;
	highlightitem();
}

int _Cdecl getitem(unsigned int keycode)
{
  int fcolor, bcolor;

  fcolor = hgetcolor();
  bcolor = hgetbkcolor();
  hsetcolor(menufcolor);
  hsetbkcolor(menubcolor);
	ispulldown = OFF;
	if(keycode != F10) {             /* F10�� ������ �g�v�a�e */
    hsetcolor(fcolor);
    hsetbkcolor(bcolor);
		return findshortcut(keycode);   /* �e�i�A���� ��a�e�a */
  }
	else {                            /* F10�� ���v�a�e */
		ispulldown = ON;
		if(!drawmenubox()) {
      hsetcolor(fcolor);
      hsetbkcolor(bcolor);
      return false;
    }
		do {
			keycode = getxch();
			switch(keycode) {
				case LEFTARROW  :
          beforemenu();
					break;
				case RIGHTARROW :
          nextmenu();
					break;
				case UPARROW    :
          beforeitem();
					break;
        case ' '    :
				case DOWNARROW  :
          nextitem();
					break;
				case HOMEKEY  :
          firstitem();
          break;
				case ENDKEY   :
          lastitem();
					break;
				case '\r'   :
          themenu = curmenu;           /* �a���a ��ѡ */
					theitem = curitem[curmenu]; /* �w�� ��ѡ */
					erasemenubox();
          ispulldown = OFF;
          hsetcolor(fcolor);
          hsetbkcolor(bcolor);
					return true; /* ��Ȃ */
				case ESC    :
          erasemenubox();
          ispulldown = OFF;
          hsetcolor(fcolor);
          hsetbkcolor(bcolor);
					return false; /* �᭡ */
				default     :
          if(findshortcut(keycode)) { /* �e�i�A�a ���a�e */
            ispulldown = OFF;
            hsetcolor(fcolor);
            hsetbkcolor(bcolor);
					  return true; /* ��Ȃ */
          }
			}/* switch */
		} while(true); /* ���e ���a */
	}/* else */
}

/* HANMENU.C������ �{ */
