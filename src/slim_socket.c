#include <linux/un.h>
#include <string.h>
#include <stdio.h>
#include "slim_socket.h"
#include "protocol/slim.pb-c.h"
#include <errno.h>

   // LooperMessage* msg;


Socket* slim_socket_new(void)
{
    Socket* self = (Socket *)malloc(sizeof(Socket));
    self->address = (struct sockaddr_un*)malloc(sizeof(struct sockaddr_un));

    self->fd = socket (AF_UNIX, SOCK_STREAM, 0);

    if (self->fd < 0)
    {
        fprintf(stderr, "socket() failed\r\n");
        return NULL;
    }
    unlink("/tmp/slim_socket");
    memset(self->address, 0, sizeof(struct sockaddr_un));
    self->address->sun_family = AF_UNIX;
    snprintf(self->address->sun_path, UNIX_PATH_MAX, "/tmp/slim_socket");

    if (bind(self->fd, (struct sockaddr *) self->address, sizeof(struct sockaddr_un)) != 0)
    {
        fprintf(stderr,"bind() failed \r\n");
        return NULL;
    }
    if(listen(self->fd, 5) != 0) 
    {
        fprintf(stderr, "listen() failed\n");
        return NULL;
    }

    self->address_length = sizeof(*(self->address));

    return self;
}


int slim_socket_read(Socket* self, char* msg_buffer)
{
    int connection_fd = accept(self->fd, 
                    (struct sockaddr*) self->address, 
                    self->address_length);

    printf("fd: %i\r\n", connection_fd);

    if (connection_fd > 0)//connection_fd != EWOULDBLOCK && connection_fd != EAGAIN)
    {
        printf("accepted connection\r\n");
        memset(msg_buffer, 0, sizeof(char) * 256);
        int n = recv(connection_fd, msg_buffer, 255, 0);
        close(connection_fd);
        return n;
    }
    else
        return 0;
}

void slim_socket_close(Socket* self)
{
    close(self->fd);
}
