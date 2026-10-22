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

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ PROJDEFS_H
#ವ್ಯಾಖ್ಯಾನಿಸು PROJDEFS_H

/*
 * Defines the prototype to which task functions must conform.  Defined in this
 * file to ensure the type is known before portable.h is included.
 */
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (* TaskFunction_t)( ಶೂನ್ಯ * );

/* Converts a time in milliseconds to a time in ticks.  This macro can be
 * overridden by a macro of the same name defined in FreeRTOSConfig.h in case the
 * definition here is not suitable for your application. */
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ pd_MS_ಗಮ್ಯ_TICKS
    #ವ್ಯಾಖ್ಯಾನಿಸು pd_MS_ಗಮ್ಯ_TICKS( x_ಸಮಯ_ಒಳ_Ms )    ( ( TickType_t ) ( ( ( uint64_t ) ( x_ಸಮಯ_ಒಳ_Ms ) * ( uint64_t ) config_TICK_ದರ_HZ ) / ( uint64_t ) 1000U ) )
#ಆದರೆ_ಅಂತ್ಯ

/* Converts a time in ticks to milliseconds. This macro can be
 * overridden by a macro of the same name defined in FreeRTOSConfig.h in case the
 * definition here is not suitable for your application.
 */
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ pd_TICKS_ಗಮ್ಯ_MS
    #ವ್ಯಾಖ್ಯಾನಿಸು pd_TICKS_ಗಮ್ಯ_MS( xTicks )    ( ( TickType_t ) ( ( uint64_t ) ( xTicks ) * 1000 / config_TICK_ದರ_HZ ) )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿಸು pdFALSE                                  ( ( ತಳ_Type_t ) 0 )
#ವ್ಯಾಖ್ಯಾನಿಸು pdTRUE                                   ( ( ತಳ_Type_t ) 1 )

#ವ್ಯಾಖ್ಯಾನಿಸು pdPASS                                   ( pdTRUE )
#ವ್ಯಾಖ್ಯಾನಿಸು pdFAIL                                   ( pdFALSE )
#ವ್ಯಾಖ್ಯಾನಿಸು err_ಸರತಿ_ಖಾಲಿಗಳು                           ( ( ತಳ_Type_t ) 0 )
#ವ್ಯಾಖ್ಯಾನಿಸು err_ಸರತಿ_ತುಂಬಿದ                            ( ( ತಳ_Type_t ) 0 )

/* FreeRTOS error definitions. */
#ವ್ಯಾಖ್ಯಾನಿಸು err_COULD_NOT_ALLOCATE_REQUIRED_ಸ್ಮರಣೆ    ( -1 )
#ವ್ಯಾಖ್ಯಾನಿಸು err_ಸರತಿ_BLOCKED                         ( -4 )
#ವ್ಯಾಖ್ಯಾನಿಸು err_ಸರತಿ_YIELD                           ( -5 )

/* Macros used for basic data corruption checks. */
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ config_USE_LIST_ದತ್ತಾಂಶ_INTEGRITY_CHECK_ಬೈಟುಗಳು
    #ವ್ಯಾಖ್ಯಾನಿಸು config_USE_LIST_ದತ್ತಾಂಶ_INTEGRITY_CHECK_ಬೈಟುಗಳು    0
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ ( config_USE_16_ಬಿಟ್_TICKS == 1 )
    #ವ್ಯಾಖ್ಯಾನಿಸು pd_INTEGRITY_CHECK_ಮೌಲ್ಯ    0x5a5a
#ಇಲ್ಲದಿದ್ದರೆ
    #ವ್ಯಾಖ್ಯಾನಿಸು pd_INTEGRITY_CHECK_ಮೌಲ್ಯ    0x5a5a5a5aUL
#ಆದರೆ_ಅಂತ್ಯ

/* The following errno values are used by FreeRTOS+ components, not FreeRTOS
 * itself. */
#ವ್ಯಾಖ್ಯಾನಿಸು pdFREERTOS_ERRNO_NONE             0   /* No errors */
#ವ್ಯಾಖ್ಯಾನಿಸು pdFREERTOS_ERRNO_ENOENT           2   /* No such file or directory */
#ವ್ಯಾಖ್ಯಾನಿಸು pdFREERTOS_ERRNO_EINTR            4   /* Interrupted system call */
#ವ್ಯಾಖ್ಯಾನಿಸು pdFREERTOS_ERRNO_EIO              5   /* I/O error */
#ವ್ಯಾಖ್ಯಾನಿಸು pdFREERTOS_ERRNO_ENXIO            6   /* No such device or address */
#ವ್ಯಾಖ್ಯಾನಿಸು pdFREERTOS_ERRNO_EBADF            9   /* Bad file number */
#ವ್ಯಾಖ್ಯಾನಿಸು pdFREERTOS_ERRNO_EAGAIN           11  /* No more processes */
#ವ್ಯಾಖ್ಯಾನಿಸು pdFREERTOS_ERRNO_EWOULDBLOCK      11  /* Operation would block */
#ವ್ಯಾಖ್ಯಾನಿಸು pdFREERTOS_ERRNO_ENOMEM           12  /* Not enough memory */
#ವ್ಯಾಖ್ಯಾನಿಸು pdFREERTOS_ERRNO_EACCES           13  /* Permission denied */
#ವ್ಯಾಖ್ಯಾನಿಸು pdFREERTOS_ERRNO_EFAULT           14  /* Bad address */
#ವ್ಯಾಖ್ಯಾನಿಸು pdFREERTOS_ERRNO_EBUSY            16  /* Mount device busy */
#ವ್ಯಾಖ್ಯಾನಿಸು pdFREERTOS_ERRNO_EEXIST           17  /* File exists */
#ವ್ಯಾಖ್ಯಾನಿಸು pdFREERTOS_ERRNO_EXDEV            18  /* Cross-device link */
#ವ್ಯಾಖ್ಯಾನಿಸು pdFREERTOS_ERRNO_ENODEV           19  /* No such device */
#ವ್ಯಾಖ್ಯಾನಿಸು pdFREERTOS_ERRNO_ENOTDIR          20  /* Not a directory */
#ವ್ಯಾಖ್ಯಾನಿಸು pdFREERTOS_ERRNO_EISDIR           21  /* Is a directory */
#ವ್ಯಾಖ್ಯಾನಿಸು pdFREERTOS_ERRNO_EINVAL           22  /* Invalid argument */
#ವ್ಯಾಖ್ಯಾನಿಸು pdFREERTOS_ERRNO_ENOSPC           28  /* No space left on device */
#ವ್ಯಾಖ್ಯಾನಿಸು pdFREERTOS_ERRNO_ESPIPE           29  /* Illegal seek */
#ವ್ಯಾಖ್ಯಾನಿಸು pdFREERTOS_ERRNO_EROFS            30  /* Read only file system */
#ವ್ಯಾಖ್ಯಾನಿಸು pdFREERTOS_ERRNO_EUNATCH          42  /* Protocol driver not attached */
#ವ್ಯಾಖ್ಯಾನಿಸು pdFREERTOS_ERRNO_EBADE            50  /* Invalid exchange */
#ವ್ಯಾಖ್ಯಾನಿಸು pdFREERTOS_ERRNO_EFTYPE           79  /* Inappropriate file type or format */
#ವ್ಯಾಖ್ಯಾನಿಸು pdFREERTOS_ERRNO_ENMFILE          89  /* No more files */
#ವ್ಯಾಖ್ಯಾನಿಸು pdFREERTOS_ERRNO_ENOTEMPTY        90  /* Directory not empty */
#ವ್ಯಾಖ್ಯಾನಿಸು pdFREERTOS_ERRNO_ENAMETOOLONG     91  /* File or path name too long */
#ವ್ಯಾಖ್ಯಾನಿಸು pdFREERTOS_ERRNO_EOPNOTSUPP       95  /* Operation not supported on transport endpoint */
#ವ್ಯಾಖ್ಯಾನಿಸು pdFREERTOS_ERRNO_ENOBUFS          105 /* No buffer space available */
#ವ್ಯಾಖ್ಯಾನಿಸು pdFREERTOS_ERRNO_ENOPROTOOPT      109 /* Protocol not available */
#ವ್ಯಾಖ್ಯಾನಿಸು pdFREERTOS_ERRNO_EADDRINUSE       112 /* Address already in use */
#ವ್ಯಾಖ್ಯಾನಿಸು pdFREERTOS_ERRNO_ETIMEDOUT        116 /* Connection timed out */
#ವ್ಯಾಖ್ಯಾನಿಸು pdFREERTOS_ERRNO_EINPROGRESS      119 /* Connection already in progress */
#ವ್ಯಾಖ್ಯಾನಿಸು pdFREERTOS_ERRNO_EALREADY         120 /* Socket already connected */
#ವ್ಯಾಖ್ಯಾನಿಸು pdFREERTOS_ERRNO_EADDRNOTAVAIL    125 /* Address not available */
#ವ್ಯಾಖ್ಯಾನಿಸು pdFREERTOS_ERRNO_EISCONN          127 /* Socket is already connected */
#ವ್ಯಾಖ್ಯಾನಿಸು pdFREERTOS_ERRNO_ENOTCONN         128 /* Socket is not connected */
#ವ್ಯಾಖ್ಯಾನಿಸು pdFREERTOS_ERRNO_ENOMEDIUM        135 /* No medium inserted */
#ವ್ಯಾಖ್ಯಾನಿಸು pdFREERTOS_ERRNO_EILSEQ           138 /* An invalid UTF-16 sequence was encountered. */
#ವ್ಯಾಖ್ಯಾನಿಸು pdFREERTOS_ERRNO_ECANCELED        140 /* Operation canceled. */

/* The following endian values are used by FreeRTOS+ components, not FreeRTOS
 * itself. */
#ವ್ಯಾಖ್ಯಾನಿಸು pdFREERTOS_LITTLE_ENDIAN          0
#ವ್ಯಾಖ್ಯಾನಿಸು pdFREERTOS_BIG_ENDIAN             1

/* Re-defining endian values for generic naming. */
#ವ್ಯಾಖ್ಯಾನಿಸು pdLITTLE_ENDIAN                   pdFREERTOS_LITTLE_ENDIAN
#ವ್ಯಾಖ್ಯಾನಿಸು pdBIG_ENDIAN                      pdFREERTOS_BIG_ENDIAN


#ಆದರೆ_ಅಂತ್ಯ /* PROJDEFS_H */
