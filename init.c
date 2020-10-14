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
#include "i2c.h"
#include "spi.h" 
#include "process.h"
#include "menu.h"

#include <xc.h>

static uint16_t itime = 0;

static MENUSTATE menustate = {MID_TOP, {0,1,true}};


/*
 * 
 */
void init_start(void){
    i2c_Init();    
    spi_IOSetup();
    spi1_Init();
    spi2_Init();
}

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
             menu_init(menustate);
             process_init();
             return true;       // exit init loop
             break;
             
         default:
             break;
     }    
     return false;
 }