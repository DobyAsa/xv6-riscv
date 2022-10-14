#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

int Open(char *, int);
int Fstat(int, struct stat *);
int find(char *, char *);

int main(int argc, char *argv[])
{
    find(argv[1], argv[2]);
    exit(0);
}

int find(char *path, char *name)
{
    char buf[512], *p;

    struct dirent de;
    struct stat st;

    int fd = Open(path, 0);
    Fstat(fd, &st);

    if (st.type != T_DIR)
    {
        fprintf(2, "path shuold be a directory\n");
        exit(1);
    }
    if (strlen(path) + 1 + DIRSIZ + 1 > sizeof buf)
    {
        printf("ls: path too long\n");
        exit(1);
    }
    strcpy(buf, path);
    p = buf + strlen(buf);
    *p++ = '/';
    while (sizeof(de) == read(fd, &de, sizeof(de)))
    {
        if (de.inum == 0)
        {
            continue;
        }
        memmove(p, de.name, DIRSIZ);
        p[DIRSIZ] = 0;
        stat(buf, &st);
        if (st.type != T_DIR)
        {
            if (strcmp(name, de.name) == 0)
            {
                fprintf(1, "%s\n", buf);
            }
        }
        else if (strcmp(de.name, ".") != 0 && strcmp(de.name, "..") != 0)
        {
            find(buf, name);
        }
    }
    close(fd);

    return 0;
}

int Open(char *file, int flag)
{
    int rt = open(file, flag);
    if (rt < 0)
    {
        fprintf(2, "Can't open %s\n", file);
        exit(1);
    }
    return rt;
}

int Fstat(int fd, struct stat *st)
{
    int rt = fstat(fd, st);
    if (rt < 0)
    {
        fprintf(2, "Can't fstat\n");
        exit(1);
    }
    return rt;
}