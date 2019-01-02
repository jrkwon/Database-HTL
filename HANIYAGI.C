#include "hanfont.h"

typedef struct {
    byte HAN1[8][INITIALNUM][FS16X16];
    byte HAN2[4][MEDIALNUM][FS16X16];
    byte HAN3[4][FINALNUM][FS16X16];
} haniyg_t;

struct linkedin_hanfont_t _haniyg = {
    ((haniyg_t far *)_HAN8MD1_font)->HAN1,
    ((haniyg_t far *)_HAN8MD1_font)->HAN2,
    ((haniyg_t far *)_HAN8MD1_font)->HAN3,
    _fontindex8x4x4
};
