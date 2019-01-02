/*
 *  ·©¤e ¢…¬á Ìa·©·i µ¡¸bŠa¶w ¸ažaÌa·©¡ ¤aŽ…”a
 *
 *  ·©¤e¢…¬á Ìa·©· Š¹¡“e ”a·q‰Á ˆ{´a´¡Ðe”a.
 *
 *  1 12 20     <-- ˆb ¸aža¶a­¡· Ça‹¡Ÿi °áº…”a.
 *  N  H  H     <-- ˆb ¸aža¶a­¡· Ñw·i °áº…”a. ¸ažaÑwµA ”Ðe ¬é¡w·e ´aœµA..
 *  ·¡ ´aœµA“e ¯©¹A¸ažaŸi °áº…”a. ¸aža ¶a­¡ˆe· Š¦…·e ‰·¤‚·a¡ Ðe”a.
 */

/*
 *  Ñe¸ Ðe‹i œa·¡§aœáŸ¡µA  x•¡¢ ¸ažaÑw·i ”a¯¡ ¸÷Ÿ¡Ða¡e ”a·q‰Á ˆ{”a
 *
 *  H : ·³b Ò ´a¢œå ÀáŸ¡Ÿi ´g·e”a
 *  D : i¼a (·³b Ò i¼aˆa ‘e.¶©.·© ®…¬á¡ –A´á ·¶“e»¡ ˆñ¬a
 *  N : ®•¸a ·³b Ò µ¡Ÿe½¢ ¸÷i, ¶E½¢ ¦¦…µA“e ‰·¤‚·i À¶‘
 *  Z : ®•¸a ·³b Ò µ¡Ÿe½¢ ¸÷i, ¶E½¢ ¦¦…µA 0·i À¶‘
 *  M : ·³b¤h»¡ ´g‰¡ ‹a· ˆåášå”a...
 *  C : ®•¸a ·³b Ò µ¡Ÿe½¢ ¸÷i, ¶E½¢¦¦…µA ‰·¤‚ À¶‰¡, 3¸aŸ¡  a”a ®ñÎa(,) ¬s·³
 *      - ‹q´‚ Îa¯¡µA º¡ ³e”a. ¸aža Ça‹¡“e ®ñÎaŒa»¡ ‰¡aÐ´¡Ðe”a.
 *  G : ‹aŸ± Ìa·© ·³b, ¯aÄáa Ìa·©µA¬á ·³b¤h·q. Ða»¡ e ! ´a»¢ »¡¶¥ ´e–Q.
 */

void get_line(FILE *fp, buf)
{
  while( 1 ) {
    fgets(fp, buf);
    if(buf[0])
      break;
  }
}

void read_write(FILE *txt, FILE *data)
{
  unsigned char buf[560];

  get_line(txt, buf);

}

void main(int argc, char *argv[])
{
  char txtfile[20], datafile[20], *tmp;
  unsigned char buf[560];
  FILE *txt, *data;

  printf("+ Ojarkyo : Text File to Ojarkyo Data File Convertor\n");
  if(argc != 2) {
    printf("> Usage : txt2data <datafilename>\n");
    return;
  }
  strcpy(txtfile, argv[1]);
  tmp = strchr(txtfile, '.');
  if(tmp)
    tmp = NULL;
  sprintf(txtfile, "%s.txt", txtfile);
  sprintf(datafilem "%s.dat", txtfile);
  txt = fopen(txtfile, "rb");
  data = fopen(datafile, "wb");
  if(!txt) {
    printf("%s File not found.\n", txtfile);
    return;
  }
  if(!data) {
    printf("%s File not found.\n", datafile);
    return;
  }
  /*
   * ÑA”áµA Ð”wÐa“e Àõ¼, –‰¼º‰·i ·ª´á—i·¥”a.
   */
  get_line(txt, buf);
  /* 
  get_line(txt, buf);


  while(!feof(txt)) {
    read_write(txt, data);
  }

  printf("Complete...\n");
  fclose(txt);
  fclose(data);
}






















