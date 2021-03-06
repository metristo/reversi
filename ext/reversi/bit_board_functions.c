#include "bit_board_functions.h"

/*
 * Convert coordinate data to a bitboard.
 */
uint64_t XY2BB(int x, int y) {
  return (uint64_t)1 << ((8-x) + (8-y) * 8);
}

/*
 * Convert a bitboard to coordinate data.
 */
VALUE BB2XY(uint64_t bb) {
  VALUE xy = rb_ary_new();
  int x = 8 - ( (int)log2((double)bb) % 8 );
  int y = 8 - ( (int)log2((double)bb) / 8 );
  rb_ary_push(xy, INT2FIX(x));
  rb_ary_push(xy, INT2FIX(y));
  return xy;
}

/*
 * Rotate a bitboard by 90 degrees to the right.
 * 
 * For example,
 * 1 1 1 1 1 1 1 1    0 0 0 0 0 0 0 1
 * 0 0 0 0 0 0 0 0    0 0 0 0 0 0 0 1
 * 0 0 0 0 0 0 0 0    0 0 0 0 0 0 0 1
 * 0 0 0 0 0 0 0 0    0 0 0 0 0 0 0 1
 * 0 0 0 0 0 0 0 0 -> 0 0 0 0 0 0 0 1
 * 0 0 0 0 0 0 0 0    0 0 0 0 0 0 0 1
 * 0 0 0 0 0 0 0 0    0 0 0 0 0 0 0 1
 * 0 0 0 0 0 0 0 0    0 0 0 0 0 0 0 1
 */
uint64_t rotate_r90(uint64_t bb) {
  bb = ((bb <<  8) & 0xAA00AA00AA00AA00) |
       ((bb >>  8) & 0x0055005500550055) |
       ((bb <<  1) & 0x00AA00AA00AA00AA) |
       ((bb >>  1) & 0x5500550055005500);
  bb = ((bb << 16) & 0xCCCC0000CCCC0000) |
       ((bb >> 16) & 0x0000333300003333) |
       ((bb <<  2) & 0x0000CCCC0000CCCC) |
       ((bb >>  2) & 0x3333000033330000);
  bb = ((bb << 32) & 0xF0F0F0F000000000) |
       ((bb >> 32) & 0x000000000F0F0F0F) |
       ((bb <<  4) & 0x00000000F0F0F0F0) |
       ((bb >>  4) & 0x0F0F0F0F00000000);
  return bb;
}

/*
 * Rotate a bitboard by 90 degrees to the left.
 * 
 * For example,
 * 1 1 1 1 1 1 1 1    1 0 0 0 0 0 0 0
 * 0 0 0 0 0 0 0 0    1 0 0 0 0 0 0 0
 * 0 0 0 0 0 0 0 0    1 0 0 0 0 0 0 0
 * 0 0 0 0 0 0 0 0    1 0 0 0 0 0 0 0
 * 0 0 0 0 0 0 0 0 -> 1 0 0 0 0 0 0 0
 * 0 0 0 0 0 0 0 0    1 0 0 0 0 0 0 0
 * 0 0 0 0 0 0 0 0    1 0 0 0 0 0 0 0
 * 0 0 0 0 0 0 0 0    1 0 0 0 0 0 0 0
 */
uint64_t rotate_l90(uint64_t bb) {
  bb = ((bb <<  1) & 0xAA00AA00AA00AA00) |
       ((bb >>  1) & 0x0055005500550055) |
       ((bb >>  8) & 0x00AA00AA00AA00AA) |
       ((bb <<  8) & 0x5500550055005500);
  bb = ((bb <<  2) & 0xCCCC0000CCCC0000) |
       ((bb >>  2) & 0x0000333300003333) |
       ((bb >> 16) & 0x0000CCCC0000CCCC) |
       ((bb << 16) & 0x3333000033330000);
  bb = ((bb <<  4) & 0xF0F0F0F000000000) |
       ((bb >>  4) & 0x000000000F0F0F0F) |
       ((bb >> 32) & 0x00000000F0F0F0F0) |
       ((bb << 32) & 0x0F0F0F0F00000000);
  return bb;
}

/*
 * Rotate a bitboard by 45 degrees to the right.
 * 
 * For example,
 * 0 0 0 0 0 0 0 1    1 1 1 1 1 1 1 1
 * 0 0 0 0 0 0 1 0    0 0 0 0 0 0 0 0
 * 0 0 0 0 0 1 0 0    0 0 0 0 0 0 0 0
 * 0 0 0 0 1 0 0 0    0 0 0 0 0 0 0 0
 * 0 0 0 1 0 0 0 0 -> 0 0 0 0 0 0 0 0
 * 0 0 1 0 0 0 0 0    0 0 0 0 0 0 0 0
 * 0 1 0 0 0 0 0 0    0 0 0 0 0 0 0 0
 * 1 0 0 0 0 0 0 0    0 0 0 0 0 0 0 0
 */
uint64_t rotate_r45(uint64_t bb) {
  bb = (bb & 0x0101010101010101) |
       (((bb <<  8) | (bb >> 56)) & 0x0202020202020202) |
       (((bb << 16) | (bb >> 48)) & 0x0404040404040404) |
       (((bb << 24) | (bb >> 40)) & 0x0808080808080808) |
       (((bb << 32) | (bb >> 32)) & 0x1010101010101010) |
       (((bb << 40) | (bb >> 24)) & 0x2020202020202020) |
       (((bb << 48) | (bb >> 16)) & 0x4040404040404040) |
       (((bb << 56) | (bb >>  8)) & 0x8080808080808080);
  return bb;
}

/*
 * Rotate a bitboard by 45 degrees to the left.
 * 
 * For example,
 * 1 0 0 0 0 0 0 0    0 0 0 0 0 0 0 0
 * 0 1 0 0 0 0 0 0    0 0 0 0 0 0 0 0
 * 0 0 1 0 0 0 0 0    0 0 0 0 0 0 0 0
 * 0 0 0 1 0 0 0 0    0 0 0 0 0 0 0 0
 * 0 0 0 0 1 0 0 0 -> 0 0 0 0 0 0 0 0
 * 0 0 0 0 0 1 0 0    0 0 0 0 0 0 0 0
 * 0 0 0 0 0 0 1 0    0 0 0 0 0 0 0 0
 * 0 0 0 0 0 0 0 1    1 1 1 1 1 1 1 1
 */
uint64_t rotate_l45(uint64_t bb) {
  bb = (bb & 0x0101010101010101) |
       (((bb >>  8) | (bb << 56)) & 0x0202020202020202) |
       (((bb >> 16) | (bb << 48)) & 0x0404040404040404) |
       (((bb >> 24) | (bb << 40)) & 0x0808080808080808) |
       (((bb >> 32) | (bb << 32)) & 0x1010101010101010) |
       (((bb >> 40) | (bb << 24)) & 0x2020202020202020) |
       (((bb >> 48) | (bb << 16)) & 0x4040404040404040) |
       (((bb >> 56) | (bb <<  8)) & 0x8080808080808080);
  return bb;
}

/*
 * 'p' is a position of player's move, 'XY2BB(FIX2INT(rb_x), FIX2INT(rb_y))'.
 * 'my' is a bitboard of my disks, and 'op' is a bitboard of  opponent's disks.
 * horizontal_pat, vertical_pat and diagonal_pat method
 * are used in Reversi::Board#flip_disks method to get the positions
 * of the opponent's disks between a new disk and another disk of my color.
 * 
 * This method returns a bitboard of flippable oppenent's disks 
 * in the horizontal direction.
 *
 * A mask for the horizontal direction is '0x7E7E7E7E7E7E7E7E'.
 * 0 1 1 1 1 1 1 0
 * 0 1 1 1 1 1 1 0
 * 0 1 1 1 1 1 1 0
 * 0 1 1 1 1 1 1 0
 * 0 1 1 1 1 1 1 0
 * 0 1 1 1 1 1 1 0
 * 0 1 1 1 1 1 1 0
 * 0 1 1 1 1 1 1 0
 */
uint64_t horizontal_pat(uint64_t my, uint64_t op, uint64_t p) {
  op &= 0x7E7E7E7E7E7E7E7E;
  return right_pat(my, op, p) | left_pat(my, op, p);
}

/*
 * This method returns a bitboard of flippable oppenent's disks 
 * in the vertical direction.
 *
 * A mask for the vertical direction is '0x00FFFFFFFFFFFF00'.
 * 0 0 0 0 0 0 0 0
 * 1 1 1 1 1 1 1 1
 * 1 1 1 1 1 1 1 1
 * 1 1 1 1 1 1 1 1
 * 1 1 1 1 1 1 1 1
 * 1 1 1 1 1 1 1 1
 * 1 1 1 1 1 1 1 1
 * 0 0 0 0 0 0 0 0
 */
uint64_t vertical_pat(uint64_t my, uint64_t op, uint64_t p) {
  my = rotate_r90(my);
  op = rotate_r90(op & 0x00FFFFFFFFFFFF00);
  p  = rotate_r90(p);
  return rotate_l90(right_pat(my, op, p) | left_pat(my, op, p));
}

/*
 * This method returns a bitboard of flippable oppenent's disks 
 * in the diagonal direction.
 *
 * a masks for the diagonal direction is '0x007E7E7E7E7E7E00'.
 * 0 0 0 0 0 0 0 0
 * 0 1 1 1 1 1 1 0
 * 0 1 1 1 1 1 1 0
 * 0 1 1 1 1 1 1 0
 * 0 1 1 1 1 1 1 0
 * 0 1 1 1 1 1 1 0
 * 0 1 1 1 1 1 1 0
 * 0 0 0 0 0 0 0 0
 */
uint64_t diagonal_pat(uint64_t my, uint64_t op, uint64_t p) {
  uint64_t my_r45 = rotate_r45(my);
  uint64_t op_r45 = rotate_r45(op & 0x007E7E7E7E7E7E00);
  uint64_t p_r45  = rotate_r45(p);
  uint64_t my_l45 = rotate_l45(my);
  uint64_t op_l45 = rotate_l45(op & 0x007E7E7E7E7E7E00);
  uint64_t p_l45  = rotate_l45(p);
  return rotate_l45(right_pat(my_r45, op_r45, p_r45) | left_pat(my_r45, op_r45, p_r45)) |
         rotate_r45(right_pat(my_l45, op_l45, p_l45) | left_pat(my_l45, op_l45, p_l45));
}

/*
 * This method returns a bitboard of flippable oppenent's disks 
 * in the right direction.
 */
uint64_t right_pat(uint64_t my, uint64_t op, uint64_t p) {
  uint64_t rev =  (p >> 1) & op;
  rev |= (rev >> 1) & op;
  rev |= (rev >> 1) & op;
  rev |= (rev >> 1) & op;
  rev |= (rev >> 1) & op;
  rev |= (rev >> 1) & op;
  return ((rev >> 1) & my) == 0 ? 0 : rev;
}

/*
 * This method returns a bitboard of flippable oppenent's disks 
 * in the left direction.
 */
uint64_t left_pat(uint64_t my, uint64_t op, uint64_t p) {
  uint64_t rev =  (p << 1) & op;
  rev |= (rev << 1) & op;
  rev |= (rev << 1) & op;
  rev |= (rev << 1) & op;
  rev |= (rev << 1) & op;
  rev |= (rev << 1) & op;
  return ((rev << 1) & my) == 0 ? 0 : rev;
}

/*
 * 'p' is a position of player's move, 'XY2BB(FIX2INT(rb_x), FIX2INT(rb_y))'.
 * 'my' is a bitboard of my disks, and 'op' is a bitboard of  opponent's disks.
 * horizontal_pos, vertical_pos and diagonal_pos method
 * are used in Reversi::Board#next_moves method to get positions of the next move.
 * 
 * This method returns a bitboard of nextmove's positions
 * in the horizontal direction.
 */
uint64_t horizontal_pos(uint64_t my, uint64_t op, uint64_t blank) {
  op &= 0x7E7E7E7E7E7E7E7E;
  return right_pos(my, op, blank) | left_pos(my, op, blank);
}

/*
 * This method returns a bitboard of nextmove's positions
 * in the vertical direction.
 */
uint64_t vertical_pos(uint64_t my, uint64_t op, uint64_t blank) {
  my    = rotate_r90(my);
  op    = rotate_r90(op & 0x00FFFFFFFFFFFF00);
  blank = rotate_r90(blank);
  return rotate_l90(right_pos(my, op, blank) | left_pos(my, op, blank));
}

/*
 * This method returns a bitboard of nextmove's positions
 * in the diagonal direction.
 */
uint64_t diagonal_pos(uint64_t my, uint64_t op, uint64_t blank) {
  uint64_t my_r45    = rotate_r45(my);
  uint64_t op_r45    = rotate_r45(op & 0x007E7E7E7E7E7E00);
  uint64_t blank_r45 = rotate_r45(blank);
  uint64_t my_l45    = rotate_l45(my);
  uint64_t op_l45    = rotate_l45(op & 0x007E7E7E7E7E7E00);
  uint64_t blank_l45 = rotate_l45(blank);
  return rotate_l45(right_pos(my_r45, op_r45, blank_r45) | left_pos(my_r45, op_r45, blank_r45)) |
         rotate_r45(right_pos(my_l45, op_l45, blank_l45) | left_pos(my_l45, op_l45, blank_l45));
}

/*
 * This method returns a bitboard of nextmove's positions
 * in the right direction.
 */
uint64_t right_pos(uint64_t my, uint64_t op, uint64_t blank) {
  uint64_t rev =  (my  << 1) & op;
  rev |= (rev << 1) & op;
  rev |= (rev << 1) & op;
  rev |= (rev << 1) & op;
  rev |= (rev << 1) & op;
  rev |= (rev << 1) & op;
  return (rev << 1) & blank;
}

/*
 * This method returns a bitboard of nextmove's positions
 * in the left direction.
 */
uint64_t left_pos(uint64_t my, uint64_t op, uint64_t blank) {
  uint64_t rev =  (my  >> 1) & op;
  rev |= (rev >> 1) & op;
  rev |= (rev >> 1) & op;
  rev |= (rev >> 1) & op;
  rev |= (rev >> 1) & op;
  rev |= (rev >> 1) & op;
  return (rev >> 1) & blank;
}
