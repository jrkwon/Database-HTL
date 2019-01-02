
#include "hanojark.h"

struct students {
  char st_no [9];
  char st_name [11];
  char st_sex [2];
  char st_resi [14];
  char st_birth [7];
  char st_solar [2];
  char st_enrolled [2];
  char st_grade [2];
  char st_class [2];
  char st_absence [2];
  char st_military [2];
  char part_no [3];
  char duty_no [3];
  char company_no [3];
  char st_school [13];
  char st_zip1 [7];
  char st_addr1 [67];
  char st_area1 [5];
  char st_tel1 [8];
  char st_zip2 [7];
  char st_addr2 [67];
  char st_area2 [5];
  char st_tel2 [8];
};

struct graduates {
  char gr_no [9];
  char gr_name [11];
  char gr_sex [2];
  char gr_resi [14];
  char gr_birth [7];
  char gr_solar [2];
  char gr_grade [3];
  char gr_course [2];
  char lab_no [3];
  char gr_absence [2];
  char gr_military [2];
  char gr_sec [2];
  char company_no [3];
  char gr_school [13];
  char gr_college [13];
  char gr_zip1 [7];
  char gr_addr1 [67];
  char gr_area1 [5];
  char gr_tel1 [8];
  char gr_zip2 [7];
  char gr_addr2 [67];
  char gr_area2 [5];
  char gr_tel2 [8];
};

struct parts {
  char part_no [3];
  char part_name [11];
};

struct duties {
  char duty_no [3];
  char duty_name [11];
};

struct companies {
  char company_no [3];
  char company_name [21];
};

struct labs {
  char lab_no [3];
  char lab_professor [13];
  char lab_note [51];
};

char *denames [] = {
  "ST_NO",
  "ST_NAME",
  "ST_SEX",
  "ST_RESI",
  "ST_BIRTH",
  "ST_SOLAR",
  "ST_ENROLLED",
  "ST_GRADE",
  "ST_CLASS",
  "ST_ABSENCE",
  "ST_MILITARY",
  "ST_SCHOOL",
  "ST_ZIP1",
  "ST_AREA1",
  "ST_TEL1",
  "ST_ADDR1",
  "ST_ZIP2",
  "ST_AREA2",
  "ST_TEL2",
  "ST_ADDR2",
  "GR_NO",
  "GR_NAME",
  "GR_SEX",
  "GR_RESI",
  "GR_BIRTH",
  "GR_SOLAR",
  "GR_GRADE",
  "GR_COURSE",
  "GR_ABSENCE",
  "GR_MILITARY",
  "GR_SEC",
  "GR_SCHOOL",
  "GR_COLLEGE",
  "GR_ZIP1",
  "GR_AREA1",
  "GR_TEL1",
  "GR_ADDR1",
  "GR_ZIP2",
  "GR_AREA2",
  "GR_TEL2",
  "GR_ADDR2",
  "PART_NO",
  "PART_NAME",
  "DUTY_NO",
  "DUTY_NAME",
  "COMPANY_NO",
  "COMPANY_NAME",
  "LAB_NO",
  "LAB_PROFESSOR",
  "LAB_NOTE",
  0
};

char *hdenames [] = {
  "Ðb¤å",
  "·¡Ÿq",
  "¬÷¥i",
  "º£¥—w¢¤åÑ¡",
  "¬—‘e¶©·©",
  "·q´·",
  "Íe·³µa¦",
  "Ðb‘e",
  "¤e",
  "¸Ðbµa¦",
  "¥wµbŠ¦…",
  "Â‰¯¥‰¡",
  "¶Íe¤åÑ¡(¶¥)",
  "»¡µb¤åÑ¡",
  "¸åÑÁ¤åÑ¡",
  "º­¡",
  "¶Íe¤åÑ¡(Ñe)",
  "»¡µb¤åÑ¡",
  "¸åÑÁ¤åÑ¡",
  "º­¡",
  "Ðb¤å",
  "·¡Ÿq",
  "¬÷¥i",
  "º£¥—w¢¤åÑ¡",
  "¬—‘e¶©·©",
  "·q´·",
  "‹¡®",
  "‰Á¸÷",
  "¸Ðbµa¦",
  "¥wµbŠ¦…",
  "Ðb¬—Š¦…",
  "Â‰¯¥‰¡",
  "Â‰¯¥”",
  "¶Íe¤åÑ¡(¶¥)",
  "»¡µb¤åÑ¡",
  "¸åÑÁ¤åÑ¡",
  "º­¡",
  "¶Íe¤åÑ¡(Ñe)",
  "»¡µb¤åÑ¡",
  "¸åÑÁ¤åÑ¡",
  "º­¡",
  "¦¬á¤åÑ¡",
  "¦¬á·¡Ÿq",
  "»¢À‚¤åÑ¡",
  "»¢À‚·¡Ÿq",
  "ÒA¬a¤åÑ¡",
  "ÒA¬a·¡Ÿq",
  "µeŠ¯©¤åÑ¡",
  "»¡•¡Ša®“±",
  "µeŠ¯©¬é¡w",
  0
};

char eltype [] = "HHHHDHHHHHHHHNNHHNNHHHHHDHNHHHHHHHNNHHNNHNHNHNHNHH";

char *elmask [] = {
  "99999999",
  "XXXXXXXXXX",
  "9 (q1/µa0)",
  "999999-9999999",
  "99.##.##",
  "9 (·q1,´·0)",
  "9 (Íe1/¯¥0)",
  "9",
  "9 (¢0)",
  "9 (ÓA1/¹©2/¸0)",
  "9 (Ñe1/Ï©2/£¡3/¡e0)",
  "XXXXXXXXXXXX",
  "999-999",
  "####",
  "###-####",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  "999-999",
  "####",
  "###-####",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  "99999999",
  "XXXXXXXXXX",
  "9 (q1/µa0)",
  "999999-9999999",
  "99.##.##",
  "9 (·q1/´·0)",
  "##",
  "9 (¬â1/¤b0)",
  "9 (ÓA1/¹©2/¸0)",
  "9 (Ñe1/Ï©2/£¡3/¡e0)",
  "9 (Î‰1/Ìa0)",
  "XXXXXXXXXXXX",
  "XXXXXXXXXXXX",
  "999-999",
  "####",
  "###-####",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  "999-999",
  "####",
  "###-####",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  "##",
  "XXXXXXXXXX",
  "##",
  "XXXXXXXXXX",
  "##",
  "XXXXXXXXXXXXXXXXXXXX",
  "##",
  "XXXXXXXXXXXX",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
};

char *dbfiles [] = {
  "STUDENTS",
  "GRADUATES",
  "PARTS",
  "DUTIES",
  "COMPANIES",
  "LABS",
  0
};


int ellen [] = {
  8,10,1,13,6,1,1,1,1,1,1,12,6,4,7,66,6,4,7,66,8,10,1,13,6,
  1,2,1,1,1,1,12,12,6,4,7,66,6,4,7,66,2,10,2,10,2,20,2,12,50
};


int f_students [] = {
  ST_NO,
  ST_NAME,
  ST_SEX,
  ST_RESI,
  ST_BIRTH,
  ST_SOLAR,
  ST_ENROLLED,
  ST_GRADE,
  ST_CLASS,
  ST_ABSENCE,
  ST_MILITARY,
  PART_NO,
  DUTY_NO,
  COMPANY_NO,
  ST_SCHOOL,
  ST_ZIP1,
  ST_ADDR1,
  ST_AREA1,
  ST_TEL1,
  ST_ZIP2,
  ST_ADDR2,
  ST_AREA2,
  ST_TEL2,
  0
};

int f_graduates [] = {
  GR_NO,
  GR_NAME,
  GR_SEX,
  GR_RESI,
  GR_BIRTH,
  GR_SOLAR,
  GR_GRADE,
  GR_COURSE,
  LAB_NO,
  GR_ABSENCE,
  GR_MILITARY,
  GR_SEC,
  COMPANY_NO,
  GR_SCHOOL,
  GR_COLLEGE,
  GR_ZIP1,
  GR_ADDR1,
  GR_AREA1,
  GR_TEL1,
  GR_ZIP2,
  GR_ADDR2,
  GR_AREA2,
  GR_TEL2,
  0
};

int f_parts [] = {
  PART_NO,
  PART_NAME,
  0
};

int f_duties [] = {
  DUTY_NO,
  DUTY_NAME,
  0
};

int f_companies [] = {
  COMPANY_NO,
  COMPANY_NAME,
  0
};

int f_labs [] = {
  LAB_NO,
  LAB_PROFESSOR,
  LAB_NOTE,
  0
};

int *file_ele [] = {
  f_students,
  f_graduates,
  f_parts,
  f_duties,
  f_companies,
  f_labs,
  0
};

int x1_students [] = {
  ST_NO,
  0
};

int x2_students [] = {
  ST_NAME,
  0
};

int *x_students [] = {
  x1_students,
  x2_students,
  0
};

int x1_graduates [] = {
  GR_NO,
  0
};

int x2_graduates [] = {
  GR_NAME,
  0
};

int *x_graduates [] = {
  x1_graduates,
  x2_graduates,
  0
};

int x1_parts [] = {
  PART_NO,
  0
};

int *x_parts [] = {
  x1_parts,
  0
};

int x1_duties [] = {
  DUTY_NO,
  0
};

int *x_duties [] = {
  x1_duties,
  0
};

int x1_companies [] = {
  COMPANY_NO,
  0
};

int *x_companies [] = {
  x1_companies,
  0
};

int x1_labs [] = {
  LAB_NO,
  0
};

int *x_labs [] = {
  x1_labs,
  0
};

int **index_ele [] = {
  x_students,
  x_graduates,
  x_parts,
  x_duties,
  x_companies,
  x_labs,
  0
};
