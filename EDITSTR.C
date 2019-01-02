static int CurLen, MaxWid, CurPos, InsertMode, Redraw;
static char *Estr;


/********************************************************************
	¢…¸aµi·i Íe»³Ðe”a.
********************************************************************/

int EditStr(int x, int y, char *str, int maxlen, int inphan)

/*--------------------------------------------------------------------
·¥  ®:	x, y	Íe»³Ði ÑÁ¡e¬w· ¶áÃ¡
		str     Íe»³Ði ¢…¸aµi
		maxlen  ¢…¸aµi· ÂA” ¤a·¡Ëa ®
		inphan  Ðe‹i·i ·³b ¤h“e”a¡e TRUE, ´a“¡¡e FALSE
¤eÑÅˆt:	<Enter>Ç¡¡ {v·a¡e 0, <ESC>¡ {v·a¡e <ESC> Ç¡ Å¡—a
--------------------------------------------------------------------*/
{
	int key, result, i, bch;
	int inpmode = inphan ? 0 : HI_EONLY;

	Estr       = str;
	MaxWid     = maxlen - 1;
	CurLen     = CurPos = strlen(Estr);
	InsertMode = Redraw = TRUE;

	PutStrB(x, y, Estr);
	while (!InKeyBuf()) DisplayCursor(x + CurPos, y, ' ');
	key = GetKey(inphan);
	EraseCursor();
	if (key >= 32 && key < 256) CurLen = CurPos = *Estr = 0;
	PutKey(key);
	for (;;) {
		if (Redraw) {
			PutStrB(x, y, Estr);
			for (i = CurLen; i < MaxWid; i++) PutChB(x + i, y, ' ');
			Redraw = FALSE;
		}
		if ((i = Estr[CurPos]) & 0x80)
			bch = (Estr[CurPos] << 8) | Estr[CurPos+1];
		else
			bch = Estr[CurPos];

		result = GetHanCh(x + CurPos, y, inpmode, &key, bch);

		if (result == HR_ASC) {
			switch (key) {
				case ENTER:
					return 0;
				case ESC:
					return ESC;
				case LSHIFT_SPACE:
					HanKbdState = !HanKbdState;
					break;
				case INS:
					InsertMode = !InsertMode;
					break;
				case LEFT:
					if (CurPos) {
						if (StrChKind(Estr, CurPos-1) == CK_HAN2)
							CurPos -= 2;
						else
							CurPos--;
					}
					break;
				case RIGHT:
					if (CurPos < CurLen) {
						if (StrChKind(Estr, CurPos) == CK_HAN1)
							CurPos += 2;
						else
							CurPos++;
					}
					break;
				case HOME:
					CurPos = 0;
					break;
				case END:
					CurPos = CurLen;
					break;
				case BS:
					if (CurPos) {
						if (StrChKind(Estr, CurPos-1) == CK_HAN2)
							CurPos -= 2;
						else
							CurPos--;
					}
					DelChStr();
					break;
				case DEL:
					DelChStr();
					break;
				default:
					if (key < 128) {
						if (!InsertMode && (Estr[CurPos] & 0x80))
							Estr[CurPos+1] = ' ';
						WriteChStr(key);
					}
					break;
            }
		} else if (result == HR_HANST) {
			i = CurPos;
			WriteChStr(key);
			CurPos = i;
		} else if (result == HR_HANIN || result == HR_HANBS) {
			Estr[CurPos]   = (key >> 8) & 0xFF;
			Estr[CurPos+1] = key & 0xFF;
		} else if (result == HR_HANEND) {
			Estr[CurPos++] = (key >> 8) & 0xFF;
			Estr[CurPos++] = key & 0xFF;
			Redraw = TRUE;
		} else if (result == HR_NOHAN) {
			DelChStr();
		}
	}
}
