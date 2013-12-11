#ifndef __SOCKET_H__
#define __SOCKET_H__

#include <sys/types.h>
#include <sys/socket.h>

typedef struct {
    int listen_fd;
    struct sockaddr_un* address;
    int address_length;
} SlimSocket;

SlimSocket* slim_socket_server_new(void);
SlimSocket* slim_socket_new(void);
int slim_socket_read(SlimSocket* self, char* msg_buffer);

#endif// __SOCKET_H__
