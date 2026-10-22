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

#جيڪڏهن_وصف_نه_ٿيل PROJDEFS_H
#وصف_ڪريو PROJDEFS_H

/*
 * Defines the prototype to which task functions must conform.  Defined in this
 * file to ensure the type is known before portable.h is included.
 */
تعريفنوع فراغ (* TaskFunction_t)( فراغ * arg );

/* Converts a time in milliseconds to a time in ticks.  This macro can be
 * overridden by a macro of the same name defined in FreeRTOSConfig.h in case the
 * definition here is not suitable for your application. */
#جيڪڏهن_وصف_نه_ٿيل pd_MS_منزل_TICKS
    #وصف_ڪريو pd_MS_منزل_TICKS( x_وقت_اندريون_Ms )    ( ( TickType_t ) ( ( ( uint64_t ) ( x_وقت_اندريون_Ms ) * ( uint64_t ) config_TICK_شرح_HZ ) / ( uint64_t ) 1000U ) )
#جيڪڏهن_پورو

/* Converts a time in ticks to a time in milliseconds.  This macro can be
 * overridden by a macro of the same name defined in FreeRTOSConfig.h in case the
 * definition here is not suitable for your application. */
#جيڪڏهن_وصف_نه_ٿيل pd_TICKS_منزل_MS
    #وصف_ڪريو pd_TICKS_منزل_MS( x_وقت_اندريون_Ticks )    ( ( TickType_t ) ( ( ( uint64_t ) ( x_وقت_اندريون_Ticks ) * ( uint64_t ) 1000U ) / ( uint64_t ) config_TICK_شرح_HZ ) )
#جيڪڏهن_پورو

#وصف_ڪريو pdFALSE                                  ( ( بنياد_Type_t ) 0 )
#وصف_ڪريو pdTRUE                                   ( ( بنياد_Type_t ) 1 )
#وصف_ڪريو pdFALSE_SIGNED                           ( ( بنياد_Type_t ) 0 )
#وصف_ڪريو pdTRUE_SIGNED                            ( ( بنياد_Type_t ) 1 )
#وصف_ڪريو pdFALSE_UNSIGNED                         ( ( UBaseType_t ) 0 )
#وصف_ڪريو pdTRUE_UNSIGNED                          ( ( UBaseType_t ) 1 )

#وصف_ڪريو pdPASS                                   ( pdTRUE )
#وصف_ڪريو pdFAIL                                   ( pdFALSE )
#وصف_ڪريو err_قطار_خالي_جايون                           ( ( بنياد_Type_t ) 0 )
#وصف_ڪريو err_قطار_ڀريل                            ( ( بنياد_Type_t ) 0 )

/* FreeRTOS error definitions. */
#وصف_ڪريو err_COULD_NOT_ALLOCATE_REQUIRED_يادگيري    ( -1 )
#وصف_ڪريو err_قطار_BLOCKED                         ( -4 )
#وصف_ڪريو err_قطار_YIELD                           ( -5 )

/* Macros used for basic data corruption checks. */
#جيڪڏهن_وصف_نه_ٿيل config_USE_LIST_معلومات_INTEGRITY_CHECK_بائيٽون
    #وصف_ڪريو config_USE_LIST_معلومات_INTEGRITY_CHECK_بائيٽون    0
#جيڪڏهن_پورو

#إذا ( config_TICK_TYPE_ويڪر_اندريون_بٽ == TICK_TYPE_ويڪر_16_بٽ )
    #وصف_ڪريو pd_INTEGRITY_CHECK_قدر    0x5a5a
#وإلاإذا ( config_TICK_TYPE_ويڪر_اندريون_بٽ == TICK_TYPE_ويڪر_32_بٽ )
    #وصف_ڪريو pd_INTEGRITY_CHECK_قدر    0x5a5a5a5aUL
#وإلاإذا ( config_TICK_TYPE_ويڪر_اندريون_بٽ == TICK_TYPE_ويڪر_64_بٽ )
    #وصف_ڪريو pd_INTEGRITY_CHECK_قدر    0x5a5a5a5a5a5a5a5aULL
#وإلا
    #غلطي config_TICK_TYPE_ويڪر_اندريون_بٽ سيٽ منزل unsupported tick type ويڪر.
#جيڪڏهن_پورو

/* The following errno values are used by FreeRTOS+ components, not FreeRTOS
 * itself. */
#وصف_ڪريو pdFREERTOS_ERRNO_NONE             0   /* No errors */
#وصف_ڪريو pdFREERTOS_ERRNO_ENOENT           2   /* No such file or directory */
#وصف_ڪريو pdFREERTOS_ERRNO_EINTR            4   /* Interrupted system call */
#وصف_ڪريو pdFREERTOS_ERRNO_EIO              5   /* I/O error */
#وصف_ڪريو pdFREERTOS_ERRNO_ENXIO            6   /* No such device or address */
#وصف_ڪريو pdFREERTOS_ERRNO_EBADF            9   /* Bad file number */
#وصف_ڪريو pdFREERTOS_ERRNO_EAGAIN           11  /* No more processes */
#وصف_ڪريو pdFREERTOS_ERRNO_EWOULDBLOCK      11  /* Operation would block */
#وصف_ڪريو pdFREERTOS_ERRNO_ENOMEM           12  /* Not enough memory */
#وصف_ڪريو pdFREERTOS_ERRNO_EACCES           13  /* Permission denied */
#وصف_ڪريو pdFREERTOS_ERRNO_EFAULT           14  /* Bad address */
#وصف_ڪريو pdFREERTOS_ERRNO_EBUSY            16  /* Mount device busy */
#وصف_ڪريو pdFREERTOS_ERRNO_EEXIST           17  /* File exists */
#وصف_ڪريو pdFREERTOS_ERRNO_EXDEV            18  /* Cross-device link */
#وصف_ڪريو pdFREERTOS_ERRNO_ENODEV           19  /* No such device */
#وصف_ڪريو pdFREERTOS_ERRNO_ENOTDIR          20  /* Not a directory */
#وصف_ڪريو pdFREERTOS_ERRNO_EISDIR           21  /* Is a directory */
#وصف_ڪريو pdFREERTOS_ERRNO_EINVAL           22  /* Invalid argument */
#وصف_ڪريو pdFREERTOS_ERRNO_ENOSPC           28  /* No space left on device */
#وصف_ڪريو pdFREERTOS_ERRNO_ESPIPE           29  /* Illegal seek */
#وصف_ڪريو pdFREERTOS_ERRNO_EROFS            30  /* Read only file system */
#وصف_ڪريو pdFREERTOS_ERRNO_EUNATCH          42  /* Protocol driver not attached */
#وصف_ڪريو pdFREERTOS_ERRNO_EBADE            50  /* Invalid exchange */
#وصف_ڪريو pdFREERTOS_ERRNO_EFTYPE           79  /* Inappropriate file type or format */
#وصف_ڪريو pdFREERTOS_ERRNO_ENMFILE          89  /* No more files */
#وصف_ڪريو pdFREERTOS_ERRNO_ENOTEMPTY        90  /* Directory not empty */
#وصف_ڪريو pdFREERTOS_ERRNO_ENAMETOOLONG     91  /* File or path name too long */
#وصف_ڪريو pdFREERTOS_ERRNO_EOPNOTSUPP       95  /* Operation not supported on transport endpoint */
#وصف_ڪريو pdFREERTOS_ERRNO_EAFNOSUPPORT     97  /* Address family not supported by protocol */
#وصف_ڪريو pdFREERTOS_ERRNO_ENOBUFS          105 /* No buffer space available */
#وصف_ڪريو pdFREERTOS_ERRNO_ENOPROTOOPT      109 /* Protocol not available */
#وصف_ڪريو pdFREERTOS_ERRNO_EADDRINUSE       112 /* Address already in use */
#وصف_ڪريو pdFREERTOS_ERRNO_ETIMEDOUT        116 /* Connection timed out */
#وصف_ڪريو pdFREERTOS_ERRNO_EINPROGRESS      119 /* Connection already in progress */
#وصف_ڪريو pdFREERTOS_ERRNO_EALREADY         120 /* Socket already connected */
#وصف_ڪريو pdFREERTOS_ERRNO_EADDRNOTAVAIL    125 /* Address not available */
#وصف_ڪريو pdFREERTOS_ERRNO_EISCONN          127 /* Socket is already connected */
#وصف_ڪريو pdFREERTOS_ERRNO_ENOTCONN         128 /* Socket is not connected */
#وصف_ڪريو pdFREERTOS_ERRNO_ENOMEDIUM        135 /* No medium inserted */
#وصف_ڪريو pdFREERTOS_ERRNO_EILSEQ           138 /* An invalid UTF-16 sequence was encountered. */
#وصف_ڪريو pdFREERTOS_ERRNO_ECANCELED        140 /* Operation canceled. */

/* The following endian values are used by FreeRTOS+ components, not FreeRTOS
 * itself. */
#وصف_ڪريو pdFREERTOS_LITTLE_ENDIAN          0
#وصف_ڪريو pdFREERTOS_BIG_ENDIAN             1

/* Re-defining endian values for generic naming. */
#وصف_ڪريو pdLITTLE_ENDIAN                   pdFREERTOS_LITTLE_ENDIAN
#وصف_ڪريو pdBIG_ENDIAN                      pdFREERTOS_BIG_ENDIAN


#جيڪڏهن_پورو /* PROJDEFS_H */
