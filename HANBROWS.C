/*
 *  �� �a���e �a���A HANQUERY.C �A ���e�a.
 *  �a�����a��e HANQUERY.C -->  HANCOMMD.C�� �a���a..
 *
 *  �a���� ���e �����e �a�q�� �{�a.
 *
 *     +-----------------------------------------------------------+
 *   1 |   �A�A ����                                               |
 *     +-----------------------------------------------------------+
 *   2 |   ���e �A��                                               |
 *     +-----------------------------------------------------------+
 *   3 |   �b �a�a�a�� ���q                                        |
 *     +-----------------------------------------------------------+
 *     |                                                           |
 *     .                                                           .
 *     .                                                           .
 *     .                                                           .
 *     +-----------------------------------------------------------+
 *  24 |   �a�a�wȁ�i �a���e�a.  �� �a�a���� �e�� �a�a��ѡ �a��..  |
 *     +-----------------------------------------------------------+
 *  25 |   �a�w��i �a���e�a.                                      |
 *     +-----------------------------------------------------------+
 */


/*
 *  �a�a�i �i�a���� (Browse)
 *
 *  HANBROWS.C
 */

void db_list(int file_no, int key_no, int *dlist)
{
  char *buf;
  int rcdcnt = 0;

  buf = (char *)malloc(rlen(file_no));
  errno = 0;
  if(key_no)                          /* ������ѡ�i ����Ж�a�e */
    first_rcd(file_no, key_no, buf);  /* �a�a�� �A�� ��q�A�� ���b */
/*  disp_header(dlist); */                /* �i�a�� �b �a�a�i ���e�A �a���e�a */
  while(errno != D_EOF) {             /* �a�� �{�a�� */
    if(key_no) {                      /* ������ѡ�i ����Ж�a�e */
      disp_list(file_no, file_ele[file_no], dlist, buf);
      rcd_cnt++;
      next_rcd(file_no, key_no, buf); /* �a ���� ���ᝡ */
    }               /* �a�a�a ��w�E ������ ����A �a�a �a�a�i �a���e�a. */
    else if(seqrcd(file_no, buf) != DBERROR)  {
      disp_list(file_no, file_ele[file_no], dlist, buf);
      rcd_cnt++;
    }
    /* �a�a �����A �a�e �១�� �a�e ���b�A �a�e �a�a ��ѡ�� �១ �w�w.. */
    if()
    /* �a�a�� �wȁ... �e�� �y���� �a�a ���A �y�弁 �a�a����.. */
  }
}

static int headlen(int el)
{
  el--;
  return strlen(elmask[el]) < strlen(hdename[el]) ? strlen(hdename[el]) : strlen(elmask[el]);
}

/* b�e ���e ����A�� �e�� ϩ�a�� ��á  ͡����, �a�a �a���i ���e�A �b */
void disp_field(char *b, char *msk, int el)
{
   char merge[80];
   register i = 0;

   if(eltype[el-1] == 'G') { /* �a�� �a���� �w�� */
     hputs("*�a��*");
   }
   else { /* �a ���A�� �w�� */
     while (*msk)    {
       if(*msk & 0x80) {
         merge[i++] = *msk++;
         merge[i++] = *msk++;
       }
       else {
         merge[i] = strchr(picture, *msk) ? *b++ : *msk;
         msk++, i++;
       }
     }
     merge[i] = '\0';
     hputs(merge);   /* ���e�i�i ͡�q�e �a�a�a���� ���a�i */
   }
}


/*
 *  �a�a�i �e�� �a���e�a.
 */

void disp_list(int file_no, int *filelist, int *dlist, char *buf)
{
  char *line, *cur_ptr, *mask_ptr;
  int width, line_width = 0;

  line = (char *)malloc(epos(0, dlist));
  if(!line) {
    memerror();
    break;
  }
  rcd_fill(buf, line, filelist, dlist);
  cur_ptr = line;
  while(*dlist) {   /* �a�a�����A ���e �a�a �a�� �����e�q */
    mask_ptr = elmask[(*dlist)-1];
    width = headlen(*dlist++);  /* �a�a �i�� ������ �e�i�a�a�a�� ���q�� �� �� */
    line_width += width + 1;
    if(line_width > 78)         /* ���e �a�����a �ỡ�e �a�e !!! */
      break;
    disp_field(line, mask_ptr, (*dlist));
  }
  free(line);
}

/*
 *  �i�a���� ������
 */

static void init_browse(int file_no, char *title, int *data_ele[])
{
  file = file_no;
  count = init_data_ele(data_ele);
  if (count == 0)  {		      /* ���e �a�a�a���i �១�a�V�a. */
    len = rlen(file);
    els = file_ele [file];
    for(i = 0; els[i]; i++)	  /*  *file_ele[]�� �a���� ���� */
      ;
    no_ele = i;
  }
  else {
    len = epos(0, iplist);
    els = data_ele;
    no_ele = count;
  }
}

/*
 *  ���b�q���� ���e ����a ���a���g�e�a ��a�a����...
 *
 *  key_no : 0 ���e �a�a�a ��w�E ���ᝡ
 *           1 ���w�� ���a�e �a ������ѡ�A �a�a ���� ���a�� ..
 */

void db_browse(int file_no, int key_no, char *title, int data_ele[])
{
  init_browse(file_no, title, data_ele);
  db_list(file_no, key_no, els);
}

