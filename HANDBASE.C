/*
 *  handbase.c
 *
 *  database manager
 */

/*
 *  ¸ažaŸi ¹¡¸bÐi ® ·¶“e ¸aža ¹¡¸b´á(DML; Data Management Language)
 *  Ÿi ¹A‰·Ð º…”a
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "handbms.h"
#include "hanerro2.h"
#include "ascii.h"
#include "extkey.h"

enum { OPEN_OK = 0, INDEX_OPEN_ERROR, FILE_OPEN_ERROR };  /* db_open_subÐq®· µ¡ŸA ¸÷¥¡ */

static int db_open_data(char *path, int *fl);
static int db_open_index(char *path, int *fl);
static int init_index(char *path, int fileno);
static void cls_index(int fileno);
static void del_indexes(int fileno, RPTR ad);
static int treeno(int fileno, int keyno);
static int relate_rcd(int fileno, char *buf);
static int data_in(char *c);
static int getrcd(int fileno, RPTR ad, char *buf);
static int rel_rcd(int fileno, RPTR ad, int *buf);

int db_opened = FALSE;	  /* ¸ažaËi·¡ µiv”a“e ¡Ÿ¡Îa  */
int curr_fd [MAXFILS];	  /* Ñe¸ Ìa·© ¤åÑ¡ */
RPTR curr_a [MAXFILS];	  /* Ñe¸ AÅ¡—a· Ìa·© º­¡   */
char *bfs [MAXFILS];		  /* Ìa·© ·³Â‰b ¤áÌá  */
int bfd [MAXFILS][MAXINDEX];
char dbpath [64];

/*
 * ¸ažaËi·i µe”a;
 * flµA ·¶“e Ìa·©¤åÑ¡(¯aÇ¡ aµA¬á ¸÷·–E)Ÿi ˆa»¥ Ìa·©·i µi”a
 * º´á»¥ ‰w¡µA ¸aža Ìa·©·¡ ¹¥¸Ð´¡ Ðq.
 */
void db_open(char *path, int *fl)
{
  char msg_index[] = "  ·¥•B¯a Ìa·©·i  e—i ® ´ô¯s“¡”a, —¡¯aÇa ‰·ˆe·¡ Â—¦…Ðe »¡ ¬iÍa¥¡­A¶a  ";
  char msg_file[] =  "  ¸aža Ìa·©·i  e—i ® ´ô¯s“¡”a, —¡¯aÇa ‰·ˆe·¡ Â—¦…Ðe»¡ ¬iÍa¥¡­A¶a  ";
  int ans;

  ans = db_open_data(path, fl);
  if(ans == FILE_OPEN_ERROR) {
    disp_cmsg(msg_file);
    exit(1);
  }
  ans = db_open_index(path, fl);
  if(ans == INDEX_OPEN_ERROR ) {
    db_cls();
    rebuild_index(0, TRUE);
    if(db_open_data(path, fl) != OPEN_OK) {    /* ”a¯¡ Ðe¤å µi´á¥¥”a   */
      disp_cmsg(msg_file);
      exit(1);               /* ·¡œ•¡ ´e–A”a“¡.. ”À‚´ô”a     */
    }
    if(db_open_index(path, fl) != OPEN_OK) {
      disp_cmsg(msg_index);
      exit(1);               /* ·¡œ•¡ ´e–A”a“¡.. ”À‚´ô”a     */
    }
  }
}

static int db_open_data(char *path, int *fl)
{
	char fnm [64];
	int i;

	if (!db_opened)	{
		for (i = 0; i < MAXFILS; i++)
			curr_fd [i] = -1;
		db_opened = TRUE;
	}
	strcpy(dbpath, path);
	while (*fl != -1)	{
		sprintf(fnm, "%s%.8s.dat", path, dbfiles [*fl]);
		curr_fd [*fl] = file_open(fnm);
    if(curr_fd [*fl] == DBERROR) {
      file_create(fnm, rlen(*fl));
  		curr_fd [*fl] = file_open(fnm);
      if(curr_fd [*fl] == DBERROR) {
        return FILE_OPEN_ERROR;  /* ·¡œý‰A –A¡e ­¥ —i´á´¡»¡... */
      }
    }
   	fl++;
	}
  return OPEN_OK;   /* ¬÷‰· !! */
}

static int db_open_index(char *path, int *fl)
{
	char fnm [64];

	strcpy(dbpath, path);
	while (*fl != -1)	{
		sprintf(fnm, "%s%.8s.dat", path, dbfiles [*fl]);
 		if(init_index(path, *fl)) {
  		if ((bfs [*fl] = (char *)malloc(rlen(*fl))) == NULL)	{
	  		errno = D_OM;
		  	dberror();
  		}
    }
    else {
      return INDEX_OPEN_ERROR;  /* ·¥•B¯a Ìa·© Á¡‹¡ÑÁ ¯©Ì */
    }
   	fl++;
	}
  return OPEN_OK;   /* ¬÷‰· !! */
}

/* ----------------- Ðe AÅ¡—aŸi Ìa·©µA ÂˆaÐe”a ----------------- */
int add_rcd(int f, char *bf)
{
	RPTR ad;
	extern RPTR new_record();
	int rtn;

	if ((rtn = relate_rcd(f, bf)) != DBERROR)	{
		ad = new_record(curr_fd [f], bf);
		if ((rtn = add_indexes(f, bf, ad)) == DBERROR)	{
			errno = D_DUPL;
			delete_record(curr_fd [f], ad);
		}
	}
	return rtn;
}

/* Ðe Ìa·©µA¬á ´á˜å AÅ¡—aŸi Àx“e”a.
 * Àx‰¡¸a Ða“e Ç¡(k; ºÇ¡-- 1, ”a·qÇ¡“e 2, 3 ...)¤åÑ¡ ˆa k·¡‰¡
 * Àx‰¡¸aÐa“e Ç¡ ˆt·e key·¥ AÅ¡—aŸi Ìa·©¤åÑ¡(f)·¥ Ìa·©µA¬á Àx´a¬á
 * bfµA ý´á •©aº…”a.
 */
int find_rcd(int f, int k, char *key, char *bf)
{
	RPTR ad;

	if ((ad = locate(treeno(f,k), key)) == 0)	{
		errno = D_NF;
		return DBERROR;
	}
	return getrcd(f, ad, bf);
}

/* find_rcdµÁ ˆ{·aa Àx·e ¸ažaŸi •©aº»¥ ´g‰¡ ·¶“e»¡ ´ô“e»¡ e ´i´a¥¥”a */
int verify_rcd(int f, int k, char *key)
{
	if (locate(treeno(f,k), key) == (RPTR) 0)	{
		errno = D_NF;
		return DBERROR;
	}
	return DBOK;
}

/* ------- find the first indexed record in a file -------- */
int first_rcd(f,k,bf)
int f;			/* file number			*/
int k;			/* key number			*/
char *bf;		/* buffer for record	*/
{
	RPTR ad;

	if ((ad = firstkey(treeno(f,k))) == 0)	{
		errno = D_EOF;
		return DBERROR;
	}
	return getrcd(f, ad, bf);
}

/* ------- find the last indexed record in a file -------- */
int last_rcd(f,k,bf)
int f;			/* file number			*/
int k;			/* key number			*/
char *bf;		/* buffer for record	*/
{
	RPTR ad;

	if ((ad = lastkey(treeno(f,k))) == 0)	{
		errno = D_BOF;
		return DBERROR;
	}
	return getrcd(f, ad, bf);
}

/* ------------- find the next record in a file ----------- */
int next_rcd(f,k,bf)
int f;			/* file number			*/
int k;			/* key number			*/
char *bf;		/* buffer for record	*/
{
	RPTR ad;

	if ((ad = nextkey(treeno(f, k))) == 0)	{
		errno = D_EOF;
		return DBERROR;
	}
	return getrcd(f, ad, bf);
}

/* -------- find the previous record in a file --------- */
int prev_rcd(f,k,bf)
int f;			/* file number			*/
int k;			/* key number			*/
char *bf;		/* buffer for record	*/
{
	RPTR ad;

	if ((ad = prevkey(treeno(f,k))) == 0)	{
		errno = D_BOF;
		return DBERROR;
	}
	return getrcd(f, ad, bf);
}

/* ----- return the current record to the data base ------- */
int rtn_rcd(f, bf)
int f;
char *bf;
{
	int rtn;

	if (curr_a [f] == 0)	{
		errno = D_PRIOR;
		return DBERROR;
	}
	if ((rtn = relate_rcd(f, bf)) != DBERROR)	{
		del_indexes(f, curr_a [f]);
		if ((rtn = add_indexes(f, bf, curr_a [f])) == DBOK)
			put_record(curr_fd [f], curr_a [f], bf);
		else
			errno = D_DUPL;
	}
	return rtn;
}

/* ------- delete the current record from the file -------- */
int del_rcd(f)
int f;
{
	if (curr_a [f])	{
		del_indexes(f, curr_a [f]);
		delete_record(curr_fd [f], curr_a [f]);
		curr_a [f] = 0;
		return DBOK;
	}
	errno = D_PRIOR;
	return DBERROR;
}

/* ---------- find the current record in a file ----------- */
int curr_rcd(f,k,bf)
int f;			/* file number			*/
int k;			/* key number			*/
char *bf;		/* buffer for record	*/
{
	RPTR ad;
	extern RPTR currkey();

	if ((ad = currkey(treeno(f,k))) == 0)	{
		errno = D_NF;
		return DBERROR;
	}
	getrcd(f, ad, bf);
	return DBOK;
}

/* --------- get the next physical sequential record from the file ------- */
int seqrcd(f, bf)
int f;
RPTR *bf;
{
	RPTR ad;
	int rtn;

	do	{
		ad = ++curr_a [f];
		if ((rtn = (rel_rcd(f,ad,bf)))==DBERROR && errno!=D_NF)
			break;
	}	while (errno == D_NF);
	return rtn;
}

/* ----------------- close the data base ------------------ */
void db_cls()
{
	int f;

	for (f = 0; f < MAXFILS; f++)
		if (curr_fd [f] != -1)	{
			file_close(curr_fd [f]);
			cls_index(f);
			free(bfs[f]);
			curr_fd [f] = -1;
		}
	db_opened = FALSE;
}

/* ----------- compute file record length ----------------- */
int rlen(f)
int f;
{
	return epos(0, file_ele [f]);
}

/* ---------- initialize a file record buffer ------------ */
void init_rcd(f, bf)
int f;			/* file number */
char *bf;		/* buffer */
{
	clrrcd(bf, file_ele[f]);
}

/* -------- set a generic record buffer to blanks --------- */
void clrrcd(bf, els)
char *bf;		/* buffer */
int *els;		/* data element list */
{
	int ln, i = 0, el;
	char *rb;

	while (*(els + i))	{
		el = *(els + i);
		rb = bf + epos(el, els);
		ln = ellen [el - 1];
		while (ln--)
			*rb++ = ' ';
		*rb = '\0';
		i++;
	}
}

/* ------- move data from one record to another ------- */
void rcd_fill(s, d, slist, dlist)
char *s;		/* source record buffer */
char *d;		/* destination record buffer */
int *slist;		/* source data element list */
int *dlist;		/* destination data element list */
{
	int *s1, *d1;

	s1 = slist;
	while (*s1)	{
		d1 = dlist;
		while (*d1)	{
			if (*s1 == *d1)
				strcpy(d+epos(*d1,dlist), s+epos(*s1,slist));
			d1++;
		}
		s1++;
	}
}

/* ------- compute relative position of a data element within a record -----*/
int epos(el, list)
int el;			/* element number */
int *list;		/* record element list */
{
	int len = 0;

	while (el != *list)
		len += ellen [(*list++)-1] + 1;
	return len;
}

/***************************************************************************
 *                       ·¥•B¯a ‰ÅŸ¡ Ðq®                                  *
 ***************************************************************************/

/*
 *  Ìa·©· ·¥•B¯a—i·i Á¡‹¡ÑÁ Ðe”a
 *  ¬÷‰·Ða¡e 1, ¯©ÌÐa¡e 0·i •©aº…”a.
 */
static int init_index(path, f)
char *path;		/* where the data base is */
int f;			/* file number */
{
	char xname [64];
	int x = 0;

	while (*(index_ele [f] + x))	{
		sprintf(xname, "%s%.8s.x%02d",path,dbfiles[f],x+1);
		if ((bfd [f] [x++] = btree_init(xname)) == DBERROR)	{
      errno = D_INDXC;
/*      dberror(); */
      return 0;
		}
	}
  return 1;
}

/*
 *  ¬‚·¥ Ìa·©·i ¥¢ŠÐe”a.
 *  allflagˆa Àq·¡¡e file_no“e ¢¯¡Ða‰¡ ¡¡—e ¬‚·¥ Ìa·©·i ¥¢ŠÐe”a.
 *  allflagˆa ˆá»µ·¡¡e file_no·a¡ »¡¸÷Ðe Ìa·© e ¬‚·¥ Ìa·©·i ¥¢ŠÐe”a.
 */
void rebuild_index(int file_no, int allflag)
{
  static int fs[MAXFILS+1];
  int f = 1, i;
  char *bf, msg[80];

  if(allflag) {
    for(f = 0; dbfiles[f]; f++)
      fs[f] = f;
  }
  else {
    *fs = file_no;
  }
  fs[f] = -1;  /* Ìa·© Ÿ¡¯aËa· { Îa¯¢ */
  for(i = 0; (f = fs[i]) != -1; i++)
    build_index("", f);  /* ‰w¡ »¡¸÷·¡ –A•¡¢.. */
  db_open("", fs);
  for(i = 0; (f = fs[i]) != -1; i++) {
    sprintf(msg, "    %s - ¬‚·¥ Ìa·©·i ¹¡¸÷Ða‰¡ ·¶¯s“¡”a    ", dbfiles[f]);
    disp_cmsg_sec(msg, 1);
    bf = (char *)malloc(rlen(f));
    while(seqrcd(f, bf) != DBERROR)
      add_indexes(f, bf, curr_a[f]);
    free(bf);
  }
  db_cls();
}

/* ---- build the indices for a file ---- */
void build_index(path, f)
char *path;
int f;			/* file number */
{
	char xname [64];
	int x = 0, x1;
	int len;

	while (*(index_ele [f] + x))	{
		sprintf(xname, "%s%.8s.x%02d", path, dbfiles[f], x+1);
		len = 0;
		x1 = 0;
		while (*(*(index_ele [f] + x) + x1))
			len += ellen [*(*(index_ele [f] + x) + (x1++))-1];
		build_b(xname, len);
		x++;
	}
}

/* ----- close the indices for a file ------ */
static void cls_index(f)
int f;
{
	int x = 0;

	while (*(index_ele [f] + x))	{
		if (bfd [f] [x] != DBERROR)
			btree_close(bfd [f] [x]);
		x++;
	}
}

/* ---- add index values from a record to the indices ---- */
int add_indexes(f, bf, ad)
int f;
char *bf;
RPTR ad;
{
	int x = 0;
	int i;
	char key[MAXKEYLEN];

	while (*(index_ele [f] + x))	{
		*key = '\0';
		i = 0;
		while(*(*(index_ele [f] + x) + i))
			strcat(key,
				bf +
			epos(*(*(index_ele[f]+x)+(i++)),file_ele [f]));
		if (insertkey(bfd [f] [x], key, ad, !x) == DBERROR)
  			return DBERROR;
		x++;
	}
  	return DBOK;
}

/* --- delete index values in a record from the indices --- */
static void del_indexes(f, ad)
int f;
RPTR ad;
{
	char *bf;
	int x = 0;
	int i;
	char key[MAXKEYLEN];

	if ((bf = (char *)malloc(rlen(f))) == NULL)	{
		errno = D_OM;
		dberror();
	}
	get_record(curr_fd [f], ad, bf);
	while (*(index_ele [f] + x))	{
		*key = '\0';
		i = 0;
		while (*(*(index_ele [f] + x) + i))
			strcat(key, bf + epos(*(*(index_ele[f]+x)+(i++)), file_ele [f]));
		deletekey(bfd [f] [x++], key, ad);
	}
	free(bf);
}

/* ---- compute tree number from file and key number ---- */
static int treeno(f, k)
int f, k;
{
	return bfd [f] [k - 1];
}

/* ---- validate the contents of a record where its file is
        related to another file in the data base ---------- */
static int relate_rcd(f, bf)
int f;
char *bf;
{
	int fx = 0, mx, *fp;
	static int ff[] = {0, -1};
	char *cp;

	while (dbfiles [fx])	{
		if (fx != f && *(*(index_ele [fx]) + 1) == 0)	{
			mx = *(*(index_ele [fx]));
			fp = file_ele [f];
			while (*fp)	{
				if (*fp == mx)	{
					cp = bf + epos(mx, file_ele [f]);
					if (data_in(cp))	{
						if (curr_fd[fx] == -1)	{
							*ff = fx;
							db_open(dbpath, ff);
						}
						if (verify_rcd(fx, 1, cp) == DBERROR)
							return DBERROR;
					}
					break;
				}
				fp++;
			}
		}
		fx++;
	}
	return DBOK;
}

/* ----- test a string for data. return TRUE if any ---- */
static int data_in(c)
char *c;
{
	while (*c == ' ')
		c++;
	return (*c != '\0');
}

/* ------------- get a record from a file -------------- */
static int getrcd(f, ad, bf)
int f;
RPTR ad;
char *bf;
{
	get_record(curr_fd [f], ad, bf);
	curr_a [f] = ad;
	return DBOK;
}

extern FHEADER fh [];

/* ----- find a record by relative record number ------ */
static int rel_rcd(f, ad, bf)
int f;			/* file number			*/
RPTR ad;
int *bf;
{
	errno = 0;
	if (ad >= fh [curr_fd [f]].next_record)	{
		errno = D_EOF;
		return DBERROR;
	}
	getrcd(f, ad, bf);
	if (*bf == -1)	{
		errno = D_NF;
		return DBERROR;
	}
	return DBOK;
}

/*
 * Convert a file name into its file token.
 * Return the token,
 * or DBERROR if the file name is not in the schema.
 */
int filename(fn)
char *fn;
{
	char fname[32];
	int f;
	void name_cvt();

	name_cvt(fname, fn);
	for (f = 0; dbfiles [f]; f++)
		if (strcmp(fname, dbfiles [f]) == 0)
			break;
	if (dbfiles [f] == 0)
		return DBERROR;
	return f;
}

/* ----------- convert a name to upper case ---------- */
void name_cvt(c2, c1)
char *c1, *c2;
{
	while (*c1)	{
		*c2 = toupper(*c1);
		c1++;
		c2++;
	}
	*c2 = '\0';
}

/* -------------------- data base error routine -------------------- */
void dberror(void)
{
	static char *ers [] = {
		"¸ažaˆa ´ô¯s“¡”a",                /* Record not found */
		"·¡¸åµA º…§¡–E ¸ažaˆa ´ô¯s“¡”a",  /* No prior record  */
		"Ìa·© {·³“¡”a",                  /* End of file */
		"Ìa·© ¯¡¸b·³“¡”a",                /* Beginning of file */
		"¸ažaˆa ·¡£¡ ¹¥¸Ðs“¡”a",         /* Record already exists */
		"‹¡´â¸w­¡ˆa ¦¹¢Ða‘A¶a",          /* Not enough memory */
		"·¥•B¯aµA ·¡¬w·¡ ·¶¯s“¡”a",       /* Index corrupted */
		"—¡¯aÇa ·³Â‰b ·¡¬w"              /* Disk i/o error */
	};
	static int fatal [] = { 0, 0, 0, 0, 0, 1, 0, 1 };

	disperror(ers [errno-1]);
	if(fatal [errno-1])
    exit(1);
}

void mov_mem(char *s, char *d, int l)
{
  if(d > s)
    while(l--)
      *(d+l) = *(s+l);
  else
    while(l--)
      *d++ = *s++;
}

void set_mem(char *s, int l, char n)
{
  while(l--)
    *s++ = n;
}
