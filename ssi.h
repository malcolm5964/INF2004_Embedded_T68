#include "lwip/apps/httpd.h"
#include "pico/cyw43_arch.h"
#include "hardware/adc.h"

#include "driver/wheelEncoder/wheelEncoder.h"

// SSI tags - tag length limited to 8 bytes by default
//const char * ssi_tags[] = {"speedLeft","speedRight"};
//
//u16_t ssi_handler(int iIndex, char *pcInsert, int iInsertLen) {
//  size_t printed;   
//  switch (iIndex) {
//  case 0: // volt
//    {
//      printf("currentleft speed: %i\n", currentSpeedLeft);
//      const int speedLeft = 50;
//      printed = snprintf(pcInsert, iInsertLen, "%d", speedLeft);
//    }
//    break;
//  case 1: // temp
//    {
//      const int speedRight = currentSpeedRight;
//      printed = snprintf(pcInsert, iInsertLen, "%d", speedRight);
//    }
//    break;
//  default:
//    printed = 0;
//    break;
//  }
//  LWIP_ASSERT("sane length", printed <= 0xFFFF);
//  return (u16_t)printed;
//}
//
//// Initialise the SSI handler
//void ssi_init() {
//  http_set_ssi_handler(ssi_handler, ssi_tags, LWIP_ARRAYSIZE(ssi_tags));
//}

 const char * ssi_example_tags[] = {
 "hello",
 "a",
 "b"
 };


 u16_t ssi_handler(int iIndex, char *pcInsert, int iInsertLen) {
 size_t printed;
 switch (iIndex) {
      case 0: /* "Hello" */
         printed = snprintf(pcInsert, iInsertLen, "Hello user number %d!", rand());
         break;
      case 1: /* test */
         //printf("testest: %i\n", currentSpeedLeft);
         //int speed = currentSpeedLeft;
         printed = snprintf(pcInsert, iInsertLen, "%imm/s", 120);
         break;
      case 2: /* test */
         printed = snprintf(pcInsert, iInsertLen, "%imm/s", currentSpeedLeft);
         break;
     default: /* unknown tag */
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
