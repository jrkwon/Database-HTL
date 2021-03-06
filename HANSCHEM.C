/* ------------------------- HTLSCHEM.C --------------------------- */

/*
 *  �a�a�i �����ᝡ �e�e �a���i ��b�e�a.
 *  ���弁 ���a�e �a�a�i �����ᝡ �E �a���i ���a.
 *  ���弁 ���a�e �� ������ �a���i ��b�e ҁ �e�i�ẉ �a�����q���a.
 *  �e�� ���a�a ��i�A�� �g�a�e ���b�����A�� �a �a�� ���q�i ���e�a.
 *  ��b ҁ �e�i�ỡ�e �a�����q�e �b�iӡ �����a�� �g�a�e ����á��
 *  ������ �a�����q�i ���w�e�a.
 *  ���i �i�� �a�a�i ���� �a�����q�� HTLDBMS.SCH�a�e HTLDBMS.C�a
 *  ����á�a �E�a.
 */

#include <stdio.h>
#include <stdlib.h>
#include <alloc.h>
#include <string.h>
#include <dir.h>
#include <dos.h>
#include <mem.h>
#include <ctype.h>
#include "hanlib.h"

#define  HANSCHEMA

#include "handbms.h"

#define  MAXCAT 3                              /* �A�� ���s �i�A�� ����  */
#define  NAMLEN 31                             /* �a�a�a���� �A�� ����   */
#define  MAXLINELEN  161                       /* �e���� �A�� ���� */
#define  iswhite(c)  ((c)==' '||(c)=='\t')     /* �������a? */
#define  REMARK      ';'                       /* ��w��    */

struct dict {                                  /* �a�a�a�� �a�巁 ����   */
  char dename[NAMLEN+1];                       /* �w�� �a�a�a�� ���q     */
  char hdename[NAMLEN+1];                      /* �e�i �a�a�a�� ���q     */
  char detype;                                 /* �a�a�a���w             */
  int delen;                                   /* �a�a�a�� ����          */
  char *demask;                                /* ���e �i                */
} dc [MAXELE];

int dectr = 0;                                 /* �a���a�� ���� �a�� ���� */
int fctr = 0;                                  /* �a�a�i ���� �a�� ����   */
char filename[MAXFILS][NAMLEN+1];              /* �a�����q ���a�i         */
int fileele[MAXFILS][MAXELE];                  /* �a���� �a�a�a��         */
int ndxele[MAXFILS][MAXINDEX][MAXCAT];         /* ���� �a�a�a��           */

char wrd[NAMLEN+1];                           /* �e�e��i �q�a�� ����    */
int lnctr = 0;                                 /* ����i�� �a���� �� �A�� */
char line[MAXLINELEN];                         /* ����i�� �e���� ����    */

enum {  INPUT, OUTPUT };

/* -------------------------- ���A �a�� ���a�i -------------------------- */
char *errstr[] = {
  "�w�� �a�a�a�� ���q���a �a�����q�� �i�v�s���a",                 /*   1  */
  "�a�a�a���� �����e ���a�� �A�ᴡ���a",                          /*   2  */
  "���a�a �a�v���a",                                              /*   3  */
  "�a�a�a���w�� �i���A����a",                                    /*   4  */
  "�a���a�a �a�v���a",                                            /*   5  */
  "- #�a�a�i - �� �a�v��a",                                      /*   6  */
  "- #<������> -�a ���ᴡ�A���a",                                 /*   7  */
  "�A��! �a�� �a���{���e �e�A�a",                                 /*   8  */
  "�{�e �a�� ���q�� ����a",                                      /*   9  */
  "�a�a�a�� �a��A ���e �a�a�a���i �����a",                       /*  10  */
  "�a�a�a�� �����a �ᢁ �g�a�a",                                  /*  11  */
  "����w���a �����a���a",                                        /*  12  */
  "�a�������A�� �����a���g�e �a�����q�����a",                     /*  13  */
  "�e �a���A �ᢁ �g�e �����i ����Ж��a",                        /*  14  */
  "�e �����A �ᢁ �g�e �a�a�a���i ����Ж���a",                    /*  15  */
  "�a�a�a�� �a��A �{�e�A ����a",                                /*  16  */
  "�a���� �ᢁ �g�a�a",                                           /*  17  */
  "�����a ����a �i���A����a"                                    /*  18  */
};

enum { EINVNAME = 1, EINVLEN, ECOMMIS, EINVTYPE, EQUOMIS, ESCHMIS, EDIRMIS,
       EUNEXPEOF, EDUPFILE, EUNKELE, E2MANDELE, EOUTMEM, EUNKFILE, E2MANIND,
       E2MANIELE, EDUPDELE, E2MANFILE, EINVORDER };

static char *get_word(char *cp);
static void get_line();
static void de_dict(void);
static void files(void);
static void keys(void);
static void disp_process(char *str);
static void defout(void);
static void strout(void);
static void schout(void);
static void lcase(char *dest, char *src);
static void error(int n);
static void skip_white(char **s);
static void name_val(void);
static void numb_val(void);
static void expect_comma(char **cp);
static void depart(int exit_code);
static void openning_scrn(char *src, char *dest);
static char *arrange_filename(char *fn, int mode);

char schfile[MAXFILE+MAXEXT-1];  /* �a�a�i���� ������ �a�� ���q */
char dbffile[MAXFILE+MAXEXT-1];  /* �e�i�ứ �a�� ���q */
FILE *sch, *dbf;
char maintitle[] = " Hantle DataBase, Data Definition Language (DDL) Compiler Ver 1.0";

/* ----------- line����A�� �e �e��i ����i���a ----------- */
static char *get_word(char *cp)
{
  int wl = 0, fst = 0;

  skip_white(&cp);
  while (*cp && *cp != '\n' && *cp != ',' && iswhite(*cp) == 0) {
    if (wl == NAMLEN && fst == 0)   {
      error(EINVNAME);
      fst++;
    }
    else
      wrd [wl++] = *cp++;
  }
  wrd [wl] = '\0';
  return cp;
}

/* ----- �a�a�i �����a�������� �e ���i ����i���a ------ */
static void get_line()
{
  *line = '\0';

  while (*line == '\0' || *line == REMARK || *line == '\n') {
    if (fgets(line, MAXLINELEN, sch) == 0) {
      error(EUNEXPEOF);
      depart(1);
    }
    lnctr++;
  }
}

/* -------- �a�a�a�� �a��i �e�e�a ---------- */
static void de_dict(void)
{
  char *cp, *cp1;
  int el;

  while (TRUE) {
    get_line();
    if (strncmp(line, "#�a��{", 7) == 0)
      break;
    if (dectr == MAXELE) {
      error(E2MANDELE);
      continue;
    }
    cp = get_word(line);
    name_val();
    for (el = 0; el < dectr; el++)
      if (strcmp(wrd, dc[el].dename) == 0)   {
        error(EDUPDELE);
        continue;
      }
    strcpy(dc[dectr].dename, wrd);
    expect_comma(&cp);
    skip_white(&cp);
    if (*cp != '"') {
      error(EQUOMIS);
      continue;
    }
    cp1 = cp + 1;
    while (*cp1 != '"' && *cp1 && *cp1 != '\n')
      cp1++;
    if (*cp1++ != '"') {
      error(EQUOMIS);
      continue;
    }
    *cp1 = '\0';
    strcpy(dc[dectr].hdename, cp);
    cp = cp1+1;
    skip_white(&cp);
    switch (toupper(*cp)) {
      case 'A':  /* �w��, ���a -- ���e�i�� �A�a */
      case 'H':  /* �e�i -- ���b���w�e ���a�a ���e�i�i ���� */
      case 'Z':  /* ���a, ���e�e 0�a�� �����a. */
      case 'N':  /* ���a, ���e�e �a�� �����a�� */
      case 'D':  /* �i�a -- �e.��.�� */
      case 'C':  /* 3�a���a�a ���a. �a�����a �g�e ��á�a�a */
      case 'M':  /* �e���a���g�� ����a */
      case 'G':  /* �a�� -- PCX�a�� �w���a�� ���� �i�A�� ���q�i �a���e �a���A ��w */
        break;
      default :
        error(EINVTYPE);
        continue;
    }
    dc[dectr].detype = toupper(*cp++);
    expect_comma(&cp);
    cp = get_word(cp);
    numb_val();
    if(dc[dectr].detype == 'G') {
      dc[dectr].delen = ((char)atoi(wrd) << 8);  /* �A�� �e �� */
      expect_comma(&cp);
      cp = get_word(cp);
      numb_val();
      dc[dectr].delen |= (char)atoi(wrd);        /* �a�� �e �� */
    }
    else
      dc[dectr].delen = atoi(wrd);
    expect_comma(&cp);
    skip_white(&cp);
    if (*cp != '"') {
      error(EQUOMIS);
      continue;
    }
    cp1 = cp + 1;
    while (*cp1 != '"' && *cp1 && *cp1 != '\n')
      cp1++;
    if (*cp1++ != '"') {
      error(EQUOMIS);
      continue;
    }
    *cp1 = '\0';
    if ((dc[dectr].demask = (char *)malloc((cp1-cp)+1)) == 0) {
      error(EOUTMEM);
      depart(1);
    }
    strcpy(dc[dectr].demask, cp);
    dectr++;
  }
}

/* ----------- �a�� �ŝe �����i ��e�a ---------------- */
static void files(void)
{
  int i, el = 0;

  if (fctr == MAXFILS)
    error(E2MANFILE);
  get_word(line + 6);  /* �a�� ���q�i ����i���a  */
  name_val();
  for (i = 0; i < fctr; i++)  /* ���� �i�w�A�� ���e ���q���a ��a */
  if (strcmp(wrd, filename[i]) == 0)
    error(EDUPFILE);
  strcpy(filename[fctr], wrd);
  /* ---------- �a���� �a�a�a�� --------- */
  while (TRUE) {
    get_line();
    if (strncmp(line, "#�a���{", 7) == 0)
      break;
    if (el == MAXELE) {
      error(E2MANDELE);
      continue;
    }
    get_word(line);    /* �a�a�a���i ����i���a */
    for (i = 0; i < dectr; i++)  /* �a�a�a�� �a��A ���e�� ��a */
      if (strcmp(wrd, dc[i].dename) == 0)
        break;
    if (i == dectr)
      error(EUNKELE);
    else if (fctr < MAXFILS)
      fileele [fctr] [el++] = i + 1; /* post to file  */
  } /* while */
  if (fctr < MAXFILS)
    fctr++;
}

/* ----------- �����ŝe���i �e�e�a ----------- */
static void keys(void)
{
  char *cp;
  int f, el, x, cat = 0;

  cp = get_word(line + 5);   /* �a�� ���q�i ����i���a */
  for (f = 0; f < fctr; f++)      /* �a ���q�� �a�a�i �����A ���a ��a */
    if (strcmp(wrd, filename[f]) == 0)
      break;
  if (f == fctr)  {
    error(EUNKFILE);
    return;
  }
  for (x = 0; x < MAXINDEX; x++)
    if (*ndxele [f] [x] == 0)
      break;
  if (x == MAXINDEX) {
    error(E2MANIND);
    return;
  }
  while (cat < MAXCAT) {
    cp = get_word(cp);  /* ���� �a�a�a���i ����i���a. */
    for (el = 0; el < dectr; el++)  /* ���� �a�����q�� �a�� �a��A ���a? */
      if (strcmp(wrd, dc[el].dename) == 0)
        break;
    if (el == dectr) {
      error(EUNKELE);
      break;
    }
    ndxele [f] [x] [cat++] = el + 1; /* �����A �a �a����ѡ�i ͡�q��Ǳ */
    skip_white(&cp);
    if (*cp++ != ',')  /* ���s �i�A���a? -- ���a�� �e�i */
      break;
    if (cat == MAXCAT) {
      error(E2MANIELE);
      break;
    }
  }
}

/* ---------------- �a�����������i ���e�A �a�� --------------- */
static void disp_process(char *str)
{
  hsetreverse(ON);
  eputchs(-1, -hgetmaxy(), hgetmaxx(), ' ');
  hprintfxy(-1, -hgetmaxy(), " %s", str);
  hsetreverse(OFF);
}

/* ------------- #define �� �����A�i �a���A �ẅ�a -------------- */
static void defout(void)
{
  int f, el, fel;
  char name [NAMLEN+1];

  disp_process("���弁 ������З��... (#define���� �����A�i �a���A �e�a)");
  /* --------  �w�� �a�a�a�� �a�� ���� --------- */
  for (el = 0; el < dectr; el++)
    fprintf(dbf, "\n#define %s %d", dc[el].dename, el + 1);
  fprintf(dbf, "\n");
  /* --------- �a�����q ���� -------- */
  for (f = 0; f < fctr; f++)
    fprintf(dbf, "\n#define %s %d", filename [f], f);
  fprintf(dbf, "\n");
  /* --------- �a�a������ �����A ------------ */
  for (f = 0; f < fctr; f++)      {
    lcase(name, filename [f]);
    fprintf(dbf, "\nstruct %s {", name);
    el = 0;
    while ((fel = fileele[f] [el++]) != 0)  {
      lcase(name, dc[fel-1].dename);
      fprintf(dbf, "\n  char %s [%d];", name, (dc[fel-1].detype == 'G') ? 2 : dc[fel-1].delen + 1);
    }
    fprintf(dbf, "\n};\n");
  }
}

/* ----- �a���� �a�a�a�� ���a�i�i �a���A �ẅ�a. ---- */
static void strout(void)
{
  int el, f;

  disp_process("���弁 ���� ��З��... (�a�����q�� ���a�i�� �a�a�a���� ���a�i�i �a���A �e�a");
  /*  --------  �w�� �a�a�a�� ���q ---------  */
  fprintf(dbf, "\nchar *denames [] = {");
  for (el = 0; el < dectr; el++)
    fprintf(dbf, "\n  \"%s\",", dc[el].dename);
  fprintf(dbf, "\n  0\n};\n");
  /*  --------  �e�i �a�a�a�� ���q  ---------  */
  fprintf(dbf, "\nchar *hdenames [] = {");
  for (el = 0; el < dectr; el++)
    fprintf(dbf, "\n  %s,", dc[el].hdename);
  fprintf(dbf, "\n  0\n};\n");
  /*  --------  �a�a�a���w ------------  */
  fprintf(dbf, "\nchar eltype [] = \"");
  for (el = 0; el < dectr; el++)
    fprintf(dbf, "%c", dc[el].detype);
  fprintf(dbf, "\";\n");
  /*  --------  �a�a�a�� ���e �i ---------  */
  fprintf(dbf, "\nchar *elmask [] = {");
  for (el = 0; el < dectr; el++)
    fprintf(dbf, (el < dectr-1 ? "\n  %s," : "\n  %s"),dc[el].demask);
  fprintf(dbf, "\n};\n");
  /* ------ �a�����q ���a�i -------- */
  fprintf(dbf, "\nchar *dbfiles [] = {");
    for (f = 0; f < fctr; f++)
      fprintf(dbf, "\n  \"%s\",", filename [f]);
    fprintf(dbf, "\n  0\n};\n");
}

/* ---------- �aǡ�a �������i �b�� ---------- */
static void schout(void)
{
  int f, el, x, x1, cat, fel;
  char name [NAMLEN+1];

  disp_process("�A�弁 ���� ��З��... (�b�� �������i)");
  /* ---------  �a�a�a�� ���� ----------  */
  fprintf(dbf, "\n\nint ellen [] = {");
  for (el = 0; el < dectr; el++)  {
    if ((el % 25) == 0)
      fprintf(dbf, "\n  ");
    fprintf(dbf, (el < dectr-1 ? "%d," : "%d"),dc[el].delen);
  }
  fprintf(dbf, "\n};\n");
  /* ---------- �a���A �i��i �w�� �a�a�a�����q ------- */
  for (f = 0; f < fctr; f++) {
    lcase(name, filename [f]);
    fprintf(dbf, "\n\nint f_%s [] = {", name);
    el = 0;
    while ((fel = fileele[f] [el++]) != 0)
      fprintf(dbf, "\n  %s,", dc[fel-1].dename);
    fprintf(dbf, "\n  0\n};");
  }
  /* ------- �a�� ������ ͡���� ���i ------- */
  fprintf(dbf, "\n\nint *file_ele [] = {");
  for (f = 0; f < fctr; f++) {
    lcase(name, filename [f]);
    fprintf(dbf, "\n  f_%s,", name);
  }
  fprintf(dbf, "\n  0\n};\n");
  /* ----------- ���B�a ���i -------------- */
  for (f = 0; f < fctr; f++) {
    lcase(name, filename [f]);
    for (x = 0; x < MAXINDEX; x++) {
      if (*ndxele [f] [x] == 0)
        break;
      fprintf(dbf, "\nint x%d_%s [] = {", x + 1, name);
      for (cat = 0; cat < MAXCAT; cat++)
        if (ndxele [f] [x] [cat])
      fprintf(dbf, "\n  %s,", dc[ndxele [f] [x] [cat] - 1].dename);
      fprintf(dbf, "\n  0\n};\n");
    }
    fprintf(dbf, "\nint *x_%s [] = {", name);
      for (x1 = 0; x1 < x; x1++)
        fprintf(dbf, "\n  x%d_%s,", x1 + 1, name);
      fprintf(dbf, "\n  0\n};\n");
  }
  fprintf(dbf, "\nint **index_ele [] = {");
  for (f = 0; f < fctr; f++) {
    lcase(name, filename [f]);
    fprintf(dbf, "\n  x_%s,", name);
  }
  fprintf(dbf, "\n  0\n};\n");
}

/* -------- ���a�i�i �����a�� ���� �a���a ---------- */
static void lcase(char *dest, char *src)
{
  while (*src) {
    *dest = tolower(*src);
    dest++;
    src++;
  }
  *dest = '\0';
}

/* ----------------------- ���A�� �a�� --------------------- */
static void error(int n)
{
  static int erct = 0;

  hprintf(" ���A %3d��: %s\n", lnctr, errstr[n-1]);

  if (erct++ == 5)        {
    erct = 0;
    if(yesno("�����i�a�a?", 0) != 1)
      depart(1);
  }
}

/* ---------- ȓ���a �����i ����a --------- */
static void skip_white(char **s)
{
  while (iswhite(**s))
    (*s)++;
}

/* ---------- �w�� �a�a�a�����q�a�� ��w�e�a --------------- */
static void name_val(void)
{
  char *s = wrd;
  if (isalpha(*s)) {
    while (isalpha(*s) || isdigit(*s) || *s == '_') {
      *s = toupper(*s);
      s++;
    }
    if (*s == '\0')
      return;
  }
  error(EINVNAME);
}

/* ------------- ���a�i�� ���a���e �A�᷶�e�� ��a ------------- */
static void numb_val(void)
{
  char *s = wrd;

  do {
    if (isdigit(*s++) == 0) {
      error(EINVLEN);
      break;
    }
  } while (*s);
}

/* ------------- �����e ���ቡ ���a�a ���e�� ---------- */
static void expect_comma(char **cp)
{
  skip_white(cp);
  if (*(*cp)++ != ',')
    error(ECOMMIS);
}

/* --------- �a�a�i ���� �a���a���i �{���a ------------- */
static void depart(int exit_code)
{
  int el;

  for (el = 0; el < dectr; el++)
    free(dc[el].demask);
  closehan();
  puts(maintitle);
  exit(exit_code);
}

/* ----------------- �������e�i �a���a ---------------- */
static void openning_scrn(char *src, char *dest)
{
  char *title = " >> �e�i�a�� �a�a������ ��b�� << ";

  drawbox(1, 1, hgetmaxx(), 3, 0);
  drawbox(1, 4, hgetmaxx(), hgetmaxy()-4, 0);
  hputsxy(hgetmaxx()/2-strlen(title), 1, title);
  hprintfxy(1, 4, "��b�i �a��: %s", strupr(src));
  hprintfxy(1, 5, "�i��   �a��: %s", strupr(dest));
  disp_process("���b�a�a�e �E��i ���a�A�a");
  getchext();
  hwindow(1, 5, -(hgetmaxx()-2), -(hgetmaxy()-6));
  hgotoxy(1, 1);
}

char *arrange_filename(char *fn, int mode)
{
  char drive[MAXDRIVE], dir[MAXDIR], file[MAXFILE], ext[MAXEXT];
  char *file2, *ext2, path[MAXPATH];

  strcpy(path, fn);
  fnsplit(path, drive, dir, file, ext);
  if(file[0] && !ext[0]) {
    file2 = file, ext2 = (mode == INPUT) ? ".SCH" : ".C";
    fnmerge(path, drive, dir, file2, ext2);
  }
  return path;
}

void main(int argc, char *argv[])
{
  if(argc != 3) {
    puts(maintitle);
    printf("Use this : C:\>SCHEMA  <source file>  <destination file>\n");
    printf("Example  : C:\>SCHEMA  HTLDBMS.SCH    HTLDBMS.C\n");
    exit(-1);
  }
  strcpy(schfile, arrange_filename(argv[1], INPUT));
  strcpy(dbffile, arrange_filename(argv[2], OUTPUT));
  sch = fopen(schfile, "r");
  if(!sch) {
    printf("File %s is not found\n", schfile);
    depart(1);
  }
  dbf = fopen(dbffile, "w");
  if(!dbf) {
    printf("File %s can't creat\n", dbffile);
    depart(2);
  }

  inithanlib(FIXRESMODE, HANDETECT, DEFHANFONT, DEFENGFONT);
  registeregrfont(EGR1);

  openning_scrn(schfile, dbffile);

  get_line();
  if(strncmp(line, "#�a�a�i ", 8))
    error(ESCHMIS);
  else {
    get_word(line + 8);
    fprintf(dbf, "\n/* ----------------------- %s ---------------------- */\n", wrd);
  }
  get_line();
  if(strncmp(line, "#�a���a��", 9) == 0)
    de_dict();
  else
    error(EINVORDER);
  while(TRUE) {
    get_line();
    if(strncmp(line, "#�a�� ", 6) == 0)
      files();
    else if(strncmp(line, "#�i�A ", 6) == 0) {
      keys();
      break;
    }
    else {
      error(EINVORDER);
      getkey();
      disp_process("Ή�a���i �{�añ���a");
      getkey();
      depart(2);
    }
  }
  while(TRUE) {
    get_line();
    if(strncmp(line, "#�i�A ", 6) == 0)
      keys();
    else {
      if(strncmp(line, "#�a�a�i�{", 9) == 0)
        break;
      else
        error(EDIRMIS);
    }
  }
  defout();
  strout();
  schout();

  disp_process("��b�� �{�v�s���a");
  getkey();
  depart(0);
}

