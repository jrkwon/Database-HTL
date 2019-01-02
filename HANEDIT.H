/*
 *  Íe»³‹¡ Ðq®·  áŸ¡
 *  HANEDIT.H
 */

/*
 *  ”a·q·e Íe»³‹¡· ‹¡“wÇ¡(¸á¸w, ¦‰œáµ¡‹¡)µA ”Ðe ¸÷ˆt·¡”a.
 *  ”e, Ðeµw É¡‹iÇ¡“e ‹¡¥¥¸â·a¡ SHIFT-SPACE·¡‰¡, ·¡¶AµA ”aŸe Ç¡•¡ —w¢Ði ® ·¶”a.
 */
extern int save_file_key       ;   /* Save : ¸á¸wÐa‹¡ */
extern int help_key            ;   /* •¡¶‘ i Ç¡ */
extern int han_toggle_key      ;   /* Ðe/µw ¤aŽ‘Ç¡ */
extern int hanja_conv_key      ;   /* Ðe‹i->Ðe¸aÇ¡ */
extern int kdb_toggle_key      ;   /* 3¤é¯¢/2¤é¯¢ ¤aŽ‘Ç¡ */
extern int ks_graph_key        ;   /* KS 2¤a·¡Ëa ‹aŸ±¢…¸a ·³b */
extern int tg_graph_key        ;   /* TG 2¤a·¡Ëa ‹aŸ±¢…¸a ·³b */
extern int quit_key            ;   /* ¸á¸wÐi ˆõ·¥»¡ ¢ˆ‰¡, Íe»³·i {‹¡ */

bool set_tabsize(int size);
int  get_tabsize(void);
bool init_editor(char *filename, char *help_filename, long bufsize, int kdb_kind, bool han_eng);
bool editor(int x, int y, int width, int height, int border);

