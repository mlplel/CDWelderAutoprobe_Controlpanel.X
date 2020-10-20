/* 
 * File:   init.h
 * Author: mjacobs
 *
 * Created on October 5, 2020, 9:39 AM
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

