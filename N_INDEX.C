/*
 *  ·©‹¡¸w ¬‚·¥ ‰ÅŸ¡ ¦¦…
 */

/*
 *  ·©‹¡¸w· ¸åÁA Š¹¡  Ver 1.0
 *
 *   ‘e ‹aÐ· ·¡Ÿq·a¡ —¡BÉ¡Ÿ¡Ÿi  e—e”a.
 *  µŸi —i´á 93‘e· ‰w¶µA“e 93·¡œa“e ¦—¡BÉ¡Ÿ¡Ÿi  e—i‰¡
 *  ‹a ´aœµA 93-INDEX.IDXµÁ ˆ{·e ¯¢·a¡ ¬‚·¥ Ìa·©·i ˆb —¡BÉ¡Ÿ¡
 *  ¥i¡  e—i´á ‰ÅŸ¡Ðe”a.
 */

#include <stdio.h>
#include <stdlib.h>
#include <dos.h>

#define  N_ITEM   5             /* ·³bÐi ¸aža· Ï©—a ® */

/*
 *  ÑÁ¡e· ‹¡¥¥ Š¹¡
 *  +---------------------------------------......--------+
 *  |   ÀaŸ±Îa                                            |
 *  +---------------------------------------......--------+
 *  |   1993‘e 05¶© 22·© É¡¶a·©  ·©‹¡ ¹A¡¢                |
 *  |                                                     |
 *
 *
         1         2         3         4         5         6         7
12345678901234567890123456789012345678901234567890123456789012345678901234567890

    1993‘e 05¶© 22·© É¡¶a·©  µa‹¡¬á¦Èá ‹ai· ·©‹¡µA ”Ðe ˆe”eÐe ¹A¡¢....
    (5)    (12) (17) (22)    (30)
 *
 */

/*
 *  ¸aža Ìa·©· Š¬÷
 *
 *  930523JR.NAL·¡ 93‘e 5¶© 23·©· ·©‹¡ Ìa·©·¡‰¡
 *  93052300.PCX ~ 93052399.PCX·¡ ·¡i· ¬a»¥Àó·¡”a.
 */

/*
 * i¼aµÁ ·©‹¡ ¹A¡¢
 */
struct date_title_t {
  byte year[5];
  byte month[3];
  byte day[3];
  byte yoil[3];
  byte title[56];
};

/*
 *  ¬‚·¥ Ìa·©· Š¬÷
 *
 *  93-INDEX.IDX   <-- 93‘e•¡ ¦…· ·©‹¡µA ”Ðe ¬‚·¥Ìa·©
 */

/*
 *  ¬‚·¥ ‰ÅŸ¡Ÿi ¶áÐe ‹¡¥¥ ”e¶á
 */
struct index_t {
  struct date_title_t date_title;
  struct index_t *prior;
  struct index_t *next;
} *first, *last, *current;

/*
 *  ´á˜å i· ·©‹¡µA ”Ðe ¬‚·¥·¡ ·¶“e»¡ ¹¡¬a
 *
 *  93‘e 5¶© 23·©· ‰w¶ date“e "930523JR"·¡ ·¶·a¡e ‹ai·
 *  ¹A¡¢·i titleµA ý´áº…”a.
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
 *  ¬ i¼aµÁ ¹A¡¢ ·³b
 *
 *  •©aº“e ˆt·¡ Àq·¡¡e ‹ai· ·©‹¡Ÿi ‹a· ³a‹¡ ¯¡¸bÐa¡e –A‰¡
 *   e´¢ ˆá»µˆt·¡¡e ·©‹¡ ³a‹¡Ÿi Í¡‹¡Ða“e ‰w¶ˆa –E”a.
 *  ‹aŸ¡‰¡ Ðeˆa»¡ ”á º·Ði ˆõ·e ·¡ Ðq®Ÿi ¦Ÿe ‰µµA¬á“e ¢ ‹ai· ·©‹¡ˆa
 *  ¯©¹A¡ —¡¯aÇa ¬wµA ·¡£¡ ·¶“e»¡Ÿi ¹¡¬aÐa“e ˆõ·¡ º—¶aÐa”a.
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
  byte *yoil[7] = { "·©", "¶©", "ÑÁ", "®", "¡¢", "‹q", "É¡" };
  byte buffer[80], title[80];
  int x_crd[] = { 5, 12, 17, 22, 30 };
  int cnt, cuttail, isquit;
  bool onoff, ans;

  cnt = isquit = 0, cuttail = _cuttail;

  /*
   * µ¡“i i¼aŸi ·ª´á¬á ¸á¸w
   */
  getdate(&today);
  sprintf(buf.year, "%04d", today.da_year);
  sprintf(buf.month, "%02d", today.da_mon);
  sprintf(buf.day, "%02d", today.da_day);
  strcpy(buf.yoil, yoil[get_yoil(today.da_year, today.da_mon, today.da_day)]);

  /*
   * µ¡“i i¼a¡ ¸b¬÷–E ·©‹¡· ¬‚·¥·¡ ·¶“e»¡ ¹¡¬aÐ¬á ·¶·a¡e
   * ‹ai· ¹A¡¢·i ·ª´áµ¥”a.
   */
  sprintf(buffer, "%02d%02d%02dJR", today.da_year % 100, today.da_mon, today.da_day);
  if(find_date_title(buffer, title))
    sprintf(buf.title, "%55s", title);
  else
    sprintf(buf.title, "%55s", " ");

  /*
   * ·³b¤h‹¡¶áÐe ÑÁ¡e Ëi·i Š¬÷ - Ðeœa 2.23· hgetdata()Ðq® ·¡¶w
   */
  strcpy(pic.year, pic[0]);
  strcpy(pic.month, pic[1]);
  strcpy(pic.day, pic[2]);
  strcpy(pic.yoil, pic[3]);
  strcpy(pic.title, pic[4]);

  /*
   * µ¡“i i¼aŸi ¹A¯¡Ða¡e¬á i¼aŸi ·³b ¤h“e”a.
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
 *                    ¯©¹A¡ ¸ažaŸi ÀáŸ¡Ða“e ¦¦…                           *
 * ************************************************************************ */

void dl_store(struct index_t *new, struct index_t **a_1st, struct index_t **a_last)
{

/* ¸ažaŸi 2»¥ a¢´eµA ¡¶¡ ý“e”a. */
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

/* ¸aža Àx‹¡ */
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

/* ¸aža Àx‹¡ */
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

/* ¸aža ptr·i »¡¶‹¡ */
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

/* ¸aža »¡¶‹¡ */
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

/* ¸aža Ìa·© ¸á¸wÐa‹¡ */
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

/* ¸aža Ìa·©µA ³a·¡“e ¥e®—i Á¡‹¡ÑÁ */
void reset_data(void)
{
  first = last = cur_rec = NULL;
/*  strncpy(cur_rec->data, "", record_size); */
  num_recs = cur_rec_num = 0;
  recs_changed = 0;
}

/* ¸aža Ìa·© ·ª´áµ¡‹¡ */
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
/*      cur_rec = first = last = NULL;
      cur_rec->data[0] = NULL;*/
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

/*
 *  Ìa·© n_index.c · {
 */
