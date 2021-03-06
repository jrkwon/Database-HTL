/*
 *  �����w ���� �ş� ����
 */

/*
 *  �����w�� ���A ����  Ver 1.0
 *
 *  ���e �aЁ�� ���q�a�� ���Bɡ���i �e�e�a.
 *  ���i �i�� 93�e�� �w���A�e 93���a�e �����Bɡ���i �e�i��
 *  �a �a���A 93-INDEX.IDX�� �{�e ���a�� ���� �a���i �b ���Bɡ��
 *  �i�� �e�i�� �ş��e�a.
 */

#include <stdio.h>
#include <stdlib.h>
#include <dos.h>

#define  N_ITEM   5             /* ���b�i �a�a�� ϩ�a �� */

/*
 *  ���e�� ���� ����
 *  +---------------------------------------......--------+
 *  |   �a���a                                            |
 *  +---------------------------------------......--------+
 *  |   1993�e 05�� 22�� ɡ�a��  ���� �A��                |
 *  |                                                     |
 *
 *
         1         2         3         4         5         6         7
12345678901234567890123456789012345678901234567890123456789012345678901234567890

    1993�e 05�� 22�� ɡ�a��  �a���ᦁ�� �a�i�� �����A ���e �e�e�e �A��....
    (5)    (12) (17) (22)    (30)
 *
 */

/*
 *  �a�a �a���� ����
 *
 *  930523JR.NAL�� 93�e 5�� 23���� ���� �a������
 *  93052300.PCX ~ 93052399.PCX�� ���i�� �a���󷡔a.
 */

/*
 * �i�a�� ���� �A��
 */
struct date_title_t {
  byte year[5];
  byte month[3];
  byte day[3];
  byte yoil[3];
  byte title[56];
};

/*
 *  ���� �a���� ����
 *
 *  93-INDEX.IDX   <-- 93�e�� ���� �����A ���e �����a��
 */

/*
 *  ���� �ş��i ���e ���� �e��
 */
struct index_t {
  struct date_title_t date_title;
  struct index_t *prior;
  struct index_t *next;
} *first, *last, *current;

/*
 *  ��� �i�� �����A ���e ������ ���e�� ���a
 *
 *  93�e 5�� 23���� �w�� date�e "930523JR"�� ���a�e �a�i��
 *  �A���i title�A ���ẅ�a.
 */
bool find_date_title(byte *date, byte *title)
{

}

/*
 *
 */
bool hookhgetdat(int *keycode)
{
  if(keycode == NULL)
    return false;
  switch(*keycode) {
    case '\t' :
      *keycode == CR;
      break;
  }
}

/*
 *  �� �i�a�� �A�� ���b
 *
 *  ���a���e �t�� �q���e �a�i�� �����i �a�� �a�� ���b�a�e �A��
 *  �e�� �ỵ�t���e ���� �a���i ͡���a�e �w���a �E�a.
 *  �a���� �e�a�� �� �����i ���e �� �q���i ���e ���A��e �� �a�i�� �����a
 *  ���A�� ���a�a �w�A ���� ���e���i ���a�a�e ���� ���a�a�a.
 */
bool new_date_title(void);
{
  struct date_title_t buf, pic, ;
  struct date today;
  byte *buf_ptr[] = { buf.year, buf.month, buf.day, buf.yoil, buf.title };
  byte *pic[] = {
    "####", "##", "##", "HH",
    "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
  };
  byte *yoil[7] = { "��", "��", "��", "��", "��", "�q", "ɡ" };
  byte buffer[80], title[80];
  int x_crd[] = { 5, 12, 17, 22, 30 };
  int cnt, cuttail, isquit;
  bool onoff, ans;

  cnt = isquit = 0, cuttail = _cuttail;

  /*
   * ���i �i�a�i ����� ��w
   */
  getdate(&today);
  sprintf(buf.year, "%04d", today.da_year);
  sprintf(buf.month, "%02d", today.da_mon);
  sprintf(buf.day, "%02d", today.da_day);
  strcpy(buf.yoil, yoil[get_yoil(today.da_year, today.da_mon, today.da_day)]);

  /*
   * ���i �i�a�� �b���E ������ ������ ���e�� ���aЁ�� ���a�e
   * �a�i�� �A���i ���ᵥ�a.
   */
  sprintf(buffer, "%02d%02d%02dJR", today.da_year % 100, today.da_mon, today.da_day);
  if(find_date_title(buffer, title))
    sprintf(buf.title, "%55s", title);
  else
    sprintf(buf.title, "%55s", " ");

  /*
   * ���b�h�����e ���e �i�i ���� - �e�a 2.23�� hgetdata()�q�� ���w
   */
  strcpy(pic.year, pic[0]);
  strcpy(pic.month, pic[1]);
  strcpy(pic.day, pic[2]);
  strcpy(pic.yoil, pic[3]);
  strcpy(pic.title, pic[4]);

  /*
   * ���i �i�a�i �A���a�e�� �i�a�i ���b �h�e�a.
   */
  onoff = isreverse();
  ans = false;
  _hookhgetdata = hookhgetdata();
  do {
    cnt = (cnt + N_ITEM) % N_ITEM;
    hgotoxy(x_crd[cnt], 2);
    hsetreverse(ON);
    _cuttail = false;
    hgetdata(buf_ptr[cnt], pic[cnt]);
    switch(_lastkey) {
      case ESC :
        isquit = 1;
        ans = false;
        break;
      case '\r' :
        if(cnt == (N_ITEM-1)) {
          isquit = 1;
          ans = true;
        }
        else
          cnt++;
        break;
      case LEFTARROW : case SHIFTTAB :
        cnt--;
        break;
      case RIGHTARROW : case TAB :
        cnt++;
        break;
    } /* switch */
  } while( !isquit);

  _cuttail = cuttail;
  hsetreverse(onoff);
  return ans;
}

/* ************************************************************************ *
 *                    ���A�� �a�a�i �១�a�e ����                           *
 * ************************************************************************ */

void dl_store(struct index_t *new, struct index_t **a_1st, struct index_t **a_last)
{

/* �a�a�i 2�� �a���e�A ���� ���e�a. */
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

/* �a�a �x�� */
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

/* �a�a �x�� */
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

/* �a�a ptr�i ������ */
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

/* �a�a ������ */
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

/* �a�a �a�� ��w�a�� */
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

/* �a�a �a���A �a���e �e���i ������ */
void reset_data(void)
{
  first = last = cur_rec = NULL;
/*  strncpy(cur_rec->data, "", record_size); */
  num_recs = cur_rec_num = 0;
  recs_changed = 0;
}

/* �a�a �a�� ���ᵡ�� */
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
/*      cur_rec = first = last = NULL;
      cur_rec->data[0] = NULL;*/
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

/*
 *  �a�� n_index.c �� �{
 */
