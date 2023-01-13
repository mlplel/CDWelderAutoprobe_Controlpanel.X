/*
    Copyright 2022 Michael Jacobs

    Permission is hereby granted, free of charge, to any person obtaining a 
    copy of this software and associated documentation files (the "Software"), 
    to deal in the Software without restriction, including without limitation 
    the rights to use, copy, modify, merge, publish, distribute, sublicense, 
    and/or sell copies of the Software, and to permit persons to whom the 
    Software is furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included 
    in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS 
    OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
    DEALINGS IN THE SOFTWARE.
*/

#include "init.h"
#include "display.h"
#include "i2c.h"
#include "spi.h" 
#include "process.h"
#include "menu.h"

#include <xc.h>


/*
 *  file scope function.
 */
bool readeeprom(uint16_t addr, uint8_t* data);
bool init_readsettings(void);
bool writeeeprom(uint16_t addr, uint8_t data);

#define EEPROMSTARTADDR 128;
#define EEPROMDATACOUNT 180;

static uint16_t itime = 0;

static MENUSTATE menustate = {
    MID_TOP,{0, 1, true},CAL_NONE,{true, 1},{true, 1}};
// init should fill this in from eeprom data
static PRESSET probl[15];
static PRESSET probr[15];


/*
 * 
 */
void init_start(void){
    dispmem_init();
    i2c_Init();    
    spi_IOSetup();
    spi1_Init();
    spi2_Init();  

}

/*
 *  called ever 1 ms at startup
 */
 bool init_loop(){
     static bool settingsf = false;
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
             
             break;
         case 3000:
             return true;  // exit init loop problem with eeprom read.
             break;
         default:
             if(settingsf == false){
                if(init_readsettings()){
                    settingsf = true;
                    return true;  //exit init loop
                }
             }
             break;
     }    
     return false;
 }
 
 /*
  * 
  */
 bool init_readsettings(){    
     
     static uint16_t count = 0;
     static uint16_t mode = 0;
     static uint8_t lbyte;
     static uint8_t hbyte;
     static uint16_t addr = EEPROMSTARTADDR;
     static uint16_t idx = 0;
     static PRESSET* pa = &probl[0];
     static uint16_t position = 0;
     uint16_t value;
    if (count < 180) {
        if (mode == 0) {
            if(!readeeprom(addr, &lbyte))
                return false;
            addr++;
            mode = 1;           
            return false;
        } else {
            if(!readeeprom(addr, &hbyte))
                return false;
            addr++;
            mode = 0;
            value = ((uint16_t)hbyte * 256) + lbyte;
            count++;            
            switch(position){
                case 0:
                    (pa + idx)->pressure = value;
                    position++;
                    break;
                case 1:
                    (pa + idx)->kp = value;
                    position++;
                    break;
                case 2:
                    (pa + idx)->ki = value;
                    position++;
                    break;
                case 3:
                    (pa + idx)->kd = value;
                    position++;
                    break;
                case 4:
                    (pa + idx)->imax = value;
                    position++;
                    break;
                case 5:
                    (pa + idx)->outlimit = value;
                    position = 0;
                    if(idx == 14){
                        pa = &probr[0];
                        idx = 0;
                    } else {
                        idx++;
                    }                    
                default:
                    break;                    
            }
            return false;
        }
    }
    else {
        count = 0;
        mode = 0;
        addr = EEPROMSTARTADDR;
        idx = 0;
        pa = &probl[0];
        position = 0;
        return true;
    }
 }
 
 
 /*
  * 
  */
 bool init_writesettings() {
    static uint16_t count = 0;
    static uint16_t mode = 0;
    static uint8_t lbyte;
    static uint8_t hbyte;
    static uint16_t addr = EEPROMSTARTADDR;
    static uint16_t idx = 0;
    static PRESSET* pa = &probl[0];
    static uint16_t position = 0;
    uint16_t value;
    if (count < 180) {
        if (mode == 0) {
            switch (position) {
                case 0:
                    value = (pa + idx)->pressure;
                    position++;
                    break;
                case 1:
                    value = (pa + idx)->kp;
                    position++;
                    break;
                case 2:
                    value = (pa + idx)->ki;
                    position++;
                    break;
                case 3:
                    value = (pa + idx)->kd;
                    position++;
                    break;
                case 4:
                    value = (pa + idx)->imax;
                    position++;
                    break;
                case 5:
                    value = (pa + idx)->outlimit;
                    position = 0;
                    if (idx == 14) {
                        pa = &probr[0];
                        idx = 0;
                    } else {
                        idx++;
                    }
                default:
                    break;
            }
            lbyte = value & 0x00FF;
            hbyte = value >> 8; 
            mode = 1;
        }
        if(mode == 1){
            if(!writeeeprom(addr, lbyte))
                return false;
            addr++;
            mode = 2;           
            return false;
        } else if(mode == 2) {
            if(!writeeeprom(addr, hbyte))
                return false;
            addr++;
            mode = 0;
            value = ((uint16_t)hbyte * 256) + lbyte;
            count++; 
            return false;        
        }
        return false;
    }
    else {
        count = 0;
        mode = 0;
        addr = EEPROMSTARTADDR;
        idx = 0;
        pa = &probl[0];
        position = 0;
        return true;
    }
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
 
 /*
  * 
  */
 void init_setprobeitem(uint16_t pv, PROBENUMBER p, CALITEM item, uint16_t data){
     PRESSET* pres;
     if((pv < 1) || (pv > 15)) return;  // error 
     if(p == PL) pres = &probl[0];
     else pres = &probr[0];
     
     switch(item){
         case CAL_PRESSURE:
             (pres + (pv - 1))->pressure = data;
             break;
         case CAL_KP:
             (pres + (pv - 1))->kp = data;
             break;
         case CAL_KI:
             (pres + (pv - 1))->ki = data;
             break;
         case CAL_KD:
             (pres + (pv - 1))->kd = data;
             break;
         case CAL_ILIMIT:
             (pres + (pv - 1))->imax = data;
             break;
         case CAL_OLIMIT:
             (pres + (pv - 1))->outlimit = data;
             break;             
         default:
             break;
     }
 }
 
 
 /*
  * 
  */
 uint16_t init_getprobeitem(uint16_t pv, PROBENUMBER p, CALITEM item){
     PRESSET* pres;
     if((pv < 1) || (pv > 15)) return 0;
     if(p == PL) pres = &probl[0];
     else pres = &probr[0];
     
     switch(item){
           case CAL_PRESSURE:
             return (pres + (pv - 1))->pressure;
             break;
         case CAL_KP:
             return (pres + (pv - 1))->kp;
             break;
         case CAL_KI:
             return (pres + (pv - 1))->ki;
             break;
         case CAL_KD:
             return (pres + (pv - 1))->kd;
             break;
         case CAL_ILIMIT:
             return (pres + (pv - 1))->imax;
             break;
         case CAL_OLIMIT:
             return (pres + (pv - 1))->outlimit;
             break; 
         default:
             return 0;
             break;
     }
 }
 
 
 /*
  *   
  */
 bool readeeprom(uint16_t addr, uint8_t* data){
    static int mode = 0;
    
    switch(mode){        
        case 0:
            if(mem_getreadstatus() == UNKNOWN){
                mode = 1;
            }            
            break;
        case 1:
            mem_readbyteaddr(addr);
            mode = 2;
            break;
        case 2:
            if(mem_getreadstatus() == READY){
                *data = mem_readbyte();
                mode = 0;
            }
            return true;
            break;
        default:
            break;
    }  
    return false;
}

/*
 * 
 */ 
 bool writeeeprom(uint16_t addr, uint8_t data){ 
    static uint16_t mode = 0;
    EESTATUS estat;
    switch(mode){        
        case 0:
            estat = mem_getwritestatus();
            if(estat == WPON){
                mode = 1;
            } else if(estat == READY){
                mode = 2;
            }           
            break;
        case 1:
            mem_writeenable();
            mode = 0;
            break;
        case 2:
            mem_writebyte(data, addr);
            mode = 0;
            return true;
            break;            
        
        default:
            break;            
    }
    return false;
}

 