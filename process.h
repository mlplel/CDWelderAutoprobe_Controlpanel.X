/* 
 * File:   process.h
 * Author: mjacobs
 *
 * Created on May 26, 2019, 12:15 PM
 */

#ifndef PROCESS_H
#define	PROCESS_H

#ifdef	__cplusplus
extern "C" {
#endif

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
        MODE_INIT = 0,
        MODE_OPERATE = 2,
        MODE_CAL = 10,

    } PROCESSMODE;

    void run100us(void);
    void run1ms(void);
    void updateSwitch(void);
    SWEVENT getSwitchEvent(void);
    SWEVENT peekswitchevent(void);
    void clearswitchevent(void);
    
    void calmode_init(void);
    
    void updateEncoder(void);
    void processMsg(void);
    void displayupdate(void);
    
    
    
    void testheader(void);


#ifdef	__cplusplus
}
#endif

#endif	/* PROCESS_H */

