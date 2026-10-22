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

#اگر_تعریف_نشده PROJDEFS_H
#تعریف_کن PROJDEFS_H

/*
 * Defines the prototype to which task functions must conform.  Defined in this
 * file to ensure the type is known before portable.h is included.
 */
تعريفنوع فراغ (* TaskFunction_t)( فراغ * );

/* Converts a time in milliseconds to a time in ticks.  This macro can be
 * overridden by a macro of the same name defined in FreeRTOSConfig.h in case the
 * definition here is not suitable for your application. */
#اگر_تعریف_نشده pd_MS_مقصد_TICKS
    #تعریف_کن pd_MS_مقصد_TICKS( x_زمان_درون_Ms )    ( ( TickType_t ) ( ( ( uint64_t ) ( x_زمان_درون_Ms ) * ( uint64_t ) config_TICK_نرخ_HZ ) / ( uint64_t ) 1000U ) )
#پایان_اگر

/* Converts a time in ticks to milliseconds. This macro can be
 * overridden by a macro of the same name defined in FreeRTOSConfig.h in case the
 * definition here is not suitable for your application.
 */
#اگر_تعریف_نشده pd_TICKS_مقصد_MS
    #تعریف_کن pd_TICKS_مقصد_MS( xTicks )    ( ( TickType_t ) ( ( uint64_t ) ( xTicks ) * 1000 / config_TICK_نرخ_HZ ) )
#پایان_اگر

#تعریف_کن pdFALSE                                  ( ( پایه_Type_t ) 0 )
#تعریف_کن pdTRUE                                   ( ( پایه_Type_t ) 1 )

#تعریف_کن pdPASS                                   ( pdTRUE )
#تعریف_کن pdFAIL                                   ( pdFALSE )
#تعریف_کن err_صف_خالی                           ( ( پایه_Type_t ) 0 )
#تعریف_کن err_صف_پر                            ( ( پایه_Type_t ) 0 )

/* FreeRTOS error definitions. */
#تعریف_کن err_COULD_NOT_ALLOCATE_REQUIRED_حافظه    ( -1 )
#تعریف_کن err_صف_BLOCKED                         ( -4 )
#تعریف_کن err_صف_YIELD                           ( -5 )

/* Macros used for basic data corruption checks. */
#اگر_تعریف_نشده config_USE_LIST_داده_INTEGRITY_CHECK_بایتها
    #تعریف_کن config_USE_LIST_داده_INTEGRITY_CHECK_بایتها    0
#پایان_اگر

#إذا ( config_USE_16_بیت_TICKS == 1 )
    #تعریف_کن pd_INTEGRITY_CHECK_مقدار    0x5a5a
#وإلا
    #تعریف_کن pd_INTEGRITY_CHECK_مقدار    0x5a5a5a5aUL
#پایان_اگر

/* The following errno values are used by FreeRTOS+ components, not FreeRTOS
 * itself. */
#تعریف_کن pdFREERTOS_ERRNO_NONE             0   /* No errors */
#تعریف_کن pdFREERTOS_ERRNO_ENOENT           2   /* No such file or directory */
#تعریف_کن pdFREERTOS_ERRNO_EINTR            4   /* Interrupted system call */
#تعریف_کن pdFREERTOS_ERRNO_EIO              5   /* I/O error */
#تعریف_کن pdFREERTOS_ERRNO_ENXIO            6   /* No such device or address */
#تعریف_کن pdFREERTOS_ERRNO_EBADF            9   /* Bad file number */
#تعریف_کن pdFREERTOS_ERRNO_EAGAIN           11  /* No more processes */
#تعریف_کن pdFREERTOS_ERRNO_EWOULDBLOCK      11  /* Operation would block */
#تعریف_کن pdFREERTOS_ERRNO_ENOMEM           12  /* Not enough memory */
#تعریف_کن pdFREERTOS_ERRNO_EACCES           13  /* Permission denied */
#تعریف_کن pdFREERTOS_ERRNO_EFAULT           14  /* Bad address */
#تعریف_کن pdFREERTOS_ERRNO_EBUSY            16  /* Mount device busy */
#تعریف_کن pdFREERTOS_ERRNO_EEXIST           17  /* File exists */
#تعریف_کن pdFREERTOS_ERRNO_EXDEV            18  /* Cross-device link */
#تعریف_کن pdFREERTOS_ERRNO_ENODEV           19  /* No such device */
#تعریف_کن pdFREERTOS_ERRNO_ENOTDIR          20  /* Not a directory */
#تعریف_کن pdFREERTOS_ERRNO_EISDIR           21  /* Is a directory */
#تعریف_کن pdFREERTOS_ERRNO_EINVAL           22  /* Invalid argument */
#تعریف_کن pdFREERTOS_ERRNO_ENOSPC           28  /* No space left on device */
#تعریف_کن pdFREERTOS_ERRNO_ESPIPE           29  /* Illegal seek */
#تعریف_کن pdFREERTOS_ERRNO_EROFS            30  /* Read only file system */
#تعریف_کن pdFREERTOS_ERRNO_EUNATCH          42  /* Protocol driver not attached */
#تعریف_کن pdFREERTOS_ERRNO_EBADE            50  /* Invalid exchange */
#تعریف_کن pdFREERTOS_ERRNO_EFTYPE           79  /* Inappropriate file type or format */
#تعریف_کن pdFREERTOS_ERRNO_ENMFILE          89  /* No more files */
#تعریف_کن pdFREERTOS_ERRNO_ENOTEMPTY        90  /* Directory not empty */
#تعریف_کن pdFREERTOS_ERRNO_ENAMETOOLONG     91  /* File or path name too long */
#تعریف_کن pdFREERTOS_ERRNO_EOPNOTSUPP       95  /* Operation not supported on transport endpoint */
#تعریف_کن pdFREERTOS_ERRNO_ENOBUFS          105 /* No buffer space available */
#تعریف_کن pdFREERTOS_ERRNO_ENOPROTOOPT      109 /* Protocol not available */
#تعریف_کن pdFREERTOS_ERRNO_EADDRINUSE       112 /* Address already in use */
#تعریف_کن pdFREERTOS_ERRNO_ETIMEDOUT        116 /* Connection timed out */
#تعریف_کن pdFREERTOS_ERRNO_EINPROGRESS      119 /* Connection already in progress */
#تعریف_کن pdFREERTOS_ERRNO_EALREADY         120 /* Socket already connected */
#تعریف_کن pdFREERTOS_ERRNO_EADDRNOTAVAIL    125 /* Address not available */
#تعریف_کن pdFREERTOS_ERRNO_EISCONN          127 /* Socket is already connected */
#تعریف_کن pdFREERTOS_ERRNO_ENOTCONN         128 /* Socket is not connected */
#تعریف_کن pdFREERTOS_ERRNO_ENOMEDIUM        135 /* No medium inserted */
#تعریف_کن pdFREERTOS_ERRNO_EILSEQ           138 /* An invalid UTF-16 sequence was encountered. */
#تعریف_کن pdFREERTOS_ERRNO_ECANCELED        140 /* Operation canceled. */

/* The following endian values are used by FreeRTOS+ components, not FreeRTOS
 * itself. */
#تعریف_کن pdFREERTOS_LITTLE_ENDIAN          0
#تعریف_کن pdFREERTOS_BIG_ENDIAN             1

/* Re-defining endian values for generic naming. */
#تعریف_کن pdLITTLE_ENDIAN                   pdFREERTOS_LITTLE_ENDIAN
#تعریف_کن pdBIG_ENDIAN                      pdFREERTOS_BIG_ENDIAN


#پایان_اگر /* PROJDEFS_H */
