#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

#include "hscreen.h"
#include "hinput.h"

#define TRUE  1
#define FALSE 0

void InputString(void)
{
	char s[80];

	PutStrB(0, 60,  "GetStr() �q���� �A�a�a:");
	GetStr(24, 60, s, 40);
	PutStrB(0, 100, "EditStr() �q���� �A�a�a:");
	EditStr(25, 100, s, 40, TRUE);
	PutStrB(0, 150, "���b�e ���w:");
	PutStrB(13, 150, s);
	PutStrB(0, 200, "�a�� ǡ�a á�A�a...");
	GetKey(FALSE);
}

void main(void)
{
	int i, vid;

	if ((vid = DetectVideoCard()) == NONE) {
		printf("Unrecognized video adapter.\n");
		exit(0);
	}
	InitGraphMode(vid, 0);			/* ���e�i �a��Ϣ ���a�� */
	HanKbdKind = HANKBD2;			/* ���� �a�e ���鯢		*/
	InputString();					/* ���a�i ���b �A�a�a	*/
	CloseGraphMode();
}
