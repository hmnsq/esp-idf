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

#अगर_अघोषित PROJDEFS_H
#घोषणा PROJDEFS_H

/*
 * Defines the prototype to which task functions must conform.  Defined in this
 * file to ensure the type is known before portable.h is included.
 */
प्रकार व्योम (* TaskFunction_t)( व्योम * );

/* Converts a time in milliseconds to a time in ticks.  This macro can be
 * overridden by a macro of the same name defined in FreeRTOSConfig.h in case the
 * definition here is not suitable for your application. */
#अगर_अघोषित pdMS_TO_TICKS
    #घोषणा pdMS_TO_TICKS( xTimeInMs )    ( ( TickType_t ) ( ( ( uint64_t ) ( xTimeInMs ) * ( uint64_t ) configTICK_RATE_HZ ) / ( uint64_t ) 1000U ) )
#पूर्ण_अगर

/* Converts a time in ticks to milliseconds. This macro can be
 * overridden by a macro of the same name defined in FreeRTOSConfig.h in case the
 * definition here is not suitable for your application.
 */
#अगर_अघोषित pdTICKS_TO_MS
    #घोषणा pdTICKS_TO_MS( xTicks )    ( ( TickType_t ) ( ( uint64_t ) ( xTicks ) * 1000 / configTICK_RATE_HZ ) )
#पूर्ण_अगर

#घोषणा pdFALSE                                  ( ( BaseType_t ) 0 )
#घोषणा pdTRUE                                   ( ( BaseType_t ) 1 )

#घोषणा pdPASS                                   ( pdTRUE )
#घोषणा pdFAIL                                   ( pdFALSE )
#घोषणा errQUEUE_EMPTY                           ( ( BaseType_t ) 0 )
#घोषणा errQUEUE_FULL                            ( ( BaseType_t ) 0 )

/* FreeRTOS error definitions. */
#घोषणा errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY    ( -1 )
#घोषणा errQUEUE_BLOCKED                         ( -4 )
#घोषणा errQUEUE_YIELD                           ( -5 )

/* Macros used for basic data corruption checks. */
#अगर_अघोषित config_USE_LIST_तथ्य_INTEGRITY_CHECK_BYTES
    #घोषणा config_USE_LIST_तथ्य_INTEGRITY_CHECK_BYTES    0
#पूर्ण_अगर

#अगर ( configUSE_16_BIT_TICKS == 1 )
    #घोषणा pd_INTEGRITY_CHECK_मूल्य    0x5a5a
#अन्यथा
    #घोषणा pd_INTEGRITY_CHECK_मूल्य    0x5a5a5a5aUL
#पूर्ण_अगर

/* The following errno values are used by FreeRTOS+ components, not FreeRTOS
 * itself. */
#घोषणा pdFREERTOS_ERRNO_NONE             0   /* No errors */
#घोषणा pdFREERTOS_ERRNO_ENOENT           2   /* No such file or directory */
#घोषणा pdFREERTOS_ERRNO_EINTR            4   /* Interrupted system call */
#घोषणा pdFREERTOS_ERRNO_EIO              5   /* I/O error */
#घोषणा pdFREERTOS_ERRNO_ENXIO            6   /* No such device or address */
#घोषणा pdFREERTOS_ERRNO_EBADF            9   /* Bad file number */
#घोषणा pdFREERTOS_ERRNO_EAGAIN           11  /* No more processes */
#घोषणा pdFREERTOS_ERRNO_EWOULDBLOCK      11  /* Operation would block */
#घोषणा pdFREERTOS_ERRNO_ENOMEM           12  /* Not enough memory */
#घोषणा pdFREERTOS_ERRNO_EACCES           13  /* Permission denied */
#घोषणा pdFREERTOS_ERRNO_EFAULT           14  /* Bad address */
#घोषणा pdFREERTOS_ERRNO_EBUSY            16  /* Mount device busy */
#घोषणा pdFREERTOS_ERRNO_EEXIST           17  /* File exists */
#घोषणा pdFREERTOS_ERRNO_EXDEV            18  /* Cross-device link */
#घोषणा pdFREERTOS_ERRNO_ENODEV           19  /* No such device */
#घोषणा pdFREERTOS_ERRNO_ENOTDIR          20  /* Not a directory */
#घोषणा pdFREERTOS_ERRNO_EISDIR           21  /* Is a directory */
#घोषणा pdFREERTOS_ERRNO_EINVAL           22  /* Invalid argument */
#घोषणा pdFREERTOS_ERRNO_ENOSPC           28  /* No space left on device */
#घोषणा pdFREERTOS_ERRNO_ESPIPE           29  /* Illegal seek */
#घोषणा pdFREERTOS_ERRNO_EROFS            30  /* Read only file system */
#घोषणा pdFREERTOS_ERRNO_EUNATCH          42  /* Protocol driver not attached */
#घोषणा pdFREERTOS_ERRNO_EBADE            50  /* Invalid exchange */
#घोषणा pdFREERTOS_ERRNO_EFTYPE           79  /* Inappropriate file type or format */
#घोषणा pdFREERTOS_ERRNO_ENMFILE          89  /* No more files */
#घोषणा pdFREERTOS_ERRNO_ENOTEMPTY        90  /* Directory not empty */
#घोषणा pdFREERTOS_ERRNO_ENAMETOOLONG     91  /* File or path name too long */
#घोषणा pdFREERTOS_ERRNO_EOPNOTSUPP       95  /* Operation not supported on transport endpoint */
#घोषणा pdFREERTOS_ERRNO_ENOBUFS          105 /* No buffer space available */
#घोषणा pdFREERTOS_ERRNO_ENOPROTOOPT      109 /* Protocol not available */
#घोषणा pdFREERTOS_ERRNO_EADDRINUSE       112 /* Address already in use */
#घोषणा pdFREERTOS_ERRNO_ETIMEDOUT        116 /* Connection timed out */
#घोषणा pdFREERTOS_ERRNO_EINPROGRESS      119 /* Connection already in progress */
#घोषणा pdFREERTOS_ERRNO_EALREADY         120 /* Socket already connected */
#घोषणा pdFREERTOS_ERRNO_EADDRNOTAVAIL    125 /* Address not available */
#घोषणा pdFREERTOS_ERRNO_EISCONN          127 /* Socket is already connected */
#घोषणा pdFREERTOS_ERRNO_ENOTCONN         128 /* Socket is not connected */
#घोषणा pdFREERTOS_ERRNO_ENOMEDIUM        135 /* No medium inserted */
#घोषणा pdFREERTOS_ERRNO_EILSEQ           138 /* An invalid UTF-16 sequence was encountered. */
#घोषणा pdFREERTOS_ERRNO_ECANCELED        140 /* Operation canceled. */

/* The following endian values are used by FreeRTOS+ components, not FreeRTOS
 * itself. */
#घोषणा pdFREERTOS_LITTLE_ENDIAN          0
#घोषणा pdFREERTOS_BIG_ENDIAN             1

/* Re-defining endian values for generic naming. */
#घोषणा pdLITTLE_ENDIAN                   pdFREERTOS_LITTLE_ENDIAN
#घोषणा pdBIG_ENDIAN                      pdFREERTOS_BIG_ENDIAN


#पूर्ण_अगर /* PROJDEFS_H */
