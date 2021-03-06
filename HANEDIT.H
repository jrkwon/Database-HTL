/*
 *  �e���� �q���� �១
 *  HANEDIT.H
 */

/*
 *  �a�q�e �e������ ���wǡ(��w, ���ᵡ��)�A ���e �����t���a.
 *  �e, �e�w ɡ�iǡ�e ������a�� SHIFT-SPACE����, ���A�A �a�e ǡ�� �w���i �� ���a.
 */
extern int save_file_key       ;   /* Save : ��w�a�� */
extern int help_key            ;   /* �����i ǡ */
extern int han_toggle_key      ;   /* �e/�w �a��ǡ */
extern int hanja_conv_key      ;   /* �e�i->�e�aǡ */
extern int kdb_toggle_key      ;   /* 3�鯢/2�鯢 �a��ǡ */
extern int ks_graph_key        ;   /* KS 2�a���a �a�����a ���b */
extern int tg_graph_key        ;   /* TG 2�a���a �a�����a ���b */
extern int quit_key            ;   /* ��w�i ������ ����, �e���i �{���� */

bool set_tabsize(int size);
int  get_tabsize(void);
bool init_editor(char *filename, char *help_filename, long bufsize, int kdb_kind, bool han_eng);
bool editor(int x, int y, int width, int height, int border);

