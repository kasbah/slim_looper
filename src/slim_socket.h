#ifndef __SOCKET_H__
#define __SOCKET_H__

#include <sys/types.h>
#define _GNU_SOURCE
#include <sys/socket.h>
#include <stddef.h>

#define MSG_BUFFER_SIZE 256

typedef struct {
    int listen_fd;
    int comm_fd;
    struct sockaddr_un* address;
    int address_length;
} SlimSocket;

SlimSocket* slim_socket_server_new(void);
SlimSocket* slim_socket_new(void);
//int slim_socket_read(SlimSocket* self, char* msg_buffer);
int slim_socket_server_read(SlimSocket* self, char* msg_buffer);
void slim_socket_free(SlimSocket* self);

#endif// __SOCKET_H__
