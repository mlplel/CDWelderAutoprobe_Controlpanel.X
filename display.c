/*
 * 
 * 
 * 
 * 
 * 
 * 
 */

#include "display.h"
#include "i2c.h"

#include <stdlib.h>


/*
 * 
 */
void sh1106_on(void) {
    uint8_t *pdata;
    if ((pdata = (uint8_t *) malloc(2)) == NULL) {
        // fatal error need to reset
        // need error handler here
        while (1);
    }
    *(pdata + 0) = 0x00;
    *(pdata + 1) = 0xAF; // turn display on
    i2c_Write(pdata, 2);
}

/*
 * 
 */
void sh1106_setup(void){
    while(i2c_Ready() != I2C_STATUS_IDLE);
    uint8_t *pdata;
    if((pdata = (uint8_t *)malloc(6)) == NULL){
        // fatal error need to reset
        // need error handler here
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
 }
 
 /*
  * 
  */
 void sh1106_clear(void){
    uint8_t *pdata;
    uint8_t page = 0;    
    while (page < 8) {
        if ((pdata = (uint8_t *) malloc(140)) == NULL) {
            // fatal error need to reset
            // need error handler here
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