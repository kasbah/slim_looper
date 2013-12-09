#ifndef __SOCKET_H__
#define __SOCKET_H__

typedef struct {
    int fd;
    int connection_fd;
} Socket;

Socket* slim_socket_new(void);
void slim_socket_close(Socket* self);

#endif// __SOCKET_H__
