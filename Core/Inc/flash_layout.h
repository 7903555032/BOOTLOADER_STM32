/*
 * flash_layout.h
 *
 *  Created on: 18-Aug-2026
 *      Author: BIT
 */

#ifndef INC_FLASH_LAYOUT_H_
#define INC_FLASH_LAYOUT_H_

#include"bl_jump.h"

#define BL_START_ADDR       0x08000000U //32KB  Sector 0 and Sector 1
#define APP_HEADER_ADDR     0x08008000U  //16KB  Sector 2
#define APP_START_ADDR      0x0800C000U  //464KB



#endif /* INC_FLASH_LAYOUT_H_ */
