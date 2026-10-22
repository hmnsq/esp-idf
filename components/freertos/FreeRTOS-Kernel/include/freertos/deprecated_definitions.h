/*
 * FreeRTOS Kernel V10.5.1 (ESP-IDF SMP modified)
 * Copyright (C) 2021 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * SPDX-FileCopyrightText: 2021 Amazon.com, Inc. or its affiliates
 *
 * SPDX-License-Identifier: MIT
 *
 * SPDX-FileContributor: 2023 Espressif Systems (Shanghai) CO LTD
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

#ifndef DEPRECATED_DEFINITIONS_H
#define DEPRECATED_DEFINITIONS_H


/* Each FreeRTOS port has a unique portmacro.h header file.  Originally a
 * pre-processor definition was used to ensure the pre-processor found the correct
 * portmacro.h file for the port being used.  That scheme was deprecated in favour
 * of setting the compiler's include path such that it found the correct
 * portmacro.h file - removing the need for the constant and allowing the
 * portmacro.h file to be located anywhere in relation to the port being used.  The
 * definitions below remain in the code for backward compatibility only.  New
 * projects should not use them. */

#ifdef OPEN_WATCOM_INDUSTRIAL_PC_PORT
    #અન્તર્ભાવ "..\..\Source\portable\owatcom\16bitdos\pc\portmacro.h"
    પ્રકારવ્યાખ્યા શૂન્ય ( __interrupt __far * pxISR )();
#endif

#ifdef OPEN_WATCOM_FLASH_LITE_186_PORT
    #અન્તર્ભાવ "..\..\Source\portable\owatcom\16bitdos\flsh186\portmacro.h"
    પ્રકારવ્યાખ્યા શૂન્ય ( __interrupt __far * pxISR )();
#endif

#ifdef GCC_MEGA_AVR
    #અન્તર્ભાવ "../portable/GCC/ATMega323/portmacro.h"
#endif

#ifdef IAR_MEGA_AVR
    #અન્તર્ભાવ "../portable/IAR/ATMega323/portmacro.h"
#endif

#ifdef MPLAB_PIC24_PORT
    #અન્તર્ભાવ "../../Source/portable/MPLAB/PIC24_dsPIC/portmacro.h"
#endif

#ifdef MPLAB_DSPIC_PORT
    #અન્તર્ભાવ "../../Source/portable/MPLAB/PIC24_dsPIC/portmacro.h"
#endif

#ifdef MPLAB_PIC18F_PORT
    #અન્તર્ભાવ "../../Source/portable/MPLAB/PIC18F/portmacro.h"
#endif

#ifdef MPLAB_PIC32MX_PORT
    #અન્તર્ભાવ "../../Source/portable/MPLAB/PIC32MX/portmacro.h"
#endif

#ifdef _FEDPICC
    #અન્તર્ભાવ "libFreeRTOS/Include/portmacro.h"
#endif

#ifdef SDCC_CYGNAL
    #અન્તર્ભાવ "../../Source/portable/SDCC/Cygnal/portmacro.h"
#endif

#ifdef GCC_ARM7
    #અન્તર્ભાવ "../../Source/portable/GCC/ARM7_LPC2000/portmacro.h"
#endif

#ifdef GCC_ARM7_ECLIPSE
    #અન્તર્ભાવ "portmacro.h"
#endif

#ifdef ROWLEY_LPC23xx
    #અન્તર્ભાવ "../../Source/portable/GCC/ARM7_LPC23xx/portmacro.h"
#endif

#ifdef IAR_MSP430
    #અન્તર્ભાવ "..\..\Source\portable\IAR\MSP430\portmacro.h"
#endif

#ifdef GCC_MSP430
    #અન્તર્ભાવ "../../Source/portable/GCC/MSP430F449/portmacro.h"
#endif

#ifdef ROWLEY_MSP430
    #અન્તર્ભાવ "../../Source/portable/Rowley/MSP430F449/portmacro.h"
#endif

#ifdef ARM7_LPC21xx_KEIL_RVDS
    #અન્તર્ભાવ "..\..\Source\portable\RVDS\ARM7_LPC21xx\portmacro.h"
#endif

#ifdef SAM7_GCC
    #અન્તર્ભાવ "../../Source/portable/GCC/ARM7_AT91SAM7S/portmacro.h"
#endif

#ifdef SAM7_IAR
    #અન્તર્ભાવ "..\..\Source\portable\IAR\AtmelSAM7S64\portmacro.h"
#endif

#ifdef SAM9XE_IAR
    #અન્તર્ભાવ "..\..\Source\portable\IAR\AtmelSAM9XE\portmacro.h"
#endif

#ifdef LPC2000_IAR
    #અન્તર્ભાવ "..\..\Source\portable\IAR\LPC2000\portmacro.h"
#endif

#ifdef STR71X_IAR
    #અન્તર્ભાવ "..\..\Source\portable\IAR\STR71x\portmacro.h"
#endif

#ifdef STR75X_IAR
    #અન્તર્ભાવ "..\..\Source\portable\IAR\STR75x\portmacro.h"
#endif

#ifdef STR75X_GCC
    #અન્તર્ભાવ "..\..\Source\portable\GCC\STR75x\portmacro.h"
#endif

#ifdef STR91X_IAR
    #અન્તર્ભાવ "..\..\Source\portable\IAR\STR91x\portmacro.h"
#endif

#ifdef GCC_H8S
    #અન્તર્ભાવ "../../Source/portable/GCC/H8S2329/portmacro.h"
#endif

#ifdef GCC_AT91FR40008
    #અન્તર્ભાવ "../../Source/portable/GCC/ARM7_AT91FR40008/portmacro.h"
#endif

#ifdef RVDS_ARMCM3_LM3S102
    #અન્તર્ભાવ "../../Source/portable/RVDS/ARM_CM3/portmacro.h"
#endif

#ifdef GCC_ARMCM3_LM3S102
    #અન્તર્ભાવ "../../Source/portable/GCC/ARM_CM3/portmacro.h"
#endif

#ifdef GCC_ARMCM3
    #અન્તર્ભાવ "../../Source/portable/GCC/ARM_CM3/portmacro.h"
#endif

#ifdef IAR_ARM_CM3
    #અન્તર્ભાવ "../../Source/portable/IAR/ARM_CM3/portmacro.h"
#endif

#ifdef IAR_ARMCM3_LM
    #અન્તર્ભાવ "../../Source/portable/IAR/ARM_CM3/portmacro.h"
#endif

#ifdef HCS12_CODE_WARRIOR
    #અન્તર્ભાવ "../../Source/portable/CodeWarrior/HCS12/portmacro.h"
#endif

#ifdef MICROBLAZE_GCC
    #અન્તર્ભાવ "../../Source/portable/GCC/MicroBlaze/portmacro.h"
#endif

#ifdef TERN_EE
    #અન્તર્ભાવ "..\..\Source\portable\Paradigm\Tern_EE\small\portmacro.h"
#endif

#ifdef GCC_HCS12
    #અન્તર્ભાવ "../../Source/portable/GCC/HCS12/portmacro.h"
#endif

#ifdef GCC_MCF5235
    #અન્તર્ભાવ "../../Source/portable/GCC/MCF5235/portmacro.h"
#endif

#ifdef COLDFIRE_V2_GCC
    #અન્તર્ભાવ "../../../Source/portable/GCC/ColdFire_V2/portmacro.h"
#endif

#ifdef COLDFIRE_V2_CODEWARRIOR
    #અન્તર્ભાવ "../../Source/portable/CodeWarrior/ColdFire_V2/portmacro.h"
#endif

#ifdef GCC_PPC405
    #અન્તર્ભાવ "../../Source/portable/GCC/PPC405_Xilinx/portmacro.h"
#endif

#ifdef GCC_PPC440
    #અન્તર્ભાવ "../../Source/portable/GCC/PPC440_Xilinx/portmacro.h"
#endif

#ifdef _16FX_SOFTUNE
    #અન્તર્ભાવ "..\..\Source\portable\Softune\MB96340\portmacro.h"
#endif

#ifdef BCC_INDUSTRIAL_PC_PORT

/* A short file name has to be used in place of the normal
 * FreeRTOSConfig.h when using the Borland compiler. */
    #અન્તર્ભાવ "frconfig.h"
    #અન્તર્ભાવ "..\portable\BCC\16BitDOS\PC\prtmacro.h"
    પ્રકારવ્યાખ્યા શૂન્ય ( __interrupt __far * pxISR )();
#endif

#ifdef BCC_FLASH_LITE_186_PORT

/* A short file name has to be used in place of the normal
 * FreeRTOSConfig.h when using the Borland compiler. */
    #અન્તર્ભાવ "frconfig.h"
    #અન્તર્ભાવ "..\portable\BCC\16BitDOS\flsh186\prtmacro.h"
    પ્રકારવ્યાખ્યા શૂન્ય ( __interrupt __far * pxISR )();
#endif

#ifdef __GNUC__
    #ifdef __AVR32_AVR32A__
        #અન્તર્ભાવ "portmacro.h"
    #endif
#endif

#ifdef __ICCAVR32__
    #ifdef __CORE__
        #જો __CORE__ == __AVR32A__
            #અન્તર્ભાવ "portmacro.h"
        #endif
    #endif
#endif

#ifdef __91467D
    #અન્તર્ભાવ "portmacro.h"
#endif

#ifdef __96340
    #અન્તર્ભાવ "portmacro.h"
#endif


#ifdef __IAR_V850ES_Fx3__
    #અન્તર્ભાવ "../../Source/portable/IAR/V850ES/portmacro.h"
#endif

#ifdef __IAR_V850ES_Jx3__
    #અન્તર્ભાવ "../../Source/portable/IAR/V850ES/portmacro.h"
#endif

#ifdef __IAR_V850ES_Jx3_L__
    #અન્તર્ભાવ "../../Source/portable/IAR/V850ES/portmacro.h"
#endif

#ifdef __IAR_V850ES_Jx2__
    #અન્તર્ભાવ "../../Source/portable/IAR/V850ES/portmacro.h"
#endif

#ifdef __IAR_V850ES_Hx2__
    #અન્તર્ભાવ "../../Source/portable/IAR/V850ES/portmacro.h"
#endif

#ifdef __IAR_78K0R_Kx3__
    #અન્તર્ભાવ "../../Source/portable/IAR/78K0R/portmacro.h"
#endif

#ifdef __IAR_78K0R_Kx3L__
    #અન્તર્ભાવ "../../Source/portable/IAR/78K0R/portmacro.h"
#endif

#endif /* DEPRECATED_DEFINITIONS_H */
