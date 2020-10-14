/* 
 * File:   menu.h
 * Author: mjacobs
 *
 * Created on October 14, 2020, 9:20 AM
 */

#ifndef MENU_H
#define	MENU_H


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
    
    typedef struct {
        bool validf;
    }MENUEVENT;
    
    typedef struct {
        bool validf;
    }MENUACTION;
    
    typedef enum {
        MID_TOP = 0,
        MID_RUN = 1,
    }MENUID;

    typedef struct {
        MENUID id;
        CURSORPOS cursorat;
    }MENUSTATE;
    
    typedef struct {
        uint8_t col;
        uint8_t row;
        uint8_t ch;
    }MENUCHAR;
    
    typedef struct {
        MENUID id;
        uint8_t count;
        MENUCHAR data[12];
    }MENUDATA;
    
    
    
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
 
  @Description

  @Preconditions

  @Returns

  @Param

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

