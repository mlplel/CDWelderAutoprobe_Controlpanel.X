/**
 precess.c

  @Company
    

  @File Name
    precess.c

  @Summary
 low priority timed loops

  @Description
    processes operations.
    
*/


#include "process.h"
#include "mcc_generated_files/pin_manager.h"
#include "stdbool.h"
#include "i2c.h"


static SWSTATE swstate = SW_UP;
static SWEVENT swevent = SW_NOEVENT;
static int16_t encvalue = 0;


void run100us(){
    
    updateEncoder();
    
}



void run1ms(){
    static uint16_t longtime = 0;
    static int count = 0;
    
    updateSwitch();
    
    
    //test
    longtime++;
    if(longtime == 2000){
        display_Value(count);
        count++;
        longtime = 0;
        
    }
   
    
    
    static int16_t oldencvalue = 0;
    
    if(oldencvalue != encvalue){
        LED1_Toggle();
        oldencvalue = encvalue;
    }
    
    
    
    
}

SWEVENT getSwitchEvent(){
    SWEVENT event = swevent;
    swevent = SW_NOEVENT;
    return event;
}


void updateSwitch(void){
    const uint16_t DOUBLECLICKTIME = 300;
    static uint8_t swinputreg = 0xFF;
    static uint16_t downtime = 0;
    static uint16_t uptime = 0;
    static uint8_t firstclick = false;   
    
    
     // debounce switch
    swinputreg = swinputreg << 1;
    if (SW_GetValue()) {
        swinputreg = swinputreg | 1;
    } else {
        swinputreg = swinputreg & 0xFE;
    }


    if (swinputreg == 0xFF) {
        switch (swstate) {
            case SW_UP:
                if (uptime == 0xFFFF) return;
                uptime++;
                if((firstclick == true) && (uptime > DOUBLECLICKTIME)){
                    swevent = SW_CLICKED;
                    firstclick = false;
                }
                return;
            case SW_DOWN:
                if (downtime < DOUBLECLICKTIME) {
                    if(firstclick == true){
                        swevent = SW_DCLICKED;
                        firstclick = false;
                    } else {
                        firstclick = true;
                    }
                } else if (downtime < 1000) {
                    if(firstclick == true){
                        swevent = SW_DCLICKED;
                    } else {
                        swevent = SW_CLICKED;
                    }
                } else if (downtime < 2000) {
                    swevent = SW_LCLICKED;
                } else {
                    swevent = SW_ELCLICKED;
                }
                swstate = SW_UP;
                downtime = 0;
            default:
                return;
        } 

    } else if (swinputreg == 0x00) {
        switch(swstate){
            case SW_UP:
                uptime = 0;
                swstate = SW_DOWN;
                return;
            case SW_DOWN:
                if(downtime == 0xFFFF) return;
                downtime++;           
            
            default:
                return;
        }
    } 
}

void updateEncoder(void){
    
    static uint8_t oldenc = 0;
    
    uint8_t enc = ENCB_GetValue() << 1 | ENCA_GetValue();
        
    if(enc == oldenc) return;
    
    if(oldenc == 0){
        if(enc == 0x01){
            encvalue++;
        } else if(enc == 0x02){
            encvalue--;
        }            
    }
    oldenc = enc;    
    
    
}