#include <sys/socket.h>
#include <linux/un.h>
#include <string.h>
#include <stdio.h>
#include "socket.h"
#include "gui/slim.pb-c.h"
int socket_run(void)
{
    LooperCommandMessage* msg;
    struct sockaddr_un address;
    int socket_fd, connection_fd;
    socklen_t address_length;
    pid_t child;

    char buffer[256];
    int n;

    socket_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (socket_fd < 0)
    {
        printf("socket() failed\r\n");
        return 1;
    }

    unlink("/tmp/slim_socket");

    memset(&address, 0, sizeof(struct sockaddr_un));

    address.sun_family = AF_UNIX;
    snprintf(address.sun_path, UNIX_PATH_MAX, "/tmp/slim_socket");

    if (bind(socket_fd, (struct sockaddr *) &address, sizeof(struct sockaddr_un)) != 0)
    {
        printf("bind() failed \r\n");
        return 1;
    }

    if(listen(socket_fd, 5) != 0) 
    {
        printf("listen() failed\n");
        return 1;
    }

    address_length = sizeof(address);

    while((connection_fd = accept(socket_fd, 
                    (struct sockaddr*) &address, 
                    &address_length)) > -1)
    {
        printf("successfully received\r\n");
        bzero(buffer, 256);
        n = read(connection_fd, buffer, 255);
        if (n < 0) error("ERROR reading from socket");
        printf("message: %s\r\n", buffer);
        msg = looper_command_message__unpack(NULL, sizeof(buffer), buffer);
        printf("command: %s\r\n", looper_command__descriptor.values[msg->command].name);
        //printf("msg->command: %s\r\n",
        //n = write(connection_fd, buffer, strlen(buffer));
        //if (n < 0) 
        //    printf("ERROR writing to socket\n");           
        break;
    }
    close(socket_fd);
    close(connection_fd);
}
