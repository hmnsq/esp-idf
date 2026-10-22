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

#यदि_अपरिभाषितम् DEPRECATED_DEFINITIONS_H
#परिभाषय DEPRECATED_DEFINITIONS_H


/* Each FreeRTOS port has a unique portmacro.h header file.  Originally a
 * pre-processor definition was used to ensure the pre-processor found the correct
 * portmacro.h file for the port being used.  That scheme was deprecated in favour
 * of setting the compiler's include path such that it found the correct
 * portmacro.h file - removing the need for the constant and allowing the
 * portmacro.h file to be located anywhere in relation to the port being used.  The
 * definitions below remain in the code for backward compatibility only.  New
 * projects should not use them. */

#यदि_परिभाषितम् OPEN_WATCOM_INDUSTRIAL_PC_पत्तनम्
    #अन्तर्भाव "..\..\Source\portable\owatcom\16bitdos\pc\portmacro.h"
    प्रकारनिर्देश शून्य ( __interrupt __far * pxISR )();
#यदि_अन्तः

#यदि_परिभाषितम् OPEN_WATCOM_FLASH_LITE_186_पत्तनम्
    #अन्तर्भाव "..\..\Source\portable\owatcom\16bitdos\flsh186\portmacro.h"
    प्रकारनिर्देश शून्य ( __interrupt __far * pxISR )();
#यदि_अन्तः

#यदि_परिभाषितम् GCC_MEGA_AVR
    #अन्तर्भाव "../portable/GCC/ATMega323/portmacro.h"
#यदि_अन्तः

#यदि_परिभाषितम् IAR_MEGA_AVR
    #अन्तर्भाव "../portable/IAR/ATMega323/portmacro.h"
#यदि_अन्तः

#यदि_परिभाषितम् MPLAB_PIC24_पत्तनम्
    #अन्तर्भाव "../../Source/portable/MPLAB/PIC24_dsPIC/portmacro.h"
#यदि_अन्तः

#यदि_परिभाषितम् MPLAB_DSPIC_पत्तनम्
    #अन्तर्भाव "../../Source/portable/MPLAB/PIC24_dsPIC/portmacro.h"
#यदि_अन्तः

#यदि_परिभाषितम् MPLAB_PIC18_F_पत्तनम्
    #अन्तर्भाव "../../Source/portable/MPLAB/PIC18F/portmacro.h"
#यदि_अन्तः

#यदि_परिभाषितम् MPLAB_PIC32_MX_पत्तनम्
    #अन्तर्भाव "../../Source/portable/MPLAB/PIC32MX/portmacro.h"
#यदि_अन्तः

#यदि_परिभाषितम् _FEDPICC
    #अन्तर्भाव "libFreeRTOS/Include/portmacro.h"
#यदि_अन्तः

#यदि_परिभाषितम् SDCC_CYGNAL
    #अन्तर्भाव "../../Source/portable/SDCC/Cygnal/portmacro.h"
#यदि_अन्तः

#यदि_परिभाषितम् GCC_ARM7
    #अन्तर्भाव "../../Source/portable/GCC/ARM7_LPC2000/portmacro.h"
#यदि_अन्तः

#यदि_परिभाषितम् GCC_ARM7_ECLIPSE
    #अन्तर्भाव "portmacro.h"
#यदि_अन्तः

#यदि_परिभाषितम् ROWLEY_LPC23xx
    #अन्तर्भाव "../../Source/portable/GCC/ARM7_LPC23xx/portmacro.h"
#यदि_अन्तः

#यदि_परिभाषितम् IAR_MSP430
    #अन्तर्भाव "..\..\Source\portable\IAR\MSP430\portmacro.h"
#यदि_अन्तः

#यदि_परिभाषितम् GCC_MSP430
    #अन्तर्भाव "../../Source/portable/GCC/MSP430F449/portmacro.h"
#यदि_अन्तः

#यदि_परिभाषितम् ROWLEY_MSP430
    #अन्तर्भाव "../../Source/portable/Rowley/MSP430F449/portmacro.h"
#यदि_अन्तः

#यदि_परिभाषितम् ARM7_LPC21xx_KEIL_RVDS
    #अन्तर्भाव "..\..\Source\portable\RVDS\ARM7_LPC21xx\portmacro.h"
#यदि_अन्तः

#यदि_परिभाषितम् SAM7_GCC
    #अन्तर्भाव "../../Source/portable/GCC/ARM7_AT91SAM7S/portmacro.h"
#यदि_अन्तः

#यदि_परिभाषितम् SAM7_IAR
    #अन्तर्भाव "..\..\Source\portable\IAR\AtmelSAM7S64\portmacro.h"
#यदि_अन्तः

#यदि_परिभाषितम् SAM9XE_IAR
    #अन्तर्भाव "..\..\Source\portable\IAR\AtmelSAM9XE\portmacro.h"
#यदि_अन्तः

#यदि_परिभाषितम् LPC2000_IAR
    #अन्तर्भाव "..\..\Source\portable\IAR\LPC2000\portmacro.h"
#यदि_अन्तः

#यदि_परिभाषितम् STR71X_IAR
    #अन्तर्भाव "..\..\Source\portable\IAR\STR71x\portmacro.h"
#यदि_अन्तः

#यदि_परिभाषितम् STR75X_IAR
    #अन्तर्भाव "..\..\Source\portable\IAR\STR75x\portmacro.h"
#यदि_अन्तः

#यदि_परिभाषितम् STR75X_GCC
    #अन्तर्भाव "..\..\Source\portable\GCC\STR75x\portmacro.h"
#यदि_अन्तः

#यदि_परिभाषितम् STR91X_IAR
    #अन्तर्भाव "..\..\Source\portable\IAR\STR91x\portmacro.h"
#यदि_अन्तः

#यदि_परिभाषितम् GCC_H8S
    #अन्तर्भाव "../../Source/portable/GCC/H8S2329/portmacro.h"
#यदि_अन्तः

#यदि_परिभाषितम् GCC_AT91FR40008
    #अन्तर्भाव "../../Source/portable/GCC/ARM7_AT91FR40008/portmacro.h"
#यदि_अन्तः

#यदि_परिभाषितम् RVDS_ARMCM3_LM3S102
    #अन्तर्भाव "../../Source/portable/RVDS/ARM_CM3/portmacro.h"
#यदि_अन्तः

#यदि_परिभाषितम् GCC_ARMCM3_LM3S102
    #अन्तर्भाव "../../Source/portable/GCC/ARM_CM3/portmacro.h"
#यदि_अन्तः

#यदि_परिभाषितम् GCC_ARMCM3
    #अन्तर्भाव "../../Source/portable/GCC/ARM_CM3/portmacro.h"
#यदि_अन्तः

#यदि_परिभाषितम् IAR_ARM_CM3
    #अन्तर्भाव "../../Source/portable/IAR/ARM_CM3/portmacro.h"
#यदि_अन्तः

#यदि_परिभाषितम् IAR_ARMCM3_LM
    #अन्तर्भाव "../../Source/portable/IAR/ARM_CM3/portmacro.h"
#यदि_अन्तः

#यदि_परिभाषितम् HCS12_संहिता_WARRIOR
    #अन्तर्भाव "../../Source/portable/CodeWarrior/HCS12/portmacro.h"
#यदि_अन्तः

#यदि_परिभाषितम् MICROBLAZE_GCC
    #अन्तर्भाव "../../Source/portable/GCC/MicroBlaze/portmacro.h"
#यदि_अन्तः

#यदि_परिभाषितम् TERN_EE
    #अन्तर्भाव "..\..\Source\portable\Paradigm\Tern_EE\small\portmacro.h"
#यदि_अन्तः

#यदि_परिभाषितम् GCC_HCS12
    #अन्तर्भाव "../../Source/portable/GCC/HCS12/portmacro.h"
#यदि_अन्तः

#यदि_परिभाषितम् GCC_MCF5235
    #अन्तर्भाव "../../Source/portable/GCC/MCF5235/portmacro.h"
#यदि_अन्तः

#यदि_परिभाषितम् COLDFIRE_V2_GCC
    #अन्तर्भाव "../../../Source/portable/GCC/ColdFire_V2/portmacro.h"
#यदि_अन्तः

#यदि_परिभाषितम् COLDFIRE_V2_CODEWARRIOR
    #अन्तर्भाव "../../Source/portable/CodeWarrior/ColdFire_V2/portmacro.h"
#यदि_अन्तः

#यदि_परिभाषितम् GCC_PPC405
    #अन्तर्भाव "../../Source/portable/GCC/PPC405_Xilinx/portmacro.h"
#यदि_अन्तः

#यदि_परिभाषितम् GCC_PPC440
    #अन्तर्भाव "../../Source/portable/GCC/PPC440_Xilinx/portmacro.h"
#यदि_अन्तः

#यदि_परिभाषितम् _16FX_SOFTUNE
    #अन्तर्भाव "..\..\Source\portable\Softune\MB96340\portmacro.h"
#यदि_अन्तः

#यदि_परिभाषितम् BCC_INDUSTRIAL_PC_पत्तनम्

/* A short file name has to be used in place of the normal
 * FreeRTOSConfig.h when using the Borland compiler. */
    #अन्तर्भाव "frconfig.h"
    #अन्तर्भाव "..\portable\BCC\16BitDOS\PC\prtmacro.h"
    प्रकारनिर्देश शून्य ( __interrupt __far * pxISR )();
#यदि_अन्तः

#यदि_परिभाषितम् BCC_FLASH_LITE_186_पत्तनम्

/* A short file name has to be used in place of the normal
 * FreeRTOSConfig.h when using the Borland compiler. */
    #अन्तर्भाव "frconfig.h"
    #अन्तर्भाव "..\portable\BCC\16BitDOS\flsh186\prtmacro.h"
    प्रकारनिर्देश शून्य ( __interrupt __far * pxISR )();
#यदि_अन्तः

#यदि_परिभाषितम् __GNUC__
    #यदि_परिभाषितम् __AVR32_AVR32A__
        #अन्तर्भाव "portmacro.h"
    #यदि_अन्तः
#यदि_अन्तः

#यदि_परिभाषितम् __ICCAVR32__
    #यदि_परिभाषितम् __CORE__
        #यदि __CORE__ == __AVR32A__
            #अन्तर्भाव "portmacro.h"
        #यदि_अन्तः
    #यदि_अन्तः
#यदि_अन्तः

#यदि_परिभाषितम् __91467D
    #अन्तर्भाव "portmacro.h"
#यदि_अन्तः

#यदि_परिभाषितम् __96340
    #अन्तर्भाव "portmacro.h"
#यदि_अन्तः


#यदि_परिभाषितम् __IAR_V850ES_Fx3__
    #अन्तर्भाव "../../Source/portable/IAR/V850ES/portmacro.h"
#यदि_अन्तः

#यदि_परिभाषितम् __IAR_V850ES_Jx3__
    #अन्तर्भाव "../../Source/portable/IAR/V850ES/portmacro.h"
#यदि_अन्तः

#यदि_परिभाषितम् __IAR_V850ES_Jx3_L__
    #अन्तर्भाव "../../Source/portable/IAR/V850ES/portmacro.h"
#यदि_अन्तः

#यदि_परिभाषितम् __IAR_V850ES_Jx2__
    #अन्तर्भाव "../../Source/portable/IAR/V850ES/portmacro.h"
#यदि_अन्तः

#यदि_परिभाषितम् __IAR_V850ES_Hx2__
    #अन्तर्भाव "../../Source/portable/IAR/V850ES/portmacro.h"
#यदि_अन्तः

#यदि_परिभाषितम् __IAR_78K0R_Kx3__
    #अन्तर्भाव "../../Source/portable/IAR/78K0R/portmacro.h"
#यदि_अन्तः

#यदि_परिभाषितम् __IAR_78K0R_Kx3L__
    #अन्तर्भाव "../../Source/portable/IAR/78K0R/portmacro.h"
#यदि_अन्तः

#यदि_अन्तः /* DEPRECATED_DEFINITIONS_H */
