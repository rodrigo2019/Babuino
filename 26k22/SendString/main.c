#include <main.h>

void main()
{

 char c;
   printf("Neste teste todos caracteres digitados serao retornados, desta forma");
   printf(" completando todo percurso de comunicacao USB/RS232-RS232/USB\n\r");
   while(TRUE)
   {
      if(kbhit(PORT1)){
         c=getc();
         putc(c);
      }
   }
}