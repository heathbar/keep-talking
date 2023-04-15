#include "bitmaps.h"

// 'command', 98x98px
struct Glyph
{
  short x;
  short y;
  short w;
  short h;
  const unsigned char *data;
};

// const Glyph glyphs[] = {
//   { 32, 40, 57, 80, epd_bitmap_ace },
//   { 23, 43, 75, 75, epd_bitmap_command },
//   { 35, 43, 50, 75, epd_bitmap_delta },
//   { 23, 44, 75, 73, epd_bitmap_dharma },
//   { 23, 44, 75, 73, epd_bitmap_diamonds },
//   { 39, 40, 43, 80, epd_bitmap_domino },
//   { 23, 44, 75, 73, epd_bitmap_ef },
//   { 34, 43, 52, 75, epd_bitmap_eight },
//   { 32, 43, 56, 75, epd_bitmap_euro },
//   { 38, 43, 44, 75, epd_bitmap_glottal },
//   { 27, 43, 67, 75, epd_bitmap_hebrew },
//   { 23, 49, 75, 63, epd_bitmap_iotified_ys },
//   { 24, 43, 73, 75, epd_bitmap_king },
//   { 42, 40, 36, 80, epd_bitmap_ksi },
//   { 25, 44, 71, 73, epd_bitmap_mahjong },
//   { 23, 44, 75, 72, epd_bitmap_mouse },
//   { 24, 43, 73, 75, epd_bitmap_music_note },
//   { 25, 43, 70, 75, epd_bitmap_psi },
//   { 34, 43, 53, 75, epd_bitmap_qoppa },
//   { 40, 40, 40, 80, epd_bitmap_section },
//   { 23, 53, 75, 54, epd_bitmap_smiley },
//   { 23, 43, 75, 75, epd_bitmap_star },
//   { 30, 43, 60, 75, epd_bitmap_stool },
//   { 24, 44, 72, 72, epd_bitmap_tapereel },
//   { 23, 49, 75, 62, epd_bitmap_yu },
//   { 23, 48, 75, 64, epd_bitmap_yus },
//   { 23, 47, 75, 66, epd_bitmap_zhe }
// };


Glyph glyph_groups[6][7] = {
  {
    {34, 43, 53, 75, epd_bitmap_qoppa},
    {23, 48, 75, 64, epd_bitmap_yus},
    {38, 43, 44, 75, epd_bitmap_glottal},
    {24, 44, 72, 72, epd_bitmap_tapereel},
    {23, 49, 75, 63, epd_bitmap_iotified_ys},
    {27, 43, 67, 75, epd_bitmap_hebrew},
    {23, 44, 75, 73, epd_bitmap_dharma}
  },
  {
    {32, 43, 56, 75, epd_bitmap_euro},
    {34, 43, 53, 75, epd_bitmap_qoppa},
    {23, 44, 75, 73, epd_bitmap_dharma},
    {23, 44, 75, 73, epd_bitmap_ef},
    {23, 43, 75, 75, epd_bitmap_command},
    {27, 43, 67, 75, epd_bitmap_hebrew},
    {24, 43, 73, 75, epd_bitmap_music_note}
  },
  {
    {23, 49, 75, 62, epd_bitmap_yu},
    {40, 40, 40, 80, epd_bitmap_section},
    {23, 44, 75, 73, epd_bitmap_ef},
    {23, 47, 75, 66, epd_bitmap_zhe},
    {32, 40, 57, 80, epd_bitmap_ace},
    {38, 43, 44, 75, epd_bitmap_glottal},
    {23, 43, 75, 75, epd_bitmap_command}
  },
  {
    {35, 43, 50, 75, epd_bitmap_delta},
    {23, 44, 75, 73, epd_bitmap_diamonds},
    {23, 44, 75, 72, epd_bitmap_mouse},
    {23, 49, 75, 63, epd_bitmap_iotified_ys},
    {23, 47, 75, 66, epd_bitmap_zhe},
    {24, 43, 73, 75, epd_bitmap_music_note},
    {23, 53, 75, 54, epd_bitmap_smiley}
  },
  {
    {25, 43, 70, 75, epd_bitmap_psi},
    {23, 53, 75, 54, epd_bitmap_smiley},
    {23, 44, 75, 72, epd_bitmap_mouse},
    {34, 43, 52, 75, epd_bitmap_eight},
    {23, 44, 75, 73, epd_bitmap_diamonds},
    {42, 40, 36, 80, epd_bitmap_ksi},
    {23, 43, 75, 75, epd_bitmap_star}
  },
  {
    {35, 43, 50, 75, epd_bitmap_delta},
    {32, 43, 56, 75, epd_bitmap_euro},
    {30, 43, 60, 75, epd_bitmap_stool},
    {39, 40, 43, 80, epd_bitmap_domino},
    {25, 43, 70, 75, epd_bitmap_psi},
    {25, 44, 71, 73, epd_bitmap_mahjong},
    {24, 43, 73, 75, epd_bitmap_king}
  }
};