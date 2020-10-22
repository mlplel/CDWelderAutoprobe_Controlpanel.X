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



/**
  @Summary
 
  @Description

  @Preconditions

  @Returns

  @Param

  @Example
*/
void i2c_Init(void);


/**
  @Summary
 
  @Description

  @Preconditions

  @Returns

  @Param

  @Example
*/
void i2c_Stop(void);

    
/**
  @Summary
 
  @Description

  @Preconditions

  @Returns

  @Param

  @Example
*/    
I2C_STATUS_MSG i2c_Write(uint8_t *data, uint8_t length);




/**
  @Summary
 
  @Description

  @Preconditions

  @Returns

  @Param

  @Example
*/
I2C_STATUS_MSG i2c_Ready(void);


   




#ifdef	__cplusplus
}
#endif

#endif	/* I2C_H */

