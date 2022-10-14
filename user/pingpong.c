#include "kernel/types.h"
#include "user/user.h"

int Fork();
int Read();
int Write();

int main(int argc, char *argv[])
{
    int p[2];
    char buf[1];

    pipe(p);

    if (0 == Fork())
    {
        // Child
        if (1 == Read(p[0], buf, 1))
        {
            fprintf(1, "%d: received ping\n", getpid());
            // fprintf(1, "buf: %c\n", *buf);
        }
        close(p[0]);

        Write(p[1], "C", 1);
        close(p[1]);
    }
    else
    {
        // Parent
        Write(p[1], "P", 1);
        close(p[1]);
        wait(0);

        if (1 == Read(p[0], buf, 1))
        {
            fprintf(1, "%d: received pong\n", getpid());
            // fprintf(1, "buf: %c\n", *buf);
        }
        close(p[0]);
    }
    exit(0);
}

int Fork()
{
    int rt = fork();
    if (rt < 0)
    {
        fprintf(2, "fork error\n");
        exit(1);
    }

    return rt;
}

int Read(int fd, char *buf, int n)
{
    int rt = read(fd, buf, n);
    if (rt < 0)
    {
        fprintf(2, "read error\n");
        exit(1);
    }
    return rt;
}

int Write(int fd, char *buf, int n)
{
    int rt = write(fd, buf, n);
    if (rt < 0)
    {
        fprintf(2, "write error\n");
        exit(1);
    }
    return rt;
}