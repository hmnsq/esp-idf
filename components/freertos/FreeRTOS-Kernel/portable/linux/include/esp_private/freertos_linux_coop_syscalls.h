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

#அந்தர்பாவ <stddef.h>
#அந்தர்பாவ <sys/types.h>
#அந்தர்பாவ <sys/select.h>
#அந்தர்பாவ <sys/time.h>
#அந்தர்பாவ <sys/uio.h>
#அந்தர்பாவ <sys/socket.h>
#அந்தர்பாவ <poll.h>
#அந்தர்பாவ <signal.h>

#ifdef __cplusplus
வெளி "C" {
#endif

/**
 * @brief Cooperative read — retries on EAGAIN when user-visible mode is blocking.
 *
 * @param fd    File descriptor to read from.
 * @param buf   Destination buffer.
 * @param count Maximum number of bytes to read.
 * @return Number of bytes read on success, or -1 on error (errno is set).
 */
ssize_t freertos_linux_coop_read(முழு fd, வெறுமை *buf, size_t மொத்தம்);

/**
 * @brief Cooperative write — retries on EAGAIN when user-visible mode is blocking.
 *
 * @param fd    File descriptor to write to.
 * @param buf   Source buffer.
 * @param count Number of bytes to write.
 * @return Number of bytes written on success, or -1 on error (errno is set).
 */
ssize_t freertos_linux_coop_write(முழு fd, மாறிலி வெறுமை *buf, size_t மொத்தம்);

/**
 * @brief Cooperative pread — retries on EAGAIN when user-visible mode is blocking.
 *
 * @param fd     File descriptor to read from.
 * @param buf    Destination buffer.
 * @param count  Maximum number of bytes to read.
 * @param offset File offset to read from.
 * @return Number of bytes read on success, or -1 on error (errno is set).
 */
ssize_t freertos_linux_coop_pread(முழு fd, வெறுமை *buf, size_t மொத்தம், off_t offset);

/**
 * @brief Cooperative pwrite — retries on EAGAIN when user-visible mode is blocking.
 *
 * @param fd     File descriptor to write to.
 * @param buf    Source buffer.
 * @param count  Number of bytes to write.
 * @param offset File offset to write at.
 * @return Number of bytes written on success, or -1 on error (errno is set).
 */
ssize_t freertos_linux_coop_pwrite(முழு fd, மாறிலி வெறுமை *buf, size_t மொத்தம், off_t offset);

/**
 * @brief Cooperative readv — retries on EAGAIN when user-visible mode is blocking.
 *
 * @param fd     File descriptor to read from.
 * @param iov    Array of iovec structures describing the buffers.
 * @param iovcnt Number of elements in the iov array.
 * @return Number of bytes read on success, or -1 on error (errno is set).
 */
ssize_t freertos_linux_coop_readv(முழு fd, மாறிலி கட்டமைப்பு iovec *iov, முழு iovcnt);

/**
 * @brief Cooperative writev — retries on EAGAIN when user-visible mode is blocking.
 *
 * @param fd     File descriptor to write to.
 * @param iov    Array of iovec structures describing the buffers.
 * @param iovcnt Number of elements in the iov array.
 * @return Number of bytes written on success, or -1 on error (errno is set).
 */
ssize_t freertos_linux_coop_writev(முழு fd, மாறிலி கட்டமைப்பு iovec *iov, முழு iovcnt);

/**
 * @brief Cooperative recv — retries on EAGAIN when user-visible mode is blocking.
 *
 * @param sockfd Socket file descriptor.
 * @param buf    Destination buffer.
 * @param len    Maximum number of bytes to receive.
 * @param flags  recv flags (MSG_PEEK, etc.).
 * @return Number of bytes received on success, or -1 on error (errno is set).
 */
ssize_t freertos_linux_coop_recv(முழு sockfd, வெறுமை *buf, size_t len, முழு flags);

/**
 * @brief Cooperative send — retries on EAGAIN when user-visible mode is blocking.
 *
 * @param sockfd Socket file descriptor.
 * @param buf    Source buffer.
 * @param len    Number of bytes to send.
 * @param flags  send flags (MSG_NOSIGNAL, etc.).
 * @return Number of bytes sent on success, or -1 on error (errno is set).
 */
ssize_t freertos_linux_coop_send(முழு sockfd, மாறிலி வெறுமை *buf, size_t len, முழு flags);

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
ssize_t freertos_linux_coop_recvfrom(முழு sockfd, வெறுமை *buf, size_t len, முழு flags,
                                     கட்டமைப்பு sockaddr *src_addr, socklen_t *addrlen);

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
ssize_t freertos_linux_coop_sendto(முழு sockfd, மாறிலி வெறுமை *buf, size_t len, முழு flags,
                                   மாறிலி கட்டமைப்பு sockaddr *dest_addr, socklen_t addrlen);

/**
 * @brief Cooperative recvmsg — retries on EAGAIN when user-visible mode is blocking.
 *
 * @param sockfd Socket file descriptor.
 * @param msg    Message header describing buffers and ancillary data.
 * @param flags  recv flags.
 * @return Number of bytes received on success, or -1 on error (errno is set).
 */
ssize_t freertos_linux_coop_recvmsg(முழு sockfd, கட்டமைப்பு msghdr *செய்தி, முழு flags);

/**
 * @brief Cooperative sendmsg — retries on EAGAIN when user-visible mode is blocking.
 *
 * @param sockfd Socket file descriptor.
 * @param msg    Message header describing buffers and ancillary data.
 * @param flags  send flags.
 * @return Number of bytes sent on success, or -1 on error (errno is set).
 */
ssize_t freertos_linux_coop_sendmsg(முழு sockfd, மாறிலி கட்டமைப்பு msghdr *செய்தி, முழு flags);

/**
 * @brief Cooperative connect — retries on EINPROGRESS with cooperative yield.
 *
 * @param sockfd  Socket file descriptor.
 * @param addr    Destination address.
 * @param addrlen Length of destination address.
 * @return 0 on success, or -1 on error (errno is set).
 */
முழு freertos_linux_coop_connect(முழு sockfd, மாறிலி கட்டமைப்பு sockaddr *addr, socklen_t addrlen);

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
முழு freertos_linux_coop_accept(முழு sockfd, கட்டமைப்பு sockaddr *addr, socklen_t *addrlen);

/**
 * @brief Cooperative open — sets the new FD to non-blocking and tracks it.
 *
 * @param path  Path of the file to open.
 * @param flags Open flags (O_RDONLY, O_WRONLY, O_CREAT, etc.).
 * @param mode  File creation mode (used when O_CREAT is set).
 * @return File descriptor on success, or -1 on error (errno is set).
 */
முழு freertos_linux_coop_open(மாறிலி எழுத்து *path, முழு flags, முழு mode);

/**
 * @brief Cooperative close — untracks the FD, retries on EINTR.
 *
 * @param fd File descriptor to close.
 * @return 0 on success, or -1 on error (errno is set).
 */
முழு freertos_linux_coop_close(முழு fd);

/**
 * @brief Cooperative fcntl — intercepts F_GETFL/F_SETFL for shadow mode.
 *
 * @param fd  File descriptor.
 * @param cmd fcntl command (F_GETFL, F_SETFL, etc.).
 * @param arg Command argument.
 * @return Command-dependent value on success, or -1 on error (errno is set).
 */
முழு freertos_linux_coop_fcntl(முழு fd, முழு cmd, முழு arg);

/**
 * @brief Cooperative socket — sets the new socket to non-blocking and tracks it.
 *
 * @param domain   Communication domain (AF_INET, AF_UNIX, etc.).
 * @param type     Socket type (SOCK_STREAM, SOCK_DGRAM, etc.).
 * @param protocol Protocol number (0 for default).
 * @return Socket file descriptor on success, or -1 on error (errno is set).
 */
முழு freertos_linux_coop_socket(முழு domain, முழு type, முழு protocol);

/**
 * @brief Cooperative socketpair — sets both sockets to non-blocking and tracks them.
 *
 * @param domain   Communication domain.
 * @param type     Socket type.
 * @param protocol Protocol number.
 * @param sv       Array of two ints to receive the file descriptors.
 * @return 0 on success, or -1 on error (errno is set).
 */
முழு freertos_linux_coop_socketpair(முழு domain, முழு type, முழு protocol, முழு *sv);

/**
 * @brief Cooperative pipe — sets both ends to non-blocking and tracks them.
 *
 * @param fds Array of two ints: fds[0] is the read end, fds[1] is the write end.
 * @return 0 on success, or -1 on error (errno is set).
 */
முழு freertos_linux_coop_pipe(முழு *fds);

/**
 * @brief Cooperative pipe2 — sets both ends to non-blocking and tracks them.
 *
 * @param fds   Array of two ints: fds[0] is the read end, fds[1] is the write end.
 * @param flags Pipe flags (O_CLOEXEC, O_NONBLOCK, etc.).
 * @return 0 on success, or -1 on error (errno is set).
 */
முழு freertos_linux_coop_pipe2(முழு *fds, முழு flags);

/**
 * @brief Cooperative dup — duplicates a FD, sets the new FD to non-blocking and tracks it.
 *
 * @param oldfd File descriptor to duplicate.
 * @return New file descriptor on success, or -1 on error (errno is set).
 */
முழு freertos_linux_coop_dup(முழு oldfd);

/**
 * @brief Cooperative dup2 — duplicates a FD to a specific number, sets non-blocking and tracks it.
 *
 * @param oldfd File descriptor to duplicate.
 * @param newfd Desired file descriptor number.
 * @return New file descriptor on success, or -1 on error (errno is set).
 */
முழு freertos_linux_coop_dup2(முழு oldfd, முழு newfd);

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
முழு freertos_linux_coop_select(முழு nfds, fd_set *readfds, fd_set *writefds,
                               fd_set *exceptfds, கட்டமைப்பு timeval *timeout);

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
முழு freertos_linux_coop_pselect(முழு nfds, fd_set *readfds, fd_set *writefds,
                                fd_set *exceptfds, மாறிலி கட்டமைப்பு timespec *timeout,
                                மாறிலி sigset_t *sigmask);

/**
 * @brief Cooperative poll — polls with cooperative yield.
 *
 * @param fds     Array of pollfd structures describing the FDs to watch.
 * @param nfds    Number of elements in the fds array.
 * @param timeout Timeout in milliseconds (-1 for infinite, 0 for non-blocking).
 * @return Number of ready descriptors, 0 on timeout, or -1 on error (errno is set).
 */
முழு freertos_linux_coop_poll(கட்டமைப்பு pollfd *fds, nfds_t nfds, முழு timeout);

/**
 * @brief Set stdin/stdout/stderr to non-blocking and start tracking them.
 *
 * Must be called early during system init.
 */
வெறுமை freertos_linux_coop_syscalls_init(வெறுமை);

#ifdef __cplusplus
}
#endif
