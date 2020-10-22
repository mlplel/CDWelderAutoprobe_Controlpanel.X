/* 
 * File:   menu.h
 * Author: mjacobs
 *
 * Created on October 14, 2020, 9:20 AM
 */

#ifndef MENU_H
#define	MENU_H

#include "process.h"

#include <stdbool.h>
#include <xc.h>

#ifdef	__cplusplus
extern "C" {
#endif

    typedef struct {
        uint8_t cursorcol;
        uint8_t cursorline;
        bool visible;
    } CURSORPOS;

    typedef enum {
        ME_MENUCHANGED,
        ME_SELCHANGED,
        ME_ENTRYSELC,
        ME_MENUINIT,
        ME_INPUTSTAT,
        ME_PROBEVALUE,
        ME_CALVALUE,
        ME_CALZERO,
        ME_IDLE,
        ME_SAVE
    } MENUMSG;
     
    typedef struct {
        MENUMSG msg;
        int16_t enc;
        SWEVENT se;
        bool validf;
    }MENUACTION;

    typedef enum {
        MID_TOP = 0,
        MID_RUN = 1,
        MID_CAL = 2,
        MID_RUNBOTH = 3,
        MID_RUNPL = 4,
        MID_RUNPR = 5,
        MID_CALPL1 = 6,
        MID_CALPL2 = 7,
        MID_CALPR1 = 8,
        MID_CALPR2 = 9
    } MENUID;
    
    typedef struct {
        bool probeactive;
        uint16_t probevalue;
    }PROBE;

    typedef struct {
        MENUID id;
        CURSORPOS cursorat;
        CALITEM cal;
        PROBE plsettings;
        PROBE prsettings;
    }MENUSTATE;
    
    typedef struct {
        uint8_t col;
        uint8_t row;
        uint8_t ch;
    }MENUCHAR;
    
    typedef struct {
        MENUID id;
        uint8_t count;
        MENUCHAR data[20];
    }MENUDATA;
    
     typedef struct {
        MENUMSG msg;
        MENUSTATE* msdata;
        bool validf;
    }MENUEVENT;    
    
    
/**
  @Summary
 
  @Description

  @Preconditions

  @Returns

  @Param

  @Example
*/    
bool menu_init(MENUSTATE m) ;  
    
    
/**
  @Summary
 main entry point to menu.
  @Description
 called with a MENUACTION for process by the menu.
  @Preconditions

  @Returns
 MENUEVENT 
  @Param
 MENUACTION
  @Example
*/      
MENUEVENT menu(MENUACTION a);




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

#endif	/* MENU_H */

