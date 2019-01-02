/*
 *  ·¡ Ìa·©·e aº—µA HANQUERY.C µA ý“e”a.
 *  ‹aŸ¡‰¡a¬á“e HANQUERY.C -->  HANCOMMD.C¡ ¤aŽ¸a..
 *
 *  ‹aŸ¡‰¡ ÑÁ¡e Š¬÷·e ”a·q‰Á ˆ{”a.
 *
 *     +-----------------------------------------------------------+
 *   1 |   ¡A“A ¦¦…                                               |
 *     +-----------------------------------------------------------+
 *   2 |   ÑÁ¡e ¹A¡¢                                               |
 *     +-----------------------------------------------------------+
 *   3 |   ˆb ¸aža¶a­¡ ·¡Ÿq                                        |
 *     +-----------------------------------------------------------+
 *     |                                                           |
 *     .                                                           .
 *     .                                                           .
 *     .                                                           .
 *     +-----------------------------------------------------------+
 *  24 |   ¸aža¬wÈŸi Îa¯¡Ðe”a.  Á· ¸aža®µÁ Ñe¸ ¸aža¤åÑ¡ ˜a¶á..  |
 *     +-----------------------------------------------------------+
 *  25 |   ¬a¶w¤ó·i Îa¯¡Ðe”a.                                      |
 *     +-----------------------------------------------------------+
 */


/*
 *  ¸ažaŸi ÍiÁa¥¡‹¡ (Browse)
 *
 *  HANBROWS.C
 */

void db_list(int file_no, int key_no, int *dlist)
{
  char *buf;
  int rcdcnt = 0;

  buf = (char *)malloc(rlen(file_no));
  errno = 0;
  if(key_no)                          /* ¬‚·¥¤åÑ¡Ÿi »¡¸÷Ð–”a¡e */
    first_rcd(file_no, key_no, buf);  /* ¸aža· ¹A·© Àá·qµA¬á ¯¡¸b */
/*  disp_header(dlist); */                /* ÍiÁa¥© ˆb ¸ažaŸi ÑÁ¡eµA Îa¯¡Ðe”a */
  while(errno != D_EOF) {             /* Ìa·© {Œa»¡ */
    if(key_no) {                      /* ¬‚·¥¤åÑ¡Ÿi »¡¸÷Ð–”a¡e */
      disp_list(file_no, file_ele[file_no], dlist, buf);
      rcd_cnt++;
      next_rcd(file_no, key_no, buf); /* ‹a ¬‚·¥ ®…¬á¡ */
    }               /* ¸ažaˆa ¸á¸w–E ¢‰Ÿ¡¸â ®…¬áµA ˜aœa ¸ažaŸi Îa¯¡Ðe”a. */
    else if(seqrcd(file_no, buf) != DBERROR)  {
      disp_list(file_no, file_ele[file_no], dlist, buf);
      rcd_cnt++;
    }
    /* ¸aža ˆ•®µA ˜aŸe ÀáŸ¡µÁ ¸aÌe ·³bµA ˜aŸe ¸aža ¤åÑ¡· ÀáŸ¡ —w—w.. */
    if()
    /* ¸aža· ¬wÈ... Ñe¸ ¡yˆ· ¸aža º—µA ¡y¤å¼ ¸aža·¥»¡.. */
  }
}

static int headlen(int el)
{
  el--;
  return strlen(elmask[el]) < strlen(hdename[el]) ? strlen(hdename[el]) : strlen(elmask[el]);
}

/* b“e ÑÁ¡e ¤áÌáµA¬á Ñe¸ Ï©—a· ¶áÃ¡  Í¡·¥Èá, ¸aža ¶a­¡Ÿi ÑÁ¡eµA Â‰b */
void disp_field(char *b, char *msk, int el)
{
   char merge[80];
   register i = 0;

   if(eltype[el-1] == 'G') { /* ‹aŸ± Ìa·©· ‰w¶ */
     hputs("*‹aŸ±*");
   }
   else { /* ‹a ·¡¶A· ‰w¶ */
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
     hputs(merge);   /* ÑÁ¡eÈi·i Í¡ÐqÐe ¸aža¶a­¡· ¢…¸aµi */
   }
}


/*
 *  ¸ažaŸi Ðeº‰ Îa¯¡Ðe”a.
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
  while(*dlist) {   /* ¸aža¡¢¢µA ·¶“e ¸aža ¶a­¡ ˆ•® eÇq */
    mask_ptr = elmask[(*dlist)-1];
    width = headlen(*dlist++);  /* ¸aža Èi· ‹©·¡µÁ Ðe‹i¸aža¶a­¡ ·¡Ÿqº— ‹¥ ˆõ */
    line_width += width + 1;
    if(line_width > 78)         /* ÑÁ¡e Ça‹¡¥¡”a Äá»¡¡e ‹a e !!! */
      break;
    disp_field(line, mask_ptr, (*dlist));
  }
  free(line);
}

/*
 *  ÍiÁa¥¡‹¡ Á¡‹¡ÑÁ
 */

static void init_browse(int file_no, char *title, int *data_ele[])
{
  file = file_no;
  count = init_data_ele(data_ele);
  if (count == 0)  {		      /* ¡¡—e ¸aža¶a­¡Ÿi ÀáŸ¡Ða‰V”a. */
    len = rlen(file);
    els = file_ele [file];
    for(i = 0; els[i]; i++)	  /*  *file_ele[]· ¶a­¡· ˆ•® */
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
 *  ·³bÐq®µÁ ÑÁ¡e ¤áÌáˆa Â—•©Ða»¡´g“eˆa ˆñ¬aÐa•¡¢...
 *
 *  key_no : 0 ·¡¡e ¸ažaˆa ¸á¸w–E ®…¬á¡
 *           1 ·¡¬w· ®œa¡e ‹a ¬‚·¥¤åÑ¡µA ˜aœa ¬‚·¥ ®…·a¡ ..
 */

void db_browse(int file_no, int key_no, char *title, int data_ele[])
{
  init_browse(file_no, title, data_ele);
  db_list(file_no, key_no, els);
}

