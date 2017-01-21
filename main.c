#include <stdlib.h>
//#include <pic16f628a.h>
//#include <pic.h>
#include <pic18.h>
#include <pic18f2550.h>

#include "max7219.h"

// 16f628
//#pragma config BOREN = 0FF
//#pragma config CP = 0FF
//#pragma config WDTE = 0FF
//#pragma config LVP = 0FF
//#pragma config FOSC = INTOSCIO

// 18f2550
#pragma config BOR = OFF
#pragma config MCLRE = ON
#pragma config LVP = OFF
#pragma config CP0 = OFF
#pragma config CP1 = OFF
#pragma config FOSC = INTOSC_EC
#pragma config WDT = OFF
#define _XTAL_FREQ 1000000

int main(int argc, char** argv) {
#if 0
  uint8_t current_char_0 = 0,
          current_char_1 = 0,
          current_char_2 = 0;
#endif

  (void) argc;  /* Ignored. */
  (void) argv;  /* Ignored. */

  //OSCF = 1;  /* 4Mhz oscillator. */

  /* Configure ports as inputs (1) or outputs(0). */
  TRISA = 0b00000001;
  TRISB = 0x0;

	ADCON1 = 0b00001110;//VSS,VDD ref. AN0 analog only
	ADCON0 = 0x00;//clear ADCON0 to select channel 0 (AN0)
	ADCON2 = 0b00001000;//ADCON2 setup: Left justified, Tacq=2Tad, Tad=2*Tosc (or Fosc/2)
	ADCON0bits.ADON = 0x01;//Enable A/D module
  ADCON0bits.CHS = 0;

  __delay_ms(250);

  maxDriverStartupInitialize();

  /* Set per-digit decode mode.
   * - If bit for the digit is 0, no decode is used.
   * - If bit for the digit is 1, code B is used
   */
  maxDriverSendData(MAX_ADDRESS_DECODE_MODE, 0xff);

  /* We're currently only 3 digits for the tests. */
  maxDriverSendData(MAX_ADDRESS_SCAN_LIMIT, MAX_LIMIT_DIGITS_012);

  /* Maximum intensity. */
  maxDriverSendData(MAX_ADDRESS_INTENSITY, 0x0f);

  /* Switch tp normal mode. */
  maxDriverSendData(MAX_ADDRESS_SHUTDOWN, MAX_MODE_NORMAL);

  for (;;) {
#if 0
    /* Test mode -- all LEDs are ON. */
    //maxDriverSendData(MAX_ADDRESS_DISPLAY_TEST, 0x01);

    maxDriverSendData(MAX_ADDRESS_DIGIT0, current_char_0);
    maxDriverSendData(MAX_ADDRESS_DIGIT1, current_char_1);
    maxDriverSendData(MAX_ADDRESS_DIGIT2, current_char_2);
    __delay_ms(250);
    maxDriverSendData(MAX_ADDRESS_DIGIT0, current_char_0 | MAX_CHAR_DOT_FLAG);
    __delay_ms(250);

    if (current_char_0 == 9) {
      current_char_0 = 0;
      if (current_char_1 == 9) {
        if (current_char_2 == 9) {
          current_char_2 = 0;
        } else {
          current_char_2++;
        }
        current_char_1 = 0;
      } else {
        current_char_1++;
      }
    } else {
      current_char_0++;
    }
#else
    ADCON0bits.GO = 1;
    while (ADCON0bits.GO_nDONE);
    int v = ADRESH;
    maxDriverSendData(MAX_ADDRESS_DIGIT0, (v % 10));
    maxDriverSendData(MAX_ADDRESS_DIGIT1, (v / 10) % 10);
    maxDriverSendData(MAX_ADDRESS_DIGIT2, (v / 100) % 10);
#endif
  }

  return EXIT_SUCCESS;
}
