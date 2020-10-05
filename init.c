/*
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 */

#include "init.h"
#include "display.h"
#include <xc.h>

static uint16_t itime = 0;
/*
 *  called ever 1 ms at startup
 */
 bool init_loop(){
     itime++; 
     
     switch(itime){
         
         case 100:
             sh1106_setup();
             sh1106_clear();
             break;
             
         case 200:
             sh1106_on();
             break;
             
         case 350:
             return true;       // exit init loop
             break;
             
         default:
             break;
     }    
     return false;
 }