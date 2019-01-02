#include <string.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>

long ftell();
void display_zip(char *giho);	/*·¡»¥ Èq¬‚¶w voidÑw Ðq® */

main()
{
   char giho[9];
   char key;
   int i;
   clrscr();

   while(1){
	   strcpy(giho,"");      /*Á¡‹¡ÑÁŸi ¯¡Åa º´á´¡ Ðe”a */
	   gotoxy(10,10);
	   printf("‹¡Ñ¡Ÿi ·³bÐa­A¶a==>");
	   gets(giho);		 /*¶Íe¤åÑ¡Ÿi ·³b ¤h“e”a*/
	   display_zip(giho);	 /*·¡»¥Èq¬‚Ðaµa º­¡Ÿi Îa¯¡Ða“e Ðq®Ñ¡Â‰*/

	   gotoxy(10,14);
	   printf("™¡ ÐiŒa¶a?[Y/anykey]");

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

   fseek(fp,0,2);    /*ÑÁ·©· {·a¡ ·¡•·*/
   rec_size=ftell(fp)/52;  /*Ðeˆ· •A·¡Èa ¤a·¡Ëa® 52¡a’¡e ¸åÁA •A·¡Èa®*/
			   /* ftell()Ðq®“e Ñe¸ ¶áÃ¡· ¤a·¡Ëa®Ÿi »¡¯¡Ðº‘*/
   start=0;		   /*Èq¬‚Ði ¤ñ¶á· Àá·q‰Á { º—ˆe·i »¡¸÷*/
   end=ftell(fp);
   middle=start+((end/52-start/52)/2)*52;

   while(start!=middle&&end!=middle){

       fseek(fp,middle,0);
       fscanf(fp,"%7s",search_giho);

       if((strcmp(search_giho,giho))==0){ /*º—ˆe· •A·¡Èaˆa ˆ{·a¡e §aA·¡Ça*/
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