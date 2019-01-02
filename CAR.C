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

extern char *sc;   /* ∑≥ùb —¡°e §·Ã· */
extern void (*_disp_card)();

char *main_title = "‘É   ¶ó   ¶ó   ∑°   ‘É";

char *main_menu[] = {
  "   1. â°  àÇ  â≈  ü°    ",
  "   2. ∏‚  ãq  â≈  ü°    ",
  "   3. ¶ó  ¶ó  ∑°  úe ?  ",
  "   4. è{  †a  √°  ã°    "
};

char *cust_title = "‘É â° àÇ â≈ ü° ‘É";

char *cust_menu[] = {
  "   1. ∏a ûa ê˝ ã°       ",
  "   2. ∏a ûa ¿x ã°       ",
  "   3. ∏a ûa â° √° ã°    ",
  "   4. ∏a ûa ª° ∂Å ã°    ",
  "   5. ∑° ∏Â ¿a ü± Œa    "
};

char *inst_title = "‘É ∏‚ ãq â≈ ü° ‘É";

char *inst_menu[] = {
  "   1. ∏a ûa ê˝ ã°       ",
  "   2. ∏a ûa ¿x ã°       ",
  "   3. ∏a ûa â° √° ã°    ",
  "   4. ∏a ûa ª° ∂Å ã°    ",
  "   5. ∑° ∏Â ¿a ü± Œa    "
};

char *search_title = "‘É ¿x ã° ¨Â »Ç ‘É";

char *search_menu[] = {
  "   1. ∑° üq ∑a ù° ..    ",
  "   2. ∫Å£•ówù¢§Â—°ù°..  ",
  "   3. ∑° ∏Â ¿a ü± Œa    "
};

char *search_menu_date[] = {
  "   1. ∑° üq ∑a ù° ..    ",
  "   2. ∫Å£•ówù¢§Â—°ù°..  ",
  "   3. •°–Ò†aàq∑©ù°..
  "   4. ∑° ∏Â ¿a ü± Œa    "
};

char *modify_title = "‘É â° √° ã° ¨Â »Ç ‘É";
char *del_title = "‘É ª° ∂Å ã° ¨Â »Ç ‘É";

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

char str[80], num[80], year_month[40];    /* ∏aûa ¿xã°üi ∂·–e ∑≥ùb §·Ã· */

/*
 *  –qÆÅ œaù°…° »a∑≥ ¨Â¥Â
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

/* ã°••—¡°eµA î· ãaùa∫Å¥·¥°–aìe ¶Å¶Ö */
void disp_card(void)
{
  bool onoff;

  eputchs(1,  8, 78, 'ƒ');
  eputchs(1, 13, 78, 'ƒ');
  eputchs(1, 15, 78, 'ƒ');
  eputchs(1, 18, 78, 'ƒ');
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
  hputsxy(5, 14, "¿aú∑àa¥Ç");
  hputsxy(33, 17, "•°π° ∂Ö∏Â¨a");
  hsetbold(onoff);
}

/*
 *   ∏Å âÅ¨e–Å¥°–aìe ¶Å¶Ö
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
 *  ¥·òÂ –w°¢ ∑≥ùb “Å ÆÅ–ó–i –qÆÅ ¨È∏˜
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
 *                        â° àÇ ∏a ûa â≈ ü°                          *
 *********************************************************************/

/*
 *  ∏aûa ∑≥ùb
 */
void cust_input(void)
{
  _disp_card = disp_card;
  db_input(CUSTOMER, "â°  àÇ  ∏a  ûa  ∑≥  ùb", ele, customer_crd);
  _disp_card = NULL;
  background();
}

/*
 *  ∑°üq∑aù° ¿xã°
 */
void cust_search_by_name(void)
{
  int ans;

  ans = win_hgetdata(10, " ¿x∑i ¨aúq∑Å ∑°üq∑i ∑≥ùb–a≠A∂a ", "XXXXXXXXXX", str);
  if(ans) {
    _disp_card = disp_card;
    db_search(CUSTOMER, "â°  àÇ  ∏a  ûa  •°  ã°", ele, customer_crd, 2, str);
    _disp_card = NULL;
    background();
  }
}

/*
 *  ∫Å£•ówù¢§Â—°ù° ¿xã°
 */
void cust_search_by_number(void)
{
  int ans;

  ans = win_hgetdata(13, " ¿x∑i ¨aúq∑Å ∫Å£•ówù¢§Â—°üi ∑≥ùb–a≠A∂a ", "999999-9999999", num);
  if(ans) {
    _disp_card = disp_card;
    db_search(CUSTOMER, "â°  àÇ  ∏a  ûa  •°  ã°", ele, customer_crd, 1, num);
    _disp_card = NULL;
    background();
  }
}

void cust_search_by_date(void)
{
  int ans;

  ans = win_hgetdata(4, " ¿x∑i •°–Ò†aàq ëe.∂© ∑i ∑≥ùb–a≠A∂a ", "99.##", year_month);
  if(ans) {
    db_browse(CUSTOMER, " •° –Ò †a àq â° àÇ ", ele, 0, year_month);
  }
}

/*
 *  ∏aûa ¿xã°
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
 * ∑°üq∑aù° ¿x¥a¨· ÆÅ∏˜
 */
void cust_modify_by_name(void)
{
  int ans;

  ans = win_hgetdata(10, " ÆÅ∏˜–i ¨aúq∑Å ∑°üq∑i ∑≥ùb–a≠A∂a ", "XXXXXXXXXX", str);
  if(ans) {
    _disp_card = disp_card;
    db_search(CUSTOMER, "â°  àÇ  ∏a  ûa  ÆÅ  ∏˜", ele, customer_crd, 2, str);
    _disp_card = NULL;
    background();
  }
}

/*
 *  ∫Å£•ówù¢§Â—°ù° ¿x¥a¨· ÆÅ∏˜
 */
void cust_modify_by_number(void)
{
  int ans;

  ans = win_hgetdata(13, " ÆÅ∏˜–i ¨aúq∑Å ∫Å£•ówù¢§Â—°üi ∑≥ùb–a≠A∂a ", "999999-9999999", num);
  if(ans) {
    _disp_card = disp_card;
    db_search(CUSTOMER, "â°  àÇ  ∏a  ûa  ÆÅ  ∏˜", ele, customer_crd, 1, num);
    _disp_card = NULL;
    background();
  }
}

/*
 *  ∏aûa â°√°ã°
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
 *  ∑°üq∑aù° ¿x¥a¨· ª°∂Åã°
 */
void cust_del_by_name(void)
{
  int ans;

  ans = win_hgetdata(10, " ª°∂â ¨aúq∑Å ∑°üq∑i ∑≥ùb–a≠A∂a ", "XXXXXXXXXX", str);
  if(ans) {
    _disp_card = disp_card;
    db_delete(CUSTOMER, "â°  àÇ  ∏a  ûa  ª°  ∂Å  ã°", ele, customer_crd, 2, str);
    _disp_card = NULL;
    background();
  }
}

/*
 *  ∫Å£•ówù¢§Â—°ù° ¿x¥a¨· ª°∂Åã°
 */
void cust_del_by_number(void)
{
  int ans;

  ans = win_hgetdata(13, " ª°∂â ¨aúq∑Å ∫Å£•ówù¢§Â—°üi ∑≥ùb–a≠A∂a ", "999999-9999999", num);
  if(ans) {
    _disp_card = disp_card;
    db_delete(CUSTOMER, "â°  àÇ  ∏a  ûa  ª°  ∂Å  ã°", ele, customer_crd, 1, num);
    _disp_card = NULL;
    background();
  }
}

/*
 *  ∏aûa ª°∂Åã°
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
 *  â°àÇ â≈ùe ∏aûa â≈ü°
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
 *                        ∏‚ ãq ∏a ûa â≈ ü°                          *
 *********************************************************************/

/*
 *   ∏aûa ∑≥ùb
 */
void inst_input(void)
{
  db_input(INSTSAVE, "∏‚  ãq  ∏a  ûa  ∑≥  ùb", ele, instsave_crd);
  background();
}

/*
 *  ∑°üq∑aù° ¿xã°
 */
 void inst_search_by_name(void)
{
  int ans;

  ans = win_hgetdata(10, " ¿x∑i ¨aúq∑Å ∑°üq∑i ∑≥ùb–a≠A∂a ", "XXXXXXXXXX", str);
  if(ans) {
    db_search(INSTSAVE, "∏‚  ãq  ∏a  ûa  •°  ã°", ele, instsave_crd, 2, str);
    background();
  }
}

/*
 *  ∫Å£•ówù¢§Â—°ù° ¿xã°
 */
void inst_search_by_number(void)
{
  int ans;

  ans = win_hgetdata(13, " ¿x∑i ¨aúq∑Å ∫Å£•ówù¢§Â—°üi ∑≥ùb–a≠A∂a ", "999999-9999999", num);
  if(ans) {
    db_search(INSTSAVE, "∏‚  ãq  ∏a  ûa  •°  ã°", ele, instsave_crd, 1, num);
    background();
  }
}

/*
 *  ∏aûa ¿xã°
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
 *  ∑°üq∑aù° ∏aûaüi ¿x¥a¨· ÆÅ∏˜
 */
void inst_modify_by_name(void)
{
  int ans;

  ans = win_hgetdata(10, " ÆÅ∏˜–i ¨aúq∑Å ∑°üq∑i ∑≥ùb–a≠A∂a ", "XXXXXXXXXX", str);
  if(ans) {
    db_search(INSTSAVE, "∏‚  ãq  ∏a  ûa  ÆÅ  ∏˜", ele, instsave_crd, 2, str);
    background();
  }
}

/*
 *  ∫Å£•ówù¢§Â—°ù° ¿x¥a¨· ÆÅ∏˜
 */
void inst_modify_by_number(void)
{
  int ans;

  ans = win_hgetdata(13, " ÆÅ∏˜–i ¨aúq∑Å ∫Å£•ówù¢§Â—°üi ∑≥ùb–a≠A∂a ", "999999-9999999", num);
  if(ans) {
    db_search(INSTSAVE, "∏‚  ãq  ∏a  ûa  ÆÅ  ∏˜", ele, instsave_crd, 1, num);
    background();
  }
}

/*
 *  ∏aûa ÆÅ∏˜
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
 *  ∑°üq∑aù° ∏aûaüi ¿x¥a¨· ª°∂Åã°
 */
void inst_del_by_name(void)
{
  int ans;

  ans = win_hgetdata(10, " ª°∂â ¨aúq∑Å ∑°üq∑i ∑≥ùb–a≠A∂a ", "XXXXXXXXXX", str);
  if(ans) {
    db_delete(INSTSAVE, "∏‚  ãq  ∏a  ûa  ª°  ∂Å  ã°", ele, instsave_crd, 2, str);
    background();
  }
}

/*
 *  ∫Å£•ówù¢§Â—°ù° ¿x¥a¨· ª°∂Åã°
 */
void inst_del_by_number(void)
{
  int ans;

  ans = win_hgetdata(13, " ª°∂â ¨aúq∑Å ∫Å£•ówù¢§Â—°üi ∑≥ùb–a≠A∂a ", "999999-9999999", num);
  if(ans) {
    db_delete(INSTSAVE, "∏‚  ãq  ∏a  ûa  ª°  ∂Å  ã°", ele, instsave_crd, 1, num);
    background();
  }
}

/*
 *  ∏aûa ª°∂Åã°
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
 *  ∏‚ãq ∏aûa â≈ü°
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
 *  œaù°ãaúë ≠°àÅ
 */
void introduct(void)
{
  if(wopen(21, 10, 40, 8)) {
    wcenter(2, "‘Ñ  ¶ó ¶ó ∑°  ‘Ñ");
    wcenter(4, "¿ı§ÂºÅ Ãe");
    wcenter(5, "∏·∏bä• (C) 1992");
    wcenter(6, "–e¥∑îÅ–bäa ∑•â∑ª°ìwµeäÅØ©");
    wcenter(7, "ä•  ∏Å  úb");
    wait_getkey(5);
    wclose();
  }
}

/*
 *  †a√°ã°
 */
void quit(void)
{
  if(yesno(" ∏˜†i è{êÅØ°ùsì°åa ? ", 1) == 0)
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
  hputsxy(-49, -1, "‘ –e¥∑îÅ–bäa ∏Â∏a…∑Ø•â∑–bâ¡ ‘");
  hsetreverse(onoff);
  hsetunder(onoff2);
}

/*
 *  Œâãaü± ∫Å ¶Å¶Ö
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