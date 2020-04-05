#include "../assert.h"
#include "../strings.h"
#include "../disassemble.h"

void main(void) {
  char buf[100];

  // mov
  {
    disassemble(buf, sizeof(buf), 0xe3a0d302);
    assert(strcmp(buf, "mov sp, #134217728") == 0);

    disassemble(buf, sizeof(buf), 0xe3a0b000);
    assert(strcmp(buf, "mov fp, #0") == 0);

    disassemble(buf, sizeof(buf), 0xe3a0b000);
    assert(strcmp(buf, "mov fp, #0") == 0);

    disassemble(buf, sizeof(buf), 0x21a0300a);
    assert(strcmp(buf, "movcs r3, sl") == 0);

    disassemble(buf, sizeof(buf), 0x01b01062);
    assert(strcmp(buf, "rrxseq r1, r2") == 0);

    disassemble(buf, sizeof(buf), 0x01b01162);
    assert(strcmp(buf, "rorseq r1, r2, #2") == 0);
  }

  // data processing
  {
    disassemble(buf, sizeof(buf), 0xe2855004);
    assert(strcmp(buf, "add r5, r5, #4") == 0);

    disassemble(buf, sizeof(buf), 0x00009b74);
    assert(strcmp(buf, "andeq r9, r0, r4, ror fp") == 0);

    disassemble(buf, sizeof(buf), 0x00109b78);
    assert(strcmp(buf, "andseq r9, r0, r8, ror fp") == 0);

    disassemble(buf, sizeof(buf), 0xe1530002);
    assert(strcmp(buf, "cmp r3, r2") == 0);

    disassemble(buf, sizeof(buf), 0x20200000);
    assert(strcmp(buf, "eorcs r0, r0, r0") == 0);

    disassemble(buf, sizeof(buf), 0xe3c2260e);
    assert(strcmp(buf, "bic r2, r2, #14680064") == 0);

    disassemble(buf, sizeof(buf), 0xe3822602);
    assert(strcmp(buf, "orr r2, r2, #2097152") == 0);

    disassemble(buf, sizeof(buf), 0xe064a00a);
    assert(strcmp(buf, "rsb sl, r4, sl") == 0);
  }

  // bx/teq
  {
    disassemble(buf, sizeof(buf), 0xe13fff1e);
    assert(strcmp(buf, "teq pc, lr, lsl pc") == 0);

    disassemble(buf, sizeof(buf), 0xe12fff1e);
    assert(strcmp(buf, "bx lr") == 0);
  }

  // ldrd
  {
    // disassemble(buf, sizeof(buf), 0x000098d4);
    // assert(strcmp(buf, "ldrdeq r9, [r0], -r4") == 0);
  }
}