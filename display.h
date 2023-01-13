/* 
 * File:   display.h
 * Author: 
 *
 * Created on October 5, 2020, 10:25 AM
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

#ifndef DISPLAY_H
#define	DISPLAY_H

#include "menu.h"

#include <stdbool.h>
#include <xc.h>

#ifdef	__cplusplus
extern "C" {
#endif
    
    
    typedef struct {
        uint16_t active;
        uint16_t priority;
        uint8_t data[142];
    }DISPLAYITEM;

  
    
    
/**
  @Summary
 
  @Description
  call to initialize display memory.
  @Preconditions

  @Returns

  @Param

*/    
void dispmem_init(void);    
    
/**
  @Summary
 
  @Description
 returns a pointer to char data Null if no memory available.
  @Preconditions

  @Returns
 uint8_t* to 142 bytes max size.
  @Param

  @Example
*/    
uint8_t* dispmem_alloc(void);    


/**
  @Summary
 
  @Description
  frees the item allocated by dispmem_alloc.
  @Preconditions

  @Returns

  @Param
  uint8_t* that was returned from dispmem_alloc. 
  @Example
*/   
void dispmem_free(uint8_t* p);    


    
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

