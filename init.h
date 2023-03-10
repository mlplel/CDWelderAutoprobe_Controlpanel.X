/* 
 * File:   init.h
 * Author: 
 *
 * Created on October 5, 2020, 9:39 AM
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

#ifndef INIT_H
#define	INIT_H

#include <stdbool.h>
#include <xc.h>

#include "process.h"

#ifdef	__cplusplus
extern "C" {
#endif
    
    
/**
  @Summary
 
  @Description

  @Preconditions

  @Returns

  @Param

  @Example
*/    
void init_start(void);    
    

/**
  @Summary
 
  @Description

  @Preconditions

  @Returns

  @Param

  @Example
*/
bool init_loop(void);

/**
  @Summary
 
  @Description

  @Preconditions

  @Returns

  @Param

  @Example
*/
PRESSET init_getprobe(uint16_t i, PROBENUMBER p);

/**
  @Summary
 
  @Description

  @Preconditions

  @Returns

  @Param

  @Example
*/
void init_setprobe(uint16_t i, PROBENUMBER p, PRESSET ps);



/**
  @Summary
 
  @Description

  @Preconditions

  @Returns

  @Param

  @Example
*/
 void init_setprobeitem(uint16_t pv, PROBENUMBER p, CALITEM item, uint16_t data);
 
 
 
 
/**
  @Summary
 
  @Description

  @Preconditions

  @Returns

  @Param

  @Example
*/
  uint16_t init_getprobeitem(uint16_t pv, PROBENUMBER p, CALITEM item);
  
  
/**
  @Summary
 
  @Description

  @Preconditions

  @Returns

  @Param

  @Example
*/  
bool init_writesettings(); 


#ifdef	__cplusplus
}
#endif

#endif	/* INIT_H */

