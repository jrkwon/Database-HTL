/*
 *  �a��   : kutil.c
 *  ���w   :
 *  ���B   : hantle.prj, nal.prj
 *  �i�a   : 93.03.16
 *  �e�e�� : �e�����b�a ��aɷ�����b�� �e�i�a�� �����b
 *  �i���i : �� Ή�a���� ���a���e �����i ��â�a�� �s���a.
 *           �ᐡ�a�e, �a�� �a�e ���a���� �����i �g�� �h�v�a���a�a.
 *           �a�a��, �� ���a���i �h�a�� ���e �a�e�A �a�w�a�a�� �A���e
 *           ���� �e�e ���a���� �i �����a�a�� ����Ё ������ �a�s���a.
 */

#include <stdio.h>
#include <string.h>
#include <dir.h>
#include <dos.h>
#include "hanin.h"
#include "hanerro2.h"
#include "kedit.h"
#include "kfile.h"
#include "kutil.h"

void beep(void)
{
  int cnt;

  for(cnt = 0; cnt < 5; cnt++)
    sound(cnt*50+500);
  nosound();
}

int get_filename_to_read(unsigned char *name)
{
  if(win_hgetdata(40, " ����i�� �a�����q�e ?", "EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE", name)) {
    strcuttail(name);
    strupr(name);
    return YES_ANS;
  }
  return CANCEL_ANS;
}

int get_filename_to_write( unsigned char *name)
{
  if(win_hgetdata(40, " ��w�i  �������q�e ?", "EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE", name)) {
    strcuttail(name);
    strupr(name);
    return YES_ANS;
  }
  return CANCEL_ANS;
}

int get_number_to_set_tab(unsigned char *name)
{
  if(win_hgetdata(40, " ���� �����i ȓ�� �췡�e ?", "EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE", name)) {
    strcuttail(name);
    strupr(name);
    return YES_ANS;
  }
  return CANCEL_ANS;
}

int get_ext_name(unsigned char *name)
{
  if(win_hgetdata(40, " ���� �¸w�a�e ? ", "EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE", name)) {
    strcuttail(name);
    strupr(name);
    return YES_ANS;
  }
  return CANCEL_ANS;
}

/*
 *   �w�� ���A�� �a�����q�e ���a���a.
 */
char *split_filename(char *path)
{
  int index;

  if(*path) {
    index = (strlen(path))-1;
    while(index >= 0 && path[index] != '\\' && path[index] != ':')
      index--;
    return path+index+1;
  }
  return NULL;
}

/*
 *  �w�� ���A �a�����q�i �A���e�a.
 *  C:\TC\BIN\TC.EXE  --> C:\TC\BIN\
 */
char *truncate_filename(char *path)
{
  int index;

  if(*path) {
    index = (strlen(path))-1;
    while(index >= 0 && path[index] != '\\' && path[index] != ':')
      index--;
    path[index+1] = NULL;
    return path;
  }
  return NULL;
}

/*
 *  ��w�i ������ �·�
 *  �� : 0, �a���� : 1, �᭡ : 2
 */
int ask_save(void)
{
  unsigned char msg[40];
  int ans;

  ans = 1;
  if(!saved && memory!=eofm ) {
    sprintf(msg,"%s�i ��w�i�a�a?",split_filename(work_file));
    ans = yesno(msg, YES_ANS);
    if(ans == YES_ANS)
      save();
  }
  return ans;
}

bool change_dir(void)
{
  char path[80], drive[3], dir[66], name[13], ext[4];

  sprintf(drive, "%c:", getdisk()+'A');
  getcurdir(0, dir);
  sprintf(path, "%s\\%s", drive, dir);
  if(win_hgetdata(40, " ������ �w���e ? ", "EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE", path)) {
    strcuttail(path);
    if(*path != NULL) {
      strupr(path);
      fnsplit(path, drive, dir, name, ext);
      if((*drive) != NULL) 
        setdisk(drive[0]-'A');
      if(chdir(path) == 0) 
        return true;
      else {
        errorf();
        return false;
      }
    }
    else 
      return false;
  }
  return false;  
}

/*
 *  �a�� kutil.c �� �{
 */
