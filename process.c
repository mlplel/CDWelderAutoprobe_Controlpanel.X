/**
 precess.c

  @Company
    

  @File Name
    precess.c

  @Summary
 low priority timed loops

  @Description
    processes operations.
    
*/


#include "process.h"
#include "mcc_generated_files/pin_manager.h"


void run100us(){
    
}



void run1ms(){
    
    
    
    //test
    
    static int blinkcount = 0;
   
    blinkcount++;
    
    
    if (blinkcount == 400){
        LED1_Toggle();
        blinkcount = 0;
    }
}
