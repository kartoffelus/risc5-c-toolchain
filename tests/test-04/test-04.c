/*
 * 16-bit load/store
 */


int main(void) {
  unsigned short u16a, u16b;
  signed short s16a, s16b;

  u16a = (unsigned short) 0x8765;
  u16b = u16a;
  /* ----- */
  s16a = (signed short) 0x8765;
  s16b = s16a;
  return 0;
}
