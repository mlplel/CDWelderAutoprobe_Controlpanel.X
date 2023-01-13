/* 
 * File:   spi.h
 * Author: 
 *
 * Created on May 27, 2019, 1:17 PM
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

#ifndef SPI_H
#define	SPI_H

#ifdef	__cplusplus
extern "C" {
#endif
    

#include "stddef.h"
#include <stdbool.h>
#include "xc.h"
#include "messages.h"
    
    
#define EEPROM_WREN    0x06        // eeprom set write enable latch
#define EEPROM_WRDI    0x04        // eeprom reset write enable latch
#define EEPROM_RDSR    0x05        // eeprom read status reg
#define EEPROM_WRSR    0x01        // eeprom write status reg
#define EEPROM_READ    0x03        // eeprom read data 
#define EEPROM_WRITE   0x02        // eeprom write data
#define EEPROM_RDY     0x01        // eeprom status ready bit
#define EEPROM_WEN     0x02        // eeprom status write enable bit
#define EEPROM_DUMMY   0xAA        // eeprom dummy data   
    
#define EEPROM_BLOCKSIZE 16          

    typedef enum {
        OFF,
        ON,
        WRITEENABLE,
        READSTATUS1,
        READSTATUS2,
        READ1,
        READ2,
        READ3,
        READ4,
        READREADY,
        WRITE1,
        WRITE2,
        WRITE3,
        WRITE4,
    } EESTATE;
    
    
    typedef enum {
        READY,
        WPON,
        BUSY,
        UNKNOWN,
    } EESTATUS;

   

    void spi_IOSetup(void);
    void spi1_Init(void);
    void spi2_Init(void);
    
/**
  @Summary
 
  @Description

  @Preconditions

  @Returns

  @Param

  @Example
*/    
void commresync(void);

    uint8_t spi1_TxByte(uint8_t data);

    void mem_WriteByte(uint8_t data, uint16_t addr);
    uint8_t mem_ReadStatus(void);
    void mem_WriteEnable(void);
    uint8_t mem_ReadByte(uint16_t addr);
    uint8_t mem_ReadByte(uint16_t addr);
    
    // eeprom access routines
    void mem_start(uint8_t cmd);
    bool mem_isidle(void);
    EESTATUS mem_getwritestatus(void);
    EESTATUS mem_getreadstatus(void);
    void mem_writeenable(void);
    void mem_writebyte(uint8_t data, uint16_t addr);
    void mem_writeblock(uint8_t *data, uint16_t addr);
    void mem_readbyteaddr(uint16_t addr);
    void mem_readblockaddr(uint16_t addr);
    uint8_t mem_readbyte(void);
    uint8_t mem_readblock(uint8_t* data);
    
    // communication routines
    MAINMSG getMsg(void);
    MAINMSG get_msg(void);

    bool isNewMsg(); 
    bool is_newmsg(void);
    
    //int16_t setMsg(MAINMSG msg);
    
    //bool isTxBusy();
    
    int16_t put_msg(MAINMSG msg);
    bool is_txbusy(void);

#ifdef	__cplusplus
}
#endif

#endif	/* SPI_H */

