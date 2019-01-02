/*
 *  ·¡Ÿq·i »¡¸÷Ða»¡ ´g´v·i ˜· Ìa·©·¡Ÿq
 */
#define  INIT_FILE   "UNTITLED.TXT"

/*
 * Ìa·© Â‰bÀwµA¬á ”až‰ ® ·¶“e Ìa·©· ÂA” ®
 */
#define  MAX_FILES   200

enum yesno_t { YES_ANS, NO_ANS, CANCEL_ANS };
enum loadmode_t { INIT_LOAD, BLOCK_LOAD };

/*
 *  ¸á¸wÐi ˆõ·¥»¡ ÑÂ·¥
 *  µ : 0, ´a“¡µ¡ : 1, Âá­¡ : 2
 */
static int ask_save(void)
{
  unsigned char msg[40];
  int ans;

  if(!saved && memory!=eofm ) {
    sprintf(msg,"%sŸi ¸á¸wÐiŒa¶a?",split_filename(work_file));
    ans = yesno(msg, YES_ANS);
    if(ans == YES_ANS)
      save_file(memory, eofm, work_file);
  }
  return ans;
}

static void new_file(void)
{
  int ans;

  ans = ask_save();
  if(ans != CANCEL_ANS) {
    initial();
    strcpy(work_file, INIT_FILE);
    status_line();
    user_task = TASK_DISPLAYPAGE;
  }
}

static int load(void)
{
  char temp_name[80];
  int  success = NO;

  strcpy(temp_name, /* open_file */work_file);
  if(get_filename_to_read(temp_name) == YES_ANS) {
    if(files(temp_name) != NULL) {
      if(load_file(temp_name, INIT_LOAD)) {
        strcpy(work_file, temp_name);
        user_task = TASK_DISPLAYPAGE;
        status_line();
        success = YES;
      }
    }
/*    strcpy(open_file, temp_file); */
  }
  return success;
}

static void block_read(void)
{
  char temp_name[80];
  char *temp_b_ptr = b_b_ptr;
  unsigned temp_size = block_size;
  
  strcpy(temp_name, block_file);
  if(get_filename_to_read(temp_name) == YES_ANS) {
    if(files(temp_name) != NULL) {
      b_b_ptr = current;
      block_size = load_file(temp_name, BLOCK_LOAD);
      if(block_size) {
        b_k_ptr = current+block_size;
        user_task = TASK_DISPLAYPAGE;
        block_display = YES;
      }
      else {
        b_b_ptr = temp_b_ptr;
        block_size = temp_size;
      }
    }
    strcpy(block_file, temp_file);
  }
}

static unsigned int ctrl_z_delete(unsigned index)
{
  if((*(current+index-1)) == 0x1a) {
    movmem(current+index, current+index-1, (size_t )(eofm-current+1));
    index -= 1;
  }
  return index;
}

static unsigned int load_file(char *filename, int load_mode)
{
  int handle;
  long     l_size;
  unsigned size;

  size = 0;
  errno = 0;

  if((handle = _open(filename, 0x8001)) != -1) {
    l_size =filelength(handle);
    if(load_mode == INIT_LOAD) {
      if(ask_save() != CANCEL_ANS) {
        if(l_size > MAXCHAR) {
          if(yesno("¡A¡¡Ÿ¡ˆa ¦¹¢Ðs“¡”a. –õ¦¦…·i ¸aŸiŒa¶a?") == YES_ANS)
            l_size = MAXCHAR;
          else 
            l_size = -1L;
        }
        initial();
      }
      else  
        l_size = -1L;
    }
    else {
      if(l_size > (MAXCHAR-(long)(eofm-memory))) {
        memerror();
        l_size = -1L;
      }
    }
    if(l_size != -1L) {
      size = (size_t)(l_size);
      movmem(current, current+size, (size_t)(eofm-current)+1);
      if(_read(handle, current, size)== -1)
        movmem(current+size, current,(size_t)(eofm-current)+1);
      else {
        size = ctrl_z_delete(size);
        max_line += line_count(current, current+size);
        eofm += size;
        strncpy(line_buffer, line_start, MAXLINEBUF);
      }
    }
    _close(handle);
  }
  else 
    errno = 38; /* file not found */
  if(errno)
    errorf();
  return size;
}

static int save(void)
{
  if(memory != eofm) {
    if(!(*work_file)) {
      strcpy(work_file, INIT_FILE);
        status_line(); /*        WORKFILE_status(); */
    }
    if(save_file(memory, eofm, work_file)) {
      saved = YES;
      return YES;
    }
    else
      return NO;
  }
  else
    return YES; /* length 0 error */
}

/*
 *  ¬·¡Ÿq·a¡ ¸á¸w ???
 */
static int write_to(void)
{
  char temp_name[80];
  int  success = NO;

  strcpy(temp_name, write_file);
  if(get_filename_to_write(temp_name) != CANCEL_ANS) {
    if(files(temp_name) != NULL) {
      if(save_file(memory, eofm, temp_name)) {
        strcpy(work_file, temp_name);
        status_line(); /* WORKFILE_status(); */
        saved = YES;
        success = YES;
      }
    }
    strcpy(write_file, temp_file);
  }
  return success;
}

static void block_write(void)
{
  char temp_name[80];

  if(b_k_ptr > b_b_ptr) {
    strcpy(temp_name, block_file);
    if(get_filename_to_write(temp_name) != CANCEL_ANS) {
      if(files(temp_name) != NULL) {
        save_file(b_b_ptr, b_k_ptr, temp_name);
      }
      strcpy(block_file, temp_file);
    }
  }
}

int search( char ch, char fn[][13], int start, int all)
{
  int i = start+1;

  if(i < all) {
    while( i < all) {
      if((ch == fn[i][0]))
        return i;
      else
        i++;
    }
  }
  else
    start = all;
  i = 0;
  while(i < start) {
    if(ch == fn[i][0])
      return i;
    else
      i++;
  }
  return -1;
}

static int get_response(char filename[][13], int count, int dir_count)
{
  unsigned   ch;
  int   select_done;
  int i, topi, last, temp;
  unsigned ox, oy, barx, bary;
  bool onoff;

  select_done = i = topi = 0, barx = bary = 1;
  last = display_name(barx, bary, 0, count, filename);
  while(!select_done) {
    ox = barx, oy = bary;
    onoff = isreverse();
    if(!onoff)
      hsetreverse(ON);
    hprintf(ox, oy, " %12s ", filename[topi+i]);
    hsetreverse(onoff);
    ch = getchext();
    switch(ch) {
      case LEFTARROW :
        i--;
        if(i < 0) {
          if(topi-4 <0 ) {
            i = 0;
          }
          else {
            topi -= 4;
            i = 3;
            last = display_name(ox, oy, topi, count, filename);
          }
        }
        break;
      case RIGHTARROW :
        i++;
        if(i > last) {
          if(last == 35) {
            if(topi+36 < count) {
              topi += 4;
              i = 32;
              last = display_name(ox, oy, topi, count, filename);
            }
            else {
              i = last;
            }
          }
          else
            i = last;
        }
        break;
      case  UPARROW :
        if(i-4 < 0) {
          if(topi-4 >= 0) {
            topi -= 4;
            last = display_name(ox, oy, topi, count, filename);
          }
        }
        else
          i -=4;
        break;
      case  DOWNARROW :
        if(i+4 > last) {
          if(topi+36 <count) {
            topi +=4;
            last = display_name(ox, oy, topi, count, filename);
            if(i>last) i-=4;
          }
        }
        else
          i += 4;
        break;
      case  HOMEKEY :
        if(topi != 0) {
          topi=0;
          last = display_name(ox, oy, topi, count, filename);
        }
        i = 0;
        break;
      case  ENDKEY  :
        if(topi+36 < count) {
          i = 31+(count & 0x03);
          topi = count-(i+1);
          last = display_name(ox, oy, topi, count, filename);
        }
        else
          i = last;
        break;
      case  PGUPKEY :
        if(topi != 0) {
          topi -= 36;
          if(topi < 0)
            topi = 0;
          last = display_name(ox, oy, topi, count, filename);
        }
        else
          i = i & 0x03;
        break;
      case  PGDNKEY :
        temp = topi+36;
        if(temp < count) {
          if((temp+(i&0x03)) < count)
            topi = temp;
          else
            topi += 32;
          last = display_name(ox, oy, topi, count, filename);
          if(i > last)
          i = last-((last-i)&0x03);
        }
        else
          i = last-((last-i)&0x03);
        break;
      case  CR   :
        select_done = 1;
        break;
      case  ESC  :
        return -1;
      default    :
        temp = search(toupper(ch), filename, topi+i, count-dir_count);
        if(temp >= 0) {
          if(topi <= temp && topi+36 > temp)
            i = (temp-topi)%36;
          else {
            topi = temp & 0xfffc;
            i = temp & 0x0003;
            last = display_name(ox, oy, topi, count, filename);
          }
        }
    }
    barx = (i&0x0003)*15+1;
    bary = (i&0xfffc)+1;
    hprintf(barx, bary, " %12s ", filename[topi+i]);
/*    onoff = isreverse();
    if(!onoff)
      hsetreverse(ON);
    hprintf(ox, oy, " %12s ", filename[topi+i]);
    hsetreverse(onoff); */
  }/* while */
  return topi+i;
}

/*
 *  ¡¡ —¡BÉ¡Ÿ¡(..)Ÿi ŠÐe”a.
 */
staitc char *get_modir(char *dir)
{
  int index;

  if(*dir) {
    index=(strlen(dir))-2;
    while(index >= 0 && dir[index] != '\\' && dir[index] != ':')
      index--;
    dir[index+1] = NULL;
  }
  return dir;
}

static int display_name(int ox, int oy, int topi, int count, char filename[][13])
{
  unsigned index, last, x, y;

  last = (count-topi) > 35 ? 35 : count-topi-1;

  for(index = 0; index < 36; index++) {
    x = (index & 0x0003)*15+1;
    y = (index & 0xfffc)+1;
    if(index > last)
      hprintf(x, y, " %12s ", " ");
    else
      hprintf(x, y, " %12s ", filename[topi+index]);
  }
  return last;
}

static char *split_filename(char *path)
{
  int index;

  if(*path) {
    index = (strlen(path))-1;
    while(index >= 0 && path[index] != '\\' && path[index] != ':')
      index--;
    return(path+index+1);
  }
  return NULL;
}

static char *files(char *name)
{
  void far *wp;
  struct ffblk ffblk;
  int  index, dir_count, count, response, select_done = NO;
  char path_name[66], search_name[13], drive[3], dir[66], file[9], ext[5];
  char filename[MAX_FILES][13], otherdir[20][13];

  while(*name) {
    if((*name)==' ')
      name++;
    else
      break;
  }
  fnsplit(name, drive, dir, file, ext);
  if((*drive) == NULL)
    sprintf(drive, "%c:", getdisk()+'A');
  if((*dir) == NULL) {
    if(getcurdir(drive[0]-'A'+1, path_name) == -1) {
      errorf();
      return NULL;
    }
    sprintf(dir, "\\%s%c", path_name, (*path_name) ? '\\' : '' );
  }
  else
    strcat(dir, (dir[strlen(dir)-1]=='\\') ? "" : "\\");
  sprintf(search_name, "%s%s", (*file) ? file : "*",ext);

  if(strchr(search_name,'*') || strchr(search_name,'?') ) {
    if(wopen(11, 10, 59 9)) {
      do {
        sprintf(temp_file, "%s%s%s", drive, dir, "*");
        dir_count=0;
        errno = 0;
        findfirst(temp_file, &ffblk, FA_DIREC);
        while(errno == 0 && dir_count < 20) {
          if(ffblk.ff_attrib==16)
            sprintf(otherdir[dir_count++], "%s\\", ffblk.ff_name);
          findnext(&ffblk);
        }
        sprintf(temp_file, "%s%s%s", drive, dir, search_name);
        wtitle(temp_file);
        count = 0;
        errno = 0;
        findfirst(temp_file, &ffblk, 0x00);
        while(errno == 0 && count < MAX_FILES) {
          sprintf(filename[count++], "%s", ffblk.ff_name);
          findnext(&ffblk);
        }
        if(count > 0)
          qsort(filename,count,13,strcmp);
        if(dir_count)
          qsort(otherdir, dir_count, 13, strcmp);
        index=0;
        response=0;
        if(dir_count && otherdir[0][0]=='.') {
          index = 2, dir_count--, response = 1;
        }
        while (count < MAX_FILES && index < dir_count+response) {
          sprintf(filename[count++],"%s",otherdir[index++]);
        }
        if(response)
          sprintf(filename[count++], "%s", otherdir[0]);
        if(count > 0) {
          response = get_response(filename, count, dir_count);
          if(response != -1)
            sprintf(name, "%s%s%s", drive, dir, filename[response]);
          else {
            wclose();
            return NULL;
          }
        }
        else {
          errno=38;
          errorf();
          wclose();
          return NULL;
        }
        if(strchr(filename[response],'\\') == NULL)
          select_done = YES;
        else {
          if(filename[response][0]=='.')
            strcpy(dir, get_modir(dir));
          else
            strcat(dir, filename[response]);
        }
      }while( !select_done );
      wclose();
      return name;
    }
    else {
      memerror();
      return NULL;
    }
  }
  else
    sprintf(temp_file, "%s%s%s", drive, dir, search_name);
  return temp_file
}

static bool change_dir(void)
{
  char  *tptr;
  char  path[80], drive[3], dir[66], name[13], ext[4];

/*  wptr = hwopen( 17, 29, 32, 11 );
  sprintf(drive,"%c:",getdisk()+'A');
  getcurdir(0,dir);
  sprintf(path,"%s\\%s",drive,dir);
  heout( 168,118, "¬¡¶… Drivea Path“e ?");
  line(137,136,390,136);
  tptr = get_string( 144, 140, 30, strlen(path), 1, path);
  hwclose( 17, 29, wptr );
  if ( tptr!=NULL )
          {       strupr(tptr);
  fnsplit(tptr,drive,dir,name,ext);
  if((*drive)!=NULL) setdisk(drive[0]-'A');
  if ( chdir(tptr)==0 )
          return( YES );
  else    {errorf();return( NO  );}
  }
  else   return( NO ); */
}


