#include <Arduino.h>
#include <TM1637.h>

#define BV(bit)                 (1 << (bit))
#define SetBit(byte, bit)       (byte |= BV(bit))
#define ClearBit(byte, bit)     (byte &= ~BV(bit))
#define ToggleBit(byte, bit)    (byte ^= BV(bit))

#define DataIn      PB4             ///D12
#define DataReg     PORTB
#define ShiftCLK    PB0             ///D8
#define ShiftReg    PORTB
#define LatchCLK    PB3             ///D11
#define LatchReg    PORTB
#define disp0       PB5             ///D13
#define disp1       PC0             ///A0
#define disp2       PC1             ///A1
#define disp3       PC2             ///A2

enum direction {cw,ccw};
TM1637 tm(LatchCLK, DataIn);

volatile uint16_t DisplayValue = 124;
volatile direction Dir = cw;
volatile bool dirFlag0 = false;
volatile bool dirFlag1 = false;

ISR(INT0_vect) {
    if (dirFlag0) {
        DisplayValue++;
        dirFlag0 = false;

    }
    else {
        dirFlag1 = true;

    }
}

ISR(INT1_vect) {
    if (dirFlag1) {
        DisplayValue--;
        dirFlag1 = false;

    }
    else {
        dirFlag0 = true;

    }
}

void InitInterrupt() {
    //enable external interrupts 0 and 1
    SetBit(EIMSK, INT0);
    SetBit(EIMSK, INT1);

    SetBit(PORTD,PD3);
    SetBit(PORTD,PD2);

    //Falling edge triggers interrupt on int0 en int1
    SetBit(EICRA,ISC01);
    SetBit(EICRA,ISC11);

    sei();
}

void setup() {

    InitInterrupt();
    tm.begin(); //set display pinnen als output

    // Init outputs

    // start values


}

void loop() {

    tm.display(1234);

}