#include <16F1455.h>
#device ADC=10
#device PASS_STRINGS=IN_RAM

#FUSES NOWDT                    //No Watch Dog Timer
#FUSES NOMCLR                   //Master Clear pin used for I/O
#FUSES NOBROWNOUT               //No brownout reset
#FUSES NOLVP                    //No low voltage prgming, B3(PIC16) or B5(PIC18) used for I/O

#use delay(clock=48MHz,crystal=16MHz,USB_FULL)
#use rs232(baud=9600,parity=N,UART1,bits=8,errors)


#use FIXED_IO( C_outputs=PIN_C4,PIN_C3,PIN_C2,PIN_C1 )
#define RESET26k22       PIN_C1
#define LEDTX           PIN_C2
#define LEDRX           PIN_C3

#define USB_CONFIG_VID 0x0461
#define USB_CONFIG_PID 0x0003
#define USB_CONFIG_BUS_POWER 500
#define USB_STRINGS_OVERWRITTEN

char const USB_STRING_DESC[]={
   //string 0 - language
      4,  //length of string index
      0x03,  //descriptor type (STRING)
      0x09,0x04,  //Microsoft Defined for US-English
   //string 1 - manufacturer
      16,  //length of string index
      0x03,  //descriptor type (STRING)
      'B',0,
      'a',0,
      'b',0,
      'u',0,
      'I',0,
      'N',0,
      'O',0,
   //string 2 - product
      24,  //length of string index
      0x03,  //descriptor type (STRING)
      'B',0,
      'a',0,
      'b',0,
      'u',0,
      'I',0,
      'N',0,
      'O',0,
      ' ',0,
      'U',0,
      'N',0,
      'O',0
};

#include <usb_cdc.h>

void ATcommands(char);
void CheckUart(void);

void ATcommands(char c){
static char buffer[16]={"               "};

   buffer[14]=c;
   for(int i=0;i<14;i++){
      buffer[i]=buffer[i+1];
   }
   
   if(c=='\r'){
      if(strstr(buffer,"AT+BOOT\r")!=0){
         usb_cdc_putc("ACK\r\n");
         output_high(RESET26k22);
         delay_us(1);
         output_low(RESET26k22);
         delay_ms(10);
         putc(0x07);
         
      }
      else if(strstr(buffer,"AT+RST\r")!=0){
         usb_cdc_putc("ACK\r\n");
         output_high(RESET26k22);
         delay_us(1);
         output_low(RESET26k22);
         
      }
      else if(strstr(buffer,"AT+INFO\r")!=0){
         usb_cdc_putc("\r\n-----------------------\r\n");
         usb_cdc_putc("BOARD INFORMATION\r\n");
         usb_cdc_putc("BOARD: UNO r2\r\n");
         usb_cdc_putc("REVISION: 1.00\r\n");
         usb_cdc_putc("PROCESSOR: PIC 18F26k22\r\n");
         usb_cdc_putc("CRYSTAL: 16Mhz\r\n");
         usb_cdc_putc("CLOCK: 64Mhz\r\n");
         usb_cdc_putc("MIPS: 16\r\n");
         usb_cdc_putc("AD CHANNELS: 13\r\n");
         usb_cdc_putc("PWM: 5\r\n");
         usb_cdc_putc("DAC: 1(5 bits)\r\n");     
         usb_cdc_putc("-----------------------\r\n");
         
      }else if(strstr(buffer,"AT+IDN?\r")!=0){
         usb_cdc_putc("BabuINO UNO"); 
         
      }else if(strstr(buffer,"AT+BAUD")){
         usb_cdc_putc("Future feature\r\n");
      }
      buffer="               ";
   }
}

void CheckUart(void){
static int32 baud=9600;
static int parity=0;
static int dataBits=8;
static int stopBits=0;
   if(baud!=usb_cdc_line_coding.dwDTERrate){
      set_uart_speed(usb_cdc_line_coding.dwDTERrate);
      baud=usb_cdc_line_coding.dwDTERrate;
   }
  
}

