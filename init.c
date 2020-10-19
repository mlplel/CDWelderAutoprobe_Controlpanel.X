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

static MENUSTATE menustate = {
    MID_TOP,{0, 1, true},CAL_NONE,
    {true, 1,{2400, 110, 20, 0, 1000, 800, false}},
    {true, 1,{2400, 110, 20, 0, 1000, 800, false}}};
// init should fill this in from eeprom data
static PRESSET probl[15];
static PRESSET probr[15];



/*
 * 
 */
void init_start(void){
    i2c_Init();    
    spi_IOSetup();
    spi1_Init();
    spi2_Init();
    
    // testing
    int i;
    for(i = 0;i<15;i++){
        probl[i].pressure = 2400 + 600 * i;
        probl[i].kp = 110;
        probl[i].ki = 20;
        probl[i].kd = 0;
        probl[i].imax = 1000;
        probl[i].outlimit = 800;
        
        probr[i].pressure = 2400 + 600 * i;
        probr[i].kp = 110;
        probr[i].ki = 20;
        probr[i].kd = 0;
        probr[i].imax = 1000;
        probr[i].outlimit = 800;        
    }
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
             //process_init();
             return true;       // exit init loop
             break;
             
         default:
             break;
     }    
     return false;
 }
 
 
 /*
  * 
  */
 PRESSET init_getprobe(uint16_t i,  PROBENUMBER p){
     PRESSET ps;
     if(p == PL){
         ps = probl[i-1];
     } else {
         ps = probr[i-1];
     }
     return ps;
 }
 
 /*
  * 
  */
 void init_setprobe(uint16_t i, PROBENUMBER p, PRESSET ps){     
     if(p == PL){
         probl[i-1] = ps;         
     } else {
         probr[i-1] = ps;         
     }
 }