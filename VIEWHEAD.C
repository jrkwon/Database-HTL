#include <stdio.h>

struct fhdr {
  long first_record;
  long next_record;
  int record_len;
};

struct fhdr header;
FILE *db_file;

void main(int argc, char *argv[])
{
  printf("+ Ojarkyo Data File Header Viewer Ver 1.00\n");
  if(argc != 2) {
    printf("> Usage : viewhead <datafilename>\n");
    return;
  }
  db_file = fopen(argv[1], "rb");
  if(!db_file) {
    printf("File %s not found\n", argv[1]);
    return;
  }
  fread(&header,sizeof(header), 1, db_file);
  printf(">> %s Header Infomation\n", argv[1]);
  printf("  First Record : %ld, Next Record : %ld, Record Length : %d\n",
           header.first_record, header.next_record, header.record_len);
  fclose(db_file);
}
