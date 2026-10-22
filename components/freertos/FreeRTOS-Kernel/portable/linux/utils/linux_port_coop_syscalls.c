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
    linux_coop_fd_state_t *गाँठ = s_fd_state_list;
    यावत् (गाँठ != रिक्त) {
        यदि (गाँठ->fd == fd) {
            प्रत्यागम गाँठ;
        }
        गाँठ = गाँठ->next;
    }
    प्रत्यागम रिक्त;
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
    यदि (fd >= 0) {
        पूर्णाङ्क flags = real_fcntl(fd, F_GETFL, 0);
        यदि (flags >= 0) {
            real_fcntl(fd, F_SETFL, flags | O_NONBLOCK);
        }
    }
}

शून्य linux_coop_track_fd(पूर्णाङ्क fd, पूर्णाङ्क user_flags)
{
    यदि (fd < 0) {
        प्रत्यागम;
    }

    pthread_mutex_lock(&s_fd_state_lock);
    linux_coop_fd_state_t *गाँठ = linux_coop_find_fd_locked(fd);
    यदि (गाँठ != रिक्त) {
        गाँठ->user_flags = user_flags;
        pthread_mutex_unlock(&s_fd_state_lock);
        प्रत्यागम;
    }

    गाँठ = calloc(1, परिमाण(*गाँठ));
    यदि (गाँठ != रिक्त) {
        गाँठ->fd = fd;
        गाँठ->user_flags = user_flags;
        गाँठ->next = s_fd_state_list;
        s_fd_state_list = गाँठ;
    }
    pthread_mutex_unlock(&s_fd_state_lock);
}

शून्य linux_coop_untrack_fd(पूर्णाङ्क fd)
{
    यदि (fd < 0) {
        प्रत्यागम;
    }

    pthread_mutex_lock(&s_fd_state_lock);
    linux_coop_fd_state_t *prev = रिक्त;
    linux_coop_fd_state_t *गाँठ = s_fd_state_list;
    यावत् (गाँठ != रिक्त) {
        यदि (गाँठ->fd == fd) {
            यदि (prev == रिक्त) {
                s_fd_state_list = गाँठ->next;
            } अन्यथा {
                prev->next = गाँठ->next;
            }
            मुक्त(गाँठ);
            भङ्ग;
        }
        prev = गाँठ;
        गाँठ = गाँठ->next;
    }
    pthread_mutex_unlock(&s_fd_state_lock);
}

शून्य linux_coop_set_user_flags(पूर्णाङ्क fd, पूर्णाङ्क user_flags)
{
    linux_coop_track_fd(fd, user_flags);
}

bool linux_coop_get_user_flags(पूर्णाङ्क fd, पूर्णाङ्क *flags)
{
    यदि (fd < 0 || flags == रिक्त) {
        प्रत्यागम false;
    }

    pthread_mutex_lock(&s_fd_state_lock);
    linux_coop_fd_state_t *गाँठ = linux_coop_find_fd_locked(fd);
    यदि (गाँठ == रिक्त) {
        pthread_mutex_unlock(&s_fd_state_lock);
        प्रत्यागम false;
    }
    *flags = गाँठ->user_flags;
    pthread_mutex_unlock(&s_fd_state_lock);
    प्रत्यागम true;
}

bool linux_coop_fd_user_nonblocking(पूर्णाङ्क fd)
{
    पूर्णाङ्क user_flags = 0;
    यदि (linux_coop_get_user_flags(fd, &user_flags)) {
        प्रत्यागम (user_flags & O_NONBLOCK) != 0;
    }
    प्रत्यागम false;
}


ssize_t freertos_linux_coop_read(पूर्णाङ्क fd, शून्य *buf, size_t गिनती)
{
    LINUX_COOP_IO_LOOP(fd, real_read(fd, buf, गिनती))
}

ssize_t freertos_linux_coop_write(पूर्णाङ्क fd, ध्रुव शून्य *buf, size_t गिनती)
{
    LINUX_COOP_IO_LOOP(fd, real_write(fd, buf, गिनती))
}

ssize_t freertos_linux_coop_pread(पूर्णाङ्क fd, शून्य *buf, size_t गिनती, off_t offset)
{
    LINUX_COOP_IO_LOOP(fd, real_pread(fd, buf, गिनती, offset))
}

ssize_t freertos_linux_coop_pwrite(पूर्णाङ्क fd, ध्रुव शून्य *buf, size_t गिनती, off_t offset)
{
    LINUX_COOP_IO_LOOP(fd, real_pwrite(fd, buf, गिनती, offset))
}

पूर्णाङ्क freertos_linux_coop_open(ध्रुव वर्ण *path, पूर्णाङ्क flags, पूर्णाङ्क mode)
{
    पूर्णाङ्क fd = real_open(path, flags, (mode_t)mode);
    linux_coop_set_nonblocking(fd);
    linux_coop_track_fd(fd, flags & O_NONBLOCK);
    प्रत्यागम fd;
}

पूर्णाङ्क freertos_linux_coop_close(पूर्णाङ्क fd)
{
    पूर्णाङ्क ret = real_close(fd);
    यदि (ret == 0 || errno == EINTR) {
        /* On Linux, close() always releases the fd even when returning
         * EINTR.  Retrying would risk closing a fd reused by another
         * thread.  Treat EINTR as success. */
        linux_coop_untrack_fd(fd);
        प्रत्यागम 0;
    }
    प्रत्यागम -1;
}

पूर्णाङ्क freertos_linux_coop_fcntl(पूर्णाङ्क fd, पूर्णाङ्क cmd, पूर्णाङ्क arg)
{
    यदि (cmd == F_GETFL) {
        पूर्णाङ्क real_flags = real_fcntl(fd, F_GETFL, 0);
        यदि (real_flags < 0) {
            प्रत्यागम -1;
        }
        पूर्णाङ्क user_flags = 0;
        यदि (linux_coop_get_user_flags(fd, &user_flags)) {
            real_flags &= ~O_NONBLOCK;
            real_flags |= (user_flags & O_NONBLOCK);
        }
        प्रत्यागम real_flags;
    }
    यदि (cmd == F_SETFL) {
        linux_coop_set_user_flags(fd, arg);
        पूर्णाङ्क internal_flags = arg | O_NONBLOCK;
        प्रत्यागम real_fcntl(fd, F_SETFL, internal_flags);
    }
    प्रत्यागम real_fcntl(fd, cmd, arg);
}

पूर्णाङ्क freertos_linux_coop_select(पूर्णाङ्क nfds, fd_set *readfds, fd_set *writefds,
                      fd_set *exceptfds, संरचना timeval *timeout)
{
    long timeout_ms = -1;
    यदि (timeout != रिक्त) {
        timeout_ms = (long)timeout->tv_sec * 1000 +
                     (timeout->tv_usec + 999) / 1000;
        यदि (timeout_ms == 0) {
            प्रत्यागम real_select(nfds, readfds, writefds, exceptfds, timeout);
        }
    }

    /* real_select() mutates the fd_sets in place (clears them, then sets bits
     * for ready descriptors). To keep polling the same descriptors across
     * iterations we must snapshot the caller's sets and restore them on every
     * loop. */
    fd_set rfds_in, wfds_in, efds_in;
    यदि (readfds != रिक्त) {
        rfds_in = *readfds;
    }
    यदि (writefds != रिक्त) {
        wfds_in = *writefds;
    }
    यदि (exceptfds != रिक्त) {
        efds_in = *exceptfds;
    }

    long waited_ms = 0;
    यावत् (1) {
        यदि (readfds != रिक्त) {
            *readfds = rfds_in;
        }
        यदि (writefds != रिक्त) {
            *writefds = wfds_in;
        }
        यदि (exceptfds != रिक्त) {
            *exceptfds = efds_in;
        }

        संरचना timeval zero_tv = {0, 0};
        पूर्णाङ्क ret = real_select(nfds, readfds, writefds, exceptfds, &zero_tv);
        यदि (ret != 0) {
            प्रत्यागम ret;
        }
        यदि (timeout_ms == 0) {
            प्रत्यागम 0;
        }
        यदि (timeout_ms > 0 && waited_ms >= timeout_ms) {
            प्रत्यागम 0;
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

__attribute__((weak)) ssize_t पठ(पूर्णाङ्क fd, शून्य *buf, size_t गिनती)
{
    प्रत्यागम freertos_linux_coop_read(fd, buf, गिनती);
}

__attribute__((weak)) ssize_t लिख(पूर्णाङ्क fd, ध्रुव शून्य *buf, size_t गिनती)
{
    प्रत्यागम freertos_linux_coop_write(fd, buf, गिनती);
}

__attribute__((weak)) ssize_t pread(पूर्णाङ्क fd, शून्य *buf, size_t गिनती, off_t offset)
{
    प्रत्यागम freertos_linux_coop_pread(fd, buf, गिनती, offset);
}

__attribute__((weak)) ssize_t pwrite(पूर्णाङ्क fd, ध्रुव शून्य *buf, size_t गिनती, off_t offset)
{
    प्रत्यागम freertos_linux_coop_pwrite(fd, buf, गिनती, offset);
}

__attribute__((weak)) पूर्णाङ्क open(ध्रुव वर्ण *path, पूर्णाङ्क flags, ...)
{
    पूर्णाङ्क mode = 0;
    यदि (flags & O_CREAT) {
        va_list ap;
        va_start(ap, flags);
        mode = va_arg(ap, पूर्णाङ्क);
        va_end(ap);
    }
    प्रत्यागम freertos_linux_coop_open(path, flags, mode);
}

__attribute__((weak)) पूर्णाङ्क close(पूर्णाङ्क fd)
{
    प्रत्यागम freertos_linux_coop_close(fd);
}

__attribute__((weak)) पूर्णाङ्क fcntl(पूर्णाङ्क fd, पूर्णाङ्क cmd, ...)
{
    पूर्णाङ्क arg = 0;
    यदि (cmd != F_GETFL) {
        va_list list;
        va_start(list, cmd);
        arg = va_arg(list, पूर्णाङ्क);
        va_end(list);
    }
    प्रत्यागम freertos_linux_coop_fcntl(fd, cmd, arg);
}

__attribute__((weak)) पूर्णाङ्क select(पूर्णाङ्क nfds, fd_set *readfds, fd_set *writefds,
                                 fd_set *exceptfds, संरचना timeval *timeout)
{
    प्रत्यागम freertos_linux_coop_select(nfds, readfds, writefds, exceptfds, timeout);
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

ssize_t freertos_linux_coop_recvmsg(पूर्णाङ्क sockfd, संरचना msghdr *संदेश, पूर्णाङ्क flags)
{
    LINUX_COOP_IO_LOOP(sockfd, real_recvmsg(sockfd, संदेश, flags | MSG_DONTWAIT))
}

ssize_t freertos_linux_coop_sendmsg(पूर्णाङ्क sockfd, ध्रुव संरचना msghdr *संदेश, पूर्णाङ्क flags)
{
    LINUX_COOP_IO_LOOP(sockfd, real_sendmsg(sockfd, संदेश, flags | MSG_DONTWAIT))
}

__attribute__((weak)) ssize_t readv(पूर्णाङ्क fd, ध्रुव संरचना iovec *iov, पूर्णाङ्क iovcnt)
{
    प्रत्यागम freertos_linux_coop_readv(fd, iov, iovcnt);
}

__attribute__((weak)) ssize_t writev(पूर्णाङ्क fd, ध्रुव संरचना iovec *iov, पूर्णाङ्क iovcnt)
{
    प्रत्यागम freertos_linux_coop_writev(fd, iov, iovcnt);
}

__attribute__((weak)) ssize_t recv(पूर्णाङ्क sockfd, शून्य *buf, size_t len, पूर्णाङ्क flags)
{
    प्रत्यागम freertos_linux_coop_recv(sockfd, buf, len, flags);
}

__attribute__((weak)) ssize_t send(पूर्णाङ्क sockfd, ध्रुव शून्य *buf, size_t len, पूर्णाङ्क flags)
{
    प्रत्यागम freertos_linux_coop_send(sockfd, buf, len, flags);
}

__attribute__((weak)) ssize_t recvfrom(पूर्णाङ्क sockfd, शून्य *buf, size_t len, पूर्णाङ्क flags,
                 संरचना sockaddr *src_addr, socklen_t *addrlen)
{
    प्रत्यागम freertos_linux_coop_recvfrom(sockfd, buf, len, flags, src_addr, addrlen);
}

__attribute__((weak)) ssize_t sendto(पूर्णाङ्क sockfd, ध्रुव शून्य *buf, size_t len, पूर्णाङ्क flags,
               ध्रुव संरचना sockaddr *dest_addr, socklen_t addrlen)
{
    प्रत्यागम freertos_linux_coop_sendto(sockfd, buf, len, flags, dest_addr, addrlen);
}

__attribute__((weak)) ssize_t recvmsg(पूर्णाङ्क sockfd, संरचना msghdr *संदेश, पूर्णाङ्क flags)
{
    प्रत्यागम freertos_linux_coop_recvmsg(sockfd, संदेश, flags);
}

__attribute__((weak)) ssize_t sendmsg(पूर्णाङ्क sockfd, ध्रुव संरचना msghdr *संदेश, पूर्णाङ्क flags)
{
    प्रत्यागम freertos_linux_coop_sendmsg(sockfd, संदेश, flags);
}

पूर्णाङ्क freertos_linux_coop_connect(पूर्णाङ्क sockfd, ध्रुव संरचना sockaddr *addr, socklen_t addrlen)
{
    यावत् (1)
    {
        पूर्णाङ्क ret = real_connect(sockfd, addr, addrlen);
        यदि (ret == 0) {
            प्रत्यागम ret;
        }

        यदि (errno == EINPROGRESS || errno == EALREADY) {
            संरचना pollfd pfd;
            pfd.fd = sockfd;
            pfd.events = POLLOUT;
            pfd.revents = 0;

            पूर्णाङ्क poll_retries = 0;
            यावत् (1)
            {
                पूर्णाङ्क press = real_poll(&pfd, 1, 0);
                यदि (press > 0) {
                    पूर्णाङ्क so_err = 0;
                    socklen_t len = परिमाण(so_err);
                    यदि (getsockopt(sockfd, SOL_SOCKET, SO_ERROR,
                                   &so_err, &len) < 0) {
                        प्रत्यागम -1;
                    }
                    यदि (so_err == 0) {
                        प्रत्यागम 0;
                    } अन्यथा {
                        errno = so_err;
                        प्रत्यागम -1;
                    }
                } अन्यथा यदि (press == 0) {
                    linux_coop_yield(LINUX_COOP_TICK_MS);
                    अनुवर्त;
                } अन्यथा {
                    यदि (errno == EINTR) {
                        अनुवर्त;
                    }
                    /* Non-EINTR poll error; bail out after a few retries */
                    यदि (++poll_retries > 3) {
                        प्रत्यागम -1;
                    }
                    linux_coop_yield(LINUX_COOP_TICK_MS);
                    अनुवर्त;
                }
            }
        }

        यदि (errno == EINTR) {
            प्रत्यागम -1;
        }

        प्रत्यागम -1;
    }
}

पूर्णाङ्क freertos_linux_coop_accept(पूर्णाङ्क sockfd, संरचना sockaddr *addr, socklen_t *addrlen)
{
    यावत् (1)
    {
        पूर्णाङ्क fd = real_accept(sockfd, addr, addrlen);
        यदि (fd >= 0) {
            linux_coop_set_nonblocking(fd);
            linux_coop_track_fd(fd, linux_coop_fd_user_nonblocking(sockfd) ? O_NONBLOCK : 0);
            प्रत्यागम fd;
        }
        यदि (errno == EAGAIN || errno == EWOULDBLOCK) {
            यदि (linux_coop_fd_user_nonblocking(sockfd)) {
                प्रत्यागम -1;
            }
            linux_coop_yield(LINUX_COOP_TICK_MS);
            अनुवर्त;
        }
        प्रत्यागम -1;
    }
}

पूर्णाङ्क freertos_linux_coop_pselect(पूर्णाङ्क nfds, fd_set *readfds, fd_set *writefds,
            fd_set *exceptfds, ध्रुव संरचना timespec *timeout,
            ध्रुव sigset_t *sigmask)
{
    long timeout_ms = -1;
    यदि (timeout != रिक्त) {
        timeout_ms = (long)timeout->tv_sec * 1000 +
                     (timeout->tv_nsec + 999999) / 1000000;
        यदि (timeout_ms == 0) {
            प्रत्यागम real_pselect(nfds, readfds, writefds, exceptfds,
                                timeout, sigmask);
        }
    }

    long waited_ms = 0;
    यावत् (1)
    {
        संरचना timespec zero_ts = {0, 0};
        पूर्णाङ्क ret = real_pselect(nfds, readfds, writefds, exceptfds,
                               &zero_ts, sigmask);
        यदि (ret != 0) {
            प्रत्यागम ret;
        }

        यदि (timeout_ms == 0 ||
            (timeout_ms > 0 && waited_ms >= timeout_ms)) {
            प्रत्यागम 0;
        }

        linux_coop_yield(LINUX_COOP_TICK_MS);
        waited_ms += LINUX_COOP_TICK_MS;
    }
}

पूर्णाङ्क freertos_linux_coop_poll(संरचना pollfd *fds, nfds_t nfds, पूर्णाङ्क timeout)
{
    यदि (timeout == 0) {
        प्रत्यागम real_poll(fds, nfds, 0);
    }

    long timeout_ms = -1;
    यदि (timeout > 0) {
        timeout_ms = timeout;
    }

    long waited_ms = 0;

    यावत् (1)
    {
        पूर्णाङ्क ret = real_poll(fds, nfds, 0);
        यदि (ret > 0) {
            प्रत्यागम ret;
        } अन्यथा यदि (ret == 0) {
            यदि (timeout_ms == 0 ||
                (timeout_ms > 0 && waited_ms >= timeout_ms)) {
                प्रत्यागम 0;
            }
            linux_coop_yield(LINUX_COOP_TICK_MS);
            waited_ms += LINUX_COOP_TICK_MS;
            अनुवर्त;
        } अन्यथा {
            यदि (errno == EINTR) {
                अनुवर्त;
            }
            linux_coop_yield(LINUX_COOP_TICK_MS);
            यदि (timeout_ms > 0 && waited_ms >= timeout_ms) {
                प्रत्यागम -1;
            }
            waited_ms += LINUX_COOP_TICK_MS;
        }
    }
}

पूर्णाङ्क nanosleep(ध्रुव संरचना timespec *req, संरचना timespec *rem)
{
    यदि (req == रिक्त) {
        errno = EFAULT;
        प्रत्यागम -1;
    }
    यदि (!linux_port_in_freertos_task()) {
        प्रत्यागम real_nanosleep(req, rem);
    }
    long ms = req->tv_sec * 1000 + (req->tv_nsec + 999999) / 1000000;
    यदि (ms <= 0) {
        ms = 1;
    }
    vTaskDelay(ms);
    यदि (rem) {
        rem->tv_sec = 0;
        rem->tv_nsec = 0;
    }
    प्रत्यागम 0;
}

unsigned पूर्णाङ्क sleep(unsigned पूर्णाङ्क seconds)
{
    linux_coop_yield(seconds * 1000);
    प्रत्यागम 0;
}

पूर्णाङ्क usleep(useconds_t usec)
{
    linux_coop_yield(usec / 1000);
    प्रत्यागम 0;
}

पूर्णाङ्क freertos_linux_coop_socket(पूर्णाङ्क domain, पूर्णाङ्क type, पूर्णाङ्क protocol)
{
    पूर्णाङ्क fd = real_socket(domain, type, protocol);
    linux_coop_set_nonblocking(fd);
#ifdef SOCK_NONBLOCK
    ध्रुव पूर्णाङ्क user_flags = (type & SOCK_NONBLOCK) ? O_NONBLOCK : 0;
#अन्यथा
    ध्रुव पूर्णाङ्क user_flags = 0;
#endif
    linux_coop_track_fd(fd, user_flags);
    प्रत्यागम fd;
}

पूर्णाङ्क freertos_linux_coop_socketpair(पूर्णाङ्क domain, पूर्णाङ्क type, पूर्णाङ्क protocol, पूर्णाङ्क *sv)
{
    पूर्णाङ्क ret = real_socketpair(domain, type, protocol, sv);
    यदि (ret == 0) {
        linux_coop_set_nonblocking(sv[0]);
        linux_coop_set_nonblocking(sv[1]);
#ifdef SOCK_NONBLOCK
        ध्रुव पूर्णाङ्क user_flags = (type & SOCK_NONBLOCK) ? O_NONBLOCK : 0;
#अन्यथा
        ध्रुव पूर्णाङ्क user_flags = 0;
#endif
        linux_coop_track_fd(sv[0], user_flags);
        linux_coop_track_fd(sv[1], user_flags);
    }
    प्रत्यागम ret;
}

पूर्णाङ्क freertos_linux_coop_pipe(पूर्णाङ्क *fds)
{
    पूर्णाङ्क ret = real_pipe(fds);
    यदि (ret == 0) {
        linux_coop_set_nonblocking(fds[0]);
        linux_coop_set_nonblocking(fds[1]);
        linux_coop_track_fd(fds[0], 0);
        linux_coop_track_fd(fds[1], 0);
    }
    प्रत्यागम ret;
}

पूर्णाङ्क freertos_linux_coop_pipe2(पूर्णाङ्क *fds, पूर्णाङ्क flags)
{
    पूर्णाङ्क ret = real_pipe2(fds, flags);
    यदि (ret == 0) {
        linux_coop_set_nonblocking(fds[0]);
        linux_coop_set_nonblocking(fds[1]);
        ध्रुव पूर्णाङ्क user_flags = (flags & O_NONBLOCK) ? O_NONBLOCK : 0;
        linux_coop_track_fd(fds[0], user_flags);
        linux_coop_track_fd(fds[1], user_flags);
    }
    प्रत्यागम ret;
}

पूर्णाङ्क freertos_linux_coop_dup(पूर्णाङ्क oldfd)
{
    पूर्णाङ्क fd = real_dup(oldfd);
    linux_coop_set_nonblocking(fd);
    linux_coop_track_fd(fd, linux_coop_fd_user_nonblocking(oldfd) ? O_NONBLOCK : 0);
    प्रत्यागम fd;
}

पूर्णाङ्क freertos_linux_coop_dup2(पूर्णाङ्क oldfd, पूर्णाङ्क newfd)
{
    पूर्णाङ्क fd = real_dup2(oldfd, newfd);
    linux_coop_set_nonblocking(fd);
    linux_coop_track_fd(fd, linux_coop_fd_user_nonblocking(oldfd) ? O_NONBLOCK : 0);
    प्रत्यागम fd;
}

__attribute__((weak)) पूर्णाङ्क connect(पूर्णाङ्क sockfd, ध्रुव संरचना sockaddr *addr, socklen_t addrlen)
{
    प्रत्यागम freertos_linux_coop_connect(sockfd, addr, addrlen);
}

__attribute__((weak)) पूर्णाङ्क accept(पूर्णाङ्क sockfd, संरचना sockaddr *addr, socklen_t *addrlen)
{
    प्रत्यागम freertos_linux_coop_accept(sockfd, addr, addrlen);
}

__attribute__((weak)) पूर्णाङ्क pselect(पूर्णाङ्क nfds, fd_set *readfds, fd_set *writefds,
            fd_set *exceptfds, ध्रुव संरचना timespec *timeout,
            ध्रुव sigset_t *sigmask)
{
    प्रत्यागम freertos_linux_coop_pselect(nfds, readfds, writefds, exceptfds, timeout, sigmask);
}

__attribute__((weak)) पूर्णाङ्क poll(संरचना pollfd *fds, nfds_t nfds, पूर्णाङ्क timeout)
{
    प्रत्यागम freertos_linux_coop_poll(fds, nfds, timeout);
}

__attribute__((weak)) पूर्णाङ्क socket(पूर्णाङ्क domain, पूर्णाङ्क type, पूर्णाङ्क protocol)
{
    प्रत्यागम freertos_linux_coop_socket(domain, type, protocol);
}

__attribute__((weak)) पूर्णाङ्क socketpair(पूर्णाङ्क domain, पूर्णाङ्क type, पूर्णाङ्क protocol, पूर्णाङ्क sv[2])
{
    प्रत्यागम freertos_linux_coop_socketpair(domain, type, protocol, sv);
}

__attribute__((weak)) पूर्णाङ्क pipe(पूर्णाङ्क fds[2])
{
    प्रत्यागम freertos_linux_coop_pipe(fds);
}

__attribute__((weak)) पूर्णाङ्क pipe2(पूर्णाङ्क fds[2], पूर्णाङ्क flags)
{
    प्रत्यागम freertos_linux_coop_pipe2(fds, flags);
}

__attribute__((weak)) पूर्णाङ्क dup(पूर्णाङ्क oldfd)
{
    प्रत्यागम freertos_linux_coop_dup(oldfd);
}

__attribute__((weak)) पूर्णाङ्क dup2(पूर्णाङ्क oldfd, पूर्णाङ्क newfd)
{
    प्रत्यागम freertos_linux_coop_dup2(oldfd, newfd);
}
