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

#నిర్వచితం_కాకపోతే PROJDEFS_H
#నిర్వచించు PROJDEFS_H

/*
 * Defines the prototype to which task functions must conform.  Defined in this
 * file to ensure the type is known before portable.h is included.
 */
రకనిర్వచనం శూన్య (* TaskFunction_t)( శూన్య * );

/* Converts a time in milliseconds to a time in ticks.  This macro can be
 * overridden by a macro of the same name defined in FreeRTOSConfig.h in case the
 * definition here is not suitable for your application. */
#నిర్వచితం_కాకపోతే pd_MS_గమ్యం_TICKS
    #నిర్వచించు pd_MS_గమ్యం_TICKS( x_సమయం_లోపల_Ms )    ( ( TickType_t ) ( ( ( uint64_t ) ( x_సమయం_లోపల_Ms ) * ( uint64_t ) config_TICK_రేటు_HZ ) / ( uint64_t ) 1000U ) )
#అయితే_ముగింపు

/* Converts a time in ticks to milliseconds. This macro can be
 * overridden by a macro of the same name defined in FreeRTOSConfig.h in case the
 * definition here is not suitable for your application.
 */
#నిర్వచితం_కాకపోతే pd_TICKS_గమ్యం_MS
    #నిర్వచించు pd_TICKS_గమ్యం_MS( xTicks )    ( ( TickType_t ) ( ( uint64_t ) ( xTicks ) * 1000 / config_TICK_రేటు_HZ ) )
#అయితే_ముగింపు

#నిర్వచించు pdFALSE                                  ( ( పునాది_Type_t ) 0 )
#నిర్వచించు pdTRUE                                   ( ( పునాది_Type_t ) 1 )

#నిర్వచించు pdPASS                                   ( pdTRUE )
#నిర్వచించు pdFAIL                                   ( pdFALSE )
#నిర్వచించు err_వరుస_ఖాళీలు                           ( ( పునాది_Type_t ) 0 )
#నిర్వచించు err_వరుస_నిండిన                            ( ( పునాది_Type_t ) 0 )

/* FreeRTOS error definitions. */
#నిర్వచించు err_COULD_NOT_ALLOCATE_REQUIRED_జ్ఞాపకం    ( -1 )
#నిర్వచించు err_వరుస_BLOCKED                         ( -4 )
#నిర్వచించు err_వరుస_YIELD                           ( -5 )

/* Macros used for basic data corruption checks. */
#నిర్వచితం_కాకపోతే config_USE_LIST_దత్తాంశం_INTEGRITY_CHECK_బైట్లు
    #నిర్వచించు config_USE_LIST_దత్తాంశం_INTEGRITY_CHECK_బైట్లు    0
#అయితే_ముగింపు

#ఒకవేళ ( config_USE_16_బిట్టు_TICKS == 1 )
    #నిర్వచించు pd_INTEGRITY_CHECK_విలువ    0x5a5a
#లేకపోతే
    #నిర్వచించు pd_INTEGRITY_CHECK_విలువ    0x5a5a5a5aUL
#అయితే_ముగింపు

/* The following errno values are used by FreeRTOS+ components, not FreeRTOS
 * itself. */
#నిర్వచించు pdFREERTOS_ERRNO_NONE             0   /* No errors */
#నిర్వచించు pdFREERTOS_ERRNO_ENOENT           2   /* No such file or directory */
#నిర్వచించు pdFREERTOS_ERRNO_EINTR            4   /* Interrupted system call */
#నిర్వచించు pdFREERTOS_ERRNO_EIO              5   /* I/O error */
#నిర్వచించు pdFREERTOS_ERRNO_ENXIO            6   /* No such device or address */
#నిర్వచించు pdFREERTOS_ERRNO_EBADF            9   /* Bad file number */
#నిర్వచించు pdFREERTOS_ERRNO_EAGAIN           11  /* No more processes */
#నిర్వచించు pdFREERTOS_ERRNO_EWOULDBLOCK      11  /* Operation would block */
#నిర్వచించు pdFREERTOS_ERRNO_ENOMEM           12  /* Not enough memory */
#నిర్వచించు pdFREERTOS_ERRNO_EACCES           13  /* Permission denied */
#నిర్వచించు pdFREERTOS_ERRNO_EFAULT           14  /* Bad address */
#నిర్వచించు pdFREERTOS_ERRNO_EBUSY            16  /* Mount device busy */
#నిర్వచించు pdFREERTOS_ERRNO_EEXIST           17  /* File exists */
#నిర్వచించు pdFREERTOS_ERRNO_EXDEV            18  /* Cross-device link */
#నిర్వచించు pdFREERTOS_ERRNO_ENODEV           19  /* No such device */
#నిర్వచించు pdFREERTOS_ERRNO_ENOTDIR          20  /* Not a directory */
#నిర్వచించు pdFREERTOS_ERRNO_EISDIR           21  /* Is a directory */
#నిర్వచించు pdFREERTOS_ERRNO_EINVAL           22  /* Invalid argument */
#నిర్వచించు pdFREERTOS_ERRNO_ENOSPC           28  /* No space left on device */
#నిర్వచించు pdFREERTOS_ERRNO_ESPIPE           29  /* Illegal seek */
#నిర్వచించు pdFREERTOS_ERRNO_EROFS            30  /* Read only file system */
#నిర్వచించు pdFREERTOS_ERRNO_EUNATCH          42  /* Protocol driver not attached */
#నిర్వచించు pdFREERTOS_ERRNO_EBADE            50  /* Invalid exchange */
#నిర్వచించు pdFREERTOS_ERRNO_EFTYPE           79  /* Inappropriate file type or format */
#నిర్వచించు pdFREERTOS_ERRNO_ENMFILE          89  /* No more files */
#నిర్వచించు pdFREERTOS_ERRNO_ENOTEMPTY        90  /* Directory not empty */
#నిర్వచించు pdFREERTOS_ERRNO_ENAMETOOLONG     91  /* File or path name too long */
#నిర్వచించు pdFREERTOS_ERRNO_EOPNOTSUPP       95  /* Operation not supported on transport endpoint */
#నిర్వచించు pdFREERTOS_ERRNO_ENOBUFS          105 /* No buffer space available */
#నిర్వచించు pdFREERTOS_ERRNO_ENOPROTOOPT      109 /* Protocol not available */
#నిర్వచించు pdFREERTOS_ERRNO_EADDRINUSE       112 /* Address already in use */
#నిర్వచించు pdFREERTOS_ERRNO_ETIMEDOUT        116 /* Connection timed out */
#నిర్వచించు pdFREERTOS_ERRNO_EINPROGRESS      119 /* Connection already in progress */
#నిర్వచించు pdFREERTOS_ERRNO_EALREADY         120 /* Socket already connected */
#నిర్వచించు pdFREERTOS_ERRNO_EADDRNOTAVAIL    125 /* Address not available */
#నిర్వచించు pdFREERTOS_ERRNO_EISCONN          127 /* Socket is already connected */
#నిర్వచించు pdFREERTOS_ERRNO_ENOTCONN         128 /* Socket is not connected */
#నిర్వచించు pdFREERTOS_ERRNO_ENOMEDIUM        135 /* No medium inserted */
#నిర్వచించు pdFREERTOS_ERRNO_EILSEQ           138 /* An invalid UTF-16 sequence was encountered. */
#నిర్వచించు pdFREERTOS_ERRNO_ECANCELED        140 /* Operation canceled. */

/* The following endian values are used by FreeRTOS+ components, not FreeRTOS
 * itself. */
#నిర్వచించు pdFREERTOS_LITTLE_ENDIAN          0
#నిర్వచించు pdFREERTOS_BIG_ENDIAN             1

/* Re-defining endian values for generic naming. */
#నిర్వచించు pdLITTLE_ENDIAN                   pdFREERTOS_LITTLE_ENDIAN
#నిర్వచించు pdBIG_ENDIAN                      pdFREERTOS_BIG_ENDIAN


#అయితే_ముగింపు /* PROJDEFS_H */
