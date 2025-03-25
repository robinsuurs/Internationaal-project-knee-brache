#include <Arduino.h>

#define BV(bit)                 (1 << (bit))
#define SetBit(byte, bit)       (byte |= BV(bit))
#define ClearBit(byte, bit)     (byte &= ~BV(bit))
#define ToggleBit(byte, bit)    (byte ^= BV(bit))

#define DataIn      PB4             ///D12
#define DataReg     PORTB
#define ShiftCLK    PB1             ///D9
#define ShiftReg    PORTB
#define LatchCLK    PB0             ///D8
#define LatchReg    PORTB
#define disp1       PB5             ///D13
#define disp2       PC0             ///A0
#define disp3       PC1             ///A1
#define disp4       PC2             ///A2


void setup() {


    // Init outputs
    SetBit(DDRB, DataIn);
    SetBit(DDRB, ShiftCLK);
    SetBit(DDRB, LatchCLK);
    SetBit(DDRB, disp1);
    SetBit(DDRC, disp2);
    SetBit(DDRC, disp3);
    SetBit(DDRC, disp4);

    // start values
    ClearBit(ShiftReg, ShiftCLK);
    ClearBit(LatchReg, LatchCLK);
    ClearBit(PORTB, disp1);
    ClearBit(PORTC, disp2);
    ClearBit(PORTC, disp3);
    ClearBit(PORTC, disp4);
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
            SetBit(PORTC, disp4);
            ClearBit(PORTB, disp1);
            ClearBit(PORTC, disp2);
            ClearBit(PORTC, disp3);
        break;

        case 2:
            SetBit(PORTC, disp3);
            ClearBit(PORTB, disp1);
            ClearBit(PORTC, disp2);
            ClearBit(PORTC, disp4);
        break;

        case 1:
            SetBit(PORTC, disp2);
            ClearBit(PORTB, disp1);
            ClearBit(PORTC, disp4);
            ClearBit(PORTC, disp3);
        break;

        case 0:
            SetBit(PORTB, disp1);
            ClearBit(PORTC, disp4);
            ClearBit(PORTC, disp2);
            ClearBit(PORTC, disp3);
        break;

        default:
            ClearBit(PORTB, disp1);
            ClearBit(PORTC, disp2);
            ClearBit(PORTC, disp3);
            ClearBit(PORTC, disp4);
        break;;
    }
}

void display(int data, int disp)
{

    ///array voor weergave getal tussen 0-9 op 7-segdisp. met als waarde 10 een leeg display
    unsigned char DECdisplay[11] = {0x03,0x9F,0x25,0x0D,0x99,0x49,0x41,0x1F,0x01,0x09,0xFF};
    send_data(DECdisplay[data]);
    send_enable(disp);


    // Toggle latchclk (geen delay nodig want: minimale puls van 13ns nodig, MAX CPU snelhied is 16Mhz dus 83ns puls AKA ruim binnen spec)
    ToggleBit(LatchReg, LatchCLK);
    ToggleBit(LatchReg, LatchCLK);

}
void DECdisplay_getal(uint16_t getal)
{
    for(int i=0; getal!=0; i++)
    {
        display(getal%10, i);
        getal = getal / 10;
    }

}

void loop() {
    DECdisplay_getal(12);
}