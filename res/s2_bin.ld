/* @author Wyeth Greenlaw Rollins <wyethg@lclark.edu> <wyeth.gr@gmail.com> */

INPUT( tmp/s2_entry.a.o tmp/s2c.elf )
OUTPUT( tmp/s2.elf )
OUTPUT_FORMAT( elf32-i386 )

SECTIONS {
  . = 0x7E00;

  .text : SUBALIGN(4) {
    *( .text.entry )
    *( .text* )
    *( .rodata* )
    *( .data )
  }

  /DISCARD/ : {
    *( .comment )
  }
}

