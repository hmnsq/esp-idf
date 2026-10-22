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

#समावेश <stdio.h>
#समावेश <stdlib.h>
#समावेश <string.h>
#समावेश <errno.h>
#समावेश <fcntl.h>
#समावेश <stdarg.h>
#समावेश <unistd.h>
#समावेश <sys/types.h>
#समावेश <sys/uio.h>
#समावेश <sys/socket.h>
#समावेश <sys/select.h>
#समावेश <sys/time.h>
#समावेश <time.h>
#समावेश <poll.h>
#समावेश <dlfcn.h>
#समावेश <pthread.h>

#समावेश "linux_port_coop_internal.h"
#समावेश "esp_private/freertos_linux_coop_syscalls.h"

अटल ssize_t  (*real_readv)(पूर्णांक, स्थिर काष्ठा iovec *, पूर्णांक);
अटल ssize_t  (*real_writev)(पूर्णांक, स्थिर काष्ठा iovec *, पूर्णांक);
अटल ssize_t  (*real_recv)(पूर्णांक, व्योम *, माप_प्रकार, पूर्णांक);
अटल ssize_t  (*real_send)(पूर्णांक, स्थिर व्योम *, माप_प्रकार, पूर्णांक);
अटल ssize_t  (*real_recvfrom)(पूर्णांक, व्योम *, माप_प्रकार, पूर्णांक,
                                 काष्ठा sockaddr *, socklen_t *);
अटल ssize_t  (*real_sendto)(पूर्णांक, स्थिर व्योम *, माप_प्रकार, पूर्णांक,
                                स्थिर काष्ठा sockaddr *, socklen_t);
अटल ssize_t  (*real_recvmsg)(पूर्णांक, काष्ठा msghdr *, पूर्णांक);
अटल ssize_t  (*real_sendmsg)(पूर्णांक, स्थिर काष्ठा msghdr *, पूर्णांक);
अटल पूर्णांक      (*real_connect)(पूर्णांक, स्थिर काष्ठा sockaddr *, socklen_t);
अटल पूर्णांक      (*real_accept)(पूर्णांक, काष्ठा sockaddr *, socklen_t *);
अटल पूर्णांक      (*real_pselect)(पूर्णांक, fd_set *, fd_set *, fd_set *,
                                स्थिर काष्ठा timespec *, स्थिर sigset_t *);
अटल पूर्णांक      (*real_poll)(काष्ठा pollfd *, nfds_t, पूर्णांक);
अटल पूर्णांक      (*real_socket)(पूर्णांक, पूर्णांक, पूर्णांक);
अटल पूर्णांक      (*real_socketpair)(पूर्णांक, पूर्णांक, पूर्णांक, पूर्णांक[2]);
अटल पूर्णांक      (*real_pipe)(पूर्णांक[2]);
अटल पूर्णांक      (*real_pipe2)(पूर्णांक[2], पूर्णांक);
अटल पूर्णांक      (*real_dup)(पूर्णांक);
अटल पूर्णांक      (*real_dup2)(पूर्णांक, पूर्णांक);
अटल पूर्णांक      (*real_nanosleep)(स्थिर काष्ठा timespec *, काष्ठा timespec *);
अटल ssize_t  (*real_read)(पूर्णांक, व्योम *, माप_प्रकार);
अटल ssize_t  (*real_write)(पूर्णांक, स्थिर व्योम *, माप_प्रकार);
अटल ssize_t  (*real_pread)(पूर्णांक, व्योम *, माप_प्रकार, off_t);
अटल ssize_t  (*real_pwrite)(पूर्णांक, स्थिर व्योम *, माप_प्रकार, off_t);
अटल पूर्णांक      (*real_close)(पूर्णांक);
अटल पूर्णांक      (*real_select)(पूर्णांक, fd_set *, fd_set *, fd_set *,
                                काष्ठा timeval *);

प्रकार पूर्णांक (*linux_coop_fcntl_fn_t)(पूर्णांक, पूर्णांक, ...);
अटल linux_coop_fcntl_fn_t real_fcntl;

प्रकार पूर्णांक (*linux_coop_open_fn_t)(स्थिर अक्षर *, पूर्णांक, mode_t);
अटल linux_coop_open_fn_t real_open;

प्रकार काष्ठा linux_coop_fd_state {
    पूर्णांक fd;
    पूर्णांक user_flags;
    काष्ठा linux_coop_fd_state *अग्र;
} linux_coop_fd_state_t;

अटल linux_coop_fd_state_t *s_fd_state_list;
अटल pthread_mutex_t s_fd_state_lock = PTHREAD_MUTEX_INITIALIZER;

अटल linux_coop_fd_state_t *linux_coop_find_fd_locked(पूर्णांक fd)
{
    linux_coop_fd_state_t *गाँठ = s_fd_state_list;
    जबतक (गाँठ != शून्य) {
        अगर (गाँठ->fd == fd) {
            वापस गाँठ;
        }
        गाँठ = गाँठ->अग्र;
    }
    वापस शून्य;
}

अटल व्योम __attribute__((constructor)) linux_coop_resolve_all(व्योम)
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
    LINUX_COOP_RESOLVE(पढ़ो);
    LINUX_COOP_RESOLVE(ग_लिखो);
    LINUX_COOP_RESOLVE(pread);
    LINUX_COOP_RESOLVE(pwrite);
    LINUX_COOP_RESOLVE(बंद);
    LINUX_COOP_RESOLVE(select);
    LINUX_COOP_RESOLVE(खोलो);
}

व्योम linux_coop_set_nonblocking(पूर्णांक fd)
{
    अगर (fd >= 0) {
        पूर्णांक flags = real_fcntl(fd, F_GETFL, 0);
        अगर (flags >= 0) {
            real_fcntl(fd, F_SETFL, flags | O_NONBLOCK);
        }
    }
}

व्योम linux_coop_track_fd(पूर्णांक fd, पूर्णांक user_flags)
{
    अगर (fd < 0) {
        वापस;
    }

    pthread_mutex_lock(&s_fd_state_lock);
    linux_coop_fd_state_t *गाँठ = linux_coop_find_fd_locked(fd);
    अगर (गाँठ != शून्य) {
        गाँठ->user_flags = user_flags;
        pthread_mutex_unlock(&s_fd_state_lock);
        वापस;
    }

    गाँठ = सुस्मृति(1, माप(*गाँठ));
    अगर (गाँठ != शून्य) {
        गाँठ->fd = fd;
        गाँठ->user_flags = user_flags;
        गाँठ->अग्र = s_fd_state_list;
        s_fd_state_list = गाँठ;
    }
    pthread_mutex_unlock(&s_fd_state_lock);
}

व्योम linux_coop_untrack_fd(पूर्णांक fd)
{
    अगर (fd < 0) {
        वापस;
    }

    pthread_mutex_lock(&s_fd_state_lock);
    linux_coop_fd_state_t *पूर्व = शून्य;
    linux_coop_fd_state_t *गाँठ = s_fd_state_list;
    जबतक (गाँठ != शून्य) {
        अगर (गाँठ->fd == fd) {
            अगर (पूर्व == शून्य) {
                s_fd_state_list = गाँठ->अग्र;
            } अन्यथा {
                पूर्व->अग्र = गाँठ->अग्र;
            }
            मुक्त(गाँठ);
            अवरोध;
        }
        पूर्व = गाँठ;
        गाँठ = गाँठ->अग्र;
    }
    pthread_mutex_unlock(&s_fd_state_lock);
}

व्योम linux_coop_set_user_flags(पूर्णांक fd, पूर्णांक user_flags)
{
    linux_coop_track_fd(fd, user_flags);
}

bool linux_coop_get_user_flags(पूर्णांक fd, पूर्णांक *flags)
{
    अगर (fd < 0 || flags == शून्य) {
        वापस false;
    }

    pthread_mutex_lock(&s_fd_state_lock);
    linux_coop_fd_state_t *गाँठ = linux_coop_find_fd_locked(fd);
    अगर (गाँठ == शून्य) {
        pthread_mutex_unlock(&s_fd_state_lock);
        वापस false;
    }
    *flags = गाँठ->user_flags;
    pthread_mutex_unlock(&s_fd_state_lock);
    वापस true;
}

bool linux_coop_fd_user_nonblocking(पूर्णांक fd)
{
    पूर्णांक user_flags = 0;
    अगर (linux_coop_get_user_flags(fd, &user_flags)) {
        वापस (user_flags & O_NONBLOCK) != 0;
    }
    वापस false;
}


ssize_t freertos_linux_coop_read(पूर्णांक fd, व्योम *buf, माप_प्रकार गिनती)
{
    LINUX_COOP_IO_LOOP(fd, real_read(fd, buf, गिनती))
}

ssize_t freertos_linux_coop_write(पूर्णांक fd, स्थिर व्योम *buf, माप_प्रकार गिनती)
{
    LINUX_COOP_IO_LOOP(fd, real_write(fd, buf, गिनती))
}

ssize_t freertos_linux_coop_pread(पूर्णांक fd, व्योम *buf, माप_प्रकार गिनती, off_t offset)
{
    LINUX_COOP_IO_LOOP(fd, real_pread(fd, buf, गिनती, offset))
}

ssize_t freertos_linux_coop_pwrite(पूर्णांक fd, स्थिर व्योम *buf, माप_प्रकार गिनती, off_t offset)
{
    LINUX_COOP_IO_LOOP(fd, real_pwrite(fd, buf, गिनती, offset))
}

पूर्णांक freertos_linux_coop_open(स्थिर अक्षर *path, पूर्णांक flags, पूर्णांक mode)
{
    पूर्णांक fd = real_open(path, flags, (mode_t)mode);
    linux_coop_set_nonblocking(fd);
    linux_coop_track_fd(fd, flags & O_NONBLOCK);
    वापस fd;
}

पूर्णांक freertos_linux_coop_close(पूर्णांक fd)
{
    पूर्णांक ret = real_close(fd);
    अगर (ret == 0 || त्रुटि_सं == EINTR) {
        /* On Linux, close() always releases the fd even when returning
         * EINTR.  Retrying would risk closing a fd reused by another
         * thread.  Treat EINTR as success. */
        linux_coop_untrack_fd(fd);
        वापस 0;
    }
    वापस -1;
}

पूर्णांक freertos_linux_coop_fcntl(पूर्णांक fd, पूर्णांक cmd, पूर्णांक arg)
{
    अगर (cmd == F_GETFL) {
        पूर्णांक real_flags = real_fcntl(fd, F_GETFL, 0);
        अगर (real_flags < 0) {
            वापस -1;
        }
        पूर्णांक user_flags = 0;
        अगर (linux_coop_get_user_flags(fd, &user_flags)) {
            real_flags &= ~O_NONBLOCK;
            real_flags |= (user_flags & O_NONBLOCK);
        }
        वापस real_flags;
    }
    अगर (cmd == F_SETFL) {
        linux_coop_set_user_flags(fd, arg);
        पूर्णांक internal_flags = arg | O_NONBLOCK;
        वापस real_fcntl(fd, F_SETFL, internal_flags);
    }
    वापस real_fcntl(fd, cmd, arg);
}

पूर्णांक freertos_linux_coop_select(पूर्णांक nfds, fd_set *readfds, fd_set *writefds,
                      fd_set *exceptfds, काष्ठा timeval *timeout)
{
    दीर्घ timeout_ms = -1;
    अगर (timeout != शून्य) {
        timeout_ms = (दीर्घ)timeout->tv_sec * 1000 +
                     (timeout->tv_usec + 999) / 1000;
        अगर (timeout_ms == 0) {
            वापस real_select(nfds, readfds, writefds, exceptfds, timeout);
        }
    }

    /* real_select() mutates the fd_sets in place (clears them, then sets bits
     * for ready descriptors). To keep polling the same descriptors across
     * iterations we must snapshot the caller's sets and restore them on every
     * loop. */
    fd_set rfds_in, wfds_in, efds_in;
    अगर (readfds != शून्य) {
        rfds_in = *readfds;
    }
    अगर (writefds != शून्य) {
        wfds_in = *writefds;
    }
    अगर (exceptfds != शून्य) {
        efds_in = *exceptfds;
    }

    दीर्घ waited_ms = 0;
    जबतक (1) {
        अगर (readfds != शून्य) {
            *readfds = rfds_in;
        }
        अगर (writefds != शून्य) {
            *writefds = wfds_in;
        }
        अगर (exceptfds != शून्य) {
            *exceptfds = efds_in;
        }

        काष्ठा timeval zero_tv = {0, 0};
        पूर्णांक ret = real_select(nfds, readfds, writefds, exceptfds, &zero_tv);
        अगर (ret != 0) {
            वापस ret;
        }
        अगर (timeout_ms == 0) {
            वापस 0;
        }
        अगर (timeout_ms > 0 && waited_ms >= timeout_ms) {
            वापस 0;
        }
        linux_coop_yield(LINUX_COOP_TICK_MS);
        waited_ms += LINUX_COOP_TICK_MS;
    }
}

व्योम freertos_linux_coop_syscalls_init(व्योम)
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

__attribute__((weak)) ssize_t पढ़ो(पूर्णांक fd, व्योम *buf, माप_प्रकार गिनती)
{
    वापस freertos_linux_coop_read(fd, buf, गिनती);
}

__attribute__((weak)) ssize_t ग_लिखो(पूर्णांक fd, स्थिर व्योम *buf, माप_प्रकार गिनती)
{
    वापस freertos_linux_coop_write(fd, buf, गिनती);
}

__attribute__((weak)) ssize_t pread(पूर्णांक fd, व्योम *buf, माप_प्रकार गिनती, off_t offset)
{
    वापस freertos_linux_coop_pread(fd, buf, गिनती, offset);
}

__attribute__((weak)) ssize_t pwrite(पूर्णांक fd, स्थिर व्योम *buf, माप_प्रकार गिनती, off_t offset)
{
    वापस freertos_linux_coop_pwrite(fd, buf, गिनती, offset);
}

__attribute__((weak)) पूर्णांक खोलो(स्थिर अक्षर *path, पूर्णांक flags, ...)
{
    पूर्णांक mode = 0;
    अगर (flags & O_CREAT) {
        बहु_सूची ap;
        बहु_शुरू(ap, flags);
        mode = बहु_तर्क(ap, पूर्णांक);
        बहु_पूर्ण(ap);
    }
    वापस freertos_linux_coop_open(path, flags, mode);
}

__attribute__((weak)) पूर्णांक बंद(पूर्णांक fd)
{
    वापस freertos_linux_coop_close(fd);
}

__attribute__((weak)) पूर्णांक fcntl(पूर्णांक fd, पूर्णांक cmd, ...)
{
    पूर्णांक arg = 0;
    अगर (cmd != F_GETFL) {
        बहु_सूची list;
        बहु_शुरू(list, cmd);
        arg = बहु_तर्क(list, पूर्णांक);
        बहु_पूर्ण(list);
    }
    वापस freertos_linux_coop_fcntl(fd, cmd, arg);
}

__attribute__((weak)) पूर्णांक select(पूर्णांक nfds, fd_set *readfds, fd_set *writefds,
                                 fd_set *exceptfds, काष्ठा timeval *timeout)
{
    वापस freertos_linux_coop_select(nfds, readfds, writefds, exceptfds, timeout);
}

ssize_t freertos_linux_coop_readv(पूर्णांक fd, स्थिर काष्ठा iovec *iov, पूर्णांक iovcnt)
{
    LINUX_COOP_IO_LOOP(fd, real_readv(fd, iov, iovcnt))
}

ssize_t freertos_linux_coop_writev(पूर्णांक fd, स्थिर काष्ठा iovec *iov, पूर्णांक iovcnt)
{
    LINUX_COOP_IO_LOOP(fd, real_writev(fd, iov, iovcnt))
}

ssize_t freertos_linux_coop_recv(पूर्णांक sockfd, व्योम *buf, माप_प्रकार len, पूर्णांक flags)
{
    LINUX_COOP_IO_LOOP(sockfd, real_recv(sockfd, buf, len, flags | MSG_DONTWAIT))
}

ssize_t freertos_linux_coop_send(पूर्णांक sockfd, स्थिर व्योम *buf, माप_प्रकार len, पूर्णांक flags)
{
    LINUX_COOP_IO_LOOP(sockfd, real_send(sockfd, buf, len, flags | MSG_DONTWAIT))
}

ssize_t freertos_linux_coop_recvfrom(पूर्णांक sockfd, व्योम *buf, माप_प्रकार len, पूर्णांक flags,
                 काष्ठा sockaddr *src_addr, socklen_t *addrlen)
{
    LINUX_COOP_IO_LOOP(sockfd, real_recvfrom(sockfd, buf, len, flags | MSG_DONTWAIT,
                            src_addr, addrlen))
}

ssize_t freertos_linux_coop_sendto(पूर्णांक sockfd, स्थिर व्योम *buf, माप_प्रकार len, पूर्णांक flags,
               स्थिर काष्ठा sockaddr *dest_addr, socklen_t addrlen)
{
    LINUX_COOP_IO_LOOP(sockfd, real_sendto(sockfd, buf, len, flags | MSG_DONTWAIT,
                          dest_addr, addrlen))
}

ssize_t freertos_linux_coop_recvmsg(पूर्णांक sockfd, काष्ठा msghdr *संदेश, पूर्णांक flags)
{
    LINUX_COOP_IO_LOOP(sockfd, real_recvmsg(sockfd, संदेश, flags | MSG_DONTWAIT))
}

ssize_t freertos_linux_coop_sendmsg(पूर्णांक sockfd, स्थिर काष्ठा msghdr *संदेश, पूर्णांक flags)
{
    LINUX_COOP_IO_LOOP(sockfd, real_sendmsg(sockfd, संदेश, flags | MSG_DONTWAIT))
}

__attribute__((weak)) ssize_t readv(पूर्णांक fd, स्थिर काष्ठा iovec *iov, पूर्णांक iovcnt)
{
    वापस freertos_linux_coop_readv(fd, iov, iovcnt);
}

__attribute__((weak)) ssize_t writev(पूर्णांक fd, स्थिर काष्ठा iovec *iov, पूर्णांक iovcnt)
{
    वापस freertos_linux_coop_writev(fd, iov, iovcnt);
}

__attribute__((weak)) ssize_t recv(पूर्णांक sockfd, व्योम *buf, माप_प्रकार len, पूर्णांक flags)
{
    वापस freertos_linux_coop_recv(sockfd, buf, len, flags);
}

__attribute__((weak)) ssize_t send(पूर्णांक sockfd, स्थिर व्योम *buf, माप_प्रकार len, पूर्णांक flags)
{
    वापस freertos_linux_coop_send(sockfd, buf, len, flags);
}

__attribute__((weak)) ssize_t recvfrom(पूर्णांक sockfd, व्योम *buf, माप_प्रकार len, पूर्णांक flags,
                 काष्ठा sockaddr *src_addr, socklen_t *addrlen)
{
    वापस freertos_linux_coop_recvfrom(sockfd, buf, len, flags, src_addr, addrlen);
}

__attribute__((weak)) ssize_t sendto(पूर्णांक sockfd, स्थिर व्योम *buf, माप_प्रकार len, पूर्णांक flags,
               स्थिर काष्ठा sockaddr *dest_addr, socklen_t addrlen)
{
    वापस freertos_linux_coop_sendto(sockfd, buf, len, flags, dest_addr, addrlen);
}

__attribute__((weak)) ssize_t recvmsg(पूर्णांक sockfd, काष्ठा msghdr *संदेश, पूर्णांक flags)
{
    वापस freertos_linux_coop_recvmsg(sockfd, संदेश, flags);
}

__attribute__((weak)) ssize_t sendmsg(पूर्णांक sockfd, स्थिर काष्ठा msghdr *संदेश, पूर्णांक flags)
{
    वापस freertos_linux_coop_sendmsg(sockfd, संदेश, flags);
}

पूर्णांक freertos_linux_coop_connect(पूर्णांक sockfd, स्थिर काष्ठा sockaddr *addr, socklen_t addrlen)
{
    जबतक (1)
    {
        पूर्णांक ret = real_connect(sockfd, addr, addrlen);
        अगर (ret == 0) {
            वापस ret;
        }

        अगर (त्रुटि_सं == EINPROGRESS || त्रुटि_सं == EALREADY) {
            काष्ठा pollfd pfd;
            pfd.fd = sockfd;
            pfd.events = POLLOUT;
            pfd.revents = 0;

            पूर्णांक poll_retries = 0;
            जबतक (1)
            {
                पूर्णांक press = real_poll(&pfd, 1, 0);
                अगर (press > 0) {
                    पूर्णांक so_err = 0;
                    socklen_t len = माप(so_err);
                    अगर (getsockopt(sockfd, SOL_SOCKET, SO_ERROR,
                                   &so_err, &len) < 0) {
                        वापस -1;
                    }
                    अगर (so_err == 0) {
                        वापस 0;
                    } अन्यथा {
                        त्रुटि_सं = so_err;
                        वापस -1;
                    }
                } अन्यथा अगर (press == 0) {
                    linux_coop_yield(LINUX_COOP_TICK_MS);
                    जारी;
                } अन्यथा {
                    अगर (त्रुटि_सं == EINTR) {
                        जारी;
                    }
                    /* Non-EINTR poll error; bail out after a few retries */
                    अगर (++poll_retries > 3) {
                        वापस -1;
                    }
                    linux_coop_yield(LINUX_COOP_TICK_MS);
                    जारी;
                }
            }
        }

        अगर (त्रुटि_सं == EINTR) {
            वापस -1;
        }

        वापस -1;
    }
}

पूर्णांक freertos_linux_coop_accept(पूर्णांक sockfd, काष्ठा sockaddr *addr, socklen_t *addrlen)
{
    जबतक (1)
    {
        पूर्णांक fd = real_accept(sockfd, addr, addrlen);
        अगर (fd >= 0) {
            linux_coop_set_nonblocking(fd);
            linux_coop_track_fd(fd, linux_coop_fd_user_nonblocking(sockfd) ? O_NONBLOCK : 0);
            वापस fd;
        }
        अगर (त्रुटि_सं == EAGAIN || त्रुटि_सं == EWOULDBLOCK) {
            अगर (linux_coop_fd_user_nonblocking(sockfd)) {
                वापस -1;
            }
            linux_coop_yield(LINUX_COOP_TICK_MS);
            जारी;
        }
        वापस -1;
    }
}

पूर्णांक freertos_linux_coop_pselect(पूर्णांक nfds, fd_set *readfds, fd_set *writefds,
            fd_set *exceptfds, स्थिर काष्ठा timespec *timeout,
            स्थिर sigset_t *sigmask)
{
    दीर्घ timeout_ms = -1;
    अगर (timeout != शून्य) {
        timeout_ms = (दीर्घ)timeout->tv_sec * 1000 +
                     (timeout->tv_nsec + 999999) / 1000000;
        अगर (timeout_ms == 0) {
            वापस real_pselect(nfds, readfds, writefds, exceptfds,
                                timeout, sigmask);
        }
    }

    दीर्घ waited_ms = 0;
    जबतक (1)
    {
        काष्ठा timespec zero_ts = {0, 0};
        पूर्णांक ret = real_pselect(nfds, readfds, writefds, exceptfds,
                               &zero_ts, sigmask);
        अगर (ret != 0) {
            वापस ret;
        }

        अगर (timeout_ms == 0 ||
            (timeout_ms > 0 && waited_ms >= timeout_ms)) {
            वापस 0;
        }

        linux_coop_yield(LINUX_COOP_TICK_MS);
        waited_ms += LINUX_COOP_TICK_MS;
    }
}

पूर्णांक freertos_linux_coop_poll(काष्ठा pollfd *fds, nfds_t nfds, पूर्णांक timeout)
{
    अगर (timeout == 0) {
        वापस real_poll(fds, nfds, 0);
    }

    दीर्घ timeout_ms = -1;
    अगर (timeout > 0) {
        timeout_ms = timeout;
    }

    दीर्घ waited_ms = 0;

    जबतक (1)
    {
        पूर्णांक ret = real_poll(fds, nfds, 0);
        अगर (ret > 0) {
            वापस ret;
        } अन्यथा अगर (ret == 0) {
            अगर (timeout_ms == 0 ||
                (timeout_ms > 0 && waited_ms >= timeout_ms)) {
                वापस 0;
            }
            linux_coop_yield(LINUX_COOP_TICK_MS);
            waited_ms += LINUX_COOP_TICK_MS;
            जारी;
        } अन्यथा {
            अगर (त्रुटि_सं == EINTR) {
                जारी;
            }
            linux_coop_yield(LINUX_COOP_TICK_MS);
            अगर (timeout_ms > 0 && waited_ms >= timeout_ms) {
                वापस -1;
            }
            waited_ms += LINUX_COOP_TICK_MS;
        }
    }
}

पूर्णांक nanosleep(स्थिर काष्ठा timespec *req, काष्ठा timespec *rem)
{
    अगर (req == शून्य) {
        त्रुटि_सं = EFAULT;
        वापस -1;
    }
    अगर (!linux_port_in_freertos_task()) {
        वापस real_nanosleep(req, rem);
    }
    दीर्घ ms = req->tv_sec * 1000 + (req->tv_nsec + 999999) / 1000000;
    अगर (ms <= 0) {
        ms = 1;
    }
    vTaskDelay(ms);
    अगर (rem) {
        rem->tv_sec = 0;
        rem->tv_nsec = 0;
    }
    वापस 0;
}

अचिन्हित पूर्णांक sleep(अचिन्हित पूर्णांक seconds)
{
    linux_coop_yield(seconds * 1000);
    वापस 0;
}

पूर्णांक usleep(useconds_t usec)
{
    linux_coop_yield(usec / 1000);
    वापस 0;
}

पूर्णांक freertos_linux_coop_socket(पूर्णांक domain, पूर्णांक type, पूर्णांक protocol)
{
    पूर्णांक fd = real_socket(domain, type, protocol);
    linux_coop_set_nonblocking(fd);
#अगर_घोषित SOCK_NONBLOCK
    स्थिर पूर्णांक user_flags = (type & SOCK_NONBLOCK) ? O_NONBLOCK : 0;
#अन्यथा
    स्थिर पूर्णांक user_flags = 0;
#पूर्ण_अगर
    linux_coop_track_fd(fd, user_flags);
    वापस fd;
}

पूर्णांक freertos_linux_coop_socketpair(पूर्णांक domain, पूर्णांक type, पूर्णांक protocol, पूर्णांक *sv)
{
    पूर्णांक ret = real_socketpair(domain, type, protocol, sv);
    अगर (ret == 0) {
        linux_coop_set_nonblocking(sv[0]);
        linux_coop_set_nonblocking(sv[1]);
#अगर_घोषित SOCK_NONBLOCK
        स्थिर पूर्णांक user_flags = (type & SOCK_NONBLOCK) ? O_NONBLOCK : 0;
#अन्यथा
        स्थिर पूर्णांक user_flags = 0;
#पूर्ण_अगर
        linux_coop_track_fd(sv[0], user_flags);
        linux_coop_track_fd(sv[1], user_flags);
    }
    वापस ret;
}

पूर्णांक freertos_linux_coop_pipe(पूर्णांक *fds)
{
    पूर्णांक ret = real_pipe(fds);
    अगर (ret == 0) {
        linux_coop_set_nonblocking(fds[0]);
        linux_coop_set_nonblocking(fds[1]);
        linux_coop_track_fd(fds[0], 0);
        linux_coop_track_fd(fds[1], 0);
    }
    वापस ret;
}

पूर्णांक freertos_linux_coop_pipe2(पूर्णांक *fds, पूर्णांक flags)
{
    पूर्णांक ret = real_pipe2(fds, flags);
    अगर (ret == 0) {
        linux_coop_set_nonblocking(fds[0]);
        linux_coop_set_nonblocking(fds[1]);
        स्थिर पूर्णांक user_flags = (flags & O_NONBLOCK) ? O_NONBLOCK : 0;
        linux_coop_track_fd(fds[0], user_flags);
        linux_coop_track_fd(fds[1], user_flags);
    }
    वापस ret;
}

पूर्णांक freertos_linux_coop_dup(पूर्णांक oldfd)
{
    पूर्णांक fd = real_dup(oldfd);
    linux_coop_set_nonblocking(fd);
    linux_coop_track_fd(fd, linux_coop_fd_user_nonblocking(oldfd) ? O_NONBLOCK : 0);
    वापस fd;
}

पूर्णांक freertos_linux_coop_dup2(पूर्णांक oldfd, पूर्णांक newfd)
{
    पूर्णांक fd = real_dup2(oldfd, newfd);
    linux_coop_set_nonblocking(fd);
    linux_coop_track_fd(fd, linux_coop_fd_user_nonblocking(oldfd) ? O_NONBLOCK : 0);
    वापस fd;
}

__attribute__((weak)) पूर्णांक connect(पूर्णांक sockfd, स्थिर काष्ठा sockaddr *addr, socklen_t addrlen)
{
    वापस freertos_linux_coop_connect(sockfd, addr, addrlen);
}

__attribute__((weak)) पूर्णांक accept(पूर्णांक sockfd, काष्ठा sockaddr *addr, socklen_t *addrlen)
{
    वापस freertos_linux_coop_accept(sockfd, addr, addrlen);
}

__attribute__((weak)) पूर्णांक pselect(पूर्णांक nfds, fd_set *readfds, fd_set *writefds,
            fd_set *exceptfds, स्थिर काष्ठा timespec *timeout,
            स्थिर sigset_t *sigmask)
{
    वापस freertos_linux_coop_pselect(nfds, readfds, writefds, exceptfds, timeout, sigmask);
}

__attribute__((weak)) पूर्णांक poll(काष्ठा pollfd *fds, nfds_t nfds, पूर्णांक timeout)
{
    वापस freertos_linux_coop_poll(fds, nfds, timeout);
}

__attribute__((weak)) पूर्णांक socket(पूर्णांक domain, पूर्णांक type, पूर्णांक protocol)
{
    वापस freertos_linux_coop_socket(domain, type, protocol);
}

__attribute__((weak)) पूर्णांक socketpair(पूर्णांक domain, पूर्णांक type, पूर्णांक protocol, पूर्णांक sv[2])
{
    वापस freertos_linux_coop_socketpair(domain, type, protocol, sv);
}

__attribute__((weak)) पूर्णांक pipe(पूर्णांक fds[2])
{
    वापस freertos_linux_coop_pipe(fds);
}

__attribute__((weak)) पूर्णांक pipe2(पूर्णांक fds[2], पूर्णांक flags)
{
    वापस freertos_linux_coop_pipe2(fds, flags);
}

__attribute__((weak)) पूर्णांक dup(पूर्णांक oldfd)
{
    वापस freertos_linux_coop_dup(oldfd);
}

__attribute__((weak)) पूर्णांक dup2(पूर्णांक oldfd, पूर्णांक newfd)
{
    वापस freertos_linux_coop_dup2(oldfd, newfd);
}
