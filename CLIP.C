/* Ïa¡‹aœ‘· º žÏa */

int main_loop(void)
{
  int isquit = FALSE;

  prompt("* ºÀaŸ±Îa : @I-¬¡ý‹¡  @S-Àx‹¡  @B-ÒŽ´á¥¡‹¡  @E-‰¡Ã¡‹¡  ");
  if(getitem(cur_key)) {             /* ÀaŸ±Îaa ”eÂ‚Ç¡ ÀáŸ¡ ¦¦… */
    switch(getcurmenu()) {           /* F10·¡a ”eÂ‚Ç¡ˆa ’‰Ÿ¡¡e   */
      case  UTIL_MENU :
        isquit = util_menu();
        break;
      case  DATA_MENU :
        data_menu();
        break;
      case PRINT_MENU :
        print_menu();
        break;
      case CFG_MENU :
        config_menu();
        break;
    } /* switch */
  } /* if */
  else {
    return ask_quit();
  }
  if(cur_task != -1) {
    cur_key = shortcuts[cur_task+8];
    cur_task = -1;
  } /* if */
  return isquit;
}

void main(void)
{
  int isquit;

  inithantle(WHITE, BLUE, WHITE);    /* ÐeËiœa·¡§aœáŸ¡ Á¡‹¡ÑÁ */
  initusermenu(WHITE, BLUE, WHITE);  /* ¬a¶w¸a ÀaŸ±Îa ¬é¸÷ */
  if(init_data()) {
    load_config_file();
    intro_hantle();                  /* ÐeËi´aŸ¡  e—e ‰µ */
    do {
      isquit = main_loop();          /* Ïa¡‹aœ‘· º ¦¦… */
    } while(!isquit);
  }
  closehantle();                     /* ÐeËiœa·¡§aœáŸ¡ { */
}






