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

#include "stdlib.h"
#include "stdbool.h"
#include "libpic30.h"

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


#define MAX_I2C_DATABLOCKS      20

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
      
    I2C1BRG = 0x12;     // baud rate set for 100KHz
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
        free(pbuffer);
        pbuffer = NULL;
    }
   // __delay_us(2);
}

I2C_STATUS_MSG i2c_Write(uint8_t *data, uint8_t length){
    
    if(dataqueuestatus.full)
        return I2C_STATUS_FULL;
    
    endblock->length = length;
    endblock->pdata = data;
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
    return I2C_STATUS_OK;
}

I2C_STATUS_MSG i2c_Ready(void){
    if(dataqueuestatus.full)
        return I2C_STATUS_FULL;
    if(dataqueuestatus.empty && (i2cstate == I2C_IDLE))
        return I2C_STATUS_IDLE;
    else return I2C_STATUS_OK;
}

 void sh1106_Init(void){    
     
    __delay_ms(100);
    while(i2c_Ready() != I2C_STATUS_IDLE);
    uint8_t *pdata;
    if((pdata = (uint8_t *)malloc(6)) == NULL){
        // fatal error need to reset
        while(1);
    }
    *pdata = 0x80;
    *(pdata + 1) = 0xA1;  //segment remap left
    *(pdata + 2) = 0x80;
    *(pdata + 3) = 0xC8;  //set scan direction
    *(pdata + 4) = 0x00;
    *(pdata + 5) = 0x50;  //set scan display line
    // init sh1106
    i2c_Write(pdata, 6);  

    sh1106_Clear();
    __delay_ms(100);
    
    if((pdata = (uint8_t *)malloc(2)) == NULL){
        // fatal error need to reset
        while(1);
    }
    *(pdata + 0) = 0x00;
    *(pdata + 1) = 0xAF; // turn display on
    i2c_Write(pdata, 2);    
    __delay_ms(150);   
 }
 
 void sh1106_Clear(void){
    uint8_t *pdata;
    uint8_t page = 0;
    
    while (page < 8) {
        if ((pdata = (uint8_t *) malloc(140)) == NULL) {
            // fatal error need to reset
            while (1);
        }
        *(pdata + 0) = 0x80;
        *(pdata + 1) = 0xB0 + page; // select page 0
        *(pdata + 2) = 0x80;
        *(pdata + 3) = 0x00;
        *(pdata + 4) = 0x80;
        *(pdata + 5) = 0x10; // column 0
        *(pdata + 6) = 0x40;
        int i = 7;
        //139 
        for (; i < 139;) {
            *(pdata + i) = 0x00;
            i++;
        }        
        while(i2c_Ready() != I2C_STATUS_IDLE);
        i2c_Write(pdata, i);
        page++;        
    }  
 }
 
 uint8_t sh1106_blankpix[] = {
     0xc0, 00
 };
 
 const uint8_t colpos[] = {
     0x04, 0x0E, 0x18, 0x22, 0x2C, 0x36, 0x40, 0x4A, 0x54, 0x5E, 0x68, 0x72
 };
 
 const uint8_t linepos[] = {
     0x00, 0x02, 0x04, 0x06     
 }; 
 
 const uint8_t chset1[] = {
     0x00, 0x08, 0x04, 0x02, 0x02, 0xFE, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x40, 0x40, 0x40, 0x40, 0x7F, 0x40, 0x40, 0x40, 0x00,
     0x00, 0x10, 0x08, 0x04, 0x02, 0x02, 0x04, 0x88, 0x70, 0x00,
     0x00, 0x60, 0x50, 0x48, 0x44, 0x42, 0x41, 0x40, 0x40, 0x00,
    
 };
 
 const uint8_t chset2[] = {
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //chr 0
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x08, 0x04, 0x02, 0x02, 0xFE, 0x00, 0x00, 0x00, 0x00, //chr 1
     0x00, 0x40, 0x40, 0x40, 0x40, 0x7F, 0x40, 0x40, 0x40, 0x00,
     0x00, 0x10, 0x08, 0x04, 0x02, 0x02, 0x04, 0x88, 0x70, 0x00, //chr 2
     0x00, 0x60, 0x50, 0x48, 0x44, 0x42, 0x41, 0x40, 0x40, 0x00,
     0x00, 0x04, 0x82, 0x82, 0x82, 0x82, 0x82, 0x82, 0xFE, 0x00, //chr 3
     0x00, 0x20, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x3F, 0x00,
     0x00, 0xFE, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xFE, 0x00, //chr 4
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7F, 0x00,
     0x00, 0x7E, 0x42, 0x42, 0x42, 0x82, 0x02, 0x02, 0x02, 0x00, //chr 5
     0x00, 0x40, 0x40, 0x40, 0x40, 0x40, 0x21, 0x12, 0x0C, 0x00,
     0x00, 0x80, 0x60, 0x10, 0x88, 0x84, 0x02, 0x02, 0x00, 0x00, //chr 6
     0x00, 0x0E, 0x12, 0x21, 0x40, 0x40, 0x21, 0x12, 0x0C, 0x00,
     0x00, 0x02, 0x02, 0x02, 0x02, 0x82, 0x62, 0x1A, 0x06, 0x00, //chr 7
     0x00, 0x00, 0x60, 0x18, 0x06, 0x01, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x30, 0x48, 0x84, 0x82, 0x82, 0x84, 0x48, 0x30, 0x00, //chr 8
     0x00, 0x0C, 0x12, 0x21, 0x41, 0x41, 0x21, 0x12, 0x0C, 0x00,
     0x00, 0x30, 0x48, 0x84, 0x02, 0x02, 0x84, 0x48, 0xF0, 0x00, //chr 9
     0x00, 0x00, 0x40, 0x20, 0x11, 0x09, 0x04, 0x02, 0x01, 0x00,
     0x00, 0xF0, 0x08, 0x04, 0x02, 0x02, 0x04, 0x08, 0xF0, 0x00, //chr 10
     0x00, 0x0F, 0x10, 0x20, 0x40, 0x40, 0x20, 0x10, 0x0F, 0x00,
     0x00, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x00, //chr 11
     0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00
 };

 
 void sh1106_Char(uint8_t col, uint8_t line, int chr) {
    uint8_t *pdata; 
    if((pdata = (uint8_t *)malloc(54)) == NULL){
        // error just return atm
        return;
    } 
    
    int charindx = chr * 20;
    uint8_t page = linepos[line];
    uint8_t collow = colpos[col] & 0x0F;
    int8_t colhigh = (colpos[col] >> 4) | 0x10;
    int i = 0;

    *(pdata + i) = 0x80;
    i++;
    *(pdata + i) = 0xB0 + page; //select page 0 - 7
    i++;
    *(pdata + i) = 0x80;
    i++;
    *(pdata + i) = collow;
    i++;
    *(pdata + i) = 0x80;
    i++;
    *(pdata + i) = colhigh;
    i++;
    int j;
    for (j = 0; j < 10; j++) {
        *(pdata + i) = 0xC0;
        i++;
        *(pdata + i) = chset2[charindx];
        i++;
        charindx++;
    }

    *(pdata + i) = 0x80;
    i++;
    *(pdata + i) = 0xB0 + page + 1; //select page 0 - 7
    i++;
    *(pdata + i) = 0x80;
    i++;
    *(pdata + i) = collow;
    i++;
    *(pdata + i) = 0x80;
    i++;
    *(pdata + i) = colhigh;
    i++;
    for (j = 0; j < 9; j++) {
        *(pdata + i) = 0xC0;
        i++;
        *(pdata + i) = chset2[charindx];
        i++;
        charindx++;
    }
    *(pdata + i) = 0x40;
    i++;
    *(pdata + i) = chset2[charindx];
    i++;
    charindx++;    
    
    //while(i2c_Ready() != I2C_STATUS_IDLE);
   
    i2c_Write(pdata, i);
   
 }
 
 void display_Value(int16_t val){
     bool negf;
     uint8_t pos= 6;
     
     if(val < 0){
         negf = true;
         val = (~val + 1);
     }
     else{
         negf = false;
     }
     if(val == 0){
         sh1106_Char(1, 1, 0);
         sh1106_Char(2, 1, 0);
         sh1106_Char(3, 1, 0);
         sh1106_Char(4, 1, 0);
         sh1106_Char(5, 1, 0);
         sh1106_Char(6, 1, 10);         
         return;
     }  
     int dig;
     while(val != 0){
     
        dig = val % 10;
        if(dig == 0)
            dig = 10;
        sh1106_Char(pos, 1, dig);
         
        val = val / 10;
        pos--;           
         
     } 
     if(negf == true){
         sh1106_Char(pos, 1, 11);
         pos--;
     }     
     for(; pos > 0 ; pos--){
         sh1106_Char(pos, 1, 0);
     }          
 }
 
 void display_Line3(int16_t val){
     bool negf;
     uint8_t pos= 6;
     
     if(val < 0){
         negf = true;
         val = (~val + 1);
     }
     else{
         negf = false;
     }
     
     if(val == 0){
         sh1106_Char(1, 4, 0);
         sh1106_Char(2, 4, 0);
         sh1106_Char(3, 4, 0);
         sh1106_Char(4, 4, 0);
         sh1106_Char(5, 4, 0);
         sh1106_Char(6, 4, 10);         
         return;
     }     
   
     int dig;
     
     while(val != 0){
     
        dig = val % 10;
        if(dig == 0)
            dig = 10;
        sh1106_Char(pos, 4, dig);
         
        val = val / 10;
        pos--;
     }         
     
     if(negf == true){
         sh1106_Char(pos, 4, 11);
         pos--;
     }
     
     for(; pos > 0 ; pos--){
         sh1106_Char(pos, 4, 0);
     }
 }
  
