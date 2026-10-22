/*
 * SPDX-FileCopyrightText: 2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Cooperative syscall interposition for Linux/macOS FreeRTOS simulator.
 *
 * This file provides:
 *  - Shadow FD state table (user-visible blocking/non-blocking tracking)
 *  - Cooperative I/O primitives (freertos_linux_coop_read/write/open/close/…)
 *  - Weak POSIX symbol definitions (read/write/open/close/fcntl/select/…)
 *    that VFS can override with strong definitions
 *  - Socket, scatter/gather, multiplexing, sleep wrappers
 *
 * Real libc entry points are resolved via dlsym(RTLD_NEXT, …).
 */

#అన్తర్భావ <stdio.h>
#అన్తర్భావ <stdlib.h>
#అన్తర్భావ <string.h>
#అన్తర్భావ <errno.h>
#అన్తర్భావ <fcntl.h>
#అన్తర్భావ <stdarg.h>
#అన్తర్భావ <unistd.h>
#అన్తర్భావ <sys/types.h>
#అన్తర్భావ <sys/uio.h>
#అన్తర్భావ <sys/socket.h>
#అన్తర్భావ <sys/select.h>
#అన్తర్భావ <sys/time.h>
#అన్తర్భావ <time.h>
#అన్తర్భావ <poll.h>
#అన్తర్భావ <dlfcn.h>
#అన్తర్భావ <pthread.h>

#అన్తర్భావ "linux_port_coop_internal.h"
#అన్తర్భావ "esp_private/freertos_linux_coop_syscalls.h"

స్థిర ssize_t  (*real_readv)(పూర్ణాంకం, స్థిరరాశి నిర్మాణం iovec *, పూర్ణాంకం);
స్థిర ssize_t  (*real_writev)(పూర్ణాంకం, స్థిరరాశి నిర్మాణం iovec *, పూర్ణాంకం);
స్థిర ssize_t  (*real_recv)(పూర్ణాంకం, శూన్య *, size_t, పూర్ణాంకం);
స్థిర ssize_t  (*real_send)(పూర్ణాంకం, స్థిరరాశి శూన్య *, size_t, పూర్ణాంకం);
స్థిర ssize_t  (*real_recvfrom)(పూర్ణాంకం, శూన్య *, size_t, పూర్ణాంకం,
                                 నిర్మాణం sockaddr *, socklen_t *);
స్థిర ssize_t  (*real_sendto)(పూర్ణాంకం, స్థిరరాశి శూన్య *, size_t, పూర్ణాంకం,
                                స్థిరరాశి నిర్మాణం sockaddr *, socklen_t);
స్థిర ssize_t  (*real_recvmsg)(పూర్ణాంకం, నిర్మాణం msghdr *, పూర్ణాంకం);
స్థిర ssize_t  (*real_sendmsg)(పూర్ణాంకం, స్థిరరాశి నిర్మాణం msghdr *, పూర్ణాంకం);
స్థిర పూర్ణాంకం      (*real_connect)(పూర్ణాంకం, స్థిరరాశి నిర్మాణం sockaddr *, socklen_t);
స్థిర పూర్ణాంకం      (*real_accept)(పూర్ణాంకం, నిర్మాణం sockaddr *, socklen_t *);
స్థిర పూర్ణాంకం      (*real_pselect)(పూర్ణాంకం, fd_set *, fd_set *, fd_set *,
                                స్థిరరాశి నిర్మాణం timespec *, స్థిరరాశి sigset_t *);
స్థిర పూర్ణాంకం      (*real_poll)(నిర్మాణం pollfd *, nfds_t, పూర్ణాంకం);
స్థిర పూర్ణాంకం      (*real_socket)(పూర్ణాంకం, పూర్ణాంకం, పూర్ణాంకం);
స్థిర పూర్ణాంకం      (*real_socketpair)(పూర్ణాంకం, పూర్ణాంకం, పూర్ణాంకం, పూర్ణాంకం[2]);
స్థిర పూర్ణాంకం      (*real_pipe)(పూర్ణాంకం[2]);
స్థిర పూర్ణాంకం      (*real_pipe2)(పూర్ణాంకం[2], పూర్ణాంకం);
స్థిర పూర్ణాంకం      (*real_dup)(పూర్ణాంకం);
స్థిర పూర్ణాంకం      (*real_dup2)(పూర్ణాంకం, పూర్ణాంకం);
స్థిర పూర్ణాంకం      (*real_nanosleep)(స్థిరరాశి నిర్మాణం timespec *, నిర్మాణం timespec *);
స్థిర ssize_t  (*real_read)(పూర్ణాంకం, శూన్య *, size_t);
స్థిర ssize_t  (*real_write)(పూర్ణాంకం, స్థిరరాశి శూన్య *, size_t);
స్థిర ssize_t  (*real_pread)(పూర్ణాంకం, శూన్య *, size_t, off_t);
స్థిర ssize_t  (*real_pwrite)(పూర్ణాంకం, స్థిరరాశి శూన్య *, size_t, off_t);
స్థిర పూర్ణాంకం      (*real_close)(పూర్ణాంకం);
స్థిర పూర్ణాంకం      (*real_select)(పూర్ణాంకం, fd_set *, fd_set *, fd_set *,
                                నిర్మాణం timeval *);

రకనిర్వచనం పూర్ణాంకం (*linux_coop_fcntl_fn_t)(పూర్ణాంకం, పూర్ణాంకం, ...);
స్థిర linux_coop_fcntl_fn_t real_fcntl;

రకనిర్వచనం పూర్ణాంకం (*linux_coop_open_fn_t)(స్థిరరాశి అక్షరం *, పూర్ణాంకం, mode_t);
స్థిర linux_coop_open_fn_t real_open;

రకనిర్వచనం నిర్మాణం linux_coop_fd_state {
    పూర్ణాంకం fd;
    పూర్ణాంకం user_flags;
    నిర్మాణం linux_coop_fd_state *తదుపరి;
} linux_coop_fd_state_t;

స్థిర linux_coop_fd_state_t *s_fd_state_list;
స్థిర pthread_mutex_t s_fd_state_తాళం = PTHREAD_MUTEX_INITIALIZER;

స్థిర linux_coop_fd_state_t *linux_coop_కనుగొను_fd_locked(పూర్ణాంకం fd)
{
    linux_coop_fd_state_t *కణుపు = s_fd_state_list;
    అయితే (కణుపు != ఖాళీ) {
        ఒకవేళ (కణుపు->fd == fd) {
            తిరిగి కణుపు;
        }
        కణుపు = కణుపు->తదుపరి;
    }
    తిరిగి ఖాళీ;
}

స్థిర శూన్య __attribute__((constructor)) linux_coop_resolve_all(శూన్య)
{
    LINUX_COOP_RESOLVE(readv);
    LINUX_COOP_RESOLVE(writev);
    LINUX_COOP_RESOLVE(recv);
    LINUX_COOP_RESOLVE(send);
    LINUX_COOP_RESOLVE(recvfrom);
    LINUX_COOP_RESOLVE(sendto);
    LINUX_COOP_RESOLVE(recvmsg);
    LINUX_COOP_RESOLVE(sendmsg);
    LINUX_COOP_RESOLVE(connect);
    LINUX_COOP_RESOLVE(accept);
    LINUX_COOP_RESOLVE(pselect);
    LINUX_COOP_RESOLVE(poll);
    LINUX_COOP_RESOLVE(socket);
    LINUX_COOP_RESOLVE(socketpair);
    LINUX_COOP_RESOLVE(pipe);
    LINUX_COOP_RESOLVE(pipe2);     /* may be NULL on macOS */
    LINUX_COOP_RESOLVE(dup);
    LINUX_COOP_RESOLVE(dup2);
    LINUX_COOP_RESOLVE(nanosleep);
    LINUX_COOP_RESOLVE(fcntl);
    LINUX_COOP_RESOLVE(పఠ);
    LINUX_COOP_RESOLVE(లిఖ);
    LINUX_COOP_RESOLVE(pread);
    LINUX_COOP_RESOLVE(pwrite);
    LINUX_COOP_RESOLVE(close);
    LINUX_COOP_RESOLVE(select);
    LINUX_COOP_RESOLVE(open);
}

శూన్య linux_coop_సమితి_nonblocking(పూర్ణాంకం fd)
{
    ఒకవేళ (fd >= 0) {
        పూర్ణాంకం flags = real_fcntl(fd, F_GETFL, 0);
        ఒకవేళ (flags >= 0) {
            real_fcntl(fd, F_SETFL, flags | O_NONBLOCK);
        }
    }
}

శూన్య linux_coop_track_fd(పూర్ణాంకం fd, పూర్ణాంకం user_flags)
{
    ఒకవేళ (fd < 0) {
        తిరిగి;
    }

    pthread_mutex_lock(&s_fd_state_తాళం);
    linux_coop_fd_state_t *కణుపు = linux_coop_కనుగొను_fd_locked(fd);
    ఒకవేళ (కణుపు != ఖాళీ) {
        కణుపు->user_flags = user_flags;
        pthread_mutex_unlock(&s_fd_state_తాళం);
        తిరిగి;
    }

    కణుపు = సున్నా_జ్ఞాపకం_ఇవ్వు(1, పరిమాణం(*కణుపు));
    ఒకవేళ (కణుపు != ఖాళీ) {
        కణుపు->fd = fd;
        కణుపు->user_flags = user_flags;
        కణుపు->తదుపరి = s_fd_state_list;
        s_fd_state_list = కణుపు;
    }
    pthread_mutex_unlock(&s_fd_state_తాళం);
}

శూన్య linux_coop_untrack_fd(పూర్ణాంకం fd)
{
    ఒకవేళ (fd < 0) {
        తిరిగి;
    }

    pthread_mutex_lock(&s_fd_state_తాళం);
    linux_coop_fd_state_t *prev = ఖాళీ;
    linux_coop_fd_state_t *కణుపు = s_fd_state_list;
    అయితే (కణుపు != ఖాళీ) {
        ఒకవేళ (కణుపు->fd == fd) {
            ఒకవేళ (prev == ఖాళీ) {
                s_fd_state_list = కణుపు->తదుపరి;
            } లేకపోతే {
                prev->తదుపరి = కణుపు->తదుపరి;
            }
            విడుదల(కణుపు);
            ఆపు;
        }
        prev = కణుపు;
        కణుపు = కణుపు->తదుపరి;
    }
    pthread_mutex_unlock(&s_fd_state_తాళం);
}

శూన్య linux_coop_సమితి_user_flags(పూర్ణాంకం fd, పూర్ణాంకం user_flags)
{
    linux_coop_track_fd(fd, user_flags);
}

bool linux_coop_get_user_flags(పూర్ణాంకం fd, పూర్ణాంకం *flags)
{
    ఒకవేళ (fd < 0 || flags == ఖాళీ) {
        తిరిగి false;
    }

    pthread_mutex_lock(&s_fd_state_తాళం);
    linux_coop_fd_state_t *కణుపు = linux_coop_కనుగొను_fd_locked(fd);
    ఒకవేళ (కణుపు == ఖాళీ) {
        pthread_mutex_unlock(&s_fd_state_తాళం);
        తిరిగి false;
    }
    *flags = కణుపు->user_flags;
    pthread_mutex_unlock(&s_fd_state_తాళం);
    తిరిగి true;
}

bool linux_coop_fd_user_nonblocking(పూర్ణాంకం fd)
{
    పూర్ణాంకం user_flags = 0;
    ఒకవేళ (linux_coop_get_user_flags(fd, &user_flags)) {
        తిరిగి (user_flags & O_NONBLOCK) != 0;
    }
    తిరిగి false;
}


ssize_t freertos_linux_coop_read(పూర్ణాంకం fd, శూన్య *buf, size_t లెక్క)
{
    LINUX_COOP_IO_LOOP(fd, real_read(fd, buf, లెక్క))
}

ssize_t freertos_linux_coop_write(పూర్ణాంకం fd, స్థిరరాశి శూన్య *buf, size_t లెక్క)
{
    LINUX_COOP_IO_LOOP(fd, real_write(fd, buf, లెక్క))
}

ssize_t freertos_linux_coop_pread(పూర్ణాంకం fd, శూన్య *buf, size_t లెక్క, off_t స్థానభ్రంశం)
{
    LINUX_COOP_IO_LOOP(fd, real_pread(fd, buf, లెక్క, స్థానభ్రంశం))
}

ssize_t freertos_linux_coop_pwrite(పూర్ణాంకం fd, స్థిరరాశి శూన్య *buf, size_t లెక్క, off_t స్థానభ్రంశం)
{
    LINUX_COOP_IO_LOOP(fd, real_pwrite(fd, buf, లెక్క, స్థానభ్రంశం))
}

పూర్ణాంకం freertos_linux_coop_open(స్థిరరాశి అక్షరం *మార్గం, పూర్ణాంకం flags, పూర్ణాంకం బహుళకం)
{
    పూర్ణాంకం fd = real_open(మార్గం, flags, (mode_t)బహుళకం);
    linux_coop_సమితి_nonblocking(fd);
    linux_coop_track_fd(fd, flags & O_NONBLOCK);
    తిరిగి fd;
}

పూర్ణాంకం freertos_linux_coop_close(పూర్ణాంకం fd)
{
    పూర్ణాంకం ret = real_close(fd);
    ఒకవేళ (ret == 0 || errno == EINTR) {
        /* On Linux, close() always releases the fd even when returning
         * EINTR.  Retrying would risk closing a fd reused by another
         * thread.  Treat EINTR as success. */
        linux_coop_untrack_fd(fd);
        తిరిగి 0;
    }
    తిరిగి -1;
}

పూర్ణాంకం freertos_linux_coop_fcntl(పూర్ణాంకం fd, పూర్ణాంకం cmd, పూర్ణాంకం arg)
{
    ఒకవేళ (cmd == F_GETFL) {
        పూర్ణాంకం real_flags = real_fcntl(fd, F_GETFL, 0);
        ఒకవేళ (real_flags < 0) {
            తిరిగి -1;
        }
        పూర్ణాంకం user_flags = 0;
        ఒకవేళ (linux_coop_get_user_flags(fd, &user_flags)) {
            real_flags &= ~O_NONBLOCK;
            real_flags |= (user_flags & O_NONBLOCK);
        }
        తిరిగి real_flags;
    }
    ఒకవేళ (cmd == F_SETFL) {
        linux_coop_సమితి_user_flags(fd, arg);
        పూర్ణాంకం అంతర్గత_flags = arg | O_NONBLOCK;
        తిరిగి real_fcntl(fd, F_SETFL, అంతర్గత_flags);
    }
    తిరిగి real_fcntl(fd, cmd, arg);
}

పూర్ణాంకం freertos_linux_coop_select(పూర్ణాంకం nfds, fd_set *readfds, fd_set *writefds,
                      fd_set *exceptfds, నిర్మాణం timeval *timeout)
{
    దీర్ఘ timeout_ms = -1;
    ఒకవేళ (timeout != ఖాళీ) {
        timeout_ms = (దీర్ఘ)timeout->tv_sec * 1000 +
                     (timeout->tv_usec + 999) / 1000;
        ఒకవేళ (timeout_ms == 0) {
            తిరిగి real_select(nfds, readfds, writefds, exceptfds, timeout);
        }
    }

    /* real_select() mutates the fd_sets in place (clears them, then sets bits
     * for ready descriptors). To keep polling the same descriptors across
     * iterations we must snapshot the caller's sets and restore them on every
     * loop. */
    fd_set rfds_లోపల, wfds_లోపల, efds_లోపల;
    ఒకవేళ (readfds != ఖాళీ) {
        rfds_లోపల = *readfds;
    }
    ఒకవేళ (writefds != ఖాళీ) {
        wfds_లోపల = *writefds;
    }
    ఒకవేళ (exceptfds != ఖాళీ) {
        efds_లోపల = *exceptfds;
    }

    దీర్ఘ waited_ms = 0;
    అయితే (1) {
        ఒకవేళ (readfds != ఖాళీ) {
            *readfds = rfds_లోపల;
        }
        ఒకవేళ (writefds != ఖాళీ) {
            *writefds = wfds_లోపల;
        }
        ఒకవేళ (exceptfds != ఖాళీ) {
            *exceptfds = efds_లోపల;
        }

        నిర్మాణం timeval zero_tv = {0, 0};
        పూర్ణాంకం ret = real_select(nfds, readfds, writefds, exceptfds, &zero_tv);
        ఒకవేళ (ret != 0) {
            తిరిగి ret;
        }
        ఒకవేళ (timeout_ms == 0) {
            తిరిగి 0;
        }
        ఒకవేళ (timeout_ms > 0 && waited_ms >= timeout_ms) {
            తిరిగి 0;
        }
        linux_coop_yield(LINUX_COOP_TICK_MS);
        waited_ms += LINUX_COOP_TICK_MS;
    }
}

శూన్య freertos_linux_coop_syscalls_init(శూన్య)
{
    linux_coop_సమితి_nonblocking(STDIN_FILENO);
    linux_coop_సమితి_nonblocking(STDOUT_FILENO);
    linux_coop_సమితి_nonblocking(STDERR_FILENO);
    linux_coop_track_fd(STDIN_FILENO, 0);
    linux_coop_track_fd(STDOUT_FILENO, 0);
    linux_coop_track_fd(STDERR_FILENO, 0);
}

/* These are overridden by strong definitions in VFS when that component
 * is linked.  When VFS is absent the weak versions provide cooperative
 * behaviour directly via real libc. */

__attribute__((weak)) ssize_t పఠ(పూర్ణాంకం fd, శూన్య *buf, size_t లెక్క)
{
    తిరిగి freertos_linux_coop_read(fd, buf, లెక్క);
}

__attribute__((weak)) ssize_t లిఖ(పూర్ణాంకం fd, స్థిరరాశి శూన్య *buf, size_t లెక్క)
{
    తిరిగి freertos_linux_coop_write(fd, buf, లెక్క);
}

__attribute__((weak)) ssize_t pread(పూర్ణాంకం fd, శూన్య *buf, size_t లెక్క, off_t స్థానభ్రంశం)
{
    తిరిగి freertos_linux_coop_pread(fd, buf, లెక్క, స్థానభ్రంశం);
}

__attribute__((weak)) ssize_t pwrite(పూర్ణాంకం fd, స్థిరరాశి శూన్య *buf, size_t లెక్క, off_t స్థానభ్రంశం)
{
    తిరిగి freertos_linux_coop_pwrite(fd, buf, లెక్క, స్థానభ్రంశం);
}

__attribute__((weak)) పూర్ణాంకం open(స్థిరరాశి అక్షరం *మార్గం, పూర్ణాంకం flags, ...)
{
    పూర్ణాంకం బహుళకం = 0;
    ఒకవేళ (flags & O_CREAT) {
        va_list ap;
        va_start(ap, flags);
        బహుళకం = va_arg(ap, పూర్ణాంకం);
        va_end(ap);
    }
    తిరిగి freertos_linux_coop_open(మార్గం, flags, బహుళకం);
}

__attribute__((weak)) పూర్ణాంకం close(పూర్ణాంకం fd)
{
    తిరిగి freertos_linux_coop_close(fd);
}

__attribute__((weak)) పూర్ణాంకం fcntl(పూర్ణాంకం fd, పూర్ణాంకం cmd, ...)
{
    పూర్ణాంకం arg = 0;
    ఒకవేళ (cmd != F_GETFL) {
        va_list list;
        va_start(list, cmd);
        arg = va_arg(list, పూర్ణాంకం);
        va_end(list);
    }
    తిరిగి freertos_linux_coop_fcntl(fd, cmd, arg);
}

__attribute__((weak)) పూర్ణాంకం select(పూర్ణాంకం nfds, fd_set *readfds, fd_set *writefds,
                                 fd_set *exceptfds, నిర్మాణం timeval *timeout)
{
    తిరిగి freertos_linux_coop_select(nfds, readfds, writefds, exceptfds, timeout);
}

ssize_t freertos_linux_coop_readv(పూర్ణాంకం fd, స్థిరరాశి నిర్మాణం iovec *iov, పూర్ణాంకం iovcnt)
{
    LINUX_COOP_IO_LOOP(fd, real_readv(fd, iov, iovcnt))
}

ssize_t freertos_linux_coop_writev(పూర్ణాంకం fd, స్థిరరాశి నిర్మాణం iovec *iov, పూర్ణాంకం iovcnt)
{
    LINUX_COOP_IO_LOOP(fd, real_writev(fd, iov, iovcnt))
}

ssize_t freertos_linux_coop_recv(పూర్ణాంకం sockfd, శూన్య *buf, size_t len, పూర్ణాంకం flags)
{
    LINUX_COOP_IO_LOOP(sockfd, real_recv(sockfd, buf, len, flags | MSG_DONTWAIT))
}

ssize_t freertos_linux_coop_send(పూర్ణాంకం sockfd, స్థిరరాశి శూన్య *buf, size_t len, పూర్ణాంకం flags)
{
    LINUX_COOP_IO_LOOP(sockfd, real_send(sockfd, buf, len, flags | MSG_DONTWAIT))
}

ssize_t freertos_linux_coop_recvfrom(పూర్ణాంకం sockfd, శూన్య *buf, size_t len, పూర్ణాంకం flags,
                 నిర్మాణం sockaddr *src_addr, socklen_t *addrlen)
{
    LINUX_COOP_IO_LOOP(sockfd, real_recvfrom(sockfd, buf, len, flags | MSG_DONTWAIT,
                            src_addr, addrlen))
}

ssize_t freertos_linux_coop_sendto(పూర్ణాంకం sockfd, స్థిరరాశి శూన్య *buf, size_t len, పూర్ణాంకం flags,
               స్థిరరాశి నిర్మాణం sockaddr *dest_addr, socklen_t addrlen)
{
    LINUX_COOP_IO_LOOP(sockfd, real_sendto(sockfd, buf, len, flags | MSG_DONTWAIT,
                          dest_addr, addrlen))
}

ssize_t freertos_linux_coop_recvmsg(పూర్ణాంకం sockfd, నిర్మాణం msghdr *సందేశం, పూర్ణాంకం flags)
{
    LINUX_COOP_IO_LOOP(sockfd, real_recvmsg(sockfd, సందేశం, flags | MSG_DONTWAIT))
}

ssize_t freertos_linux_coop_sendmsg(పూర్ణాంకం sockfd, స్థిరరాశి నిర్మాణం msghdr *సందేశం, పూర్ణాంకం flags)
{
    LINUX_COOP_IO_LOOP(sockfd, real_sendmsg(sockfd, సందేశం, flags | MSG_DONTWAIT))
}

__attribute__((weak)) ssize_t readv(పూర్ణాంకం fd, స్థిరరాశి నిర్మాణం iovec *iov, పూర్ణాంకం iovcnt)
{
    తిరిగి freertos_linux_coop_readv(fd, iov, iovcnt);
}

__attribute__((weak)) ssize_t writev(పూర్ణాంకం fd, స్థిరరాశి నిర్మాణం iovec *iov, పూర్ణాంకం iovcnt)
{
    తిరిగి freertos_linux_coop_writev(fd, iov, iovcnt);
}

__attribute__((weak)) ssize_t recv(పూర్ణాంకం sockfd, శూన్య *buf, size_t len, పూర్ణాంకం flags)
{
    తిరిగి freertos_linux_coop_recv(sockfd, buf, len, flags);
}

__attribute__((weak)) ssize_t send(పూర్ణాంకం sockfd, స్థిరరాశి శూన్య *buf, size_t len, పూర్ణాంకం flags)
{
    తిరిగి freertos_linux_coop_send(sockfd, buf, len, flags);
}

__attribute__((weak)) ssize_t recvfrom(పూర్ణాంకం sockfd, శూన్య *buf, size_t len, పూర్ణాంకం flags,
                 నిర్మాణం sockaddr *src_addr, socklen_t *addrlen)
{
    తిరిగి freertos_linux_coop_recvfrom(sockfd, buf, len, flags, src_addr, addrlen);
}

__attribute__((weak)) ssize_t sendto(పూర్ణాంకం sockfd, స్థిరరాశి శూన్య *buf, size_t len, పూర్ణాంకం flags,
               స్థిరరాశి నిర్మాణం sockaddr *dest_addr, socklen_t addrlen)
{
    తిరిగి freertos_linux_coop_sendto(sockfd, buf, len, flags, dest_addr, addrlen);
}

__attribute__((weak)) ssize_t recvmsg(పూర్ణాంకం sockfd, నిర్మాణం msghdr *సందేశం, పూర్ణాంకం flags)
{
    తిరిగి freertos_linux_coop_recvmsg(sockfd, సందేశం, flags);
}

__attribute__((weak)) ssize_t sendmsg(పూర్ణాంకం sockfd, స్థిరరాశి నిర్మాణం msghdr *సందేశం, పూర్ణాంకం flags)
{
    తిరిగి freertos_linux_coop_sendmsg(sockfd, సందేశం, flags);
}

పూర్ణాంకం freertos_linux_coop_connect(పూర్ణాంకం sockfd, స్థిరరాశి నిర్మాణం sockaddr *addr, socklen_t addrlen)
{
    అయితే (1)
    {
        పూర్ణాంకం ret = real_connect(sockfd, addr, addrlen);
        ఒకవేళ (ret == 0) {
            తిరిగి ret;
        }

        ఒకవేళ (errno == EINPROGRESS || errno == EALREADY) {
            నిర్మాణం pollfd pfd;
            pfd.fd = sockfd;
            pfd.events = POLLOUT;
            pfd.revents = 0;

            పూర్ణాంకం poll_retries = 0;
            అయితే (1)
            {
                పూర్ణాంకం press = real_poll(&pfd, 1, 0);
                ఒకవేళ (press > 0) {
                    పూర్ణాంకం so_err = 0;
                    socklen_t len = పరిమాణం(so_err);
                    ఒకవేళ (getsockopt(sockfd, SOL_SOCKET, SO_ERROR,
                                   &so_err, &len) < 0) {
                        తిరిగి -1;
                    }
                    ఒకవేళ (so_err == 0) {
                        తిరిగి 0;
                    } లేకపోతే {
                        errno = so_err;
                        తిరిగి -1;
                    }
                } లేకపోతే ఒకవేళ (press == 0) {
                    linux_coop_yield(LINUX_COOP_TICK_MS);
                    కొనసాగు;
                } లేకపోతే {
                    ఒకవేళ (errno == EINTR) {
                        కొనసాగు;
                    }
                    /* Non-EINTR poll error; bail out after a few retries */
                    ఒకవేళ (++poll_retries > 3) {
                        తిరిగి -1;
                    }
                    linux_coop_yield(LINUX_COOP_TICK_MS);
                    కొనసాగు;
                }
            }
        }

        ఒకవేళ (errno == EINTR) {
            తిరిగి -1;
        }

        తిరిగి -1;
    }
}

పూర్ణాంకం freertos_linux_coop_accept(పూర్ణాంకం sockfd, నిర్మాణం sockaddr *addr, socklen_t *addrlen)
{
    అయితే (1)
    {
        పూర్ణాంకం fd = real_accept(sockfd, addr, addrlen);
        ఒకవేళ (fd >= 0) {
            linux_coop_సమితి_nonblocking(fd);
            linux_coop_track_fd(fd, linux_coop_fd_user_nonblocking(sockfd) ? O_NONBLOCK : 0);
            తిరిగి fd;
        }
        ఒకవేళ (errno == EAGAIN || errno == EWOULDBLOCK) {
            ఒకవేళ (linux_coop_fd_user_nonblocking(sockfd)) {
                తిరిగి -1;
            }
            linux_coop_yield(LINUX_COOP_TICK_MS);
            కొనసాగు;
        }
        తిరిగి -1;
    }
}

పూర్ణాంకం freertos_linux_coop_pselect(పూర్ణాంకం nfds, fd_set *readfds, fd_set *writefds,
            fd_set *exceptfds, స్థిరరాశి నిర్మాణం timespec *timeout,
            స్థిరరాశి sigset_t *sigmask)
{
    దీర్ఘ timeout_ms = -1;
    ఒకవేళ (timeout != ఖాళీ) {
        timeout_ms = (దీర్ఘ)timeout->tv_sec * 1000 +
                     (timeout->tv_nsec + 999999) / 1000000;
        ఒకవేళ (timeout_ms == 0) {
            తిరిగి real_pselect(nfds, readfds, writefds, exceptfds,
                                timeout, sigmask);
        }
    }

    దీర్ఘ waited_ms = 0;
    అయితే (1)
    {
        నిర్మాణం timespec zero_ts = {0, 0};
        పూర్ణాంకం ret = real_pselect(nfds, readfds, writefds, exceptfds,
                               &zero_ts, sigmask);
        ఒకవేళ (ret != 0) {
            తిరిగి ret;
        }

        ఒకవేళ (timeout_ms == 0 ||
            (timeout_ms > 0 && waited_ms >= timeout_ms)) {
            తిరిగి 0;
        }

        linux_coop_yield(LINUX_COOP_TICK_MS);
        waited_ms += LINUX_COOP_TICK_MS;
    }
}

పూర్ణాంకం freertos_linux_coop_poll(నిర్మాణం pollfd *fds, nfds_t nfds, పూర్ణాంకం timeout)
{
    ఒకవేళ (timeout == 0) {
        తిరిగి real_poll(fds, nfds, 0);
    }

    దీర్ఘ timeout_ms = -1;
    ఒకవేళ (timeout > 0) {
        timeout_ms = timeout;
    }

    దీర్ఘ waited_ms = 0;

    అయితే (1)
    {
        పూర్ణాంకం ret = real_poll(fds, nfds, 0);
        ఒకవేళ (ret > 0) {
            తిరిగి ret;
        } లేకపోతే ఒకవేళ (ret == 0) {
            ఒకవేళ (timeout_ms == 0 ||
                (timeout_ms > 0 && waited_ms >= timeout_ms)) {
                తిరిగి 0;
            }
            linux_coop_yield(LINUX_COOP_TICK_MS);
            waited_ms += LINUX_COOP_TICK_MS;
            కొనసాగు;
        } లేకపోతే {
            ఒకవేళ (errno == EINTR) {
                కొనసాగు;
            }
            linux_coop_yield(LINUX_COOP_TICK_MS);
            ఒకవేళ (timeout_ms > 0 && waited_ms >= timeout_ms) {
                తిరిగి -1;
            }
            waited_ms += LINUX_COOP_TICK_MS;
        }
    }
}

పూర్ణాంకం nanosleep(స్థిరరాశి నిర్మాణం timespec *req, నిర్మాణం timespec *rem)
{
    ఒకవేళ (req == ఖాళీ) {
        errno = EFAULT;
        తిరిగి -1;
    }
    ఒకవేళ (!linux_పోర్టు_లోపల_freertos_task()) {
        తిరిగి real_nanosleep(req, rem);
    }
    దీర్ఘ ms = req->tv_sec * 1000 + (req->tv_nsec + 999999) / 1000000;
    ఒకవేళ (ms <= 0) {
        ms = 1;
    }
    vTaskDelay(ms);
    ఒకవేళ (rem) {
        rem->tv_sec = 0;
        rem->tv_nsec = 0;
    }
    తిరిగి 0;
}

గుర్తులేని పూర్ణాంకం sleep(గుర్తులేని పూర్ణాంకం seconds)
{
    linux_coop_yield(seconds * 1000);
    తిరిగి 0;
}

పూర్ణాంకం usleep(useconds_t usec)
{
    linux_coop_yield(usec / 1000);
    తిరిగి 0;
}

పూర్ణాంకం freertos_linux_coop_socket(పూర్ణాంకం domain, పూర్ణాంకం type, పూర్ణాంకం protocol)
{
    పూర్ణాంకం fd = real_socket(domain, type, protocol);
    linux_coop_సమితి_nonblocking(fd);
#నిర్వచితమైతే SOCK_NONBLOCK
    స్థిరరాశి పూర్ణాంకం user_flags = (type & SOCK_NONBLOCK) ? O_NONBLOCK : 0;
#లేకపోతే
    స్థిరరాశి పూర్ణాంకం user_flags = 0;
#అయితే_ముగింపు
    linux_coop_track_fd(fd, user_flags);
    తిరిగి fd;
}

పూర్ణాంకం freertos_linux_coop_socketpair(పూర్ణాంకం domain, పూర్ణాంకం type, పూర్ణాంకం protocol, పూర్ణాంకం *sv)
{
    పూర్ణాంకం ret = real_socketpair(domain, type, protocol, sv);
    ఒకవేళ (ret == 0) {
        linux_coop_సమితి_nonblocking(sv[0]);
        linux_coop_సమితి_nonblocking(sv[1]);
#నిర్వచితమైతే SOCK_NONBLOCK
        స్థిరరాశి పూర్ణాంకం user_flags = (type & SOCK_NONBLOCK) ? O_NONBLOCK : 0;
#లేకపోతే
        స్థిరరాశి పూర్ణాంకం user_flags = 0;
#అయితే_ముగింపు
        linux_coop_track_fd(sv[0], user_flags);
        linux_coop_track_fd(sv[1], user_flags);
    }
    తిరిగి ret;
}

పూర్ణాంకం freertos_linux_coop_pipe(పూర్ణాంకం *fds)
{
    పూర్ణాంకం ret = real_pipe(fds);
    ఒకవేళ (ret == 0) {
        linux_coop_సమితి_nonblocking(fds[0]);
        linux_coop_సమితి_nonblocking(fds[1]);
        linux_coop_track_fd(fds[0], 0);
        linux_coop_track_fd(fds[1], 0);
    }
    తిరిగి ret;
}

పూర్ణాంకం freertos_linux_coop_pipe2(పూర్ణాంకం *fds, పూర్ణాంకం flags)
{
    పూర్ణాంకం ret = real_pipe2(fds, flags);
    ఒకవేళ (ret == 0) {
        linux_coop_సమితి_nonblocking(fds[0]);
        linux_coop_సమితి_nonblocking(fds[1]);
        స్థిరరాశి పూర్ణాంకం user_flags = (flags & O_NONBLOCK) ? O_NONBLOCK : 0;
        linux_coop_track_fd(fds[0], user_flags);
        linux_coop_track_fd(fds[1], user_flags);
    }
    తిరిగి ret;
}

పూర్ణాంకం freertos_linux_coop_dup(పూర్ణాంకం oldfd)
{
    పూర్ణాంకం fd = real_dup(oldfd);
    linux_coop_సమితి_nonblocking(fd);
    linux_coop_track_fd(fd, linux_coop_fd_user_nonblocking(oldfd) ? O_NONBLOCK : 0);
    తిరిగి fd;
}

పూర్ణాంకం freertos_linux_coop_dup2(పూర్ణాంకం oldfd, పూర్ణాంకం newfd)
{
    పూర్ణాంకం fd = real_dup2(oldfd, newfd);
    linux_coop_సమితి_nonblocking(fd);
    linux_coop_track_fd(fd, linux_coop_fd_user_nonblocking(oldfd) ? O_NONBLOCK : 0);
    తిరిగి fd;
}

__attribute__((weak)) పూర్ణాంకం connect(పూర్ణాంకం sockfd, స్థిరరాశి నిర్మాణం sockaddr *addr, socklen_t addrlen)
{
    తిరిగి freertos_linux_coop_connect(sockfd, addr, addrlen);
}

__attribute__((weak)) పూర్ణాంకం accept(పూర్ణాంకం sockfd, నిర్మాణం sockaddr *addr, socklen_t *addrlen)
{
    తిరిగి freertos_linux_coop_accept(sockfd, addr, addrlen);
}

__attribute__((weak)) పూర్ణాంకం pselect(పూర్ణాంకం nfds, fd_set *readfds, fd_set *writefds,
            fd_set *exceptfds, స్థిరరాశి నిర్మాణం timespec *timeout,
            స్థిరరాశి sigset_t *sigmask)
{
    తిరిగి freertos_linux_coop_pselect(nfds, readfds, writefds, exceptfds, timeout, sigmask);
}

__attribute__((weak)) పూర్ణాంకం poll(నిర్మాణం pollfd *fds, nfds_t nfds, పూర్ణాంకం timeout)
{
    తిరిగి freertos_linux_coop_poll(fds, nfds, timeout);
}

__attribute__((weak)) పూర్ణాంకం socket(పూర్ణాంకం domain, పూర్ణాంకం type, పూర్ణాంకం protocol)
{
    తిరిగి freertos_linux_coop_socket(domain, type, protocol);
}

__attribute__((weak)) పూర్ణాంకం socketpair(పూర్ణాంకం domain, పూర్ణాంకం type, పూర్ణాంకం protocol, పూర్ణాంకం sv[2])
{
    తిరిగి freertos_linux_coop_socketpair(domain, type, protocol, sv);
}

__attribute__((weak)) పూర్ణాంకం pipe(పూర్ణాంకం fds[2])
{
    తిరిగి freertos_linux_coop_pipe(fds);
}

__attribute__((weak)) పూర్ణాంకం pipe2(పూర్ణాంకం fds[2], పూర్ణాంకం flags)
{
    తిరిగి freertos_linux_coop_pipe2(fds, flags);
}

__attribute__((weak)) పూర్ణాంకం dup(పూర్ణాంకం oldfd)
{
    తిరిగి freertos_linux_coop_dup(oldfd);
}

__attribute__((weak)) పూర్ణాంకం dup2(పూర్ణాంకం oldfd, పూర్ణాంకం newfd)
{
    తిరిగి freertos_linux_coop_dup2(oldfd, newfd);
}
