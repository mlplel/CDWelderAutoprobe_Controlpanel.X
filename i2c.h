/* 
 * File:   i2c.h
 * Author: 
 *
 * Created on May 27, 2019, 8:11 AM
 */
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

