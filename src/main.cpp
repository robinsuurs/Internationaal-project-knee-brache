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

void Init_ADC(void)
{
    SetBit(ADMUX, REFS0);       //referentie voltage VCC
    SetBit(ADMUX, ADLAR);       //Left adjust ADC register
    ADCSRA |= BV(ADPS2) | BV(ADPS1); //prescale van 64 dus frequentie van 187Hz
    SetBit(ADCSRA, ADEN);           //enable ADC

}

  

void setup() {

    Init_ADC();
    
    tm.begin(); //set display pinnen als output
    // Init outputs

    // start values


}

void loop() {
    uint8_t ADCVal[3];

    for(int channel=0; channel<3; channel++) {
        ADMUX = (0xf0 & ADMUX) | (channel+5);
        SetBit(ADCSRA, ADSC);
        loop_until_bit_is_clear(ADCSRA, ADSC);
        ADCVal[channel]= ADCH;
    }

     DECdisplay_getal(ADCVal[0]);
    _delay_ms(500);
    DECdisplay_getal(ADCVal[1]);
    _delay_ms(500);
    DECdisplay_getal(ADCVal[2]);
    _delay_ms(500);

}