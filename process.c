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

#define ENCMINVAL   0
#define ENCMAXVAL   15


#include "process.h"
#include "mcc_generated_files/pin_manager.h"
#include "stdbool.h"
#include "i2c.h"
#include "displaydata.h"
#include "spi.h"

bool testmem(uint16_t addr, uint8_t* data);
void testmemdisplay(uint16_t addr, uint8_t data);
int testwritemem(void);


static SWSTATE swstate = SW_UP;
static SWEVENT swevent = SW_NOEVENT;
static int16_t encvalue = 1;
static PROCESSMODE processmode = MODE_INIT;


void run100us(){
    
    updateEncoder();    
}

/*
 *  main exacution loop run every 1 ms
 */
void run1ms(){
    static uint16_t longtime = 0;
    //static uint8_t data = 0;
    static uint16_t address = 100;
    //static uint8_t temp = 0;
    //static uint16_t alt = 0;
    static uint16_t displaycount = 0;
    static int mode = 0;
    
    updateSwitch();
    
    
    SWEVENT se = peekswitchevent();
    if(se == SW_CLICKED){
    // process switch clicked
    
    }
    else if(se == SW_DCLICKED){
        // process switch double clicked
   
    }
    else if(se == SW_LCLICKED){
        // process switch long clicked
      //testheader();  
     
    }
    else if(se == SW_ELCLICKED){
        //process switch extra long clicked
        calmode_init();
                
    }

    processMsg();
    
    if(displaycount == 100){
        displaycount = 0;
        displayupdate();
        //testmemdisplay(address, data);
    }
    displaycount++;
  
    //test
    
    if(longtime == 1000){
        if(mode == 1){
            address++;  
        }
        
        longtime = 0;
    }
    longtime++;
    
    
    // spi testing
    //if(mode == 0){
    //    mode = testwritemem();
        
    //} else {
    //   testmem(address, &data); 
    //}
      
    static int16_t oldencvalue = 0;
    
    if(oldencvalue != encvalue){
        LED1_Toggle();
        display_int(encvalue, 4);
        oldencvalue = encvalue;
        
    }  
   
}

SWEVENT getSwitchEvent(){
    SWEVENT event = swevent;
    swevent = SW_NOEVENT;
    return event;
}
SWEVENT peekswitchevent(){
    return swevent;
}

void clearswitchevent(){
    swevent = SW_NOEVENT; 
}

/*
 *  sets swevent to the last switch event
 * 
 */
void updateSwitch(void){
    const uint16_t DOUBLECLICKTIME = 300;
    static uint8_t swinputreg = 0xFF;
    static uint16_t downtime = 0;
    static uint16_t uptime = 0;
    static uint8_t firstclick = false;   
    
    
     // de-bounce switch
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
        if(enc == 0x01 && encvalue != ENCMAXVAL){
            encvalue++;
        } else if(enc == 0x02 && encvalue != ENCMINVAL){
            encvalue--;
        }            
    }
    oldenc = enc;     
}

void processMsg(){
    static int16_t msgerrcnt = 0;
    
    if(is_newmsg() == false) return;  
    MAINMSG msg = get_msg();
    if(msg.validf == false) return;

    switch (msg.command) {


        case CMD_poweron:
            put_msg(RPY_init);
            processmode = MODE_OPERATE;
            break;

        case CMD_testmode1:

            //testing
            if (getSwitchEvent() == SW_CLICKED) {

                MAINMSG m;
                
                m.command = RPO_probepressure;
                m.data1 = encvalue;
                m.data2 = encvalue;
                m.data3 = encvalue;

                put_msg(m);

            }
            break;
            
        case CMD_cal:
            // in calibrate and test mode
            processmode = MODE_CAL;
            LED4_Toggle();
            
            break;

        default:
            msgerrcnt++;
            break;
    }
    
    // test for too many errors.
    if(msgerrcnt > 5){
        // reset msg process here.
    }    
}

void displayupdate(){
    
    MAINMSG msg = getMsg();
    
    display_int(msg.data1, 2);
    display_int(msg.data2, 3);
    //display_int(encvalue, 4);    
    
    if((msg.data3 & 0x0040) == 0){
        LED1_SetLow();
    } else{
        LED1_SetHigh();
    }
    
    if((msg.data3 & 0x0020) == 0){
        LED2_SetLow();
    } else{
        LED2_SetHigh();
    }
    
    if((msg.data3 & 0x0010) == 0){
        LED3_SetLow();
    } else{
        LED3_SetHigh();
    }      
}

void testmemdisplay(uint16_t addr, uint8_t data){
    
    display_int(addr, 1);
    display_int(data, 3);
    
}

bool testmem(uint16_t addr, uint8_t* data){
    static int mode = 0;
    static uint16_t oldaddr = 0xFFFF;
    
    if(addr == oldaddr)
        return false;
    
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
                oldaddr = addr;
                mode = 0;
            }
            return true;
            break;
        
        
        
        default:
            break;
    }  
    return false;
}

int testwritemem(){
    static uint16_t addr = 100;
    static uint8_t value = 100;
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
            mem_writebyte(value, addr);
            mode = 0;
            if(value == 0){
                return 1;
            }
            addr++;
            value--;
            break;            
        
        default:
            break;            
    }
    return 0;
}


void testheader(){

    if (getSwitchEvent() == SW_LCLICKED) {
        display_header(sym_P1set, sym_blank, sym_blank);
    }
    
 
        
}

// switch to calibrate and test mode
void calmode_init() {

    if(put_msg(RPY_cal) == -1){
        return;
    };
    clearswitchevent();

}