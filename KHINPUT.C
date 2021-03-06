/*
 *  �a��   : khinput.c
 *  ���w   : 2, 3 �鯢 �e�i���b �១ ���� 
 *  ���B   : hantle.prj, nal.prj
 *  �i�a   : 93.03.16
 *  �e�e�� : �e�����b�a ��aɷ�����b�� �e�i�a�� �����b
 *  �i���i : �� Ή�a���� ���a���e �����i ��â�a�� �s���a.
 *           �ᐡ�a�e, �a�� �a�e ���a���� �����i �g�� �h�v�a���a�a.
 *           �a�a��, �� ���a���i �h�a�� ���e �a�e�A �a�w�a�a�� �A���e
 *           ���� �e�e ���a���� �i �����a�a�� ����Ё ������ �a�s���a.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mem.h>
#include <bios.h>

#include "hanlib.h"
#include "hanout.h"
#include "typedefs.h"
#include "hinput.h"
#include "hkeydef.h"

#define TRUE   1
#define FALSE  0

typedef struct {
  int curhanst;
  int key;
  int charcode;
} INPSTACK;

INPSTACK InpStack[10];         /* ��ɡ�a�a ���b �aȂ          */
int OutStack[5];               /* ��ɡ�a�a �b �aȂ          */
int InpSP = 0;                 /* ��ɡ�a�a ���b �aȂ ͡���� */
int OutSP = 0;                 /* ��ɡ�a�a �b �aȂ ͡���� */
int CurHanState = 0;           /* ��ɡ�a�a�� �e�� �wȁ      */
int CursorCode;

int HanAutomata2(int key);
int HanAutomata3(int key);

/*
** �e�� ����E �e�i �a�e ���A�A �a�a Ё�w�a�e ��ɡ�a�a�i ���� ���a.
*/
int HanAutomata(int key)
{
  return (HanKbdKind == HANKBD2) ? HanAutomata2(key) : HanAutomata3(key);
}

#define BLINKCOUNT 7                   /* ��� �q�b�� ����           */

static int CursorState = 0;            /* �e���� ��� �wȁ                         */
                                       /*     0 = �a���A�� ���� �g�q               */
                                       /*     1 = �w�� �a�� ( 8 x 16)                      */
                                       /*     2 = �e�i �a�� (16 x 16)                      */
static int CursorBlinkTime;            /* ���a �e��A�ᴡ �i ���� ���b      */
static bool ReverseType;      /* �e�� �b�w�� �b����; �e�巡�e �q */
static bool BlinkOnOff;

/*
** ���i ���� �a���e�a.  ���� �a���A�� ���a�e �q�b�� ���e��
** �A���e�� ��aЁ�� ��� �����i �e�寡ǥ�a.
 ! ���a�e ϩ�a ���i �� �{���� �e�A...
   �ᐡ�a�e �� ��� �a���q���e ��A�a �e�� ��á�A�� ���� �� �{�e�A..
*/
void DisplayCursor(int x, int y, int bch)
{
  int i;
  hgotoxy(x+1, y+1);
  if (CursorState == 0) {     /* �a���A�� ���� �g�a�e ���� �a�� */
    if (CurHanState) {
      CursorCode = InpStack[InpSP-1].charcode;
      CursorState = 2;
    } else {
      if (bch & 0x8000) {
        CursorCode = bch;
        CursorState = 2;
      } else {
        CursorCode = (((bch == '\r') || (bch == '\0') || (bch == '\t')) ? ' ' : bch);
        CursorState = 1;
      }
    }
    CursorBlinkTime = (int)biostime(0,0);
  }
  /*
  ** �q�b�� �១
  */
  if ((i =(int) biostime(0,0)) >= CursorBlinkTime) {
    CursorBlinkTime = i + BLINKCOUNT;
    if(CursorState) {
      BlinkOnOff = !BlinkOnOff;
      hsetreverse(BlinkOnOff);
    }
    if(CursorState == 2) {
      _hputch((CursorCode>>8) & 0xFF, CursorCode & 0xFF);
    }
    else {
      _eputch(CursorCode); /* , hwherex(), hwherey());
      hgotoxy(hwherex()+1, hwherey());                */
    }
  }
}

/*
** ���i �����a.
*/
void EraseCursor(int x, int y)
{
  if (CursorState == 0)
    return;
  if(CursorState) {
    hgotoxy(x+1, y+1);
    hsetreverse(ReverseType);
    if(CursorState == 2)
      _hputch((CursorCode>>8) & 0xFF, CursorCode & 0xFF);
    else {
      _eputch(CursorCode); /*, hwherex(), hwherey());
      hgotoxy(hwherex()+1, hwherey()); */
    }
  }
  CursorState = 0;
}

/********************************************************************
        ǡ���a������ �e���� ���a�i ���b�h�e�a.
********************************************************************/

int GetHanCh(int x, int y, int *ch, int bch)

/*--------------------------------------------------------------------

��  ��: x           ���i �a���i ��á�� x ���a (�a���a �e��)
               y           ���i �a���i ��á�� y ���a (���a �e��)
               ch       ���b�E ���a�� š�a�i ��w�i �A���� ����
               bch      ���i �a���i ��á�A �e�� �a���E ���a š�a

�e�ňt: ���b�E �i��
               HR_ASC     ���a š�a 1-127�巁 ���e �a�aǡ ���a ���e
                                ���a š�a 256 ���w�� �b�� ǡ�a ���b�Q.
               HR_HANST   �e�i ���b�� ���b�A���q�i �a�a���a.
               HR_HANIN   �e�i ���b�� �����A�� ���q�i �a�a���a.
               HR_HANEND  �e�i ���b�� �{�v�q�i �a�a���a.
               HR_HANBS   �e�i ���b ���A <BS>�a ���b�A���q�i �a�a���a.
                                �a��a �e�i�e ���� ���s �����a.
               HR_NOHAN   �e�i ���b���A <BS>�� ���� �������� �a����
                                ���w�� �q�� �g�v�q�i �a�a���a.
--------------------------------------------------------------------*/
{
  int i, key, result, ar;
  static int hanstart = FALSE;
  int noctrlcode;

  noctrlcode = _noctrlcode;
  _noctrlcode = true;

  BlinkOnOff = ReverseType = isreverse();
  for (;;) {
    if (hanstart) {
      hanstart = FALSE;
      result = HR_HANST;
      *ch = InpStack[InpSP-1].charcode;
      break;      /* for ���a�i �a�a �a�e�a. */
      /*
      ** hanstart �e���e �a����A �e�i �e �i�a�a �Ŭ��A����,
      ** �Ŭ��A�� �q�e �a���i �a���� ���� �a�q �i�a�i ���sЁ
      ** �a�i ϩ�a�a ���i �� TRUE�� ����E�a.  �� �e���e
      ** �����e �w���A HR_HANST�i ���a �� ����a�� �a�e�A���a.
      */
    }
    DisplayCursor(x, y, bch);
    if (!InKeyBuf()) {
      continue;   /* for ���a�i �e�� (ǡ���a ���b�i ���a��) */
    }
    EraseCursor(x, y);
    key = *ch = GetKey(TRUE);
    if (key == '\b') {
      if (InpSP) {
        InpSP--;
        if (InpSP) {
          i = InpSP - 1;
          CurHanState = InpStack[i].curhanst;
          *ch = InpStack[i].charcode;
          result = HR_HANBS;
        } else {
          result = HR_NOHAN;
          CurHanState = 0;
        }
      } else {
        result = HR_ASC;
      }
    } else if (key >= 128 && key <= 255) {
      i = CurHanState;
      ar = HanAutomata(key);
      *ch = InpStack[InpSP-1].charcode;
      if (ar) {               /* �e�i ���b�� �{ */
        CurHanState = 0;
        InpSP = 0;
        result = HR_HANEND;
        if (OutSP) {
          hanstart = TRUE;
          while (OutSP) HanAutomata(OutStack[--OutSP]);
        }
      } else if (i == 0) {
        /*
        ** �a�� �巁 ��ɡ�a�a �wȁ�a 0������, �w�q �e�i
        ** ���b�� �i�� �֔a�e ������ �e�i�� ���b���a.
        */
        result = HR_HANST;
      } else {
        result = HR_HANIN;
      }
    } else if (CurHanState) {
      /*
      ** �e�� �e�i�i ���s���� �wȁ�A�� �e�i �a�� ���A�� ǡ�a
      ** ���b�A���a�e ���q�a�� ���s�E �e�i�i �Ŭ��E ���a�� �b�a��,
      ** �w�q ���b�E ǡ�e ����A ���� �a�q�a�� �១�A���� ���.
      */
      PutKey(key);
      *ch = InpStack[InpSP-1].charcode;
      result = HR_HANEND;
      CurHanState = InpSP = 0;
    } else {
      result = HR_ASC;
    }
    break;  /* for ���a�i ����q */
  }
  _noctrlcode = noctrlcode;
  return result;
}

/*
** ǡ���a ����i �����a.  �e�� �e�� ���s���� �e�i�� ���a�e
** �a���a�� �����a.
*/
void EraseKeyBuf(void)
{
  CurHanState = InpSP = OutSP = 0;
  while (InKeyBuf()) GetKey(FALSE);
}

/*
 *  khinput.c
 */