unsigned getlnkey( void)
{
	int key, lo, hi;

	key = bioskey( 0);
	lo = key & 0x00ff;
	hi = key >> 8;
	return(( lo ? lo : hi + 256));
}

#define  put_crs( i)   putimage( x + (i<<3), y, crs, XOR_PUT)
#define  clr_line()    clrblk( (x>>3)+i, (y>>2), len-i+1, 4, 0)
#define  clr_char()    clrblk( (x>>3)+len-1, (y>>2), 1, 4, 0)
#define  out_line()    heout( x + (i<<3), y, ( tp + i))
#define  move_crs()    put_crs( it);  put_crs( i)

char *get_string( int x, int y, int limit, int pos, char first, char *str)
{
    int i, it, len;
    unsigned ch;
    char *tp;
    static int crs[] = { 0x0007, 0x000f, 0x0000, 0x0000, 0x0000,
			 0x0000, 0x0000, 0x0000, 0x0000, 0xffff };

    tp = ( char *) farmalloc( limit--);
    strcpy( tp, str);
    i = it = pos;
    len = strlen( tp);
    heout( x, y, tp);
    if ( first)   view_inverse( x, y, len);
    put_crs( i);
    ch = getlnkey();
    if( isprint( ch) && first)   *tp = i = NULL;
    if ( first)   view_inverse( x, y, len);
    if ( !i && len && first)
      {
	  clr_line();
	  move_crs();
	  len = NULL;
      }
    while( ch != ESC && ch != CR)
	  {
	      switch( ch)
		{
		    case BS    :  if ( i)
				    {
					movmem( &tp[i], &tp[i-1], len-i+1);
					i--;
					put_crs( it);
					out_line();
					clr_char();
					len--;
					put_crs( i);
				    }   break;
		    case DEL   :  if ( i < len)
				    {
					movmem( &tp[i+1], &tp[i], len-i);
					put_crs( i);
					out_line();
					clr_char();
					len--;
					put_crs( i);
				    }   break;
		    case HOME  :  i = NULL;    move_crs();  break;
		    case END   :  i = len;     move_crs();  break;
		    case LEFT  :  if ( i)  {  i--;  move_crs();  }
				  break;
		    case RIGHT :  if ( i < len)  {  i++;  move_crs();  }
				  break;
		    case CTRL_Y:  *tp = i = NULL;   clr_line();
				  len = NULL;   put_crs( i);
				  break;
		    default    :  if ( isprint( ch) && len < limit)
				    {
					movmem( &tp[i], &tp[i+1], len-i+1);
					put_crs( it);
					*( tp + i) = ch;
					out_line();
					i++;   len++;
					put_crs( i);
				    }
		}
	      it = i;
	      ch = getlnkey();
	  }
    put_crs( i);
    if ( ch == ESC)
      {
	  farfree( tp);
	  return( NULL);
      }
    return( strcpy( str, tp));
}

get_filename_to_read(  char *name )
{
	void  *wptr;
	char  *tptr;

	wptr = hwopen( 17, 13, 32, 11 );
	heout( 168,54, " ·ª´á—i·© ÑÁ·©·¡Ÿq·e");
	line(137,72,390,72);
 tptr = get_string( 144, 76, 30, strlen(name), 1, name);
	strupr( name);
	hwclose( 17, 13, wptr );
	return ( (tptr == NULL ) ? ESC : YES );
}

get_filename_to_write(  char *name )
{
	void far *wptr;
	char *tptr;

	wptr = hwopen( 17, 21, 32, 11 );
	heout( 168,86, "  ¸á¸wÐi  ÑÁ·©·¡Ÿq·e ");
	line(137,104,390,104);
 tptr = get_string( 144,108, 30, strlen(name), 1, name);
	strupr( name);
	hwclose( 17, 21, wptr );
	return ( ( tptr == NULL ) ? ESC : YES );
}
