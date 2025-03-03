/**
 *  Defines for your entire project at one place
 * 
 *	@author 	Tilen Majerle
 *	@email		tilen@majerle.eu
 *	@website	http://stm32f4-discovery.com
 *	@version 	v1.0
 *	@ide		Keil uVision 5
 *	@license	GNU GPL v3
 *	
 * |----------------------------------------------------------------------
 * | Copyright (C) Tilen Majerle, 2014
 * | 
 * | This program is free software: you can redistribute it and/or modify
 * | it under the terms of the GNU General Public License as published by
 * | the Free Software Foundation, either version 3 of the License, or
 * | any later version.
 * |  
 * | This program is distributed in the hope that it will be useful,
 * | but WITHOUT ANY WARRANTY; without even the implied warranty of
 * | MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * | GNU General Public License for more details.
 * | 
 * | You should have received a copy of the GNU General Public License
 * | along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * |----------------------------------------------------------------------
 */
#ifndef TM_DEFINES_H
#define TM_DEFINES_H

#define    FATFS_USE_SDIO                1

/* Put your global defines for all libraries here used in your project */
#define FATFS_SDIO_4BIT					 0

//Use detect pin
#define FATFS_USE_DETECT_PIN				0
//Use writeprotect pin
#define FATFS_USE_WRITEPROTECT_PIN			0
 
//If you want to overwrite default CD pin, then change this settings

/*
#define FATFS_USE_DETECT_PIN_RCC        RCC_AHB1Periph_GPIOB    
#define FATFS_USE_DETECT_PIN_PORT        GPIOB
#define FATFS_USE_DETECT_PIN_PIN        GPIO_Pin_15
*/

//If you want to overwrite default WP pin, then change this settings
/*
#define FATFS_USE_WRITEPROTECT_PIN_RCC        RCC_AHB1Periph_GPIOB    
#define FATFS_USE_WRITEPROTECT_PIN_PORT        GPIOB
#define FATFS_USE_WRITEPROTECT_PIN_PIN        GPIO_Pin_7
*/

#if 0
//Use SPI communication with SDCard
#define    FATFS_USE_SDIO                0

//Select your SPI settings
#define FATFS_SPI                SPI1
#define FATFS_SPI_PINSPACK        TM_SPI_PinsPack_1


//Custom CS pin for SPI communication
#define FATFS_CS_RCC        RCC_AHB1Periph_GPIOB            
#define FATFS_CS_PORT        GPIOB
#define FATFS_CS_PIN        GPIO_Pin_5
#endif

#endif
