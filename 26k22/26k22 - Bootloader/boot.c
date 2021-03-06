#include <boot.h>

#define _bootloader
//#define BOOTLOADER_MODE2X

#include <bootloader.h>
#include <loader.c>

#if defined(__PCM__)
 #org LOADER_END+1,LOADER_END+2
#elif defined(__PCH__)
 #org LOADER_END+2,LOADER_END+4
#endif
void application(void) {
  while(TRUE);
}

void main(void) {
char c=0x00;
int i=0;
   //if(kbhit())c=getc();
   while(c!=0x07){
      if(kbhit())c=getc();
      if(++i>40)break;
      delay_ms(1);
   }
   if(c==0x07)
   {
      printf("\r\nBootloader v1.04\r\n");
      printf("Aguardando programa...\r\n");
      putc(0x07);
   
      load_program();
   }

   application();
}

#int_global
void isr(void) {
   jump_to_isr(LOADER_END+5*(getenv("BITS_PER_INSTRUCTION")/8));
}
