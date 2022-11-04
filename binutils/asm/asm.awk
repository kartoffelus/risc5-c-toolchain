#
# asm.awk -- implement a poor man's variant of inline assembler
#

{
  if ($1 != ".GLOBAL" || substr($2,1,5) != "_asm_")
  {
    if ($1 == "C" && substr($2,1,5) == "_asm_")
    {
      printf def[substr($2,6)]
    }
    else
    {
      print $0
    }
  }
  # else do nothing: suppress .GLOBAL declarations for _asm_xyz
}
