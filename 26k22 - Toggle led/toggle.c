#include <babuino.h>

#define DELAY        500

void main()
{
//int32 i=0;
unsigned long long int i=0; //or int32 i=0;
   while(TRUE)
   {

   output_toggle(PIN_13);
   printf("%3lu ",++i);
   
   delay_ms(DELAY);

      //TODO: User Code
   }

}
