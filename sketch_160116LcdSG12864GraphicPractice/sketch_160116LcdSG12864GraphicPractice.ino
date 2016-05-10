#include <mglcd.h>

const TLcdPinAssignTable PinAssignTable = {
  A0_DI  : 3, // A0 for SG12232, D/I for SG12864
  CS1_E1 : 13, // CS1
  CS2_E2 : 12, // CS2
E      : A4,
RW     : A2,
  DB0    : 8 ,
  DB1    : 9 ,
  DB2    : 10,
  DB3    : 11,
  DB4    : 4 ,
  DB5    : 5 ,
  DB6    : 6 ,
  DB7    : 7
}; // PinAssignTable;

static mglcd_SG12864 MGLCD(PinAssignTable);

void setup() {
  while (MGLCD.Reset());
} // setup

void loop() {
  MGLCD.ClearScreen();
  for (int i = 0; i < 63; i += 7) {
    delay(100);
    MGLCD.Line(i, 0, 63, i);
    delay(100);
    MGLCD.Line(63, i, 63 - i, 63);
    delay(100);
    MGLCD.Circle(63 +  i, 31, 30);
  }
} // loop
