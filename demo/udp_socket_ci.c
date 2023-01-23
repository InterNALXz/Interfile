#include "stdio.h"
#include "sys/types.h"
#include "sys/socket.h"
#include "linux/in.h"
#include "sys/un.h"
#include "errno.h"
#include "unistd.h"

typedef struct so_fun_i {
    void (*func)(void *arg);
    int i;
}so_fun;

void use_create(void *arg) {
    printf("we are in use!\n");
}

#define SIZE 125

int main(int argc, char const *argv[])
{
    int fd = socket(AF_UNIX, SOCK_DGRAM, 0);

    struct sockaddr_un srvaddr;
    socklen_t len = sizeof(srvaddr);
    bzero(&srvaddr, len);

    srvaddr.sun_family = AF_UNIX;
    strncpy(srvaddr.sun_path, "/data/sock/io_sock", 19);

    char buf[SIZE];

    so_fun jet = {0};
    jet.func = &use_create;

    while(1)
    {
        bzero(buf, SIZE);
        fgets(buf, SIZE, stdin);

        int sz = sendto(fd, (void *)&jet, sizeof(so_fun), 0,
        // int sz = sendto(fd, buf, strlen(buf), 0,
                (struct sockaddr *)&srvaddr, len);

        printf("local is %p\n", jet.func);
        if (sz < 0)    {
            // if (errno == ECONNREFUSED)
            printf("not send;err[%d]:%s\n", errno, strerror(errno));
        }
        sleep(10);
    }

    return 0;
}