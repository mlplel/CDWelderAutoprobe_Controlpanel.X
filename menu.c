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
MENUEVENT menu_run(MENUACTION a);
MENUEVENT menu_runboth(MENUACTION a);

MENUDATA menudata_top, menudata_run, menudata_cal, menudata_runboth, menudata_runpl,
        menudata_runpr, menudata_calpl1, menudata_calpl2, menudata_calpr1,menudata_calpr2;



MENUDATA* menudata[] = {&menudata_top, &menudata_run, &menudata_cal, &menudata_runboth,
    &menudata_runpl, &menudata_runpr, &menudata_calpl1, &menudata_calpl2, 
    &menudata_calpr1, &menudata_calpr2
};

static MENUSTATE menustate;
static bool displayf = false;   // should menu be drawn to screen.
static bool cursorf = true;     // should cursor blink





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
    static uint16_t cursorcount = 0;
    MENUEVENT e;
    e.validf = false;
    if(displayf == true){
        if(!display_menu(*(menudata[menustate.id]))){
          return e;  
        }
        cursor(menustate.cursorat);
        displayf = false;
        a.msg = ME_MENUINIT;
    } 
    if(cursorf) cursorcount++;
    if(cursorcount == 400){
        cursorflash();
        cursorcount = 0;
    }
   

    switch(menustate.id){
        
        case MID_TOP:
            e = menu_top(a);
            break;
        case MID_RUN:
            e = menu_run(a);
            break;
        case MID_CAL:
            
            break;
        case MID_RUNBOTH:
            e = menu_runboth(a);
            break;            
        default:
            break;
    }
    return e;
}

/*
 * 
 */
MENUEVENT menu_top(MENUACTION a) {
    MENUEVENT e;
    e.validf = false;

    if (a.se == SW_CLICKED || a.se == SW_DCLICKED) {
        menustate.cursorat.visible = false;
        cursor(menustate.cursorat);
        e.msg = ME_MENUCHANGED;
        e.validf = true;
        if (menustate.cursorat.cursorline == 1) {
            menustate.id = MID_RUN;
        } else {
            menustate.id = MID_CAL;
        }
        menustate.cursorat.cursorcol = 0;
        menustate.cursorat.cursorline = 0;
        menustate.cursorat.visible = true;
        displayf = true;
    }
    else if(a.enc != 0){
        if(menustate.cursorat.cursorline == 1){
            menustate.cursorat.cursorline = 2;
        } else {
            menustate.cursorat.cursorline = 1;
        }
        cursor(menustate.cursorat);
        e.msg = ME_SELCHANGED;
        e.validf = true;
    }
    return e;
}


/*
 * 
 */
MENUEVENT menu_run(MENUACTION a) {
    MENUEVENT e;
    e.validf = false;

    if (a.se == SW_CLICKED || a.se == SW_DCLICKED) {
        menustate.cursorat.visible = false;
        cursor(menustate.cursorat);
        e.msg = ME_MENUCHANGED;
        e.validf = true;
        if (menustate.cursorat.cursorline == 0) {
            menustate.id = MID_TOP;
            menustate.cursorat.cursorline = 1;
        } else if(menustate.cursorat.cursorline == 1){
            menustate.id = MID_RUNBOTH;
            menustate.cursorat.cursorline = 0;
        } else if(menustate.cursorat.cursorline == 2){
            menustate.id = MID_RUNPL;
            menustate.cursorat.cursorline = 0;
        } else if (menustate.cursorat.cursorline == 3){
            menustate.id = MID_RUNPR;
            menustate.cursorat.cursorline = 0;
        }
        menustate.cursorat.cursorcol = 0;
        menustate.cursorat.visible = true;
        displayf = true;
    }
    else if(a.enc != 0){
        if(a.enc >= 1){
            if(menustate.cursorat.cursorline == 3){
               menustate.cursorat.cursorline = 0; 
            } else {
               menustate.cursorat.cursorline++; 
            }
        } else {
             if(menustate.cursorat.cursorline == 0){
               menustate.cursorat.cursorline = 3; 
            } else {
               menustate.cursorat.cursorline--; 
            }
        }
        cursor(menustate.cursorat);
        e.msg = ME_SELCHANGED;
        e.validf = true;
    }

    return e;
}

/*
 * 
 */
MENUEVENT menu_runboth(MENUACTION a){
    static bool entryf = false;
    static bool bothf = false;
    static uint16_t initcnt = 0;
    MENUEVENT e;
    e.validf = false;
    e.msdata = &menustate;
        
    // run when menu is 1st displayed.
    if(a.msg == ME_MENUINIT) initcnt = 2;
    if(initcnt != 0){
        if(!display_2digit(menustate.plsettings.probevalue,7,1)) return e;
        initcnt--;
        if(!display_2digit(menustate.prsettings.probevalue,7,2)) return e;
        initcnt--;
        menustate.plsettings.probeactive = true;
        menustate.prsettings.probeactive = true;
    }    

    if (a.se == SW_CLICKED || a.se == SW_DCLICKED) {
        if (menustate.cursorat.cursorline == 0) {
            menustate.cursorat.visible = false;
            cursor(menustate.cursorat);
            e.msg = ME_MENUCHANGED;
            e.validf = true;
            menustate.id = MID_RUN;
            menustate.cursorat.cursorline = 0;
            menustate.cursorat.cursorcol = 0;
            menustate.cursorat.visible = true;
            displayf = true;
        } else {
            e.msg = ME_ENTRYSELC;
            e.validf = true;
            entryf = !entryf;
            cursorf = !entryf;
            cursor(menustate.cursorat);
        }        
    } else if (a.enc != 0) {
        if (entryf) {
            // entry value change here.
            if (bothf || (menustate.cursorat.cursorline == 1)) {
                if (a.enc > 0) {
                    if (menustate.plsettings.probevalue < 15)
                        menustate.plsettings.probevalue++;                    
                } else {
                    if (menustate.plsettings.probevalue > 1)
                        menustate.plsettings.probevalue--;
                }
                display_2digit(menustate.plsettings.probevalue, 7, 1);
            }
            if (bothf || (menustate.cursorat.cursorline == 2)) {
                if (a.enc > 0) {
                    if (menustate.prsettings.probevalue < 15)
                        menustate.prsettings.probevalue++;                    
                } else {
                    if (menustate.prsettings.probevalue > 1)
                        menustate.prsettings.probevalue--;
                }
                display_2digit(menustate.prsettings.probevalue, 7, 2);
            }
            e.msg = ME_PROBEVALUE;
            e.validf = true;

        } else {
            // menu selection change here.
            if (a.enc >= 1) {
                if (menustate.cursorat.cursorline == 2) {
                    if(bothf){
                     menustate.cursorat.cursorline = 0; 
                     bothf = false;
                    } else {
                        bothf = true;
                    }
                    
                } else {
                    menustate.cursorat.cursorline++;
                }
            } else {
                if (menustate.cursorat.cursorline == 0) {
                    menustate.cursorat.cursorline = 2;
                    bothf = true;
                } else {
                    if((menustate.cursorat.cursorline == 2) && (bothf == true)){
                        bothf = false;
                    } else {
                        menustate.cursorat.cursorline--;
                    }
                }
            }
            cursor(menustate.cursorat);
            e.msg = ME_SELCHANGED;
            e.validf = true;
            if(menustate.cursorat.cursorline == 0){
                sh1106_charat(1,1,0);
                sh1106_charat(1,2,0);
            } else {
               if(bothf){
                   sh1106_charat(1,1,39);
                   sh1106_charat(1,2,39);
               } else {
                   sh1106_charat(1,1,(menustate.cursorat.cursorline == 1)?39:0);
                   sh1106_charat(1,2,(menustate.cursorat.cursorline == 2)?39:0);
               }
            }
        }
    }
    return e;   
}



MENUDATA menudata_top = {
    MID_TOP,6,{{2,1,30},{3,1,33},{4,1,26},{2,2,15},{3,2,13},{4,2,24}}
};

MENUDATA menudata_run = {
    MID_RUN,12,{{2,0,14},{3,0,13},{4,0,15},{5,0,23},{2,1,14},{3,1,27},{4,1,32},
    {5,1,20},{3,2,28},{4,2,24},{3,3,28},{4,3,30}}
};

MENUDATA menudata_cal = {
    MID_CAL,20,{{2,0,14},{3,0,13},{4,0,15},{5,0,23},{2,1,38},{3,1,17},{4,1,30},
    {5,1,27},{2,2,15},{3,2,13},{4,2,24},{5,2,0},{6,2,28},{7,2,24},{2,3,15},
    {3,3,13},{4,3,24},{5,3,0},{6,3,28},{7,3,30}}
};

MENUDATA menudata_runboth = {
    MID_RUNBOTH,12,{{2,0,14},{3,0,13},{4,0,15},{5,0,23},{3,1,28},{4,1,24},{9,1,24},
    {10,1,14},{3,2,28},{4,2,30},{9,2,24},{10,2,14}}
};

MENUDATA menudata_runpl = {
    MID_RUNPL,8,{{2,0,14},{3,0,13},{4,0,15},{5,0,23},{3,2,28},
    {4,2,24},{9,2,24},{10,2,14}}    
};

MENUDATA menudata_runpr = {
    MID_RUNPR,8,{{2,0,14},{3,0,13},{4,0,15},{5,0,23},{3,2,28},
    {4,2,30},{9,2,24},{10,2,14}}    
};

MENUDATA menudata_calpl1 = {
    MID_CALPL1,16,{{2,0,14},{3,0,13},{4,0,15},{5,0,23},{8,0,25},{9,0,17},{10,0,36},
    {11,0,32},{2,1,28},{3,1,30},{4,1,17},{5,1,31},{2,2,23},{3,2,28},{2,3,23},{3,3,21}}
};

MENUDATA menudata_calpl2 = {
    MID_CALPL2,16,{{2,0,14},{3,0,13},{4,0,15},{5,0,23},{2,1,21},{3,1,24},{4,1,21},
    {5,1,25},{6,1,32},{2,2,27},{3,2,24},{4,2,21},{5,2,25},{6,2,32},{2,3,23},{3,3,16}}
};

MENUDATA menudata_calpr1 = {
    MID_CALPR1,16,{{2,0,14},{3,0,13},{4,0,15},{5,0,23},{8,0,25},{9,0,17},{10,0,36},
    {11,0,32},{2,1,28},{3,1,30},{4,1,17},{5,1,31},{2,2,23},{3,2,28},{2,3,23},{3,3,21}}
};

MENUDATA menudata_calpr2 = {
    MID_CALPR2,16,{{2,0,14},{3,0,13},{4,0,15},{5,0,23},{2,1,21},{3,1,24},{4,1,21},
    {5,1,25},{6,1,32},{2,2,27},{3,2,24},{4,2,21},{5,2,25},{6,2,32},{2,3,23},{3,3,16}}
};