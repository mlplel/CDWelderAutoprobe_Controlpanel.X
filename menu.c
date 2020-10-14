/**
menu.c

  @Company
    

  @File Name
    menu.c

  @Summary
 low priority timed loops

  @Description
    processes operations.    
*/

#include "menu.h"
#include "display.h"

#include <stdlib.h>

/*
 *  file scope prototypes and Forward Declarations
 */
MENUEVENT menu_top(MENUACTION a);

MENUDATA menudata_top, menudata_run;



MENUDATA* menudata[] = {&menudata_top, &menudata_run    
};

static MENUSTATE menustate;
static bool displayf = false;   // should menu be drawn to screen.





/*
 * 
 */
bool menu_init(MENUSTATE m){
    menustate = m;
    displayf = true;
    return true;    
}

/*
 * 
 */
MENUEVENT menu(MENUACTION a) {
    MENUEVENT e;
    e.validf = false;
    if(displayf == true){
        if(!display_menu(*(menudata[menustate.id]))){
          return e;  
        }
        cursor(menustate.cursorat);
        displayf = false;
    }

    switch(menustate.id){
        
        case MID_TOP:
            e = menu_top(a);
            break;
        default:
            break;
    }
    return e;
}

/*
 * 
 */
MENUEVENT menu_top(MENUACTION a){
    
    
    
}




MENUDATA menudata_top = {
    MID_TOP,6,{{2,1,30},{3,1,33},{4,1,26},{2,2,15},{3,2,13},{4,2,24}}
};

MENUDATA menudata_run = {
    MID_RUN,12,{{2,0,14},{3,0,13},{4,0,15},{5,0,23},{2,1,14},{3,1,27},{4,1,32},
    {5,1,20},{3,2,28},{4,2,24},{3,3,28},{3,4,30}}
};


