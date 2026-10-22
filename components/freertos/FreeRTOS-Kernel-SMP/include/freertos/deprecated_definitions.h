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

#جے_بنا_تعریف DEPRECATED_DEFINITIONS_H
#تعریف_کرو DEPRECATED_DEFINITIONS_H


/* Each FreeRTOS port has a unique portmacro.h header file.  Originally a
 * pre-processor definition was used to ensure the pre-processor found the correct
 * portmacro.h file for the port being used.  That scheme was deprecated in favour
 * of setting the compiler's include path such that it found the correct
 * portmacro.h file - removing the need for the constant and allowing the
 * portmacro.h file to be located anywhere in relation to the port being used.  The
 * definitions below remain in the code for backward compatibility only.  New
 * projects should not use them. */

#جے_تعریف_شدہ OPEN_WATCOM_INDUSTRIAL_PC_پورٹ
    #ضمن "..\..\Source\portable\owatcom\16bitdos\pc\portmacro.h"
    تعريفنوع فراغ ( __interrupt __far * pxISR )();
#جے_انت

#جے_تعریف_شدہ OPEN_WATCOM_FLASH_LITE_186_پورٹ
    #ضمن "..\..\Source\portable\owatcom\16bitdos\flsh186\portmacro.h"
    تعريفنوع فراغ ( __interrupt __far * pxISR )();
#جے_انت

#جے_تعریف_شدہ GCC_MEGA_AVR
    #ضمن "../portable/GCC/ATMega323/portmacro.h"
#جے_انت

#جے_تعریف_شدہ IAR_MEGA_AVR
    #ضمن "../portable/IAR/ATMega323/portmacro.h"
#جے_انت

#جے_تعریف_شدہ MPLAB_PIC24_پورٹ
    #ضمن "../../Source/portable/MPLAB/PIC24_dsPIC/portmacro.h"
#جے_انت

#جے_تعریف_شدہ MPLAB_DSPIC_پورٹ
    #ضمن "../../Source/portable/MPLAB/PIC24_dsPIC/portmacro.h"
#جے_انت

#جے_تعریف_شدہ MPLAB_PIC18_F_پورٹ
    #ضمن "../../Source/portable/MPLAB/PIC18F/portmacro.h"
#جے_انت

#جے_تعریف_شدہ MPLAB_PIC32_MX_پورٹ
    #ضمن "../../Source/portable/MPLAB/PIC32MX/portmacro.h"
#جے_انت

#جے_تعریف_شدہ _FEDPICC
    #ضمن "libFreeRTOS/Include/portmacro.h"
#جے_انت

#جے_تعریف_شدہ SDCC_CYGNAL
    #ضمن "../../Source/portable/SDCC/Cygnal/portmacro.h"
#جے_انت

#جے_تعریف_شدہ GCC_ARM7
    #ضمن "../../Source/portable/GCC/ARM7_LPC2000/portmacro.h"
#جے_انت

#جے_تعریف_شدہ GCC_ARM7_ECLIPSE
    #ضمن "portmacro.h"
#جے_انت

#جے_تعریف_شدہ ROWLEY_LPC23xx
    #ضمن "../../Source/portable/GCC/ARM7_LPC23xx/portmacro.h"
#جے_انت

#جے_تعریف_شدہ IAR_MSP430
    #ضمن "..\..\Source\portable\IAR\MSP430\portmacro.h"
#جے_انت

#جے_تعریف_شدہ GCC_MSP430
    #ضمن "../../Source/portable/GCC/MSP430F449/portmacro.h"
#جے_انت

#جے_تعریف_شدہ ROWLEY_MSP430
    #ضمن "../../Source/portable/Rowley/MSP430F449/portmacro.h"
#جے_انت

#جے_تعریف_شدہ ARM7_LPC21xx_KEIL_RVDS
    #ضمن "..\..\Source\portable\RVDS\ARM7_LPC21xx\portmacro.h"
#جے_انت

#جے_تعریف_شدہ SAM7_GCC
    #ضمن "../../Source/portable/GCC/ARM7_AT91SAM7S/portmacro.h"
#جے_انت

#جے_تعریف_شدہ SAM7_IAR
    #ضمن "..\..\Source\portable\IAR\AtmelSAM7S64\portmacro.h"
#جے_انت

#جے_تعریف_شدہ SAM9XE_IAR
    #ضمن "..\..\Source\portable\IAR\AtmelSAM9XE\portmacro.h"
#جے_انت

#جے_تعریف_شدہ LPC2000_IAR
    #ضمن "..\..\Source\portable\IAR\LPC2000\portmacro.h"
#جے_انت

#جے_تعریف_شدہ STR71X_IAR
    #ضمن "..\..\Source\portable\IAR\STR71x\portmacro.h"
#جے_انت

#جے_تعریف_شدہ STR75X_IAR
    #ضمن "..\..\Source\portable\IAR\STR75x\portmacro.h"
#جے_انت

#جے_تعریف_شدہ STR75X_GCC
    #ضمن "..\..\Source\portable\GCC\STR75x\portmacro.h"
#جے_انت

#جے_تعریف_شدہ STR91X_IAR
    #ضمن "..\..\Source\portable\IAR\STR91x\portmacro.h"
#جے_انت

#جے_تعریف_شدہ GCC_H8S
    #ضمن "../../Source/portable/GCC/H8S2329/portmacro.h"
#جے_انت

#جے_تعریف_شدہ GCC_AT91FR40008
    #ضمن "../../Source/portable/GCC/ARM7_AT91FR40008/portmacro.h"
#جے_انت

#جے_تعریف_شدہ RVDS_ARMCM3_LM3S102
    #ضمن "../../Source/portable/RVDS/ARM_CM3/portmacro.h"
#جے_انت

#جے_تعریف_شدہ GCC_ARMCM3_LM3S102
    #ضمن "../../Source/portable/GCC/ARM_CM3/portmacro.h"
#جے_انت

#جے_تعریف_شدہ GCC_ARMCM3
    #ضمن "../../Source/portable/GCC/ARM_CM3/portmacro.h"
#جے_انت

#جے_تعریف_شدہ IAR_ARM_CM3
    #ضمن "../../Source/portable/IAR/ARM_CM3/portmacro.h"
#جے_انت

#جے_تعریف_شدہ IAR_ARMCM3_LM
    #ضمن "../../Source/portable/IAR/ARM_CM3/portmacro.h"
#جے_انت

#جے_تعریف_شدہ HCS12_کوڈ_WARRIOR
    #ضمن "../../Source/portable/CodeWarrior/HCS12/portmacro.h"
#جے_انت

#جے_تعریف_شدہ MICROBLAZE_GCC
    #ضمن "../../Source/portable/GCC/MicroBlaze/portmacro.h"
#جے_انت

#جے_تعریف_شدہ TERN_EE
    #ضمن "..\..\Source\portable\Paradigm\Tern_EE\small\portmacro.h"
#جے_انت

#جے_تعریف_شدہ GCC_HCS12
    #ضمن "../../Source/portable/GCC/HCS12/portmacro.h"
#جے_انت

#جے_تعریف_شدہ GCC_MCF5235
    #ضمن "../../Source/portable/GCC/MCF5235/portmacro.h"
#جے_انت

#جے_تعریف_شدہ COLDFIRE_V2_GCC
    #ضمن "../../../Source/portable/GCC/ColdFire_V2/portmacro.h"
#جے_انت

#جے_تعریف_شدہ COLDFIRE_V2_CODEWARRIOR
    #ضمن "../../Source/portable/CodeWarrior/ColdFire_V2/portmacro.h"
#جے_انت

#جے_تعریف_شدہ GCC_PPC405
    #ضمن "../../Source/portable/GCC/PPC405_Xilinx/portmacro.h"
#جے_انت

#جے_تعریف_شدہ GCC_PPC440
    #ضمن "../../Source/portable/GCC/PPC440_Xilinx/portmacro.h"
#جے_انت

#جے_تعریف_شدہ _16FX_SOFTUNE
    #ضمن "..\..\Source\portable\Softune\MB96340\portmacro.h"
#جے_انت

#جے_تعریف_شدہ BCC_INDUSTRIAL_PC_پورٹ

/* A short file name has to be used in place of the normal
 * FreeRTOSConfig.h when using the Borland compiler. */
    #ضمن "frconfig.h"
    #ضمن "..\portable\BCC\16BitDOS\PC\prtmacro.h"
    تعريفنوع فراغ ( __interrupt __far * pxISR )();
#جے_انت

#جے_تعریف_شدہ BCC_FLASH_LITE_186_پورٹ

/* A short file name has to be used in place of the normal
 * FreeRTOSConfig.h when using the Borland compiler. */
    #ضمن "frconfig.h"
    #ضمن "..\portable\BCC\16BitDOS\flsh186\prtmacro.h"
    تعريفنوع فراغ ( __interrupt __far * pxISR )();
#جے_انت

#جے_تعریف_شدہ __GNUC__
    #جے_تعریف_شدہ __AVR32_AVR32A__
        #ضمن "portmacro.h"
    #جے_انت
#جے_انت

#جے_تعریف_شدہ __ICCAVR32__
    #جے_تعریف_شدہ __CORE__
        #إذا __CORE__ == __AVR32A__
            #ضمن "portmacro.h"
        #جے_انت
    #جے_انت
#جے_انت

#جے_تعریف_شدہ __91467D
    #ضمن "portmacro.h"
#جے_انت

#جے_تعریف_شدہ __96340
    #ضمن "portmacro.h"
#جے_انت


#جے_تعریف_شدہ __IAR_V850ES_Fx3__
    #ضمن "../../Source/portable/IAR/V850ES/portmacro.h"
#جے_انت

#جے_تعریف_شدہ __IAR_V850ES_Jx3__
    #ضمن "../../Source/portable/IAR/V850ES/portmacro.h"
#جے_انت

#جے_تعریف_شدہ __IAR_V850ES_Jx3_L__
    #ضمن "../../Source/portable/IAR/V850ES/portmacro.h"
#جے_انت

#جے_تعریف_شدہ __IAR_V850ES_Jx2__
    #ضمن "../../Source/portable/IAR/V850ES/portmacro.h"
#جے_انت

#جے_تعریف_شدہ __IAR_V850ES_Hx2__
    #ضمن "../../Source/portable/IAR/V850ES/portmacro.h"
#جے_انت

#جے_تعریف_شدہ __IAR_78K0R_Kx3__
    #ضمن "../../Source/portable/IAR/78K0R/portmacro.h"
#جے_انت

#جے_تعریف_شدہ __IAR_78K0R_Kx3L__
    #ضمن "../../Source/portable/IAR/78K0R/portmacro.h"
#جے_انت

#جے_انت /* DEPRECATED_DEFINITIONS_H */
