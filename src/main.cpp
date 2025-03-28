#include <Arduino.h>

#define BV(bit)                 (1 << (bit))
#define SetBit(byte, bit)       (byte |= BV(bit))
#define ClearBit(byte, bit)     (byte &= ~BV(bit))
#define ToggleBit(byte, bit)    (byte ^= BV(bit))

#define DataIn      PB4             ///D12
#define DataReg     PORTB
#define ShiftCLK    PB0             ///D8
#define ShiftReg    PORTB
#define LatchCLK    PB1             ///D9
#define LatchReg    PORTB
#define disp0       PB5             ///D13
#define disp1       PC0             ///A0
#define disp2       PC1             ///A1
#define disp3       PC2             ///A2

enum direction {cw,ccw};

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

void send_data(unsigned char data)
{
    // Herhaal voor alle bits in een char
    for(int i=0; i<8; i++)
    {
        // Bepaal de waarde van de bit die je naar het schuifregister
        // wil sturen
        if (bit_is_set(data, i))
        {
            SetBit(DataReg, DataIn);
        }
        else
        {
            ClearBit(DataReg, DataIn);
        }


        ToggleBit(ShiftReg, ShiftCLK);
        ToggleBit(ShiftReg, ShiftCLK);


    }
}

void send_enable(int Displaynr)
{
    switch (Displaynr)
    {
        case 3:
            ClearBit(PORTC, disp3);
            SetBit(PORTB, disp0);
            SetBit(PORTC, disp1);
            SetBit(PORTC, disp2);
        break;

        case 2:
            ClearBit(PORTC, disp2);
            SetBit(PORTB, disp0);
            SetBit(PORTC, disp1);
            SetBit(PORTC, disp3);
        break;

        case 1:
            ClearBit(PORTC, disp1);
            SetBit(PORTB, disp0);
            SetBit(PORTC, disp3);
            SetBit(PORTC, disp2);
        break;

        case 0:
            ClearBit(PORTB, disp0);
            SetBit(PORTC, disp3);
            SetBit(PORTC, disp1);
            SetBit(PORTC, disp2);
        break;

        default:
            ClearBit(PORTB, disp0);
            ClearBit(PORTC, disp1);
            ClearBit(PORTC, disp2);
            ClearBit(PORTC, disp3);
        break;;
    }
}

void display(int data, int disp)
{

    ///array voor weergave getal tussen 0-9 op 7-segdisp. met als waarde 10 een leeg display
    unsigned char DECdisplay[11] = {0xFC,0x60,0xDA,0xF2,0x66,0xB6,0xBF,0xE0,0xFF,0xF6,0x00};
    send_data(DECdisplay[data]);
    send_enable(disp);


    // Toggle latchclk (geen delay nodig want: minimale puls van 13ns nodig, MAX CPU snelhied is 16Mhz dus 83ns puls AKA ruim binnen spec)
    ToggleBit(LatchReg, LatchCLK);
    ToggleBit(LatchReg, LatchCLK);

}
void DECdisplay_getal(uint16_t getal)
{
    for(int i=3; getal!=0; i--)
    {
        display(getal%10, i);
        getal = getal / 10;
    }

}
void setup() {

    InitInterrupt();

    // Init outputs
    SetBit(DDRB, DataIn);
    SetBit(DDRB, ShiftCLK);
    SetBit(DDRB, LatchCLK);
    SetBit(DDRB, disp0);
    SetBit(DDRC, disp1);
    SetBit(DDRC, disp2);
    SetBit(DDRC, disp3);

    // start values
    ClearBit(ShiftReg, ShiftCLK);
    ClearBit(LatchReg, LatchCLK);
    ClearBit(PORTB, disp0);
    ClearBit(PORTC, disp1);
    ClearBit(PORTC, disp2);
    ClearBit(PORTC, disp3);


}

void loop() {

    DECdisplay_getal(DisplayValue);

}