/*
 * ���a�i �ş� �q��
 */

/* ���a�i���� ���e �����i �����a */

char *str_trim(char *str)
{
  char temp[80];
  register i, j;

  i = j = 0;
  while(str[j]) {
    if(str[j] == ' ') j++;
    else
      temp[i++] = str[j++];
  }
  temp[i] = '\0';
  strcpy(str, temp);
  return str;
}

/* ���a�i �E���A �����i len�� �e�q ���e�a.  */
char *ins_left_space(char *str, int len)
{
  char spc[80];
  register i = 0;

  while(len--)
    spc[i++] = ' ';
  spc[i] = '\0';
  strcat(spc, str);
  strcpy(str, spc);
  return str;
}

/* ���a�i �E���A '0'�i len�� �e�q ���e�a.  */
char *ins_left_zero(char *str, int len)
{
  char zero[80];
  register i = 0;

  while(len--)
    zero[i++] = '0';
  zero[i] = '\0';
  strcat(zero, str);
  strcpy(str, zero);
  return str;
}

/* ���a�i ���e���A �����i len�� �e�q ���e�a.  */
char *ins_right_space(char *str, int len)
{
  char spc[80];
  register i = 0;

  while(len--)
    spc[i++] = ' ';
  spc[i] = '\0';
  strcat(str, spc);
  return str;
}

