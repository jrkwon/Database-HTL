/*
 *  �a��   : khinput.c
 *  ���w   : �e������ �e�i ���b�� �១
 *  ���B   : hantle.prj, nal.prj
 *  �i�a   : 93.03.16
 *  �e�e�� : �e�����b�a ��aɷ�����b�� �e�i�a�� �����b
 *  �i���i : �� Ή�a���� ���a���e �����i ��â�a�� �q���a.
 *           �ᐡ�a�e, �a�� �a�e ���a���� �����i �g�� �h�v�a���a�a.
 *           �a�a��, �� ���a���i �h�a�� ���e �a�e�A �a�w�a�a�� �A���e
 *           ���� �e�e ���a���� �i �����a�a�� ����Ё ������ �a�s���a.
 */

#define   LSHIFT_SPACE  512
#define   RSHIFT_SPACE	513

enum HANINPRESULTS {
	HR_NOKEY,
	HR_ASC,
	HR_HANST,
	HR_HANIN,
	HR_HANEND,
	HR_HANBS,
        HR_NOHAN
};

enum { HANKBD2, HANKBD3 };

extern int HanKbdState;    /* �e/�w �wȁ, �e�i�wȁ���� �q */
extern int HanKbdKind;     /* �e�i�a�e���A 2(HANKBD2), 3�鯢(HANKBD3) */

int GetKey(int inphan);
void PutKey(int key);
int InKeyBuf(void);

void DisplayCursor(int x, int y, int bch);
void EraseCursor(int x, int y);
int GetHanCh(int x, int y, int *ch, int bch);
void EraseKeyBuf(void);
/*
 *  �a�� khinput.c �� �{
 */
