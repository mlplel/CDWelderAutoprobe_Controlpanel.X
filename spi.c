
/**
 precess.c

  @Company
    

  @File Name
    spi.c

  @Summary
 * spi I/O

  @Description
    processes operations.
    
*/


#include "spi.h"
#include "xc.h"
#include "mcc_generated_files/pin_manager.h"
#include <stdbool.h>
#include "messages.h"

static EESTATUS writestatus = UNKNOWN;
static EESTATUS readstatus = UNKNOWN;
static EESTATE eerxstate = OFF;
static EESTATE eetxstate = OFF;

static uint16_t memaddr = 0;
static uint8_t  memdata = 0;
static uint8_t tempread;

static int16_t txdata[6];
static int16_t rxdata[6];
static bool newrxf = 0;
static bool newtxf = 0;

static int16_t msglog[80];
static int16_t msglogp = 0;


void spi_IOSetup(void){

    __builtin_write_OSCCONL(OSCCON & 0xbf); // unlock PPS

    RPOR3bits.RP7R = 0x0007; //RB7->SPI1:SDO1
    RPOR3bits.RP6R = 0x0008; //RB6->SPI1:SCK1OUT
    RPINR20bits.SDI1R = 0x0004; //RB4->SPI1:SDI1
    
    RPOR7bits.RP14R = 0x000A;   //RB14->SPI2:SDO  MISO
    RPINR22bits.SCK2R = 0x000D; //RB13->SPI2:SCK SCK
    RPINR22bits.SDI2R = 0x000F; //RB15->SPI2:SDI  MOSI
    RPINR23bits.SS2R = 0x0C;    //RB12->SPI2:SS    SS

    __builtin_write_OSCCONL(OSCCON | 0x40); // lock PPS
   
}

void spi1_Init(void){
    SPI1BRGL = 0x07;    // 4 MHz spi clock
    SPI1CON1L = 0x8120;  // spi mode 0,no enhanced buffer      
    SPI1STATLbits.SPIROV = 0;
    
    SPI1IMSKL = 0x0009;  // interrupt on tx empty and rx full.
    SPI1IMSKH = 0x0000;
    
    SPI1STATLbits.SPIROV = 0;
    IFS3bits.SPI1RXIF = 0;
    IEC3bits.SPI1RXIE = 1;
    
    IFS0bits.SPI1TXIF = 0;
    IEC0bits.SPI1TXIE = 1;
    
    eerxstate = ON;
    eetxstate = ON;
}

void spi2_Init(void){
    
     SPI2CON1L = 0x8501;     // slave mode 0 enhanced buffer 16 bit
     SPI2CON1Lbits.SSEN = 1; // enable SS input
     SPI2STATLbits.SPIROV = 0;
     //SPI2IMSKLbits.SRMTEN = 1;
     SPI2IMSKL = 0X0000;    // mask to interrupt on rx buffer = 4
     SPI2IMSKH = 0X8400;
     
     int i = 0;
     for(;i<4;i++){
         SPI2BUFL = txdata[i];
     }
     SPI2STATLbits.SPIROV = 0;
     //IFS2bits.SPI2IF = 0;
     //IFS2bits.SPI2TXIF = 0;
     IFS3bits.SPI2RXIF = 0;
     
     //IEC2bits.SPI2IE = 1;
     //IEC2bits.SPI2TXIE = 1;
     IEC3bits.SPI2RXIE = 1;    
}

/*
 * 
 */
void commresync(){
    LED2_Toggle();
    while(!C_CS_GetValue());
    SPI2CON1Lbits.SPIEN = 0;
    Nop();
    Nop();
    SPI2CON1Lbits.SPIEN = 1;
    int i;
    for(i=0;i<4;i++){
        SPI2BUFL = txdata[i];
    }
}


void mem_start(uint8_t cmd){   
   MCS_SetLow(); 
   SPI1BUFL = cmd;     
}

bool mem_isidle(void) {
    if ((eerxstate == ON) && (eetxstate == ON)) {
        return true;
    } else
        return false;
}

/**
 * @Summary
 *  returns eeprom write status.
 * 
 * @Description
 *  Non blocking status check of eeprom.  Requires 2 calls to get status.
 * 
 * @Param
 *  None.
 * 
 * @Returns
 *  EESTATUS.UNKNOWN    Update status in progress.
 *  EESTATUS.BUSY       eerpm is busy.
 *  EESTATUS.WPON       write protect for eeprom is active.
 *  EESTATUS.READY      ready to write byte or block.
 */
EESTATUS mem_getwritestatus(void) {

    if (writestatus == UNKNOWN) {
        if (mem_isidle()) {
            eerxstate = READSTATUS1;
            eetxstate = READSTATUS1;
            mem_start(EEPROM_RDSR);
            return UNKNOWN;
        } else {
            return BUSY;
        }
    } else {
        EESTATUS t = writestatus;
        writestatus = UNKNOWN;
        return t;
    }
}

EESTATUS mem_getreadstatus(void){
    
    if(readstatus == READY){
        return readstatus;        
    } else {
        
        if(mem_isidle()){
            return UNKNOWN;
        } else {
            return BUSY;
        }        
    }    
}

void mem_writeenable(void){
    if(!mem_isidle()){
        return;
    }
    eerxstate = WRITEENABLE;
    eetxstate = WRITEENABLE;
    mem_start(EEPROM_WREN);    
}

void mem_readbyteaddr(uint16_t addr){
    if(!mem_isidle()){
        return;
    }
    memaddr = addr;
    eerxstate = READ1;
    eetxstate = READ1;
    mem_start(EEPROM_READ);    
}

uint8_t mem_readbyte(void){
    readstatus = UNKNOWN;
    return memdata;
}


void mem_writebyte(uint8_t data, uint16_t addr){
    if(!mem_isidle()){
        return;
    }
    memaddr = addr;
    memdata = data;
    eerxstate = WRITE1;
    eetxstate = WRITE1;
    mem_start(EEPROM_WRITE);       
}

/*
 * 
 * 
 * 
 * 
 */
MAINMSG get_msg(){     
    MAINMSG msg;
    newrxf = false;
    
    msg.command = rxdata[0];
    msg.data1 = rxdata[1];
    msg.data2 = rxdata[2];
    msg.data3 = rxdata[3];

    if (newrxf == true) {
        msg.validf = false;
        return msg;
    }
    msg.validf = true;
    return msg;    
}

bool isNewMsg(){
    return newrxf;
}
bool is_newmsg(){
    return newrxf;
}

int16_t put_msg(MAINMSG msg) {
    if (newtxf == true) return -1; // message buffer is busy

    txdata[0] = msg.command;
    txdata[1] = msg.data1;
    txdata[2] = msg.data2;
    txdata[3] = msg.data3;

    newtxf = true;
    return 0;
}

bool is_txbusy(){
    return newtxf;
}


void __attribute__ ( ( interrupt, no_auto_psv ) ) _SPI1Interrupt ( void ){  
    
    IFS0bits.SPI1IF = 0;   
}

void __attribute__ ( ( interrupt, no_auto_psv ) ) _SPI1TXInterrupt ( void ){
    
    IFS0bits.SPI1TXIF = 0;  

    switch (eetxstate) {

        case READSTATUS1:
            SPI1BUFL = EEPROM_DUMMY;
            eetxstate = READSTATUS2;
            break;
        case READSTATUS2:
            eetxstate = ON;
            break;
        case READ1:
            SPI1BUFL = (memaddr >> 8);
            eetxstate = READ2;
            break;
        case READ2:
            SPI1BUFL = memaddr;
            eetxstate = READ3;
            break;
        case READ3:
            SPI1BUFL = EEPROM_DUMMY;
            eetxstate = READ4;
            break;
        case READ4:
            eetxstate = ON;
            break;
        case WRITEENABLE:
            eetxstate = ON;
            break;
        case WRITE1:
            SPI1BUFL = (memaddr >> 8);
            eetxstate = WRITE2;
            break;
        case WRITE2:
            SPI1BUFL = memaddr;
            eetxstate = WRITE3;
            break;
        case WRITE3:
            SPI1BUFL = memdata;
            eetxstate = WRITE4;
            break;
        case WRITE4:
            eetxstate = ON;
            break;      
                    
        default:
            break;
    }    
}

void __attribute__ ( ( interrupt, no_auto_psv ) ) _SPI1RXInterrupt ( void ){
    
    IFS3bits.SPI1RXIF = 0;

    switch (eerxstate) {

        case READSTATUS1:
            tempread = SPI1BUFL;
            eerxstate = READSTATUS2;
            break;
        case READSTATUS2:
            tempread = SPI1BUFL;
            MCS_SetHigh();
            if ((tempread & 0x01) == 0x01) {
                writestatus = BUSY;
            } else if ((tempread & 0x02) == 0x02) {
                writestatus = READY;
            } else {
                writestatus = WPON;
            }            
            eerxstate = ON;
            break;
        case READ1:
            tempread = SPI1BUFL;
            eerxstate = READ2;
            break;
        case READ2:
            tempread = SPI1BUFL;
            eerxstate = READ3;
            break;
        case READ3:
            tempread = SPI1BUFL;
            eerxstate = READ4;
            break;
        case READ4:
            memdata = SPI1BUFL;
            MCS_SetHigh();
            readstatus = READY;
            eerxstate = ON;
            break;
        case WRITEENABLE:
            tempread = SPI1BUFL;
            MCS_SetHigh();
            eerxstate = ON;
            break;
        case WRITE1:
            tempread = SPI1BUFL;
            eerxstate = WRITE2;
            break;
        case WRITE2:
            tempread = SPI1BUFL;
            eerxstate = WRITE3;
            break;
        case WRITE3:
            tempread = SPI1BUFL;
            eerxstate = WRITE4;
            break;
        case WRITE4:
            tempread = SPI1BUFL;
            MCS_SetHigh();
            eerxstate = ON;
            break;            
                  
        default:
            break;

    }    
}


void __attribute__ ( ( interrupt, no_auto_psv ) ) _SPI2Interrupt ( void ){  
    
    IFS2bits.SPI2IF = 0;   
}

void __attribute__ ( ( interrupt, no_auto_psv ) ) _SPI2TXInterrupt ( void ){
    
    IFS2bits.SPI2TXIF = 0;  
}

void __attribute__ ( ( interrupt, no_auto_psv ) ) _SPI2RXInterrupt ( void ){
    
    IFS3bits.SPI2RXIF = 0;   
     TP3_Toggle();  
    
    int i = 0;
    // if valid msg to go out move to SPI2BUFL.
    // else move dummy msg to output and move received msg to buffer.
    if (newtxf == true) {
        for (; i < 4; i++) {
            SPI2BUFL = txdata[i];
            rxdata[i] = SPI2BUFL;
        }
    } else {
        SPI2BUFL = RPY_none;
        rxdata[0] = SPI2BUFL;
        SPI2BUFL = 0x00;
        rxdata[1] = SPI2BUFL;
        SPI2BUFL = 0x00;
        rxdata[2] = SPI2BUFL;
        SPI2BUFL = 0x00;
        rxdata[3] = SPI2BUFL;        
    }
    newtxf = false;
    newrxf = true;
    
    
    // test log
    for(i = 0; i < 4; i++){
        msglog[msglogp] = rxdata[i];
        if (msglogp == 79){
            msglogp = 0;
        }
        else msglogp++;
    }
   
    
}