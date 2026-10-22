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

#अन्तर्भाव <stdio.h>
#अन्तर्भाव <stdlib.h>
#अन्तर्भाव <string.h>
#अन्तर्भाव <errno.h>
#अन्तर्भाव <fcntl.h>
#अन्तर्भाव <stdarg.h>
#अन्तर्भाव <unistd.h>
#अन्तर्भाव <sys/types.h>
#अन्तर्भाव <sys/uio.h>
#अन्तर्भाव <sys/socket.h>
#अन्तर्भाव <sys/select.h>
#अन्तर्भाव <sys/time.h>
#अन्तर्भाव <time.h>
#अन्तर्भाव <poll.h>
#अन्तर्भाव <dlfcn.h>
#अन्तर्भाव <pthread.h>

#अन्तर्भाव "linux_port_coop_internal.h"
#अन्तर्भाव "esp_private/freertos_linux_coop_syscalls.h"

स्थिर ssize_t  (*real_readv)(पूर्णाङ्क, ध्रुव संरचना iovec *, पूर्णाङ्क);
स्थिर ssize_t  (*real_writev)(पूर्णाङ्क, ध्रुव संरचना iovec *, पूर्णाङ्क);
स्थिर ssize_t  (*real_recv)(पूर्णाङ्क, शून्य *, size_t, पूर्णाङ्क);
स्थिर ssize_t  (*real_send)(पूर्णाङ्क, ध्रुव शून्य *, size_t, पूर्णाङ्क);
स्थिर ssize_t  (*real_recvfrom)(पूर्णाङ्क, शून्य *, size_t, पूर्णाङ्क,
                                 संरचना sockaddr *, socklen_t *);
स्थिर ssize_t  (*real_sendto)(पूर्णाङ्क, ध्रुव शून्य *, size_t, पूर्णाङ्क,
                                ध्रुव संरचना sockaddr *, socklen_t);
स्थिर ssize_t  (*real_recvmsg)(पूर्णाङ्क, संरचना msghdr *, पूर्णाङ्क);
स्थिर ssize_t  (*real_sendmsg)(पूर्णाङ्क, ध्रुव संरचना msghdr *, पूर्णाङ्क);
स्थिर पूर्णाङ्क      (*real_connect)(पूर्णाङ्क, ध्रुव संरचना sockaddr *, socklen_t);
स्थिर पूर्णाङ्क      (*real_accept)(पूर्णाङ्क, संरचना sockaddr *, socklen_t *);
स्थिर पूर्णाङ्क      (*real_pselect)(पूर्णाङ्क, fd_set *, fd_set *, fd_set *,
                                ध्रुव संरचना timespec *, ध्रुव sigset_t *);
स्थिर पूर्णाङ्क      (*real_poll)(संरचना pollfd *, nfds_t, पूर्णाङ्क);
स्थिर पूर्णाङ्क      (*real_socket)(पूर्णाङ्क, पूर्णाङ्क, पूर्णाङ्क);
स्थिर पूर्णाङ्क      (*real_socketpair)(पूर्णाङ्क, पूर्णाङ्क, पूर्णाङ्क, पूर्णाङ्क[2]);
स्थिर पूर्णाङ्क      (*real_pipe)(पूर्णाङ्क[2]);
स्थिर पूर्णाङ्क      (*real_pipe2)(पूर्णाङ्क[2], पूर्णाङ्क);
स्थिर पूर्णाङ्क      (*real_dup)(पूर्णाङ्क);
स्थिर पूर्णाङ्क      (*real_dup2)(पूर्णाङ्क, पूर्णाङ्क);
स्थिर पूर्णाङ्क      (*real_nanosleep)(ध्रुव संरचना timespec *, संरचना timespec *);
स्थिर ssize_t  (*real_read)(पूर्णाङ्क, शून्य *, size_t);
स्थिर ssize_t  (*real_write)(पूर्णाङ्क, ध्रुव शून्य *, size_t);
स्थिर ssize_t  (*real_pread)(पूर्णाङ्क, शून्य *, size_t, off_t);
स्थिर ssize_t  (*real_pwrite)(पूर्णाङ्क, ध्रुव शून्य *, size_t, off_t);
स्थिर पूर्णाङ्क      (*real_close)(पूर्णाङ्क);
स्थिर पूर्णाङ्क      (*real_select)(पूर्णाङ्क, fd_set *, fd_set *, fd_set *,
                                संरचना timeval *);

प्रकारनिर्देश पूर्णाङ्क (*linux_coop_fcntl_fn_t)(पूर्णाङ्क, पूर्णाङ्क, ...);
स्थिर linux_coop_fcntl_fn_t real_fcntl;

प्रकारनिर्देश पूर्णाङ्क (*linux_coop_open_fn_t)(ध्रुव वर्ण *, पूर्णाङ्क, mode_t);
स्थिर linux_coop_open_fn_t real_open;

प्रकारनिर्देश संरचना linux_coop_fd_state {
    पूर्णाङ्क fd;
    पूर्णाङ्क user_flags;
    संरचना linux_coop_fd_state *next;
} linux_coop_fd_state_t;

स्थिर linux_coop_fd_state_t *s_fd_state_list;
स्थिर pthread_mutex_t s_fd_state_lock = PTHREAD_MUTEX_INITIALIZER;

स्थिर linux_coop_fd_state_t *linux_coop_find_fd_locked(पूर्णाङ्क fd)
{
    linux_coop_fd_state_t *ग्रन्थिः = s_fd_state_list;
    याव (ग्रन्थिः != रिक्त) {
        सचे (ग्रन्थिः->fd == fd) {
            पटिनिवत्त ग्रन्थिः;
        }
        ग्रन्थिः = ग्रन्थिः->next;
    }
    पटिनिवत्त रिक्त;
}

स्थिर शून्य __attribute__((constructor)) linux_coop_resolve_all(शून्य)
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
    LINUX_COOP_RESOLVE(पठ);
    LINUX_COOP_RESOLVE(लिख);
    LINUX_COOP_RESOLVE(pread);
    LINUX_COOP_RESOLVE(pwrite);
    LINUX_COOP_RESOLVE(close);
    LINUX_COOP_RESOLVE(select);
    LINUX_COOP_RESOLVE(open);
}

शून्य linux_coop_set_nonblocking(पूर्णाङ्क fd)
{
    सचे (fd >= 0) {
        पूर्णाङ्क flags = real_fcntl(fd, F_GETFL, 0);
        सचे (flags >= 0) {
            real_fcntl(fd, F_SETFL, flags | O_NONBLOCK);
        }
    }
}

शून्य linux_coop_track_fd(पूर्णाङ्क fd, पूर्णाङ्क user_flags)
{
    सचे (fd < 0) {
        पटिनिवत्त;
    }

    pthread_mutex_lock(&s_fd_state_lock);
    linux_coop_fd_state_t *ग्रन्थिः = linux_coop_find_fd_locked(fd);
    सचे (ग्रन्थिः != रिक्त) {
        ग्रन्थिः->user_flags = user_flags;
        pthread_mutex_unlock(&s_fd_state_lock);
        पटिनिवत्त;
    }

    ग्रन्थिः = calloc(1, परिमाण(*ग्रन्थिः));
    सचे (ग्रन्थिः != रिक्त) {
        ग्रन्थिः->fd = fd;
        ग्रन्थिः->user_flags = user_flags;
        ग्रन्थिः->next = s_fd_state_list;
        s_fd_state_list = ग्रन्थिः;
    }
    pthread_mutex_unlock(&s_fd_state_lock);
}

शून्य linux_coop_untrack_fd(पूर्णाङ्क fd)
{
    सचे (fd < 0) {
        पटिनिवत्त;
    }

    pthread_mutex_lock(&s_fd_state_lock);
    linux_coop_fd_state_t *prev = रिक्त;
    linux_coop_fd_state_t *ग्रन्थिः = s_fd_state_list;
    याव (ग्रन्थिः != रिक्त) {
        सचे (ग्रन्थिः->fd == fd) {
            सचे (prev == रिक्त) {
                s_fd_state_list = ग्रन्थिः->next;
            } अञ्ञथा {
                prev->next = ग्रन्थिः->next;
            }
            मुक्त(ग्रन्थिः);
            भङ्ग;
        }
        prev = ग्रन्थिः;
        ग्रन्थिः = ग्रन्थिः->next;
    }
    pthread_mutex_unlock(&s_fd_state_lock);
}

शून्य linux_coop_set_user_flags(पूर्णाङ्क fd, पूर्णाङ्क user_flags)
{
    linux_coop_track_fd(fd, user_flags);
}

bool linux_coop_get_user_flags(पूर्णाङ्क fd, पूर्णाङ्क *flags)
{
    सचे (fd < 0 || flags == रिक्त) {
        पटिनिवत्त false;
    }

    pthread_mutex_lock(&s_fd_state_lock);
    linux_coop_fd_state_t *ग्रन्थिः = linux_coop_find_fd_locked(fd);
    सचे (ग्रन्थिः == रिक्त) {
        pthread_mutex_unlock(&s_fd_state_lock);
        पटिनिवत्त false;
    }
    *flags = ग्रन्थिः->user_flags;
    pthread_mutex_unlock(&s_fd_state_lock);
    पटिनिवत्त true;
}

bool linux_coop_fd_user_nonblocking(पूर्णाङ्क fd)
{
    पूर्णाङ्क user_flags = 0;
    सचे (linux_coop_get_user_flags(fd, &user_flags)) {
        पटिनिवत्त (user_flags & O_NONBLOCK) != 0;
    }
    पटिनिवत्त false;
}


ssize_t freertos_linux_coop_read(पूर्णाङ्क fd, शून्य *buf, size_t गणनम्)
{
    LINUX_COOP_IO_LOOP(fd, real_read(fd, buf, गणनम्))
}

ssize_t freertos_linux_coop_write(पूर्णाङ्क fd, ध्रुव शून्य *buf, size_t गणनम्)
{
    LINUX_COOP_IO_LOOP(fd, real_write(fd, buf, गणनम्))
}

ssize_t freertos_linux_coop_pread(पूर्णाङ्क fd, शून्य *buf, size_t गणनम्, off_t offset)
{
    LINUX_COOP_IO_LOOP(fd, real_pread(fd, buf, गणनम्, offset))
}

ssize_t freertos_linux_coop_pwrite(पूर्णाङ्क fd, ध्रुव शून्य *buf, size_t गणनम्, off_t offset)
{
    LINUX_COOP_IO_LOOP(fd, real_pwrite(fd, buf, गणनम्, offset))
}

पूर्णाङ्क freertos_linux_coop_open(ध्रुव वर्ण *path, पूर्णाङ्क flags, पूर्णाङ्क mode)
{
    पूर्णाङ्क fd = real_open(path, flags, (mode_t)mode);
    linux_coop_set_nonblocking(fd);
    linux_coop_track_fd(fd, flags & O_NONBLOCK);
    पटिनिवत्त fd;
}

पूर्णाङ्क freertos_linux_coop_close(पूर्णाङ्क fd)
{
    पूर्णाङ्क ret = real_close(fd);
    सचे (ret == 0 || errno == EINTR) {
        /* On Linux, close() always releases the fd even when returning
         * EINTR.  Retrying would risk closing a fd reused by another
         * thread.  Treat EINTR as success. */
        linux_coop_untrack_fd(fd);
        पटिनिवत्त 0;
    }
    पटिनिवत्त -1;
}

पूर्णाङ्क freertos_linux_coop_fcntl(पूर्णाङ्क fd, पूर्णाङ्क cmd, पूर्णाङ्क arg)
{
    सचे (cmd == F_GETFL) {
        पूर्णाङ्क real_flags = real_fcntl(fd, F_GETFL, 0);
        सचे (real_flags < 0) {
            पटिनिवत्त -1;
        }
        पूर्णाङ्क user_flags = 0;
        सचे (linux_coop_get_user_flags(fd, &user_flags)) {
            real_flags &= ~O_NONBLOCK;
            real_flags |= (user_flags & O_NONBLOCK);
        }
        पटिनिवत्त real_flags;
    }
    सचे (cmd == F_SETFL) {
        linux_coop_set_user_flags(fd, arg);
        पूर्णाङ्क internal_flags = arg | O_NONBLOCK;
        पटिनिवत्त real_fcntl(fd, F_SETFL, internal_flags);
    }
    पटिनिवत्त real_fcntl(fd, cmd, arg);
}

पूर्णाङ्क freertos_linux_coop_select(पूर्णाङ्क nfds, fd_set *readfds, fd_set *writefds,
                      fd_set *exceptfds, संरचना timeval *timeout)
{
    long timeout_ms = -1;
    सचे (timeout != रिक्त) {
        timeout_ms = (long)timeout->tv_sec * 1000 +
                     (timeout->tv_usec + 999) / 1000;
        सचे (timeout_ms == 0) {
            पटिनिवत्त real_select(nfds, readfds, writefds, exceptfds, timeout);
        }
    }

    /* real_select() mutates the fd_sets in place (clears them, then sets bits
     * for ready descriptors). To keep polling the same descriptors across
     * iterations we must snapshot the caller's sets and restore them on every
     * loop. */
    fd_set rfds_in, wfds_in, efds_in;
    सचे (readfds != रिक्त) {
        rfds_in = *readfds;
    }
    सचे (writefds != रिक्त) {
        wfds_in = *writefds;
    }
    सचे (exceptfds != रिक्त) {
        efds_in = *exceptfds;
    }

    long waited_ms = 0;
    याव (1) {
        सचे (readfds != रिक्त) {
            *readfds = rfds_in;
        }
        सचे (writefds != रिक्त) {
            *writefds = wfds_in;
        }
        सचे (exceptfds != रिक्त) {
            *exceptfds = efds_in;
        }

        संरचना timeval zero_tv = {0, 0};
        पूर्णाङ्क ret = real_select(nfds, readfds, writefds, exceptfds, &zero_tv);
        सचे (ret != 0) {
            पटिनिवत्त ret;
        }
        सचे (timeout_ms == 0) {
            पटिनिवत्त 0;
        }
        सचे (timeout_ms > 0 && waited_ms >= timeout_ms) {
            पटिनिवत्त 0;
        }
        linux_coop_yield(LINUX_COOP_TICK_MS);
        waited_ms += LINUX_COOP_TICK_MS;
    }
}

शून्य freertos_linux_coop_syscalls_init(शून्य)
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

__attribute__((weak)) ssize_t पठ(पूर्णाङ्क fd, शून्य *buf, size_t गणनम्)
{
    पटिनिवत्त freertos_linux_coop_read(fd, buf, गणनम्);
}

__attribute__((weak)) ssize_t लिख(पूर्णाङ्क fd, ध्रुव शून्य *buf, size_t गणनम्)
{
    पटिनिवत्त freertos_linux_coop_write(fd, buf, गणनम्);
}

__attribute__((weak)) ssize_t pread(पूर्णाङ्क fd, शून्य *buf, size_t गणनम्, off_t offset)
{
    पटिनिवत्त freertos_linux_coop_pread(fd, buf, गणनम्, offset);
}

__attribute__((weak)) ssize_t pwrite(पूर्णाङ्क fd, ध्रुव शून्य *buf, size_t गणनम्, off_t offset)
{
    पटिनिवत्त freertos_linux_coop_pwrite(fd, buf, गणनम्, offset);
}

__attribute__((weak)) पूर्णाङ्क open(ध्रुव वर्ण *path, पूर्णाङ्क flags, ...)
{
    पूर्णाङ्क mode = 0;
    सचे (flags & O_CREAT) {
        va_list ap;
        va_start(ap, flags);
        mode = va_arg(ap, पूर्णाङ्क);
        va_end(ap);
    }
    पटिनिवत्त freertos_linux_coop_open(path, flags, mode);
}

__attribute__((weak)) पूर्णाङ्क close(पूर्णाङ्क fd)
{
    पटिनिवत्त freertos_linux_coop_close(fd);
}

__attribute__((weak)) पूर्णाङ्क fcntl(पूर्णाङ्क fd, पूर्णाङ्क cmd, ...)
{
    पूर्णाङ्क arg = 0;
    सचे (cmd != F_GETFL) {
        va_list list;
        va_start(list, cmd);
        arg = va_arg(list, पूर्णाङ्क);
        va_end(list);
    }
    पटिनिवत्त freertos_linux_coop_fcntl(fd, cmd, arg);
}

__attribute__((weak)) पूर्णाङ्क select(पूर्णाङ्क nfds, fd_set *readfds, fd_set *writefds,
                                 fd_set *exceptfds, संरचना timeval *timeout)
{
    पटिनिवत्त freertos_linux_coop_select(nfds, readfds, writefds, exceptfds, timeout);
}

ssize_t freertos_linux_coop_readv(पूर्णाङ्क fd, ध्रुव संरचना iovec *iov, पूर्णाङ्क iovcnt)
{
    LINUX_COOP_IO_LOOP(fd, real_readv(fd, iov, iovcnt))
}

ssize_t freertos_linux_coop_writev(पूर्णाङ्क fd, ध्रुव संरचना iovec *iov, पूर्णाङ्क iovcnt)
{
    LINUX_COOP_IO_LOOP(fd, real_writev(fd, iov, iovcnt))
}

ssize_t freertos_linux_coop_recv(पूर्णाङ्क sockfd, शून्य *buf, size_t len, पूर्णाङ्क flags)
{
    LINUX_COOP_IO_LOOP(sockfd, real_recv(sockfd, buf, len, flags | MSG_DONTWAIT))
}

ssize_t freertos_linux_coop_send(पूर्णाङ्क sockfd, ध्रुव शून्य *buf, size_t len, पूर्णाङ्क flags)
{
    LINUX_COOP_IO_LOOP(sockfd, real_send(sockfd, buf, len, flags | MSG_DONTWAIT))
}

ssize_t freertos_linux_coop_recvfrom(पूर्णाङ्क sockfd, शून्य *buf, size_t len, पूर्णाङ्क flags,
                 संरचना sockaddr *src_addr, socklen_t *addrlen)
{
    LINUX_COOP_IO_LOOP(sockfd, real_recvfrom(sockfd, buf, len, flags | MSG_DONTWAIT,
                            src_addr, addrlen))
}

ssize_t freertos_linux_coop_sendto(पूर्णाङ्क sockfd, ध्रुव शून्य *buf, size_t len, पूर्णाङ्क flags,
               ध्रुव संरचना sockaddr *dest_addr, socklen_t addrlen)
{
    LINUX_COOP_IO_LOOP(sockfd, real_sendto(sockfd, buf, len, flags | MSG_DONTWAIT,
                          dest_addr, addrlen))
}

ssize_t freertos_linux_coop_recvmsg(पूर्णाङ्क sockfd, संरचना msghdr *सन्देशः, पूर्णाङ्क flags)
{
    LINUX_COOP_IO_LOOP(sockfd, real_recvmsg(sockfd, सन्देशः, flags | MSG_DONTWAIT))
}

ssize_t freertos_linux_coop_sendmsg(पूर्णाङ्क sockfd, ध्रुव संरचना msghdr *सन्देशः, पूर्णाङ्क flags)
{
    LINUX_COOP_IO_LOOP(sockfd, real_sendmsg(sockfd, सन्देशः, flags | MSG_DONTWAIT))
}

__attribute__((weak)) ssize_t readv(पूर्णाङ्क fd, ध्रुव संरचना iovec *iov, पूर्णाङ्क iovcnt)
{
    पटिनिवत्त freertos_linux_coop_readv(fd, iov, iovcnt);
}

__attribute__((weak)) ssize_t writev(पूर्णाङ्क fd, ध्रुव संरचना iovec *iov, पूर्णाङ्क iovcnt)
{
    पटिनिवत्त freertos_linux_coop_writev(fd, iov, iovcnt);
}

__attribute__((weak)) ssize_t recv(पूर्णाङ्क sockfd, शून्य *buf, size_t len, पूर्णाङ्क flags)
{
    पटिनिवत्त freertos_linux_coop_recv(sockfd, buf, len, flags);
}

__attribute__((weak)) ssize_t send(पूर्णाङ्क sockfd, ध्रुव शून्य *buf, size_t len, पूर्णाङ्क flags)
{
    पटिनिवत्त freertos_linux_coop_send(sockfd, buf, len, flags);
}

__attribute__((weak)) ssize_t recvfrom(पूर्णाङ्क sockfd, शून्य *buf, size_t len, पूर्णाङ्क flags,
                 संरचना sockaddr *src_addr, socklen_t *addrlen)
{
    पटिनिवत्त freertos_linux_coop_recvfrom(sockfd, buf, len, flags, src_addr, addrlen);
}

__attribute__((weak)) ssize_t sendto(पूर्णाङ्क sockfd, ध्रुव शून्य *buf, size_t len, पूर्णाङ्क flags,
               ध्रुव संरचना sockaddr *dest_addr, socklen_t addrlen)
{
    पटिनिवत्त freertos_linux_coop_sendto(sockfd, buf, len, flags, dest_addr, addrlen);
}

__attribute__((weak)) ssize_t recvmsg(पूर्णाङ्क sockfd, संरचना msghdr *सन्देशः, पूर्णाङ्क flags)
{
    पटिनिवत्त freertos_linux_coop_recvmsg(sockfd, सन्देशः, flags);
}

__attribute__((weak)) ssize_t sendmsg(पूर्णाङ्क sockfd, ध्रुव संरचना msghdr *सन्देशः, पूर्णाङ्क flags)
{
    पटिनिवत्त freertos_linux_coop_sendmsg(sockfd, सन्देशः, flags);
}

पूर्णाङ्क freertos_linux_coop_connect(पूर्णाङ्क sockfd, ध्रुव संरचना sockaddr *addr, socklen_t addrlen)
{
    याव (1)
    {
        पूर्णाङ्क ret = real_connect(sockfd, addr, addrlen);
        सचे (ret == 0) {
            पटिनिवत्त ret;
        }

        सचे (errno == EINPROGRESS || errno == EALREADY) {
            संरचना pollfd pfd;
            pfd.fd = sockfd;
            pfd.events = POLLOUT;
            pfd.revents = 0;

            पूर्णाङ्क poll_retries = 0;
            याव (1)
            {
                पूर्णाङ्क press = real_poll(&pfd, 1, 0);
                सचे (press > 0) {
                    पूर्णाङ्क so_err = 0;
                    socklen_t len = परिमाण(so_err);
                    सचे (getsockopt(sockfd, SOL_SOCKET, SO_ERROR,
                                   &so_err, &len) < 0) {
                        पटिनिवत्त -1;
                    }
                    सचे (so_err == 0) {
                        पटिनिवत्त 0;
                    } अञ्ञथा {
                        errno = so_err;
                        पटिनिवत्त -1;
                    }
                } अञ्ञथा सचे (press == 0) {
                    linux_coop_yield(LINUX_COOP_TICK_MS);
                    अनुवर्त;
                } अञ्ञथा {
                    सचे (errno == EINTR) {
                        अनुवर्त;
                    }
                    /* Non-EINTR poll error; bail out after a few retries */
                    सचे (++poll_retries > 3) {
                        पटिनिवत्त -1;
                    }
                    linux_coop_yield(LINUX_COOP_TICK_MS);
                    अनुवर्त;
                }
            }
        }

        सचे (errno == EINTR) {
            पटिनिवत्त -1;
        }

        पटिनिवत्त -1;
    }
}

पूर्णाङ्क freertos_linux_coop_accept(पूर्णाङ्क sockfd, संरचना sockaddr *addr, socklen_t *addrlen)
{
    याव (1)
    {
        पूर्णाङ्क fd = real_accept(sockfd, addr, addrlen);
        सचे (fd >= 0) {
            linux_coop_set_nonblocking(fd);
            linux_coop_track_fd(fd, linux_coop_fd_user_nonblocking(sockfd) ? O_NONBLOCK : 0);
            पटिनिवत्त fd;
        }
        सचे (errno == EAGAIN || errno == EWOULDBLOCK) {
            सचे (linux_coop_fd_user_nonblocking(sockfd)) {
                पटिनिवत्त -1;
            }
            linux_coop_yield(LINUX_COOP_TICK_MS);
            अनुवर्त;
        }
        पटिनिवत्त -1;
    }
}

पूर्णाङ्क freertos_linux_coop_pselect(पूर्णाङ्क nfds, fd_set *readfds, fd_set *writefds,
            fd_set *exceptfds, ध्रुव संरचना timespec *timeout,
            ध्रुव sigset_t *sigmask)
{
    long timeout_ms = -1;
    सचे (timeout != रिक्त) {
        timeout_ms = (long)timeout->tv_sec * 1000 +
                     (timeout->tv_nsec + 999999) / 1000000;
        सचे (timeout_ms == 0) {
            पटिनिवत्त real_pselect(nfds, readfds, writefds, exceptfds,
                                timeout, sigmask);
        }
    }

    long waited_ms = 0;
    याव (1)
    {
        संरचना timespec zero_ts = {0, 0};
        पूर्णाङ्क ret = real_pselect(nfds, readfds, writefds, exceptfds,
                               &zero_ts, sigmask);
        सचे (ret != 0) {
            पटिनिवत्त ret;
        }

        सचे (timeout_ms == 0 ||
            (timeout_ms > 0 && waited_ms >= timeout_ms)) {
            पटिनिवत्त 0;
        }

        linux_coop_yield(LINUX_COOP_TICK_MS);
        waited_ms += LINUX_COOP_TICK_MS;
    }
}

पूर्णाङ्क freertos_linux_coop_poll(संरचना pollfd *fds, nfds_t nfds, पूर्णाङ्क timeout)
{
    सचे (timeout == 0) {
        पटिनिवत्त real_poll(fds, nfds, 0);
    }

    long timeout_ms = -1;
    सचे (timeout > 0) {
        timeout_ms = timeout;
    }

    long waited_ms = 0;

    याव (1)
    {
        पूर्णाङ्क ret = real_poll(fds, nfds, 0);
        सचे (ret > 0) {
            पटिनिवत्त ret;
        } अञ्ञथा सचे (ret == 0) {
            सचे (timeout_ms == 0 ||
                (timeout_ms > 0 && waited_ms >= timeout_ms)) {
                पटिनिवत्त 0;
            }
            linux_coop_yield(LINUX_COOP_TICK_MS);
            waited_ms += LINUX_COOP_TICK_MS;
            अनुवर्त;
        } अञ्ञथा {
            सचे (errno == EINTR) {
                अनुवर्त;
            }
            linux_coop_yield(LINUX_COOP_TICK_MS);
            सचे (timeout_ms > 0 && waited_ms >= timeout_ms) {
                पटिनिवत्त -1;
            }
            waited_ms += LINUX_COOP_TICK_MS;
        }
    }
}

पूर्णाङ्क nanosleep(ध्रुव संरचना timespec *req, संरचना timespec *rem)
{
    सचे (req == रिक्त) {
        errno = EFAULT;
        पटिनिवत्त -1;
    }
    सचे (!linux_port_in_freertos_task()) {
        पटिनिवत्त real_nanosleep(req, rem);
    }
    long ms = req->tv_sec * 1000 + (req->tv_nsec + 999999) / 1000000;
    सचे (ms <= 0) {
        ms = 1;
    }
    vTaskDelay(ms);
    सचे (rem) {
        rem->tv_sec = 0;
        rem->tv_nsec = 0;
    }
    पटिनिवत्त 0;
}

unsigned पूर्णाङ्क sleep(unsigned पूर्णाङ्क seconds)
{
    linux_coop_yield(seconds * 1000);
    पटिनिवत्त 0;
}

पूर्णाङ्क usleep(useconds_t usec)
{
    linux_coop_yield(usec / 1000);
    पटिनिवत्त 0;
}

पूर्णाङ्क freertos_linux_coop_socket(पूर्णाङ्क domain, पूर्णाङ्क type, पूर्णाङ्क protocol)
{
    पूर्णाङ्क fd = real_socket(domain, type, protocol);
    linux_coop_set_nonblocking(fd);
#ifdef SOCK_NONBLOCK
    ध्रुव पूर्णाङ्क user_flags = (type & SOCK_NONBLOCK) ? O_NONBLOCK : 0;
#अञ्ञथा
    ध्रुव पूर्णाङ्क user_flags = 0;
#endif
    linux_coop_track_fd(fd, user_flags);
    पटिनिवत्त fd;
}

पूर्णाङ्क freertos_linux_coop_socketpair(पूर्णाङ्क domain, पूर्णाङ्क type, पूर्णाङ्क protocol, पूर्णाङ्क *sv)
{
    पूर्णाङ्क ret = real_socketpair(domain, type, protocol, sv);
    सचे (ret == 0) {
        linux_coop_set_nonblocking(sv[0]);
        linux_coop_set_nonblocking(sv[1]);
#ifdef SOCK_NONBLOCK
        ध्रुव पूर्णाङ्क user_flags = (type & SOCK_NONBLOCK) ? O_NONBLOCK : 0;
#अञ्ञथा
        ध्रुव पूर्णाङ्क user_flags = 0;
#endif
        linux_coop_track_fd(sv[0], user_flags);
        linux_coop_track_fd(sv[1], user_flags);
    }
    पटिनिवत्त ret;
}

पूर्णाङ्क freertos_linux_coop_pipe(पूर्णाङ्क *fds)
{
    पूर्णाङ्क ret = real_pipe(fds);
    सचे (ret == 0) {
        linux_coop_set_nonblocking(fds[0]);
        linux_coop_set_nonblocking(fds[1]);
        linux_coop_track_fd(fds[0], 0);
        linux_coop_track_fd(fds[1], 0);
    }
    पटिनिवत्त ret;
}

पूर्णाङ्क freertos_linux_coop_pipe2(पूर्णाङ्क *fds, पूर्णाङ्क flags)
{
    पूर्णाङ्क ret = real_pipe2(fds, flags);
    सचे (ret == 0) {
        linux_coop_set_nonblocking(fds[0]);
        linux_coop_set_nonblocking(fds[1]);
        ध्रुव पूर्णाङ्क user_flags = (flags & O_NONBLOCK) ? O_NONBLOCK : 0;
        linux_coop_track_fd(fds[0], user_flags);
        linux_coop_track_fd(fds[1], user_flags);
    }
    पटिनिवत्त ret;
}

पूर्णाङ्क freertos_linux_coop_dup(पूर्णाङ्क oldfd)
{
    पूर्णाङ्क fd = real_dup(oldfd);
    linux_coop_set_nonblocking(fd);
    linux_coop_track_fd(fd, linux_coop_fd_user_nonblocking(oldfd) ? O_NONBLOCK : 0);
    पटिनिवत्त fd;
}

पूर्णाङ्क freertos_linux_coop_dup2(पूर्णाङ्क oldfd, पूर्णाङ्क newfd)
{
    पूर्णाङ्क fd = real_dup2(oldfd, newfd);
    linux_coop_set_nonblocking(fd);
    linux_coop_track_fd(fd, linux_coop_fd_user_nonblocking(oldfd) ? O_NONBLOCK : 0);
    पटिनिवत्त fd;
}

__attribute__((weak)) पूर्णाङ्क connect(पूर्णाङ्क sockfd, ध्रुव संरचना sockaddr *addr, socklen_t addrlen)
{
    पटिनिवत्त freertos_linux_coop_connect(sockfd, addr, addrlen);
}

__attribute__((weak)) पूर्णाङ्क accept(पूर्णाङ्क sockfd, संरचना sockaddr *addr, socklen_t *addrlen)
{
    पटिनिवत्त freertos_linux_coop_accept(sockfd, addr, addrlen);
}

__attribute__((weak)) पूर्णाङ्क pselect(पूर्णाङ्क nfds, fd_set *readfds, fd_set *writefds,
            fd_set *exceptfds, ध्रुव संरचना timespec *timeout,
            ध्रुव sigset_t *sigmask)
{
    पटिनिवत्त freertos_linux_coop_pselect(nfds, readfds, writefds, exceptfds, timeout, sigmask);
}

__attribute__((weak)) पूर्णाङ्क poll(संरचना pollfd *fds, nfds_t nfds, पूर्णाङ्क timeout)
{
    पटिनिवत्त freertos_linux_coop_poll(fds, nfds, timeout);
}

__attribute__((weak)) पूर्णाङ्क socket(पूर्णाङ्क domain, पूर्णाङ्क type, पूर्णाङ्क protocol)
{
    पटिनिवत्त freertos_linux_coop_socket(domain, type, protocol);
}

__attribute__((weak)) पूर्णाङ्क socketpair(पूर्णाङ्क domain, पूर्णाङ्क type, पूर्णाङ्क protocol, पूर्णाङ्क sv[2])
{
    पटिनिवत्त freertos_linux_coop_socketpair(domain, type, protocol, sv);
}

__attribute__((weak)) पूर्णाङ्क pipe(पूर्णाङ्क fds[2])
{
    पटिनिवत्त freertos_linux_coop_pipe(fds);
}

__attribute__((weak)) पूर्णाङ्क pipe2(पूर्णाङ्क fds[2], पूर्णाङ्क flags)
{
    पटिनिवत्त freertos_linux_coop_pipe2(fds, flags);
}

__attribute__((weak)) पूर्णाङ्क dup(पूर्णाङ्क oldfd)
{
    पटिनिवत्त freertos_linux_coop_dup(oldfd);
}

__attribute__((weak)) पूर्णाङ्क dup2(पूर्णाङ्क oldfd, पूर्णाङ्क newfd)
{
    पटिनिवत्त freertos_linux_coop_dup2(oldfd, newfd);
}
