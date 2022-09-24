/*
 * shifting
 */


int main(void) {
  unsigned int u32a, u32b, u32c;
  signed int s32a, s32b, s32c;

  u32a = (unsigned int) 0x87654321;
  u32b = u32a << 13;
  u32c = u32a >> 13;
  /* ----- */
  s32a = (signed int) 0x87654321;
  s32b = s32a << 13;
  s32c = s32a >> 13;
  return 0;
}
