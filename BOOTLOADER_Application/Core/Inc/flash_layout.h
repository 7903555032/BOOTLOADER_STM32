/*
 * flash_layout.h
 *
 *  Created on: 18-Aug-2026
 *      Author: BIT
 */

#ifndef INC_FLASH_LAYOUT_H_
#define INC_FLASH_LAYOUT_H_



#define BOOTLOADER_START_ADDR   0x08000000U
#define BOOTLOADER_END_ADDR     0x08007FFFU

#define APP_HEADER_ADDR         0x08008000U
#define APP_HEADER_END_ADDR     0x0800BFFFU

#define APP_START_ADDR          0x0800C000U
#define APP_END_ADDR            0x0807FFFFU
#define APP_MAX_SIZE            464*1024


#endif /* INC_FLASH_LAYOUT_H_ */
