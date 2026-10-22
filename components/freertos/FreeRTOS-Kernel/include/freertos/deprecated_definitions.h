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

#నిర్వచితం_కాకపోతే DEPRECATED_DEFINITIONS_H
#నిర్వచించు DEPRECATED_DEFINITIONS_H


/* Each FreeRTOS port has a unique portmacro.h header file.  Originally a
 * pre-processor definition was used to ensure the pre-processor found the correct
 * portmacro.h file for the port being used.  That scheme was deprecated in favour
 * of setting the compiler's include path such that it found the correct
 * portmacro.h file - removing the need for the constant and allowing the
 * portmacro.h file to be located anywhere in relation to the port being used.  The
 * definitions below remain in the code for backward compatibility only.  New
 * projects should not use them. */

#నిర్వచితమైతే OPEN_WATCOM_INDUSTRIAL_PC_పోర్టు
    #అన్తర్భావ "..\..\Source\portable\owatcom\16bitdos\pc\portmacro.h"
    రకనిర్వచనం శూన్య ( __interrupt __far * pxISR )();
#అయితే_ముగింపు

#నిర్వచితమైతే OPEN_WATCOM_FLASH_LITE_186_పోర్టు
    #అన్తర్భావ "..\..\Source\portable\owatcom\16bitdos\flsh186\portmacro.h"
    రకనిర్వచనం శూన్య ( __interrupt __far * pxISR )();
#అయితే_ముగింపు

#నిర్వచితమైతే GCC_MEGA_AVR
    #అన్తర్భావ "../portable/GCC/ATMega323/portmacro.h"
#అయితే_ముగింపు

#నిర్వచితమైతే IAR_MEGA_AVR
    #అన్తర్భావ "../portable/IAR/ATMega323/portmacro.h"
#అయితే_ముగింపు

#నిర్వచితమైతే MPLAB_PIC24_పోర్టు
    #అన్తర్భావ "../../Source/portable/MPLAB/PIC24_dsPIC/portmacro.h"
#అయితే_ముగింపు

#నిర్వచితమైతే MPLAB_DSPIC_పోర్టు
    #అన్తర్భావ "../../Source/portable/MPLAB/PIC24_dsPIC/portmacro.h"
#అయితే_ముగింపు

#నిర్వచితమైతే MPLAB_PIC18_F_పోర్టు
    #అన్తర్భావ "../../Source/portable/MPLAB/PIC18F/portmacro.h"
#అయితే_ముగింపు

#నిర్వచితమైతే MPLAB_PIC32_MX_పోర్టు
    #అన్తర్భావ "../../Source/portable/MPLAB/PIC32MX/portmacro.h"
#అయితే_ముగింపు

#నిర్వచితమైతే _FEDPICC
    #అన్తర్భావ "libFreeRTOS/Include/portmacro.h"
#అయితే_ముగింపు

#నిర్వచితమైతే SDCC_CYGNAL
    #అన్తర్భావ "../../Source/portable/SDCC/Cygnal/portmacro.h"
#అయితే_ముగింపు

#నిర్వచితమైతే GCC_ARM7
    #అన్తర్భావ "../../Source/portable/GCC/ARM7_LPC2000/portmacro.h"
#అయితే_ముగింపు

#నిర్వచితమైతే GCC_ARM7_ECLIPSE
    #అన్తర్భావ "portmacro.h"
#అయితే_ముగింపు

#నిర్వచితమైతే ROWLEY_LPC23xx
    #అన్తర్భావ "../../Source/portable/GCC/ARM7_LPC23xx/portmacro.h"
#అయితే_ముగింపు

#నిర్వచితమైతే IAR_MSP430
    #అన్తర్భావ "..\..\Source\portable\IAR\MSP430\portmacro.h"
#అయితే_ముగింపు

#నిర్వచితమైతే GCC_MSP430
    #అన్తర్భావ "../../Source/portable/GCC/MSP430F449/portmacro.h"
#అయితే_ముగింపు

#నిర్వచితమైతే ROWLEY_MSP430
    #అన్తర్భావ "../../Source/portable/Rowley/MSP430F449/portmacro.h"
#అయితే_ముగింపు

#నిర్వచితమైతే ARM7_LPC21xx_KEIL_RVDS
    #అన్తర్భావ "..\..\Source\portable\RVDS\ARM7_LPC21xx\portmacro.h"
#అయితే_ముగింపు

#నిర్వచితమైతే SAM7_GCC
    #అన్తర్భావ "../../Source/portable/GCC/ARM7_AT91SAM7S/portmacro.h"
#అయితే_ముగింపు

#నిర్వచితమైతే SAM7_IAR
    #అన్తర్భావ "..\..\Source\portable\IAR\AtmelSAM7S64\portmacro.h"
#అయితే_ముగింపు

#నిర్వచితమైతే SAM9XE_IAR
    #అన్తర్భావ "..\..\Source\portable\IAR\AtmelSAM9XE\portmacro.h"
#అయితే_ముగింపు

#నిర్వచితమైతే LPC2000_IAR
    #అన్తర్భావ "..\..\Source\portable\IAR\LPC2000\portmacro.h"
#అయితే_ముగింపు

#నిర్వచితమైతే STR71X_IAR
    #అన్తర్భావ "..\..\Source\portable\IAR\STR71x\portmacro.h"
#అయితే_ముగింపు

#నిర్వచితమైతే STR75X_IAR
    #అన్తర్భావ "..\..\Source\portable\IAR\STR75x\portmacro.h"
#అయితే_ముగింపు

#నిర్వచితమైతే STR75X_GCC
    #అన్తర్భావ "..\..\Source\portable\GCC\STR75x\portmacro.h"
#అయితే_ముగింపు

#నిర్వచితమైతే STR91X_IAR
    #అన్తర్భావ "..\..\Source\portable\IAR\STR91x\portmacro.h"
#అయితే_ముగింపు

#నిర్వచితమైతే GCC_H8S
    #అన్తర్భావ "../../Source/portable/GCC/H8S2329/portmacro.h"
#అయితే_ముగింపు

#నిర్వచితమైతే GCC_AT91FR40008
    #అన్తర్భావ "../../Source/portable/GCC/ARM7_AT91FR40008/portmacro.h"
#అయితే_ముగింపు

#నిర్వచితమైతే RVDS_ARMCM3_LM3S102
    #అన్తర్భావ "../../Source/portable/RVDS/ARM_CM3/portmacro.h"
#అయితే_ముగింపు

#నిర్వచితమైతే GCC_ARMCM3_LM3S102
    #అన్తర్భావ "../../Source/portable/GCC/ARM_CM3/portmacro.h"
#అయితే_ముగింపు

#నిర్వచితమైతే GCC_ARMCM3
    #అన్తర్భావ "../../Source/portable/GCC/ARM_CM3/portmacro.h"
#అయితే_ముగింపు

#నిర్వచితమైతే IAR_ARM_CM3
    #అన్తర్భావ "../../Source/portable/IAR/ARM_CM3/portmacro.h"
#అయితే_ముగింపు

#నిర్వచితమైతే IAR_ARMCM3_LM
    #అన్తర్భావ "../../Source/portable/IAR/ARM_CM3/portmacro.h"
#అయితే_ముగింపు

#నిర్వచితమైతే HCS12_సంకేతం_WARRIOR
    #అన్తర్భావ "../../Source/portable/CodeWarrior/HCS12/portmacro.h"
#అయితే_ముగింపు

#నిర్వచితమైతే MICROBLAZE_GCC
    #అన్తర్భావ "../../Source/portable/GCC/MicroBlaze/portmacro.h"
#అయితే_ముగింపు

#నిర్వచితమైతే TERN_EE
    #అన్తర్భావ "..\..\Source\portable\Paradigm\Tern_EE\small\portmacro.h"
#అయితే_ముగింపు

#నిర్వచితమైతే GCC_HCS12
    #అన్తర్భావ "../../Source/portable/GCC/HCS12/portmacro.h"
#అయితే_ముగింపు

#నిర్వచితమైతే GCC_MCF5235
    #అన్తర్భావ "../../Source/portable/GCC/MCF5235/portmacro.h"
#అయితే_ముగింపు

#నిర్వచితమైతే COLDFIRE_V2_GCC
    #అన్తర్భావ "../../../Source/portable/GCC/ColdFire_V2/portmacro.h"
#అయితే_ముగింపు

#నిర్వచితమైతే COLDFIRE_V2_CODEWARRIOR
    #అన్తర్భావ "../../Source/portable/CodeWarrior/ColdFire_V2/portmacro.h"
#అయితే_ముగింపు

#నిర్వచితమైతే GCC_PPC405
    #అన్తర్భావ "../../Source/portable/GCC/PPC405_Xilinx/portmacro.h"
#అయితే_ముగింపు

#నిర్వచితమైతే GCC_PPC440
    #అన్తర్భావ "../../Source/portable/GCC/PPC440_Xilinx/portmacro.h"
#అయితే_ముగింపు

#నిర్వచితమైతే _16FX_SOFTUNE
    #అన్తర్భావ "..\..\Source\portable\Softune\MB96340\portmacro.h"
#అయితే_ముగింపు

#నిర్వచితమైతే BCC_INDUSTRIAL_PC_పోర్టు

/* A short file name has to be used in place of the normal
 * FreeRTOSConfig.h when using the Borland compiler. */
    #అన్తర్భావ "frconfig.h"
    #అన్తర్భావ "..\portable\BCC\16BitDOS\PC\prtmacro.h"
    రకనిర్వచనం శూన్య ( __interrupt __far * pxISR )();
#అయితే_ముగింపు

#నిర్వచితమైతే BCC_FLASH_LITE_186_పోర్టు

/* A short file name has to be used in place of the normal
 * FreeRTOSConfig.h when using the Borland compiler. */
    #అన్తర్భావ "frconfig.h"
    #అన్తర్భావ "..\portable\BCC\16BitDOS\flsh186\prtmacro.h"
    రకనిర్వచనం శూన్య ( __interrupt __far * pxISR )();
#అయితే_ముగింపు

#నిర్వచితమైతే __GNUC__
    #నిర్వచితమైతే __AVR32_AVR32A__
        #అన్తర్భావ "portmacro.h"
    #అయితే_ముగింపు
#అయితే_ముగింపు

#నిర్వచితమైతే __ICCAVR32__
    #నిర్వచితమైతే __CORE__
        #ఒకవేళ __CORE__ == __AVR32A__
            #అన్తర్భావ "portmacro.h"
        #అయితే_ముగింపు
    #అయితే_ముగింపు
#అయితే_ముగింపు

#నిర్వచితమైతే __91467D
    #అన్తర్భావ "portmacro.h"
#అయితే_ముగింపు

#నిర్వచితమైతే __96340
    #అన్తర్భావ "portmacro.h"
#అయితే_ముగింపు


#నిర్వచితమైతే __IAR_V850ES_Fx3__
    #అన్తర్భావ "../../Source/portable/IAR/V850ES/portmacro.h"
#అయితే_ముగింపు

#నిర్వచితమైతే __IAR_V850ES_Jx3__
    #అన్తర్భావ "../../Source/portable/IAR/V850ES/portmacro.h"
#అయితే_ముగింపు

#నిర్వచితమైతే __IAR_V850ES_Jx3_L__
    #అన్తర్భావ "../../Source/portable/IAR/V850ES/portmacro.h"
#అయితే_ముగింపు

#నిర్వచితమైతే __IAR_V850ES_Jx2__
    #అన్తర్భావ "../../Source/portable/IAR/V850ES/portmacro.h"
#అయితే_ముగింపు

#నిర్వచితమైతే __IAR_V850ES_Hx2__
    #అన్తర్భావ "../../Source/portable/IAR/V850ES/portmacro.h"
#అయితే_ముగింపు

#నిర్వచితమైతే __IAR_78K0R_Kx3__
    #అన్తర్భావ "../../Source/portable/IAR/78K0R/portmacro.h"
#అయితే_ముగింపు

#నిర్వచితమైతే __IAR_78K0R_Kx3L__
    #అన్తర్భావ "../../Source/portable/IAR/78K0R/portmacro.h"
#అయితే_ముగింపు

#అయితే_ముగింపు /* DEPRECATED_DEFINITIONS_H */
