#include "hancar.h"

struct customer {
  char cust_num [14];
  char cust_name [11];
  char cust_zip [7];
  char cust_tel [13];
  char cust_addr [60];
  char cust_car [14];
  char cust_code [25];
  char cust_year [4];
  char cust_from [7];
  char cust_to [7];
  char cust_money [10];
  char cust_from2 [7];
  char cust_to2 [7];
  char cust_person0 [8];
  char cust_person1 [4];
  char cust_person2 [4];
  char cust_person3 [4];
  char cust_person4 [11];
  char cust_thing0 [8];
  char cust_thing1 [4];
  char cust_thing2 [4];
  char cust_thing3 [4];
  char cust_thing4 [11];
  char cust_loss0 [8];
  char cust_loss1 [4];
  char cust_loss2 [4];
  char cust_loss3 [4];
  char cust_loss4 [11];
  char cust_car0 [8];
  char cust_car1 [4];
  char cust_car2 [4];
  char cust_car3 [4];
  char cust_car4 [11];
  char cust_total [11];
  char cust_num2 [14];
  char cust_name2 [11];
  char cust_totalins [2];
  char cust_drive [2];
};

struct instsave {
  char save_num [14];
  char save_name [11];
  char save_zip [7];
  char save_tel [13];
  char save_addr [60];
  char save_work [45];
  char save_day [7];
  char save_term [3];
  char save_money [8];
};

char *denames [] = {
  "CUST_NUM",
  "CUST_NAME",
  "CUST_ZIP",
  "CUST_TEL",
  "CUST_ADDR",
  "CUST_CAR",
  "CUST_CODE",
  "CUST_YEAR",
  "CUST_FROM",
  "CUST_TO",
  "CUST_MONEY",
  "CUST_FROM2",
  "CUST_TO2",
  "CUST_PERSON0",
  "CUST_PERSON1",
  "CUST_PERSON2",
  "CUST_PERSON3",
  "CUST_PERSON4",
  "CUST_THING0",
  "CUST_THING1",
  "CUST_THING2",
  "CUST_THING3",
  "CUST_THING4",
  "CUST_LOSS0",
  "CUST_LOSS1",
  "CUST_LOSS2",
  "CUST_LOSS3",
  "CUST_LOSS4",
  "CUST_CAR0",
  "CUST_CAR1",
  "CUST_CAR2",
  "CUST_CAR3",
  "CUST_CAR4",
  "CUST_TOTAL",
  "CUST_NUM2",
  "CUST_NAME2",
  "CUST_TOTALINS",
  "CUST_DRIVE",
  "SAVE_NUM",
  "SAVE_NAME",
  "SAVE_ZIP",
  "SAVE_TEL",
  "SAVE_ADDR",
  "SAVE_WORK",
  "SAVE_DAY",
  "SAVE_TERM",
  "SAVE_MONEY",
  0
};

char *hdenames [] = {
  "º£¥—w¢¤åÑ¡",
  "·¡  Ÿq      ",
  "¶Íe¤åÑ¡    ",
  "¸åÑÁ¤åÑ¡    ",
  "º  ­¡      ",
  "Àaœ·¤åÑ¡    ",
  "Å¡—a¤åÑ¡    ",
  "Àaœ·¡w‘e•¡  ",
  "¥¡Ðñ‹¡ˆe    ",
  "",
  "¥¡Ðñ‹q´‚    ",
  "À‚·±¥¡Ðñ‹¡ˆe",
  "",
  "”  ·¥      ",
  "",
  "",
  "",
  "",
  "”  ¢‰      ",
  "",
  "",
  "",
  "",
  "¸a  ­¥      ",
  "",
  "",
  "",
  "",
  "Àa  œ·      ",
  "",
  "",
  "",
  "",
  "",
  "º£¥—w¢¤åÑ¡",
  "·¡  Ÿq      ",
  "¹·Ðs¥¡Ðñ    ",
  "ˆa¹¢¶…¸å    ",
  "º£¥—w¢¤åÑ¡",
  "·¡  Ÿq      ",
  "¶Íe¤åÑ¡    ",
  "¸åÑÁ¤åÑ¡    ",
  "º  ­¡      ",
  "»¢¸w ¡w     ",
  "¥¡Ðñˆa·³·©  ",
  "¥¡Ðñ‹¡ˆe    ",
  "¶©‹q´‚      ",
  0
};

char eltype [] = "HHHHHHHHDDCDDCNNNMCNNNMCNNNMCNNNMMNHHHHHHHHHDNC";

char *elmask [] = {
  "999999-9999999",
  "XXXXXXXXXX",
  "(999)-(999)",
  "(#####)-(###)-(9999)",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  "HHHH 9HH 9999",
  "99999 XXXXXXXXXXXXXXXXXX",
  "99A",
  "99.##.##",
  "99.##.##",
  "#########",
  "99.##.##",
  "99.##.##",
  "#######",
  "###",
  "###",
  "###",
  "##########",
  "#######",
  "###",
  "###",
  "###",
  "##########",
  "#######",
  "###",
  "###",
  "###",
  "##########",
  "#######",
  "###",
  "###",
  "###",
  "##########",
  "##########",
  "999999-9999999",
  "XXXXXXXXXX",
  "[Y]",
  "[Y]",
  "999999-9999999",
  "XXXXXXXXXX",
  "(999)-(999)",
  "(#####)-(###)-(9999)",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  "99.##.##",
  "##‘e",
  "#######"
};

char *dbfiles [] = {
  "CUSTOMER",
  "INSTSAVE",
  0
};


int ellen [] = {
  13,10,6,12,59,13,24,3,6,6,9,6,6,7,3,3,3,10,7,3,3,3,10,7,3,
  3,3,10,7,3,3,3,10,10,13,10,1,1,13,10,6,12,59,44,6,2,7
};


int f_customer [] = {
  CUST_NUM,
  CUST_NAME,
  CUST_ZIP,
  CUST_TEL,
  CUST_ADDR,
  CUST_CAR,
  CUST_CODE,
  CUST_YEAR,
  CUST_FROM,
  CUST_TO,
  CUST_MONEY,
  CUST_FROM2,
  CUST_TO2,
  CUST_PERSON0,
  CUST_PERSON1,
  CUST_PERSON2,
  CUST_PERSON3,
  CUST_PERSON4,
  CUST_THING0,
  CUST_THING1,
  CUST_THING2,
  CUST_THING3,
  CUST_THING4,
  CUST_LOSS0,
  CUST_LOSS1,
  CUST_LOSS2,
  CUST_LOSS3,
  CUST_LOSS4,
  CUST_CAR0,
  CUST_CAR1,
  CUST_CAR2,
  CUST_CAR3,
  CUST_CAR4,
  CUST_TOTAL,
  CUST_NUM2,
  CUST_NAME2,
  CUST_TOTALINS,
  CUST_DRIVE,
  0
};

int f_instsave [] = {
  SAVE_NUM,
  SAVE_NAME,
  SAVE_ZIP,
  SAVE_TEL,
  SAVE_ADDR,
  SAVE_WORK,
  SAVE_DAY,
  SAVE_TERM,
  SAVE_MONEY,
  0
};

int *file_ele [] = {
  f_customer,
  f_instsave,
  0
};

int x1_customer [] = {
  CUST_NUM,
  0
};

int x2_customer [] = {
  CUST_NAME,
  0
};

int *x_customer [] = {
  x1_customer,
  x2_customer,
  0
};

int x1_instsave [] = {
  SAVE_NUM,
  0
};

int x2_instsave [] = {
  SAVE_NAME,
  0
};

int *x_instsave [] = {
  x1_instsave,
  x2_instsave,
  0
};

int **index_ele [] = {
  x_customer,
  x_instsave,
  0
};
