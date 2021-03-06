/*
 *  �a��   : khauto2.c
 *  ���w   : 2�鯢 �e�i���b ��ɡ�a�a
 *  ���B   : hantle.prj, nal.prj
 *  �i�a   : 93.03.16
 *  �e�e�� : �e�����b�a ��aɷ�����b�� �e�i�a�� �����b
 *  �i���i : �� Ή�a���� ���a���e �����i ��â�a�� �s���a.
 *           �ᐡ�a�e, �a�� �a�e ���a���� �����i �g�� �h�v�a���a�a.
 *           �a�a��, �� ���a���i �h�a�� ���e �a�e�A �a�w�a�a�� �A���e
 *           ���� �e�e ���a���� �i �����a�a�� ����Ё ������ �a�s���a.
 */

#include "hkeydef.h"

#define TRUE  1
#define FALSE 0

enum HAN_STATUS {              /* �b �wȁ�i ���� */
        HS_Start,
        HS_Chosung,
        HS_Joongsung, HS_dJoongsung,
        HS_Jongsung,  HS_dJongsung,
        HS_End1,         HS_End2
};

enum HAN_CHKIND { CONSONANT,  VOWEL };

/*
** �e ���� ǡ ���b�a�a �}�a �a�a�e ���b �aȂ�� �����i ����
*/
typedef struct {
        int curhanst;         /* ��ɡ�a�a �wȁ    */
        int key;              /* ���b�E ǡ š�a   */
        int charcode;         /* ���s�E ���a š�a */
} INPSTACK;

extern INPSTACK InpStack[];   /* ���b �aȂ�� ���w     */
extern int InpSP;             /* ���b �aȂ ͡����     */
extern int OutStack[];        /* �b �aȂ ���i       */
extern int OutSP;             /* �b �aȂ ͡����     */
extern int CurHanState;       /* �e���� ��ɡ�a�a �wȁ */

static int CharCode;
static int OldKey;
static int KeyCode;

/*
** �s���q�i �����e�� ��a�e�a.
*/
static int JoongsungPair(void)
{
	static unsigned char dJoongTable[7][3] = {
		0xad, 0xa3, 0xae,		/* ��, �a, �� */
		0xad, 0xa4, 0xaf,		/* ��, ��, �� */
		0xad, 0xbd, 0xb2,		/* ��, ��, �A */
		0xb4, 0xa7, 0xb5,		/* ��, ��, �� */
		0xb4, 0xaa, 0xb6,		/* ��, �A, �� */
		0xb4, 0xbd, 0xb7, 		/* ��, ��, �� */
		0xbb, 0xbd, 0xbc		/* �a, ��, �� */
	};
	int i;

	for (i = 0; i < 7; i++) {
		if (dJoongTable[i][0] == OldKey && dJoongTable[i][1] == KeyCode)
			return (KeyCode = dJoongTable[i][2]);
	}
	return 0;
}

/*
** �s�hñ�i �����e�� ��a�e�a.
*/
static int JongsungPair(void)
{
	static unsigned char dJongTable[11][3] = {
		0x82, 0x8b, 0xc4,		/* ���b, ���� */
		0x84, 0x8e, 0xc6,		/* ���e, ���x */
		0x84, 0x94, 0xc7,		/* ���e, ӡ�} */
		0x87, 0x82, 0xca,		/* ���i, ���b */
		0x87, 0x88, 0xcb,		/* ���i, ���q */
		0x87, 0x89, 0xcc,		/* ���i, ���s */
		0x87, 0x8b, 0xcd,		/* ���i, ���� */
		0x87, 0x92, 0xce,		/* ���i, ˡ�{ */
		0x87, 0x93, 0xcf,		/* ���i, ϡ�| */
		0x87, 0x94, 0xd0,		/* ���i, ӡ�} */
		0x89, 0x8b, 0xd4,		/* ���s, ���� */
	};
	int i;

	for (i = 0; i < 11; i++) {
		if (dJongTable[i][0] == OldKey && dJongTable[i][1] == KeyCode)
			return (KeyCode = dJongTable[i][2]);
	}
	return 0;
}

/********************************************************************
	���鯢 ��ɡ�a�a
********************************************************************/

int HanAutomata2(int key)

/*--------------------------------------------------------------------
��  ��:	key = ���b�E ǡ š�a
�e�ňt:	�e �i�a�� ���s�� �{�a�e 0���A, ���� ���s�����e 1
		�Ŭ��E �i�a�� š�a�e ���b �aȂ�� �a�w �a���b�A��
		���i �� ���a.
--------------------------------------------------------------------*/
{
	int ChKind, CanBeJongsung = FALSE;
	static unsigned char Cho2Jong[] = {	/* ���� š�a�A ���w�a�e ���� š�a */
		0xc2,	/*  ���b				*/
		0xc3,	/*  �w���b				*/
		0xc5,	/*  ���e				*/
		0xc8,	/*  ���h				*/
		0x00,	/*  �w���h (Ё�w ���q)	*/
		0xc9,	/*  ���i				*/
		0xd1,	/*  ���q				*/
		0xd3,	/*  ���s				*/
		0x00,	/*  �w���s (Ё�w ���q)	*/
		0xd5,	/*  ����				*/
		0xd6,	/*  �w����				*/
		0xd7,	/*  ���w				*/
		0xd8,	/*  ���x				*/
		0x00,	/*  �w���x (Ё�w ���q)	*/
		0xd9,	/*  á�y				*/
		0xda,	/*  ǡ�z				*/
		0xdb,	/*  ˡ�{				*/
		0xdc,	/*  ϡ�|				*/
		0xdd	/*  ӡ�}				*/
    };

	if ((key & 0x60) == 0x20) {
		ChKind = VOWEL;
	} else {
		ChKind = CONSONANT;
		if (!(key == 0x86 || key == 0x8A || key == 0x8F))
			CanBeJongsung = TRUE;
	}
	if (CurHanState) {
		CharCode = InpStack[InpSP-1].charcode;
		OldKey   = InpStack[InpSP-1].key;
	} else {
		CharCode = 0x8441;
		OldKey   = 0;
	}
	KeyCode = key;
	switch (CurHanState) {
		case HS_Start:
			if (ChKind == CONSONANT)
				CurHanState = HS_Chosung;
			else
				CurHanState = HS_Joongsung;
			break;
		case HS_Chosung:
			if (ChKind == VOWEL)
				CurHanState = HS_Joongsung;
			else
				CurHanState = HS_End1;
			break;
		case HS_Joongsung:
			if (CanBeJongsung)
				CurHanState = HS_Jongsung;
			else if (JoongsungPair())
				CurHanState = HS_dJoongsung;
			else
				CurHanState = HS_End1;
			break;
		case HS_dJoongsung:
			if (CanBeJongsung)
				CurHanState = HS_Jongsung;
			else
				CurHanState = HS_End1;
			break;
		case HS_Jongsung:
			if (ChKind == CONSONANT && JongsungPair())
				CurHanState = HS_dJongsung;
			else if (ChKind == VOWEL)
				CurHanState = HS_End2;
			else
				CurHanState = HS_End1;
			break;
		case HS_dJongsung:
			if (ChKind == VOWEL)
				CurHanState = HS_End2;
			else
				CurHanState = HS_End1;
			break;
	}
	switch (CurHanState) {
		case HS_Chosung:
			CharCode = (CharCode & 0x83FF) | ((KeyCode - 0x80) << 10);
			break;
		case HS_Joongsung:
		case HS_dJoongsung:
			CharCode = (CharCode & 0xFC1F) | ((KeyCode - 0xA0) << 5);
			break;
		case HS_Jongsung:
			KeyCode = Cho2Jong[KeyCode - 0x82];
		case HS_dJongsung:
			CharCode = (CharCode & 0xFFE0) | (KeyCode - 0xC0);
			break;
		case HS_End1:
			OutStack[OutSP++] = key;
			return TRUE;
		case HS_End2:
			OutStack[OutSP++] = key;
                      OutStack[OutSP++] = OldKey;
                      InpSP--;
                      return TRUE;
        }
        InpStack[InpSP].curhanst = CurHanState;
        InpStack[InpSP].charcode = CharCode;
        InpStack[InpSP++].key   = key;
        return FALSE;
}

/*
 * khauto2.c
 */
