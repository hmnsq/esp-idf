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

#اگر_غیر_تعریف_شدہ PROJDEFS_H
#تعریف_کرو PROJDEFS_H

/*
 * Defines the prototype to which task functions must conform.  Defined in this
 * file to ensure the type is known before portable.h is included.
 */
قسمتعریف خالی (* TaskFunction_t)( خالی * arg );

/* Converts a time in milliseconds to a time in ticks.  This macro can be
 * overridden by a macro of the same name defined in FreeRTOSConfig.h in case the
 * definition here is not suitable for your application. */
#اگر_غیر_تعریف_شدہ pd_MS_مقصد_TICKS
    #تعریف_کرو pd_MS_مقصد_TICKS( x_وقت_اندر_Ms )    ( ( TickType_t ) ( ( ( uint64_t ) ( x_وقت_اندر_Ms ) * ( uint64_t ) config_TICK_شرح_HZ ) / ( uint64_t ) 1000U ) )
#اگر_ختم

/* Converts a time in ticks to a time in milliseconds.  This macro can be
 * overridden by a macro of the same name defined in FreeRTOSConfig.h in case the
 * definition here is not suitable for your application. */
#اگر_غیر_تعریف_شدہ pd_TICKS_مقصد_MS
    #تعریف_کرو pd_TICKS_مقصد_MS( x_وقت_اندر_Ticks )    ( ( TickType_t ) ( ( ( uint64_t ) ( x_وقت_اندر_Ticks ) * ( uint64_t ) 1000U ) / ( uint64_t ) config_TICK_شرح_HZ ) )
#اگر_ختم

#تعریف_کرو pdFALSE                                  ( ( بنیاد_Type_t ) 0 )
#تعریف_کرو pdTRUE                                   ( ( بنیاد_Type_t ) 1 )
#تعریف_کرو pdFALSE_SIGNED                           ( ( بنیاد_Type_t ) 0 )
#تعریف_کرو pdTRUE_SIGNED                            ( ( بنیاد_Type_t ) 1 )
#تعریف_کرو pdFALSE_UNSIGNED                         ( ( UBaseType_t ) 0 )
#تعریف_کرو pdTRUE_UNSIGNED                          ( ( UBaseType_t ) 1 )

#تعریف_کرو pdPASS                                   ( pdTRUE )
#تعریف_کرو pdFAIL                                   ( pdFALSE )
#تعریف_کرو err_قطار_خالی_خانے                           ( ( بنیاد_Type_t ) 0 )
#تعریف_کرو err_قطار_بھرا                            ( ( بنیاد_Type_t ) 0 )

/* FreeRTOS error definitions. */
#تعریف_کرو err_COULD_NOT_ALLOCATE_REQUIRED_حافظہ    ( -1 )
#تعریف_کرو err_قطار_BLOCKED                         ( -4 )
#تعریف_کرو err_قطار_YIELD                           ( -5 )

/* Macros used for basic data corruption checks. */
#اگر_غیر_تعریف_شدہ config_USE_LIST_ڈیٹا_INTEGRITY_CHECK_بائٹیں
    #تعریف_کرو config_USE_LIST_ڈیٹا_INTEGRITY_CHECK_بائٹیں    0
#اگر_ختم

#اگر ( config_TICK_TYPE_چوڑائی_اندر_بٹ == TICK_TYPE_چوڑائی_16_بٹ )
    #تعریف_کرو pd_INTEGRITY_CHECK_قدر    0x5a5a
#وگرنہ_اگر ( config_TICK_TYPE_چوڑائی_اندر_بٹ == TICK_TYPE_چوڑائی_32_بٹ )
    #تعریف_کرو pd_INTEGRITY_CHECK_قدر    0x5a5a5a5aUL
#وگرنہ_اگر ( config_TICK_TYPE_چوڑائی_اندر_بٹ == TICK_TYPE_چوڑائی_64_بٹ )
    #تعریف_کرو pd_INTEGRITY_CHECK_قدر    0x5a5a5a5a5a5a5a5aULL
#ورنہ
    #غلطی config_TICK_TYPE_چوڑائی_اندر_بٹ سیٹ مقصد unsupported tick type چوڑائی.
#اگر_ختم

/* The following errno values are used by FreeRTOS+ components, not FreeRTOS
 * itself. */
#تعریف_کرو pdFREERTOS_ERRNO_NONE             0   /* No errors */
#تعریف_کرو pdFREERTOS_ERRNO_ENOENT           2   /* No such file or directory */
#تعریف_کرو pdFREERTOS_ERRNO_EINTR            4   /* Interrupted system call */
#تعریف_کرو pdFREERTOS_ERRNO_EIO              5   /* I/O error */
#تعریف_کرو pdFREERTOS_ERRNO_ENXIO            6   /* No such device or address */
#تعریف_کرو pdFREERTOS_ERRNO_EBADF            9   /* Bad file number */
#تعریف_کرو pdFREERTOS_ERRNO_EAGAIN           11  /* No more processes */
#تعریف_کرو pdFREERTOS_ERRNO_EWOULDBLOCK      11  /* Operation would block */
#تعریف_کرو pdFREERTOS_ERRNO_ENOMEM           12  /* Not enough memory */
#تعریف_کرو pdFREERTOS_ERRNO_EACCES           13  /* Permission denied */
#تعریف_کرو pdFREERTOS_ERRNO_EFAULT           14  /* Bad address */
#تعریف_کرو pdFREERTOS_ERRNO_EBUSY            16  /* Mount device busy */
#تعریف_کرو pdFREERTOS_ERRNO_EEXIST           17  /* File exists */
#تعریف_کرو pdFREERTOS_ERRNO_EXDEV            18  /* Cross-device link */
#تعریف_کرو pdFREERTOS_ERRNO_ENODEV           19  /* No such device */
#تعریف_کرو pdFREERTOS_ERRNO_ENOTDIR          20  /* Not a directory */
#تعریف_کرو pdFREERTOS_ERRNO_EISDIR           21  /* Is a directory */
#تعریف_کرو pdFREERTOS_ERRNO_EINVAL           22  /* Invalid argument */
#تعریف_کرو pdFREERTOS_ERRNO_ENOSPC           28  /* No space left on device */
#تعریف_کرو pdFREERTOS_ERRNO_ESPIPE           29  /* Illegal seek */
#تعریف_کرو pdFREERTOS_ERRNO_EROFS            30  /* Read only file system */
#تعریف_کرو pdFREERTOS_ERRNO_EUNATCH          42  /* Protocol driver not attached */
#تعریف_کرو pdFREERTOS_ERRNO_EBADE            50  /* Invalid exchange */
#تعریف_کرو pdFREERTOS_ERRNO_EFTYPE           79  /* Inappropriate file type or format */
#تعریف_کرو pdFREERTOS_ERRNO_ENMFILE          89  /* No more files */
#تعریف_کرو pdFREERTOS_ERRNO_ENOTEMPTY        90  /* Directory not empty */
#تعریف_کرو pdFREERTOS_ERRNO_ENAMETOOLONG     91  /* File or path name too long */
#تعریف_کرو pdFREERTOS_ERRNO_EOPNOTSUPP       95  /* Operation not supported on transport endpoint */
#تعریف_کرو pdFREERTOS_ERRNO_EAFNOSUPPORT     97  /* Address family not supported by protocol */
#تعریف_کرو pdFREERTOS_ERRNO_ENOBUFS          105 /* No buffer space available */
#تعریف_کرو pdFREERTOS_ERRNO_ENOPROTOOPT      109 /* Protocol not available */
#تعریف_کرو pdFREERTOS_ERRNO_EADDRINUSE       112 /* Address already in use */
#تعریف_کرو pdFREERTOS_ERRNO_ETIMEDOUT        116 /* Connection timed out */
#تعریف_کرو pdFREERTOS_ERRNO_EINPROGRESS      119 /* Connection already in progress */
#تعریف_کرو pdFREERTOS_ERRNO_EALREADY         120 /* Socket already connected */
#تعریف_کرو pdFREERTOS_ERRNO_EADDRNOTAVAIL    125 /* Address not available */
#تعریف_کرو pdFREERTOS_ERRNO_EISCONN          127 /* Socket is already connected */
#تعریف_کرو pdFREERTOS_ERRNO_ENOTCONN         128 /* Socket is not connected */
#تعریف_کرو pdFREERTOS_ERRNO_ENOMEDIUM        135 /* No medium inserted */
#تعریف_کرو pdFREERTOS_ERRNO_EILSEQ           138 /* An invalid UTF-16 sequence was encountered. */
#تعریف_کرو pdFREERTOS_ERRNO_ECANCELED        140 /* Operation canceled. */

/* The following endian values are used by FreeRTOS+ components, not FreeRTOS
 * itself. */
#تعریف_کرو pdFREERTOS_LITTLE_ENDIAN          0
#تعریف_کرو pdFREERTOS_BIG_ENDIAN             1

/* Re-defining endian values for generic naming. */
#تعریف_کرو pdLITTLE_ENDIAN                   pdFREERTOS_LITTLE_ENDIAN
#تعریف_کرو pdBIG_ENDIAN                      pdFREERTOS_BIG_ENDIAN


#اگر_ختم /* PROJDEFS_H */
