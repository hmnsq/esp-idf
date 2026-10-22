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

स्थिर ssize_t  (*real_readv)(पूर्णांक, स्थिरांक संरचना iovec *, पूर्णांक);
स्थिर ssize_t  (*real_writev)(पूर्णांक, स्थिरांक संरचना iovec *, पूर्णांक);
स्थिर ssize_t  (*real_recv)(पूर्णांक, शून्य *, माप_प्रकार, पूर्णांक);
स्थिर ssize_t  (*real_send)(पूर्णांक, स्थिरांक शून्य *, माप_प्रकार, पूर्णांक);
स्थिर ssize_t  (*real_recvfrom)(पूर्णांक, शून्य *, माप_प्रकार, पूर्णांक,
                                 संरचना sockaddr *, socklen_t *);
स्थिर ssize_t  (*real_sendto)(पूर्णांक, स्थिरांक शून्य *, माप_प्रकार, पूर्णांक,
                                स्थिरांक संरचना sockaddr *, socklen_t);
स्थिर ssize_t  (*real_recvmsg)(पूर्णांक, संरचना msghdr *, पूर्णांक);
स्थिर ssize_t  (*real_sendmsg)(पूर्णांक, स्थिरांक संरचना msghdr *, पूर्णांक);
स्थिर पूर्णांक      (*real_connect)(पूर्णांक, स्थिरांक संरचना sockaddr *, socklen_t);
स्थिर पूर्णांक      (*real_accept)(पूर्णांक, संरचना sockaddr *, socklen_t *);
स्थिर पूर्णांक      (*real_pselect)(पूर्णांक, fd_set *, fd_set *, fd_set *,
                                स्थिरांक संरचना timespec *, स्थिरांक sigset_t *);
स्थिर पूर्णांक      (*real_poll)(संरचना pollfd *, nfds_t, पूर्णांक);
स्थिर पूर्णांक      (*real_socket)(पूर्णांक, पूर्णांक, पूर्णांक);
स्थिर पूर्णांक      (*real_socketpair)(पूर्णांक, पूर्णांक, पूर्णांक, पूर्णांक[2]);
स्थिर पूर्णांक      (*real_pipe)(पूर्णांक[2]);
स्थिर पूर्णांक      (*real_pipe2)(पूर्णांक[2], पूर्णांक);
स्थिर पूर्णांक      (*real_dup)(पूर्णांक);
स्थिर पूर्णांक      (*real_dup2)(पूर्णांक, पूर्णांक);
स्थिर पूर्णांक      (*real_nanosleep)(स्थिरांक संरचना timespec *, संरचना timespec *);
स्थिर ssize_t  (*real_read)(पूर्णांक, शून्य *, माप_प्रकार);
स्थिर ssize_t  (*real_write)(पूर्णांक, स्थिरांक शून्य *, माप_प्रकार);
स्थिर ssize_t  (*real_pread)(पूर्णांक, शून्य *, माप_प्रकार, off_t);
स्थिर ssize_t  (*real_pwrite)(पूर्णांक, स्थिरांक शून्य *, माप_प्रकार, off_t);
स्थिर पूर्णांक      (*real_close)(पूर्णांक);
स्थिर पूर्णांक      (*real_select)(पूर्णांक, fd_set *, fd_set *, fd_set *,
                                संरचना timeval *);

प्रकारव्याख्या पूर्णांक (*linux_coop_fcntl_fn_t)(पूर्णांक, पूर्णांक, ...);
स्थिर linux_coop_fcntl_fn_t real_fcntl;

प्रकारव्याख्या पूर्णांक (*linux_coop_open_fn_t)(स्थिरांक अक्षर *, पूर्णांक, mode_t);
स्थिर linux_coop_open_fn_t real_open;

प्रकारव्याख्या संरचना linux_coop_fd_state {
    पूर्णांक fd;
    पूर्णांक user_flags;
    संरचना linux_coop_fd_state *next;
} linux_coop_fd_state_t;

स्थिर linux_coop_fd_state_t *s_fd_state_list;
स्थिर pthread_mutex_t s_fd_state_lock = PTHREAD_MUTEX_INITIALIZER;

स्थिर linux_coop_fd_state_t *linux_coop_find_fd_locked(पूर्णांक fd)
{
    linux_coop_fd_state_t *गाठ = s_fd_state_list;
    पर्यंत (गाठ != रिक्त) {
        जर (गाठ->fd == fd) {
            परत गाठ;
        }
        गाठ = गाठ->next;
    }
    परत रिक्त;
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
    LINUX_COOP_RESOLVE(बंद);
    LINUX_COOP_RESOLVE(select);
    LINUX_COOP_RESOLVE(खोलो);
}

शून्य linux_coop_set_nonblocking(पूर्णांक fd)
{
    जर (fd >= 0) {
        पूर्णांक flags = real_fcntl(fd, F_GETFL, 0);
        जर (flags >= 0) {
            real_fcntl(fd, F_SETFL, flags | O_NONBLOCK);
        }
    }
}

शून्य linux_coop_track_fd(पूर्णांक fd, पूर्णांक user_flags)
{
    जर (fd < 0) {
        परत;
    }

    pthread_mutex_lock(&s_fd_state_lock);
    linux_coop_fd_state_t *गाठ = linux_coop_find_fd_locked(fd);
    जर (गाठ != रिक्त) {
        गाठ->user_flags = user_flags;
        pthread_mutex_unlock(&s_fd_state_lock);
        परत;
    }

    गाठ = सुस्मृति(1, आकार(*गाठ));
    जर (गाठ != रिक्त) {
        गाठ->fd = fd;
        गाठ->user_flags = user_flags;
        गाठ->next = s_fd_state_list;
        s_fd_state_list = गाठ;
    }
    pthread_mutex_unlock(&s_fd_state_lock);
}

शून्य linux_coop_untrack_fd(पूर्णांक fd)
{
    जर (fd < 0) {
        परत;
    }

    pthread_mutex_lock(&s_fd_state_lock);
    linux_coop_fd_state_t *prev = रिक्त;
    linux_coop_fd_state_t *गाठ = s_fd_state_list;
    पर्यंत (गाठ != रिक्त) {
        जर (गाठ->fd == fd) {
            जर (prev == रिक्त) {
                s_fd_state_list = गाठ->next;
            } अन्यथा {
                prev->next = गाठ->next;
            }
            मुक्त(गाठ);
            थांबा;
        }
        prev = गाठ;
        गाठ = गाठ->next;
    }
    pthread_mutex_unlock(&s_fd_state_lock);
}

शून्य linux_coop_set_user_flags(पूर्णांक fd, पूर्णांक user_flags)
{
    linux_coop_track_fd(fd, user_flags);
}

bool linux_coop_get_user_flags(पूर्णांक fd, पूर्णांक *flags)
{
    जर (fd < 0 || flags == रिक्त) {
        परत false;
    }

    pthread_mutex_lock(&s_fd_state_lock);
    linux_coop_fd_state_t *गाठ = linux_coop_find_fd_locked(fd);
    जर (गाठ == रिक्त) {
        pthread_mutex_unlock(&s_fd_state_lock);
        परत false;
    }
    *flags = गाठ->user_flags;
    pthread_mutex_unlock(&s_fd_state_lock);
    परत true;
}

bool linux_coop_fd_user_nonblocking(पूर्णांक fd)
{
    पूर्णांक user_flags = 0;
    जर (linux_coop_get_user_flags(fd, &user_flags)) {
        परत (user_flags & O_NONBLOCK) != 0;
    }
    परत false;
}


ssize_t freertos_linux_coop_read(पूर्णांक fd, शून्य *buf, माप_प्रकार मोजणी)
{
    LINUX_COOP_IO_LOOP(fd, real_read(fd, buf, मोजणी))
}

ssize_t freertos_linux_coop_write(पूर्णांक fd, स्थिरांक शून्य *buf, माप_प्रकार मोजणी)
{
    LINUX_COOP_IO_LOOP(fd, real_write(fd, buf, मोजणी))
}

ssize_t freertos_linux_coop_pread(पूर्णांक fd, शून्य *buf, माप_प्रकार मोजणी, off_t offset)
{
    LINUX_COOP_IO_LOOP(fd, real_pread(fd, buf, मोजणी, offset))
}

ssize_t freertos_linux_coop_pwrite(पूर्णांक fd, स्थिरांक शून्य *buf, माप_प्रकार मोजणी, off_t offset)
{
    LINUX_COOP_IO_LOOP(fd, real_pwrite(fd, buf, मोजणी, offset))
}

पूर्णांक freertos_linux_coop_open(स्थिरांक अक्षर *path, पूर्णांक flags, पूर्णांक mode)
{
    पूर्णांक fd = real_open(path, flags, (mode_t)mode);
    linux_coop_set_nonblocking(fd);
    linux_coop_track_fd(fd, flags & O_NONBLOCK);
    परत fd;
}

पूर्णांक freertos_linux_coop_close(पूर्णांक fd)
{
    पूर्णांक ret = real_close(fd);
    जर (ret == 0 || त्रुटि_सं == EINTR) {
        /* On Linux, close() always releases the fd even when returning
         * EINTR.  Retrying would risk closing a fd reused by another
         * thread.  Treat EINTR as success. */
        linux_coop_untrack_fd(fd);
        परत 0;
    }
    परत -1;
}

पूर्णांक freertos_linux_coop_fcntl(पूर्णांक fd, पूर्णांक cmd, पूर्णांक arg)
{
    जर (cmd == F_GETFL) {
        पूर्णांक real_flags = real_fcntl(fd, F_GETFL, 0);
        जर (real_flags < 0) {
            परत -1;
        }
        पूर्णांक user_flags = 0;
        जर (linux_coop_get_user_flags(fd, &user_flags)) {
            real_flags &= ~O_NONBLOCK;
            real_flags |= (user_flags & O_NONBLOCK);
        }
        परत real_flags;
    }
    जर (cmd == F_SETFL) {
        linux_coop_set_user_flags(fd, arg);
        पूर्णांक internal_flags = arg | O_NONBLOCK;
        परत real_fcntl(fd, F_SETFL, internal_flags);
    }
    परत real_fcntl(fd, cmd, arg);
}

पूर्णांक freertos_linux_coop_select(पूर्णांक nfds, fd_set *readfds, fd_set *writefds,
                      fd_set *exceptfds, संरचना timeval *timeout)
{
    दीर्घ timeout_ms = -1;
    जर (timeout != रिक्त) {
        timeout_ms = (दीर्घ)timeout->tv_sec * 1000 +
                     (timeout->tv_usec + 999) / 1000;
        जर (timeout_ms == 0) {
            परत real_select(nfds, readfds, writefds, exceptfds, timeout);
        }
    }

    /* real_select() mutates the fd_sets in place (clears them, then sets bits
     * for ready descriptors). To keep polling the same descriptors across
     * iterations we must snapshot the caller's sets and restore them on every
     * loop. */
    fd_set rfds_in, wfds_in, efds_in;
    जर (readfds != रिक्त) {
        rfds_in = *readfds;
    }
    जर (writefds != रिक्त) {
        wfds_in = *writefds;
    }
    जर (exceptfds != रिक्त) {
        efds_in = *exceptfds;
    }

    दीर्घ waited_ms = 0;
    पर्यंत (1) {
        जर (readfds != रिक्त) {
            *readfds = rfds_in;
        }
        जर (writefds != रिक्त) {
            *writefds = wfds_in;
        }
        जर (exceptfds != रिक्त) {
            *exceptfds = efds_in;
        }

        संरचना timeval zero_tv = {0, 0};
        पूर्णांक ret = real_select(nfds, readfds, writefds, exceptfds, &zero_tv);
        जर (ret != 0) {
            परत ret;
        }
        जर (timeout_ms == 0) {
            परत 0;
        }
        जर (timeout_ms > 0 && waited_ms >= timeout_ms) {
            परत 0;
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

__attribute__((weak)) ssize_t पठ(पूर्णांक fd, शून्य *buf, माप_प्रकार मोजणी)
{
    परत freertos_linux_coop_read(fd, buf, मोजणी);
}

__attribute__((weak)) ssize_t लिख(पूर्णांक fd, स्थिरांक शून्य *buf, माप_प्रकार मोजणी)
{
    परत freertos_linux_coop_write(fd, buf, मोजणी);
}

__attribute__((weak)) ssize_t pread(पूर्णांक fd, शून्य *buf, माप_प्रकार मोजणी, off_t offset)
{
    परत freertos_linux_coop_pread(fd, buf, मोजणी, offset);
}

__attribute__((weak)) ssize_t pwrite(पूर्णांक fd, स्थिरांक शून्य *buf, माप_प्रकार मोजणी, off_t offset)
{
    परत freertos_linux_coop_pwrite(fd, buf, मोजणी, offset);
}

__attribute__((weak)) पूर्णांक खोलो(स्थिरांक अक्षर *path, पूर्णांक flags, ...)
{
    पूर्णांक mode = 0;
    जर (flags & O_CREAT) {
        बहु_सूची ap;
        बहु_शुरू(ap, flags);
        mode = बहु_तर्क(ap, पूर्णांक);
        बहु_पूर्ण(ap);
    }
    परत freertos_linux_coop_open(path, flags, mode);
}

__attribute__((weak)) पूर्णांक बंद(पूर्णांक fd)
{
    परत freertos_linux_coop_close(fd);
}

__attribute__((weak)) पूर्णांक fcntl(पूर्णांक fd, पूर्णांक cmd, ...)
{
    पूर्णांक arg = 0;
    जर (cmd != F_GETFL) {
        बहु_सूची list;
        बहु_शुरू(list, cmd);
        arg = बहु_तर्क(list, पूर्णांक);
        बहु_पूर्ण(list);
    }
    परत freertos_linux_coop_fcntl(fd, cmd, arg);
}

__attribute__((weak)) पूर्णांक select(पूर्णांक nfds, fd_set *readfds, fd_set *writefds,
                                 fd_set *exceptfds, संरचना timeval *timeout)
{
    परत freertos_linux_coop_select(nfds, readfds, writefds, exceptfds, timeout);
}

ssize_t freertos_linux_coop_readv(पूर्णांक fd, स्थिरांक संरचना iovec *iov, पूर्णांक iovcnt)
{
    LINUX_COOP_IO_LOOP(fd, real_readv(fd, iov, iovcnt))
}

ssize_t freertos_linux_coop_writev(पूर्णांक fd, स्थिरांक संरचना iovec *iov, पूर्णांक iovcnt)
{
    LINUX_COOP_IO_LOOP(fd, real_writev(fd, iov, iovcnt))
}

ssize_t freertos_linux_coop_recv(पूर्णांक sockfd, शून्य *buf, माप_प्रकार len, पूर्णांक flags)
{
    LINUX_COOP_IO_LOOP(sockfd, real_recv(sockfd, buf, len, flags | MSG_DONTWAIT))
}

ssize_t freertos_linux_coop_send(पूर्णांक sockfd, स्थिरांक शून्य *buf, माप_प्रकार len, पूर्णांक flags)
{
    LINUX_COOP_IO_LOOP(sockfd, real_send(sockfd, buf, len, flags | MSG_DONTWAIT))
}

ssize_t freertos_linux_coop_recvfrom(पूर्णांक sockfd, शून्य *buf, माप_प्रकार len, पूर्णांक flags,
                 संरचना sockaddr *src_addr, socklen_t *addrlen)
{
    LINUX_COOP_IO_LOOP(sockfd, real_recvfrom(sockfd, buf, len, flags | MSG_DONTWAIT,
                            src_addr, addrlen))
}

ssize_t freertos_linux_coop_sendto(पूर्णांक sockfd, स्थिरांक शून्य *buf, माप_प्रकार len, पूर्णांक flags,
               स्थिरांक संरचना sockaddr *dest_addr, socklen_t addrlen)
{
    LINUX_COOP_IO_LOOP(sockfd, real_sendto(sockfd, buf, len, flags | MSG_DONTWAIT,
                          dest_addr, addrlen))
}

ssize_t freertos_linux_coop_recvmsg(पूर्णांक sockfd, संरचना msghdr *संदेश, पूर्णांक flags)
{
    LINUX_COOP_IO_LOOP(sockfd, real_recvmsg(sockfd, संदेश, flags | MSG_DONTWAIT))
}

ssize_t freertos_linux_coop_sendmsg(पूर्णांक sockfd, स्थिरांक संरचना msghdr *संदेश, पूर्णांक flags)
{
    LINUX_COOP_IO_LOOP(sockfd, real_sendmsg(sockfd, संदेश, flags | MSG_DONTWAIT))
}

__attribute__((weak)) ssize_t readv(पूर्णांक fd, स्थिरांक संरचना iovec *iov, पूर्णांक iovcnt)
{
    परत freertos_linux_coop_readv(fd, iov, iovcnt);
}

__attribute__((weak)) ssize_t writev(पूर्णांक fd, स्थिरांक संरचना iovec *iov, पूर्णांक iovcnt)
{
    परत freertos_linux_coop_writev(fd, iov, iovcnt);
}

__attribute__((weak)) ssize_t recv(पूर्णांक sockfd, शून्य *buf, माप_प्रकार len, पूर्णांक flags)
{
    परत freertos_linux_coop_recv(sockfd, buf, len, flags);
}

__attribute__((weak)) ssize_t send(पूर्णांक sockfd, स्थिरांक शून्य *buf, माप_प्रकार len, पूर्णांक flags)
{
    परत freertos_linux_coop_send(sockfd, buf, len, flags);
}

__attribute__((weak)) ssize_t recvfrom(पूर्णांक sockfd, शून्य *buf, माप_प्रकार len, पूर्णांक flags,
                 संरचना sockaddr *src_addr, socklen_t *addrlen)
{
    परत freertos_linux_coop_recvfrom(sockfd, buf, len, flags, src_addr, addrlen);
}

__attribute__((weak)) ssize_t sendto(पूर्णांक sockfd, स्थिरांक शून्य *buf, माप_प्रकार len, पूर्णांक flags,
               स्थिरांक संरचना sockaddr *dest_addr, socklen_t addrlen)
{
    परत freertos_linux_coop_sendto(sockfd, buf, len, flags, dest_addr, addrlen);
}

__attribute__((weak)) ssize_t recvmsg(पूर्णांक sockfd, संरचना msghdr *संदेश, पूर्णांक flags)
{
    परत freertos_linux_coop_recvmsg(sockfd, संदेश, flags);
}

__attribute__((weak)) ssize_t sendmsg(पूर्णांक sockfd, स्थिरांक संरचना msghdr *संदेश, पूर्णांक flags)
{
    परत freertos_linux_coop_sendmsg(sockfd, संदेश, flags);
}

पूर्णांक freertos_linux_coop_connect(पूर्णांक sockfd, स्थिरांक संरचना sockaddr *addr, socklen_t addrlen)
{
    पर्यंत (1)
    {
        पूर्णांक ret = real_connect(sockfd, addr, addrlen);
        जर (ret == 0) {
            परत ret;
        }

        जर (त्रुटि_सं == EINPROGRESS || त्रुटि_सं == EALREADY) {
            संरचना pollfd pfd;
            pfd.fd = sockfd;
            pfd.events = POLLOUT;
            pfd.revents = 0;

            पूर्णांक poll_retries = 0;
            पर्यंत (1)
            {
                पूर्णांक press = real_poll(&pfd, 1, 0);
                जर (press > 0) {
                    पूर्णांक so_err = 0;
                    socklen_t len = आकार(so_err);
                    जर (getsockopt(sockfd, SOL_SOCKET, SO_ERROR,
                                   &so_err, &len) < 0) {
                        परत -1;
                    }
                    जर (so_err == 0) {
                        परत 0;
                    } अन्यथा {
                        त्रुटि_सं = so_err;
                        परत -1;
                    }
                } अन्यथा जर (press == 0) {
                    linux_coop_yield(LINUX_COOP_TICK_MS);
                    सुरूठेवा;
                } अन्यथा {
                    जर (त्रुटि_सं == EINTR) {
                        सुरूठेवा;
                    }
                    /* Non-EINTR poll error; bail out after a few retries */
                    जर (++poll_retries > 3) {
                        परत -1;
                    }
                    linux_coop_yield(LINUX_COOP_TICK_MS);
                    सुरूठेवा;
                }
            }
        }

        जर (त्रुटि_सं == EINTR) {
            परत -1;
        }

        परत -1;
    }
}

पूर्णांक freertos_linux_coop_accept(पूर्णांक sockfd, संरचना sockaddr *addr, socklen_t *addrlen)
{
    पर्यंत (1)
    {
        पूर्णांक fd = real_accept(sockfd, addr, addrlen);
        जर (fd >= 0) {
            linux_coop_set_nonblocking(fd);
            linux_coop_track_fd(fd, linux_coop_fd_user_nonblocking(sockfd) ? O_NONBLOCK : 0);
            परत fd;
        }
        जर (त्रुटि_सं == EAGAIN || त्रुटि_सं == EWOULDBLOCK) {
            जर (linux_coop_fd_user_nonblocking(sockfd)) {
                परत -1;
            }
            linux_coop_yield(LINUX_COOP_TICK_MS);
            सुरूठेवा;
        }
        परत -1;
    }
}

पूर्णांक freertos_linux_coop_pselect(पूर्णांक nfds, fd_set *readfds, fd_set *writefds,
            fd_set *exceptfds, स्थिरांक संरचना timespec *timeout,
            स्थिरांक sigset_t *sigmask)
{
    दीर्घ timeout_ms = -1;
    जर (timeout != रिक्त) {
        timeout_ms = (दीर्घ)timeout->tv_sec * 1000 +
                     (timeout->tv_nsec + 999999) / 1000000;
        जर (timeout_ms == 0) {
            परत real_pselect(nfds, readfds, writefds, exceptfds,
                                timeout, sigmask);
        }
    }

    दीर्घ waited_ms = 0;
    पर्यंत (1)
    {
        संरचना timespec zero_ts = {0, 0};
        पूर्णांक ret = real_pselect(nfds, readfds, writefds, exceptfds,
                               &zero_ts, sigmask);
        जर (ret != 0) {
            परत ret;
        }

        जर (timeout_ms == 0 ||
            (timeout_ms > 0 && waited_ms >= timeout_ms)) {
            परत 0;
        }

        linux_coop_yield(LINUX_COOP_TICK_MS);
        waited_ms += LINUX_COOP_TICK_MS;
    }
}

पूर्णांक freertos_linux_coop_poll(संरचना pollfd *fds, nfds_t nfds, पूर्णांक timeout)
{
    जर (timeout == 0) {
        परत real_poll(fds, nfds, 0);
    }

    दीर्घ timeout_ms = -1;
    जर (timeout > 0) {
        timeout_ms = timeout;
    }

    दीर्घ waited_ms = 0;

    पर्यंत (1)
    {
        पूर्णांक ret = real_poll(fds, nfds, 0);
        जर (ret > 0) {
            परत ret;
        } अन्यथा जर (ret == 0) {
            जर (timeout_ms == 0 ||
                (timeout_ms > 0 && waited_ms >= timeout_ms)) {
                परत 0;
            }
            linux_coop_yield(LINUX_COOP_TICK_MS);
            waited_ms += LINUX_COOP_TICK_MS;
            सुरूठेवा;
        } अन्यथा {
            जर (त्रुटि_सं == EINTR) {
                सुरूठेवा;
            }
            linux_coop_yield(LINUX_COOP_TICK_MS);
            जर (timeout_ms > 0 && waited_ms >= timeout_ms) {
                परत -1;
            }
            waited_ms += LINUX_COOP_TICK_MS;
        }
    }
}

पूर्णांक nanosleep(स्थिरांक संरचना timespec *req, संरचना timespec *rem)
{
    जर (req == रिक्त) {
        त्रुटि_सं = EFAULT;
        परत -1;
    }
    जर (!linux_port_in_freertos_task()) {
        परत real_nanosleep(req, rem);
    }
    दीर्घ ms = req->tv_sec * 1000 + (req->tv_nsec + 999999) / 1000000;
    जर (ms <= 0) {
        ms = 1;
    }
    vTaskDelay(ms);
    जर (rem) {
        rem->tv_sec = 0;
        rem->tv_nsec = 0;
    }
    परत 0;
}

अचिन्हित पूर्णांक sleep(अचिन्हित पूर्णांक seconds)
{
    linux_coop_yield(seconds * 1000);
    परत 0;
}

पूर्णांक usleep(useconds_t usec)
{
    linux_coop_yield(usec / 1000);
    परत 0;
}

पूर्णांक freertos_linux_coop_socket(पूर्णांक domain, पूर्णांक type, पूर्णांक protocol)
{
    पूर्णांक fd = real_socket(domain, type, protocol);
    linux_coop_set_nonblocking(fd);
#ifdef SOCK_NONBLOCK
    स्थिरांक पूर्णांक user_flags = (type & SOCK_NONBLOCK) ? O_NONBLOCK : 0;
#अन्यथा
    स्थिरांक पूर्णांक user_flags = 0;
#endif
    linux_coop_track_fd(fd, user_flags);
    परत fd;
}

पूर्णांक freertos_linux_coop_socketpair(पूर्णांक domain, पूर्णांक type, पूर्णांक protocol, पूर्णांक *sv)
{
    पूर्णांक ret = real_socketpair(domain, type, protocol, sv);
    जर (ret == 0) {
        linux_coop_set_nonblocking(sv[0]);
        linux_coop_set_nonblocking(sv[1]);
#ifdef SOCK_NONBLOCK
        स्थिरांक पूर्णांक user_flags = (type & SOCK_NONBLOCK) ? O_NONBLOCK : 0;
#अन्यथा
        स्थिरांक पूर्णांक user_flags = 0;
#endif
        linux_coop_track_fd(sv[0], user_flags);
        linux_coop_track_fd(sv[1], user_flags);
    }
    परत ret;
}

पूर्णांक freertos_linux_coop_pipe(पूर्णांक *fds)
{
    पूर्णांक ret = real_pipe(fds);
    जर (ret == 0) {
        linux_coop_set_nonblocking(fds[0]);
        linux_coop_set_nonblocking(fds[1]);
        linux_coop_track_fd(fds[0], 0);
        linux_coop_track_fd(fds[1], 0);
    }
    परत ret;
}

पूर्णांक freertos_linux_coop_pipe2(पूर्णांक *fds, पूर्णांक flags)
{
    पूर्णांक ret = real_pipe2(fds, flags);
    जर (ret == 0) {
        linux_coop_set_nonblocking(fds[0]);
        linux_coop_set_nonblocking(fds[1]);
        स्थिरांक पूर्णांक user_flags = (flags & O_NONBLOCK) ? O_NONBLOCK : 0;
        linux_coop_track_fd(fds[0], user_flags);
        linux_coop_track_fd(fds[1], user_flags);
    }
    परत ret;
}

पूर्णांक freertos_linux_coop_dup(पूर्णांक oldfd)
{
    पूर्णांक fd = real_dup(oldfd);
    linux_coop_set_nonblocking(fd);
    linux_coop_track_fd(fd, linux_coop_fd_user_nonblocking(oldfd) ? O_NONBLOCK : 0);
    परत fd;
}

पूर्णांक freertos_linux_coop_dup2(पूर्णांक oldfd, पूर्णांक newfd)
{
    पूर्णांक fd = real_dup2(oldfd, newfd);
    linux_coop_set_nonblocking(fd);
    linux_coop_track_fd(fd, linux_coop_fd_user_nonblocking(oldfd) ? O_NONBLOCK : 0);
    परत fd;
}

__attribute__((weak)) पूर्णांक connect(पूर्णांक sockfd, स्थिरांक संरचना sockaddr *addr, socklen_t addrlen)
{
    परत freertos_linux_coop_connect(sockfd, addr, addrlen);
}

__attribute__((weak)) पूर्णांक accept(पूर्णांक sockfd, संरचना sockaddr *addr, socklen_t *addrlen)
{
    परत freertos_linux_coop_accept(sockfd, addr, addrlen);
}

__attribute__((weak)) पूर्णांक pselect(पूर्णांक nfds, fd_set *readfds, fd_set *writefds,
            fd_set *exceptfds, स्थिरांक संरचना timespec *timeout,
            स्थिरांक sigset_t *sigmask)
{
    परत freertos_linux_coop_pselect(nfds, readfds, writefds, exceptfds, timeout, sigmask);
}

__attribute__((weak)) पूर्णांक poll(संरचना pollfd *fds, nfds_t nfds, पूर्णांक timeout)
{
    परत freertos_linux_coop_poll(fds, nfds, timeout);
}

__attribute__((weak)) पूर्णांक socket(पूर्णांक domain, पूर्णांक type, पूर्णांक protocol)
{
    परत freertos_linux_coop_socket(domain, type, protocol);
}

__attribute__((weak)) पूर्णांक socketpair(पूर्णांक domain, पूर्णांक type, पूर्णांक protocol, पूर्णांक sv[2])
{
    परत freertos_linux_coop_socketpair(domain, type, protocol, sv);
}

__attribute__((weak)) पूर्णांक pipe(पूर्णांक fds[2])
{
    परत freertos_linux_coop_pipe(fds);
}

__attribute__((weak)) पूर्णांक pipe2(पूर्णांक fds[2], पूर्णांक flags)
{
    परत freertos_linux_coop_pipe2(fds, flags);
}

__attribute__((weak)) पूर्णांक dup(पूर्णांक oldfd)
{
    परत freertos_linux_coop_dup(oldfd);
}

__attribute__((weak)) पूर्णांक dup2(पूर्णांक oldfd, पूर्णांक newfd)
{
    परत freertos_linux_coop_dup2(oldfd, newfd);
}
