/*
 *  �a��   : kutil.h
 *  ���w   :
 *  ���B   : hantle.prj, nal.prj
 *  �i�a   : 93.03.16
 *  �e�e�� : �e�����b�a ��aɷ�����b�� �e�i�a�� �����b
 *  �i���i : �� Ή�a���� ���a���e �����i ��â�a�� �s���a.
 *           �ᐡ�a�e, �a�� �a�e ���a���� �����i �g�� �h�v�a���a�a.
 *           �a�a��, �� ���a���i �h�a�� ���e �a�e�A �a�w�a�a�� �A���e
 *           ���� �e�e ���a���� �i �����a�a�� ����Ё ������ �a�s���a.
 */

#ifndef _KUTIL_H
#define _KUTIL_H

#include "typedefs.h"

void beep(void);
int get_filename_to_read(unsigned char *name);
int get_filename_to_write(unsigned char *name);
int get_number_to_set_tab(unsigned char *name);
int get_ext_name(unsigned char *name);
char *split_filename(char *path);
char *truncate_filename(char *path);
int ask_save(void);
void new_file(void);
bool change_dir(void);

#endif

/*
 *  �a�� kutil.h �� �{
 */
