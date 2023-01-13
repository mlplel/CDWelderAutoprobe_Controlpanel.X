/* 
 * File:   messages.h
 * Author: 
 *
 * Created on June 5, 2019, 2:13 PM
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
        RPY_idle,
        RPY_calpl,
        RPY_calpr,
        RPY_calzero,
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
        MODE_IDLE
    } MODE;

    typedef enum {
        TESTPROBE1 = 0x0140,
        TESTPROBE2,
    } CTLCMD;


#ifdef	__cplusplus
}
#endif

#endif	/* MESSAGES_H */

