/*
 *      File HANFONT.H
 *      A Header for Hangul Font Library
 *      '90.7.25-8.3
 *      '91.4.22,26, 7.10, 11.16
 *      '92.2.5, 4.23, 5.13,22-23
 *      '93.5.6
 *      Written by Lim Y.K.
 */


#ifndef _HANFONT_H_
#define _HANFONT_H_

#ifndef _CHKVER_H_
    #include "chkver.h"
#endif

#ifndef _HANTYPES_H_
    #include "hantypes.h"
#endif


/** Macro constants ********************************************************/

/* Parameters to registerhanfont */

#define HAN10MD1  (&_han10md1)
#define HAN8MD1   (&_han8md1)
#define HAN8MD2   (&_han8md2)
#define HAN8GD1   (&_han8gd1)
#define HAN8GS1   (&_han8gs1)
#define HAN8GD2   (&_han8gd2)
#define HAN8GS2   (&_han8gs1)
#define HAN8GD3   (&_han8gd3)
#define HAN412GS1 (&_han412gs1)
#define HAN213GD1 (&_han213gd1)
#define HAN212MD1 (&_han212md1)
#define HAN212GD1 (&_han212gd1)
#define HAN113GD1 (&_han113gd1)
#define HAN121GD1 (&_han121gd1)
#define HANIYG    (&_haniyg)

/* Parameters to registerengfont */

#define ENGMD1 _ENGMD1_font
#define ENGMD2 _ENGMD2_font
#define ENGMD3 _ENGMD3_font
#define ENGMD4 _ENGMD4_font
#define ENGGD5 _ENGGD5_font
#define ENGMD6 _ENGMD6_font
#define ENGGS6 _ENGGS6_font
#define ENGIYG _ENGIYG_font

/* Parameters to registerauxfont */

#define ENGAUX1 _ENGAUX1_font
#define ENGAUX2 _ENGAUX2_font
#define ENGAUX3 _ENGAUX3_font
#define ENGAUX4 _ENGAUX4_font
#define ENGAUX5 _ENGAUX5_font
#define ENGAUX6 _ENGAUX6_font

/* Parameters to registerhgrfont */

#define HGR1   _HGR1_font
#define HGRIYG _HGRIYG_font

/* Parameters to registeregrfont */

#define EGR1   _EGR1_font
#define EGR2   _EGR2_font
#define EGR3   _EGR3_font
#define EGR4   _EGR4_font
#define EGR5   _EGR5_font
#define EGR6   _EGR6_font
#define EGRIYG _EGRIYG_font

/* Parameter to registerkssfont */

#define KSS1   _KSS1_font
#define KSSIYG _KSSIYG_font

/* Parameters to loadhanfont */

#define HINFO10X4X4 (&_hinfo10x4x4)
#define HINFO8X4X4  (&_hinfo8x4x4)
#define HINFO4X1X2  (&_hinfo4x1x2)
#define HINFO2X1X3  (&_hinfo2x1x3)
#define HINFO2X1X2  (&_hinfo2x1x2)
#define HINFO1X1X3  (&_hinfo1x1x3)
#define HINFO1X2X1  (&_hinfo1x2x1)

/* The number of characters */

#define ENGNUM  128
#define AUXNUM   94
#define HGRNUM  128
#define EGRNUM  128
#define KSSNUM ((unsigned)1128)

/* Byte size of a font */

#ifndef FS8X16
    #define FS8X16  16  /*  8x16 bitmap font */
    #define FS16X16 32  /* 16x16 bitmap font */
    #define FS12X24 36  /* 12x24 bitmap font */
    #define FS24X24 72  /* 24x24 bitmap font */
#endif

#ifndef INITIALNUM
    #define INITIALNUM 19
    #define MEDIALNUM  21
    #define FINALNUM   27
#endif

#ifndef MAXFONTINDEX
    #define MAXFONTINDEX 21
#endif


/** Type definition ********************************************************/

#ifndef _FONTTYPES
    #define _FONTTYPES

    typedef void far *linkedin_font_t;

    typedef const byte (*fontindex_t)[2][MAXFONTINDEX + 1];

    struct linkedin_hanfont_t {
        linkedin_font_t HAN1;
        linkedin_font_t HAN2;
        linkedin_font_t HAN3;
        fontindex_t fontindex;
    };

    typedef struct linkedin_hanfont_t *hanfont_t;
    typedef linkedin_font_t engfont_t;
    typedef linkedin_font_t auxfont_t;
    typedef linkedin_font_t hgrfont_t;
    typedef linkedin_font_t egrfont_t;
    typedef linkedin_font_t kssfont_t;
    typedef linkedin_font_t hanjafont_t;
#endif

struct haninfo_t {
    byte initial;
    byte medial;
    byte final;
    fontindex_t fontindex;
};


/** Function declarations **************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/* Register HAN/ENG/AUX/HGR/EGR/KSS fonts respectively */

void registerhanfont(hanfont_t hanfont);
void registerengfont(engfont_t engfont);
void registerauxfont(auxfont_t auxfont);
void registerhgrfont(hgrfont_t hgrfont);
void registeregrfont(egrfont_t egrfont);
void registerkssfont(kssfont_t kssfont);

/* Get HAN/ENG/AUX/HGR/EGR fonts respectively */

hanfont_t gethanfont(void);
engfont_t getengfont(void);
auxfont_t getauxfont(void);
hgrfont_t gethgrfont(void);
egrfont_t getegrfont(void);
kssfont_t getkssfont(void);

/* Store/restore HAN/ENG/AUX/HGR/EGR fonts */

int storehanfont(void);
int storeengfont(void);
int storeauxfont(void);
int storehgrfont(void);
int storeegrfont(void);
int storekssfont(void);
int restorehanfont(void);
int restoreengfont(void);
int restoreauxfont(void);
int restorehgrfont(void);
int restoreegrfont(void);
int restorekssfont(void);

/* Load a font from a file */

int loadhanfont(int id, char *hanfont, struct haninfo_t *hinfo);
int loadengfont(int id, char *engfont);
int loadauxfont(int id, char *engfont);
int loadhgrfont(int id, char *hgrfont);
int loadegrfont(int id, char *egrfont);
int loadkssfont(int id, char *kssfont);
int loadhanjafont(char *hanjafont);

/* Change the current font to new one */

int changehanfont(int id);
int changeengfont(int id);
int changeauxfont(int id);
int changehgrfont(int id);
int changeegrfont(int id);
int changekssfont(int id);

/* Release the memory used by font id */

void releasehanfont(int id);
void releaseengfont(int id);
void releaseauxfont(int id);
void releasehgrfont(int id);
void releaseegrfont(int id);
void releaskssrfont(int id);

#ifdef __cplusplus
}
#endif


/** External variables *****************************************************/

/* The maximum number of fonts to be loaded                               */
/* This variable must be set before first calling of load...font function */
/* The default are 8, 8, 4, 4 and 4, 4, respectively                      */

extern int _maxhanfont;
extern int _maxengfont;
extern int _maxauxfont;
extern int _maxhgrfont;
extern int _maxegrfont;
extern int _maxkssfont;

/* If false, the size of font files are not checked                   */
/* Otherwise true, size of font files are always checked when loading */
/* The default is true                                                */

extern bool _checkfontsize;

/* _maxstore...font defines the stack size,                   */
/* ie. the maximum nest level for calling store...font        */
/* This variable must be set                                  */
/* before first calling of store...font function respectively */
/* The defaults are 8, 8, 4, 4, 4 and 4, respectively         */
/* _...stackptr is current stack pointer, ie. the nest level  */

extern int _maxstorehanfont;
extern int _maxstoreengfont;
extern int _maxstoreauxfont;
extern int _maxstorehgrfont;
extern int _maxstoreegrfont;
extern int _maxstorekssfont;
extern int _hanstackptr;
extern int _engstackptr;
extern int _auxstackptr;
extern int _hgrstackptr;
extern int _egrstackptr;
extern int _kssstackptr;

/* Informations on Hangul fonts */

extern struct haninfo_t _hinfo10x4x4;
extern struct haninfo_t _hinfo8x4x4;
extern struct haninfo_t _hinfo4x1x2;
extern struct haninfo_t _hinfo2x1x3;
extern struct haninfo_t _hinfo2x1x2;
extern struct haninfo_t _hinfo1x1x3;
extern struct haninfo_t _hinfo1x2x1;

/* Font index tables */

extern const byte _fontindex10x4x4[3][2][MAXFONTINDEX + 1];  /* Total 18 set */
extern const byte _fontindex8x4x4[3][2][MAXFONTINDEX + 1];   /* Total 16 set */
extern const byte _fontindex4x1x2[3][2][MAXFONTINDEX + 1];   /* Total  7 set */
extern const byte _fontindex2x1x3[3][2][MAXFONTINDEX + 1];   /* Total  6 set */
extern const byte _fontindex2x1x2[3][2][MAXFONTINDEX + 1];   /* Total  5 set */
extern const byte _fontindex1x1x3[3][2][MAXFONTINDEX + 1];   /* Total  5 set */
extern const byte _fontindex1x2x1[3][2][MAXFONTINDEX + 1];   /* Total  4 set */

/* Information structures on linked-in Hangul fonts */

extern struct linkedin_hanfont_t _han10md1;
extern struct linkedin_hanfont_t _han8md1;
extern struct linkedin_hanfont_t _han8md2;
extern struct linkedin_hanfont_t _han8gd1;
extern struct linkedin_hanfont_t _han8gs1;
extern struct linkedin_hanfont_t _han8gd2;
extern struct linkedin_hanfont_t _han8gs2;
extern struct linkedin_hanfont_t _han8gd3;
extern struct linkedin_hanfont_t _han412gs1;
extern struct linkedin_hanfont_t _han213gd1;
extern struct linkedin_hanfont_t _han212md1;
extern struct linkedin_hanfont_t _han212gd1;
extern struct linkedin_hanfont_t _han113gd1;
extern struct linkedin_hanfont_t _han121gd1;
extern struct linkedin_hanfont_t _haniyg;

/* Linked-in Hangul fonts */

extern byte far _HAN10MD1_font[382][FS16X16];   /* 12224 byte */
extern byte far _HAN8MD1_font[344][FS16X16];    /* 11008 byte */
extern byte far _HAN8MD2_font[344][FS16X16];
extern byte far _HAN8GD1_font[344][FS16X16];
extern byte far _HAN8GS1_font[344][FS16X16];
extern byte far _HAN8GD2_font[344][FS16X16];
extern byte far _HAN8GS2_font[344][FS16X16];
extern byte far _HAN8GD3_font[344][FS16X16];
extern byte far _HANIYG_font[344][FS16X16];
extern byte far _HAN412GS1_font[151][FS16X16];  /* 4832 byte */
extern byte far _HAN213GD1_font[140][FS16X16];  /* 4480 byte */
extern byte far _HAN212MD1_font[113][FS16X16];  /* 3616 byte */
extern byte far _HAN212GD1_font[113][FS16X16];
extern byte far _HAN113GD1_font[121][FS16X16];  /* 3872 byte */
extern byte far _HAN121GD1_font[88][FS16X16];   /* 2816 byte */

/* Linked-in English fonts */

extern byte far _ENGMD1_font[ENGNUM][FS8X16];  /* 2048 byte */
extern byte far _ENGMD2_font[ENGNUM][FS8X16];
extern byte far _ENGMD3_font[ENGNUM][FS8X16];
extern byte far _ENGMD4_font[ENGNUM][FS8X16];
extern byte far _ENGGD5_font[ENGNUM][FS8X16];
extern byte far _ENGMD6_font[ENGNUM][FS8X16];
extern byte far _ENGGS6_font[ENGNUM][FS8X16];
extern byte far _ENGIYG_font[ENGNUM][FS8X16];

/* Linked-in English auxiliary fonts */

extern byte far _ENGAUX1_font[AUXNUM][FS8X16];  /* 1504 byte */
extern byte far _ENGAUX2_font[AUXNUM][FS8X16];
extern byte far _ENGAUX3_font[AUXNUM][FS8X16];
extern byte far _ENGAUX4_font[AUXNUM][FS8X16];
extern byte far _ENGAUX5_font[AUXNUM][FS8X16];
extern byte far _ENGAUX6_font[AUXNUM][FS8X16];

/* Linked-in Hangul graphics character fonts */

extern byte far _HGR1_font[HGRNUM][FS16X16];  /* 4096 byte */
extern byte far _HGRIYG_font[HGRNUM][FS16X16];

/* Linked-in English graphics character fonts */

extern byte far _EGR1_font[EGRNUM][FS8X16];  /* 2048 byte */
extern byte far _EGR2_font[EGRNUM][FS8X16];
extern byte far _EGR3_font[EGRNUM][FS8X16];
extern byte far _EGR4_font[EGRNUM][FS8X16];
extern byte far _EGR5_font[EGRNUM][FS8X16];
extern byte far _EGR6_font[EGRNUM][FS8X16];
extern byte far _EGRIYG_font[EGRNUM][FS8X16];

/* Linked-in KS special character fonts (KS C 5601-1987) */

extern byte far _KSS1_font[KSSNUM][FS16X16];  /* 36096 byte */
extern byte far _KSSIYG_font[KSSNUM][FS16X16];

#endif
