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

#यदि_अपरिभाषितम् PROJDEFS_H
#परिभाषय PROJDEFS_H

/*
 * Defines the prototype to which task functions must conform.  Defined in this
 * file to ensure the type is known before portable.h is included.
 */
प्रकारनिर्देश शून्य (* TaskFunction_t)( शून्य * arg );

/* Converts a time in milliseconds to a time in ticks.  This macro can be
 * overridden by a macro of the same name defined in FreeRTOSConfig.h in case the
 * definition here is not suitable for your application. */
#यदि_अपरिभाषितम् pd_MS_गन्तव्यम्_TICKS
    #परिभाषय pd_MS_गन्तव्यम्_TICKS( x_Time_अन्तर्_Ms )    ( ( TickType_t ) ( ( ( uint64_t ) ( x_Time_अन्तर्_Ms ) * ( uint64_t ) config_TICK_दरः_HZ ) / ( uint64_t ) 1000U ) )
#यदि_अन्तः

/* Converts a time in ticks to a time in milliseconds.  This macro can be
 * overridden by a macro of the same name defined in FreeRTOSConfig.h in case the
 * definition here is not suitable for your application. */
#यदि_अपरिभाषितम् pd_TICKS_गन्तव्यम्_MS
    #परिभाषय pd_TICKS_गन्तव्यम्_MS( x_Time_अन्तर्_Ticks )    ( ( TickType_t ) ( ( ( uint64_t ) ( x_Time_अन्तर्_Ticks ) * ( uint64_t ) 1000U ) / ( uint64_t ) config_TICK_दरः_HZ ) )
#यदि_अन्तः

#परिभाषय pdFALSE                                  ( ( आधारः_Type_t ) 0 )
#परिभाषय pdTRUE                                   ( ( आधारः_Type_t ) 1 )
#परिभाषय pdFALSE_SIGNED                           ( ( आधारः_Type_t ) 0 )
#परिभाषय pdTRUE_SIGNED                            ( ( आधारः_Type_t ) 1 )
#परिभाषय pdFALSE_UNSIGNED                         ( ( UBaseType_t ) 0 )
#परिभाषय pdTRUE_UNSIGNED                          ( ( UBaseType_t ) 1 )

#परिभाषय pdPASS                                   ( pdTRUE )
#परिभाषय pdFAIL                                   ( pdFALSE )
#परिभाषय err_QUEUE_रिक्तानि                           ( ( आधारः_Type_t ) 0 )
#परिभाषय err_QUEUE_पूर्णम्                            ( ( आधारः_Type_t ) 0 )

/* FreeRTOS error definitions. */
#परिभाषय err_COULD_NOT_ALLOCATE_REQUIRED_स्मृतिः    ( -1 )
#परिभाषय errQUEUE_BLOCKED                         ( -4 )
#परिभाषय errQUEUE_YIELD                           ( -5 )

/* Macros used for basic data corruption checks. */
#यदि_अपरिभाषितम् config_USE_LIST_दत्त_INTEGRITY_CHECK_अष्टकानि
    #परिभाषय config_USE_LIST_दत्त_INTEGRITY_CHECK_अष्टकानि    0
#यदि_अन्तः

#यदि ( config_TICK_TYPE_विस्तारः_अन्तर्_द्वयङ्काः == TICK_TYPE_विस्तारः_16_द्वयङ्काः )
    #परिभाषय pd_INTEGRITY_CHECK_मानम्    0x5a5a
#नोचेद्यदि ( config_TICK_TYPE_विस्तारः_अन्तर्_द्वयङ्काः == TICK_TYPE_विस्तारः_32_द्वयङ्काः )
    #परिभाषय pd_INTEGRITY_CHECK_मानम्    0x5a5a5a5aUL
#नोचेद्यदि ( config_TICK_TYPE_विस्तारः_अन्तर्_द्वयङ्काः == TICK_TYPE_विस्तारः_64_द्वयङ्काः )
    #परिभाषय pd_INTEGRITY_CHECK_मानम्    0x5a5a5a5a5a5a5a5aULL
#अन्यथा
    #दोषः config_TICK_TYPE_विस्तारः_अन्तर्_द्वयङ्काः समुच्चयः गन्तव्यम् unsupported tick type विस्तारः.
#यदि_अन्तः

/* The following errno values are used by FreeRTOS+ components, not FreeRTOS
 * itself. */
#परिभाषय pdFREERTOS_ERRNO_NONE             0   /* No errors */
#परिभाषय pdFREERTOS_ERRNO_ENOENT           2   /* No such file or directory */
#परिभाषय pdFREERTOS_ERRNO_EINTR            4   /* Interrupted system call */
#परिभाषय pdFREERTOS_ERRNO_EIO              5   /* I/O error */
#परिभाषय pdFREERTOS_ERRNO_ENXIO            6   /* No such device or address */
#परिभाषय pdFREERTOS_ERRNO_EBADF            9   /* Bad file number */
#परिभाषय pdFREERTOS_ERRNO_EAGAIN           11  /* No more processes */
#परिभाषय pdFREERTOS_ERRNO_EWOULDBLOCK      11  /* Operation would block */
#परिभाषय pdFREERTOS_ERRNO_ENOMEM           12  /* Not enough memory */
#परिभाषय pdFREERTOS_ERRNO_EACCES           13  /* Permission denied */
#परिभाषय pdFREERTOS_ERRNO_EFAULT           14  /* Bad address */
#परिभाषय pdFREERTOS_ERRNO_EBUSY            16  /* Mount device busy */
#परिभाषय pdFREERTOS_ERRNO_EEXIST           17  /* File exists */
#परिभाषय pdFREERTOS_ERRNO_EXDEV            18  /* Cross-device link */
#परिभाषय pdFREERTOS_ERRNO_ENODEV           19  /* No such device */
#परिभाषय pdFREERTOS_ERRNO_ENOTDIR          20  /* Not a directory */
#परिभाषय pdFREERTOS_ERRNO_EISDIR           21  /* Is a directory */
#परिभाषय pdFREERTOS_ERRNO_EINVAL           22  /* Invalid argument */
#परिभाषय pdFREERTOS_ERRNO_ENOSPC           28  /* No space left on device */
#परिभाषय pdFREERTOS_ERRNO_ESPIPE           29  /* Illegal seek */
#परिभाषय pdFREERTOS_ERRNO_EROFS            30  /* Read only file system */
#परिभाषय pdFREERTOS_ERRNO_EUNATCH          42  /* Protocol driver not attached */
#परिभाषय pdFREERTOS_ERRNO_EBADE            50  /* Invalid exchange */
#परिभाषय pdFREERTOS_ERRNO_EFTYPE           79  /* Inappropriate file type or format */
#परिभाषय pdFREERTOS_ERRNO_ENMFILE          89  /* No more files */
#परिभाषय pdFREERTOS_ERRNO_ENOTEMPTY        90  /* Directory not empty */
#परिभाषय pdFREERTOS_ERRNO_ENAMETOOLONG     91  /* File or path name too long */
#परिभाषय pdFREERTOS_ERRNO_EOPNOTSUPP       95  /* Operation not supported on transport endpoint */
#परिभाषय pdFREERTOS_ERRNO_EAFNOSUPPORT     97  /* Address family not supported by protocol */
#परिभाषय pdFREERTOS_ERRNO_ENOBUFS          105 /* No buffer space available */
#परिभाषय pdFREERTOS_ERRNO_ENOPROTOOPT      109 /* Protocol not available */
#परिभाषय pdFREERTOS_ERRNO_EADDRINUSE       112 /* Address already in use */
#परिभाषय pdFREERTOS_ERRNO_ETIMEDOUT        116 /* Connection timed out */
#परिभाषय pdFREERTOS_ERRNO_EINPROGRESS      119 /* Connection already in progress */
#परिभाषय pdFREERTOS_ERRNO_EALREADY         120 /* Socket already connected */
#परिभाषय pdFREERTOS_ERRNO_EADDRNOTAVAIL    125 /* Address not available */
#परिभाषय pdFREERTOS_ERRNO_EISCONN          127 /* Socket is already connected */
#परिभाषय pdFREERTOS_ERRNO_ENOTCONN         128 /* Socket is not connected */
#परिभाषय pdFREERTOS_ERRNO_ENOMEDIUM        135 /* No medium inserted */
#परिभाषय pdFREERTOS_ERRNO_EILSEQ           138 /* An invalid UTF-16 sequence was encountered. */
#परिभाषय pdFREERTOS_ERRNO_ECANCELED        140 /* Operation canceled. */

/* The following endian values are used by FreeRTOS+ components, not FreeRTOS
 * itself. */
#परिभाषय pdFREERTOS_LITTLE_ENDIAN          0
#परिभाषय pdFREERTOS_BIG_ENDIAN             1

/* Re-defining endian values for generic naming. */
#परिभाषय pdLITTLE_ENDIAN                   pdFREERTOS_LITTLE_ENDIAN
#परिभाषय pdBIG_ENDIAN                      pdFREERTOS_BIG_ENDIAN


#यदि_अन्तः /* PROJDEFS_H */
