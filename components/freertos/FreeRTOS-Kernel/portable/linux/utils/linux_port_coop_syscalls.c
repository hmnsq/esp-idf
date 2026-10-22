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

#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <stdlib.h>
#ଅନ୍ତର୍ଭାଵ <string.h>
#ଅନ୍ତର୍ଭାଵ <errno.h>
#ଅନ୍ତର୍ଭାଵ <fcntl.h>
#ଅନ୍ତର୍ଭାଵ <stdarg.h>
#ଅନ୍ତର୍ଭାଵ <unistd.h>
#ଅନ୍ତର୍ଭାଵ <sys/types.h>
#ଅନ୍ତର୍ଭାଵ <sys/uio.h>
#ଅନ୍ତର୍ଭାଵ <sys/socket.h>
#ଅନ୍ତର୍ଭାଵ <sys/select.h>
#ଅନ୍ତର୍ଭାଵ <sys/time.h>
#ଅନ୍ତର୍ଭାଵ <time.h>
#ଅନ୍ତର୍ଭାଵ <poll.h>
#ଅନ୍ତର୍ଭାଵ <dlfcn.h>
#ଅନ୍ତର୍ଭାଵ <pthread.h>

#ଅନ୍ତର୍ଭାଵ "linux_port_coop_internal.h"
#ଅନ୍ତର୍ଭାଵ "esp_private/freertos_linux_coop_syscalls.h"

ସ୍ଥିର ssize_t  (*real_readv)(ପୂର୍ଣ୍ଣସଂଖ୍ୟା, ଅଚଳ ସଂରଚନା iovec *, ପୂର୍ଣ୍ଣସଂଖ୍ୟା);
ସ୍ଥିର ssize_t  (*real_writev)(ପୂର୍ଣ୍ଣସଂଖ୍ୟା, ଅଚଳ ସଂରଚନା iovec *, ପୂର୍ଣ୍ଣସଂଖ୍ୟା);
ସ୍ଥିର ssize_t  (*real_recv)(ପୂର୍ଣ୍ଣସଂଖ୍ୟା, ଶୂନ୍ୟ *, size_t, ପୂର୍ଣ୍ଣସଂଖ୍ୟା);
ସ୍ଥିର ssize_t  (*real_send)(ପୂର୍ଣ୍ଣସଂଖ୍ୟା, ଅଚଳ ଶୂନ୍ୟ *, size_t, ପୂର୍ଣ୍ଣସଂଖ୍ୟା);
ସ୍ଥିର ssize_t  (*real_recvfrom)(ପୂର୍ଣ୍ଣସଂଖ୍ୟା, ଶୂନ୍ୟ *, size_t, ପୂର୍ଣ୍ଣସଂଖ୍ୟା,
                                 ସଂରଚନା sockaddr *, socklen_t *);
ସ୍ଥିର ssize_t  (*real_sendto)(ପୂର୍ଣ୍ଣସଂଖ୍ୟା, ଅଚଳ ଶୂନ୍ୟ *, size_t, ପୂର୍ଣ୍ଣସଂଖ୍ୟା,
                                ଅଚଳ ସଂରଚନା sockaddr *, socklen_t);
ସ୍ଥିର ssize_t  (*real_recvmsg)(ପୂର୍ଣ୍ଣସଂଖ୍ୟା, ସଂରଚନା msghdr *, ପୂର୍ଣ୍ଣସଂଖ୍ୟା);
ସ୍ଥିର ssize_t  (*real_sendmsg)(ପୂର୍ଣ୍ଣସଂଖ୍ୟା, ଅଚଳ ସଂରଚନା msghdr *, ପୂର୍ଣ୍ଣସଂଖ୍ୟା);
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା      (*real_connect)(ପୂର୍ଣ୍ଣସଂଖ୍ୟା, ଅଚଳ ସଂରଚନା sockaddr *, socklen_t);
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା      (*real_accept)(ପୂର୍ଣ୍ଣସଂଖ୍ୟା, ସଂରଚନା sockaddr *, socklen_t *);
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା      (*real_pselect)(ପୂର୍ଣ୍ଣସଂଖ୍ୟା, fd_set *, fd_set *, fd_set *,
                                ଅଚଳ ସଂରଚନା timespec *, ଅଚଳ sigset_t *);
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା      (*real_poll)(ସଂରଚନା pollfd *, nfds_t, ପୂର୍ଣ୍ଣସଂଖ୍ୟା);
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା      (*real_socket)(ପୂର୍ଣ୍ଣସଂଖ୍ୟା, ପୂର୍ଣ୍ଣସଂଖ୍ୟା, ପୂର୍ଣ୍ଣସଂଖ୍ୟା);
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା      (*real_socketpair)(ପୂର୍ଣ୍ଣସଂଖ୍ୟା, ପୂର୍ଣ୍ଣସଂଖ୍ୟା, ପୂର୍ଣ୍ଣସଂଖ୍ୟା, ପୂର୍ଣ୍ଣସଂଖ୍ୟା[2]);
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା      (*real_pipe)(ପୂର୍ଣ୍ଣସଂଖ୍ୟା[2]);
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା      (*real_pipe2)(ପୂର୍ଣ୍ଣସଂଖ୍ୟା[2], ପୂର୍ଣ୍ଣସଂଖ୍ୟା);
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା      (*real_dup)(ପୂର୍ଣ୍ଣସଂଖ୍ୟା);
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା      (*real_dup2)(ପୂର୍ଣ୍ଣସଂଖ୍ୟା, ପୂର୍ଣ୍ଣସଂଖ୍ୟା);
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା      (*real_nanosleep)(ଅଚଳ ସଂରଚନା timespec *, ସଂରଚନା timespec *);
ସ୍ଥିର ssize_t  (*real_read)(ପୂର୍ଣ୍ଣସଂଖ୍ୟା, ଶୂନ୍ୟ *, size_t);
ସ୍ଥିର ssize_t  (*real_write)(ପୂର୍ଣ୍ଣସଂଖ୍ୟା, ଅଚଳ ଶୂନ୍ୟ *, size_t);
ସ୍ଥିର ssize_t  (*real_pread)(ପୂର୍ଣ୍ଣସଂଖ୍ୟା, ଶୂନ୍ୟ *, size_t, off_t);
ସ୍ଥିର ssize_t  (*real_pwrite)(ପୂର୍ଣ୍ଣସଂଖ୍ୟା, ଅଚଳ ଶୂନ୍ୟ *, size_t, off_t);
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା      (*real_close)(ପୂର୍ଣ୍ଣସଂଖ୍ୟା);
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା      (*real_select)(ପୂର୍ଣ୍ଣସଂଖ୍ୟା, fd_set *, fd_set *, fd_set *,
                                ସଂରଚନା timeval *);

ପ୍ରକାରବ୍ୟାଖ୍ୟା ପୂର୍ଣ୍ଣସଂଖ୍ୟା (*linux_coop_fcntl_fn_t)(ପୂର୍ଣ୍ଣସଂଖ୍ୟା, ପୂର୍ଣ୍ଣସଂଖ୍ୟା, ...);
ସ୍ଥିର linux_coop_fcntl_fn_t real_fcntl;

ପ୍ରକାରବ୍ୟାଖ୍ୟା ପୂର୍ଣ୍ଣସଂଖ୍ୟା (*linux_coop_open_fn_t)(ଅଚଳ ଅକ୍ଷର *, ପୂର୍ଣ୍ଣସଂଖ୍ୟା, mode_t);
ସ୍ଥିର linux_coop_open_fn_t real_open;

ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା linux_coop_fd_state {
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା user_flags;
    ସଂରଚନା linux_coop_fd_state *next;
} linux_coop_fd_state_t;

ସ୍ଥିର linux_coop_fd_state_t *s_fd_state_list;
ସ୍ଥିର pthread_mutex_t s_fd_state_lock = PTHREAD_MUTEX_INITIALIZER;

ସ୍ଥିର linux_coop_fd_state_t *linux_coop_find_fd_locked(ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd)
{
    linux_coop_fd_state_t *ଗଣ୍ଠି = s_fd_state_list;
    ଯେତେବେଳେ (ଗଣ୍ଠି != ଖାଲି) {
        ଯଦି (ଗଣ୍ଠି->fd == fd) {
            ଫେରନ୍ତୁ ଗଣ୍ଠି;
        }
        ଗଣ୍ଠି = ଗଣ୍ଠି->next;
    }
    ଫେରନ୍ତୁ ଖାଲି;
}

ସ୍ଥିର ଶୂନ୍ୟ __attribute__((constructor)) linux_coop_resolve_all(ଶୂନ୍ୟ)
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
    LINUX_COOP_RESOLVE(ପଠ);
    LINUX_COOP_RESOLVE(ଲିଖ);
    LINUX_COOP_RESOLVE(pread);
    LINUX_COOP_RESOLVE(pwrite);
    LINUX_COOP_RESOLVE(close);
    LINUX_COOP_RESOLVE(select);
    LINUX_COOP_RESOLVE(open);
}

ଶୂନ୍ୟ linux_coop_set_nonblocking(ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd)
{
    ଯଦି (fd >= 0) {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags = real_fcntl(fd, F_GETFL, 0);
        ଯଦି (flags >= 0) {
            real_fcntl(fd, F_SETFL, flags | O_NONBLOCK);
        }
    }
}

ଶୂନ୍ୟ linux_coop_track_fd(ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd, ପୂର୍ଣ୍ଣସଂଖ୍ୟା user_flags)
{
    ଯଦି (fd < 0) {
        ଫେରନ୍ତୁ;
    }

    pthread_mutex_lock(&s_fd_state_lock);
    linux_coop_fd_state_t *ଗଣ୍ଠି = linux_coop_find_fd_locked(fd);
    ଯଦି (ଗଣ୍ଠି != ଖାଲି) {
        ଗଣ୍ଠି->user_flags = user_flags;
        pthread_mutex_unlock(&s_fd_state_lock);
        ଫେରନ୍ତୁ;
    }

    ଗଣ୍ଠି = calloc(1, ଆକାର(*ଗଣ୍ଠି));
    ଯଦି (ଗଣ୍ଠି != ଖାଲି) {
        ଗଣ୍ଠି->fd = fd;
        ଗଣ୍ଠି->user_flags = user_flags;
        ଗଣ୍ଠି->next = s_fd_state_list;
        s_fd_state_list = ଗଣ୍ଠି;
    }
    pthread_mutex_unlock(&s_fd_state_lock);
}

ଶୂନ୍ୟ linux_coop_untrack_fd(ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd)
{
    ଯଦି (fd < 0) {
        ଫେରନ୍ତୁ;
    }

    pthread_mutex_lock(&s_fd_state_lock);
    linux_coop_fd_state_t *prev = ଖାଲି;
    linux_coop_fd_state_t *ଗଣ୍ଠି = s_fd_state_list;
    ଯେତେବେଳେ (ଗଣ୍ଠି != ଖାଲି) {
        ଯଦି (ଗଣ୍ଠି->fd == fd) {
            ଯଦି (prev == ଖାଲି) {
                s_fd_state_list = ଗଣ୍ଠି->next;
            } ଅନ୍ୟଥା {
                prev->next = ଗଣ୍ଠି->next;
            }
            ମୁକ୍ତ(ଗଣ୍ଠି);
            ଭାଙ୍ଗ;
        }
        prev = ଗଣ୍ଠି;
        ଗଣ୍ଠି = ଗଣ୍ଠି->next;
    }
    pthread_mutex_unlock(&s_fd_state_lock);
}

ଶୂନ୍ୟ linux_coop_set_user_flags(ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd, ପୂର୍ଣ୍ଣସଂଖ୍ୟା user_flags)
{
    linux_coop_track_fd(fd, user_flags);
}

bool linux_coop_get_user_flags(ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd, ପୂର୍ଣ୍ଣସଂଖ୍ୟା *flags)
{
    ଯଦି (fd < 0 || flags == ଖାଲି) {
        ଫେରନ୍ତୁ false;
    }

    pthread_mutex_lock(&s_fd_state_lock);
    linux_coop_fd_state_t *ଗଣ୍ଠି = linux_coop_find_fd_locked(fd);
    ଯଦି (ଗଣ୍ଠି == ଖାଲି) {
        pthread_mutex_unlock(&s_fd_state_lock);
        ଫେରନ୍ତୁ false;
    }
    *flags = ଗଣ୍ଠି->user_flags;
    pthread_mutex_unlock(&s_fd_state_lock);
    ଫେରନ୍ତୁ true;
}

bool linux_coop_fd_user_nonblocking(ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା user_flags = 0;
    ଯଦି (linux_coop_get_user_flags(fd, &user_flags)) {
        ଫେରନ୍ତୁ (user_flags & O_NONBLOCK) != 0;
    }
    ଫେରନ୍ତୁ false;
}


ssize_t freertos_linux_coop_read(ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd, ଶୂନ୍ୟ *buf, size_t ଗଣନା_2)
{
    LINUX_COOP_IO_LOOP(fd, real_read(fd, buf, ଗଣନା_2))
}

ssize_t freertos_linux_coop_write(ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd, ଅଚଳ ଶୂନ୍ୟ *buf, size_t ଗଣନା_2)
{
    LINUX_COOP_IO_LOOP(fd, real_write(fd, buf, ଗଣନା_2))
}

ssize_t freertos_linux_coop_pread(ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd, ଶୂନ୍ୟ *buf, size_t ଗଣନା_2, off_t offset)
{
    LINUX_COOP_IO_LOOP(fd, real_pread(fd, buf, ଗଣନା_2, offset))
}

ssize_t freertos_linux_coop_pwrite(ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd, ଅଚଳ ଶୂନ୍ୟ *buf, size_t ଗଣନା_2, off_t offset)
{
    LINUX_COOP_IO_LOOP(fd, real_pwrite(fd, buf, ଗଣନା_2, offset))
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା freertos_linux_coop_open(ଅଚଳ ଅକ୍ଷର *path, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags, ପୂର୍ଣ୍ଣସଂଖ୍ୟା mode)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd = real_open(path, flags, (mode_t)mode);
    linux_coop_set_nonblocking(fd);
    linux_coop_track_fd(fd, flags & O_NONBLOCK);
    ଫେରନ୍ତୁ fd;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା freertos_linux_coop_close(ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = real_close(fd);
    ଯଦି (ret == 0 || errno == EINTR) {
        /* On Linux, close() always releases the fd even when returning
         * EINTR.  Retrying would risk closing a fd reused by another
         * thread.  Treat EINTR as success. */
        linux_coop_untrack_fd(fd);
        ଫେରନ୍ତୁ 0;
    }
    ଫେରନ୍ତୁ -1;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା freertos_linux_coop_fcntl(ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd, ପୂର୍ଣ୍ଣସଂଖ୍ୟା arg)
{
    ଯଦି (cmd == F_GETFL) {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା real_flags = real_fcntl(fd, F_GETFL, 0);
        ଯଦି (real_flags < 0) {
            ଫେରନ୍ତୁ -1;
        }
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା user_flags = 0;
        ଯଦି (linux_coop_get_user_flags(fd, &user_flags)) {
            real_flags &= ~O_NONBLOCK;
            real_flags |= (user_flags & O_NONBLOCK);
        }
        ଫେରନ୍ତୁ real_flags;
    }
    ଯଦି (cmd == F_SETFL) {
        linux_coop_set_user_flags(fd, arg);
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା internal_flags = arg | O_NONBLOCK;
        ଫେରନ୍ତୁ real_fcntl(fd, F_SETFL, internal_flags);
    }
    ଫେରନ୍ତୁ real_fcntl(fd, cmd, arg);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା freertos_linux_coop_select(ପୂର୍ଣ୍ଣସଂଖ୍ୟା nfds, fd_set *readfds, fd_set *writefds,
                      fd_set *exceptfds, ସଂରଚନା timeval *timeout)
{
    long timeout_ms = -1;
    ଯଦି (timeout != ଖାଲି) {
        timeout_ms = (long)timeout->tv_sec * 1000 +
                     (timeout->tv_usec + 999) / 1000;
        ଯଦି (timeout_ms == 0) {
            ଫେରନ୍ତୁ real_select(nfds, readfds, writefds, exceptfds, timeout);
        }
    }

    /* real_select() mutates the fd_sets in place (clears them, then sets bits
     * for ready descriptors). To keep polling the same descriptors across
     * iterations we must snapshot the caller's sets and restore them on every
     * loop. */
    fd_set rfds_in, wfds_in, efds_in;
    ଯଦି (readfds != ଖାଲି) {
        rfds_in = *readfds;
    }
    ଯଦି (writefds != ଖାଲି) {
        wfds_in = *writefds;
    }
    ଯଦି (exceptfds != ଖାଲି) {
        efds_in = *exceptfds;
    }

    long waited_ms = 0;
    ଯେତେବେଳେ (1) {
        ଯଦି (readfds != ଖାଲି) {
            *readfds = rfds_in;
        }
        ଯଦି (writefds != ଖାଲି) {
            *writefds = wfds_in;
        }
        ଯଦି (exceptfds != ଖାଲି) {
            *exceptfds = efds_in;
        }

        ସଂରଚନା timeval zero_tv = {0, 0};
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = real_select(nfds, readfds, writefds, exceptfds, &zero_tv);
        ଯଦି (ret != 0) {
            ଫେରନ୍ତୁ ret;
        }
        ଯଦି (timeout_ms == 0) {
            ଫେରନ୍ତୁ 0;
        }
        ଯଦି (timeout_ms > 0 && waited_ms >= timeout_ms) {
            ଫେରନ୍ତୁ 0;
        }
        linux_coop_yield(LINUX_COOP_TICK_MS);
        waited_ms += LINUX_COOP_TICK_MS;
    }
}

ଶୂନ୍ୟ freertos_linux_coop_syscalls_init(ଶୂନ୍ୟ)
{
    linux_coop_set_nonblocking(STDIN_FILENO);
    linux_coop_set_nonblocking(STDOUT_FILENO);
    linux_coop_set_nonblocking(STDERR_FILENO);
    linux_coop_track_fd(STDIN_FILENO, 0);
    linux_coop_track_fd(STDOUT_FILENO, 0);
    linux_coop_track_fd(STDERR_FILENO, 0);
}

/* These are overridden by strong definitions in VFS when that component
 * is linked.  When VFS is absent the weak versions provide cooperative
 * behaviour directly via real libc. */

__attribute__((weak)) ssize_t ପଠ(ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd, ଶୂନ୍ୟ *buf, size_t ଗଣନା_2)
{
    ଫେରନ୍ତୁ freertos_linux_coop_read(fd, buf, ଗଣନା_2);
}

__attribute__((weak)) ssize_t ଲିଖ(ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd, ଅଚଳ ଶୂନ୍ୟ *buf, size_t ଗଣନା_2)
{
    ଫେରନ୍ତୁ freertos_linux_coop_write(fd, buf, ଗଣନା_2);
}

__attribute__((weak)) ssize_t pread(ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd, ଶୂନ୍ୟ *buf, size_t ଗଣନା_2, off_t offset)
{
    ଫେରନ୍ତୁ freertos_linux_coop_pread(fd, buf, ଗଣନା_2, offset);
}

__attribute__((weak)) ssize_t pwrite(ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd, ଅଚଳ ଶୂନ୍ୟ *buf, size_t ଗଣନା_2, off_t offset)
{
    ଫେରନ୍ତୁ freertos_linux_coop_pwrite(fd, buf, ଗଣନା_2, offset);
}

__attribute__((weak)) ପୂର୍ଣ୍ଣସଂଖ୍ୟା open(ଅଚଳ ଅକ୍ଷର *path, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags, ...)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା mode = 0;
    ଯଦି (flags & O_CREAT) {
        va_list ap;
        va_start(ap, flags);
        mode = va_arg(ap, ପୂର୍ଣ୍ଣସଂଖ୍ୟା);
        va_end(ap);
    }
    ଫେରନ୍ତୁ freertos_linux_coop_open(path, flags, mode);
}

__attribute__((weak)) ପୂର୍ଣ୍ଣସଂଖ୍ୟା close(ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd)
{
    ଫେରନ୍ତୁ freertos_linux_coop_close(fd);
}

__attribute__((weak)) ପୂର୍ଣ୍ଣସଂଖ୍ୟା fcntl(ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd, ...)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା arg = 0;
    ଯଦି (cmd != F_GETFL) {
        va_list list;
        va_start(list, cmd);
        arg = va_arg(list, ପୂର୍ଣ୍ଣସଂଖ୍ୟା);
        va_end(list);
    }
    ଫେରନ୍ତୁ freertos_linux_coop_fcntl(fd, cmd, arg);
}

__attribute__((weak)) ପୂର୍ଣ୍ଣସଂଖ୍ୟା select(ପୂର୍ଣ୍ଣସଂଖ୍ୟା nfds, fd_set *readfds, fd_set *writefds,
                                 fd_set *exceptfds, ସଂରଚନା timeval *timeout)
{
    ଫେରନ୍ତୁ freertos_linux_coop_select(nfds, readfds, writefds, exceptfds, timeout);
}

ssize_t freertos_linux_coop_readv(ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd, ଅଚଳ ସଂରଚନା iovec *iov, ପୂର୍ଣ୍ଣସଂଖ୍ୟା iovcnt)
{
    LINUX_COOP_IO_LOOP(fd, real_readv(fd, iov, iovcnt))
}

ssize_t freertos_linux_coop_writev(ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd, ଅଚଳ ସଂରଚନା iovec *iov, ପୂର୍ଣ୍ଣସଂଖ୍ୟା iovcnt)
{
    LINUX_COOP_IO_LOOP(fd, real_writev(fd, iov, iovcnt))
}

ssize_t freertos_linux_coop_recv(ପୂର୍ଣ୍ଣସଂଖ୍ୟା sockfd, ଶୂନ୍ୟ *buf, size_t len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags)
{
    LINUX_COOP_IO_LOOP(sockfd, real_recv(sockfd, buf, len, flags | MSG_DONTWAIT))
}

ssize_t freertos_linux_coop_send(ପୂର୍ଣ୍ଣସଂଖ୍ୟା sockfd, ଅଚଳ ଶୂନ୍ୟ *buf, size_t len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags)
{
    LINUX_COOP_IO_LOOP(sockfd, real_send(sockfd, buf, len, flags | MSG_DONTWAIT))
}

ssize_t freertos_linux_coop_recvfrom(ପୂର୍ଣ୍ଣସଂଖ୍ୟା sockfd, ଶୂନ୍ୟ *buf, size_t len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags,
                 ସଂରଚନା sockaddr *src_addr, socklen_t *addrlen)
{
    LINUX_COOP_IO_LOOP(sockfd, real_recvfrom(sockfd, buf, len, flags | MSG_DONTWAIT,
                            src_addr, addrlen))
}

ssize_t freertos_linux_coop_sendto(ପୂର୍ଣ୍ଣସଂଖ୍ୟା sockfd, ଅଚଳ ଶୂନ୍ୟ *buf, size_t len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags,
               ଅଚଳ ସଂରଚନା sockaddr *dest_addr, socklen_t addrlen)
{
    LINUX_COOP_IO_LOOP(sockfd, real_sendto(sockfd, buf, len, flags | MSG_DONTWAIT,
                          dest_addr, addrlen))
}

ssize_t freertos_linux_coop_recvmsg(ପୂର୍ଣ୍ଣସଂଖ୍ୟା sockfd, ସଂରଚନା msghdr *ସନ୍ଦେଶ, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags)
{
    LINUX_COOP_IO_LOOP(sockfd, real_recvmsg(sockfd, ସନ୍ଦେଶ, flags | MSG_DONTWAIT))
}

ssize_t freertos_linux_coop_sendmsg(ପୂର୍ଣ୍ଣସଂଖ୍ୟା sockfd, ଅଚଳ ସଂରଚନା msghdr *ସନ୍ଦେଶ, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags)
{
    LINUX_COOP_IO_LOOP(sockfd, real_sendmsg(sockfd, ସନ୍ଦେଶ, flags | MSG_DONTWAIT))
}

__attribute__((weak)) ssize_t readv(ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd, ଅଚଳ ସଂରଚନା iovec *iov, ପୂର୍ଣ୍ଣସଂଖ୍ୟା iovcnt)
{
    ଫେରନ୍ତୁ freertos_linux_coop_readv(fd, iov, iovcnt);
}

__attribute__((weak)) ssize_t writev(ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd, ଅଚଳ ସଂରଚନା iovec *iov, ପୂର୍ଣ୍ଣସଂଖ୍ୟା iovcnt)
{
    ଫେରନ୍ତୁ freertos_linux_coop_writev(fd, iov, iovcnt);
}

__attribute__((weak)) ssize_t recv(ପୂର୍ଣ୍ଣସଂଖ୍ୟା sockfd, ଶୂନ୍ୟ *buf, size_t len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags)
{
    ଫେରନ୍ତୁ freertos_linux_coop_recv(sockfd, buf, len, flags);
}

__attribute__((weak)) ssize_t send(ପୂର୍ଣ୍ଣସଂଖ୍ୟା sockfd, ଅଚଳ ଶୂନ୍ୟ *buf, size_t len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags)
{
    ଫେରନ୍ତୁ freertos_linux_coop_send(sockfd, buf, len, flags);
}

__attribute__((weak)) ssize_t recvfrom(ପୂର୍ଣ୍ଣସଂଖ୍ୟା sockfd, ଶୂନ୍ୟ *buf, size_t len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags,
                 ସଂରଚନା sockaddr *src_addr, socklen_t *addrlen)
{
    ଫେରନ୍ତୁ freertos_linux_coop_recvfrom(sockfd, buf, len, flags, src_addr, addrlen);
}

__attribute__((weak)) ssize_t sendto(ପୂର୍ଣ୍ଣସଂଖ୍ୟା sockfd, ଅଚଳ ଶୂନ୍ୟ *buf, size_t len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags,
               ଅଚଳ ସଂରଚନା sockaddr *dest_addr, socklen_t addrlen)
{
    ଫେରନ୍ତୁ freertos_linux_coop_sendto(sockfd, buf, len, flags, dest_addr, addrlen);
}

__attribute__((weak)) ssize_t recvmsg(ପୂର୍ଣ୍ଣସଂଖ୍ୟା sockfd, ସଂରଚନା msghdr *ସନ୍ଦେଶ, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags)
{
    ଫେରନ୍ତୁ freertos_linux_coop_recvmsg(sockfd, ସନ୍ଦେଶ, flags);
}

__attribute__((weak)) ssize_t sendmsg(ପୂର୍ଣ୍ଣସଂଖ୍ୟା sockfd, ଅଚଳ ସଂରଚନା msghdr *ସନ୍ଦେଶ, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags)
{
    ଫେରନ୍ତୁ freertos_linux_coop_sendmsg(sockfd, ସନ୍ଦେଶ, flags);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା freertos_linux_coop_connect(ପୂର୍ଣ୍ଣସଂଖ୍ୟା sockfd, ଅଚଳ ସଂରଚନା sockaddr *addr, socklen_t addrlen)
{
    ଯେତେବେଳେ (1)
    {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = real_connect(sockfd, addr, addrlen);
        ଯଦି (ret == 0) {
            ଫେରନ୍ତୁ ret;
        }

        ଯଦି (errno == EINPROGRESS || errno == EALREADY) {
            ସଂରଚନା pollfd pfd;
            pfd.fd = sockfd;
            pfd.events = POLLOUT;
            pfd.revents = 0;

            ପୂର୍ଣ୍ଣସଂଖ୍ୟା poll_retries = 0;
            ଯେତେବେଳେ (1)
            {
                ପୂର୍ଣ୍ଣସଂଖ୍ୟା press = real_poll(&pfd, 1, 0);
                ଯଦି (press > 0) {
                    ପୂର୍ଣ୍ଣସଂଖ୍ୟା so_err = 0;
                    socklen_t len = ଆକାର(so_err);
                    ଯଦି (getsockopt(sockfd, SOL_SOCKET, SO_ERROR,
                                   &so_err, &len) < 0) {
                        ଫେରନ୍ତୁ -1;
                    }
                    ଯଦି (so_err == 0) {
                        ଫେରନ୍ତୁ 0;
                    } ଅନ୍ୟଥା {
                        errno = so_err;
                        ଫେରନ୍ତୁ -1;
                    }
                } ଅନ୍ୟଥା ଯଦି (press == 0) {
                    linux_coop_yield(LINUX_COOP_TICK_MS);
                    ଜାରି;
                } ଅନ୍ୟଥା {
                    ଯଦି (errno == EINTR) {
                        ଜାରି;
                    }
                    /* Non-EINTR poll error; bail out after a few retries */
                    ଯଦି (++poll_retries > 3) {
                        ଫେରନ୍ତୁ -1;
                    }
                    linux_coop_yield(LINUX_COOP_TICK_MS);
                    ଜାରି;
                }
            }
        }

        ଯଦି (errno == EINTR) {
            ଫେରନ୍ତୁ -1;
        }

        ଫେରନ୍ତୁ -1;
    }
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା freertos_linux_coop_accept(ପୂର୍ଣ୍ଣସଂଖ୍ୟା sockfd, ସଂରଚନା sockaddr *addr, socklen_t *addrlen)
{
    ଯେତେବେଳେ (1)
    {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd = real_accept(sockfd, addr, addrlen);
        ଯଦି (fd >= 0) {
            linux_coop_set_nonblocking(fd);
            linux_coop_track_fd(fd, linux_coop_fd_user_nonblocking(sockfd) ? O_NONBLOCK : 0);
            ଫେରନ୍ତୁ fd;
        }
        ଯଦି (errno == EAGAIN || errno == EWOULDBLOCK) {
            ଯଦି (linux_coop_fd_user_nonblocking(sockfd)) {
                ଫେରନ୍ତୁ -1;
            }
            linux_coop_yield(LINUX_COOP_TICK_MS);
            ଜାରି;
        }
        ଫେରନ୍ତୁ -1;
    }
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା freertos_linux_coop_pselect(ପୂର୍ଣ୍ଣସଂଖ୍ୟା nfds, fd_set *readfds, fd_set *writefds,
            fd_set *exceptfds, ଅଚଳ ସଂରଚନା timespec *timeout,
            ଅଚଳ sigset_t *sigmask)
{
    long timeout_ms = -1;
    ଯଦି (timeout != ଖାଲି) {
        timeout_ms = (long)timeout->tv_sec * 1000 +
                     (timeout->tv_nsec + 999999) / 1000000;
        ଯଦି (timeout_ms == 0) {
            ଫେରନ୍ତୁ real_pselect(nfds, readfds, writefds, exceptfds,
                                timeout, sigmask);
        }
    }

    long waited_ms = 0;
    ଯେତେବେଳେ (1)
    {
        ସଂରଚନା timespec zero_ts = {0, 0};
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = real_pselect(nfds, readfds, writefds, exceptfds,
                               &zero_ts, sigmask);
        ଯଦି (ret != 0) {
            ଫେରନ୍ତୁ ret;
        }

        ଯଦି (timeout_ms == 0 ||
            (timeout_ms > 0 && waited_ms >= timeout_ms)) {
            ଫେରନ୍ତୁ 0;
        }

        linux_coop_yield(LINUX_COOP_TICK_MS);
        waited_ms += LINUX_COOP_TICK_MS;
    }
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା freertos_linux_coop_poll(ସଂରଚନା pollfd *fds, nfds_t nfds, ପୂର୍ଣ୍ଣସଂଖ୍ୟା timeout)
{
    ଯଦି (timeout == 0) {
        ଫେରନ୍ତୁ real_poll(fds, nfds, 0);
    }

    long timeout_ms = -1;
    ଯଦି (timeout > 0) {
        timeout_ms = timeout;
    }

    long waited_ms = 0;

    ଯେତେବେଳେ (1)
    {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = real_poll(fds, nfds, 0);
        ଯଦି (ret > 0) {
            ଫେରନ୍ତୁ ret;
        } ଅନ୍ୟଥା ଯଦି (ret == 0) {
            ଯଦି (timeout_ms == 0 ||
                (timeout_ms > 0 && waited_ms >= timeout_ms)) {
                ଫେରନ୍ତୁ 0;
            }
            linux_coop_yield(LINUX_COOP_TICK_MS);
            waited_ms += LINUX_COOP_TICK_MS;
            ଜାରି;
        } ଅନ୍ୟଥା {
            ଯଦି (errno == EINTR) {
                ଜାରି;
            }
            linux_coop_yield(LINUX_COOP_TICK_MS);
            ଯଦି (timeout_ms > 0 && waited_ms >= timeout_ms) {
                ଫେରନ୍ତୁ -1;
            }
            waited_ms += LINUX_COOP_TICK_MS;
        }
    }
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା nanosleep(ଅଚଳ ସଂରଚନା timespec *req, ସଂରଚନା timespec *rem)
{
    ଯଦି (req == ଖାଲି) {
        errno = EFAULT;
        ଫେରନ୍ତୁ -1;
    }
    ଯଦି (!linux_port_in_freertos_task()) {
        ଫେରନ୍ତୁ real_nanosleep(req, rem);
    }
    long ms = req->tv_sec * 1000 + (req->tv_nsec + 999999) / 1000000;
    ଯଦି (ms <= 0) {
        ms = 1;
    }
    vTaskDelay(ms);
    ଯଦି (rem) {
        rem->tv_sec = 0;
        rem->tv_nsec = 0;
    }
    ଫେରନ୍ତୁ 0;
}

unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା sleep(unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା seconds)
{
    linux_coop_yield(seconds * 1000);
    ଫେରନ୍ତୁ 0;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା usleep(useconds_t usec)
{
    linux_coop_yield(usec / 1000);
    ଫେରନ୍ତୁ 0;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା freertos_linux_coop_socket(ପୂର୍ଣ୍ଣସଂଖ୍ୟା domain, ପୂର୍ଣ୍ଣସଂଖ୍ୟା type, ପୂର୍ଣ୍ଣସଂଖ୍ୟା protocol)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd = real_socket(domain, type, protocol);
    linux_coop_set_nonblocking(fd);
#ifdef SOCK_NONBLOCK
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା user_flags = (type & SOCK_NONBLOCK) ? O_NONBLOCK : 0;
#ଅନ୍ୟଥା
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା user_flags = 0;
#endif
    linux_coop_track_fd(fd, user_flags);
    ଫେରନ୍ତୁ fd;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା freertos_linux_coop_socketpair(ପୂର୍ଣ୍ଣସଂଖ୍ୟା domain, ପୂର୍ଣ୍ଣସଂଖ୍ୟା type, ପୂର୍ଣ୍ଣସଂଖ୍ୟା protocol, ପୂର୍ଣ୍ଣସଂଖ୍ୟା *sv)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = real_socketpair(domain, type, protocol, sv);
    ଯଦି (ret == 0) {
        linux_coop_set_nonblocking(sv[0]);
        linux_coop_set_nonblocking(sv[1]);
#ifdef SOCK_NONBLOCK
        ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା user_flags = (type & SOCK_NONBLOCK) ? O_NONBLOCK : 0;
#ଅନ୍ୟଥା
        ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା user_flags = 0;
#endif
        linux_coop_track_fd(sv[0], user_flags);
        linux_coop_track_fd(sv[1], user_flags);
    }
    ଫେରନ୍ତୁ ret;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା freertos_linux_coop_pipe(ପୂର୍ଣ୍ଣସଂଖ୍ୟା *fds)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = real_pipe(fds);
    ଯଦି (ret == 0) {
        linux_coop_set_nonblocking(fds[0]);
        linux_coop_set_nonblocking(fds[1]);
        linux_coop_track_fd(fds[0], 0);
        linux_coop_track_fd(fds[1], 0);
    }
    ଫେରନ୍ତୁ ret;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା freertos_linux_coop_pipe2(ପୂର୍ଣ୍ଣସଂଖ୍ୟା *fds, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = real_pipe2(fds, flags);
    ଯଦି (ret == 0) {
        linux_coop_set_nonblocking(fds[0]);
        linux_coop_set_nonblocking(fds[1]);
        ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା user_flags = (flags & O_NONBLOCK) ? O_NONBLOCK : 0;
        linux_coop_track_fd(fds[0], user_flags);
        linux_coop_track_fd(fds[1], user_flags);
    }
    ଫେରନ୍ତୁ ret;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା freertos_linux_coop_dup(ପୂର୍ଣ୍ଣସଂଖ୍ୟା oldfd)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd = real_dup(oldfd);
    linux_coop_set_nonblocking(fd);
    linux_coop_track_fd(fd, linux_coop_fd_user_nonblocking(oldfd) ? O_NONBLOCK : 0);
    ଫେରନ୍ତୁ fd;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା freertos_linux_coop_dup2(ପୂର୍ଣ୍ଣସଂଖ୍ୟା oldfd, ପୂର୍ଣ୍ଣସଂଖ୍ୟା newfd)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd = real_dup2(oldfd, newfd);
    linux_coop_set_nonblocking(fd);
    linux_coop_track_fd(fd, linux_coop_fd_user_nonblocking(oldfd) ? O_NONBLOCK : 0);
    ଫେରନ୍ତୁ fd;
}

__attribute__((weak)) ପୂର୍ଣ୍ଣସଂଖ୍ୟା connect(ପୂର୍ଣ୍ଣସଂଖ୍ୟା sockfd, ଅଚଳ ସଂରଚନା sockaddr *addr, socklen_t addrlen)
{
    ଫେରନ୍ତୁ freertos_linux_coop_connect(sockfd, addr, addrlen);
}

__attribute__((weak)) ପୂର୍ଣ୍ଣସଂଖ୍ୟା accept(ପୂର୍ଣ୍ଣସଂଖ୍ୟା sockfd, ସଂରଚନା sockaddr *addr, socklen_t *addrlen)
{
    ଫେରନ୍ତୁ freertos_linux_coop_accept(sockfd, addr, addrlen);
}

__attribute__((weak)) ପୂର୍ଣ୍ଣସଂଖ୍ୟା pselect(ପୂର୍ଣ୍ଣସଂଖ୍ୟା nfds, fd_set *readfds, fd_set *writefds,
            fd_set *exceptfds, ଅଚଳ ସଂରଚନା timespec *timeout,
            ଅଚଳ sigset_t *sigmask)
{
    ଫେରନ୍ତୁ freertos_linux_coop_pselect(nfds, readfds, writefds, exceptfds, timeout, sigmask);
}

__attribute__((weak)) ପୂର୍ଣ୍ଣସଂଖ୍ୟା poll(ସଂରଚନା pollfd *fds, nfds_t nfds, ପୂର୍ଣ୍ଣସଂଖ୍ୟା timeout)
{
    ଫେରନ୍ତୁ freertos_linux_coop_poll(fds, nfds, timeout);
}

__attribute__((weak)) ପୂର୍ଣ୍ଣସଂଖ୍ୟା socket(ପୂର୍ଣ୍ଣସଂଖ୍ୟା domain, ପୂର୍ଣ୍ଣସଂଖ୍ୟା type, ପୂର୍ଣ୍ଣସଂଖ୍ୟା protocol)
{
    ଫେରନ୍ତୁ freertos_linux_coop_socket(domain, type, protocol);
}

__attribute__((weak)) ପୂର୍ଣ୍ଣସଂଖ୍ୟା socketpair(ପୂର୍ଣ୍ଣସଂଖ୍ୟା domain, ପୂର୍ଣ୍ଣସଂଖ୍ୟା type, ପୂର୍ଣ୍ଣସଂଖ୍ୟା protocol, ପୂର୍ଣ୍ଣସଂଖ୍ୟା sv[2])
{
    ଫେରନ୍ତୁ freertos_linux_coop_socketpair(domain, type, protocol, sv);
}

__attribute__((weak)) ପୂର୍ଣ୍ଣସଂଖ୍ୟା pipe(ପୂର୍ଣ୍ଣସଂଖ୍ୟା fds[2])
{
    ଫେରନ୍ତୁ freertos_linux_coop_pipe(fds);
}

__attribute__((weak)) ପୂର୍ଣ୍ଣସଂଖ୍ୟା pipe2(ପୂର୍ଣ୍ଣସଂଖ୍ୟା fds[2], ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags)
{
    ଫେରନ୍ତୁ freertos_linux_coop_pipe2(fds, flags);
}

__attribute__((weak)) ପୂର୍ଣ୍ଣସଂଖ୍ୟା dup(ପୂର୍ଣ୍ଣସଂଖ୍ୟା oldfd)
{
    ଫେରନ୍ତୁ freertos_linux_coop_dup(oldfd);
}

__attribute__((weak)) ପୂର୍ଣ୍ଣସଂଖ୍ୟା dup2(ପୂର୍ଣ୍ଣସଂଖ୍ୟା oldfd, ପୂର୍ଣ୍ଣସଂଖ୍ୟା newfd)
{
    ଫେରନ୍ତୁ freertos_linux_coop_dup2(oldfd, newfd);
}
