/* 
 * File:   display.h
 * Author: mjacobs
 *
 * Created on October 5, 2020, 10:25 AM
 */

#ifndef DISPLAY_H
#define	DISPLAY_H

#include "menu.h"

#include <stdbool.h>
#include <xc.h>

#ifdef	__cplusplus
extern "C" {
#endif

  
    
/**
  @Summary
 
  @Description
    setup for oled display.  may block till i2c bus ready.
  @Preconditions

  @Returns

  @Param

  @Example
*/
 void sh1106_setup(void);   
    
 
/**
  @Summary
 
  @Description
    clear oled display memory.  may block till i2c bus ready.
  @Preconditions

  @Returns

  @Param

  @Example
*/   
void sh1106_clear(void);


/**
  @Summary
    sh1106_clearpage(page)
  @Description
    clear oled display page.  Non blocking.
  @Preconditions

  @Returns
    true if page cleared, false if buffer was full.
  @Param
    number 0 to 7. no range check is done.
*/
bool sh1106_clearpage(uint8_t page);


/**
  @Summary
 
  @Description

  @Preconditions

  @Returns

  @Param

  @Example
*/ 
void sh1106_on(void);


/**
  @Summary
 
  @Description

  @Preconditions

  @Returns

  @Param

  @Example
*/ 
bool sh1106_charat(uint8_t col, uint8_t line, int chr);



/**
  @Summary
 
  @Description

  @Preconditions

  @Returns

  @Param

  @Example
*/
void display_int(int16_t val, uint8_t line);


/**
  @Summary
 
  @Description

  @Preconditions

  @Returns

  @Param

  @Example
*/
bool display_2digit(uint8_t val, uint8_t col, uint8_t line);


/**
  @Summary
 
  @Description

  @Preconditions

  @Returns

  @Param

  @Example
*/
bool display_5digit(uint16_t val, uint8_t col, uint8_t line);
/**
  @Summary
 
  @Description

  @Preconditions

  @Returns

  @Param

  @Example
*/
void cursor(CURSORPOS cp);



/**
  @Summary
 
  @Description

  @Preconditions

  @Returns

  @Param

  @Example
*/
void cursorflash(void);



/**
  @Summary
 
  @Description

  @Preconditions

  @Returns

  @Param

  @Example
*/
bool display_menu(MENUDATA m);





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

#endif	/* DISPLAY_H */

