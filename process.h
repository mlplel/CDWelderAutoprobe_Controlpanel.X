/* 
 * File:   process.h
 * Author: mjacobs
 *
 * Created on May 26, 2019, 12:15 PM
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
    
    void run100us(void);
    void run1ms(void);
    void updateSwitch(void);
    SWEVENT getSwitchEvent(void);
    SWEVENT peekswitchevent(void);
    void clearswitchevent(void);
    
    void calmode_init(void);
    
    void updateEncoder(void);
    bool processMsg(void);
    void displayupdate(void);
    void testheader(void);
    
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

