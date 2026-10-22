/*
 * FreeRTOS Kernel V11.1.0
 * Copyright (C) 2021 Amazon.com, Inc. or its affiliates. All Rights Reserved.
 *
 * SPDX-FileCopyrightText: 2021 Amazon.com, Inc. or its affiliates
 *
 * SPDX-License-Identifier: MIT
 *
 * SPDX-FileContributor: 2023-2024 Espressif Systems (Shanghai) CO LTD
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * https://www.FreeRTOS.org
 * https://github.com/FreeRTOS
 *
 */

#अगर_अघोषित DEPRECATED_DEFINITIONS_H
#घोषणा DEPRECATED_DEFINITIONS_H


/* Each FreeRTOS port has a unique portmacro.h header file.  Originally a
 * pre-processor definition was used to ensure the pre-processor found the correct
 * portmacro.h file for the port being used.  That scheme was deprecated in favour
 * of setting the compiler's include path such that it found the correct
 * portmacro.h file - removing the need for the constant and allowing the
 * portmacro.h file to be located anywhere in relation to the port being used.  The
 * definitions below remain in the code for backward compatibility only.  New
 * projects should not use them. */

#अगर_घोषित OPEN_WATCOM_INDUSTRIAL_PC_PORT
    #समावेश "..\..\Source\portable\owatcom\16bitdos\pc\portmacro.h"
    प्रकार व्योम ( __interrupt __far * pxISR )();
#पूर्ण_अगर

#अगर_घोषित OPEN_WATCOM_FLASH_LITE_186_PORT
    #समावेश "..\..\Source\portable\owatcom\16bitdos\flsh186\portmacro.h"
    प्रकार व्योम ( __interrupt __far * pxISR )();
#पूर्ण_अगर

#अगर_घोषित GCC_MEGA_AVR
    #समावेश "../portable/GCC/ATMega323/portmacro.h"
#पूर्ण_अगर

#अगर_घोषित IAR_MEGA_AVR
    #समावेश "../portable/IAR/ATMega323/portmacro.h"
#पूर्ण_अगर

#अगर_घोषित MPLAB_PIC24_PORT
    #समावेश "../../Source/portable/MPLAB/PIC24_dsPIC/portmacro.h"
#पूर्ण_अगर

#अगर_घोषित MPLAB_DSPIC_PORT
    #समावेश "../../Source/portable/MPLAB/PIC24_dsPIC/portmacro.h"
#पूर्ण_अगर

#अगर_घोषित MPLAB_PIC18F_PORT
    #समावेश "../../Source/portable/MPLAB/PIC18F/portmacro.h"
#पूर्ण_अगर

#अगर_घोषित MPLAB_PIC32MX_PORT
    #समावेश "../../Source/portable/MPLAB/PIC32MX/portmacro.h"
#पूर्ण_अगर

#अगर_घोषित _FEDPICC
    #समावेश "libFreeRTOS/Include/portmacro.h"
#पूर्ण_अगर

#अगर_घोषित SDCC_CYGNAL
    #समावेश "../../Source/portable/SDCC/Cygnal/portmacro.h"
#पूर्ण_अगर

#अगर_घोषित GCC_ARM7
    #समावेश "../../Source/portable/GCC/ARM7_LPC2000/portmacro.h"
#पूर्ण_अगर

#अगर_घोषित GCC_ARM7_ECLIPSE
    #समावेश "portmacro.h"
#पूर्ण_अगर

#अगर_घोषित ROWLEY_LPC23xx
    #समावेश "../../Source/portable/GCC/ARM7_LPC23xx/portmacro.h"
#पूर्ण_अगर

#अगर_घोषित IAR_MSP430
    #समावेश "..\..\Source\portable\IAR\MSP430\portmacro.h"
#पूर्ण_अगर

#अगर_घोषित GCC_MSP430
    #समावेश "../../Source/portable/GCC/MSP430F449/portmacro.h"
#पूर्ण_अगर

#अगर_घोषित ROWLEY_MSP430
    #समावेश "../../Source/portable/Rowley/MSP430F449/portmacro.h"
#पूर्ण_अगर

#अगर_घोषित ARM7_LPC21xx_KEIL_RVDS
    #समावेश "..\..\Source\portable\RVDS\ARM7_LPC21xx\portmacro.h"
#पूर्ण_अगर

#अगर_घोषित SAM7_GCC
    #समावेश "../../Source/portable/GCC/ARM7_AT91SAM7S/portmacro.h"
#पूर्ण_अगर

#अगर_घोषित SAM7_IAR
    #समावेश "..\..\Source\portable\IAR\AtmelSAM7S64\portmacro.h"
#पूर्ण_अगर

#अगर_घोषित SAM9XE_IAR
    #समावेश "..\..\Source\portable\IAR\AtmelSAM9XE\portmacro.h"
#पूर्ण_अगर

#अगर_घोषित LPC2000_IAR
    #समावेश "..\..\Source\portable\IAR\LPC2000\portmacro.h"
#पूर्ण_अगर

#अगर_घोषित STR71X_IAR
    #समावेश "..\..\Source\portable\IAR\STR71x\portmacro.h"
#पूर्ण_अगर

#अगर_घोषित STR75X_IAR
    #समावेश "..\..\Source\portable\IAR\STR75x\portmacro.h"
#पूर्ण_अगर

#अगर_घोषित STR75X_GCC
    #समावेश "..\..\Source\portable\GCC\STR75x\portmacro.h"
#पूर्ण_अगर

#अगर_घोषित STR91X_IAR
    #समावेश "..\..\Source\portable\IAR\STR91x\portmacro.h"
#पूर्ण_अगर

#अगर_घोषित GCC_H8S
    #समावेश "../../Source/portable/GCC/H8S2329/portmacro.h"
#पूर्ण_अगर

#अगर_घोषित GCC_AT91FR40008
    #समावेश "../../Source/portable/GCC/ARM7_AT91FR40008/portmacro.h"
#पूर्ण_अगर

#अगर_घोषित RVDS_ARMCM3_LM3S102
    #समावेश "../../Source/portable/RVDS/ARM_CM3/portmacro.h"
#पूर्ण_अगर

#अगर_घोषित GCC_ARMCM3_LM3S102
    #समावेश "../../Source/portable/GCC/ARM_CM3/portmacro.h"
#पूर्ण_अगर

#अगर_घोषित GCC_ARMCM3
    #समावेश "../../Source/portable/GCC/ARM_CM3/portmacro.h"
#पूर्ण_अगर

#अगर_घोषित IAR_ARM_CM3
    #समावेश "../../Source/portable/IAR/ARM_CM3/portmacro.h"
#पूर्ण_अगर

#अगर_घोषित IAR_ARMCM3_LM
    #समावेश "../../Source/portable/IAR/ARM_CM3/portmacro.h"
#पूर्ण_अगर

#अगर_घोषित HCS12_CODE_WARRIOR
    #समावेश "../../Source/portable/CodeWarrior/HCS12/portmacro.h"
#पूर्ण_अगर

#अगर_घोषित MICROBLAZE_GCC
    #समावेश "../../Source/portable/GCC/MicroBlaze/portmacro.h"
#पूर्ण_अगर

#अगर_घोषित TERN_EE
    #समावेश "..\..\Source\portable\Paradigm\Tern_EE\small\portmacro.h"
#पूर्ण_अगर

#अगर_घोषित GCC_HCS12
    #समावेश "../../Source/portable/GCC/HCS12/portmacro.h"
#पूर्ण_अगर

#अगर_घोषित GCC_MCF5235
    #समावेश "../../Source/portable/GCC/MCF5235/portmacro.h"
#पूर्ण_अगर

#अगर_घोषित COLDFIRE_V2_GCC
    #समावेश "../../../Source/portable/GCC/ColdFire_V2/portmacro.h"
#पूर्ण_अगर

#अगर_घोषित COLDFIRE_V2_CODEWARRIOR
    #समावेश "../../Source/portable/CodeWarrior/ColdFire_V2/portmacro.h"
#पूर्ण_अगर

#अगर_घोषित GCC_PPC405
    #समावेश "../../Source/portable/GCC/PPC405_Xilinx/portmacro.h"
#पूर्ण_अगर

#अगर_घोषित GCC_PPC440
    #समावेश "../../Source/portable/GCC/PPC440_Xilinx/portmacro.h"
#पूर्ण_अगर

#अगर_घोषित _16FX_SOFTUNE
    #समावेश "..\..\Source\portable\Softune\MB96340\portmacro.h"
#पूर्ण_अगर

#अगर_घोषित BCC_INDUSTRIAL_PC_PORT

/* A short file name has to be used in place of the normal
 * FreeRTOSConfig.h when using the Borland compiler. */
    #समावेश "frconfig.h"
    #समावेश "..\portable\BCC\16BitDOS\PC\prtmacro.h"
    प्रकार व्योम ( __interrupt __far * pxISR )();
#पूर्ण_अगर

#अगर_घोषित BCC_FLASH_LITE_186_PORT

/* A short file name has to be used in place of the normal
 * FreeRTOSConfig.h when using the Borland compiler. */
    #समावेश "frconfig.h"
    #समावेश "..\portable\BCC\16BitDOS\flsh186\prtmacro.h"
    प्रकार व्योम ( __interrupt __far * pxISR )();
#पूर्ण_अगर

#अगर_घोषित __GNUC__
    #अगर_घोषित __AVR32_AVR32A__
        #समावेश "portmacro.h"
    #पूर्ण_अगर
#पूर्ण_अगर

#अगर_घोषित __ICCAVR32__
    #अगर_घोषित __CORE__
        #अगर __CORE__ == __AVR32A__
            #समावेश "portmacro.h"
        #पूर्ण_अगर
    #पूर्ण_अगर
#पूर्ण_अगर

#अगर_घोषित __91467D
    #समावेश "portmacro.h"
#पूर्ण_अगर

#अगर_घोषित __96340
    #समावेश "portmacro.h"
#पूर्ण_अगर


#अगर_घोषित __IAR_V850ES_Fx3__
    #समावेश "../../Source/portable/IAR/V850ES/portmacro.h"
#पूर्ण_अगर

#अगर_घोषित __IAR_V850ES_Jx3__
    #समावेश "../../Source/portable/IAR/V850ES/portmacro.h"
#पूर्ण_अगर

#अगर_घोषित __IAR_V850ES_Jx3_L__
    #समावेश "../../Source/portable/IAR/V850ES/portmacro.h"
#पूर्ण_अगर

#अगर_घोषित __IAR_V850ES_Jx2__
    #समावेश "../../Source/portable/IAR/V850ES/portmacro.h"
#पूर्ण_अगर

#अगर_घोषित __IAR_V850ES_Hx2__
    #समावेश "../../Source/portable/IAR/V850ES/portmacro.h"
#पूर्ण_अगर

#अगर_घोषित __IAR_78K0R_Kx3__
    #समावेश "../../Source/portable/IAR/78K0R/portmacro.h"
#पूर्ण_अगर

#अगर_घोषित __IAR_78K0R_Kx3L__
    #समावेश "../../Source/portable/IAR/78K0R/portmacro.h"
#पूर्ण_अगर

#पूर्ण_अगर /* DEPRECATED_DEFINITIONS_H */
