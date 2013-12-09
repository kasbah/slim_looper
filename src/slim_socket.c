#include <sys/socket.h>
#include <linux/un.h>
#include <string.h>
#include <stdio.h>
#include "slim_socket.h"
#include "protocol/slim.pb-c.h"
int slim_socket_run(void)
{
    LooperMessage* msg;
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
    connection_fd = accept(socket_fd, 
                    (struct sockaddr*) &address, 
                    &address_length);

    if (connection_fd == -1)
    {
        perror("ERROR accepting socket connection");
    }

    printf("accepted connection\r\n");

    while(1)
    {
        memset(buffer, 0, sizeof(char) * 256);
        n = read(connection_fd, buffer, 255);
        if (n < 0) perror("ERROR reading from socket");
        else if (n > 0)
        {
            msg = looper_message__unpack(NULL, n, buffer);
            if (msg > 0)
            {
                switch(msg->type)
                {
                    case MESSAGE_TYPE__COMMAND:
                        printf ("command: %i\r\n", msg->command->value);
                        break;
                    case MESSAGE_TYPE__SETTING:
                        printf ("setting dry: %f\r\n", msg->setting->dry);
                        break;
                }
            }
            else if (msg < 0)
            {
                perror("fuck");
            }
        }
        //n = write(connection_fd, buffer, strlen(buffer));
        //if (n < 0) 
        //    printf("ERROR writing to socket\n");           
        //break;
    }
    close(socket_fd);
    close(connection_fd);
}
