#include "lwip/apps/httpd.h"
#include "pico/cyw43_arch.h"
#include "hardware/adc.h"

#include "driver/wheelEncoder/wheelEncoder.h"
#include "driver/barcode/barcode.h"
#include "driver/mapping/mapping.h"

// SSI tags - tag length limited to 8 bytes by default
 const char * ssi_example_tags[] = {
 "a",
 "b",
 "c",
 "d"
 };


 u16_t ssi_handler(int iIndex, char *pcInsert, int iInsertLen) {
   char buffer[1024];
   size_t printed;

   switch (iIndex) {
         case 0: 
            printed = snprintf(pcInsert, iInsertLen, "%s", mazeHTML3);
            break;
         case 1: 
            printed = snprintf(pcInsert, iInsertLen, "%s", mazeHTML4);
            break;
         case 2: 
            printed = snprintf(pcInsert, iInsertLen, "%s", mazeHTML1);
            break;
         case 3: 
            printed = snprintf(pcInsert, iInsertLen, "%s", mazeHTML2);
            break;
      default:
            printed = 0;
            break;
   }
   LWIP_ASSERT("sane length", printed <= 0xFFFF);
   return (u16_t)printed;
}

//// Initialise the SSI handler
void ssi_init() {
  const size_t tags_number = LWIP_ARRAYSIZE(ssi_example_tags);
  http_set_ssi_handler(ssi_handler, ssi_example_tags, tags_number);
}
