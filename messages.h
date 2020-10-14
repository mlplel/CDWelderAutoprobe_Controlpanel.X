/* 
 * File:   messages.h
 * Author: mjacobs
 *
 * Created on June 5, 2019, 2:13 PM
 */

#ifndef MESSAGES_H
#define	MESSAGES_H

#ifdef	__cplusplus
extern "C" {
#endif
    
#include <stddef.h>
#include <xc.h>
#include <stdbool.h>

    typedef enum {
        RPY_init = 0x3B01,
        RPY_poweron,
        RPY_initfinished,
        RPY_status,
        RPY_none,
        RPY_command,
        RPY_probepressure,
        RPY_setpl1,
        RPY_setpl2,
        RPY_setpr1,
        RPY_setpr2,
        RPY_runboth,
        RPY_runpl,
        RPY_runpr,
        RPY_calmode,
        CMD_poweron = 0xA301,
        CMD_init,
        CMD_ok,
        CMD_status,
        CMD_busbusy,
        CMD_sync,
        CMD_cal,
        CMD_testmode1 = 0xB101,
        CMD_none = 1100,
        RPY_error = 0xFFFF,
        CMD_error = 0xFFFF,
    } CMD;

    typedef struct {
        uint16_t SETPRESSURE1 : 1;
        uint16_t SETPRESSURE2 : 1;
        uint16_t : 13;
        uint16_t SETTESTMODE : 1;
    } STATUS1BITS;

    typedef struct {
        CMD command;
        int16_t data1;
        int16_t data2;
        int16_t data3;
        bool validf;
    } MAINMSG;

    // operating mode
    typedef enum {
        MODE_RUNBOTH = 10,
        MODE_RUNPL,
        MODE_RUNPR,
        MODE_CALPL,
        MODE_CALPR,
        MODE_CALZERO,
        MODE_POWERON,
        MODE_INIT,
    } MODE;

    typedef enum {
        TESTPROBE1 = 0x0140,
        TESTPROBE2,
    } CTLCMD;

    /*
    extern const MAINMSG MSG_status;
    extern const MAINMSG MSG_poweron;
    extern const MAINMSG MSG_init;
    extern const MAINMSG MSG_cmdok;
    extern const MAINMSG MSG_testmode1;
    extern const MAINMSG MSG_sync;
    extern const MAINMSG MSG_error;
    extern const MAINMSG MSG_calmode;

    extern const MAINMSG RPY_init;
    extern const MAINMSG RPY_status;
    extern const MAINMSG RPY_command;
    extern const MAINMSG RPY_none; 
    extern const MAINMSG PRY_probepressure;
    extern const MAINMSG RPY_error;
    extern const MAINMSG RPY_cal;
     */

#ifdef	__cplusplus
}
#endif

#endif	/* MESSAGES_H */

