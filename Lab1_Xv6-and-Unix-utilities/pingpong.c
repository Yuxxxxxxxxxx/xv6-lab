/**
 * FileName:      pingpong.c
 * Author:        Yuxxxxxxxxxx
 * Version :      V1.0
 * Date:          2024.7.30
 * Description:  Parent and child processes communicate via pipes.
 * CopyRight (c)  2023-2024 All Right Reseverd
 */
#include <user/user.h>

#define RD 0 // pipe 的 read 端
#define WR 1 // pipe 的 write 端

int
main (int argc, char *argv[])
{
    char buf = 'P';

    int fd_p2c[2]; // 父进程->子进程
    int fd_c2p[2]; // 子进程->父进程
    pipe(fd_p2c);
    pipe(fd_c2p);

    int pid = fork();
    int exit_status = 0;

    if(pid < 0) {
        fprintf(2, "fork() error!\n");

        close(fd_p2c[RD]);
        close(fd_p2c[WR]);
        close(fd_c2p[RD]);
        close(fd_c2p[WR]);

        exit(1);
    } else if (pid == 0) { // 子进程
        // 先从父进程读，再向父进程写入
        // 所以关掉父进程的写端和子进程的读端
        close(fd_p2c[WR]);
        close(fd_c2p[RD]);

        if (read(fd_p2c[RD], &buf, sizeof(char)) != sizeof(char)) {
            fprintf(2, "child read() error!\n");
	    exit_status = 1;
        } else {
            fprintf(1, "%d: received ping\n", getpid());
        }

        if (write(fd_c2p[WR], &buf, sizeof(char)) != sizeof(char)) {
            fprintf(2, "write() error!\n");
            exit_status = 1;
        }

        close(fd_p2c[RD]);
        close(fd_c2p[WR]);

        exit(exit_status);
    } else { // 父进程
        // 先向子进程写，再从子进程读
        // 所以关掉父进程的读端和子进程的写端
        close(fd_p2c[RD]);
        close(fd_c2p[WR]);

        if (write(fd_p2c[WR], &buf, sizeof(char)) != sizeof(char)) {
            fprintf(2, "write() error!\n");
            exit_status = 1;
        }

        if (read(fd_c2p[RD], &buf, sizeof(char)) != sizeof(char)) {
            fprintf(2, "read() error!\n");
            exit_status = 1;
        } else {
            fprintf(1, "%d: received pong\n", getpid());
        }

        close(fd_p2c[WR]);
	    close(fd_c2p[RD]);

        exit(exit_status);
    }

    exit(0);
}