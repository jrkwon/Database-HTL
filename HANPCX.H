typedef struct {
  char manuf;     /* �w�w 10 (0x0a) */
  char ver;       /*                */
  char encod;     /* RLE ���e 1     */
  char bitpx;     /* �e��i �a���a�e ���a �� */
  int  x1;        /* �a���� ���a    */
  int  y1;
  int  x2;
  int  y2;
  int  hres;      /* Ё�w��         */
  int  vres;
  char palette[48];
  char reserved;     /* �a���i �h�e ������ ���a ? */
  char nplanes;   /* �a�w�E �i�A���� */
  int  bplin;     /* �e�a���w �a���a �� */
  int  palinfo;   /* 1=COLOR, 0=GREY */
  int  shres;     /* �aā�� Ё�w��?*/
  int  svres;
  char xtra[54];  /* �a�w �g�q */
} PCXHDR;    /* PCX �w�� ������ �A�� */

bool view_pcx_file(int x, int y, char *file_name, int fcolor, int bcolor);
