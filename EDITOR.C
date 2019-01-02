#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#define  BS      8
#define  CR      13
#define  LF      10
#define  ESC     27
#define  HOME    327
#define  END     335
#define  PGUP    329
#define  PGDN    337
#define  LEFT    331
#define  RIGHT   333
#define  UP      328
#define  DOWN    336
#define  INS     338
#define  DEL     339
#define  MAXCOL  79
#define  MAXCO_1 80      /* MAXCOL + 1 */
#define  HEADLIN 1       /* HEAD - 1 */
#define  HEAD    2
#define  HEAD_1  3       /* HEAD + 1 */
#define  BOTTOM  21
#define  BOTTO_1 22      /* BOTTOM + 1 */
#define  HE_BO_1 24      /* HEAD + BOTTOM + 1 */
#define  NORMAL  0x0700
#define  INTENSE 0x0F00
#define  REVERSE 0x7000
#define  BLINK_REVERSE 0xF000
#define  I_SPACE 0x0F20  /* INTENSE + ' ' */
#define  N_SPACE 0x0720  /* NORMAL + ' ' */
#define  YES     1
#define  NO      0

const unsigned int MAXCHAR = 57200;
typedef struct {
  int x, y, width, height;
  int *wind_mem;
} window;
struct WORDREGS
    { unsigned int ax,bx,cx,dx,si,di,cflag,flags;};
struct BYTEREGS
    { unsigned char al,ah,bl,bh,cl,ch,dl,dh; };
union  REGS
    { struct WORDREGS x;  struct BYTEREGS h; };

extern int errno;    /* Gloabal Variables */
extern char *sys_errlist[];
char *memory, *line_start, *current, *last_pos, *b_b_ptr,
     *b_k_ptr, *eofm, work_file[80], block_name[80],
     line_buffer[161], window_x=0, insert_mode = YES,
     indent_mode = YES, curx, cury, saved, block_display;
int far *VRAM;
unsigned int BLOCK_COLOR, user_task, max_line, base_y,
	     base_x, block_size;

void display_page(), wclose(), errorf(), mode();
int  get_key(), strnicmp(), strncmp();
char *one_line();
window *wopen();

void initial(void)
{
 line_start=current=last_pos=b_b_ptr=b_k_ptr=eofm=memory;
 *memory=*work_file=*line_buffer=curx=cury=0;
 max_line=base_y=base_x=block_size=0;
 saved=block_display=YES;
 user_task=0x0100;
}

void gotoxy(int x, int y)
{
  union REGS regs;
  regs.h.ah = 2;
  regs.h.bh = 0;
  regs.h.dh = y;
  regs.h.dl = x;
  int86(16,&regs,&regs);
}

void scroll(direction, lines, x1, y1, x2, y2)
int direction, lines, x1, y1, x2, y2;
{
 union REGS reg;
 reg.x.ax = lines+(direction<<8);
 reg.x.bx = NORMAL;
 reg.x.cx = (y1<<8)+x1;
 reg.x.dx = (y2<<8)+x2;
 int86(0x10,&reg,&reg);
}

char *pre_ptr(char *pt,int count)
{
 int line=0;
 while ( line<=count && pt>memory)
   if(*(--pt) == LF && *(pt-1)==CR) line++;
 return((line<=count)?memory:pt+1);
}

char *next_ptr(char *pt,int count)
{
 int line=0;
 while(line<count && pt<eofm)if(*pt++ == CR && *pt==LF)
			       {++pt; ++line; }
 return(pt);
}

unsigned int width(char *pt)
{
 unsigned int size=0;
 while(eofm>pt && !(*pt++ == CR && *pt==LF)) ++size;
 return(size);
}

void writef(int sx,int ex,int y,int color,char *msg)
{
 int far *video;
 video = VRAM + (sx+ y*80);
 while(*msg) { *video++ = color + *msg++; ++sx; }
 while(sx++ <ex)*video++=N_SPACE;
}

/* Interrupt which called when Hardware error occur */
int handler(void)
{
  hardretn(-1);
}

void down_line(int delta)
{
 if (base_y+cury+delta<=max_line)
    one_line(next_ptr(line_start,delta),cury+delta);
}

void scroll_2(int direc,int y ) /*Insline, Delline */
{
 scroll(direc,1,0,y+HEAD,79,HE_BO_1);
}

void user_f(int kbd_check)  /* display with Bit masking */
		 /* Here, can add print spooler routine */
{
 if(user_task & 0x0100) display_page(kbd_check);
 else {
    if (user_task & 0x0040)scroll_2(6,0);
    else if (user_task & 0x0020)scroll_2(6,cury);
	 else if (user_task & 0x0010)scroll_2(7,cury);
    if((user_task&0x0008)&&cury)
      one_line(pre_ptr(line_start,1),cury-1);
    if(user_task&0x0004)down_line(1);
      else if(user_task & 0x0002)down_line(BOTTO_1-cury);
    if(user_task & 0x0001)one_line(line_start,cury);
    user_task = 0;
 }
}

int get_key(int x,int y)
{
 int lo,hi,key;
 if(user_task && !kbhit()) user_f(YES);
 gotoxy(x,y);
 key=bioskey(0);
 lo = key & 0x00FF;
 hi = (key & 0xFF00)>>8;
 return((lo==0)?hi+256:lo);
}

int get_key2(int indicator)
{
 #define PREFIX 0x075e /* NORMAL + '^' */
 int key;
 *(VRAM+80)=PREFIX;  *(VRAM+81)=NORMAL+ (char)indicator;
 key = get_key(2,HEADLIN); /* alphabet to cntrl char */
 if(('A'<=key&&key<='Z')||('a'<=key&&key<='z'))key &= 31;
 *(VRAM+80) = I_SPACE; *(VRAM+81) = I_SPACE;
 return(key);
}

window *wopen(int x, int y, int width, int height)
{
  #define EDGE 0x072B /* NORMAL + '+' */
  #define VBAR 0x077C /* NORMAL + '|' */
  #define HVAR 0x072D /* NORMAL + '-' */
  window *w;
  int j, k=0, far *video, i;  /* window open */

  if(user_task) user_f(NO);
  if( (w=(window *)malloc(sizeof(window)) ) == NULL ) exit(1);
  if( (w->wind_mem = (int *) malloc( (width+1)*(height+1)*2) ) == NULL ) exit(1);

  video = VRAM + (x+y*80);
  for( i=0 ; i<=height ; ++i ){
    for( j=0 ; j<=width ; ++j ) w->wind_mem[k++] = *(video+j);
    video += 80;
  }
  w->x =x; w->y=y;
  w->width=width; w->height=height;
  scroll(6,0,x,y,x+width,y+height); /* clear window */
  height *=80;
  video -= (height+80);  /* draw edge lines */
  for(j=1;j<width;j++)
     { *(video+j)=HVAR; *(video+j+height)=HVAR;  }
  for(j=80;j<height;j+=80)
     { *(video+j)=VBAR; *(video+j+width)=VBAR;  }
  *video   =EDGE; *(video+width) =EDGE;
  *(video+=height)=EDGE;  *(video+=width)=EDGE;
  return (w);
}

void wclose(window *wd)     /* window close */
{
 int *pt = wd -> wind_mem;
 int j,far *video,i;

 video = VRAM +(wd->x +(wd->y)*80);
 for(i=0;i<=wd->height;++i){
   for(j=0;j<=wd->width; ++j)  *(video+j) = *pt++;
   video += 80;
 }
 free(wd->wind_mem);
 free(wd);
}

int get_response(entry,first_ch_check,choice,num_x,
			    num_y,locate,top_line,total)
char entry[][13];
int first_ch_check,choice,num_x,num_y,
    locate[],top_line,total;
{
 int x,y=0;
 int base=0,selected,display= YES;
 int key,t_x,t_y;
 x=choice;
 do {
   if (display){
     t_x=x; t_y=y; selected=base;
     for(y=0; y<num_y; ++y) {
	for(x = 0;x<num_x;++x)
	writef(locate[x],locate[x+1],top_line+y,
			NORMAL,entry[selected++]);
     }
     display = NO; x = t_x; y = t_y;
   }
   selected = base + x +y*num_x;
   writef(locate[x],locate[x+1],top_line+y,
		   REVERSE,entry[selected]);
   key = get_key(locate[x], top_line+y);
   writef(locate[x],locate[x+1], top_line+y,
		    NORMAL,entry[selected]);
   switch (key)  {
   case LEFT  : if(x) {   --x; break; }
		if(total >= num_x) x = num_x-1;
   case UP    : if(y) --y;
		 else if(base>0)
		  { base -= num_x; display = YES; }
		break;
   case RIGHT : if (x+1<num_x){
		  if(selected+1 < total) ++x;
		  break;
		}
		x = 0; selected -= (num_x-1);
   case DOWN  : if(num_x+selected+1 > total)break;
		if(y+1<num_y) ++y;
		else { base += num_x; display = YES; }
		break;
   default    :if(first_ch_check && key<256) {
		 for(t_x=0; t_x<num_x; ++t_x)
		   if(*entry[t_x]==toupper(key))
		     { selected = t_x; key = CR; break; }
	       }
   }
 } while(key!=CR && key!=ESC);
 return((key==ESC)? -1:selected);
}

int insert(unsigned int size,char content)
{
 if (MAXCHAR-(unsigned int)(eofm-memory)<size) return(NO);
 movmem(current,current+size,
	  (unsigned int)(eofm-current+1));
 if((*current=content)==CR)
	{ *(current+1)=LF; ++max_line;}
   else user_task |= 0x0001;
 eofm += size;
 saved = NO;
 if(last_pos>current)last_pos += size;
 if(block_size){
     if(b_b_ptr > current) b_b_ptr += size;
     if(b_k_ptr > current)b_k_ptr += size;
     block_size = (unsigned int)(b_k_ptr - b_b_ptr);
 }
 return(YES);
}

unsigned int line_count(char *start,char *finish)
{
 unsigned int line=0;
 while(finish>start)
      if(*start++ == CR && *start == LF) ++line;
 return(line);
}

unsigned int block_line_count(char *start,char *finish)
{
 if((unsigned int)(finish-start) <
		    (unsigned int)(eofm-(finish-start)))
    return(line_count(start,finish));
 else return(max_line-line_count(memory,start)-
			     line_count(finish,eofm));
}

int delete(unsigned int size)
{
 char *del_e;
 unsigned int line=0;

 if(eofm==current || size ==0) return(NO);
 del_e=current + size;
 if((line=block_line_count(current,del_e))!=0)
	max_line -= line;
   else user_task |= 0x0001;
 movmem(del_e,current,(unsigned int)(eofm-del_e+1));
 eofm -= size;
 saved = NO;
 if(last_pos>current)
    last_pos = (last_pos >del_e)? last_pos-size : current;
 if(block_size) {
   if(current<=b_b_ptr && b_k_ptr <= del_e)
	 { b_b_ptr = b_k_ptr =current;}
   else {
     if(del_e<= b_b_ptr)
	{b_b_ptr-=size; b_k_ptr -=size; }
     else if (current<=b_b_ptr)
	     { b_b_ptr=current; b_k_ptr -= size; }
	  else if(del_e<=b_k_ptr)b_k_ptr -= size;
		 else if(current<=b_k_ptr)
				b_k_ptr = current;
   }
   block_size = (unsigned int)(b_k_ptr - b_b_ptr);
 }
 return(YES);
}

int get_string(int sx, int y, char *string)
{
 int flag = YES,key,x;
 x = strlen(string);
 do {
    writef(sx, sx+30,y, NORMAL, string);
    switch (key = get_key(sx+x,y)) {
      case LEFT :
      case BS   : if(x) --x;
		  break;
      case ESC  : x=0;
		  break;
      default   : if(x<30 && key && key<256 &&
		     strchr("\r\n\x1a",key)==NULL) {
				if(flag) x= 0;
				string[x++] = key; }
      }
      flag = NO;
      string[x]=0;
 }while(key!=CR && key != ESC);
 return(key);
}

/* get string in window */
int win_gets(int fname_option,char *msg,char *str)
{
 window *wd; int key, size;
 char drive[3],dir[66],file[9],ext[5];
 wd = wopen(window_x,1,31,2);
 size = strlen(msg)/2;
 writef(window_x+16-size,window_x+16+size,1,NORMAL,msg);
 key = get_string(window_x+1,2,str);
 wclose(wd);
 if(fname_option)        { strupr(str);
   fnsplit(str,drive,dir,file,ext);
   fnmerge(str,drive,dir,file,ext); }
 return(key);
}

void mode(void)
{
 char msg[80];
 sprintf(msg,"       Line %-4u Col %-4u %s %s %-s",
	    base_y+cury+1,base_x+curx+1,
	    (insert_mode)?"Insert" : "      ",
	    (indent_mode)?"Indent" : "      ",work_file);
 writef(0,80,HEADLIN,NORMAL,msg);
}

int in_block(char *pt)
{
  return(b_b_ptr<=pt && pt<b_k_ptr);
}

char *one_line(char *start,int y) /* display one line */
{
 register char *pt= start;
 register int far *video,pos =0, limit,block_inst, color;
 unsigned int size;

 size = width(start);
 block_inst = block_size && block_display;
 y += HEAD;
 video = VRAM + y*80;
 if(base_x<size) {
  pt += base_x;
  limit = (base_x+MAXCOL>=size)?size-base_x : MAXCO_1;
  for(;pos<limit; ++pos)
	   *video++ = *pt++ + ((block_inst&&b_b_ptr <=
		 pt&&pt<b_k_ptr)?BLOCK_COLOR : INTENSE);
 }
 color=(block_inst&&in_block(start+size))
			   ? BLOCK_COLOR +  ' ' : INTENSE ;
 while(pos++ <=MAXCOL)  *video++ = color;
 start += (size+2);
 return(start);
}

void display_page(int kbd_check)
{
 char *pt;
 int y,last;

 one_line(line_start,cury);
 if(base_y+BOTTOM<max_line)last=BOTTO_1;
 else { last = max_line - base_y;
	scroll(6,0,0,last+HEAD_1,79,HE_BO_1);  }
 pt = pre_ptr(line_start,cury);
 for(y=0;y<=last; ++y)  {
    if(kbd_check && kbhit()) return;
    pt = one_line(pt,y);
 }
 user_task = 0 ;
}

void hor_locate(unsigned int pos)

{

 char cursor[20];
 if (pos<base_x || base_x+MAXCOL<pos)  {
   curx = (pos<base_x) ? 0 : MAXCOL;
   base_x = pos -curx;
   user_task = 0x0100;
 }
 else curx = pos - base_x;
 last_pos = current;
 current = line_start + pos;
 sprintf(cursor,"%-4u Col %-4u",base_y+cury+1,pos+1);
 writef(12,26,HEADLIN,NORMAL,cursor);
}

void ver_locate(unsigned int line_no,char *destin)
{
 if(line_no<base_y || base_y+BOTTOM<line_no) {
   if(cury == 0&&line_no+1==base_y)user_task |= 0x0011;
      else if(cury == BOTTOM && base_y +BOTTO_1==line_no)
		user_task |= 0x0045;
	     else  user_task = 0x0100;
    if(line_no<cury && line_no<base_y)cury = line_no;
    base_y = line_no - cury;
    }
    else cury = line_no - base_y;
 strncpy(line_buffer,line_start,160);
 hor_locate((unsigned int)(destin-line_start));
}

void to_pointer(char *destin)
{
 unsigned int line_no;

 line_no = base_y + cury;
 if(destin<=current) {
    if((unsigned int)(destin-memory)<
		     (unsigned int)(current-destin))
      line_no = line_count(memory,destin);
    else line_no -= line_count(destin,current);
 }
 else {
    if((unsigned int)(destin-current) <
			(unsigned int)(eofm-destin))
      line_no += line_count(current,destin);
    else line_no = max_line - line_count(destin,eofm);
 }
 line_start = pre_ptr(destin,0);
 ver_locate(line_no,destin);
}

void left(void)
{
 if(memory<current) {
    if(current==line_start) to_pointer(current-2);
    else hor_locate(base_x+curx-1);
 }
}

int is_eol(char *pt)
{
 return (*pt==CR && *(pt+1)==LF);
}

void right(void)
{
 if(current<eofm) {
   if(is_eol(current)) to_pointer(current+2);
   else hor_locate(base_x+curx+1);
 }
}

void del(void)
{
 if(is_eol(current)){ if(delete(2))user_task |= 0x0023;}
    else delete(1);
}

void english(int ch)
{
 if(insert_mode&&insert(1,' ')==NO)return;
 if(eofm>current&& !is_eol(current)) {
    *current = (char)ch;  user_task |= 0x0001;
    saved = NO; right();
 }
}

void up_down(int delta)
{
 char *destin;
 unsigned int line_no,size;
 if(delta <= 0) {
   delta = -delta;
   line_no=(delta>base_y+cury)?0:base_y+cury-delta;
   line_start=pre_ptr(line_start,delta);
 }
 else {
     line_no = base_y+cury;
     if(delta>max_line-line_no)delta=max_line-line_no;
     line_no += delta;
     line_start = next_ptr(line_start,delta);
 }
 size=width(line_start);
 destin = line_start +
		 ((base_x+curx>size)?size:base_x+curx);
 ver_locate(line_no,destin);
}

void del_eol(void)
{
  delete(width(current));
}






void ins_return( void )
{
   unsigned int pos =0;
   if( indent_mode ) {
      pos = strspn(line_start," ");
      if( pos >= base_x+curx) pos = base_x;
   }
   if( insert(pos+2,CR) ) {
      user_task |= 0x0019;
      memset(current+2 , ' ' , pos );
      to_pointer( current+pos+2 );
   }
}

char get_yes_no( char get_esc , char *msg)
{
   char key1,key2,ch;
   window *wd; int size;
   size = strlen( msg ) +1;
   wd  =  wopen( window_x ,  1, size ,  2);
   writef( window_x+1, window_x+size,2, REVERSE , msg);
   if( get_esc ) { key1 = key2 = ESC; }
   else { key1 = 'Y'; key2='N'; }
   do {
       ch =  toupper( get_key( window_x+size , 2) );
     }  while( ch  != key1 && ch != key2 );
     wclose(wd);
     return(ch);
}

void errorf( void )
{
   static char *errmsg[] = { "File is too Big",
			     "Search string not found",
			     "File exist or Illegal name"};
   get_yes_no(YES,
       (errno>=36)? errmsg[errno-36] : sys_errlist[errno]);
   errno = 0;
}

char first_key = 0xFF , keys[30] , object[30] , option[30] ,
	    replace_str[30] , SENSE =1 , f_count = 1,WORD= NO,
	    REPLACE=NO, LEN = 0;
int compare( char *st1)
{
  int count =1 , same = YES;
  char *st2 = object  ,*key = keys;
  while( count++ < LEN )
   if( (*++st1 - *++st2) & *++key ) { same = NO; break; }
   return (same);
}

int find( void )
{
    register char *pt = current;
    int found =0 , left, right , success ;
    unsigned int line = base_y+cury;

    gotoxy( 79 , HEADLIN );
    do {
       success = NO;
       while( *pt) {
	   if( *pt == LF ) line += SENSE;
	   else if( !((*pt - *object)&first_key ) && compare(pt) ) {
	   left = right = YES;
	   if( WORD ) { left = (*(pt-1) <= ' ' );
			right = (*(pt+LEN ) <= ' ') ; }
	   if( left&& right) { success = YES ; ++found; break; }
	   }  pt+=SENSE;
	  }  if( success == NO || found == f_count ) break;
	  pt+=SENSE;
	}while( YES );
	 success = (found== f_count);
	 if( success)
	  { line_start = pre_ptr( pt ,0 ); ver_locate( line , pt); }
	 else { errno = 37 ; errorf(); }
	 return (success);
}
int change_line( int mem_size , int buf_size , char *buf_ptr)
{
       int success;
       success = ( buf_size < mem_size ) ?
		  delete( (unsigned int) (mem_size-buf_size) ) :
		  insert( (unsigned int) (buf_size-mem_size) , ' ') ;
       if( success )  strncpy( current , buf_ptr , buf_size );
       return ( success );
}

void find_replace( void )
{
   #define  ASK     	1
   #define  NOT_ASK     2
   int repeat , key = 0 , replace_len , delta;

   if( strchr( option ,'N') ) REPLACE = NOT_ASK ;
   if( strchr( option , 'G'))
     {    SENSE = 1 ; f_count = 1; repeat = YES;
     to_pointer(memory); user_task = 0x0100; }
     else repeat = NO;
     replace_len = strlen( replace_str);
     while( find() ) {
	switch( REPLACE ) {
	   case ASK :
		  delta = LEN ;
		  writef(0,14,HEADLIN,BLINK_REVERSE,"Replace?(Y/N) ");
		  key = toupper( get_key( curx , cury+HEAD) ); mode();
		  if( key != 'Y' ) break;
	   case NOT_ASK :
		delta = replace_len;
		if(change_line( LEN, replace_len, replace_str )==NO)
			repeat = NO;
		else if(REPLACE	== ASK) user_f(YES);
	}
     if(kbhit()) key = 0x00f & bioskey(0);
     if(repeat==NO || key==ESC) break;
     to_pointer(current+delta);
   }
}

void find_string( int repeat_last_find )
{
  if( *object ) {
    if( SENSE == 1 ) {
       if( repeat_last_find)
		right();}
       else if( repeat_last_find || current == eofm ) left();
    if( REPLACE) find_replace();
    else find();
    }
}



void  find_condition(void)
{
   char *op; int i;
   window_x = 1;

   if( win_gets( NO , "Enter Find string ",object) != ESC )  {
      if( REPLACE & win_gets( NO ,
			"Enter replace string",replace_str) == ESC ) return;
       if( win_gets( NO,"Enter option ",option)!= ESC) {
	strupr( option );
	if( strchr( option ,'U') )
		{ first_key = 0xFF; strupr( object ); }
	else  first_key = 0xFF;
	LEN = ( char ) strlen( object );
	for ( i =0 ; i< LEN ; i++ )
		keys[i] = ( object[i] == 1) ? 0 :first_key;
	first_key = keys[0]; f_count = 1;
	for( op = option ; *op ; ++op)
		if( '1' <= *op && *op <='9')  f_count = *op -'0';
	SENSE= (strchr(option , 'B') ) ? -1 :1 ;
	WORD =  (strchr(option , 'W') ) ? YES :NO;
	find_string( NO);
    }
   }
}

int in_word( char *ch )
{
  return ( strchr( " \r\n<>,:.()[]^'*+-/$[",*ch) == NULL );
}

void word_left( void )
{
   char *pt =  current;
   while( memory<pt && !in_word( pt-1) ) --pt;
   while( memory<pt &&  in_word( pt-1) ) --pt;
   to_pointer(pt);
}

void word_right( void )
{
    char *pt = current;
   while( eofm>pt && in_word( pt) ) ++pt;
   while( eofm>pt && !in_word( pt) ) ++pt;
   to_pointer(pt);
}

int load_file( char *filename)
{
    int handle,success = YES;
    unsigned int hi,lo; char *end_byte;
    errno = 0 ;  user_task = 0x0100;

    if( ( handle =  open( filename , 0x8001 ) ) != -1) {
	    filelength(handle) ; lo = _AX; hi = _DX; }
    if( errno == 0 ) {
      if( hi || lo >= MAXCHAR - (unsigned int)(eofm-memory) )
	      errno = 36 ;
      else if(lo) {
	  movmem(current , current+lo, (unsigned int)(eofm-memory) +1 );
	  if( read( handle , current , lo ) == -1 )
	    movmem( current+lo , current , (unsigned int)(eofm-current) +1) ;
	  else {
	      end_byte = current + (lo -1);
	      if( strchr("\x1A\0",*end_byte) ) *end_byte = ' ' ;
	      eofm += lo ;
	      max_line += line_count( current , current+lo );
	      strncpy( line_buffer , line_start , 160 );
	  }
       }
    }
    if( handle != -1 ) _close( handle);
    if( errno ) { errorf() ; success = NO; }
    return ( success );
}

int file_or_dir_exist( char *fname)
{
   return (access( fname , 0 ) == 0 );
}

int save_file( char *start , char *finish , char *filename)
{
    int handle, success = YES ;
    char back_up_name[80];
    char *f_n_pt = filename , *b_n_pt = back_up_name;
    while( *f_n_pt && *f_n_pt !=  '.') *b_n_pt++ = *f_n_pt++;
    strcpy( b_n_pt , ".BAK");
    if( file_or_dir_exist( filename ) ) {
    if( file_or_dir_exist( back_up_name) ) {
	 if( get_yes_no(NO,"overwrite backup file ?(Y/N)") == 'N')
		 return NO;
	 unlink( back_up_name);
	 }
	if( strcmp( filename, back_up_name) )
	       rename( filename , back_up_name);
    }
   errno = 0;
   if( ( handle = creat(filename,0x0180) ) != -1 ) {
	 _write(handle,start, (unsigned int)(finish -start ) );
	 close(handle);
  }
  if( errno ) { errorf() ; success = NO; }
  return ( success);
}

void files( char *name)
{
     struct ffblk {
	  char ff_reserved[21],ff_attrib;
	  unsigned ff_ftime,ff_fdate;
	  long ff_fsize;
	  char ff_name[13];
     } ffblk ;
    int strcmp(), count , res , locate[] = {8,21,34,47,60,72};
    char drive[3],dir[66],file[9],ext[5],filename[200][13];
    window *wd;
    fnsplit( name , drive , dir , file , ext);
    strcat( name , ( ( strchr(name , '.') != NULL ) ? "*" : "*.*") );
    errno = 0;
    for( count = 199 ; count > 0 ; --count )  *filename[count] = 0;
    findfirst( name , &ffblk , 0 );
    while( errno == 0 && count < 200 ) {
	strcpy( filename[count++],ffblk.ff_name);
	findnext(&ffblk);
    }
    *name = 0;
    if( count > 0 ) {
       qsort( filename , count , 13 , strcmp) ;
       wd = wopen(7,2,65,14);
       res = get_response(filename , NO , 0 , 5, 13, locate , 3 , count);
       if( res != -1)
	 sprintf(name ,"%s%s%s",drive, dir , filename[res] );
       wclose(wd);
    }
    else  errorf();
}

void get_filename_to_read( char *name)
 {
   if( win_gets(YES , "Enter name to read",name) != ESC && !file_or_dir_exist(name) )
   files(name);
}

int get_filename_to_write( char *name)
 {
   return ( win_gets(YES , "Enter name to write",name) );
}
void block_decision( char **block_ptr)
{
  *block_ptr = current;
  block_size = (b_b_ptr >= b_k_ptr) ? 0 : (unsigned int)( b_k_ptr-b_b_ptr);
  block_display = YES;
  user_task = 0x0100;
}

void block_copy( void)
{
   if( !in_block( current ) && insert( block_size,' ') ) {
      max_line += block_line_count( b_b_ptr,b_k_ptr) ;
      movmem( b_b_ptr, current , block_size);
      b_b_ptr = current;
      b_k_ptr = b_b_ptr+ block_size;
      user_task = 0x0100;
   }
}

void line_after_block_move( void )
{
  char *homep;
  homep = pre_ptr( line_start, cury );
  if( homep <= b_b_ptr ) cury  -=  line_count( b_b_ptr , b_k_ptr );
  else if ( homep <= b_k_ptr ) {
     base_y  -= line_count( b_b_ptr , homep );
     cury = line_count( b_k_ptr , current);
  }
  else base_y -= line_count(b_b_ptr , b_k_ptr);
  saved = NO;
}

void block_move(void)
{
   char buffer[2001],*start, *mid , *endp;
   unsigned int count , len , size;

   if( in_block( current ) ) return ;
   if( b_k_ptr <= current ) {
     start =  b_b_ptr; mid = b_k_ptr; endp =current;
     line_after_block_move();
     b_b_ptr += (current - b_k_ptr); }
   else {
     start = current ;  mid = b_b_ptr ;  endp = b_k_ptr;
     b_b_ptr = current; }
   len = ( unsigned int)(endp - start );
   count = (unsigned int) (mid - start );
   while( count > 0) {
      size = ( count >= 2000 )? 2000 :count ;
      count -= size;
      strncpy( buffer , start , size );
      movmem( start + size , start , len - size );
      strncpy( endp - size , buffer , size );
   }

   b_k_ptr = b_b_ptr + block_size;
   line_start = pre_ptr( b_b_ptr, 0);
   hor_locate( ( unsigned int ) ( b_b_ptr - line_start ) );
   user_task = 0x0100 ; saved = NO;
}

void block_delete( void )
{
    char *temp;
    if( in_block( current ) ) to_pointer( b_b_ptr );
    else if( b_k_ptr  <= current ) {
	line_after_block_move();
	current -= block_size;
    }
    temp = current;
    current = b_b_ptr;
    delete( block_size );
    current = temp ;
    user_task = 0x0100;
    line_start = pre_ptr( current , 0 );
    hor_locate( ( unsigned int) ( current - line_start) );
}


void  block_write( void )
{
    window_x = 1;
    if( get_filename_to_write( block_name ) != ESC )
	      save_file( b_b_ptr , b_k_ptr , block_name);
}

void  block_read( void )
{
  char *temp = eofm;
  window_x = 7;
  get_filename_to_read( block_name );
  if( *block_name && load_file( block_name) ) {
     block_size = ( unsigned int ) (eofm - temp );
     b_b_ptr = current; b_k_ptr = b_b_ptr + block_size;
     block_display = YES ; saved = NO;
  }
}

void restore_line( void )
{
    char *pt = line_buffer ;
    unsigned int buf_size = 0;
    hor_locate(0);
    while( buf_size < 160 && *pt && !is_eol(pt) )
	  {  ++buf_size; ++pt;  }
    change_line( width(current ) , buf_size , line_buffer );
}

void ctrl_k( int key , int *done )
{
    if( strchr( "CVMWY",(char) key +'@' ) \
	    && ( block_size ==0 || block_display == NO) ) return;
    switch( key ) {
      case 2: block_decision( &b_b_ptr);
	      break;
      case 11: block_decision( &b_k_ptr );
	      break;
      case 8 : block_display = !block_display;
	       user_task = 0x0100;
	       break;
     case 3 : block_copy();
	       break;
     case 22 : block_move();
	       break;
     case 25 : block_delete();
		break;
     case 18 : block_read();
		break;
     case 23 : block_write();
	       break;
     case 4 : *done = YES ;
               break;
    }
}

void ctrl_q( int key )
{
       switch( key ) {
	case 2 : to_pointer(b_b_ptr);
		  break;
	case 11: to_pointer(b_k_ptr);
		  break;
	case 16: to_pointer(last_pos);
		  break;
	case 12:restore_line();
		  break;
	case 6: REPLACE = NO;
		find_condition();
		break;
	case 1: REPLACE = YES;
		find_condition();
		break;
	case 9: indent_mode = !(indent_mode);
		 break;
        case 25:del_eol();
     		break;
	}
}

void editor(void)
{
  int done = NO , key;
  do{
    key = get_key( curx , cury+HEAD);
    switch( key ) {
      case 11: ctrl_k( get_key2('K') , &done);
	       break;
      case 17: ctrl_q( get_key2('Q') );
	       break;
      case 16: key = get_key2('P');
		if( (key <' ' ) && key && \
		 (strchr("\r\n\x1A",(char) key )== NULL))
		      english(key);
	       break;
      case LEFT : left();
	       break;
      case RIGHT : right();
	       break;
      case 371: word_left();
	       break;
      case 372 : word_right();
        break;
      case UP: up_down( -1) ;
	       break;
      case DOWN: up_down(1);
	       break;
      case PGUP: up_down( -BOTTO_1);
		break;
      case PGDN: up_down( BOTTO_1);
		break;
      case HOME: hor_locate( 0 );
	       break;
      case END : hor_locate(width(line_start) ) ;
	       break;
      case 375 : up_down( -cury) ;
	       break;
      case 373 : up_down( BOTTOM- cury);
	       break;
      case 388 : to_pointer( memory) ;
	       break;
      case 374: to_pointer( eofm) ;
	       break;
      case 12 : find_string( YES );
	       break;
      case CR: if( insert_mode ) ins_return();
	       else { hor_locate( 0 ) ; up_down(1); }
	       break;
      case 14 : if( insert(2,CR) ) user_task |= 0x0015;
	       break;
      case 324 : done = 1;
        break;
      case INS: insert_mode = !insert_mode;
	       mode();
	       break;
      case BS: if( current == memory ) break;
	       left();
	       if( user_task > 1) user_f(YES);
	       if( !insert_mode )  break;
      case DEL: del();
	       break;
      case 25: hor_locate(0);
	       if( base_y+cury == max_line ) del_eol();
		else if( delete(width( current)+2 ) )
			   user_task |= 0x0022;
	       break;
      default: if( ' ' <= key && key < 256 ) english(key);
      }
      }while( !done );
}
void save(void)
{
   char t_name[80];
   strcpy( t_name , work_file);
   if( get_filename_to_write(t_name) !=ESC && save_file(memory,eofm,t_name) )
   {  strcpy( work_file, t_name); mode(); saved = YES; }
}
void load(void)
{
   char temp_name[80];
   strcpy(temp_name,work_file);
   get_filename_to_read( temp_name );
   if( *temp_name) {
      initial();
      if( load_file( temp_name) ) strcpy(work_file ,  temp_name);
      mode();
    }
}

int ask_saving( void )
{
  if( !saved ) {
     if( get_yes_no( NO, "Work file is not saved save?(Y/N)") == 'N' )  return (YES);
     save();
  }
  return (saved);
}
void new(void)
{
  char name[80];
  int handle;
  *name = 0;
  window_x =0 ;
  if( ask_saving() && win_gets( YES , "Enter New Filename",name) != ESC )  {
     if( !file_or_dir_exist( name ) && (handle = _creat(name , 0x20 )) != -1 )
     { close(handle); unlink(name); initial(); strcpy( work_file , name ); mode(); }
     else { errno = 38; errorf(); }
 }
}

void card( void )
{
   union REGS reg ;
   reg.h.ah = 15;
   int86( 16 , &reg ,&reg);
   if( reg.h.al == 7 ) {
      VRAM = (int far *)0xB0000000;
      BLOCK_COLOR= REVERSE ; reg.h.al = 2; }
   else {
    VRAM = (int far *)0xB8000000;
    BLOCK_COLOR = 0x4700; reg.h.al = 3; }
    reg.h.ah = 0;
    int86(16,&reg,&reg);
    mode();
    user_task = 0x0100;
}
void cls( void)
{
   scroll( 6 , 0,0,0,79,24 );
   gotoxy(0,0);
}

main()
{
   int work_func =0 , res,
       locate[] = {6,15,25,33,42,52,61,70,79};
   char path[80],*comspec ,
      entry[][13]={"Edit","Load","New","Dir","Chdir","O/S","Quit","Save"};
   if( (memory = (char *) malloc( MAXCHAR+2) ) == NULL ) exit(1);
   harderr(handler);
   *memory++ = 0;
   *replace_str = *object = *option = *block_name =0;
   initial();
   card();
   do {
      res = get_response( entry , YES , work_func , 8,1,locate,0,8);
      if( res  != -1) {

      work_func = res ;
      writef( locate[work_func],locate[work_func+1],0,REVERSE , entry[work_func] );

      switch( work_func) {
	 case 0 : window_x = 1;
		  if( *work_file == 0 ) load();
		  if( *work_file )  editor();
		  break;
	case 1:   window_x = 5;
		   if( ask_saving() )  load();
		   break;
	case 2:   new();
		  break;
	case 3: window_x = 18 ; *path = NULL;
		if( win_gets( YES , "Enter new dir mask",path) != ESC )
			 files(path);
		break;

	case 4:  getcwd(path,80);
		 window_x = 30;
		 if( win_gets( YES , "Enter new directory",path) != ESC )  {
			 files(path);

		if( path[1] == ':' && path[2] == '\\')
		       setdisk(path[0]-'A');
		if( chdir(path) == -1 ) errorf();
		}
		break;
	case 5: cls();
		  system("command.com");
		  errorf();
	 card();
	 break;
	 case 6: window_x = 43;
	 if( ask_saving())res = 8;
	 break;
	  case 7: window_x = 48;
		if( *work_file ) save();

	}
   }
 } while(res != 8);
 cls();
}