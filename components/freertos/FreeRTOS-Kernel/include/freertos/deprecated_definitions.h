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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ DEPRECATED_DEFINITIONS_H
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DEPRECATED_DEFINITIONS_H


/* Each FreeRTOS port has a unique portmacro.h header file.  Originally a
 * pre-processor definition was used to ensure the pre-processor found the correct
 * portmacro.h file for the port being used.  That scheme was deprecated in favour
 * of setting the compiler's include path such that it found the correct
 * portmacro.h file - removing the need for the constant and allowing the
 * portmacro.h file to be located anywhere in relation to the port being used.  The
 * definitions below remain in the code for backward compatibility only.  New
 * projects should not use them. */

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ OPEN_WATCOM_INDUSTRIAL_PC_ਪੋਰਟ
    #ਅਨ੍ਤਰ੍ਭਾਵ "..\..\Source\portable\owatcom\16bitdos\pc\portmacro.h"
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ ( __interrupt __far * pxISR )();
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ OPEN_WATCOM_FLASH_LITE_186_ਪੋਰਟ
    #ਅਨ੍ਤਰ੍ਭਾਵ "..\..\Source\portable\owatcom\16bitdos\flsh186\portmacro.h"
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ ( __interrupt __far * pxISR )();
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ GCC_MEGA_AVR
    #ਅਨ੍ਤਰ੍ਭਾਵ "../portable/GCC/ATMega323/portmacro.h"
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ IAR_MEGA_AVR
    #ਅਨ੍ਤਰ੍ਭਾਵ "../portable/IAR/ATMega323/portmacro.h"
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ MPLAB_PIC24_ਪੋਰਟ
    #ਅਨ੍ਤਰ੍ਭਾਵ "../../Source/portable/MPLAB/PIC24_dsPIC/portmacro.h"
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ MPLAB_DSPIC_ਪੋਰਟ
    #ਅਨ੍ਤਰ੍ਭਾਵ "../../Source/portable/MPLAB/PIC24_dsPIC/portmacro.h"
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ MPLAB_PIC18_F_ਪੋਰਟ
    #ਅਨ੍ਤਰ੍ਭਾਵ "../../Source/portable/MPLAB/PIC18F/portmacro.h"
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ MPLAB_PIC32_MX_ਪੋਰਟ
    #ਅਨ੍ਤਰ੍ਭਾਵ "../../Source/portable/MPLAB/PIC32MX/portmacro.h"
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ _FEDPICC
    #ਅਨ੍ਤਰ੍ਭਾਵ "libFreeRTOS/Include/portmacro.h"
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ SDCC_CYGNAL
    #ਅਨ੍ਤਰ੍ਭਾਵ "../../Source/portable/SDCC/Cygnal/portmacro.h"
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ GCC_ARM7
    #ਅਨ੍ਤਰ੍ਭਾਵ "../../Source/portable/GCC/ARM7_LPC2000/portmacro.h"
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ GCC_ARM7_ECLIPSE
    #ਅਨ੍ਤਰ੍ਭਾਵ "portmacro.h"
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ROWLEY_LPC23xx
    #ਅਨ੍ਤਰ੍ਭਾਵ "../../Source/portable/GCC/ARM7_LPC23xx/portmacro.h"
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ IAR_MSP430
    #ਅਨ੍ਤਰ੍ਭਾਵ "..\..\Source\portable\IAR\MSP430\portmacro.h"
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ GCC_MSP430
    #ਅਨ੍ਤਰ੍ਭਾਵ "../../Source/portable/GCC/MSP430F449/portmacro.h"
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ROWLEY_MSP430
    #ਅਨ੍ਤਰ੍ਭਾਵ "../../Source/portable/Rowley/MSP430F449/portmacro.h"
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ARM7_LPC21xx_KEIL_RVDS
    #ਅਨ੍ਤਰ੍ਭਾਵ "..\..\Source\portable\RVDS\ARM7_LPC21xx\portmacro.h"
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ SAM7_GCC
    #ਅਨ੍ਤਰ੍ਭਾਵ "../../Source/portable/GCC/ARM7_AT91SAM7S/portmacro.h"
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ SAM7_IAR
    #ਅਨ੍ਤਰ੍ਭਾਵ "..\..\Source\portable\IAR\AtmelSAM7S64\portmacro.h"
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ SAM9XE_IAR
    #ਅਨ੍ਤਰ੍ਭਾਵ "..\..\Source\portable\IAR\AtmelSAM9XE\portmacro.h"
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ LPC2000_IAR
    #ਅਨ੍ਤਰ੍ਭਾਵ "..\..\Source\portable\IAR\LPC2000\portmacro.h"
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ STR71X_IAR
    #ਅਨ੍ਤਰ੍ਭਾਵ "..\..\Source\portable\IAR\STR71x\portmacro.h"
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ STR75X_IAR
    #ਅਨ੍ਤਰ੍ਭਾਵ "..\..\Source\portable\IAR\STR75x\portmacro.h"
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ STR75X_GCC
    #ਅਨ੍ਤਰ੍ਭਾਵ "..\..\Source\portable\GCC\STR75x\portmacro.h"
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ STR91X_IAR
    #ਅਨ੍ਤਰ੍ਭਾਵ "..\..\Source\portable\IAR\STR91x\portmacro.h"
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ GCC_H8S
    #ਅਨ੍ਤਰ੍ਭਾਵ "../../Source/portable/GCC/H8S2329/portmacro.h"
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ GCC_AT91FR40008
    #ਅਨ੍ਤਰ੍ਭਾਵ "../../Source/portable/GCC/ARM7_AT91FR40008/portmacro.h"
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ RVDS_ARMCM3_LM3S102
    #ਅਨ੍ਤਰ੍ਭਾਵ "../../Source/portable/RVDS/ARM_CM3/portmacro.h"
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ GCC_ARMCM3_LM3S102
    #ਅਨ੍ਤਰ੍ਭਾਵ "../../Source/portable/GCC/ARM_CM3/portmacro.h"
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ GCC_ARMCM3
    #ਅਨ੍ਤਰ੍ਭਾਵ "../../Source/portable/GCC/ARM_CM3/portmacro.h"
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ IAR_ARM_CM3
    #ਅਨ੍ਤਰ੍ਭਾਵ "../../Source/portable/IAR/ARM_CM3/portmacro.h"
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ IAR_ARMCM3_LM
    #ਅਨ੍ਤਰ੍ਭਾਵ "../../Source/portable/IAR/ARM_CM3/portmacro.h"
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HCS12_ਕੋਡ_WARRIOR
    #ਅਨ੍ਤਰ੍ਭਾਵ "../../Source/portable/CodeWarrior/HCS12/portmacro.h"
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ MICROBLAZE_GCC
    #ਅਨ੍ਤਰ੍ਭਾਵ "../../Source/portable/GCC/MicroBlaze/portmacro.h"
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ TERN_EE
    #ਅਨ੍ਤਰ੍ਭਾਵ "..\..\Source\portable\Paradigm\Tern_EE\small\portmacro.h"
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ GCC_HCS12
    #ਅਨ੍ਤਰ੍ਭਾਵ "../../Source/portable/GCC/HCS12/portmacro.h"
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ GCC_MCF5235
    #ਅਨ੍ਤਰ੍ਭਾਵ "../../Source/portable/GCC/MCF5235/portmacro.h"
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ COLDFIRE_V2_GCC
    #ਅਨ੍ਤਰ੍ਭਾਵ "../../../Source/portable/GCC/ColdFire_V2/portmacro.h"
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ COLDFIRE_V2_CODEWARRIOR
    #ਅਨ੍ਤਰ੍ਭਾਵ "../../Source/portable/CodeWarrior/ColdFire_V2/portmacro.h"
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ GCC_PPC405
    #ਅਨ੍ਤਰ੍ਭਾਵ "../../Source/portable/GCC/PPC405_Xilinx/portmacro.h"
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ GCC_PPC440
    #ਅਨ੍ਤਰ੍ਭਾਵ "../../Source/portable/GCC/PPC440_Xilinx/portmacro.h"
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ _16FX_SOFTUNE
    #ਅਨ੍ਤਰ੍ਭਾਵ "..\..\Source\portable\Softune\MB96340\portmacro.h"
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ BCC_INDUSTRIAL_PC_ਪੋਰਟ

/* A short file name has to be used in place of the normal
 * FreeRTOSConfig.h when using the Borland compiler. */
    #ਅਨ੍ਤਰ੍ਭਾਵ "frconfig.h"
    #ਅਨ੍ਤਰ੍ਭਾਵ "..\portable\BCC\16BitDOS\PC\prtmacro.h"
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ ( __interrupt __far * pxISR )();
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ BCC_FLASH_LITE_186_ਪੋਰਟ

/* A short file name has to be used in place of the normal
 * FreeRTOSConfig.h when using the Borland compiler. */
    #ਅਨ੍ਤਰ੍ਭਾਵ "frconfig.h"
    #ਅਨ੍ਤਰ੍ਭਾਵ "..\portable\BCC\16BitDOS\flsh186\prtmacro.h"
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ ( __interrupt __far * pxISR )();
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __GNUC__
    #ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __AVR32_AVR32A__
        #ਅਨ੍ਤਰ੍ਭਾਵ "portmacro.h"
    #ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __ICCAVR32__
    #ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __CORE__
        #ਜੇ __CORE__ == __AVR32A__
            #ਅਨ੍ਤਰ੍ਭਾਵ "portmacro.h"
        #ਜੇ_ਅੰਤ
    #ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __91467D
    #ਅਨ੍ਤਰ੍ਭਾਵ "portmacro.h"
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __96340
    #ਅਨ੍ਤਰ੍ਭਾਵ "portmacro.h"
#ਜੇ_ਅੰਤ


#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __IAR_V850ES_Fx3__
    #ਅਨ੍ਤਰ੍ਭਾਵ "../../Source/portable/IAR/V850ES/portmacro.h"
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __IAR_V850ES_Jx3__
    #ਅਨ੍ਤਰ੍ਭਾਵ "../../Source/portable/IAR/V850ES/portmacro.h"
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __IAR_V850ES_Jx3_L__
    #ਅਨ੍ਤਰ੍ਭਾਵ "../../Source/portable/IAR/V850ES/portmacro.h"
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __IAR_V850ES_Jx2__
    #ਅਨ੍ਤਰ੍ਭਾਵ "../../Source/portable/IAR/V850ES/portmacro.h"
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __IAR_V850ES_Hx2__
    #ਅਨ੍ਤਰ੍ਭਾਵ "../../Source/portable/IAR/V850ES/portmacro.h"
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __IAR_78K0R_Kx3__
    #ਅਨ੍ਤਰ੍ਭਾਵ "../../Source/portable/IAR/78K0R/portmacro.h"
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __IAR_78K0R_Kx3L__
    #ਅਨ੍ਤਰ੍ਭਾਵ "../../Source/portable/IAR/78K0R/portmacro.h"
#ਜੇ_ਅੰਤ

#ਜੇ_ਅੰਤ /* DEPRECATED_DEFINITIONS_H */
