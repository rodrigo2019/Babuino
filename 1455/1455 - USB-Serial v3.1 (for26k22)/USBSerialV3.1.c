#include <USBSerialV3.1.h>
#ORG 0x1F7F, 0x1FFF {}
#ID 0x0001

/* TODO: Use usb_cdc_putc() to transmit data to the USB
virtual COM port. Use usb_cdc_kbhit() and usb_cdc_getc() to
receive data from the USB virtual COM port. usb_enumerated()
can be used to see if connected to a host and ready to
communicate. */

#INT_TIMER1
void  TIMER1_isr(void) 
{
output_low(LEDRX);
output_low(LEDTX);
}

void main()
{
char cUSB;

   set_tris_a(0xFF);
   set_tris_c(0xE3);
   output_low(RESET26k22);
   
   setup_timer_1(T1_INTERNAL|T1_DIV_BY_4);      //21,8 ms
   
   enable_interrupts(INT_TIMER1);
   enable_interrupts(GLOBAL);
   
   usb_init();
   
   while(TRUE)
   {
      CheckUart();   
      if(usb_cdc_kbhit()){
         output_high(LEDRX);
         cUSB=usb_cdc_getc();
         putc(cUSB);     
         ATcommands(cUSB);
      }  
      if(kbhit()){
         output_high(LEDTX);
         usb_cdc_putc(getc());
      }
      //TODO: User Code
   }
}
