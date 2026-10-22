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

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ DEPRECATED_DEFINITIONS_H
#ವ್ಯಾಖ್ಯಾನಿಸು DEPRECATED_DEFINITIONS_H


/* Each FreeRTOS port has a unique portmacro.h header file.  Originally a
 * pre-processor definition was used to ensure the pre-processor found the correct
 * portmacro.h file for the port being used.  That scheme was deprecated in favour
 * of setting the compiler's include path such that it found the correct
 * portmacro.h file - removing the need for the constant and allowing the
 * portmacro.h file to be located anywhere in relation to the port being used.  The
 * definitions below remain in the code for backward compatibility only.  New
 * projects should not use them. */

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ OPEN_WATCOM_INDUSTRIAL_PC_ಪೋರ್ಟ್
    #ಅನ್ತರ್ಭಾವ "..\..\Source\portable\owatcom\16bitdos\pc\portmacro.h"
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ ( __interrupt __far * pxISR )();
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ OPEN_WATCOM_FLASH_LITE_186_ಪೋರ್ಟ್
    #ಅನ್ತರ್ಭಾವ "..\..\Source\portable\owatcom\16bitdos\flsh186\portmacro.h"
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ ( __interrupt __far * pxISR )();
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ GCC_MEGA_AVR
    #ಅನ್ತರ್ಭಾವ "../portable/GCC/ATMega323/portmacro.h"
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ IAR_MEGA_AVR
    #ಅನ್ತರ್ಭಾವ "../portable/IAR/ATMega323/portmacro.h"
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ MPLAB_PIC24_ಪೋರ್ಟ್
    #ಅನ್ತರ್ಭಾವ "../../Source/portable/MPLAB/PIC24_dsPIC/portmacro.h"
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ MPLAB_DSPIC_ಪೋರ್ಟ್
    #ಅನ್ತರ್ಭಾವ "../../Source/portable/MPLAB/PIC24_dsPIC/portmacro.h"
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ MPLAB_PIC18_F_ಪೋರ್ಟ್
    #ಅನ್ತರ್ಭಾವ "../../Source/portable/MPLAB/PIC18F/portmacro.h"
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ MPLAB_PIC32_MX_ಪೋರ್ಟ್
    #ಅನ್ತರ್ಭಾವ "../../Source/portable/MPLAB/PIC32MX/portmacro.h"
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ _FEDPICC
    #ಅನ್ತರ್ಭಾವ "libFreeRTOS/Include/portmacro.h"
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ SDCC_CYGNAL
    #ಅನ್ತರ್ಭಾವ "../../Source/portable/SDCC/Cygnal/portmacro.h"
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ GCC_ARM7
    #ಅನ್ತರ್ಭಾವ "../../Source/portable/GCC/ARM7_LPC2000/portmacro.h"
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ GCC_ARM7_ECLIPSE
    #ಅನ್ತರ್ಭಾವ "portmacro.h"
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ROWLEY_LPC23xx
    #ಅನ್ತರ್ಭಾವ "../../Source/portable/GCC/ARM7_LPC23xx/portmacro.h"
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ IAR_MSP430
    #ಅನ್ತರ್ಭಾವ "..\..\Source\portable\IAR\MSP430\portmacro.h"
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ GCC_MSP430
    #ಅನ್ತರ್ಭಾವ "../../Source/portable/GCC/MSP430F449/portmacro.h"
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ROWLEY_MSP430
    #ಅನ್ತರ್ಭಾವ "../../Source/portable/Rowley/MSP430F449/portmacro.h"
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ARM7_LPC21xx_KEIL_RVDS
    #ಅನ್ತರ್ಭಾವ "..\..\Source\portable\RVDS\ARM7_LPC21xx\portmacro.h"
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ SAM7_GCC
    #ಅನ್ತರ್ಭಾವ "../../Source/portable/GCC/ARM7_AT91SAM7S/portmacro.h"
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ SAM7_IAR
    #ಅನ್ತರ್ಭಾವ "..\..\Source\portable\IAR\AtmelSAM7S64\portmacro.h"
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ SAM9XE_IAR
    #ಅನ್ತರ್ಭಾವ "..\..\Source\portable\IAR\AtmelSAM9XE\portmacro.h"
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ LPC2000_IAR
    #ಅನ್ತರ್ಭಾವ "..\..\Source\portable\IAR\LPC2000\portmacro.h"
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ STR71X_IAR
    #ಅನ್ತರ್ಭಾವ "..\..\Source\portable\IAR\STR71x\portmacro.h"
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ STR75X_IAR
    #ಅನ್ತರ್ಭಾವ "..\..\Source\portable\IAR\STR75x\portmacro.h"
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ STR75X_GCC
    #ಅನ್ತರ್ಭಾವ "..\..\Source\portable\GCC\STR75x\portmacro.h"
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ STR91X_IAR
    #ಅನ್ತರ್ಭಾವ "..\..\Source\portable\IAR\STR91x\portmacro.h"
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ GCC_H8S
    #ಅನ್ತರ್ಭಾವ "../../Source/portable/GCC/H8S2329/portmacro.h"
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ GCC_AT91FR40008
    #ಅನ್ತರ್ಭಾವ "../../Source/portable/GCC/ARM7_AT91FR40008/portmacro.h"
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ RVDS_ARMCM3_LM3S102
    #ಅನ್ತರ್ಭಾವ "../../Source/portable/RVDS/ARM_CM3/portmacro.h"
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ GCC_ARMCM3_LM3S102
    #ಅನ್ತರ್ಭಾವ "../../Source/portable/GCC/ARM_CM3/portmacro.h"
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ GCC_ARMCM3
    #ಅನ್ತರ್ಭಾವ "../../Source/portable/GCC/ARM_CM3/portmacro.h"
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ IAR_ARM_CM3
    #ಅನ್ತರ್ಭಾವ "../../Source/portable/IAR/ARM_CM3/portmacro.h"
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ IAR_ARMCM3_LM
    #ಅನ್ತರ್ಭಾವ "../../Source/portable/IAR/ARM_CM3/portmacro.h"
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HCS12_ಸಂಕೇತ_WARRIOR
    #ಅನ್ತರ್ಭಾವ "../../Source/portable/CodeWarrior/HCS12/portmacro.h"
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ MICROBLAZE_GCC
    #ಅನ್ತರ್ಭಾವ "../../Source/portable/GCC/MicroBlaze/portmacro.h"
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ TERN_EE
    #ಅನ್ತರ್ಭಾವ "..\..\Source\portable\Paradigm\Tern_EE\small\portmacro.h"
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ GCC_HCS12
    #ಅನ್ತರ್ಭಾವ "../../Source/portable/GCC/HCS12/portmacro.h"
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ GCC_MCF5235
    #ಅನ್ತರ್ಭಾವ "../../Source/portable/GCC/MCF5235/portmacro.h"
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ COLDFIRE_V2_GCC
    #ಅನ್ತರ್ಭಾವ "../../../Source/portable/GCC/ColdFire_V2/portmacro.h"
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ COLDFIRE_V2_CODEWARRIOR
    #ಅನ್ತರ್ಭಾವ "../../Source/portable/CodeWarrior/ColdFire_V2/portmacro.h"
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ GCC_PPC405
    #ಅನ್ತರ್ಭಾವ "../../Source/portable/GCC/PPC405_Xilinx/portmacro.h"
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ GCC_PPC440
    #ಅನ್ತರ್ಭಾವ "../../Source/portable/GCC/PPC440_Xilinx/portmacro.h"
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ _16FX_SOFTUNE
    #ಅನ್ತರ್ಭಾವ "..\..\Source\portable\Softune\MB96340\portmacro.h"
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ BCC_INDUSTRIAL_PC_ಪೋರ್ಟ್

/* A short file name has to be used in place of the normal
 * FreeRTOSConfig.h when using the Borland compiler. */
    #ಅನ್ತರ್ಭಾವ "frconfig.h"
    #ಅನ್ತರ್ಭಾವ "..\portable\BCC\16BitDOS\PC\prtmacro.h"
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ ( __interrupt __far * pxISR )();
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ BCC_FLASH_LITE_186_ಪೋರ್ಟ್

/* A short file name has to be used in place of the normal
 * FreeRTOSConfig.h when using the Borland compiler. */
    #ಅನ್ತರ್ಭಾವ "frconfig.h"
    #ಅನ್ತರ್ಭಾವ "..\portable\BCC\16BitDOS\flsh186\prtmacro.h"
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ ( __interrupt __far * pxISR )();
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __GNUC__
    #ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __AVR32_AVR32A__
        #ಅನ್ತರ್ಭಾವ "portmacro.h"
    #ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __ICCAVR32__
    #ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __CORE__
        #ಆದರೆ __CORE__ == __AVR32A__
            #ಅನ್ತರ್ಭಾವ "portmacro.h"
        #ಆದರೆ_ಅಂತ್ಯ
    #ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __91467D
    #ಅನ್ತರ್ಭಾವ "portmacro.h"
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __96340
    #ಅನ್ತರ್ಭಾವ "portmacro.h"
#ಆದರೆ_ಅಂತ್ಯ


#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __IAR_V850ES_Fx3__
    #ಅನ್ತರ್ಭಾವ "../../Source/portable/IAR/V850ES/portmacro.h"
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __IAR_V850ES_Jx3__
    #ಅನ್ತರ್ಭಾವ "../../Source/portable/IAR/V850ES/portmacro.h"
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __IAR_V850ES_Jx3_L__
    #ಅನ್ತರ್ಭಾವ "../../Source/portable/IAR/V850ES/portmacro.h"
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __IAR_V850ES_Jx2__
    #ಅನ್ತರ್ಭಾವ "../../Source/portable/IAR/V850ES/portmacro.h"
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __IAR_V850ES_Hx2__
    #ಅನ್ತರ್ಭಾವ "../../Source/portable/IAR/V850ES/portmacro.h"
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __IAR_78K0R_Kx3__
    #ಅನ್ತರ್ಭಾವ "../../Source/portable/IAR/78K0R/portmacro.h"
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __IAR_78K0R_Kx3L__
    #ಅನ್ತರ್ಭಾವ "../../Source/portable/IAR/78K0R/portmacro.h"
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ_ಅಂತ್ಯ /* DEPRECATED_DEFINITIONS_H */
