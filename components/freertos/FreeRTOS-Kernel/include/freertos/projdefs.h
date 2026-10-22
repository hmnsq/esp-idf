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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ PROJDEFS_H
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ PROJDEFS_H

/*
 * Defines the prototype to which task functions must conform.  Defined in this
 * file to ensure the type is known before portable.h is included.
 */
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (* TaskFunction_t)( ਖਾਲੀ * );

/* Converts a time in milliseconds to a time in ticks.  This macro can be
 * overridden by a macro of the same name defined in FreeRTOSConfig.h in case the
 * definition here is not suitable for your application. */
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ pd_MS_ਮੰਜ਼ਿਲ_TICKS
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ pd_MS_ਮੰਜ਼ਿਲ_TICKS( x_ਸਮਾਂ_ਵਿੱਚ_Ms )    ( ( TickType_t ) ( ( ( uint64_t ) ( x_ਸਮਾਂ_ਵਿੱਚ_Ms ) * ( uint64_t ) config_TICK_ਦਰ_HZ ) / ( uint64_t ) 1000U ) )
#ਜੇ_ਅੰਤ

/* Converts a time in ticks to milliseconds. This macro can be
 * overridden by a macro of the same name defined in FreeRTOSConfig.h in case the
 * definition here is not suitable for your application.
 */
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ pd_TICKS_ਮੰਜ਼ਿਲ_MS
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ pd_TICKS_ਮੰਜ਼ਿਲ_MS( xTicks )    ( ( TickType_t ) ( ( uint64_t ) ( xTicks ) * 1000 / config_TICK_ਦਰ_HZ ) )
#ਜੇ_ਅੰਤ

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ pdFALSE                                  ( ( ਅਧਾਰ_Type_t ) 0 )
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ pdTRUE                                   ( ( ਅਧਾਰ_Type_t ) 1 )

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ pdPASS                                   ( pdTRUE )
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ pdFAIL                                   ( pdFALSE )
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ err_ਕਤਾਰ_ਖਾਲੀ_ਥਾਵਾਂ                           ( ( ਅਧਾਰ_Type_t ) 0 )
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ err_ਕਤਾਰ_ਭਰਿਆ                            ( ( ਅਧਾਰ_Type_t ) 0 )

/* FreeRTOS error definitions. */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ err_COULD_NOT_ALLOCATE_REQUIRED_ਸਮ੍ਰਿਤੀ    ( -1 )
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ err_ਕਤਾਰ_BLOCKED                         ( -4 )
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ err_ਕਤਾਰ_YIELD                           ( -5 )

/* Macros used for basic data corruption checks. */
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ config_USE_LIST_ਅੰਕੜੇ_INTEGRITY_CHECK_ਬਾਈਟਾਂ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ config_USE_LIST_ਅੰਕੜੇ_INTEGRITY_CHECK_ਬਾਈਟਾਂ    0
#ਜੇ_ਅੰਤ

#ਜੇ ( config_USE_16_ਦੋਆਧਾਰੀ_ਅੰਕ_TICKS == 1 )
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ pd_INTEGRITY_CHECK_ਮੁੱਲ    0x5a5a
#ਨਹੀਂਤਾਂ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ pd_INTEGRITY_CHECK_ਮੁੱਲ    0x5a5a5a5aUL
#ਜੇ_ਅੰਤ

/* The following errno values are used by FreeRTOS+ components, not FreeRTOS
 * itself. */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ pdFREERTOS_ERRNO_NONE             0   /* No errors */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ pdFREERTOS_ERRNO_ENOENT           2   /* No such file or directory */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ pdFREERTOS_ERRNO_EINTR            4   /* Interrupted system call */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ pdFREERTOS_ERRNO_EIO              5   /* I/O error */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ pdFREERTOS_ERRNO_ENXIO            6   /* No such device or address */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ pdFREERTOS_ERRNO_EBADF            9   /* Bad file number */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ pdFREERTOS_ERRNO_EAGAIN           11  /* No more processes */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ pdFREERTOS_ERRNO_EWOULDBLOCK      11  /* Operation would block */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ pdFREERTOS_ERRNO_ENOMEM           12  /* Not enough memory */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ pdFREERTOS_ERRNO_EACCES           13  /* Permission denied */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ pdFREERTOS_ERRNO_EFAULT           14  /* Bad address */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ pdFREERTOS_ERRNO_EBUSY            16  /* Mount device busy */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ pdFREERTOS_ERRNO_EEXIST           17  /* File exists */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ pdFREERTOS_ERRNO_EXDEV            18  /* Cross-device link */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ pdFREERTOS_ERRNO_ENODEV           19  /* No such device */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ pdFREERTOS_ERRNO_ENOTDIR          20  /* Not a directory */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ pdFREERTOS_ERRNO_EISDIR           21  /* Is a directory */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ pdFREERTOS_ERRNO_EINVAL           22  /* Invalid argument */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ pdFREERTOS_ERRNO_ENOSPC           28  /* No space left on device */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ pdFREERTOS_ERRNO_ESPIPE           29  /* Illegal seek */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ pdFREERTOS_ERRNO_EROFS            30  /* Read only file system */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ pdFREERTOS_ERRNO_EUNATCH          42  /* Protocol driver not attached */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ pdFREERTOS_ERRNO_EBADE            50  /* Invalid exchange */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ pdFREERTOS_ERRNO_EFTYPE           79  /* Inappropriate file type or format */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ pdFREERTOS_ERRNO_ENMFILE          89  /* No more files */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ pdFREERTOS_ERRNO_ENOTEMPTY        90  /* Directory not empty */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ pdFREERTOS_ERRNO_ENAMETOOLONG     91  /* File or path name too long */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ pdFREERTOS_ERRNO_EOPNOTSUPP       95  /* Operation not supported on transport endpoint */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ pdFREERTOS_ERRNO_ENOBUFS          105 /* No buffer space available */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ pdFREERTOS_ERRNO_ENOPROTOOPT      109 /* Protocol not available */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ pdFREERTOS_ERRNO_EADDRINUSE       112 /* Address already in use */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ pdFREERTOS_ERRNO_ETIMEDOUT        116 /* Connection timed out */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ pdFREERTOS_ERRNO_EINPROGRESS      119 /* Connection already in progress */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ pdFREERTOS_ERRNO_EALREADY         120 /* Socket already connected */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ pdFREERTOS_ERRNO_EADDRNOTAVAIL    125 /* Address not available */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ pdFREERTOS_ERRNO_EISCONN          127 /* Socket is already connected */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ pdFREERTOS_ERRNO_ENOTCONN         128 /* Socket is not connected */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ pdFREERTOS_ERRNO_ENOMEDIUM        135 /* No medium inserted */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ pdFREERTOS_ERRNO_EILSEQ           138 /* An invalid UTF-16 sequence was encountered. */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ pdFREERTOS_ERRNO_ECANCELED        140 /* Operation canceled. */

/* The following endian values are used by FreeRTOS+ components, not FreeRTOS
 * itself. */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ pdFREERTOS_LITTLE_ENDIAN          0
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ pdFREERTOS_BIG_ENDIAN             1

/* Re-defining endian values for generic naming. */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ pdLITTLE_ENDIAN                   pdFREERTOS_LITTLE_ENDIAN
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ pdBIG_ENDIAN                      pdFREERTOS_BIG_ENDIAN


#ਜੇ_ਅੰਤ /* PROJDEFS_H */
