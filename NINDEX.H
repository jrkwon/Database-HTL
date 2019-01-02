/*
 *  Ìa·©   : nindex.h
 *  ¶w   : i¸â·¡ ¬‚·¥Ìa·©
 *  ‹¡ÒB   : nal.prj
 *  i¼a   : 93.03.16
 *   e—e·¡ : Ðe´·”ÐbŠa ¸å¸aÉ·¯¥‰·Ðb‰Á ÐeËi´aŸ¡ Š¥¸œb
 *  ´iŸ±‹i : ·¡ Î‰‹aŸ±· £»‹aŸ±·e ‰·ˆŸi ¶¥Ã¢·a¡ Ðs“¡”a.
 *           µá¡Ða¡e, a•¡ ”aŸe £»‹aŸ±· •¡¶‘·i  g·¡ ¤h´v·a“¡Œa¶a.
 *           ˜aœa¬á, ·¡ £»‹aŸ±·i ¤h·a¯¥ ¦…·e ”aŸe•A ¬a¶wÐa­a•¡ –A»¡ e
 *           ¬¡  e—e £»‹aŸ±•¡ ¸i ¸÷Ÿ¡Ða­a¬á ‰·ˆÐ º¯¡‹¡ ¤aœs“¡”a.
 */

#define MAXDATELEN   (15)
#define MAXTITLELEN  (35)

struct date_title_t {
  char date[MAXDATELEN];   /* 93‘e_05¶©_02·© */
  char yoil[3];            /* ¶a·©           */
  char nalsi[13];          /* i³¡           */
  char title[MAXTITLELEN]; /* ·©‹¡ ¹A¡¢      */
};

/*
 *  Ìa·© nindex.h · {
 */
