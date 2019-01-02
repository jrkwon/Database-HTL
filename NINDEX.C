#define DEBUG

/*
 *  Ìa·©   : nindex.c
 *  ¶w   : i¸â·¡(·©‹¡)· ¬‚·¥ Ìa·©
 *  ‹¡ÒB   : nal.prj
 *  i¼a   : 93.03.16
 *   e—e·¡ : Ðe´·”ÐbŠa ¸å¸aÉ·¯¥‰·Ðb‰Á ÐeËi´aŸ¡ Š¥¸œb
 *  ´iŸ±‹i : ·¡ Î‰‹aŸ±· £»‹aŸ±·e ‰·ˆŸi ¶¥Ã¢·a¡ Ðs“¡”a.
 *           µá¡Ða¡e, a•¡ ”aŸe £»‹aŸ±· •¡¶‘·i  g·¡ ¤h´v·a“¡Œa¶a.
 *           ˜aœa¬á, ·¡ £»‹aŸ±·i ¤h·a¯¥ ¦…·e ”aŸe•A ¬a¶wÐa­a•¡ –A»¡ e
 *           ¬¡  e—e £»‹aŸ±•¡ ¸i ¸÷Ÿ¡Ða­a¬á ‰·ˆÐ º¯¡‹¡ ¤aœs“¡”a.
 */

#include <stdio.h>
#include <string.h>
#include <dos.h>
#include "hanin.h"
#include "ascii.h"
#include "nindex.h"

#define HDR_LEN        (32)                  /* ¸aža Ìa·©·  áŸ¡ ‹©·¡ */
#define MAX_FIELDS     (4)                   /* ÂA” Ï©—a ® */
#define MAXVIEWLINE    (15)
#define MAXVIEWWIDTH   (MAXTITLELEN+MAXDATELEN+4)

struct descriptor {
  int position;                /* AÅ¡—a · Ï©—a ¶áÃ¡ */
  int size;                    /* Ï©—a Ça‹¡ - ¤a·¡Ëa */
  int x, y;                    /* Ï©—a ¶áÃ¡ */
} fields[MAX_FIELDS];          /* Ï©—a Š¹¡ÁA */

int num_fields;                /* Ï©—a ˆ•® */
int record_size;               /* Ðe AÅ¡—a· Ça‹¡ (ˆb Ï©—a· ‹©·¡+1)· Ðs */
int sort_field;                /* ¸÷i–I Ï©—a ¤åÑ¡ */
int num_recs;                  /* Ìa·©µA¬á AÅ¡—a ˆ•® */

int recs_changed;              /* AÅ¡—aˆa ˆ—¯¥–A´öa ? */
int cur_rec_num;               /* Ñe¸ AÅ¡—a ¤åÑ¡ */

char *indexfile = "NAL.IDX";   /* i¸â·¡ ¬‚·¥ Ìa·©· ·¡Ÿq */
char *indexhead = "Nalgergy Index File Ver 1.0"; /* ¸aža Ìa·©·  áŸ¡ */

struct db_type {
  unsigned char *data;            /* ¸ažaµA ”Ðe Í¡·¥Èá */
  struct db_type *prior;          /* ·¡¸å Ÿ¡¯aËa· º­¡ */
  struct db_type *next;           /* ”a·q Ÿ¡¯aËa· º­¡ */
} *first, *last, *cur_rec;

char *picture[] = {
  "##‘e ##¶© ##·©", "HH", "XXXXXXXXXXXX", "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
};

char *buffer[] = {
  "              ", "  ", "            ", "                                  "
};

/* ************************************************************************ *
 *                        ¸aža ¬a·¡· ·¡•·                                  *
 * ************************************************************************ */

/*
 *  Àá·q ¸aža¡
 */
static struct db_type *first_data(struct db_type *p)
{
  p = first, cur_rec_num = 0;
  return p;
}

/*
 *   a»¡ b ¸aža¡
 */
static struct db_type *last_data(struct db_type *p)
{
  p = last, cur_rec_num = num_recs-1;
  return p;
}

/*
 *  ”a·q ¸aža¡
 */
static struct db_type *next_data(struct db_type *p)
{
  p = p->next, cur_rec_num++;
  return p;
}

/*
 *   ·¡¸å ¸aža¡
 */
static struct db_type *prior_data(struct db_type *p)
{
  p = p->prior, cur_rec_num--;
  return p;
}

/*
 *  i¸â·¡µA i¼aµÁ ¹A¡¢µA ”Ðe ¬‚·¥Ìa·©
 */
int init_date_index(void)
{
  int size[] = { 14, 2, 12, 34 };
  int off_pos[] = { 0, 14, 16, 28, 62 };
  int x[] = { 3, 19, 27, 39 };
  register i;

  num_fields = 4;

  record_size = 0;
  for(i = 0; i < num_fields; i++) {
    fields[i].position = off_pos[i];
    fields[i].size = size[i];
    fields[i].x = x[i];
    fields[i].y = 3;
    record_size += size[i];
  }
  sort_field = 0;   /* i¼aˆa ¸÷i· ‹¡º… */
  num_recs = 0;

  _hookhgetdata = hookhgetdata();

  return load_index() ? YES : NO;
}

bool hookhgetdata(int *keycode)
{
  if(keycode == NULL)
    return false;
  switch(*keycode) {
    case '\t' : case UPARROW : case DOWNARROW : case LEFTARROW :
    case RIGHTARROW : case SHIFT_TAB : case CTRL_PGUP : case CTRL_PGDN :
      *keycode == CR;
      break;
  }
}

/*
 *  ‹¡“w        : µ¡“i ·©‹¡· i¼aµÁ ¹A¡¢ ³a‹¡
 *  •©aº“e ˆt : Íe»³Ði Ìa·©·¡Ÿq; µ > 1993‘e 5¶© 5·© --> "930505"
 */
unsigned char *today_date_title(void)
{
  int cnt, exit_key, isquit, in_isquit, ans;
  struct db_type *p, *tmp;
  bool onoff;
  struct date today;
  char key[80];

  isquit = in_isquit = ans = 0;
  do {
    p = (struct db_type *)malloc(sizeof(struct db_type));
    p->data = (unsigned char *)calloc(1, record_size);
    if(!p || !p->data) {
      memerror();
      return;
    }
    cnt = exit_key = 0;

    getdate(&today);
    sprintf(key, "%2d‘e %2d¶© %2d·©", today.da_year % 100, today.da_mon, today.da_day);
    if(find(key, first, 0)) {
      /* µ¡“ii¼a· ·©‹¡ˆa ·¡£¡ ·¶”a */
      if(yesno("µ¡“i i¼a¡ ·©‹¡ˆa ·¡£¡ ·¶Š…¶a. Íe»³Ða¯¡‰V¯s“¡Œa ?", 0) == 0) {
        /* µ¡“i ·©‹¡Ÿi Íe»³Ða‰V”a. */
        /* i¼a, ¶a·©, i³¡, ¹A¡¢—w·i Îa¯¡Ða‰¡ Íe»³¦¦…·a¡ ˆe”a.*/
        sprintf(key, "%02d%02d%02d", today.da_year % 100, today.da_mon, today.da_day);
        return key;
      }
    }

    do {
      cnt = (cnt+num_fields) % num_fields;
      onoff = isunder();
      hsetunder(ON);
      hgotoxy(fields[cnt].x+strlen(fields[cnt].name)+1, fields[cnt].y,);
      _cuttail = false;
      hgetdata(buffer[cnt], picture[cnt]);
      /*
       *  ·³bÐe i¼aµA ·©‹¡ˆa ·¡£¡ ·¶“e»¡ ˆñ¬aÐ´¡
       */
      switch(_lastkey) {
        case '\t' :
          strcpy(&(p->data)[fields[cnt].position], buffer[cnt]);
          if(cnt < 4)
            cnt++;
          else
            cnt = 0;
          break;
        case ESC :
          in_isquit = 1;
          break;
        case '\r' :
          strcpy(&(p->data)[fields[cnt].position], buffer[cnt]);
          if(cnt == 3)
            in_isquit = ans = 1;
          else
            cnt++;
          break;
      }
    } while(!in_isquit);
    hsetunder(onoff);

    if(_lastkey == ESC) {
      free(p);
      isquit = 1;
    }
    else {
      if(p->data[0]) {
      num_recs++, recs_changed = 1;
      cur_rec = p;
      dls_store(p, &first, &last);
      cnt = 0;
      tmp = first;
      while(tmp != cur_rec) {
        tmp = tmp->next;
        cnt++;
      }
      cur_rec_num = cnt;
    }
  } while(!isquit);
  returm ans;
}

#ifndef DEBUG
/* ‹¡¹¥ ¸aža· Íe»³ */
void edit_data(void)
{
  int cnt, exit_key, isquit = NO, save_flag = NO, flag = YES, menu_flag = YES;
  struct db_type *ptr, *tmp;

  ptr = (struct db_type *)malloc(sizeof(struct db_type));
  ptr->data = (unsigned char *)calloc(1, record_size);
  if(!ptr || !ptr->data) {
    memerror();
    return;
  }
  if(num_recs != 0)
    memcpy(ptr->data, cur_rec->data, record_size);

  do {
    cnt = exit_key = 0;
    while((exit_key != ESCKEY) && (exit_key != F2KEY) && (exit_key != SWITCH_KEY)) {
      if(flag) {
        display_rec(ptr);        /* Íe»³Ði ¸ažaŸi ÑÁ¡eµA °áº…”a. */
        flag = NO;
      }
      if(cnt == num_fields) {
        exit_key = F2KEY;
        break;
      }
      cnt = (cnt+num_fields) % num_fields;
      wsetunderline(ON);
      exit_key = hgets2(fields[cnt].x+strlen(fields[cnt].name)+1, fields[cnt].y,
                        fields[cnt].size-1, fields[cnt].size-1, &(ptr->data)[fields[cnt].position],
                        YES);
      wsetunderline(OFF);
      if(findshortcut(exit_key) || (exit_key == F0KEY)) {           /* ”eÂ‚Ç¡œa¡e */
        save_flag = YES;
        isquit = YES, menu_flag = NO;
        cur_key = exit_key;
        break;
      }
      switch(exit_key) {
        case UPARROW :       /* ·¡¸å Ðw¡¢·a¡ */
          cnt--;
          break;
        case DOWNARROW : case '\r':  /* ”a·q Ðw¡¢·a¡ */
          cnt++;
          break;
        case SWITCH_KEY :
          if(num_recs != 0) {
            save_flag = isquit = YES, menu_flag = NO;
            cur_task = VIEW_ITEM;
          }
          else
            disperror("¸ažaˆa Ðaa•¡ ´ô“e•A ¢© ¥¡¯¡a‰¡¶a?");
          break;
        case F2KEY :
          break;
        case CTRLPGUPKEY :  /* ˆa¸w Àá·q ¸aža¡ */
          cur_rec = first_data(cur_rec);
          memcpy(ptr->data, cur_rec->data, record_size);
          flag = YES;
          break;
        case CTRLPGDNKEY :  /* ˆa¸w  a»¡ b ¸aža¡ */
          cur_rec = last_data(cur_rec);
          memcpy(ptr->data, cur_rec->data, record_size);
          flag = YES;
          break;
        case PGDNKEY :      /* ”a·q ¸aža¡ */
          if(cur_rec->next) {
            cur_rec = next_data(cur_rec);
            memcpy(ptr->data, cur_rec->data, record_size);
            flag = YES;
          }
          break;
        case PGUPKEY :      /* ·¡¸å ¸aža¡ */
          if(cur_rec->prior) {
            cur_rec = prior_data(cur_rec);
            memcpy(ptr->data, cur_rec->data, record_size);
            flag = YES;
          }
          break;
      }
      if(exit_key == ESCKEY) {
        isquit = YES;
        break;           /* while ¤c·a¡ ÈiÂ‰ */
      } /* ¤aŒ{ ½¢· if */
    } /* while */
    if((exit_key == F2KEY) || (save_flag)) {
      if(ptr->data[0]) {
        recs_changed = 1;
        delete(cur_rec, &first, &last);
        cur_rec = ptr;
        dls_store(ptr, &first, &last);   /* ¬ ¸aža Àñˆa */
        display_name(ptr);               /* ÍiÁa¥¡‹¡ ¦¦…·i ”a¯¡ °áº…”a. */
        cnt = 0;
        tmp = first;
        while(tmp != cur_rec) {
          tmp = tmp->next;
          cnt++;
        }
        cur_rec_num = cnt;
        if(!save_flag) {   /* ”eÂ‚Ç¡ˆa ’‰Ÿ¡»¡´g‰¡ µa‹¡Œa»¡ µÖ­¡·¡”a. */
          if(yesno("Íe»³·i  aÃ¡¯¡‰V¯s“¡Œa ?") == YES)
            isquit = YES;
        }
      }
    }
  } while(!isquit);
  cur_key = (menu_flag) ? F0KEY : exit_key;
  drawbox(CARD_X-1, CARD_Y-1, 64+2, VIEW_HEIGHT+2, L1T1R1B1, NO);
}
#endif

/*
 *  ˆb Ðw¡¢· ·¡Ÿq·i ³a‰¡ ˆbˆb· ·³b¦Ÿi »¡¶…”a.
 */
void display_fields(void)
{
  register int i, j;
  bool bold, under;

  bold = isbold();
  under = isunder();
  for ( i = 0; i < num_fields; i++) {
    hsetbold(ON);
    hputsxy(fields[i].x, fields[i].y, fields[i].name);
    hsetbold(OFF);
    hsetunder(ON);
    for(j = 0; j < fields[i].size-1; j++)
      _eputchxy(' ', fields[i].x+strlen(fields[i].name)+1+j, fields[i].y);
    hsetunder(OFF);
  }
  hsetbold(bold);
  hsetunder(under);
}

/*
 *  ·³b µwµb·i ‹aaº‰¡ ˆb Ðw¡¢· µwµbµA ¸ažaŸi Îa¯¡Ðº…”a.
 */
void display_rec(struct db_type *p)
{
  register int i, j;
  bool onoff;

  onoff = isunder();
  display_fields();
  hsetunder(ON);
  if(!p) {
    for (i = 0; i < num_fields; i++) {
      for(j = 0; j < fields[i].size-1; j++)
        _eputchxy(' ', fields[i].x+strlen(fields[i].name)+1+j, fields[i].y);
    }
    hsetunderline(onoff);
    return;
  }
  for (i = 0; i < num_fields; i++) {
    hputsxy(fields[i].x+strlen(fields[i].name)+1, fields[i].y, p->data+fields[i].position);
    for(j = strlen(p->data+fields[i].position); j < fields[i].size-1; j++)
      _eputchxy(' ', fields[i].x+strlen(fields[i].name)+1+j, fields[i].y);
  }
  hsetunder(onoff);
}

/*
 *  keyµA¬á ‰·¤‚·i ¹AˆáÐe”a.
 */
char *del_space(unsigned char *key)
{
  char str[80]; /* maximum key length is 80 */
  register i = 0;

  while(*key) {
    if(!isspace(*key)) /* !caustion : key is must be unsigned char */
      str[i++] = *key;
    key++;
  }
  str[i] = '\0';
  key = str;
  return str;
}

/* ************************************************************************ *
 *                       ¯©¹A ¸ažaŸi ÀáŸ¡Ða“e ¦¦…                          *
 * ************************************************************************ */

/*
 *  ¸ažaŸi 2»¥ a¢´eµA ¡¶¡ ý“e”a.
 */
void dls_store(struct db_type *i, struct db_type **first, struct db_type **last)
{
  struct db_type *old, *p;

  if (*last == NULL) {    /* Ÿ¡¯aËaµA Àá·q Àñˆa –A“e ‰w¶ */
    i->next =  NULL;
    i->prior = NULL;
    *last = i ;
    *first = i;
    return;
  }

  p = *first;             /* Ÿ¡¯aËa ¢”‹¡µA¬á ¯¡¸b */
  old = NULL;
  while(p) {              /* ¸ažaŸi ¡¶¡ ý·i ¶áÃ¡Ÿi Àx“e”a. */
    if(strcmp(p->data + fields[sort_field].position,
              i->data + fields[sort_field].position) < 0 ) {
      old = p;
      p = p->next;
    }
    else {
      if (p->prior) {      /* Ÿ¡¯aËa º—ˆeµA¬á ‰­¢ */
        p->prior->next = i;
        i->next = p;
        i->prior = p->prior;
        p->prior = i;
        return;
      }
      i->next = p;          /* Àñˆa */
      i->prior = NULL;
      p->prior = i;
      *first = i;
      return;
    }
  } /* while */
  old->next = i;              /* {µA ý“e”a */
  i->next = NULL;
  i->prior = old;
  *last = i;
}

/*
 *  ¸aža ÍiÁa¥¡‹¡
 *
 *  i¼aµÁ ¹A¡¢ e Îa¯¡
 *  15º‰ e Îa¯¡Ða¸a
 */
void browse_data(void)
{
  struct db_type *info, *temp_rec, *temp_info;
  int cur_row = 0, key, temp_num, i, len;
  bool onoff;

  temp_num = cur_rec_num;
  temp_rec = cur_rec;
  if (cur_rec) info = cur_rec;
  else info = first;

  display_name(info);

  if(!info) {
    disperror("¸ažaˆa Ðaa•¡ ´ô“e•A ¢© ¥¡¯¡a‰¡?");
    return;
  }
  if(!wopen((hgetmaxax()-MAXVIEWWIDTH)/2, 5, MAXVIEWWIDTH, MAXVIEWLINE))
    return;

  onoff = isreverse();
  while (info) {
    hsetreverse(ON);
    hprintfxy(1, 1+cur_row, " %s  %s ", info->data, info->data+field[3].position);
    key = getchext();
    hsetreverse(OFF);
    hprintfxy(1, 1+cur_row, " %s  %s ", info->data, info->data+field[3].position);
    switch(key) {
      case LEFTARROW :  case UPARROW  : /* ·¡¸å ¸aža¡ */
        if(info->prior) {
          info = info->prior;
          cur_row--;
          cur_rec_num--;
          if(cur_row < 0) {
            cur_row = 0;
            hbackscroll();
          }
        }
        break;
      case CTRL_HOME :
        while(cur_row) {
          info = info->prior;
          cur_row--, cur_rec_num--;
        }
        break;
      case RIGHTARROW :  case DOWNARROW :  case ' ' :  /* ”a·q ¸aža¡ */
        if(info->next) {
          info = info->next;
          cur_row++;
          cur_rec_num++;
          if(cur_row > MAXVIEWLINE-1) {
            cur_row = MAXVIEWLINE-1;
            hscroll();
          }
        }
        break;
      case CTRL_END :
        while(cur_row < MAXVIEWLINE-1) {
          if(info->next) {
            info = info->next;
            cur_row++, cur_rec_num++;
          }
          else {
            break;   /* while ¢…·i ÈiÂ‰ */
          }
        }
        break;
      case PGUP :   /* Ðe ÑÁ¡e eÇq  å¸á ¸aža¡ */
        while(cur_row) {
          info = info->prior;
          cur_row--, cur_rec_num--;
        }
        while(cur_row < MAXVIEWLINE-1) {
          if(info->prior) {
            info = info->prior;
            cur_row++, cur_rec_num--;
          }
          else
            break;
        }
        if(info->prior) {
          info = info->prior;
          cur_rec_num--;
        }
        cur_row = 0;
        display_name(info);
        break;
      case PGDN :                /* Ðe ÑÁ¡e eÇq ”a·q ¸aža¡ */
        while(cur_row < MAXVIEWLINE-1) {
          if(info->next) {
            info = info->next;
            cur_row++, cur_rec_num++;
          }
          else {
            break;   /* while ¢…·i ÈiÂ‰ */
          }
        }
        if(info->next) {
          cur_row = 0, cur_rec_num++;
          info = info->next;
          display_name(info);
        }
        break;
      case CTRL_PGUP :               /* ˆa¸w Àá·q ¸aža¡ */
        info = first;
        display_name(info);
        cur_row = 0;
        cur_rec_num = 0;
        break;
      case CTRL_PGDN  :               /* ˆa¸w aº— ¸aža¡ */
        info = pageup_data(last);
        display_name(info);
        info = last;
        if(num_recs > MAXVIEWLINE)
          cur_row = MAXVIEWLINE-1;
        else
          cur_row = num_recs-1;
        cur_rec_num = num_recs-1;
        break;
      case '\r' :                  /* ‹a ¸ažaŸi È‚Ðq --> ‹a ¸aža Íe»³¬wÈ¡ */
        cur_rec = info;
        hsetreverse(onoff);
        return;
      case DELKEY :             /* ¸aža »¡¶‹¡ */
        /* ·¡¦¦…µA ¤éA... ‹ai ·©‹¡ Ìa·©·i »¡¶‹¡•¡ Ð´¡»¡ */
        if(yesno("·¡ ¸ažaŸi »¡¶‰V¯s“¡Œa ?", 1) == YES) {
          temp_info = info;
          info = delete(info, &first, &last);
          num_recs--, recs_changed = 1;
          if(info) {
            temp_info = info;
            for(i = 0; i < cur_row; i++)
              info = info->prior;
            display_name(info);
            info = temp_info;
            cur_key = F0KEY;
          }
          else {  /* ¸aža  …  a»¡ b·© ˜ */
            info = temp_info->prior;
            if(!info || !num_recs)  {  /* Ðaa q·e ¸ažaŸi  a¸á »¡¶“e ‰w¶ */
              display_name(NULL);
              cur_key = F0KEY;
              break;
            }
            cur_row--, cur_rec_num--;
            if(cur_row < 0) {
              cur_row = 0;
              info = info->prior;
            }
            temp_info = info;
            for(i = 0; i < cur_row; i++)
              info = info->prior;
            display_name(info);
            info = temp_info;
          }
        }
        break;
      case ESCKEY :               /* Âá­¡ --> ÍiÁa¥¡‹¡Ÿi È‚Ða‹¡ ¸å· ¬wÈ¡ •©´aˆq */
        cur_rec = temp_rec;
        hsetreverse(onoff);
        return;
      default :
        break;
    } /* switch */
  } /* while */
}

/*
 *  Ðe ÑÁ¡e eÇq ·¡¸å ¸aža¡
 */
struct db_type *pageup_data(struct db_type *info)
{
  int i = MAXVIEWLINE+1;

  while(info && i) {
    info = info->prior;
    i--;
  }
  if(i > 0 || !info) info = first;

  return info;
}

/*
 *  ·¡Ÿq Îa¯¡ µwµbµA ·¡Ÿq·i Îa¯¡Ðe”a.
 */
void display_name(struct db_type *info)
{
  register i = 0;

  hclrscr();
  while(info && (i < MAXVIEWLINE)) {
    hprintfxy(1, 1+cur_row, " %s  %s ", info->data, info->data+field[3].position);
    info = info->next;
    i++;
  }
}

#ifndef DEBUG
/*
 *  ¸aža Àx‹¡
 */
void search_data(void)
{
  int field;
  char key[80], ch;
  struct db_type *info, *temp, *temp_rec;

  temp_rec = cur_rec;         /* Ñe¸ AÅ¡—a· Í¡·¥Èá */
  display_fields();
  field = select_field("* Àx·i ˜ ³i Ðw¡¢·i ‰¡Ÿa­A¶a");
  if(field == -1)  /* Âá­¡ */
    return;
  key[0] = '\0';
  if(mhgets2(fields[field].size-1, "Àx·i Ðw¡¢· ¶w·i ³a­A¶a", key, YES) == ESCKEY)
    return;
  ch = 'Y';
  info = find(key, first, field); /* Àx´aœa */
  while (info && ch == 'Y') {
    temp = info;
    display_rec(info);              /* ¸ažaŸi Àx´v·a¡e */
    info = find(key, info->next, field);  /* ˆ{·e ¸ažaˆa ”á ·¶“e»¡ Àx´a¥¥”a. */
    if (info) {                           /* ˆ{·e ¸ažaˆa ”á ·¶”a. */
      ch = disp_cmsg("ˆ{·e ¸ažaˆa ™¡ ·¶¯s“¡”a. ”á Àx´a ¥©Œa¶a (Y/N) ?");
      ch = toupper(ch);
      if(ch == 'Y')
        continue; /* ‰­¢ Àx´a¥¥”a. */
      else {
        /* AÅ¡—a ¤åÑ¡ ¸÷Ÿ¡ */
        cur_rec = info;
        break;
      } /* else */
    } /* if */
    else {            /* ˆ{·e ‰A ”á ´ô”a. ‹a e Àx“e”a. */
      info = temp;
      break;
    }
  }
  if(!info) { /* ¡µ Àx´v”a. */
    cur_rec_num = 0;
    disperror("‹aœå ¸aža“e ´ô“e•A¶a !!!");
    cur_rec = temp_rec;
  }
  else     /* Àx´v·i ˜ */
    cur_rec = info;
  prompt("");
  cur_task = MODIFY_ITEM;
}
#endif

/*
 *  ¸aža Àx‹¡
 */
struct db_type *find(char *key, struct db_type *from, int field)
{
  struct db_type *info;
  char  str[80] = "", tmp[80] = "", *pstr;

  info = from;
  strcpy(tmp, del_space(key));
  while (info) {
    cur_rec_num++;
    strcpy(str, info->data+fields[field].position);
    pstr = del_space(str);
    if(!strcmp((const char *)tmp, (const char *)pstr))
        return info;  /* Àx´v”a. */
    info = info->next;
  }
  return NULL;  /* ˆ{·e ˆõ·¡ ´ô”a. */
}

/*
 *  ¸aža ptr·i »¡¶‹¡
 */
struct db_type *delete(struct db_type *ptr, struct db_type **start, struct db_type **last)
{
  struct db_type *tmp;

  tmp = ptr;
  if(!ptr) return NULL;
  if(cur_rec == ptr)
    cur_rec = NULL;
  if (*start == ptr) {
    *start = ptr->next;
    if (*start) (*start)->prior = NULL;
    else *last = NULL;
  }
  else {
    ptr->prior->next = ptr->next;
    if (ptr != *last)
      ptr->next->prior = ptr->prior;
    else
      *last = ptr->prior;
  }
  free(ptr->data);
  free(ptr);
  return tmp->next;
}

#ifndef DEBUG
/*
 *  ¸aža »¡¶‹¡
 */
void delete_data(struct db_type **start, struct db_type **last)
{
  struct db_type *info, *tmp;
  char key[80];
  int field, flag = NO;

  display_fields();
  field = select_field("* »¡¶‰ ¸aža“e ´á“a Ðw¡¢·a¡ Àx·iŒa¶a ?");
  if(field == -1)
    return;
  key[0] = '\0';
  if(mhgets2(fields[field].size-1, "Àx·i Ðw¡¢· ¶w·i ³a­A¶a", key, YES) == ESCKEY)
    return;
  info = find(key, first, field);
  if(!info) {
    disperror("‹aœå ¸aža“e ´ô“e•A¶a !!");
    return;
  }
  while(info && !flag) {
    display_rec(info);
    switch(yesno("·¡ ¸ažaŸi »¡¶‰Œa¶a ?")) {
      case -1 : /* Âá­¡ */
        cur_key = F0KEY;
        return;
      case NO :
        tmp = info;
        tmp = find(key, tmp->next, field);
        if(tmp) {
          switch(yesno("‹aœñ, ·¡ˆå »¡¶‰œ¶a?")) {
            case -1 :  case  NO :
              cur_key = F0KEY;
              return;
            case YES :
              flag = YES;
              break;
          }
        }
        else
          return;
        break;
      case YES :
        flag = YES;
        break;
    }
  }
  if (cur_rec == info) cur_rec = NULL;
  num_recs--;  recs_changed = 1;

  if (*start == info) {
    *start = info->next;
    if (*start) (*start)->prior = NULL;
    else *last = NULL;
  }
  else {
    info->prior->next = info->next;
    if (info != *last)
      info->next->prior = info->prior;
    else
      *last = info->prior;
  }
  cur_rec = info->next;
  free(info->data);
  free(info);
  if(cur_rec) {
    display_rec(cur_rec);
    display_rec_state(DISP_ALL);
    display_name(cur_rec);
    cur_task = MODIFY_ITEM;
  }
  else
    cur_key = F0KEY;
}

static void print_headline(void)
{
/*                          1         2         3         4         5         6         7         8         9         1         2  */
/*                 1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012*/
  lprintf("\n\r\n\r");  /* – º‰ ¸÷•¡ ›‰¡ ¯¡¸b */
  lprintf("----------------------------------------------------------------------------------------------------------\n\r");
  lprintf("·¡    Ÿq     ¬÷ ¬—‘e¶©·©    ‰iÑ¥          º           ­¡                          »¡µb  ¸åÑÁ¤åÑ¡ ¶Íe¤åÑ¡\n\r");
  lprintf("----------------------------------------------------------------------------------------------------------\n\r");
}

/*  Â‰b ¦¦… */
void print_data(int whatdata)
{
  int field, selective, ch, line_cnt;
  struct db_type *info;
  char key[80];

  if(disp_cmsg(" å¸á, ÏaŸ¥Èá“e Åva ÑÂ·¥Ða­A¶a.") == ESCKEY)
    return;
  ch = yesno("¹¡ˆå¦¡ Â‰bÐa¯¡‰V¯s“¡Œa ?");
  if(ch == YES) {
    display_fields();
    field = select_field("* ´á˜å Ðw¡¢·a¡ ¹¡ˆå·i ”iœ¶a?");
    if(field == -1)
      return;
    key[0] = '\0';
    if(mhgets2(fields[field].size-1, "Àx·i Ðw¡¢· ¶w·i ³a­A¶a", key, YES) == ESCKEY)
      return;
    selective = 1;
  }
  else if(ch == NO) {
    selective = 0;
  }
  else
    return;  /* cancel */

  prompt("* ·¥­á º—.....");
  if (selective) {
    info = find(key, first, field);
    if(!info) {
      disperror("¸ažaˆa Ðaa•¡ ´ô¯s“¡”a.");
      return;
    }
  }
  else {
    info = first;
    if(!info) {
      disperror("‹aœå ¸aža“e Ðaa•¡ ´ô“e•A¶a");
      return;
    }
  }
  if(!isOKprn()) {
    disperror("·¥­áÐi ®ˆa ´ô´á¶a. ÏaŸ¥ÈáŸi ¸ñˆñÐ ¥¡­A¶a");
    return;
  }
  print_headline();
  line_cnt = 0;
  while(info) {
    if(line_cnt == MAXPAPERLINE) {
      line_cnt = 0;
      lprintf("%c", '\f');   /* form feed */
      print_headline();
    }
    line_cnt++;
    display_rec(info);
    lprintf("%-12s %s %-8s(%c) %s %-49s %-5s %-8s %7s\n\r",
      info->data+fields[0].position, strcmp(info->data+fields[1].position, "1") ? "µa" : "q",
      info->data+fields[2].position, strcmp(info->data+fields[3].position, "1") ? '+' : '-',
      strcmp(info->data+fields[4].position, "1") ? "‹¡Ñ¥" : "£¡Ñ¥",
      info->data+fields[5+whatdata*4].position,
      info->data+fields[6+whatdata*4].position,
      info->data+fields[7+whatdata*4].position,
      info->data+fields[8+whatdata*4].position);
    if(selective)
      info = find(key, info->next, field);
    else
      info = info->next;
  }
  lprintf("%c", '\f'); /* form feed */
}
#endif

/*
 *  ¸aža Ìa·© ¸á¸wÐa‹¡
 */
int save_data(void)
{
  struct db_type *info;
  char file_header[HDR_LEN];
  FILE *fp;

  fp = fopen(datafile, "wb");            /* ¸aža Ìa·©·i µe”a */
  if(!fp) {
    disperror("¸aža Ìa·©·i ¸á¸wÐi ® ´ô¯s“¡”a.");
    return NO;
  }
  prompt("* ¸aža ¸á¸w º— ......");

  sprintf(file_header, "%s%c", datahead, 0x1A);
  fwrite(file_header, HDR_LEN, 1, fp);  /* ¸ažaÌa·©·  áŸ¡¦¦… ³a‹¡ */
  if(ferror(fp)) {
    disperror("¸aža Ìa·© áŸ¡Ÿi ³i ® ´ô¯s“¡”a.");
    fclose(fp);
    prompt("");
    return NO;
  }
  info = first;
  while (info) {                   /* ¸ažaŸi Ìa·©µA °áý‹¡ */
    fwrite(info->data, record_size, 1, fp);
    info = info->next;
    if(ferror(fp)) {
      disperror("¸aža Ìa·©·i ¸á¸wÐi ® ´ô¯s“¡”a.");
      fclose(fp);
      prompt("");
      return NO;
    }
  }
  fclose(fp);
  recs_changed = OFF;
  return YES;
}

/*
 *  ¸aža Ìa·©µA ³a·¡“e ¥e®—i Á¡‹¡ÑÁ
 */
void reset_data(void)
{
  first = last = cur_rec = NULL;
  num_recs = cur_rec_num = 0;
  recs_changed = 0;
}

/*
 *  ¸aža Ìa·© ·ª´áµ¡‹¡
 */
int load_data(void)
{
  struct db_type *info;
  char file_header[HDR_LEN];
  char wait_msg[] = "¸aža Ìa·© ·ª“eº—·¡µ¡“¡ ¸qŒe e ‹¡”aŸ¡¯¡´á¶a.";
  char clr_msg[]  = "                                           ";
  FILE *fp;

  fp = fopen(datafile, "rb");          /* ¸aža Ìa·©·i µe”a */
  if(!fp) {                            /* Ñe¸ —¡BÉ¡Ÿ¡µA Ìa·©·¡ ´ô·a¡e */
    if(yesno("¸aža Ìa·©·¡ ´ô´á¶a. ¬¡  e—iŒa¶a ?") == YES) {
      reset_data();                    /* ¥e®—i Á¡‹¡ÑÁ Ða‰¡ */
      fp = fopen(datafile, "wb");            /* ¸aža Ìa·©·i µe”a */
      if(!fp) {
        disperror("¸aža Ìa·©·i  e—i ® ´ô¯s“¡”a.");
        return NO;
      }
      sprintf(file_header, "%s%c", datahead, 0x1A);
      fwrite(file_header, HDR_LEN, 1, fp);  /* ¸ažaÌa·©·  áŸ¡¦¦… ³a‹¡ */
      if(ferror(fp)) {
        disperror("¸aža Ìa·© áŸ¡Ÿi ³i ® ´ô¯s“¡”a.");
        fclose(fp);
        prompt("");
        return NO;
      }
      fclose(fp);
      return YES;
    }
    else
      return NO;
  }
  hputsxy((getmaxix()-strlen(wait_msg))/2, getmaxiy()/2-1, wait_msg);
  fread(file_header, HDR_LEN, 1, fp);    /* ¸ažaÌa·©·  áŸ¡¦¦…·ª‹¡ */
  if(!ferror(fp) && strncmp(file_header, datahead, strlen(datahead))) {
    disperror("¸aža Ìa·©· Ñw¯¢·¡ ËiŸ³“¡”a.");
    fclose(fp);
    hputsxy((getmaxix()-strlen(wait_msg))/2, getmaxiy()/2-1, clr_msg);
    return NO;
  }
  first = last = cur_rec = NULL;
  num_recs = 0;

  while(!feof(fp)) {                /* Ìa·© {·© ˜ Œa»¡ */
    info = (struct db_type *)calloc(1, sizeof(struct db_type));
    if(!info) {
      memerror();
      fclose(fp);
      hputsxy((getmaxix()-strlen(wait_msg))/2, getmaxiy()/2-1, clr_msg);
      return NO;
    }
    info->data = (unsigned char *)calloc(1, record_size);
    if(!info->data) {
      memerror();
      fclose(fp);
      hputsxy((getmaxix()-strlen(wait_msg))/2, getmaxiy()/2-1, clr_msg);
      return NO;
    }
    if(fread(info->data, record_size, 1, fp) != 1) {
      if(ferror(fp)) {
        disperror("¸aža Ìa·©·i ·ª·i ® ´ô¯s“¡”a.");
        fclose(fp);
        hputsxy((getmaxix()-strlen(wait_msg))/2, getmaxiy()/2-1, clr_msg);
        return NO;
      }
      fclose(fp);
      cur_rec = first;
      recs_changed = 0;
      cur_rec_num = 0;
      hputsxy((getmaxix()-strlen(wait_msg))/2, getmaxiy()/2-1, clr_msg);
      return YES;
    }
    dls_store(info, &first, &last);  /* ·¡»¥ a¢Š¹¡µA ¸aža Ÿ¡¯aËaŸi ÀñˆaÐe”a. */
    num_recs++;
  }
  fclose(fp);
  cur_rec = first;
  prompt("");
  hputsxy((getmaxix()-strlen(wait_msg))/2, getmaxiy()/2-1, clr_msg);
  return NO;
}

void get_date_title(struct date_title_t *info)
{
  char *picture[] = {
    "##‘e ##¶© ##·©", "HH", "XXXXXXXXXXXX",
    "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
  };
  char *buffer[] = {
    "              ", "  ", "            ",
    "                                  "
  };
  int len[] = { 0, 15, 17, 29, 63 };
  int x[]   = { 3, 19, 27, 42 };
  int cnt, isquit;

  hprintfxy(19+2, 3, "¶a·©");
  cnt = isquit = 0;
/*  for(cnt = 0; cnt < 4; cnt++)
    strcpy(buffer[cnt], (char *)info+len[cnt]);*/
  do {
    hgotoxy(x[cnt], 3);
    hgetdata(buffer[cnt], picture[cnt]);
    switch(_lastkey) {
      case '\t' :
        if(cnt < 4)
          cnt++;
        else
          cnt = 0;
        break;
      case ESC :
        isquit = 1;
        break;
      default :
        if(cnt == 3)
          isquit = 1;
        else
          cnt++;
    }
  } while(!isquit);
/*  for(cnt = 0; cnt < 4; cnt++)
    strcpy((char *)info+len[cnt], buffer[cnt]);*/
}

/*
 *  Ìa·© nindex.c · {
 */
