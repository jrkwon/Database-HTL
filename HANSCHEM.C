/* ------------------------- HTLSCHEM.C --------------------------- */

/*
 *  ¸ažaËi ¸÷·´á¡  e—e Ìa·©·i ¤åµbÐe”a.
 *  Àõ¤å¼ ·¥¸a“e ¸ažaËi ¸÷·´á¡ –E Ìa·©·i º…”a.
 *  –¤å¼ ·¥¸a“e ·¡ ¸÷·´á Ìa·©·i ¤åµbÐe Ò  e—i´áº‰ Ìa·©·¡Ÿq·¡”a.
 *   e´¢ ·¥¸aˆa ¸å”i–A»¡ ´g·a¡e ¯¡¸b¦¦…µA¬á ‹a Ìa·© ·¡Ÿq·i ¢ˆ“e”a.
 *  ¤åµb Ò  e—i´á»¡“e Ìa·©·¡Ÿq·e Ëb¥iÓ¡ »¡¸÷Ða»¡ ´g·a¡e ¸÷Ã¡¡
 *  ¸÷·´á Ìa·©·¡Ÿq·i ·¡¶wÐe”a.
 *  µŸi —i´á ¸ažaËi ¸÷· Ìa·©·¡Ÿq·¡ HTLDBMS.SCHœa¡e HTLDBMS.Cˆa
 *  ¸÷Ã¡ˆa –E”a.
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

#define  MAXCAT 3                              /* ÂA” ¹¡Ðs µi®A· ˆ•®  */
#define  NAMLEN 31                             /* ¸aža¶a­¡· ÂA” ‹©·¡   */
#define  MAXLINELEN  161                       /* Ðeº‰· ÂA” ‹©·¡ */
#define  iswhite(c)  ((c)==' '||(c)=='\t')     /* ‰·¤‚·¥ˆa? */
#define  REMARK      ';'                       /* ¬é¡w¢…    */

struct dict {                                  /* ¸aža¶a­¡ ¬a¸å· Š¹¡   */
  char dename[NAMLEN+1];                       /* µw¢… ¸aža¶a­¡ ·¡Ÿq     */
  char hdename[NAMLEN+1];                      /* Ðe‹i ¸aža¶a­¡ ·¡Ÿq     */
  char detype;                                 /* ¸aža¶a­¡Ñw             */
  int delen;                                   /* ¸aža¶a­¡ ‹©·¡          */
  char *demask;                                /* ÑÁ¡e Èi                */
} dc [MAXELE];

int dectr = 0;                                 /* ¶a­¡¬a¸å · ¶a­¡ ˆ•® */
int fctr = 0;                                  /* ¸ažaËi · Ìa·© ˆ•®   */
char filename[MAXFILS][NAMLEN+1];              /* Ìa·©·¡Ÿq ¢…¸aµi         */
int fileele[MAXFILS][MAXELE];                  /* Ìa·©· ¸aža¶a­¡         */
int ndxele[MAXFILS][MAXINDEX][MAXCAT];         /* ¬‚·¥ ¸aža¶a­¡           */

char wrd[NAMLEN+1];                           /* Ðe”e´áŸi ”q´a–‰ ¤áÌá    */
int lnctr = 0;                                 /* ·ª´á—i·¥ Ìa·©· º‰ ­A‹¡ */
char line[MAXLINELEN];                         /* ·ª´á—i·¥ Ðeº‰· ¤áÌá    */

enum {  INPUT, OUTPUT };

/* -------------------------- µ¡ŸA Îa¯¡ ¢…¸aµi -------------------------- */
char *errstr[] = {
  "µw¢… ¸aža¶a­¡ ·¡Ÿq·¡a Ìa·©·¡Ÿq·¡ Ëiv¯s“¡”a",                 /*   1  */
  "¸aža¶a­¡· ‹©·¡“e ®•¸a¡ –A´á´¡»¡¶a",                          /*   2  */
  "®ñÎaˆa ¨a¹vŠ…¶a",                                              /*   3  */
  "¸aža¶a­¡Ñw·¡ ¸i¡µ–A´ö´á¶a",                                    /*   4  */
  "˜aµ±Îaˆa ¨a¹vŠ…¶a",                                            /*   5  */
  "- #¸ažaËi - ·¡ ¨a¹v´á¶a",                                      /*   6  */
  "- #<»¡¯¡´á> -ˆa ·¶´á´¡–A»¡¶a",                                 /*   7  */
  "µA··! ´a»¢ Ìa·©{·¡¡e ´e–A¶a",                                 /*   8  */
  "ˆ{·e Ìa·© ·¡Ÿq·¡ ·¶´á¶a",                                      /*   9  */
  "¸aža¶a­¡ ¬a¸åµA ´ô“e ¸aža¶a­¡Ÿi °öŠ…¶a",                       /*  10  */
  "¸aža¶a­¡ ˆ•®ˆa á¢  g´a¶a",                                  /*  11  */
  "‹¡´â¸w­¡ˆa ¦¹¢ÐaŠ…¶a",                                        /*  12  */
  "Ìa·©»¡¸÷µA¬á »¡¸÷Ða»¡´g·e Ìa·©·¡Ÿq·¡µ¶a",                     /*  13  */
  "Ðe Ìa·©µA á¢  g·e ¬‚·¥·i »¡¸÷Ð–´á¶a",                        /*  14  */
  "Ðe ¬‚·¥µA á¢  g·e ¸aža¶a­¡Ÿi »¡¸÷Ð–Š…¶a",                    /*  15  */
  "¸aža¶a­¡ ¬a¸åµA ˆ{·e‰A ·¶´á¶a",                                /*  16  */
  "Ìa·©·¡ á¢  g´a¶a",                                           /*  17  */
  "»¡¯¡¸a ®…¬áˆa ¸i¡µ–A´ö´á¶a"                                    /*  18  */
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

char schfile[MAXFILE+MAXEXT-1];  /* ¸ažaËiŠ¹¡ ¸÷·´á Ìa·© ·¡Ÿq */
char dbffile[MAXFILE+MAXEXT-1];  /*  e—i´á»© Ìa·© ·¡Ÿq */
FILE *sch, *dbf;
char maintitle[] = " Hantle DataBase, Data Definition Language (DDL) Compiler Ver 1.0";

/* ----------- line¤áÌáµA¬á Ðe ”e´áŸi ·ª´á—i·¥”a ----------- */
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

/* ----- ¸ažaËi ¸÷·Ìa·©¡¦Èá Ðe º‰·i ·ª´á—i·¥”a ------ */
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

/* -------- ¸aža¶a­¡ ¬a¸å·i  e—e”a ---------- */
static void de_dict(void)
{
  char *cp, *cp1;
  int el;

  while (TRUE) {
    get_line();
    if (strncmp(line, "#¬a¸å{", 7) == 0)
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
      case 'A':  /* µw¢…, ®•¸a -- ÑÁ¡eÈi·¡ ·AÒa */
      case 'H':  /* Ðe‹i -- ·³b‹¡“w·e ¹½·aa ÑÁ¡eÈi·i ¢¯¡ */
      case 'Z':  /* ®•¸a, §¥Äe·e 0·a¡ À¶…”a. */
      case 'N':  /* ®•¸a, §¥Äe·e ‹a· ‰·¤‚·a¡ */
      case 'D':  /* i¼a -- ‘e.¶©.·© */
      case 'C':  /* 3¸aŸ¡ a”a ®ñÎa. ¸aŸ¡®ˆa  g·e ®Ã¡¸aža */
      case 'M':  /* Íe»³Ða»¡´g‰¡ ˆåášå”a */
      case 'G':  /* ‹aŸ± -- PCXÌa·© Ñw¯¢·a¡ ‹¡¥¥ µi®A· ·¡Ÿq·i ˆa»¡“e Ìa·©µA ¸á¸w */
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
      dc[dectr].delen = ((char)atoi(wrd) << 8);  /* ­A¡ Äe ® */
      expect_comma(&cp);
      cp = get_word(cp);
      numb_val();
      dc[dectr].delen |= (char)atoi(wrd);        /* ˆa¡ Äe ® */
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

/* ----------- Ìa·© ‰Åe ¸÷¥¡Ÿi ´è“e”a ---------------- */
static void files(void)
{
  int i, el = 0;

  if (fctr == MAXFILS)
    error(E2MANFILE);
  get_word(line + 6);  /* Ìa·© ·¡Ÿq·i ·ª´á—i·¥”a  */
  name_val();
  for (i = 0; i < fctr; i++)  /* ·¡£¡ Ði”w–A´á ·¶“e ·¡Ÿq·¥ˆa ˆñ¬a */
  if (strcmp(wrd, filename[i]) == 0)
    error(EDUPFILE);
  strcpy(filename[fctr], wrd);
  /* ---------- Ìa·©· ¸aža¶a­¡ --------- */
  while (TRUE) {
    get_line();
    if (strncmp(line, "#Ìa·©{", 7) == 0)
      break;
    if (el == MAXELE) {
      error(E2MANDELE);
      continue;
    }
    get_word(line);    /* ¸aža¶a­¡Ÿi ·ª´á—i·¥”a */
    for (i = 0; i < dectr; i++)  /* ¸aža¶a­¡ ¬a¸åµA ·¶“e»¡ ˆñ¬a */
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

/* ----------- ¬‚·¥‰Åe¦Ÿi  e—e”a ----------- */
static void keys(void)
{
  char *cp;
  int f, el, x, cat = 0;

  cp = get_word(line + 5);   /* Ìa·© ·¡Ÿq·i ·ª´á—i·¥”a */
  for (f = 0; f < fctr; f++)      /* ‹a ·¡Ÿq·¡ ¸ažaËi Š¹¡µA ·¶a ˆñ¬a */
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
    cp = get_word(cp);  /* ¬‚·¥ ¸aža¶a­¡Ÿi ·ª´á—i·¥”a. */
    for (el = 0; el < dectr; el++)  /* ¬‚·¥ ¶a­¡·¡Ÿq·¡ ¶a­¡ ¬a¸åµA ·¶a? */
      if (strcmp(wrd, dc[el].dename) == 0)
        break;
    if (el == dectr) {
      error(EUNKELE);
      break;
    }
    ndxele [f] [x] [cat++] = el + 1; /* ¬‚·¥µA ‹a ¶a­¡¤åÑ¡Ÿi Í¡Ðq¯¡Ç± */
    skip_white(&cp);
    if (*cp++ != ',')  /* ¹¡Ðs µi®A·¥ˆa? -- ®ñÎa¡ µe‰i */
      break;
    if (cat == MAXCAT) {
      error(E2MANIELE);
      break;
    }
  }
}

/* ---------------- Ìa·©¬—¬÷‰Á¸÷·i ÑÁ¡eµA Îa¯¡ --------------- */
static void disp_process(char *str)
{
  hsetreverse(ON);
  eputchs(-1, -hgetmaxy(), hgetmaxx(), ' ');
  hprintfxy(-1, -hgetmaxy(), " %s", str);
  hsetreverse(OFF);
}

/* ------------- #define ‰Á Š¹¡ÁAŸi Ìa·©µA °áº…”a -------------- */
static void defout(void)
{
  int f, el, fel;
  char name [NAMLEN+1];

  disp_process("Àõ¤å¼ ‰Á¸÷®Ð—º—... (#define¢…‰Á Š¹¡ÁAŸi Ìa·©µA ³e”a)");
  /* --------  µw¢… ¸aža¶a­¡ ¬a¸å ¸÷· --------- */
  for (el = 0; el < dectr; el++)
    fprintf(dbf, "\n#define %s %d", dc[el].dename, el + 1);
  fprintf(dbf, "\n");
  /* --------- Ìa·©·¡Ÿq ¸÷· -------- */
  for (f = 0; f < fctr; f++)
    fprintf(dbf, "\n#define %s %d", filename [f], f);
  fprintf(dbf, "\n");
  /* --------- ¸ažaŠ¹¡· Š¹¡ÁA ------------ */
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

/* ----- Ìa·©‰Á ¸aža¶a­¡ ¢…¸aµi·i Ìa·©µA °áº…”a. ---- */
static void strout(void)
{
  int el, f;

  disp_process("–¤å¼ ‰Á¸÷ ®Ð—º—... (Ìa·©·¡Ÿq· ¢…¸aµi‰Á ¸aža¶a­¡· ¢…¸aµi·i Ìa·©µA ³e”a");
  /*  --------  µw¢… ¸aža¶a­¡ ·¡Ÿq ---------  */
  fprintf(dbf, "\nchar *denames [] = {");
  for (el = 0; el < dectr; el++)
    fprintf(dbf, "\n  \"%s\",", dc[el].dename);
  fprintf(dbf, "\n  0\n};\n");
  /*  --------  Ðe‹i ¸aža¶a­¡ ·¡Ÿq  ---------  */
  fprintf(dbf, "\nchar *hdenames [] = {");
  for (el = 0; el < dectr; el++)
    fprintf(dbf, "\n  %s,", dc[el].hdename);
  fprintf(dbf, "\n  0\n};\n");
  /*  --------  ¸aža¶a­¡Ñw ------------  */
  fprintf(dbf, "\nchar eltype [] = \"");
  for (el = 0; el < dectr; el++)
    fprintf(dbf, "%c", dc[el].detype);
  fprintf(dbf, "\";\n");
  /*  --------  ¸aža¶a­¡ ÑÁ¡e Èi ---------  */
  fprintf(dbf, "\nchar *elmask [] = {");
  for (el = 0; el < dectr; el++)
    fprintf(dbf, (el < dectr-1 ? "\n  %s," : "\n  %s"),dc[el].demask);
  fprintf(dbf, "\n};\n");
  /* ------ Ìa·©·¡Ÿq ¢…¸aµi -------- */
  fprintf(dbf, "\nchar *dbfiles [] = {");
    for (f = 0; f < fctr; f++)
      fprintf(dbf, "\n  \"%s\",", filename [f]);
    fprintf(dbf, "\n  0\n};\n");
}

/* ---------- ¯aÇ¡ a ¶¥¯¡´å´áŸi ¸b¬÷ ---------- */
static void schout(void)
{
  int f, el, x, x1, cat, fel;
  char name [NAMLEN+1];

  disp_process("­A¤å¼ ‰Á¸÷ ®Ð—º—... (ˆb¹· ¸÷®¤µi)");
  /* ---------  ¸aža¶a­¡ ‹©·¡ ----------  */
  fprintf(dbf, "\n\nint ellen [] = {");
  for (el = 0; el < dectr; el++)  {
    if ((el % 25) == 0)
      fprintf(dbf, "\n  ");
    fprintf(dbf, (el < dectr-1 ? "%d," : "%d"),dc[el].delen);
  }
  fprintf(dbf, "\n};\n");
  /* ---------- Ìa·©µA —i´áˆi µw¢… ¸aža¶a­¡·¡Ÿq ------- */
  for (f = 0; f < fctr; f++) {
    lcase(name, filename [f]);
    fprintf(dbf, "\n\nint f_%s [] = {", name);
    el = 0;
    while ((fel = fileele[f] [el++]) != 0)
      fprintf(dbf, "\n  %s,", dc[fel-1].dename);
    fprintf(dbf, "\n  0\n};");
  }
  /* ------- Ìa·© ¡¢¢· Í¡·¥Èá ¤µi ------- */
  fprintf(dbf, "\n\nint *file_ele [] = {");
  for (f = 0; f < fctr; f++) {
    lcase(name, filename [f]);
    fprintf(dbf, "\n  f_%s,", name);
  }
  fprintf(dbf, "\n  0\n};\n");
  /* ----------- ·¥•B¯a ¤µi -------------- */
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

/* -------- ¢…¸aµi·i ­¡¢…¸a¡ ¡¡– ¤aŽ…”a ---------- */
static void lcase(char *dest, char *src)
{
  while (*src) {
    *dest = tolower(*src);
    dest++;
    src++;
  }
  *dest = '\0';
}

/* ----------------------- µ¡ŸA¢… Îa¯¡ --------------------- */
static void error(int n)
{
  static int erct = 0;

  hprintf(" µ¡ŸA %3dº‰: %s\n", lnctr, errstr[n-1]);

  if (erct++ == 5)        {
    erct = 0;
    if(yesno("‰­¢ÐiŒa¶a?", 0) != 1)
      depart(1);
  }
}

/* ---------- È“·¡a ‰·¤‚·i ˆåášå”a --------- */
static void skip_white(char **s)
{
  while (iswhite(**s))
    (*s)++;
}

/* ---------- µw¢… ¸aža¶a­¡·¡Ÿq·a¡ ¸â”wÐeˆa --------------- */
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

/* ------------- ¢…¸aµi·¡ ®•¸a¡ e –A´á·¶“e»¡ ˆñ¬a ------------- */
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

/* ------------- ‰·¤‚·e ˆåášá‰¡ ®ñÎaˆa ·¶“e»¡ ---------- */
static void expect_comma(char **cp)
{
  skip_white(cp);
  if (*(*cp)++ != ',')
    error(ECOMMIS);
}

/* --------- ¸ažaËi Š¹¡ Ïa¡‹aœ‘·i {…”a ------------- */
static void depart(int exit_code)
{
  int el;

  for (el = 0; el < dectr; el++)
    free(dc[el].demask);
  closehan();
  puts(maintitle);
  exit(exit_code);
}

/* ----------------- Á¡‹¡ÑÁ¡e·i ‹aŸ¥”a ---------------- */
static void openning_scrn(char *src, char *dest)
{
  char *title = " >> ÐeËi´aŸ¡ ¸aža¸÷·´á ¤åµb‹¡ << ";

  drawbox(1, 1, hgetmaxx(), 3, 0);
  drawbox(1, 4, hgetmaxx(), hgetmaxy()-4, 0);
  hputsxy(hgetmaxx()/2-strlen(title), 1, title);
  hprintfxy(1, 4, "¤åµbÐi Ìa·©: %s", strupr(src));
  hprintfxy(1, 5, "‰i‰Á   Ìa·©: %s", strupr(dest));
  disp_process("¯¡¸bÐaa¡e µEÈáŸi ’Ÿa­A¶a");
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
  if(strncmp(line, "#¸ažaËi ", 8))
    error(ESCHMIS);
  else {
    get_word(line + 8);
    fprintf(dbf, "\n/* ----------------------- %s ---------------------- */\n", wrd);
  }
  get_line();
  if(strncmp(line, "#¶a­¡¬a¸å", 9) == 0)
    de_dict();
  else
    error(EINVORDER);
  while(TRUE) {
    get_line();
    if(strncmp(line, "#Ìa·© ", 6) == 0)
      files();
    else if(strncmp(line, "#µi®A ", 6) == 0) {
      keys();
      break;
    }
    else {
      error(EINVORDER);
      getkey();
      disp_process("Î‰‹aŸ±·i { aÃ±“¡”a");
      getkey();
      depart(2);
    }
  }
  while(TRUE) {
    get_line();
    if(strncmp(line, "#µi®A ", 6) == 0)
      keys();
    else {
      if(strncmp(line, "#¸ažaËi{", 9) == 0)
        break;
      else
        error(EDIRMIS);
    }
  }
  defout();
  strout();
  schout();

  disp_process("¤åµb·¡ {v¯s“¡”a");
  getkey();
  depart(0);
}

