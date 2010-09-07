IMPLEMENTATION[ia32]:

#include "io.h"
#include "processor.h"

// reset PC
void __attribute__ ((noreturn))
pc_reset()
{
  // i8042: store the next byte at port 0x60 as command byte
  while (Io::in8 (0x64) & 0x2)
    ;
  Io::out8_p (0x60, 0x64);

  // i8042 command byte (PS/2-compatible mode):
  //   b0=0 ... no IRQ 1 is generated when data available in input buffer
  //   b1=0 ... no IRQ 1 is generated when mouse data available in input buffer
  //   b2=1 ... set SYS flag in status register -- tells POST to perform
  //            "warm boot" tests/initiailization
  //   b3=0 ... reserved
  //   b4=0 ... keyboard interface enabled
  //   b5=0 ... auxillary PS/2 device (mouse) interface enabled
  //   b6=0 ... translation disabled -- data appears at input buffer exactly
  //            as read from keyboard
  //   b7=0 ... reserved
  while (Io::in8 (0x64) & 0x2)
    ;
  Io::out8_p (0x4, 0x60);

  // i8042: pulse output port with 1110b
  //   b0=0 ... reset computer
  //   b1=1 ... set gate A20
  //   b2=1 ... pull mouse data low
  //   b3=1 ... pull mouse clock low
  while (Io::in8 (0x64) & 0x2)
    ;
  Io::out8_p (0xfe,0x64);

  for (;;)
    Proc::pause();
}
