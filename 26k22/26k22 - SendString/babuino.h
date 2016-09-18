#include <18F26K22.h>
#device ADC=10

#FUSES PRIMARY                  //Primary clock is system clock when scs=00
#FUSES FCMEN                    //Fail-safe clock monitor enabled
#FUSES IESO                     //Internal External Switch Over mode enabled
#FUSES PUT                      //Power Up Timer
#FUSES NOBROWNOUT               //No brownout reset
#FUSES BORV19                   //Brownout reset at 1.9V
#FUSES PBADEN                   //PORTB pins are configured as analog input channels on RESET
#FUSES CCP3B5                   //CCP3 input/output multiplexed with RB5
#FUSES HFOFST                   //High Frequency INTRC starts clocking CPU immediately
#FUSES TIMER3C0                 //Timer3 Clock In is on pin C0
#FUSES CCP2B5                   //CCP2 input/output multiplexed with RB5
#FUSES MCLR                     //Master Clear pin enabled
#FUSES STVREN                   //Stack full/underflow will cause reset
#FUSES NOLVP                    //No low voltage prgming, B3(PIC16) or B5(PIC18) used for I/O
#FUSES NOXINST                  //Extended set extension and Indexed Addressing mode disabled (Legacy mode)
#FUSES NOPROTECT                //Code not protected from reading
#FUSES NOCPB                    //No Boot Block code protection
#FUSES NOCPD                    //No EE protection
#FUSES NOWRT                    //Program memory not write protected
#FUSES NOWRTC                   //Configuration registers not write protected
#FUSES NOWRTB                   //Boot block not write protected
#FUSES NOWRTD                   //Data EEPROM not write protected
#FUSES NOEBTR                   //Memory not protected from table reads
#FUSES NOEBTRB                  //Boot block not protected from table reads


#use delay(clock=64MHz,crystal=16MHz)
#use rs232(baud=19200,parity=N,UART1,bits=8,stream=RS2USB)

#include <bootloader.h>

#define PIN_0              PIN_C7
#define PIN_1              PIN_C6
#define PIN_2              PIN_B5
#define PIN_3              PIN_B0
#define PIN_4              PIN_C2
#define PIN_5              PIN_C1
#define PIN_6              PIN_A4
#define PIN_7              PIN_C0
#define PIN_8              PIN_B6
#define PIN_9              PIN_B7
#define PIN_10             PIN_A5
#define PIN_11             PIN_C4
#define PIN_12             PIN_C5
#define PIN_13             PIN_C3

#define PIN_AN0            PIN_A1
#define PIN_AN1            PIN_A0
#define PIN_AN2            PIN_B4
#define PIN_AN3            PIN_B3   
#define PIN_AN4            PIN_B2
#define PIN_AN5            PIN_B1


