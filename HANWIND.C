/*
 *  �a���a��  : �e�i�a���a�១
 *  ���� ���q : HANWIND.C
 *  �e�e ��   : �� �� �b
 *  �i�a      : 1992.  6. 24
 *  ���� ���w : �w��(WINDOW) �ŝe �q��
 */

#include <alloc.h>
#include <stdarg.h>
#include "hanlib.h"

byte     _winleft, _wintop, _winwidth, _winheight;
HWINDOW  _win[MAXWINDOW];
WINSTATE _state[MAXWINDOW];
static byte  winsp, statesp;

/*
    ��ͻ     ��ͻ     ��͸     ��͸
    � 0�     � 1�     � 2�     � 3�
    ��ͼ     ��Ľ     ��;     ����

    ��͸     ��͸     ��ͻ     ��ͻ
    � 4�     � 5�     � 6�     � 7�
    ��;     ����     ��ͼ     ��Ľ

    ��Ŀ     ��Ŀ     ��ķ     ��ķ
    � 8�     � 9�     �10�     �11�
    ����     ��;     ��Ľ     ��ͼ

    ��ķ     ��Ŀ     ��ķ     ��Ŀ
    �12�     �13�     �14�     �15�
    ��Ľ     ����     ��ͼ     ��;

    ����     ����     ����     ����
    �16�     �17�     �18�     �19�
    ����     ����     ����     ����

    20�� �w�a�e �����a�� �w�a�i �a���a.
*/

static int boxch[][8] = {
    { '�', '�', '�', '�', '�', '�', '�', '�' }, /*  0 */
    { '�', '�', '�', '�', '�', '�', '�', '�' }, /*  1 */
    { '�', '�', '�', '�', '�', '�', '�', '�' }, /*  2 */
    { '�', '�', '�', '�', '�', '�', '�', '�' }, /*  3 */

    { '�', '�', '�', '�', '�', '�', '�', '�' }, /*  4 */
    { '�', '�', '�', '�', '�', '�', '�', '�' }, /*  5 */
    { '�', '�', '�', '�', '�', '�', '�', '�' }, /*  6 */
    { '�', '�', '�', '�', '�', '�', '�', '�' }, /*  7 */

    { '�', '�', '�', '�', '�', '�', '�', '�' }, /*  8 */
    { '�', '�', '�', '�', '�', '�', '�', '�' }, /*  9 */
    { '�', '�', '�', '�', '�', '�', '�', '�' }, /* 10 */
    { '�', '�', '�', '�', '�', '�', '�', '�' }, /* 11 */

    { '�', '�', '�', '�', '�', '�', '�', '�' }, /* 12 */
    { '�', '�', '�', '�', '�', '�', '�', '�' }, /* 13 */
    { '�', '�', '�', '�', '�', '�', '�', '�' }, /* 14 */
    { '�', '�', '�', '�', '�', '�', '�', '�' }, /* 15 */

    { '�', '�', '�', '�', '�', '�', '�', '�' }, /* 16 */
    { '�', '�', '�', '�', '�', '�', '�', '�' }, /* 17 */
    { '�', '�', '�', '�', '�', '�', '�', '�' }, /* 18 */
    { '�', '�', '�', '�', '�', '�', '�', '�' }, /* 19 */

    { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' }  /* 20 */
};

/*
  �a�e �� : �啡�� �wȁ�i �啡 �aȂ�A ��w
*/
static int _Cdecl pushwinstate(void)
{
    if (statesp >= MAXWINDOW) return 0;
    _state[statesp].xpos  = hwherex();
    _state[statesp].ypos = hwherey();
	_state[statesp].winleft = _winleft;
	_state[statesp].wintop = _wintop;
	_state[statesp].winwidth = _winwidth;
	_state[statesp].winheight = _winheight;
    _state[statesp].forcolor = getforcolor();
    _state[statesp].bckcolor = getbckcolor();
	_state[statesp].linecolor = wgetlinecolor();
	_state[statesp].attr = wgetattr();
	statesp++;
}

/*
  �a�e �� : �aȂ�A ��w�E �啡�wȁ�i ����
*/
static void _Cdecl popwinstate(void)
{
	if (--statesp < 0) return;
	hwindow(_state[statesp].winleft, _state[statesp].wintop, _state[statesp].winwidth, _state[statesp].winheight);
	whgotoxy(_state[statesp].xpos, _state[statesp].ypos);
	wsetforcolor(_state[statesp].forcolor);
	wsetbckcolor(_state[statesp].bckcolor);
	wsetlinecolor(_state[statesp].linecolor);
	wsetattr(_state[statesp].attr);
}

/*
  �a�e �� : �a�� �� �a���� ..
  �i�� �� : �w�� ��w �b �q���� �w�a���a �b �e ����
*/
void _Cdecl drawhline(int left, int top, int width, int linetype)
{
  unsigned char tempfcolor;
  unsigned char hline[] = { '�', '�','�', '�', '�', '�', ' ' };
  int baseleft, xpos, ypos;
  FONTATTR tempattr;

  tempattr = getattr();
  sethmagnify(OFF); setvmagnify(OFF);
  xpos = whereisx(); ypos = whereisy();
  tempfcolor = getforcolor();
  setforcolor(getlinecolor());
  for(baseleft = left; left < width+baseleft; left++)
    eputchxy(left, top, hline[linetype]);
  setforcolor(tempfcolor);
  hgotoxy(xpos, ypos);
  setattr(tempattr);
}

/*
  �a�e �� : �A�� �� �a���� ..
*/
void _Cdecl drawvline(int left, int top, int height, int linetype)
{
  unsigned char tempfcolor;
  unsigned char vline[] = { '�', '�', '�', '�', '�', '�', ' ' };
  int basetop, xpos, ypos;
  FONTATTR tempattr;

  tempattr = getattr();
  sethmagnify(OFF); setvmagnify(OFF);
  xpos = whereisx(); ypos = whereisy();
  tempfcolor = getforcolor();
  setforcolor(getlinecolor());
  for(basetop = top; top < basetop+height; top++) {
    eputchxy(left, top, vline[linetype]);
    if(isvmagnify()) top++;
  }
  setforcolor(tempfcolor);
  hgotoxy(xpos, ypos);
  setattr(tempattr);
}

/*
  �a�e �� : �w�a �a���� ..
  ���a �t : boxtype�e 20�a��, clrfalg�a ON���e �w�a�i �a���� �w�a �����i ����
*/
void _Cdecl drawbox(int left, int top, int width, int height, int boxtype, int clrflag)
{
  unsigned char tempfcolor;
  FONTATTR tempattr;
	register x, y;
  int xpos, ypos;

  xpos = whereisx(); ypos = whereisy();
  tempattr = getattr();
  sethmagnify(OFF); setvmagnify(OFF);
  tempfcolor = getforcolor();
  setforcolor(getlinecolor());

  eputchxy(left, top, boxch[boxtype][0]);
  eputchxy(left, top+height-1, boxch[boxtype][6]);
	for(x = left+1; x < left+width-1; x++) {
    eputchxy(x, top, boxch[boxtype][1]);
    eputchxy(x, top+height-1, boxch[boxtype][5]);
	}
  eputchxy(left+width-1, top, boxch[boxtype][2]);
  eputchxy(left+width-1, top+height-1, boxch[boxtype][4]);
  for(y = top+1; y < top+height-1; y++) {
    eputchxy(left, y, boxch[boxtype][7]);
    eputchxy(left+width-1, y, boxch[boxtype][3]);
	}
  if(clrflag) clrwindow(left+1, top+1, width-2, height-2);
  setforcolor(tempfcolor);
  setattr(tempattr);
  hgotoxy(xpos, ypos);
}

/*
  �a�e �� : �啡 ���A�� �a�� �� �a����
*/
void _Cdecl wdrawhline(int left, int top, int width, int linetype)
{
  unsigned char tempfcolor;
  unsigned char hline[] = { '�', '�','�', '�', '�', '�', ' ' };
  int baseleft, xpos, ypos;
  FONTATTR tempattr;

  tempattr = wgetattr();
  wsethmagnify(OFF); wsetvmagnify(OFF);
  xpos = wwhereisx(); ypos = wwhereisy();
  tempfcolor = wgetforcolor();
  wsetforcolor(wgetlinecolor());
  for(baseleft = left; left < width+baseleft; left++)
    weputchxy(left, top, hline[linetype]);
  wsetforcolor(tempfcolor);
  whgotoxy(xpos, ypos);
  wsetattr(tempattr);
}

/*
  �a�e �� : �啡 ���A�� �A�� �� �a����
*/
void _Cdecl wdrawvline(int left, int top, int height, int linetype)
{
  unsigned char tempfcolor;
  unsigned char vline[] = { '�', '�', '�', '�', '�', '�', ' ' };
  int basetop, xpos, ypos;
  FONTATTR tempattr;

  tempattr = wgetattr();
  wsethmagnify(OFF); wsetvmagnify(OFF);
  xpos = wwhereisx(); ypos = wwhereisy();
  tempfcolor = wgetforcolor();
  wsetforcolor(wgetlinecolor());
  for(basetop = top; top < basetop+height; top++)
    weputchxy(left, top, vline[linetype]);
  wsetforcolor(tempfcolor);
  whgotoxy(xpos, ypos);
  wsetattr(tempattr);
}

/*
  �a�e �� : �啡 ���A�� �w�a �a���� ..
  ���a �t : boxtype�e 20�a��, clrfalg�a ON���e �w�a�i �a���� �w�a �����i ����
*/
void _Cdecl wdrawbox(int left, int top, int width, int height, int boxtype, int clrflag)
{
  unsigned char tempfcolor;
  FONTATTR tempattr;
	register x, y;
  int xpos, ypos;

  xpos = wwhereisx(); ypos = wwhereisy();
  tempattr = wgetattr();
  wsethmagnify(OFF); wsetvmagnify(OFF);
  tempfcolor = wgetforcolor();
  wsetforcolor(wgetlinecolor());

  weputchxy(left, top, boxch[boxtype][0]);
  weputchxy(left, top+height-1, boxch[boxtype][6]);
	for(x = left+1; x < left+width-1; x++) {
    weputchxy(x, top, boxch[boxtype][1]);
    weputchxy(x, top+height-1, boxch[boxtype][5]);
	}
  weputchxy(left+width-1, top, boxch[boxtype][2]);
  weputchxy(left+width-1, top+height-1, boxch[boxtype][4]);
  for(y = top+1; y < top+height-1; y++) {
    weputchxy(left, y, boxch[boxtype][7]);
    weputchxy(left+width-1, y, boxch[boxtype][3]);
	}
  if(clrflag) clrwindow(left+1, top+1, width-2, height-2);
  wsetforcolor(tempfcolor);
  wsetattr(tempattr);
  whgotoxy(xpos, ypos);
}

/*
  �a�e �� : �啡 �i��, �����E ���i�e �啡 ���� ���i�a�� ���..
  ���� �t : ���� --> 1, ��́.. ���a ���� �i��, �啡 ���� ����, �A���� ����.
*/
int _Cdecl wopen(int left, int top, int width, int height, int boxtype, int forcolor, int bckcolor, int linecolor)
{
	unsigned long size;
	register i;

  if(left < 0 || left > getmaxix() || top < 0 || top > getmaxiy()
    || width < 0 || width > getmaxix() || height < 0 || height > getmaxiy())
    return WEINVALP;
	if (winsp >= MAXWINDOW) return WE2MANY;
	size = (width + 2) * (height + 2) * 16;
	for (i = 0; i < _plane; i++) {
		if ((_win[winsp].buffer[i] = farmalloc(size)) == NULL) {
			while (i-- > 0) farfree(_win[winsp].buffer[i]);
			memerror();
			return WENOMEM;
		}
	}
	scrsave(left-1, top-1, width+2, height+2, _win[winsp].buffer);
	_win[winsp].left = left, _win[winsp].top = top;
	_win[winsp].width = width, _win[winsp].height = height;
	winsp++;
	pushwinstate();
	if (isHGC()) wsetbckcolor(bckcolor);
	else {
		wsetforcolor(forcolor);
		wsetbckcolor(bckcolor);
		wsetlinecolor(linecolor);
	}
	hwindow(left, top, width, height);
	whgotoxy(0, 0);
  savefblcolor();
  setlinecolor(linecolor);
  setbckcolor(bckcolor);
	drawbox(left-1, top-1, width+2, height+2, boxtype, ON);
  restorefblcolor();
	return SUCCESS;
}

/*
  �a�e �� : �啡 �h��
*/
void _Cdecl wclose(void)
{
	register i;

	if (--winsp >= 0) {
		popwinstate();
		scrrestore(_win[winsp].left-1, _win[winsp].top-1, _win[winsp].width+2, _win[winsp].height+2, _win[winsp].buffer);
		for (i = 0; i < _plane; i++) farfree(_win[winsp].buffer[i]);
	}
}

/*
  �a�e �� : width�e�q �a���� XOR�e�a
*/
int _Cdecl xorhoriz(int left, int top, int width)
{
	unsigned long size;
	register int i, j;
  unsigned char *buffer[4];

	size = width * 16;

	for (i = 0; i < _plane; i++) {
		if ((buffer[i] = farmalloc(size)) == NULL) {
			while (i-- > 0) farfree(buffer[i]);
			memerror();
			return FAIL;
		}
	}
	scrsave(left, top, width, 1, buffer);
	for (i = 0; i < _plane; i++) {
    for(j = 0; j < width*16; j++) {
		  buffer[i][j] ^= 0xFF;        /* XOR */
		}
	}
  scrrestore(left, top, width, 1, buffer);
  for (i = 0; i < _plane; i++) farfree(buffer[i]);
	return SUCCESS;
}

/*
  �a�e �� : height�e�q �A���� XOR�e�a
*/
int _Cdecl xorvert(int left, int top, int height)
{
	unsigned long size;
	register int i, j;
  unsigned char *buffer[4];

	size = height * 16;

	for (i = 0; i < _plane; i++) {
		if ((buffer[i] = farmalloc(size)) == NULL) {
			while (i-- > 0) farfree(buffer[i]);
			memerror();
			return FAIL;
		}
	}
	scrsave(left, top, 1, height, buffer);
	for (i = 0; i < _plane; i++) {
    for(j = 0; j < height*16; j++) {
		  buffer[i][j] ^= 0xFF;        /* XOR */
		}
	}
  scrrestore(left, top, 1, height, buffer);
  for (i = 0; i < _plane; i++) farfree(buffer[i]);
	return SUCCESS;
}

/*
  �a�e �� : �啡 ���A�� width�e�q �a���� XOR�e�a
*/
int _Cdecl wxorhoriz(int left, int top, int width)
{
  return xorhoriz(_winleft+left, _wintop+top, width);
}

/*
  �a�e �� : �啡 ���A�� height�e�q �A���� XOR�e�a
*/
int _Cdecl wxorvert(int left, int top, int height)
{
	return xorvert(_winleft+left, _wintop+top, height);
}

/*
  �a�e �� : �啡�� �w�a �A�����A �A���i �����a.
  ���a �t : ���a�� ���ỡ�e ���i�e �� �q�������A��e �A�a.
            �b, �� �q�� ѡ ҁ�A ���a�Q�� ���i�� �a�ỡ�e �g�e�a.
  �i�� �� : �w�a ��, �a���� �a���A, �E��, ���e�������A �i �� ���a�a
            �e�� �锁���a �b������ �A�� ���a�e �����A���e
            �a�� ���e �����Q.
*/
void _Cdecl wtitle(unsigned char *str, int position, int forcolor, int bckcolor)
{
  unsigned char tempx, tempy, tempfcolor, tempbcolor, x, y, length;
  FONTATTR tempattr;

  tempx = whereisx(); tempy = whereisy();
  tempfcolor = getforcolor(); tempbcolor = getbckcolor();
  tempattr = getattr();
  setvmagnify(OFF);
  length = ishmagnify() ? strlen(str)*2 : strlen(str);
  switch(position) {
    case TLEFT :
      x = _winleft+1, y = _wintop-1;
      break;
    case TCENTER :
      x = _winleft+(_winwidth/2 - length/2), y = _wintop-1;
      break;
    case TRIGHT :
      x = _winleft+_winwidth-length-1, y = _wintop-1;
      break;
    case BLEFT :
      x = _winleft+1, y = _wintop+_winheight;
      break;
    case BCENTER :
      x = _winleft+(_winwidth/2 - length/2), y = _wintop+_winheight;
      break;
    case BRIGHT :
      x = _winleft+_winwidth-length-1, y = _wintop+_winheight;
      break;
    default :
      break;
  }
  setforcolor(forcolor);
  setbckcolor(bckcolor);
  hputsxy(x, y, str);
  hgotoxy(tempx, tempy);
  setforcolor(tempfcolor);
  setbckcolor(tempbcolor);
  setattr(tempattr);
}

/*
  �a�e �� : �e�� �啡�� �A�����i �a�� �a���a.
  �i�� �� : �����E ���� ���i�e �� �q�� �����A��e �A�a�a��,
            �����E ���a�� �a��� �១���e �g�e�a.
*/
void _Cdecl wbox(int boxtype, unsigned char linecolor)
{
  unsigned char templcolor;
  FONTATTR temp_attr;

  templcolor = getlinecolor();
  setlinecolor(linecolor);
  temp_attr = getattr();
  setattr(wgetattr());
  drawbox(_winleft-1, _wintop-1, _winwidth+2, _winheight+2, boxtype, OFF);
  setattr(temp_attr);
  setlinecolor(templcolor);
}

/*
  �a�e �� : �e�� �啡�� �w�����a�A ���a�i�i �a���A ���i���a �b�e�a.
  �i�� �� : �i���� ���A �១������ ���a�a��, ���a�i �����A ���w�i �ᴡ..
*/
void _Cdecl wcenter(int y, unsigned char *str)
{
  int length;

  length = wishmagnify() ? strlen(str)*2 : strlen(str);
  whputsxy(_winwidth/2 - length/2, y, str);
}

/*
  �a�e �� : �e�� �啡�i �a�a�� �e�a.
*/
void _Cdecl wscroll(int direction)
{
  switch(direction) {
    case SUP :
      scrollup(_winleft, _wintop, _winwidth, _winheight);
      break;
    case SDOWN :
      scrolldown(_winleft, _wintop, _winwidth, _winheight);
      break;
    case SLEFT :
      scrollleft(_winleft, _wintop, _winwidth, _winheight);
      break;
    case SRIGHT :
      scrollright(_winleft, _wintop, _winwidth, _winheight);
      break;
  }
}

/* HTLWIN.C �� ���� �{ */
