#include <string.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>

long ftell();
void display_zip(char *giho);	/*���� �q���w void�w �q�� */

main()
{
   char giho[9];
   char key;
   int i;
   clrscr();

   while(1){
	   strcpy(giho,"");      /*�������i ���a ���ᴡ �e�a */
	   gotoxy(10,10);
	   printf("��ѡ�i ���b�a�A�a==>");
	   gets(giho);		 /*���e��ѡ�i ���b �h�e�a*/
	   display_zip(giho);	 /*�����q���a�a �����i �a���a�e �q��ѡ*/

	   gotoxy(10,14);
	   printf("�� �i�a�a?[Y/anykey]");

	   key=getch();
	     switch(key){
		case 'y':
		case 'Y':break;
		default :exit(1);
		}
	   }

    }

void display_zip(char *giho)
{
   FILE *fp;
   long rec_size,start,end,middle;
   char search_giho[9];

   char si[9],gu[9],dong[30];

   strcpy(search_giho,"");
   strcpy(si,"");
   strcpy(gu,"");
   strcpy(dong,"");

   if((fp=fopen("zip.dat","r"))==NULL)exit(1);

   fseek(fp,0,2);    /*������ �{�a�� ����*/
   rec_size=ftell(fp)/52;  /*�e���� �A���a �a���a�� 52���a���e ���A �A���a��*/
			   /* ftell()�q���e �e�� ��á�� �a���a���i ����Ё��*/
   start=0;		   /*�q���i ��᷁ ��q�� �{ ���e�i ����*/
   end=ftell(fp);
   middle=start+((end/52-start/52)/2)*52;

   while(start!=middle&&end!=middle){

       fseek(fp,middle,0);
       fscanf(fp,"%7s",search_giho);

       if((strcmp(search_giho,giho))==0){ /*���e�� �A���a�a �{�a�e �a�A���a*/
	     fseek(fp,middle+7,0);
	     fscanf(fp,"%8s%8s%29s",si,gu,dong);
	     break;
	     }

	 if((strcmp(search_giho,giho))<0){
	     start=middle;
	     end=end;
	     middle=start+((end/52-start/52)/2)*52;
	     }

	 if((strcmp(search_giho,giho))>0){
	     start=start;
	     end=middle;
	     middle=start+((end/52-start/52)/2)*52;
	     }

	 }
	 gotoxy(1,1);
	 printf("%8s %8s %29s",si,gu,dong);
}
