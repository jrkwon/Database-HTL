#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "hanlib.h"
#include "ascii.h"
#include "hanin.h"
#include "handbms.h"
#include "hanerror2.h"
#include "hancar.h"
#include "hanpop.h"

extern char *sc;   /* ���b ���e ���� */
extern void (*_disp_card)();

char *main_title = "ԃ   ��   ��   ��   ԃ";

char *main_menu[] = {
  "   1. ��  ��  ��  ��    ",
  "   2. ��  �q  ��  ��    ",
  "   3. ��  ��  ��  �e ?  ",
  "   4. �{  �a  á  ��    "
};

char *cust_title = "ԃ �� �� �� �� ԃ";

char *cust_menu[] = {
  "   1. �a �a �� ��       ",
  "   2. �a �a �x ��       ",
  "   3. �a �a �� á ��    ",
  "   4. �a �a �� �� ��    ",
  "   5. �� �� �a �� �a    "
};

char *inst_title = "ԃ �� �q �� �� ԃ";

char *inst_menu[] = {
  "   1. �a �a �� ��       ",
  "   2. �a �a �x ��       ",
  "   3. �a �a �� á ��    ",
  "   4. �a �a �� �� ��    ",
  "   5. �� �� �a �� �a    "
};

char *search_title = "ԃ �x �� �� Ȃ ԃ";

char *search_menu[] = {
  "   1. �� �q �a �� ..    ",
  "   2. �����w����ѡ��..  ",
  "   3. �� �� �a �� �a    "
};

char *search_menu_date[] = {
  "   1. �� �q �a �� ..    ",
  "   2. �����w����ѡ��..  ",
  "   3. ����a�q����..
  "   4. �� �� �a �� �a    "
};

char *modify_title = "ԃ �� á �� �� Ȃ ԃ";
char *del_title = "ԃ �� �� �� �� Ȃ ԃ";

static int isexit = 0;

int fl[] = { CUSTOMER, INSTSAVE, -1 };
int ele[] = { 0, -1 };
struct coord_t customer_crd[] = {
  { 18, 2 }, { 51, 2 }, { 18, 3 }, { 51, 3 }, { 18, 4 },
  { 18, 5 }, { 51, 5 }, { 18, 6 }, { 51, 6 }, { 62, 6 },
  { 18, 7 }, { 51, 7 }, { 62, 7 },
  { 18, 9 }, { 31, 9 }, { 43, 9 }, { 55, 9 }, { 66, 9 },
  { 18,10 }, { 31,10 }, { 43,10 }, { 55,10 }, { 66,10 },
  { 18,11 }, { 31,11 }, { 43,11 }, { 55,11 }, { 66,11 },
  { 18,12 }, { 31,12 }, { 43,12 }, { 55,12 }, { 66,12 },
  { 66,14 }, { 18,19 }, { 51,19 }, { 18,20 }, { 51,20 }
};
struct coord_t instsave_crd[] = {
  { 18, 3 }, { 51, 3 }, { 18, 6 }, { 51, 6 }, { 18, 9 },
  { 18,12 }, { 18,15 }, { 51,15 }, { 18,18 }
};

char str[80], num[80], year_month[40];    /* �a�a �x���i ���e ���b ���� */

/*
 *  �q�� �a��ɡ �a�� ���
 */

void disp_card(void);
int recalc_total(void);
int recalc_person(void);
int recalc_thing(void);
int recalc_loss(void);
int recalc_car(void);
void init_edit(void);
void cust_input(void);
void cust_search_by_name(void);
void cust_search_by_number(void);
void cust_search(void);
void cust_modify_by_name(void);
void cust_modify_by_number(void);
void cust_modify(void);
void cust_del_by_name(void);
void cust_del_by_number(void);
void cust_del(void);
void cust_man(void);
void inst_input(void);
void inst_search_by_number(void);
void inst_search(void);
void inst_modify_by_name(void);
void inst_modify_by_number(void);
void inst_modify(void);
void inst_del_by_name(void);
void inst_del_by_number(void);
void inst_del(void);
void inst_man(void);
void repair_index(void);
void quit(void);
void background(void);

/* �������e�A �� �a�a���ᴡ�a�e ���� */
void disp_card(void)
{
  bool onoff;

  eputchs(1,  8, 78, '�');
  eputchs(1, 13, 78, '�');
  eputchs(1, 15, 78, '�');
  eputchs(1, 18, 78, '�');
  hputsxy(60, 6, "-");
  hputsxy(60, 7, "-");
  hputsxy(29,  9, "x");
  hputsxy(29, 10, "x");
  hputsxy(29, 11, "x");
  hputsxy(29, 12, "x");
  hputsxy(41,  9, "x");
  hputsxy(41, 10, "x");
  hputsxy(41, 11, "x");
  hputsxy(41, 12, "x");
  hputsxy(53,  9, "x");
  hputsxy(53, 10, "x");
  hputsxy(53, 11, "x");
  hputsxy(53, 12, "x");
  hputsxy(35,  9, "%");
  hputsxy(35, 10, "%");
  hputsxy(35, 11, "%");
  hputsxy(35, 12, "%");
  hputsxy(47,  9, "%");
  hputsxy(47, 10, "%");
  hputsxy(47, 11, "%");
  hputsxy(47, 12, "%");
  hputsxy(59,  9, "%");
  hputsxy(59, 10, "%");
  hputsxy(59, 11, "%");
  hputsxy(59, 12, "%");
  hputsxy(62,  9, "=");
  hputsxy(62, 10, "=");
  hputsxy(62, 11, "=");
  hputsxy(62, 12, "=");
  hputsxy(62, 14, "+");
  onoff = isbold();
  hsetbold(ON);
  hputsxy(5, 14, "�a���a��");
  hputsxy(33, 17, "���� ����a");
  hsetbold(onoff);
}

/*
 *   �� ���eЁ���a�e ����
 */
int recalc_total(void)
{
  long p0, p1, p2, p3;
  long sum;
  char val[25];

  p0 = atol(del_comma(sc + epos(CUST_PERSON4, file_ele[CUSTOMER])));
  p1 = atol(del_comma(sc + epos(CUST_THING4, file_ele[CUSTOMER])));
  p2 = atol(del_comma(sc + epos(CUST_LOSS4, file_ele[CUSTOMER])));
  p3 = atol(del_comma(sc + epos(CUST_CAR4, file_ele[CUSTOMER])));

  ins_comma(sc + epos(CUST_PERSON4, file_ele[CUSTOMER]));
  ins_comma(sc + epos(CUST_THING4, file_ele[CUSTOMER]));
  ins_comma(sc + epos(CUST_LOSS4, file_ele[CUSTOMER]));
  ins_comma(sc + epos(CUST_CAR4, file_ele[CUSTOMER]));

  sum = p0 + p1 + p2 + p3;
  ltoa(sum, val, 10);
  ins_left_space(val, 10-strlen(val));
  ins_comma(val);
  strcpy(sc + epos(CUST_TOTAL, file_ele[CUSTOMER]), val);
  put_field(CUST_TOTAL);
  return DBOK;
}

int recalc_person(void)
{
  long p0, p1, p2, p3;
  long sum;
  char val[25];

  p0 = atol(del_comma(sc + epos(CUST_PERSON0, file_ele[CUSTOMER])));
  p1 = atol(del_comma(sc + epos(CUST_PERSON1, file_ele[CUSTOMER])));
  p2 = atol(del_comma(sc + epos(CUST_PERSON2, file_ele[CUSTOMER])));
  p3 = atol(del_comma(sc + epos(CUST_PERSON3, file_ele[CUSTOMER])));

  ins_comma(sc + epos(CUST_PERSON0, file_ele[CUSTOMER]));
  ins_comma(sc + epos(CUST_PERSON1, file_ele[CUSTOMER]));
  ins_comma(sc + epos(CUST_PERSON2, file_ele[CUSTOMER]));
  ins_comma(sc + epos(CUST_PERSON3, file_ele[CUSTOMER]));

  sum = p0 * p1/100 * p2/100 * p3/100;
  ltoa(sum, val, 10);
  ins_left_space(val, 10-strlen(val));
  ins_comma(val);
  strcpy(sc + epos(CUST_PERSON4, file_ele[CUSTOMER]), val);
  recalc_total();
  put_field(CUST_PERSON4);
  return DBOK;
}

int recalc_thing(void)
{
  long p0, p1, p2, p3;
  long sum;
  char val[25];

  p0 = atol(del_comma(sc + epos(CUST_THING0, file_ele[CUSTOMER])));
  p1 = atol(del_comma(sc + epos(CUST_THING1, file_ele[CUSTOMER])));
  p2 = atol(del_comma(sc + epos(CUST_THING2, file_ele[CUSTOMER])));
  p3 = atol(del_comma(sc + epos(CUST_THING3, file_ele[CUSTOMER])));

  ins_comma(sc + epos(CUST_THING0, file_ele[CUSTOMER]));
  ins_comma(sc + epos(CUST_THING1, file_ele[CUSTOMER]));
  ins_comma(sc + epos(CUST_THING2, file_ele[CUSTOMER]));
  ins_comma(sc + epos(CUST_THING3, file_ele[CUSTOMER]));

  sum = p0 * p1/100 * p2/100 * p3/100;
  ltoa(sum, val, 10);
  ins_left_space(val, 10-strlen(val));
  ins_comma(val);
  strcpy(sc + epos(CUST_THING4, file_ele[CUSTOMER]), val);
  recalc_total();
  put_field(CUST_THING4);
  return DBOK;
}

int recalc_loss(void)
{
  long p0, p1, p2, p3;
  long sum;
  char val[25];

  p0 = atol(del_comma(sc + epos(CUST_LOSS0, file_ele[CUSTOMER])));
  p1 = atol(del_comma(sc + epos(CUST_LOSS1, file_ele[CUSTOMER])));
  p2 = atol(del_comma(sc + epos(CUST_LOSS2, file_ele[CUSTOMER])));
  p3 = atol(del_comma(sc + epos(CUST_LOSS3, file_ele[CUSTOMER])));

  ins_comma(sc + epos(CUST_LOSS0, file_ele[CUSTOMER]));
  ins_comma(sc + epos(CUST_LOSS1, file_ele[CUSTOMER]));
  ins_comma(sc + epos(CUST_LOSS2, file_ele[CUSTOMER]));
  ins_comma(sc + epos(CUST_LOSS3, file_ele[CUSTOMER]));

  sum = p0 * p1/100 * p2/100 * p3/100;
  ltoa(sum, val, 10);
  ins_left_space(val, 10-strlen(val));
  ins_comma(val);
  strcpy(sc + epos(CUST_LOSS4, file_ele[CUSTOMER]), val);
  recalc_total();
  put_field(CUST_LOSS4);
  return DBOK;
}

int recalc_car(void)
{
  long p0, p1, p2, p3;
  long sum;
  char val[25];

  p0 = atol(del_comma(sc + epos(CUST_CAR0, file_ele[CUSTOMER])));
  p1 = atol(del_comma(sc + epos(CUST_CAR1, file_ele[CUSTOMER])));
  p2 = atol(del_comma(sc + epos(CUST_CAR2, file_ele[CUSTOMER])));
  p3 = atol(del_comma(sc + epos(CUST_CAR3, file_ele[CUSTOMER])));

  ins_comma(sc + epos(CUST_CAR0, file_ele[CUSTOMER]));
  ins_comma(sc + epos(CUST_CAR1, file_ele[CUSTOMER]));
  ins_comma(sc + epos(CUST_CAR2, file_ele[CUSTOMER]));
  ins_comma(sc + epos(CUST_CAR3, file_ele[CUSTOMER]));

  sum = p0 * p1/100 * p2/100 * p3/100;
  ltoa(sum, val, 10);
  ins_left_space(val, 10-strlen(val));
  ins_comma(val);
  strcpy(sc + epos(CUST_CAR4, file_ele[CUSTOMER]), val);
  recalc_total();
  put_field(CUST_CAR4);
  return DBOK;
}

/*
 *  ��� �w�� ���b ҁ ��З�i �q�� ���
 */
void init_edit(void)
{
  edit(CUST_PERSON0, recalc_person);
  edit(CUST_PERSON1, recalc_person);
  edit(CUST_PERSON2, recalc_person);
  edit(CUST_PERSON3, recalc_person);
  edit(CUST_THING0, recalc_thing);
  edit(CUST_THING1, recalc_thing);
  edit(CUST_THING2, recalc_thing);
  edit(CUST_THING3, recalc_thing);
  edit(CUST_LOSS0, recalc_loss);
  edit(CUST_LOSS1, recalc_loss);
  edit(CUST_LOSS2, recalc_loss);
  edit(CUST_LOSS3, recalc_loss);
  edit(CUST_CAR0, recalc_car);
  edit(CUST_CAR1, recalc_car);
  edit(CUST_CAR2, recalc_car);
  edit(CUST_CAR3, recalc_car);
}

/*********************************************************************
 *                        �� �� �a �a �� ��                          *
 *********************************************************************/

/*
 *  �a�a ���b
 */
void cust_input(void)
{
  _disp_card = disp_card;
  db_input(CUSTOMER, "��  ��  �a  �a  ��  �b", ele, customer_crd);
  _disp_card = NULL;
  background();
}

/*
 *  ���q�a�� �x��
 */
void cust_search_by_name(void)
{
  int ans;

  ans = win_hgetdata(10, " �x�i �a�q�� ���q�i ���b�a�A�a ", "XXXXXXXXXX", str);
  if(ans) {
    _disp_card = disp_card;
    db_search(CUSTOMER, "��  ��  �a  �a  ��  ��", ele, customer_crd, 2, str);
    _disp_card = NULL;
    background();
  }
}

/*
 *  �����w����ѡ�� �x��
 */
void cust_search_by_number(void)
{
  int ans;

  ans = win_hgetdata(13, " �x�i �a�q�� �����w����ѡ�i ���b�a�A�a ", "999999-9999999", num);
  if(ans) {
    _disp_card = disp_card;
    db_search(CUSTOMER, "��  ��  �a  �a  ��  ��", ele, customer_crd, 1, num);
    _disp_card = NULL;
    background();
  }
}

void cust_search_by_date(void)
{
  int ans;

  ans = win_hgetdata(4, " �x�i ����a�q �e.�� �i ���b�a�A�a ", "99.##", year_month);
  if(ans) {
    db_browse(CUSTOMER, " �� �� �a �q �� �� ", ele, 0, year_month);
  }
}

/*
 *  �a�a �x��
 */
void cust_search(void)
{
  int ans, isquit = 0;
  void (*func[])(void) = {
    cust_search_by_name, cust_search_by_number, cust_search_by_date
  };

  do {
    ans = popup(hgetmaxax()/2-12, 10, 3, search_menu_date, search_title, "1234", 1);
    if(ans == -1 || ans == 3)
      isquit = 1;
    else
      func[ans]();
  } while(!isquit);
}

/*
 * ���q�a�� �x�a�� ����
 */
void cust_modify_by_name(void)
{
  int ans;

  ans = win_hgetdata(10, " �����i �a�q�� ���q�i ���b�a�A�a ", "XXXXXXXXXX", str);
  if(ans) {
    _disp_card = disp_card;
    db_search(CUSTOMER, "��  ��  �a  �a  ��  ��", ele, customer_crd, 2, str);
    _disp_card = NULL;
    background();
  }
}

/*
 *  �����w����ѡ�� �x�a�� ����
 */
void cust_modify_by_number(void)
{
  int ans;

  ans = win_hgetdata(13, " �����i �a�q�� �����w����ѡ�i ���b�a�A�a ", "999999-9999999", num);
  if(ans) {
    _disp_card = disp_card;
    db_search(CUSTOMER, "��  ��  �a  �a  ��  ��", ele, customer_crd, 1, num);
    _disp_card = NULL;
    background();
  }
}

/*
 *  �a�a ��á��
 */
void cust_modify(void)
{
  int ans, isquit = 0;
  void (*func[])(void) = {
    cust_modify_by_name, cust_modify_by_number
  };

  do {
    ans = popup(hgetmaxax()/2-12, 10, 3, search_menu, modify_title, "123", 1);
    if(ans == -1 || ans == 2)
      isquit = 1;
    else
      func[ans]();
  } while(!isquit);
}

/*
 *  ���q�a�� �x�a�� ������
 */
void cust_del_by_name(void)
{
  int ans;

  ans = win_hgetdata(10, " ���� �a�q�� ���q�i ���b�a�A�a ", "XXXXXXXXXX", str);
  if(ans) {
    _disp_card = disp_card;
    db_delete(CUSTOMER, "��  ��  �a  �a  ��  ��  ��", ele, customer_crd, 2, str);
    _disp_card = NULL;
    background();
  }
}

/*
 *  �����w����ѡ�� �x�a�� ������
 */
void cust_del_by_number(void)
{
  int ans;

  ans = win_hgetdata(13, " ���� �a�q�� �����w����ѡ�i ���b�a�A�a ", "999999-9999999", num);
  if(ans) {
    _disp_card = disp_card;
    db_delete(CUSTOMER, "��  ��  �a  �a  ��  ��  ��", ele, customer_crd, 1, num);
    _disp_card = NULL;
    background();
  }
}

/*
 *  �a�a ������
 */
void cust_del(void)
{
  int ans, isquit = 0;
  void (*func[])(void) = {
    cust_del_by_name, cust_del_by_number
  };

  do {
    ans = popup(hgetmaxax()/2-12, 10, 3, search_menu, del_title, "123", 1);
    if(ans == -1 || ans == 2)
      isquit = 1;
    else
      func[ans]();
  } while(!isquit);
}

/*
 *  ���� �ŝe �a�a �ş�
 */
void cust_man(void)
{
  int ans, isquit = 0;
  void (*func[])(void) = {
    cust_input, cust_search, cust_modify, cust_del
  };

  do {
    ans = popup(hgetmaxax()/2-12, 10, 5, cust_menu, cust_title, "12345", 1);
    if(ans == -1 || ans == 4)
      isquit = 1;
    else
      func[ans]();
  } while(!isquit);
}

/*********************************************************************
 *                        �� �q �a �a �� ��                          *
 *********************************************************************/

/*
 *   �a�a ���b
 */
void inst_input(void)
{
  db_input(INSTSAVE, "��  �q  �a  �a  ��  �b", ele, instsave_crd);
  background();
}

/*
 *  ���q�a�� �x��
 */
 void inst_search_by_name(void)
{
  int ans;

  ans = win_hgetdata(10, " �x�i �a�q�� ���q�i ���b�a�A�a ", "XXXXXXXXXX", str);
  if(ans) {
    db_search(INSTSAVE, "��  �q  �a  �a  ��  ��", ele, instsave_crd, 2, str);
    background();
  }
}

/*
 *  �����w����ѡ�� �x��
 */
void inst_search_by_number(void)
{
  int ans;

  ans = win_hgetdata(13, " �x�i �a�q�� �����w����ѡ�i ���b�a�A�a ", "999999-9999999", num);
  if(ans) {
    db_search(INSTSAVE, "��  �q  �a  �a  ��  ��", ele, instsave_crd, 1, num);
    background();
  }
}

/*
 *  �a�a �x��
 */
void inst_search(void)
{
  int ans, isquit = 0;
  void (*func[])(void) = {
    inst_search_by_name, inst_search_by_number
  };

  do {
    ans = popup(hgetmaxax()/2-12, 10, 3, search_menu, search_title, "123", 1);
    if(ans == -1 || ans == 2)
      isquit = 1;
    else
      func[ans]();
  } while(!isquit);
}

/*
 *  ���q�a�� �a�a�i �x�a�� ����
 */
void inst_modify_by_name(void)
{
  int ans;

  ans = win_hgetdata(10, " �����i �a�q�� ���q�i ���b�a�A�a ", "XXXXXXXXXX", str);
  if(ans) {
    db_search(INSTSAVE, "��  �q  �a  �a  ��  ��", ele, instsave_crd, 2, str);
    background();
  }
}

/*
 *  �����w����ѡ�� �x�a�� ����
 */
void inst_modify_by_number(void)
{
  int ans;

  ans = win_hgetdata(13, " �����i �a�q�� �����w����ѡ�i ���b�a�A�a ", "999999-9999999", num);
  if(ans) {
    db_search(INSTSAVE, "��  �q  �a  �a  ��  ��", ele, instsave_crd, 1, num);
    background();
  }
}

/*
 *  �a�a ����
 */
void inst_modify(void)
{
  int ans, isquit = 0;
  void (*func[])(void) = {
    inst_modify_by_name, inst_modify_by_number
  };

  do {
    ans = popup(hgetmaxax()/2-12, 10, 3, search_menu, modify_title, "123", 1);
    if(ans == -1 || ans == 2)
      isquit = 1;
    else
      func[ans]();
  } while(!isquit);
}

/*
 *  ���q�a�� �a�a�i �x�a�� ������
 */
void inst_del_by_name(void)
{
  int ans;

  ans = win_hgetdata(10, " ���� �a�q�� ���q�i ���b�a�A�a ", "XXXXXXXXXX", str);
  if(ans) {
    db_delete(INSTSAVE, "��  �q  �a  �a  ��  ��  ��", ele, instsave_crd, 2, str);
    background();
  }
}

/*
 *  �����w����ѡ�� �x�a�� ������
 */
void inst_del_by_number(void)
{
  int ans;

  ans = win_hgetdata(13, " ���� �a�q�� �����w����ѡ�i ���b�a�A�a ", "999999-9999999", num);
  if(ans) {
    db_delete(INSTSAVE, "��  �q  �a  �a  ��  ��  ��", ele, instsave_crd, 1, num);
    background();
  }
}

/*
 *  �a�a ������
 */
void inst_del(void)
{
  int ans, isquit = 0;
  void (*func[])(void) = {
    inst_del_by_name, inst_del_by_number
  };

  do {
    ans = popup(hgetmaxax()/2-12, 10, 3, search_menu, del_title, "123", 1);
    if(ans == -1 || ans == 2)
      isquit = 1;
    else
      func[ans]();
  } while(!isquit);
}

/*
 *  ��q �a�a �ş�
 */
void inst_man(void)
{
  int ans, isquit = 0;
  void (*func[])(void) = {
    inst_input, inst_search, inst_modify, inst_del
  };

  do {
    ans = popup(hgetmaxax()/2-12, 10, 5, inst_menu, inst_title, "12345", 1);
    if(ans == -1 || ans == 4)
      isquit = 1;
    else
      func[ans]();
  } while(!isquit);
}

/*
 *  �a���a�� ����
 */
void introduct(void)
{
  if(wopen(21, 10, 40, 8)) {
    wcenter(2, "Ԅ  �� �� ��  Ԅ");
    wcenter(4, "���弁 �e");
    wcenter(5, "��b�� (C) 1992");
    wcenter(6, "�e�����b�a �������w�e����");
    wcenter(7, "��  ��  �b");
    wait_getkey(5);
    wclose();
  }
}

/*
 *  �aá��
 */
void quit(void)
{
  if(yesno(" ���i �{�����s���a ? ", 1) == 0)
    isexit = 1;
}

void background(void)
{
  bool onoff, onoff2;

  hclrscr();
  onoff = isreverse();
  onoff2 = isunder();
  hsetreverse(ON);
  eputchs(-1, -hgetmaxax(), 80, ' ');
  hputsxy(-(hgetmaxax()-5), -hgetmaxay(), "1992");
  hsetunder(ON);
  eputchs(-1, -1, 80, ' ');
  hputsxy(-49, -1, "�� �e�����b�a ��aɷ�����b�� ��");
  hsetreverse(onoff);
  hsetunder(onoff2);
}

/*
 *  Ή�a�� �� ����
 */
void main(void)
{
  void (*func[])(void) = {
    cust_man, inst_man, introduct, quit
  };
  int ans;

  inithanlib(FIXRESMODE, HANDETECT, HAN8GD2, ENGGD5);
  registeregrfont(EGR1);
  registerhgrfont(HGR1);

  db_open("", fl);

  background();
  introduct();
  init_screen(CUSTOMER, file_ele[CUSTOMER], sc);
  init_edit();
  do {
    ans = popup(hgetmaxax()/2-12, 10, 4, main_menu, main_title, "1234", 0);
    if(ans == -1)
      isexit = 1;
    else
      func[ans]();
  } while(!isexit);
  db_cls();
}


