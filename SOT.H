/********** The Son of Tetris Project ************/

/* Include file for SOT project */

/************ DEFINES ***********/


#define TRUE  1
#define FALSE !TRUE

#define TABLE_LEN 5   /* no of entries in hi score table */

#define BLW 12       /* Width */
#define BLH 22       /* Height */
#define SCRW 80      /* Screen width */

#define NO_OF_KEY_PRESSES 10  /* Max no of key presses processed per iteration */

#define HORIZ_MOVE 1        /* Increments of horizontal movement */
#define VERT_MOVE 1         /* Vertical movement */

/* Attributes */

#define CLEAR  0     /* = BLACK, illegal shape colour */
#define BORDER 256

#define LEFT   (75+256)
#define RIGHT  (77+256)
#define HOME   (71+256)
#define PGUP   (73+256)
#define UP     (72+256)
#define ESC    (27)

/* Type definitions */

typedef enum{ test, insert, remve} sh_op_type; /* Operations permissible on shapes */
typedef enum{ left, down, right, up, anti, clk} drctn_type; /* Directions to go */


/************ TYPEDEFS ***********/

typedef struct
{
  int        ht;   /* ht/wd in orientation 0 */
  int        wd;
  int        col;
  int       *map[4];
  void      *next_shp;
} SHP_TYPE;


typedef struct
{
  int       o_x, o_y;   /* current position */
  int       o_or;       /* current orientation */
  SHP_TYPE *o_sh;       /* object shape */
  int       o_temp;     /* object temperature */
} OBJ_TYPE;

/************ DATA ***********/

extern int        arena[BLW][BLH];   /* Where everything is stored */
extern SHP_TYPE  *shp_lst;           /* List of shape defns */
extern int        no_of_shapes;      /* Total shape defns held */

/************ PROTOTYPES ***********/

void      champ_val(char *name, long *score_val, int rank);
void      draw_ground(void);
void      end_score(void);
void      end_video(void);
void      end_game(void);
int       get_key(void);
void      game(void);
unsigned  get_another_go(long new_score);/* Dialogue between games */
int       get_cursor_state(void);
int       get_level(void);        /* Get start level */
void      init_arena(void);       /* Clear out arena and define borders */
void      init_score(char *prog_name);
void      init_shapes(void);
void      init_video(void);
unsigned  pause_game(void);       /* Pause window */
void      p_arena(int left,int top,int right,int bottom);  /* video display */
void      set_cursor_state(int state);
void      update_score(unsigned level, long score, unsigned lines_del, SHP_TYPE *next_sh, int show_shape);
void      update_score_table(unsigned char *name,long new_score);
void      v_kill_row(int  t_row);

