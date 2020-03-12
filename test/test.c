#include <sys/types.h>
#include <linux/input.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>


static int getTouchEventNum()
{
        char          name[64];           /* RATS: Use ok, but could be better */
        char          buf[256] = { 0, };  /* RATS: Use ok */
        int           fd = 0;
        int           i;
        for (i = 0; i < 32; i++)
        {
                sprintf(name, "/dev/input/event%d", i);
                if ((fd = open(name, O_RDONLY, 0)) >= 0)
                {
                        ioctl(fd, EVIOCGNAME(sizeof(buf)), buf);
                        printf("/dev/input/event%d :name: %s\n", i, buf);
                        close(fd);
                }
        }
        return 0;
}

int main(int argc, char** argv)
{
    printf("main start\n");
    getTouchEventNum();
    return 0;
}

