
#include "min-stru.h"

struct prof {
  char prof_name [11];
  char prof_major [11];
  char prof_sch [21];
  char prof_status [11];
  char prof_zip1 [7];
  char prof_addr1 [56];
  char prof_area1 [5];
  char prof_tel1 [8];
  char prof_zip2 [7];
  char prof_addr2 [56];
  char prof_area2 [5];
  char prof_tel2 [8];
  char prof_memo [56];
};

struct group {
  char group_name [21];
  char group_presi [11];
  char group_zip [7];
  char group_addr [56];
  char group_area [5];
  char group_tel [8];
  char group_memo [56];
};

struct media {
  char media_name [21];
  char media_part [11];
  char media_person [11];
  char media_zip [7];
  char media_addr [56];
  char media_area [5];
  char media_tel [8];
  char media_fax [8];
  char media_memo [56];
};

struct support {
  char support_name [21];
  char support_part [11];
  char support_job [11];
  char support_invi [11];
  char support_zip [7];
  char support_addr [56];
  char support_area [5];
  char support_tel [8];
  char support_memo [56];
};

struct camp {
  char camp_name [11];
  char camp_no [4];
  char camp_hak [3];
  char camp_sosok [21];
  char camp_major [11];
  char camp_zip [7];
  char camp_addr [56];
  char camp_area [5];
  char camp_tel [8];
  char camp_memo [56];
};

char *denames [] = {
  "PROF_NAME",
  "PROF_MAJOR",
  "PROF_SCH",
  "PROF_STATUS",
  "PROF_ZIP1",
  "PROF_ADDR1",
  "PROF_AREA1",
  "PROF_TEL1",
  "PROF_ZIP2",
  "PROF_ADDR2",
  "PROF_AREA2",
  "PROF_TEL2",
  "PROF_MEMO",
  "GROUP_NAME",
  "GROUP_PRESI",
  "GROUP_ZIP",
  "GROUP_ADDR",
  "GROUP_AREA",
  "GROUP_TEL",
  "GROUP_MEMO",
  "MEDIA_NAME",
  "MEDIA_PART",
  "MEDIA_PERSON",
  "MEDIA_ZIP",
  "MEDIA_ADDR",
  "MEDIA_AREA",
  "MEDIA_TEL",
  "MEDIA_FAX",
  "MEDIA_MEMO",
  "SUPPORT_NAME",
  "SUPPORT_PART",
  "SUPPORT_JOB",
  "SUPPORT_INVI",
  "SUPPORT_ZIP",
  "SUPPORT_ADDR",
  "SUPPORT_AREA",
  "SUPPORT_TEL",
  "SUPPORT_MEMO",
  "CAMP_NAME",
  "CAMP_NO",
  "CAMP_HAK",
  "CAMP_SOSOK",
  "CAMP_MAJOR",
  "CAMP_ZIP",
  "CAMP_ADDR",
  "CAMP_AREA",
  "CAMP_TEL",
  "CAMP_MEMO",
  0
};

char *hdenames [] = {
  "·¡   Ÿq   ",
  "¸å   ‰·   ",
  "Ðb   Ša   ",
  "»¢   À‚   ",
  "»³ ¶Íe¤åÑ¡   ",
  "»³ º­¡   ",
  "»³ »¡µb¤åÑ¡   ",
  "»³ ¸åÑÁ¤åÑ¡   ",
  "»¢¸w ¶Íe¤åÑ¡ ",
  "»¢¸w º­¡ ",
  "»¢¸w »¡µb¤åÑ¡ ",
  "»¢¸w ¸åÑÁ¤åÑ¡ ",
  "¸â´á–‰ ˆõ ",
  "”eÁA·¡Ÿq   ",
  "”    Îa   ",
  "¶Íe¤åÑ¡   ",
  "º    ­¡   ",
  "»¡µb¤åÑ¡   ",
  "¸åÑÁ¤åÑ¡   ",
  "¸â´á–‰ ˆõ  ",
  "‹¡‰Å·¡Ÿq   ",
  "‹¡‰ÅŠ¦…   ",
  "”q ”w ¸a   ",
  "¶Íe¤åÑ¡   ",
  "º    ­¡   ",
  "»¡µb¤åÑ¡   ",
  "¸åÑÁ¤åÑ¡   ",
  "Ì‚¯a¤åÑ¡   ",
  "¸â´á–‰ ˆõ  ",
  "ÒA¶¥·¡Ÿq   ",
  "ÒA¶¥Š¦…   ",
  "»¢    ´ó   ",
  "­¡ˆÐe ¬aœq",
  "¶Íe¤åÑ¡   ",
  "º    ­¡   ",
  "»¡µb¤åÑ¡   ",
  "¸åÑÁ¤åÑ¡   ",
  "¸â´á–‰ ˆõ  ",
  "·¡    Ÿq   ",
  "Àq ˆa ÒA   ",
  "Ðb    ¤å   ",
  "­¡    ­¢   ",
  "¸å    ‰·   ",
  "¶Íe¤åÑ¡   ",
  "º    ­¡   ",
  "»¡µb¤åÑ¡   ",
  "¸åÑÁ¤åÑ¡   ",
  "¸â´á–‰ ˆõ  ",
  0
};

char eltype [] = "HHHHHHNNHHNNHHHHHNNHHHHHHNNNHHHHHHHNNHHNNHHHHNNH";

char *elmask [] = {
  "XXXXXXXXXX",
  "XXXXXXXXXX",
  "XXXXXXXXXXXXXXXXXXXX",
  "XXXXXXXXXX",
  "999-999",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  "####",
  "###-####",
  "999-999",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  "####",
  "###-####",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  "XXXXXXXXXXXXXXXXXXXX",
  "XXXXXXXXXX",
  "999-999",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  "####",
  "###-####",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  "XXXXXXXXXXXXXXXXXXXX",
  "XXXXXXXXXX",
  "XXXXXXXXXX",
  "999-999",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  "####",
  "###-####",
  "###-####",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  "XXXXXXXXXXXXXXXXXXXX",
  "XXXXXXXXXX",
  "XXXXXXXXXX",
  "XXXXXXXXXX",
  "999-999",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  "####",
  "###-####",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  "XXXXXXXXXX",
  "###",
  "##",
  "XXXXXXXXXXXXXXXXXXXX",
  "XXXXXXXXXX",
  "999-999",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  "####",
  "###-####",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
};

char *dbfiles [] = {
  "PROF",
  "GROUP",
  "MEDIA",
  "SUPPORT",
  "CAMP",
  0
};


int ellen [] = {
  10,10,20,10,6,55,4,7,6,55,4,7,55,20,10,6,55,4,7,55,20,10,10,6,55,
  4,7,7,55,20,10,10,10,6,55,4,7,55,10,3,2,20,10,6,55,4,7,55
};


int f_prof [] = {
  PROF_NAME,
  PROF_MAJOR,
  PROF_SCH,
  PROF_STATUS,
  PROF_ZIP1,
  PROF_ADDR1,
  PROF_AREA1,
  PROF_TEL1,
  PROF_ZIP2,
  PROF_ADDR2,
  PROF_AREA2,
  PROF_TEL2,
  PROF_MEMO,
  0
};

int f_group [] = {
  GROUP_NAME,
  GROUP_PRESI,
  GROUP_ZIP,
  GROUP_ADDR,
  GROUP_AREA,
  GROUP_TEL,
  GROUP_MEMO,
  0
};

int f_media [] = {
  MEDIA_NAME,
  MEDIA_PART,
  MEDIA_PERSON,
  MEDIA_ZIP,
  MEDIA_ADDR,
  MEDIA_AREA,
  MEDIA_TEL,
  MEDIA_FAX,
  MEDIA_MEMO,
  0
};

int f_support [] = {
  SUPPORT_NAME,
  SUPPORT_PART,
  SUPPORT_JOB,
  SUPPORT_INVI,
  SUPPORT_ZIP,
  SUPPORT_ADDR,
  SUPPORT_AREA,
  SUPPORT_TEL,
  SUPPORT_MEMO,
  0
};

int f_camp [] = {
  CAMP_NAME,
  CAMP_NO,
  CAMP_HAK,
  CAMP_SOSOK,
  CAMP_MAJOR,
  CAMP_ZIP,
  CAMP_ADDR,
  CAMP_AREA,
  CAMP_TEL,
  CAMP_MEMO,
  0
};

int *file_ele [] = {
  f_prof,
  f_group,
  f_media,
  f_support,
  f_camp,
  0
};

int x1_prof [] = {
  PROF_NAME,
  0
};

int *x_prof [] = {
  x1_prof,
  0
};

int x1_group [] = {
  GROUP_NAME,
  0
};

int x2_group [] = {
  GROUP_PRESI,
  0
};

int *x_group [] = {
  x1_group,
  x2_group,
  0
};

int x1_media [] = {
  MEDIA_NAME,
  0
};

int x2_media [] = {
  MEDIA_PERSON,
  0
};

int *x_media [] = {
  x1_media,
  x2_media,
  0
};

int x1_support [] = {
  SUPPORT_NAME,
  0
};

int x2_support [] = {
  SUPPORT_INVI,
  0
};

int *x_support [] = {
  x1_support,
  x2_support,
  0
};

int x1_camp [] = {
  CAMP_NAME,
  CAMP_NO,
  0
};

int *x_camp [] = {
  x1_camp,
  0
};

int **index_ele [] = {
  x_prof,
  x_group,
  x_media,
  x_support,
  x_camp,
  0
};
