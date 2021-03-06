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
#use rs232(baud=19200,parity=N,UART1,bits=8)

