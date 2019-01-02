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

	PutStrB(0, 60,  "GetStr() Ðq®· ÉA¯aËa:");
	GetStr(24, 60, s, 40);
	PutStrB(0, 100, "EditStr() Ðq®· ÉA¯aËa:");
	EditStr(25, 100, s, 40, TRUE);
	PutStrB(0, 150, "·³bÐe ¶w:");
	PutStrB(13, 150, s);
	PutStrB(0, 200, "´a¢ Ç¡a Ã¡­A¶a...");
	GetKey(FALSE);
}

void main(void)
{
	int i, vid;

	if ((vid = DetectVideoCard()) == NONE) {
		printf("Unrecognized video adapter.\n");
		exit(0);
	}
	InitGraphMode(vid, 0);			/* ÑÁ¡e·i ‹aœÏ¢ ¡¡—a¡ */
	HanKbdKind = HANKBD2;			/* Á¡‹¡ ¸aÌe –¤é¯¢		*/
	InputString();					/* ¢…¸aµi ·³b ÉA¯aËa	*/
	CloseGraphMode();
}
