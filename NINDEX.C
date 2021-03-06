#define DEBUG

/*
 *  �a��   : nindex.c
 *  ���w   : �i�ⷡ(����)�� ���� �a��
 *  ���B   : nal.prj
 *  �i�a   : 93.03.16
 *  �e�e�� : �e�����b�a ��aɷ�����b�� �e�i�a�� �����b
 *  �i���i : �� Ή�a���� ���a���e �����i ��â�a�� �s���a.
 *           �ᐡ�a�e, �a�� �a�e ���a���� �����i �g�� �h�v�a���a�a.
 *           �a�a��, �� ���a���i �h�a�� ���e �a�e�A �a�w�a�a�� �A���e
 *           ���� �e�e ���a���� �i �����a�a�� ����Ё ������ �a�s���a.
 */

#include <stdio.h>
#include <string.h>
#include <dos.h>
#include "hanin.h"
#include "ascii.h"
#include "nindex.h"

#define HDR_LEN        (32)                  /* �a�a �a���� �១ ���� */
#define MAX_FIELDS     (4)                   /* �A�� ϩ�a �� */
#define MAXVIEWLINE    (15)
#define MAXVIEWWIDTH   (MAXTITLELEN+MAXDATELEN+4)

struct descriptor {
  int position;                /* �Aš�a ���� ϩ�a ��á */
  int size;                    /* ϩ�a �a�� - �a���a */
  int x, y;                    /* ϩ�a ��á */
} fields[MAX_FIELDS];          /* ϩ�a �����A */

int num_fields;                /* ϩ�a ���� */
int record_size;               /* �e �Aš�a�� �a�� (�b ϩ�a�� ����+1)�� �s */
int sort_field;                /* ���i�I ϩ�a ��ѡ */
int num_recs;                  /* �a���A�� �Aš�a ���� */

int recs_changed;              /* �Aš�a�a �����A���a ? */
int cur_rec_num;               /* �e�� �Aš�a ��ѡ */

char *indexfile = "NAL.IDX";   /* �i�ⷡ ���� �a���� ���q */
char *indexhead = "Nalgergy Index File Ver 1.0"; /* �a�a �a���� �១ */

struct db_type {
  unsigned char *data;            /* �a�a�A ���e ͡���� */
  struct db_type *prior;          /* ���� ���a�a�� ���� */
  struct db_type *next;           /* �a�q ���a�a�� ���� */
} *first, *last, *cur_rec;

char *picture[] = {
  "##�e ##�� ##��", "HH", "XXXXXXXXXXXX", "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
};

char *buffer[] = {
  "              ", "  ", "            ", "                                  "
};

/* ************************************************************************ *
 *                        �a�a �a���� ����                                  *
 * ************************************************************************ */

/*
 *  ��q �a�a��
 */
static struct db_type *first_data(struct db_type *p)
{
  p = first, cur_rec_num = 0;
  return p;
}

/*
 *  �a���b �a�a��
 */
static struct db_type *last_data(struct db_type *p)
{
  p = last, cur_rec_num = num_recs-1;
  return p;
}

/*
 *  �a�q �a�a��
 */
static struct db_type *next_data(struct db_type *p)
{
  p = p->next, cur_rec_num++;
  return p;
}

/*
 *   ���� �a�a��
 */
static struct db_type *prior_data(struct db_type *p)
{
  p = p->prior, cur_rec_num--;
  return p;
}

/*
 *  �i�ⷡ�A �i�a�� �A���A ���e �����a��
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
  sort_field = 0;   /* �i�a�a ���i�� ���� */
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
 *  ���w        : ���i ������ �i�a�� �A�� �a��
 *  ���a���e �t : �e���i �a�����q; �� > 1993�e 5�� 5�� --> "930505"
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
    sprintf(key, "%2d�e %2d�� %2d��", today.da_year % 100, today.da_mon, today.da_day);
    if(find(key, first, 0)) {
      /* ���i�i�a�� �����a ���� ���a */
      if(yesno("���i �i�a�� �����a ���� �����a. �e���a���V�s���a ?", 0) == 0) {
        /* ���i �����i �e���a�V�a. */
        /* �i�a, �a��, �i��, �A���w�i �a���a�� �e�������a�� �e�a.*/
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
       *  ���b�e �i�a�A �����a ���� ���e�� ��aЁ��
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
/* ���� �a�a�� �e�� */
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
        display_rec(ptr);        /* �e���i �a�a�i ���e�A �ẅ�a. */
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
      if(findshortcut(exit_key) || (exit_key == F0KEY)) {           /* �eǡ�a�e */
        save_flag = YES;
        isquit = YES, menu_flag = NO;
        cur_key = exit_key;
        break;
      }
      switch(exit_key) {
        case UPARROW :       /* ���� �w���a�� */
          cnt--;
          break;
        case DOWNARROW : case '\r':  /* �a�q �w���a�� */
          cnt++;
          break;
        case SWITCH_KEY :
          if(num_recs != 0) {
            save_flag = isquit = YES, menu_flag = NO;
            cur_task = VIEW_ITEM;
          }
          else
            disperror("�a�a�a �a�a�� ���e�A �� �����a���a?");
          break;
        case F2KEY :
          break;
        case CTRLPGUPKEY :  /* �a�w ��q �a�a�� */
          cur_rec = first_data(cur_rec);
          memcpy(ptr->data, cur_rec->data, record_size);
          flag = YES;
          break;
        case CTRLPGDNKEY :  /* �a�w �a���b �a�a�� */
          cur_rec = last_data(cur_rec);
          memcpy(ptr->data, cur_rec->data, record_size);
          flag = YES;
          break;
        case PGDNKEY :      /* �a�q �a�a�� */
          if(cur_rec->next) {
            cur_rec = next_data(cur_rec);
            memcpy(ptr->data, cur_rec->data, record_size);
            flag = YES;
          }
          break;
        case PGUPKEY :      /* ���� �a�a�� */
          if(cur_rec->prior) {
            cur_rec = prior_data(cur_rec);
            memcpy(ptr->data, cur_rec->data, record_size);
            flag = YES;
          }
          break;
      }
      if(exit_key == ESCKEY) {
        isquit = YES;
        break;           /* while �c�a�� �i */
      } /* �a�{ ���� if */
    } /* while */
    if((exit_key == F2KEY) || (save_flag)) {
      if(ptr->data[0]) {
        recs_changed = 1;
        delete(cur_rec, &first, &last);
        cur_rec = ptr;
        dls_store(ptr, &first, &last);   /* �� �a�a ��a */
        display_name(ptr);               /* �i�a���� �����i �a�� �ẅ�a. */
        cnt = 0;
        tmp = first;
        while(tmp != cur_rec) {
          tmp = tmp->next;
          cnt++;
        }
        cur_rec_num = cnt;
        if(!save_flag) {   /* �eǡ�a �������g�� �a���a�� �֭����a. */
          if(yesno("�e���i �aá���V�s���a ?") == YES)
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
 *  �b �w���� ���q�i �a�� �b�b�� ���b���i �����a.
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
 *  ���b �w�b�i �a�a���� �b �w���� �w�b�A �a�a�i �a��Ё���a.
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
 *  key�A�� �����i �A���e�a.
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
 *                       ���A �a�a�i �១�a�e ����                          *
 * ************************************************************************ */

/*
 *  �a�a�i 2�� �a���e�A ���� ���e�a.
 */
void dls_store(struct db_type *i, struct db_type **first, struct db_type **last)
{
  struct db_type *old, *p;

  if (*last == NULL) {    /* ���a�a�A ��q ��a �A�e �w�� */
    i->next =  NULL;
    i->prior = NULL;
    *last = i ;
    *first = i;
    return;
  }

  p = *first;             /* ���a�a �������A�� ���b */
  old = NULL;
  while(p) {              /* �a�a�i ���� ���i ��á�i �x�e�a. */
    if(strcmp(p->data + fields[sort_field].position,
              i->data + fields[sort_field].position) < 0 ) {
      old = p;
      p = p->next;
    }
    else {
      if (p->prior) {      /* ���a�a ���e�A�� ���� */
        p->prior->next = i;
        i->next = p;
        i->prior = p->prior;
        p->prior = i;
        return;
      }
      i->next = p;          /* ��a */
      i->prior = NULL;
      p->prior = i;
      *first = i;
      return;
    }
  } /* while */
  old->next = i;              /* �{�A ���e�a */
  i->next = NULL;
  i->prior = old;
  *last = i;
}

/*
 *  �a�a �i�a����
 *
 *  �i�a�� �A���e �a��
 *  15���e �a���a�a
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
    disperror("�a�a�a �a�a�� ���e�A �� �����a��?");
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
      case LEFTARROW :  case UPARROW  : /* ���� �a�a�� */
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
      case RIGHTARROW :  case DOWNARROW :  case ' ' :  /* �a�q �a�a�� */
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
            break;   /* while ���i �i */
          }
        }
        break;
      case PGUP :   /* �e ���e�e�q ��� �a�a�� */
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
      case PGDN :                /* �e ���e�e�q �a�q �a�a�� */
        while(cur_row < MAXVIEWLINE-1) {
          if(info->next) {
            info = info->next;
            cur_row++, cur_rec_num++;
          }
          else {
            break;   /* while ���i �i */
          }
        }
        if(info->next) {
          cur_row = 0, cur_rec_num++;
          info = info->next;
          display_name(info);
        }
        break;
      case CTRL_PGUP :               /* �a�w ��q �a�a�� */
        info = first;
        display_name(info);
        cur_row = 0;
        cur_rec_num = 0;
        break;
      case CTRL_PGDN  :               /* �a�w �a�� �a�a�� */
        info = pageup_data(last);
        display_name(info);
        info = last;
        if(num_recs > MAXVIEWLINE)
          cur_row = MAXVIEWLINE-1;
        else
          cur_row = num_recs-1;
        cur_rec_num = num_recs-1;
        break;
      case '\r' :                  /* �a �a�a�i Ȃ�q --> �a �a�a �e���wȁ�� */
        cur_rec = info;
        hsetreverse(onoff);
        return;
      case DELKEY :             /* �a�a ������ */
        /* �������A ��A... �a�i ���� �a���i �������� Ё���� */
        if(yesno("�� �a�a�i �����V�s���a ?", 1) == YES) {
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
          else {  /* �a�a �� �a���b�� �� */
            info = temp_info->prior;
            if(!info || !num_recs)  {  /* �a�a �q�e �a�a�i �a�� �����e �w�� */
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
      case ESCKEY :               /* �᭡ --> �i�a�����i Ȃ�a�� �巁 �wȁ�� ���a�q */
        cur_rec = temp_rec;
        hsetreverse(onoff);
        return;
      default :
        break;
    } /* switch */
  } /* while */
}

/*
 *  �e ���e�e�q ���� �a�a��
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
 *  ���q �a�� �w�b�A ���q�i �a���e�a.
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
 *  �a�a �x��
 */
void search_data(void)
{
  int field;
  char key[80], ch;
  struct db_type *info, *temp, *temp_rec;

  temp_rec = cur_rec;         /* �e�� �Aš�a�� ͡���� */
  display_fields();
  field = select_field("* �x�i �� �i �w���i ���a�A�a");
  if(field == -1)  /* �᭡ */
    return;
  key[0] = '\0';
  if(mhgets2(fields[field].size-1, "�x�i �w���� ���w�i �a�A�a", key, YES) == ESCKEY)
    return;
  ch = 'Y';
  info = find(key, first, field); /* �x�a�a */
  while (info && ch == 'Y') {
    temp = info;
    display_rec(info);              /* �a�a�i �x�v�a�e */
    info = find(key, info->next, field);  /* �{�e �a�a�a �� ���e�� �x�a���a. */
    if (info) {                           /* �{�e �a�a�a �� ���a. */
      ch = disp_cmsg("�{�e �a�a�a �� ���s���a. �� �x�a ���a�a (Y/N) ?");
      ch = toupper(ch);
      if(ch == 'Y')
        continue; /* ���� �x�a���a. */
      else {
        /* �Aš�a ��ѡ ���� */
        cur_rec = info;
        break;
      } /* else */
    } /* if */
    else {            /* �{�e �A �� ���a. �a�e �x�e�a. */
      info = temp;
      break;
    }
  }
  if(!info) { /* �� �x�v�a. */
    cur_rec_num = 0;
    disperror("�a�� �a�a�e ���e�A�a !!!");
    cur_rec = temp_rec;
  }
  else     /* �x�v�i �� */
    cur_rec = info;
  prompt("");
  cur_task = MODIFY_ITEM;
}
#endif

/*
 *  �a�a �x��
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
        return info;  /* �x�v�a. */
    info = info->next;
  }
  return NULL;  /* �{�e ���� ���a. */
}

/*
 *  �a�a ptr�i ������
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
 *  �a�a ������
 */
void delete_data(struct db_type **start, struct db_type **last)
{
  struct db_type *info, *tmp;
  char key[80];
  int field, flag = NO;

  display_fields();
  field = select_field("* ���� �a�a�e ��a �w���a�� �x�i�a�a ?");
  if(field == -1)
    return;
  key[0] = '\0';
  if(mhgets2(fields[field].size-1, "�x�i �w���� ���w�i �a�A�a", key, YES) == ESCKEY)
    return;
  info = find(key, first, field);
  if(!info) {
    disperror("�a�� �a�a�e ���e�A�a !!");
    return;
  }
  while(info && !flag) {
    display_rec(info);
    switch(yesno("�� �a�a�i �����a�a ?")) {
      case -1 : /* �᭡ */
        cur_key = F0KEY;
        return;
      case NO :
        tmp = info;
        tmp = find(key, tmp->next, field);
        if(tmp) {
          switch(yesno("�a��, ���� �������a?")) {
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
  lprintf("\n\r\n\r");  /* �� �� ���� ���� ���b */
  lprintf("----------------------------------------------------------------------------------------------------------\n\r");
  lprintf("��    �q     �� ���e����    �iѥ          ��           ��                          ���b  ������ѡ ���e��ѡ\n\r");
  lprintf("----------------------------------------------------------------------------------------------------------\n\r");
}

/*  �b ���� */
void print_data(int whatdata)
{
  int field, selective, ch, line_cnt;
  struct db_type *info;
  char key[80];

  if(disp_cmsg("���, �a����e �v�a �·��a�A�a.") == ESCKEY)
    return;
  ch = yesno("���妁�� �b�a���V�s���a ?");
  if(ch == YES) {
    display_fields();
    field = select_field("* ��� �w���a�� ����i �i���a?");
    if(field == -1)
      return;
    key[0] = '\0';
    if(mhgets2(fields[field].size-1, "�x�i �w���� ���w�i �a�A�a", key, YES) == ESCKEY)
      return;
    selective = 1;
  }
  else if(ch == NO) {
    selective = 0;
  }
  else
    return;  /* cancel */

  prompt("* ���� ��.....");
  if (selective) {
    info = find(key, first, field);
    if(!info) {
      disperror("�a�a�a �a�a�� ���s���a.");
      return;
    }
  }
  else {
    info = first;
    if(!info) {
      disperror("�a�� �a�a�e �a�a�� ���e�A�a");
      return;
    }
  }
  if(!isOKprn()) {
    disperror("�����i ���a ����a. �a����i ���Ё ���A�a");
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
      info->data+fields[0].position, strcmp(info->data+fields[1].position, "1") ? "�a" : "�q",
      info->data+fields[2].position, strcmp(info->data+fields[3].position, "1") ? '+' : '-',
      strcmp(info->data+fields[4].position, "1") ? "��ѥ" : "��ѥ",
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
 *  �a�a �a�� ��w�a��
 */
int save_data(void)
{
  struct db_type *info;
  char file_header[HDR_LEN];
  FILE *fp;

  fp = fopen(datafile, "wb");            /* �a�a �a���i �e�a */
  if(!fp) {
    disperror("�a�a �a���i ��w�i �� ���s���a.");
    return NO;
  }
  prompt("* �a�a ��w �� ......");

  sprintf(file_header, "%s%c", datahead, 0x1A);
  fwrite(file_header, HDR_LEN, 1, fp);  /* �a�a�a���� �១���� �a�� */
  if(ferror(fp)) {
    disperror("�a�a �a���១�i �i �� ���s���a.");
    fclose(fp);
    prompt("");
    return NO;
  }
  info = first;
  while (info) {                   /* �a�a�i �a���A ����� */
    fwrite(info->data, record_size, 1, fp);
    info = info->next;
    if(ferror(fp)) {
      disperror("�a�a �a���i ��w�i �� ���s���a.");
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
 *  �a�a �a���A �a���e �e���i ������
 */
void reset_data(void)
{
  first = last = cur_rec = NULL;
  num_recs = cur_rec_num = 0;
  recs_changed = 0;
}

/*
 *  �a�a �a�� ���ᵡ��
 */
int load_data(void)
{
  struct db_type *info;
  char file_header[HDR_LEN];
  char wait_msg[] = "�a�a �a�� ���e�������� �q�e�e ���a������a.";
  char clr_msg[]  = "                                           ";
  FILE *fp;

  fp = fopen(datafile, "rb");          /* �a�a �a���i �e�a */
  if(!fp) {                            /* �e�� ���Bɡ���A �a���� ���a�e */
    if(yesno("�a�a �a���� ����a. ���� �e�i�a�a ?") == YES) {
      reset_data();                    /* �e���i ������ �a�� */
      fp = fopen(datafile, "wb");            /* �a�a �a���i �e�a */
      if(!fp) {
        disperror("�a�a �a���i �e�i �� ���s���a.");
        return NO;
      }
      sprintf(file_header, "%s%c", datahead, 0x1A);
      fwrite(file_header, HDR_LEN, 1, fp);  /* �a�a�a���� �១���� �a�� */
      if(ferror(fp)) {
        disperror("�a�a �a���១�i �i �� ���s���a.");
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
  fread(file_header, HDR_LEN, 1, fp);    /* �a�a�a���� �១�������� */
  if(!ferror(fp) && strncmp(file_header, datahead, strlen(datahead))) {
    disperror("�a�a �a���� �w���� �i�����a.");
    fclose(fp);
    hputsxy((getmaxix()-strlen(wait_msg))/2, getmaxiy()/2-1, clr_msg);
    return NO;
  }
  first = last = cur_rec = NULL;
  num_recs = 0;

  while(!feof(fp)) {                /* �a�� �{�� �� �a�� */
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
        disperror("�a�a �a���i ���i �� ���s���a.");
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
    dls_store(info, &first, &last);  /* ���� �a�������A �a�a ���a�a�i ��a�e�a. */
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
    "##�e ##�� ##��", "HH", "XXXXXXXXXXXX",
    "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
  };
  char *buffer[] = {
    "              ", "  ", "            ",
    "                                  "
  };
  int len[] = { 0, 15, 17, 29, 63 };
  int x[]   = { 3, 19, 27, 42 };
  int cnt, isquit;

  hprintfxy(19+2, 3, "�a��");
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
 *  �a�� nindex.c �� �{
 */
