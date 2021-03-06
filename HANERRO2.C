/*
 *  �a���a��  : �e�i�a���a�១
 *  ���� ���q : HANERRO2.C
 *  �e�e ��   : �� �� �b
 *  �i�a      : 1992. 6. 26
 *  ���� ���w : ���A�១ ����
 */

#include <dos.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include "hanout.h"
#include "hancoord.h"
#include "hancolor.h"
#include "hanin.h"
#include "ascii.h"
#include "extkey.h"
#include "hanwindw.h"

#define  MAXERRNO   35

/* ���a�� ���A ���� */
byte *hsys_errlist[] = {
  "���A 0",
  "�i�� �q����ѡ���a",
  "�a�� �������a ���Bɡ���e ����a",
  "�a�� �w��(path)�e ���e�A�a",
  "�ᢁ �g�e �����i �i����a",
  "��a�A�� �g�e��a",
  "������ѡ�a �i�v��a",
  "����w�� ���b�� �a�A�A���A�a",
  "����w���a �����a���a",
  "����w�� ���b�����a �i�v�A�a",
  "�ŉw(environment)�� �i���A����a",
  "�w��(format)�� �i���A���A�a",
  "��e��ѡ(access cose)�a �i�v��a",
  "�a�a(data)�a �i�v���a",
  "",
  "�a�� �wá�e ����a",
  "�e�� ���Bɡ���i �����a�a�a��..",
  "�{�e �wá�a �a���A�a",
  "�� ���w ������ ����a",
  "���a(argument)�a �i�v��a",
  "���a ������ �ᢁ ����a",
  "��З �w���� �i���A����a",
  "�wá �e�i�� ���v��a",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "���e�A�� �w�b�� �i���A����a",
  "�i���a �ᢁ �a���a",
  "������ ���� ���e�A�a"
};

/* á�w�ⷥ ���A - critical error - */
byte *crit_errlist[] = {
  "���a�a�a �a���w�� �A�᷶���a",    /* Write-protect error   */
  "�a�a���a ���q�� �i�v��a",        /* Invalid drive number  */
  "�a�a���a�� ���� �i�a���A�a",      /* Drive not ready       */
  "�w�w��a �i�v��a",               /* Invalid command       */
  "CRC ���A�a �v�e�A�a",             /* CRC error             */
  "���� ���� �a���a �i�v�a���a",     /* Bad request structure length  */
  "Seek ���A���a",                   /* Seek error            */
  "�a�� �w��(format)�� �a�����a",    /* Unknown medium; disk format not recognized */
  "�B��i ���i�� ���A�a",            /* Sector not found      */
  "�a����A �����a ����a",          /* Printer out of paper  */
  "�a�e ���e ���A�a ���v�A�a",       /* Write error           */
  "���e ���e ���A�a ���v�A�a",       /* Read error            */
  "���a�Q ���A-���e�� ���A���a"      /* General, nonspecific error  */
};

/* �a�w�a ���A ���� */
byte *user_errlist[] = {
  "�A�����a �����a�A�a",
};

/* ���w�E ��á�A �A���i �a���a�q ���a�i�i �b�a�� ǡ ���b�i ���a���a. */

int _Cdecl disp_msg(int x, int y, byte *msg)
{
  bool onoff;
  int ret, width;

  width = (strlen(msg)+4 > 10) ? strlen(msg)+4 : 10;
  if( !wopen(x, y, width, 3))
    return 0;
  wtitle(msg);
  onoff = isreverse();
  hsetreverse(ON);
  wcenter(2, " �·� ");
  hsetreverse(onoff);
  ret = getxch();
  wclose();
  return ret;
}

/* ���e �a���A �A�����i �b�a�� ǡ ���b�i ���a���a. */

int _Cdecl disp_cmsg(byte *msg)
{
  int ret, width;
  bool onoff;

  width = (strlen(msg)+4 > 10) ? strlen(msg)+4 : 10;
  if( !wopen(hgetmaxax()/2-strlen(msg)/2-1, hgetmaxay()/2-1, width, 3))
    return 0;
  wtitle(msg);
  onoff = isreverse();
  hsetreverse(ON);
  wcenter(2, " �·� ");
  ret = getxch();
  hsetreverse(onoff);
  wclose();
  return ret;
}

/*
 * format�i �q���a�a buffer�A�� ϩ�a�����a�i �A���e ҁ data�A ����, ��
 * data�i �A�����a.
 */

byte *pickoutdata(byte *buffer, byte *format, byte *data)
{
  byte picture[] = "#CcAa9HhEeKkLlYy!xX*";
  register i, j, len;

  len = strlen(format);

  for(i = 0, j = 0; i < len; i++) {
    if(format[i] & 0x80) {
      data[j++] = buffer[i++];
      data[j++] = buffer[i];
    }
    else {
      if(strchr(picture, format[i]))
        data[j++] = buffer[i];
    }
  }
  data[j] = '\0';
  return data;
}

/*
 * format�i �q���a�a buffer�A ϩ�a�����a�i �������e ҁ data�A �t�i ���a.
 */

byte *putindata(byte *buffer, byte *format, byte *data)
{
  byte picture[] = "#CcAa9HhEeKkLlYy!xX*";
  register i, j, k, len;

  len = strlen(format);

  for(i = 0, j = 0, k = 0; i < len; i++) {
    if(format[i] & 0x80) {
      data[j++] = format[i++];
      data[j++] = format[i];
    }
    else {
      if(strchr(picture, format[i]))
        data[j++] = buffer[k++];
      else
        data[j++] = format[i];
    }
  }
  return data;
}

/*
 *  �w�� �i��� ���a�i ���b�i �h�e�a.
 *  maxlen :  ���b�i �� ���e �A�� ����
 *  title  :  ���b �w�� �A��
 *  str    :  ���b�h�i ���a�i�� ͡����
 */
bool _Cdecl win_hgets(int maxlen, byte *title, byte *str)
{
  int width, ans, cnt;
  int allowcanceldelline, restoreline, cuttail, allowfirstdelline;

  if((width = strlen(title)) < maxlen)
    width = maxlen;
  if( !wopen(hgetmaxax()/2-width/2-1, hgetmaxay()/2-1, width+4, 3))
    return 0;
  wtitle(title);
  hgotoxy(2, 2);
  allowcanceldelline = _allowcanceldelline, restoreline = _restoreline;
  cuttail = _cuttail, allowfirstdelline = _allowfirstdelline;

  _cuttail = false, _allowcanceldelline = _restoreline = true;

  ans = hgetln(str, maxlen);
  cnt = strlen(str);
  while(cnt < maxlen)
    str[cnt++] = ' ';
  str[cnt] = '\0';
  _allowcanceldelline = allowcanceldelline, _restoreline = restoreline;
  _cuttail = cuttail, _allowfirstdelline = allowfirstdelline;
  wclose();
  return (ans == '\r') ? true : false;
}

/*
 *  �w�� �i��� �a�a�i ���b�i �h�e�a. - ���b���� �����a�w
 *  maxlen :  ���b�i �� ���e �A�� ����
 *  title  :  ���b �w�� �A��
 *  mask   :  ���b����
 *  str    :  ���b�h�i ���a�i�� ͡����
 */

bool _Cdecl win_hgetdata(int maxlen, byte *title, byte *mask, byte *str)
{
  int width, ans, cnt;
  int allowcanceldelline, restoreline, cuttail, allowfirstdelline;
  bool onoff;
  char buf[80];

  if((width = strlen(title)) < maxlen)
    width = maxlen;
  if( !wopen(hgetmaxax()/2-width/2-1, hgetmaxay()/2-1, width+4, 3))
    return 0;
  wtitle(title);
  hgotoxy(2, 2);
  allowcanceldelline = _allowcanceldelline, restoreline = _restoreline;
  cuttail = _cuttail, allowfirstdelline = _allowfirstdelline;

  _cuttail = false, _allowcanceldelline = _restoreline = true;
  putindata(str, mask, buf);    /* ���a�i�A ���b�����i ���� ���e�a */
  onoff = isreverse();
  hsetreverse(ON);
  ans = hgetdata(buf, mask);    /* �a�a�i ���b�h�e�a. */
  hsetreverse(onoff);
  pickoutdata(buf, mask, str);  /* ���b�h�e ���a�i�A�� ���b�����i �A���e�a */
  cnt = strlen(str);
  while(cnt < maxlen)           /* �a�ỡ �����e �����a�� �������e�a */
    str[cnt++] = ' ';
  str[cnt] = '\0';
  _allowcanceldelline = allowcanceldelline, _restoreline = restoreline;
  _cuttail = cuttail, _allowfirstdelline = allowfirstdelline;
  wclose();
  return (ans == '\r') ? true : false;
}

/*
 * sec�����e ǡ���b�� ���a�e ESC�a ���� ���a�� �e���a��
 * �a ����A ǡ���b�� ���a�e �b�� �{���a.
 */

int _Cdecl wait_getkey(int sec)
{
  time_t time1, time2;
  double diff;
  int ans, isquit = 0;

  time(&time1);
  do {
    diff = difftime( time(&time2), time1 );
    if((int)diff >= sec)
      isquit = true, ans = ESC;
    else {
      ans = getkey();
      if(ans)
        isquit = true;
    }
  } while(!isquit);
  return ans;
}

/* ���e �a���A �A�����i �b�a�� ǡ ���b�i sec���e�q�e ���a���a. */

int _Cdecl disp_cmsg_sec(byte *msg, int sec)
{
  int ret, width;
  bool onoff;

  width = (strlen(msg)+4 > 10) ? strlen(msg)+4 : 10;
  if( !wopen(hgetmaxax()/2-strlen(msg)/2-1, hgetmaxay()/2-1, width, 3))
    return 0;
  wtitle(msg);
  onoff = isreverse();
  hsetreverse(ON);
  wcenter(2, " �·� ");
  ret = wait_getkey(sec);
  hsetreverse(onoff);
  wclose();
  return ret;
}

/*  �A���i �a���� ���A �����i �b�e ҁ �a�e ���b�i ���a���a. */

void _Cdecl disperror(byte *msg)
{
  int color, width;

  width = (strlen(msg)+4 > 26) ? strlen(msg)+4 : 26;
  if( !wopen(hgetmaxax()/2-strlen(msg)/2-1, hgetmaxay()/2-1, width, 3))
    return;
  wtitle(msg);
  color = hgetcolor();
  hsetcolor(HRED);
  wcenter(2, "�a����a ���a�A�a");
  getxch();
  hsetcolor(color);
  wclose();
}

/*  �A���� ���� �a��..  �a�� �a�a�� �a�� �q���� �e�q
 *  �a�w�a�a �a���a�� �������i��
 *  ��ᕡ �� ���A���� �a���i ���e �A���� ���e�� ���e��
 *  ��a�a�e ���� ���a.
 */

void _Cdecl memerror(void)
{
  disperror("����w���a �����a�A�a");
}

/*  ��b�e�� errno�A �x�e ���A�i ���e ���w�A �a�� */

void _Cdecl hperror(void)
{
  if(errno > MAXERRNO) /* ���A��ѡ�a �i�� �����A���q */
    return;
  disperror(hsys_errlist[errno]);
}

/*
 * �a��, ���e �a�e�i ���b �h�e �q��
 * �a��--> 0, �a�e--> 1, ESC--> 2
 * ".. �i�a�a?", ������á
 */

int _Cdecl retry(byte *str, int initpos)
{
  int pos, key, ret, tbl[] = { 11, 20, 29 };
  bool onoff, isquit = 0;
  char *msg2[] = { " �a�� ", " �a�e ", " �᭡ " };
  char msg[] =  " �a��     �a�e     �᭡ ";

  pos = initpos;
  if((pos < 0) || (pos > 2))  /* ������á�a �i�� �����A�e */
    pos = 0;

  if( !wopen(hgetmaxax()/2-22, hgetmaxay()/2-1, 44, 3))
    return 0;
  onoff = isreverse();   /* �e�� ���� ���� */
  wtitle(str);
  hputsxy(11, 2, msg);
  do {
    hsetreverse(ON);
    hputsxy(tbl[pos], 2, msg2[pos]);
    hsetreverse(OFF);
    key = getxch();
    switch(key) {
      case LEFTARROW :
        hputsxy(tbl[pos], 2, msg2[pos]);
        if(pos > 0)
          pos--;
        else
          pos = 2;
        hsetreverse(ON);
        hputsxy(tbl[pos], 2, msg2[pos]);
        hsetreverse(OFF);
        break;
      case RIGHTARROW :
      case ' ' :
        hputsxy(tbl[pos], 2, msg2[pos]);
        if(pos < 2)
          pos++;
        else
          pos = 0;
        hsetreverse(ON);
        hputsxy(tbl[pos], 2, msg2[pos]);
        hsetreverse(OFF);
        break;
      case ESC :
        isquit = 1;
        ret = 2;  /* �᭡ */
        break;
      case '\r' :
        isquit = 1;
        ret = pos;
        break;
    }
  } while( !isquit);
  hsetreverse(onoff);
  wclose();
  return ret;
}

/*
 * ��, �a�����i ���b�h�e �q��
 * ��--> 0, �a����--> 1, ESC--> 2
 * ".. �i�a�a?", ������á
 */
int _Cdecl yesno(byte *str, int initpos)
{
  int pos, key, ret, tbl[] = { 11, 20, 29 };
  bool onoff, isquit = 0;
  char *msg2[] = { "  ��  ", "�a����", " �᭡ " };
  char msg[] =  "  ��     �a����    �᭡ ";

  pos = initpos;
  if((pos < 0) || (pos > 2))  /* ������á�a �i�� �����A�e */
    pos = 0;

  if( !wopen(hgetmaxax()/2-22, hgetmaxay()/2-1, 44, 3))
    return 0;
  onoff = isreverse();   /* �e�� ���� ���� */
  wtitle(str);
  hputsxy(11, 2, msg);
  do {
    hsetreverse(ON);
    hputsxy(tbl[pos], 2, msg2[pos]);
    hsetreverse(OFF);
    key = getxch();
    switch(key) {
      case LEFTARROW :
        hputsxy(tbl[pos], 2, msg2[pos]);
        if(pos > 0)
          pos--;
        else
          pos = 2;
        hsetreverse(ON);
        hputsxy(tbl[pos], 2, msg2[pos]);
        hsetreverse(OFF);
        break;
      case RIGHTARROW :
      case ' ' :
        hputsxy(tbl[pos], 2, msg2[pos]);
        if(pos < 2)
          pos++;
        else
          pos = 0;
        hsetreverse(ON);
        hputsxy(tbl[pos], 2, msg2[pos]);
        hsetreverse(OFF);
        break;
      case ESC :
        isquit = 1;
        ret = 2;  /* �᭡ */
        break;
      case '\r' :
        isquit = 1;
        ret = pos;
        break;
    }
  } while( !isquit);
  hsetreverse(onoff);
  wclose();
  return ret;
}

/*
 * á�w�� ���A �����a
 * �a��--> 1, �a�e--> 2
 */

int _Cdecl criticalhandler(void)
{
  int error, check;
  byte *str;

  error = _DI;
  if(error < 14)
    str = crit_errlist[error];
  else
    str = "���a�Q ���A";
  check = retry(str, 0);
  hardresume(2-check);
  return 1;
}

/*
 * ���a�� �a�A���a �����a
 * �i ���� ���� �a�� 1�i �A����
 */
int _Cdecl ctrlbrkhandler(void)
{
  return 1;
}

/*
 * ���a�� �a�A���a �����a��, á�w�� ���A(�a�a���� ���A)�����a �w��
 */
void _Cdecl seterrhandler(void)
{
  ctrlbrk(ctrlbrkhandler);
  harderr(criticalhandler);
}

/* HANERRO2.C �� ���� �{ */
