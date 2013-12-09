#include <sys/socket.h>
#include <linux/un.h>
#include <string.h>
#include <stdio.h>
#include "slim_socket.h"
#include "protocol/slim.pb-c.h"

   // LooperMessage* msg;


Socket* slim_socket_new(void)
{
    Socket* self = (Socket *)malloc(sizeof(Socket));
    struct sockaddr_un address;// = self->address;

    self->fd = socket (AF_UNIX, SOCK_STREAM, 0);

    if (self->fd < 0)
    {
        fprintf(stderr, "socket() failed\r\n");
        return NULL;
    }
    unlink("/tmp/slim_socket");
    memset(&address, 0, sizeof(struct sockaddr_un));
    address.sun_family = AF_UNIX;
    snprintf(address.sun_path, UNIX_PATH_MAX, "/tmp/slim_socket");

    if (bind(self->fd, (struct sockaddr *) &address, sizeof(struct sockaddr_un)) != 0)
    {
        fprintf(stderr,"bind() failed \r\n");
        return NULL;
    }
    if(listen(self->fd, 5) != 0) 
    {
        fprintf(stderr, "listen() failed\n");
        return NULL;
    }

    int address_length = sizeof(address);
    self->connection_fd = accept(self->fd, 
                    (struct sockaddr*) &address, 
                    &address_length);

    if (self->connection_fd == -1)
    {
        fprintf(stderr,"ERROR accepting socket connection");
        return NULL;
    }

    printf("accepted connection\r\n");
    return self;
}

void slim_socket_close(Socket* self)
{
    close(self->fd);
    close(self->connection_fd);
}
