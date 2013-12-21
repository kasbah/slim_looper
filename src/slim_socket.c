#include <linux/un.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "slim_socket.h"

SlimSocket* slim_socket_server_new(void)
{
    SlimSocket* self = slim_socket_new(); 
    self->listen_fd = socket (AF_UNIX, SOCK_STREAM | SOCK_NONBLOCK, 0);

    if (self->listen_fd < 0)
    {
        perror("ERROR socket() failed");
        return NULL;
    }

    unlink("/tmp/slim_socket");
    if (bind(self->listen_fd, (struct sockaddr *) self->address, sizeof(struct sockaddr_un)) != 0)
    {
        perror("ERROR bind() failed");
        return NULL;
    }

    if(listen(self->listen_fd, 5) != 0) 
    {
        perror("ERROR listen() failed");
        return NULL;
    }

    return self;
}

int slim_socket_server_read(SlimSocket* self, char* msg_buffer)
{
    int n = -1;
    int read_fd = accept4(self->listen_fd, self->address, &self->address_length, SOCK_NONBLOCK);
    if(read_fd > 0)
        n = recv(read_fd, msg_buffer, 255, 0);
    close(read_fd);
    return n;
}

SlimSocket* slim_socket_new(void)
{
    SlimSocket* self = (SlimSocket *)malloc(sizeof(SlimSocket));
    self->address = (struct sockaddr_un*)malloc(sizeof(struct sockaddr_un));

    memset(self->address, 0, sizeof(struct sockaddr_un));
    self->address->sun_family = AF_UNIX;
    snprintf(self->address->sun_path, UNIX_PATH_MAX, "/tmp/slim_socket");

    self->address_length = sizeof(*(self->address));

    return self;
}

int slim_socket_read(SlimSocket* self, char* msg_buffer)
{
    int read_fd = socket (AF_UNIX, SOCK_STREAM, 0);
    int n = connect(read_fd, self->address, self->address_length);
    n = recv(read_fd, msg_buffer, 255, 0);
    close(read_fd);
    return n;
}

void slim_socket_free(SlimSocket* self)
{
    close(self->listen_fd);
    free(self);
}
