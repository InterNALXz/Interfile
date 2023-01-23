#include "stdio.h"
#include "sys/types.h"
#include "sys/socket.h"
#include "linux/in.h"
#include "sys/un.h"
#include "signal.h"
#include <fcntl.h>
#include <unistd.h>

#define I_PATH  "/data/sock/io_sock"
#define SIZE 125
static int fd = 0;
static int kill_1 = 0;

typedef struct so_fun_i {
    void (*func)(void *arg);
    int i;
}so_fun;

void signal_handle(int signum) {
    if (fd != 0)
        close(fd);
    kill_1 = 1;
    printf("over\n");
}

int main(int argc, char const *argv[])
{
    signal(SIGINT, signal_handle);
    fd = socket(AF_UNIX, SOCK_DGRAM, 0);

    struct sockaddr_un srvaddr;
    socklen_t len = sizeof(srvaddr);
    bzero(&srvaddr, len);


    srvaddr.sun_family = AF_UNIX;
    
    strncpy(srvaddr.sun_path, I_PATH, 19);

    if (access(srvaddr.sun_path,0) != -1)
    {
        remove(srvaddr.sun_path);
    }
    bind(fd, (const struct sockaddr *)&srvaddr, len);
    char buf[SIZE];
    so_fun link_i = {0};

    while(kill_1 != 1)
    {
        bzero(buf, SIZE);

        // int sz = recvfrom(fd, buf, SIZE, MSG_DONTWAIT, (struct sockaddr *)&srvaddr, &len);

        // if (sz > 0)
        //     printf("buf is %s\n", buf);

        int sz = recvfrom(fd, (void *)&link_i, sizeof(so_fun), MSG_DONTWAIT, (struct sockaddr *)&srvaddr, &len);

        if (link_i.func != NULL)
            printf("p is %p\n", link_i.func);
            // link_i.func(&buf);
    
        sleep(1);
    }


    return 0;
}