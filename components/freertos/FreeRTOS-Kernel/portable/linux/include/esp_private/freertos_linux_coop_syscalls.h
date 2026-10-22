/*
 * SPDX-FileCopyrightText: 2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Public API for the FreeRTOS Linux cooperative syscall layer.
 *
 * This header is included by the VFS component (and any other component
 * that needs to call the cooperative I/O functions directly).
 *
 * Internal helpers (LINUX_COOP_IO_LOOP, LINUX_COOP_RESOLVE, FD state
 * table functions, etc.) live in linux_port_coop_internal.h.
 */

#pragma once

#अन्तर्भाव <stddef.h>
#अन्तर्भाव <sys/types.h>
#अन्तर्भाव <sys/select.h>
#अन्तर्भाव <sys/time.h>
#अन्तर्भाव <sys/uio.h>
#अन्तर्भाव <sys/socket.h>
#अन्तर्भाव <poll.h>
#अन्तर्भाव <signal.h>

#ifdef __cplusplus
बाह्य "C" {
#endif

/**
 * @brief Cooperative read — retries on EAGAIN when user-visible mode is blocking.
 *
 * @param fd    File descriptor to read from.
 * @param buf   Destination buffer.
 * @param count Maximum number of bytes to read.
 * @return Number of bytes read on success, or -1 on error (errno is set).
 */
ssize_t freertos_linux_coop_read(पूर्णाङ्क fd, शून्य *buf, size_t मोजणी);

/**
 * @brief Cooperative write — retries on EAGAIN when user-visible mode is blocking.
 *
 * @param fd    File descriptor to write to.
 * @param buf   Source buffer.
 * @param count Number of bytes to write.
 * @return Number of bytes written on success, or -1 on error (errno is set).
 */
ssize_t freertos_linux_coop_write(पूर्णाङ्क fd, ध्रुव शून्य *buf, size_t मोजणी);

/**
 * @brief Cooperative pread — retries on EAGAIN when user-visible mode is blocking.
 *
 * @param fd     File descriptor to read from.
 * @param buf    Destination buffer.
 * @param count  Maximum number of bytes to read.
 * @param offset File offset to read from.
 * @return Number of bytes read on success, or -1 on error (errno is set).
 */
ssize_t freertos_linux_coop_pread(पूर्णाङ्क fd, शून्य *buf, size_t मोजणी, off_t offset);

/**
 * @brief Cooperative pwrite — retries on EAGAIN when user-visible mode is blocking.
 *
 * @param fd     File descriptor to write to.
 * @param buf    Source buffer.
 * @param count  Number of bytes to write.
 * @param offset File offset to write at.
 * @return Number of bytes written on success, or -1 on error (errno is set).
 */
ssize_t freertos_linux_coop_pwrite(पूर्णाङ्क fd, ध्रुव शून्य *buf, size_t मोजणी, off_t offset);

/**
 * @brief Cooperative readv — retries on EAGAIN when user-visible mode is blocking.
 *
 * @param fd     File descriptor to read from.
 * @param iov    Array of iovec structures describing the buffers.
 * @param iovcnt Number of elements in the iov array.
 * @return Number of bytes read on success, or -1 on error (errno is set).
 */
ssize_t freertos_linux_coop_readv(पूर्णाङ्क fd, ध्रुव संरचना iovec *iov, पूर्णाङ्क iovcnt);

/**
 * @brief Cooperative writev — retries on EAGAIN when user-visible mode is blocking.
 *
 * @param fd     File descriptor to write to.
 * @param iov    Array of iovec structures describing the buffers.
 * @param iovcnt Number of elements in the iov array.
 * @return Number of bytes written on success, or -1 on error (errno is set).
 */
ssize_t freertos_linux_coop_writev(पूर्णाङ्क fd, ध्रुव संरचना iovec *iov, पूर्णाङ्क iovcnt);

/**
 * @brief Cooperative recv — retries on EAGAIN when user-visible mode is blocking.
 *
 * @param sockfd Socket file descriptor.
 * @param buf    Destination buffer.
 * @param len    Maximum number of bytes to receive.
 * @param flags  recv flags (MSG_PEEK, etc.).
 * @return Number of bytes received on success, or -1 on error (errno is set).
 */
ssize_t freertos_linux_coop_recv(पूर्णाङ्क sockfd, शून्य *buf, size_t len, पूर्णाङ्क flags);

/**
 * @brief Cooperative send — retries on EAGAIN when user-visible mode is blocking.
 *
 * @param sockfd Socket file descriptor.
 * @param buf    Source buffer.
 * @param len    Number of bytes to send.
 * @param flags  send flags (MSG_NOSIGNAL, etc.).
 * @return Number of bytes sent on success, or -1 on error (errno is set).
 */
ssize_t freertos_linux_coop_send(पूर्णाङ्क sockfd, ध्रुव शून्य *buf, size_t len, पूर्णाङ्क flags);

/**
 * @brief Cooperative recvfrom — retries on EAGAIN when user-visible mode is blocking.
 *
 * @param sockfd   Socket file descriptor.
 * @param buf      Destination buffer.
 * @param len      Maximum number of bytes to receive.
 * @param flags    recv flags.
 * @param src_addr Source address (may be NULL).
 * @param addrlen  Length of source address (may be NULL).
 * @return Number of bytes received on success, or -1 on error (errno is set).
 */
ssize_t freertos_linux_coop_recvfrom(पूर्णाङ्क sockfd, शून्य *buf, size_t len, पूर्णाङ्क flags,
                                     संरचना sockaddr *src_addr, socklen_t *addrlen);

/**
 * @brief Cooperative sendto — retries on EAGAIN when user-visible mode is blocking.
 *
 * @param sockfd    Socket file descriptor.
 * @param buf       Source buffer.
 * @param len       Number of bytes to send.
 * @param flags     send flags.
 * @param dest_addr Destination address.
 * @param addrlen   Length of destination address.
 * @return Number of bytes sent on success, or -1 on error (errno is set).
 */
ssize_t freertos_linux_coop_sendto(पूर्णाङ्क sockfd, ध्रुव शून्य *buf, size_t len, पूर्णाङ्क flags,
                                   ध्रुव संरचना sockaddr *dest_addr, socklen_t addrlen);

/**
 * @brief Cooperative recvmsg — retries on EAGAIN when user-visible mode is blocking.
 *
 * @param sockfd Socket file descriptor.
 * @param msg    Message header describing buffers and ancillary data.
 * @param flags  recv flags.
 * @return Number of bytes received on success, or -1 on error (errno is set).
 */
ssize_t freertos_linux_coop_recvmsg(पूर्णाङ्क sockfd, संरचना msghdr *संदेश, पूर्णाङ्क flags);

/**
 * @brief Cooperative sendmsg — retries on EAGAIN when user-visible mode is blocking.
 *
 * @param sockfd Socket file descriptor.
 * @param msg    Message header describing buffers and ancillary data.
 * @param flags  send flags.
 * @return Number of bytes sent on success, or -1 on error (errno is set).
 */
ssize_t freertos_linux_coop_sendmsg(पूर्णाङ्क sockfd, ध्रुव संरचना msghdr *संदेश, पूर्णाङ्क flags);

/**
 * @brief Cooperative connect — retries on EINPROGRESS with cooperative yield.
 *
 * @param sockfd  Socket file descriptor.
 * @param addr    Destination address.
 * @param addrlen Length of destination address.
 * @return 0 on success, or -1 on error (errno is set).
 */
पूर्णाङ्क freertos_linux_coop_connect(पूर्णाङ्क sockfd, ध्रुव संरचना sockaddr *addr, socklen_t addrlen);

/**
 * @brief Cooperative accept — retries on EAGAIN when user-visible mode is blocking.
 *
 * Sets the accepted socket to non-blocking and starts tracking it.
 *
 * @param sockfd  Listening socket file descriptor.
 * @param addr    Peer address (may be NULL).
 * @param addrlen Length of peer address (may be NULL).
 * @return New socket file descriptor on success, or -1 on error (errno is set).
 */
पूर्णाङ्क freertos_linux_coop_accept(पूर्णाङ्क sockfd, संरचना sockaddr *addr, socklen_t *addrlen);

/**
 * @brief Cooperative open — sets the new FD to non-blocking and tracks it.
 *
 * @param path  Path of the file to open.
 * @param flags Open flags (O_RDONLY, O_WRONLY, O_CREAT, etc.).
 * @param mode  File creation mode (used when O_CREAT is set).
 * @return File descriptor on success, or -1 on error (errno is set).
 */
पूर्णाङ्क freertos_linux_coop_open(ध्रुव वर्ण *path, पूर्णाङ्क flags, पूर्णाङ्क mode);

/**
 * @brief Cooperative close — untracks the FD, retries on EINTR.
 *
 * @param fd File descriptor to close.
 * @return 0 on success, or -1 on error (errno is set).
 */
पूर्णाङ्क freertos_linux_coop_close(पूर्णाङ्क fd);

/**
 * @brief Cooperative fcntl — intercepts F_GETFL/F_SETFL for shadow mode.
 *
 * @param fd  File descriptor.
 * @param cmd fcntl command (F_GETFL, F_SETFL, etc.).
 * @param arg Command argument.
 * @return Command-dependent value on success, or -1 on error (errno is set).
 */
पूर्णाङ्क freertos_linux_coop_fcntl(पूर्णाङ्क fd, पूर्णाङ्क cmd, पूर्णाङ्क arg);

/**
 * @brief Cooperative socket — sets the new socket to non-blocking and tracks it.
 *
 * @param domain   Communication domain (AF_INET, AF_UNIX, etc.).
 * @param type     Socket type (SOCK_STREAM, SOCK_DGRAM, etc.).
 * @param protocol Protocol number (0 for default).
 * @return Socket file descriptor on success, or -1 on error (errno is set).
 */
पूर्णाङ्क freertos_linux_coop_socket(पूर्णाङ्क domain, पूर्णाङ्क type, पूर्णाङ्क protocol);

/**
 * @brief Cooperative socketpair — sets both sockets to non-blocking and tracks them.
 *
 * @param domain   Communication domain.
 * @param type     Socket type.
 * @param protocol Protocol number.
 * @param sv       Array of two ints to receive the file descriptors.
 * @return 0 on success, or -1 on error (errno is set).
 */
पूर्णाङ्क freertos_linux_coop_socketpair(पूर्णाङ्क domain, पूर्णाङ्क type, पूर्णाङ्क protocol, पूर्णाङ्क *sv);

/**
 * @brief Cooperative pipe — sets both ends to non-blocking and tracks them.
 *
 * @param fds Array of two ints: fds[0] is the read end, fds[1] is the write end.
 * @return 0 on success, or -1 on error (errno is set).
 */
पूर्णाङ्क freertos_linux_coop_pipe(पूर्णाङ्क *fds);

/**
 * @brief Cooperative pipe2 — sets both ends to non-blocking and tracks them.
 *
 * @param fds   Array of two ints: fds[0] is the read end, fds[1] is the write end.
 * @param flags Pipe flags (O_CLOEXEC, O_NONBLOCK, etc.).
 * @return 0 on success, or -1 on error (errno is set).
 */
पूर्णाङ्क freertos_linux_coop_pipe2(पूर्णाङ्क *fds, पूर्णाङ्क flags);

/**
 * @brief Cooperative dup — duplicates a FD, sets the new FD to non-blocking and tracks it.
 *
 * @param oldfd File descriptor to duplicate.
 * @return New file descriptor on success, or -1 on error (errno is set).
 */
पूर्णाङ्क freertos_linux_coop_dup(पूर्णाङ्क oldfd);

/**
 * @brief Cooperative dup2 — duplicates a FD to a specific number, sets non-blocking and tracks it.
 *
 * @param oldfd File descriptor to duplicate.
 * @param newfd Desired file descriptor number.
 * @return New file descriptor on success, or -1 on error (errno is set).
 */
पूर्णाङ्क freertos_linux_coop_dup2(पूर्णाङ्क oldfd, पूर्णाङ्क newfd);

/**
 * @brief Cooperative select — polls with cooperative yield.
 *
 * @param nfds      Highest-numbered file descriptor plus one.
 * @param readfds   Set of file descriptors to watch for readability (may be NULL).
 * @param writefds  Set of file descriptors to watch for writability (may be NULL).
 * @param exceptfds Set of file descriptors to watch for exceptions (may be NULL).
 * @param timeout   Maximum wait time (NULL for infinite).
 * @return Number of ready descriptors, 0 on timeout, or -1 on error (errno is set).
 */
पूर्णाङ्क freertos_linux_coop_select(पूर्णाङ्क nfds, fd_set *readfds, fd_set *writefds,
                               fd_set *exceptfds, संरचना timeval *timeout);

/**
 * @brief Cooperative pselect — polls with cooperative yield and signal mask.
 *
 * @param nfds      Highest-numbered file descriptor plus one.
 * @param readfds   Set of file descriptors to watch for readability (may be NULL).
 * @param writefds  Set of file descriptors to watch for writability (may be NULL).
 * @param exceptfds Set of file descriptors to watch for exceptions (may be NULL).
 * @param timeout   Maximum wait time (NULL for infinite).
 * @param sigmask   Signal mask to apply during the wait (may be NULL).
 * @return Number of ready descriptors, 0 on timeout, or -1 on error (errno is set).
 */
पूर्णाङ्क freertos_linux_coop_pselect(पूर्णाङ्क nfds, fd_set *readfds, fd_set *writefds,
                                fd_set *exceptfds, ध्रुव संरचना timespec *timeout,
                                ध्रुव sigset_t *sigmask);

/**
 * @brief Cooperative poll — polls with cooperative yield.
 *
 * @param fds     Array of pollfd structures describing the FDs to watch.
 * @param nfds    Number of elements in the fds array.
 * @param timeout Timeout in milliseconds (-1 for infinite, 0 for non-blocking).
 * @return Number of ready descriptors, 0 on timeout, or -1 on error (errno is set).
 */
पूर्णाङ्क freertos_linux_coop_poll(संरचना pollfd *fds, nfds_t nfds, पूर्णाङ्क timeout);

/**
 * @brief Set stdin/stdout/stderr to non-blocking and start tracking them.
 *
 * Must be called early during system init.
 */
शून्य freertos_linux_coop_syscalls_init(शून्य);

#ifdef __cplusplus
}
#endif
