/* 
 * File:   process.h
 * Author: 
 *
 * Created on May 26, 2019, 12:15 PM
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

#include <stdbool.h>
#include <xc.h>


#ifndef PROCESS_H
#define	PROCESS_H

#ifdef	__cplusplus
extern "C" {
#endif

    typedef enum {
        PR = 1,
        PL = 2
    } PROBENUMBER;

    typedef enum {
        SW_DOWN = 0,
        SW_UP = 1,
    } SWSTATE;

    typedef enum {
        SW_NOEVENT = 0,
        SW_CLICKED = 1,
        SW_LCLICKED = 2,
        SW_ELCLICKED = 3,
        SW_DCLICKED = 4
    } SWEVENT;

   
    typedef enum {
        RUNMODE_RUN,
        RUNMODE_SET,
        RUNMODE_CAL,
        RUNMODE_TEST,
        RUNMODE_STARTUP
    } RUNMODE;
    
    
    typedef struct {
        bool MOTORPOWER;
        bool MOTORINIT;
        bool MOTORRUN;
        bool CTLPOWER;
        bool CTLENCCHANGED;
        bool RESENDMODE;
    } PROC_STAT;

    typedef struct {
        int16_t pressure;
        int16_t kp;
        int16_t ki;
        int16_t kd;
        int16_t imax;
        int16_t outlimit;   
        bool validf;
    }PRESSET;
    
     typedef enum {
        CAL_PRESSURE = 1,
        CAL_ILIMIT = 2,
        CAL_OLIMIT = 3,
        CAL_KP = 4,
        CAL_KI = 5,
        CAL_KD = 6,
        CAL_NONE = 7
    } CALITEM;
    
    
/**
  @Summary
 
  @Description

  @Preconditions

  @Returns

  @Param

  @Example
*/    
void run100us(void);


/**
  @Summary
 
  @Description

  @Preconditions

  @Returns

  @Param

  @Example
*/
void run1ms(void);




  

 
    
/**
  @Summary
 
  @Description

  @Preconditions

  @Returns

  @Param

  @Example
*/
void process_init(void);    
    
/**
  @Summary
 
  @Description

  @Preconditions

  @Returns

  @Param

  @Example
*/ 

/**
  @Summary
 
  @Description

  @Preconditions

  @Returns

  @Param

  @Example
*/     
    void run_startup(void);
    
    
/**
  @Summary
 
  @Description

  @Preconditions

  @Returns

  @Param

  @Example
*/    
    

#ifdef	__cplusplus
}
#endif

#endif	/* PROCESS_H */

