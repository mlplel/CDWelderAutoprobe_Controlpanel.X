/**
 precess.c

  @Company
    

  @File Name
    i2c.c

  @Summary
 * i2c I/O

  @Description
    processes operations.
    
*/


#include "i2c.h"
#include "displaydata.h"
#include "maincharset.h"
#include "stdlib.h"
#include "stdbool.h"
#include "libpic30.h"
#include "display.h"

typedef enum {
    I2C_IDLE,
    I2C_SENDADDR,
    I2C_SENDDATA,    
}I2C_STATES;

typedef struct
{
    uint8_t empty;
    uint8_t full;    
}DATA_QUEUE_STATUS;


#define MAX_I2C_DATABLOCKS      64

volatile static I2C_STATES i2cstate = I2C_IDLE;
static uint8_t sh1106_address = 0x78;   // 0x3c << 1 = 0x78
static I2C_DATABLOCK    i2cdatablocks[MAX_I2C_DATABLOCKS];
volatile static I2C_DATABLOCK    *startblock;
volatile static I2C_DATABLOCK    *endblock;
volatile static DATA_QUEUE_STATUS dataqueuestatus;
static uint8_t *pbuffer;


void i2c_Init(void){    
    
    startblock = i2cdatablocks;
    endblock = i2cdatablocks;
    dataqueuestatus.empty = 1;
    dataqueuestatus.full = 0;
    pbuffer = NULL;
    // using RB8 and RB9  
    I2C1BRG = 0x12;     // baud rate set for 400KHz
    //I2C1BRG = 0x12;       // baud rate set for 400Khz  
    I2C1CONL = 0x8000;   // enable i2c 7bit addr
    I2C1CONH = 0x0000;
    I2C1STAT = 0x00; 
    // clear interrupt flag
    IFS1bits.MI2C1IF = 0;
    // enable i2c interrupt
    IEC1bits.MI2C1IE = 1;    
}


/*
 * Called on I2C1 Master interrupt flag set  
 */
void __attribute__ ( ( interrupt, no_auto_psv ) ) _MI2C1Interrupt ( void ){ 
    
    static uint8_t bytesleft;
    static uint8_t *databuf;     
   
    IFS1bits.MI2C1IF = 0;       // clear interrupt flag  
    //IO_RC3_Toggle();
    
    if(I2C1STATbits.IWCOL){        
        I2C1STATbits.IWCOL = 0;
        // write collision occured reset state to IDLE
        i2cstate = I2C_IDLE;        
    }
    
    switch(i2cstate){
        case I2C_IDLE:
            if(dataqueuestatus.empty == 0){
                // there is data to send
                bytesleft = startblock->length;
                databuf = startblock->pdata;
                pbuffer = startblock->pdata;
                startblock++;
                if(startblock == (i2cdatablocks + MAX_I2C_DATABLOCKS)){
                    startblock = i2cdatablocks;
                }
                dataqueuestatus.full = 0;
                if(startblock == endblock){
                    dataqueuestatus.empty = 1;
                }
                // send start bit
                I2C1CONLbits.SEN = 1;
                i2cstate = I2C_SENDADDR;
            }            
            break;
        case I2C_SENDADDR:
            // send address with r/w bit cleared
            I2C1TRN = sh1106_address;
            i2cstate = I2C_SENDDATA;
            break;
        case I2C_SENDDATA:
            if(I2C1STATbits.ACKSTAT){
                // no ack from device
                // reset i2c here
                I2C1STATbits.ACKSTAT = 0;
                i2c_Stop();
            }
            else{
                if(bytesleft-- == 0){
                    i2c_Stop();
                }
                else{
                    I2C1TRN = *databuf++;
                }                
            }            
            break;
            
        default:
            // should not get here
            i2c_Stop();
            break;                 
    }    
}

void i2c_Stop(void){
    // set stop bit 
    I2C1CONLbits.PEN = 1;
    i2cstate = I2C_IDLE;
    if(pbuffer != NULL){
        dispmem_free(pbuffer);
        pbuffer = NULL;
    }
   // __delay_us(2);
}

I2C_STATUS_MSG i2c_Write(uint8_t *data, uint8_t length){
    
    if(dataqueuestatus.full)
       return I2C_STATUS_FULL;
    
    endblock->length = length;
    endblock->pdata = data;
    __write_to_IEC(IEC1bits.MI2C1IE = 0);
    endblock++;
    if(endblock == (i2cdatablocks + MAX_I2C_DATABLOCKS)){
        endblock = i2cdatablocks;
    }
    dataqueuestatus.empty = 0;
    if(endblock == startblock){
        dataqueuestatus.full = 1;
    }
    
    if(i2cstate == I2C_IDLE){
        // force interrupt to start i2c transfer
        IFS1bits.MI2C1IF = 1;
    }
    __write_to_IEC(IEC1bits.MI2C1IE = 1);
    return I2C_STATUS_OK;
}

I2C_STATUS_MSG i2c_Ready(void){
    if(dataqueuestatus.full)
        return I2C_STATUS_FULL;
    if(dataqueuestatus.empty && (i2cstate == I2C_IDLE))
        return I2C_STATUS_IDLE;
    else return I2C_STATUS_OK;
}

