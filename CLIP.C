/* �a���a���� �� ���a */

int main_loop(void)
{
  int isquit = FALSE;

  prompt("* ���a���a : @I-��������  @S-�x��  @B-Ҏ�ᥡ��  @E-��á��  ");
  if(getitem(cur_key)) {             /* �a���a�a �eǡ �១ ���� */
    switch(getcurmenu()) {           /* F10���a �eǡ�a �����e   */
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

  inithantle(WHITE, BLUE, WHITE);    /* �e�i�a���a�១ ������ */
  initusermenu(WHITE, BLUE, WHITE);  /* �a�w�a �a���a ��� */
  if(init_data()) {
    load_config_file();
    intro_hantle();                  /* �e�i�a�� �e�e �� */
    do {
      isquit = main_loop();          /* �a���a���� �� ���� */
    } while(!isquit);
  }
  closehantle();                     /* �e�i�a���a�១ �{ */
}






