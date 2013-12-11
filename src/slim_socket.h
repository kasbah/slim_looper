#ifndef __SOCKET_H__
#define __SOCKET_H__

#include <sys/types.h>
#include <sys/socket.h>

typedef struct {
    int fd;
    struct sockaddr_un* address;
    int address_length;
} Socket;

Socket* slim_socket_new(void);
int slim_socket_read(Socket* self, char* msg_buffer);
void slim_socket_close(Socket* self);

#endif// __SOCKET_H__
