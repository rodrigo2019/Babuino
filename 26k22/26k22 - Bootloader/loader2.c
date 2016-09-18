///////////////////////////////////////////////////////////////////////////
////                         loader.c                                  ////
////                                                                   ////
//// This driver will take an Intel 8-bit Hex file over RS232 channels ////
//// and modify the flash program memory with the new code.  A proxy   ////
//// function is required to sit between the real loading function     ////
//// and the main code because the #org preprocessor command could     ////
//// possibly change the order of functions within a defined block.    ////
////                                                                   ////
//// After each good line, the loader sends an ACKLOD character.  The  ////
//// driver uses XON/XOFF flow control.  Also, any buffer on the PC    ////
//// UART must be turned off, or to its lowest setting, otherwise it   ////
//// will miss data.                                                   ////
////                                                                   ////
////                                                                   ////
///////////////////////////////////////////////////////////////////////////
////        (C) Copyright 1996,2014 Custom Computer Services           ////
//// This source code may only be used by licensed users of the CCS C  ////
//// compiler.  This source code may only be distributed to other      ////
//// licensed users of the CCS C compiler.  No other use, reproduction ////
//// or distribution is permitted without written permission.          ////
//// Derivative programs created using this software in object code    ////
//// form are not restricted in any way.                               ////
///////////////////////////////////////////////////////////////////////////

#ifndef LOADER_END
 #define LOADER_END      getenv("PROGRAM_MEMORY")-1

 #if defined(__PCM__)
  #define LOADER_SIZE   0x27F
 #elif defined(__PCH__)
  #if (getenv("PROGRAM_MEMORY") % getenv("FLASH_ERASE_SIZE")) == 0
   #define LOADER_SIZE   0x3FF
  #else
   #define LOADER_SIZE   (getenv("PROGRAM_MEMORY") % (getenv("FLASH_ERASE_SIZE")) - 1)
  #endif
 #endif
#endif

#define LOADER_ADDR LOADER_END-LOADER_SIZE

#ifndef BOOTLOADER_AT_START
 #ORG LOADER_ADDR+4, LOADER_END auto=0 default
#endif

#ifndef BUFFER_LEN_LOD
 #define BUFFER_LEN_LOD    64
#endif

#ifdef BOOTLOADER_MODE2X
 #define BUFFER_COUNT   2
#else
 #define BUFFER_COUNT   1
#endif

#if defined(__PCM__) && !getenv("ENH16") && (BUFFER_COUNT == 2)
 struct
 {
   int8 idx;
   char *buffer;
 } rBuffer[BUFFER_COUNT];

 char Buffer1[BUFFER_LEN_LOD];
 char Buffer2[BUFFER_LEN_LOD];
#else
 struct
 {
   int8 idx;
   char buffer[BUFFER_LEN_LOD];
 } rBuffer[BUFFER_COUNT];
#endif

#define ACKLOD 0x06
#define XON    0x11
#define XOFF   0x13

unsigned int atoi_b16(char *s);

#ifdef BOOTLOADER_STREAM
 #define BootLoaderGetc()     fgetc(BOOTLOADER_STREAM)
 #define BootLoaderPutc(c)    fputc(c, BOOTLOADER_STREAM)
#else
 #define BootLoaderGetc()     getc()
 #define BootLoaderPutc(c)    putc(c)
#endif

void real_load_program (void)
{
   int1  do_ACKLOD, done=FALSE;
   int8  checksum, line_type;
   int16 l_addr,h_addr=0;
   int32 addr;
  #if getenv("FLASH_ERASE_SIZE") != getenv("FLASH_WRITE_SIZE")
   int32 next_addr;
  #endif
   int8  dataidx, i, count;
   int8  data[BUFFER_LEN_LOD / 2];
  #ifdef BOOTLOADER_MODE2X
   int8  buffidx;
  #endif
   
  #if defined(__PCM__) && !getenv("ENH16") && (BUFFER_COUNT == 2)
   rBuffer[0].buffer = &Buffer1[0];
   rBuffer[1].buffer = &Buffer2[0];
  #endif
   
  #ifdef BOOTLOADER_MODE2X
   buffidx = 0;

   while (!done)  // Loop until the entire program is downloaded
   {
      rBuffer[buffidx].idx = 0;     // Read into the buffer until 0x0D ('\r') is received or the buffer is full
      
      do {
         rBuffer[buffidx].buffer[rBuffer[buffidx].idx] = BootLoaderGetc();
         
      } while ((rBuffer[buffidx].buffer[rBuffer[buffidx].idx++] != 0x0D) && (rBuffer[buffidx].idx <= BUFFER_LEN_LOD));
      
      if(++buffidx >= BUFFER_COUNT)
      {
  #else
   while (!done)  // Loop until the entire program is downloaded
   {
      rBuffer[0].idx = 0;
      
      do {
         rBuffer[0].buffer[rBuffer[0].idx] = BootLoaderGetc();
         
      } while ((rBuffer[0].buffer[rBuffer[0].idx++] != 0x0D) && (rBuffer[0].idx <= BUFFER_LEN_LOD));
  #endif
  
         BootLoaderPutc(XOFF);  // Suspend sender
   
         do_ACKLOD = TRUE;
         
        #ifdef BOOTLOADER_MODE2X
         if(rBuffer[0].idx != rBuffer[1].idx)
            do_ACKLOD = FALSE;
         else
         {
            for(i=0;i<(rBuffer[0].idx-1);i++)
            {
               if(rBuffer[0].buffer[i] != rBuffer[1].buffer[i])
               {
                  do_ACKLOD = FALSE;
                  break;
               }
            }
            
            if(do_ACKLOD)
            {
        #endif   
               // Only process data blocks that start with ':'
               if (rBuffer[0].buffer[0] == ':')
               {
                  count = atoi_b16 (&rBuffer[0].buffer[1]);  // Get the number of bytes from the buffer
         
                  // Get the lower 16 bits of address
                  l_addr = make16(atoi_b16(&rBuffer[0].buffer[3]),atoi_b16(&rBuffer[0].buffer[5]));
         
                  line_type = atoi_b16 (&rBuffer[0].buffer[7]);
         
                  addr = make32(h_addr,l_addr);
         
                 #if defined(__PCM__)  // PIC16 uses word addresses
                  addr /= 2;
                 #endif
                  
                  checksum = 0;  // Sum the bytes to find the check sum value
                  for (i=1; i<(rBuffer[0].idx-3); i+=2)
                     checksum += atoi_b16 (&rBuffer[0].buffer[i]);
                  checksum = 0xFF - checksum + 1;
         
                  if (checksum != atoi_b16 (&rBuffer[0].buffer[rBuffer[0].idx-3]))
                     do_ACKLOD = FALSE;
                  else
                  {
                     // If the line type is 1, then data is done being sent
                     if (line_type == 1)
                        done = TRUE;
                     else if (line_type == 4)
                        h_addr = make16(atoi_b16(&rBuffer[0].buffer[9]), atoi_b16(&rBuffer[0].buffer[11]));
                     else if (line_type == 0)
                     {
                        if ((addr < LOADER_ADDR || addr > LOADER_END) && addr < getenv("PROGRAM_MEMORY"))
                        {
                           // Loops through all of the data and stores it in data
                           // The last 2 bytes are the check sum, hence buffidx-3
                           for (i = 9,dataidx=0; i < rBuffer[0].idx-3; i += 2)
                              data[dataidx++]=atoi_b16(&rBuffer[0].buffer[i]);
         
                           #if getenv("FLASH_ERASE_SIZE") > getenv("FLASH_WRITE_SIZE")
                              if ((addr!=next_addr) && (addr > (next_addr + (getenv("FLASH_ERASE_SIZE") - (next_addr % getenv("FLASH_ERASE_SIZE"))))) && ((addr & (getenv("FLASH_ERASE_SIZE")-1)) != 0))
                              {
                                 #if defined(__PCH__) && defined(BOOTLOADER_AT_START)
                                    #if ((getenv("PROGRAM_MEMORY") % getenv("FLASH_ERASE_SIZE")) != 0)
                                       if (addr > (getenv("PROGRAM_MEMORY") - (getenv("PROGRAM_MEMORY") % getenv("FLASH_ERASE_SIZE"))))
                                       {
                                          read_program_memory(getenv("PROGRAM_MEMORY"), rBuffer[0].buffer, getenv("FLASH_ERASE_SIZE") - (getenv("PROGRAM_MEMORY") % getenv("FLASH_ERASE_SIZE")));
                                          erase_program_eeprom(addr);
                                          write_program_memory(getenv("PROGRAM_MEMORY"), rBuffer[0].buffer, getenv("FLASH_ERASE_SIZE") - (getenv("PROGRAM_MEMORY") % getenv("FLASH_ERASE_SIZE")));
                                       }
                                       else
                                    #endif
                                 #endif
                                          erase_program_eeprom(addr);
                              }
                              #if defined(__PCM__)
                                 next_addr = addr + count/2;
                              #else
                                 next_addr = addr + count;
                              #endif
                           #endif
                           
                           #if defined(__PCH__) && defined(BOOTLOADER_AT_START)
                              #if ((getenv("PROGRAM_MEMORY") % getenv("FLASH_ERASE_SIZE")) != 0)
                                 if (addr == (getenv("PROGRAM_MEMORY") - (getenv("PROGRAM_MEMORY") % getenv("FLASH_ERASE_SIZE"))))
                                 {
                                    read_program_memory(getenv("PROGRAM_MEMORY"), rBuffer[0].buffer, getenv("FLASH_ERASE_SIZE") - (getenv("PROGRAM_MEMORY") % getenv("FLASH_ERASE_SIZE")));
                                    write_program_memory(addr, data, count);
                                    write_program_memory(getenv("PROGRAM_MEMORY"), rBuffer[0].buffer, getenv("FLASH_ERASE_SIZE") - (getenv("PROGRAM_MEMORY") % getenv("FLASH_ERASE_SIZE")));
                                 }
                                 else
                              #endif
                           #endif
                                    write_program_memory(addr, data, count);
                        }
                     }
                  }
               }
        #ifdef BOOTLOADER_MODE2X
            }
         }
         
         buffidx = 0;
        #endif

         if (do_ACKLOD)
            BootLoaderPutc(ACKLOD);
         
         BootLoaderPutc(XON);
         
     #ifdef BOOTLOADER_MODE2X
      } 
     #endif
   }

   BootLoaderPutc(ACKLOD);
   BootLoaderPutc(XON);

   reset_cpu();
}

unsigned int atoi_b16(char *s) {  // Convert two hex characters to a int8
   unsigned int result = 0;
   int i;

   for (i=0; i<2; i++,s++)  {
      if (*s >= 'A')
         result = 16*result + (*s) - 'A' + 10;
      else
         result = 16*result + (*s) - '0';
   }

   return(result);
}

#ifndef BOOTLOADER_AT_START
 #ORG default
 #ORG LOADER_ADDR, LOADER_ADDR+3
#endif
void load_program(void)
{
   real_load_program();
}
