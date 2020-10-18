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

#define ENCMINVAL   1
#define ENCMAXVAL   15


#include "process.h"
#include "mcc_generated_files/pin_manager.h"
#include "stdbool.h"
#include "i2c.h"
#include "displaydata.h"
#include "spi.h"
#include "messages.h"
#include "display.h"
#include "menu.h"
#include "init.h"

/*
 *   function only called from this file.
 */
bool processswitch(void);
bool config_motor(void);
bool rpyqueue(uint16_t rpycount);
void process_probemsg(MENUEVENT me);

static PROC_STAT status = {false,false,false,false,false,false};

bool testmem(uint16_t addr, uint8_t* data);
void testmemdisplay(uint16_t addr, uint8_t data);
int testwritemem(void);

static MAINMSG lastmsg = {CMD_none, 0x00, 0x00, 0x00, false};
static MAINMSG lastrpy = {RPY_poweron, 0x00, 0x00, 0x00, false};
MAINMSG statusmsg = {RPY_status, 0x00, 0x00, 0x00, false};
MAINMSG datamsg[8];
//static MODE mode;
//static RUNMODE runmode = RUNMODE_STARTUP;

static SWSTATE swstate = SW_UP;
static SWEVENT swevent = SW_NOEVENT;
static int16_t encvalue = 1;
static int16_t encinc = 0;


/*
 *  main exacution loop run every 1 ms
 */
void run1ms(){ 
    updateSwitch();
    if (processMsg() && (lastmsg.command == CMD_status)) {

        if (status.MOTORRUN) { // motor module is in operate mode.
            if (status.MOTORPOWER && status.MOTORINIT) {
                status.MOTORPOWER = false;
                status.MOTORINIT = false;
            }
            lastrpy = statusmsg;
            // process control changes here
            if(status.RESENDMODE == true){
                if(rpyqueue(0)){
                    status.RESENDMODE  = false;
                }
            }
        } else { // motor module is in startup mode.
            if (status.MOTORPOWER && status.MOTORINIT) {
                // send motor module configure data.
                if (rpyqueue(0)) {
                    // motor was configured.
                    status.MOTORRUN = true;
                }
            }
        }
    }
          
    MENUACTION ma;
    ma.msg = ME_INPUTSTAT;
    ma.validf = true;
    ma.se = getSwitchEvent();
    ma.enc = encinc;
    encinc = 0;
    MENUEVENT me = menu(ma);
    if(me.validf){
        // process here
        switch(me.msg){        
            case ME_PROBEVALUE:
                process_probemsg(me);                
                break;
            default:
                break;
        }
    }
}

/*
 *  called with data count to start process.
 *  called with 0 argument to process reply messages.
 *  returns true when all all data sent.
 */
bool rpyqueue(uint16_t rpycount){
    static uint16_t count = 0;
    if(rpycount != 0){
       count = rpycount; 
       return false;
    }
    if(count == 0) return true;
    count--;
    lastrpy = datamsg[count];    
    return false;    
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

/*
 * 
 */
void process_probemsg(MENUEVENT me) {
    uint16_t dataidx;
    PRESSET ps;
    dataidx = 1;
    if (me.msdata->plsettings.probeactive) {
        datamsg[0].command = RPY_runpl;
        ps = init_getprobe(me.msdata->plsettings.probevalue, PL);
        datamsg[dataidx].command = RPY_setpl1;
        datamsg[dataidx].data1 = ps.pressure;
        datamsg[dataidx].data2 = ps.imax;
        datamsg[dataidx].data3 = ps.outlimit;
        dataidx++;
        datamsg[dataidx].command = RPY_setpl2;
        datamsg[dataidx].data1 = ps.kp;
        datamsg[dataidx].data2 = ps.ki;
        datamsg[dataidx].data3 = ps.kd;
        dataidx++;
    }
    if (me.msdata->prsettings.probeactive) {
        datamsg[0].command = RPY_runpr;
        ps = init_getprobe(me.msdata->prsettings.probevalue, PR);
        datamsg[dataidx].command = RPY_setpr1;
        datamsg[dataidx].data1 = ps.pressure;
        datamsg[dataidx].data2 = ps.imax;
        datamsg[dataidx].data3 = ps.outlimit;
        dataidx++;
        datamsg[dataidx].command = RPY_setpr2;
        datamsg[dataidx].data1 = ps.kp;
        datamsg[dataidx].data2 = ps.ki;
        datamsg[dataidx].data3 = ps.kd;
        dataidx++;
    }
    if (dataidx == 5) {
        datamsg[0].command = RPY_runboth;
        rpyqueue(dataidx);
    } else if (dataidx == 3) {
        rpyqueue(dataidx);
    }
    status.RESENDMODE = true;
}

/*
 * called every 100 us.
 */
void run100us(){    
    updateEncoder();    
}
void updateEncoder(void){ 
    static uint16_t timecount = 0;
    static uint16_t fastmode = 0;
    static uint8_t oldenc = 0;  
    static uint16_t counts[4] = {1000,1000,1000,1000};
    static uint16_t i = 0;
    uint8_t enc = ENCB_GetValue() << 1 | ENCA_GetValue();   
    
    timecount++;
    if(timecount == 50000){
        timecount = 0;
        fastmode = 0;
        counts[0] = 5000;
        counts[1] = 5000;
        counts[2] = 5000;
        counts[3] = 5000;
    }
    if(enc == oldenc) return;
    
    counts[i] = ((timecount > 5000) ? 5000 : timecount);
    timecount = 0;
    i = ((i == 3) ? 0 : i+1);
    uint16_t avtime = (counts[0] + counts[1] + counts[2] + counts[3])/4;
    if(avtime < 150){
        fastmode = 10;
    }else if(avtime < 400){
        fastmode = 5;
    }else if(avtime < 800){
        fastmode = 2;
    } else {
        fastmode = 0;
    }
    
 
    if(oldenc == 0){
        if(enc == 0x01){
            status.CTLENCCHANGED = true;
            switch (fastmode) {
                case 0:
                    encinc = 1;
                    break;
                case 2:
                    encinc = 4;
                    break;
                case 5:
                    encinc = 10;
                    break;
                case 10:
                    encinc = 50;
                    break;
                default:
                    break;
            }            
            if(encvalue != ENCMAXVAL){
                encvalue++;
            }
        } else if(enc == 0x02){
            status.CTLENCCHANGED = true;
             switch (fastmode) {
                case 0:
                    encinc = -1;
                    break;
                case 2:
                    encinc = -4;
                    break;
                case 5:
                    encinc = -10;
                    break;
                case 10:
                    encinc = -50;
                    break;
                default:
                    break;
            }
           
            if(enc != ENCMINVAL){
                encvalue--;
            }
        }
    }
    oldenc = enc;   
    
}

bool processMsg(){
    // don't want to process message until last cmd sent and rpy received.
    if((is_newmsg() == false) || (is_txbusy() == true)) return false;  
    MAINMSG msg = get_msg();
    if(msg.validf == false) return false;
    lastmsg = msg;
    
    switch(lastmsg.command){
        
        case CMD_init:
            status.MOTORPOWER = true;
            status.MOTORINIT = true; 
            lastrpy = statusmsg;
            break;
        case CMD_poweron:
            status.MOTORPOWER = true;
            lastrpy.command = RPY_init;
            status.MOTORINIT = true;
            status.MOTORRUN = false;
            break;
        default:
            break;
    }
    put_msg(lastrpy);
    return true;
}

/*
 * 
 */
bool processswitch(){
    
  return false;  
}
void displayupdate() {
    static uint16_t count = 100;
    if (count == 0) {
        count = 100;
        if (lastmsg.command == CMD_status) {
            
            display_int(lastmsg.data1, 1);
            display_int(lastmsg.data2, 2);
            //display_int(encvalue, 4);    

            if ((lastmsg.data3 & 0x0040) == 0) {
                LED1_SetLow();
            } else {
                LED1_SetHigh();
            }

            if ((lastmsg.data3 & 0x0020) == 0) {
                LED2_SetLow();
            } else {
                LED2_SetHigh();
            }

            if ((lastmsg.data3 & 0x0010) == 0) {
                LED3_SetLow();
            } else {
                LED3_SetHigh();
            }
        }
    }
    count--;
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



