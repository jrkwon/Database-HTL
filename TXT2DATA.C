/*
 *  ���e ���� �a���i ���b�a�w �a�a�a���� �a���a
 *
 *  ���e���� �a���� �����e �a�q�� �{�a���e�a.
 *
 *  1 12 20     <-- �b �a�a�a���� �a���i �ẅ�a.
 *  N  H  H     <-- �b �a�a�a���� �w�i �ẅ�a. �a�a�w�A ���e ��w�e �a���A..
 *  �� �a���A�e ���A�a�a�i �ẅ�a. �a�a �a���e�� �����e �����a�� �e�a.
 */

/*
 *  �e�� �e�i �a���a�១�A �x���� �a�a�w�i �a�� �����a�e �a�q�� �{�a
 *
 *  H : ���b ҁ �a���� �១�i �g�e�a
 *  D : �i�a (���b ҁ �i�a�a �e.��.�� ���ᝡ �A�� ���e�� ��a
 *  N : ���a ���b ҁ ���e�� ���i, �E�� �����A�e �����i ����
 *  Z : ���a ���b ҁ ���e�� ���i, �E�� �����A 0�i ����
 *  M : ���b�h�� �g�� �a�� ����a...
 *  C : ���a ���b ҁ ���e�� ���i, �E�������A ���� ������, 3�a�� �a�a ���a(,) �s��
 *      - �q�� �a���A ���� �e�a. �a�a �a���e ���a�a�� ���aЁ���e�a.
 *  G : �a�� �a�� ���b, �aā��a �a���A�� ���b�h�q. �a���e ! �a�� ���� �e�Q.
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
   * �A��A Ё�w�a�e ����, �������i ����i���a.
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






















