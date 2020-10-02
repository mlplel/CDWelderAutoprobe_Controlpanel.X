/* 
 * File:   i2c.h
 * Author: mjacobs
 *
 * Created on May 27, 2019, 8:11 AM
 */

#ifndef I2C_H
#define	I2C_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "mcc_generated_files/mcc.h"
#define FCY     (_XTAL_FREQ/2)

#include <stddef.h>
#include  "xc.h"
    
    typedef struct 
    {
        uint8_t length;
        uint8_t *pdata;
    } I2C_DATABLOCK;

    typedef enum {
        I2C_STATUS_IDLE,
        I2C_STATUS_FULL,
        I2C_STATUS_OK,

    } I2C_STATUS_MSG;




    void i2c_Init(void);

    void i2c_Stop(void);

    I2C_STATUS_MSG i2c_Write(uint8_t *data, uint8_t length);
    I2C_STATUS_MSG i2c_Ready(void);


    void sh1106_Init(void);
    void sh1106_Clear(void);

    void sh1106_Char(uint8_t col, uint8_t line, int chr);
    void sh1106_charat(uint8_t col, uint8_t line, int chr);
    
    void display_int(int16_t val, uint8_t line);
    
    void display_header(const uint8_t* s1, const uint8_t* s2, const uint8_t* s3);




#ifdef	__cplusplus
}
#endif

#endif	/* I2C_H */

