/*
 *  �a��   : kprn.h
 *  ���w   :
 *  ���B   : hantle.prj, nal.prj
 *  �i�a   : 93.03.16
 *  �e�e�� : �e�����b�a ��aɷ�����b�� �e�i�a�� �����b
 *  �i���i : �� Ή�a���� ���a���e �����i ��â�a�� �s���a.
 *           �ᐡ�a�e, �a�� �a�e ���a���� �����i �g�� �h�v�a���a�a.
 *           �a�a��, �� ���a���i �h�a�� ���e �a�e�A �a�w�a�a�� �A���e
 *           ���� �e�e ���a���� �i �����a�a�� ����Ё ������ �a�s���a.
 */


#define  PAPERWIDTH   106
#define  PAPERLINE    55

#define  DATA2PRN     0
#define  DATA2FILE    1

extern int prn_port;
extern int formfeed_mode;

bool data_print(unsigned char *start, unsigned char *end);

/*
 *  �a�� kprn.h �� �{
 */
