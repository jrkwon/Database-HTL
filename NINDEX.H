/*
 *  �a��   : nindex.h
 *  ���w   : �i�ⷡ �����a��
 *  ���B   : nal.prj
 *  �i�a   : 93.03.16
 *  �e�e�� : �e�����b�a ��aɷ�����b�� �e�i�a�� �����b
 *  �i���i : �� Ή�a���� ���a���e �����i ��â�a�� �s���a.
 *           �ᐡ�a�e, �a�� �a�e ���a���� �����i �g�� �h�v�a���a�a.
 *           �a�a��, �� ���a���i �h�a�� ���e �a�e�A �a�w�a�a�� �A���e
 *           ���� �e�e ���a���� �i �����a�a�� ����Ё ������ �a�s���a.
 */

#define MAXDATELEN   (15)
#define MAXTITLELEN  (35)

struct date_title_t {
  char date[MAXDATELEN];   /* 93�e_05��_02�� */
  char yoil[3];            /* �a��           */
  char nalsi[13];          /* �i��           */
  char title[MAXTITLELEN]; /* ���� �A��      */
};

/*
 *  �a�� nindex.h �� �{
 */
