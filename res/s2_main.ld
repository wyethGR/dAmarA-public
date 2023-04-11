/* @author Wyeth Greenlaw Rollins <wyethg@lclark.edu> <wyeth.gr@gmail.com> */

OUTPUT( tmp/s2c.elf )
OUTPUT_FORMAT( elf32-i386 )

SECTIONS {
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

