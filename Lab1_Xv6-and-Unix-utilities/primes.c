/**
 * FileName:      primes.c
 * Author:        Yuxxxxxxxxxx
 * Version :      V1.0
 * Date:          2024.7.31
 * Description:   The pipeline and multi-process are used to filter the prime numbers.
 * CopyRight (c)  2023-2024 All Right Reseverd
 */
#include <kernel/types.h>
#include <user/user.h>

#define RD 0 // pipe 管道读端
#define WR 1 // pipe 管道写端

const uint INT_SIZE = sizeof(int);

/*
 * @brief 筛选素数
 * @param lpipe 左邻居管道
 */
void
primes(int lpipe[2])
{
    int first;
    read(lpipe[RD], &first, INT_SIZE); // 读第一个数，必然是素数
    if (first == -1) // 如果为 -1 结尾标志，则退出
        exit(-1);

    printf("prime %d\n", first);

    int rpipe[2]; // 用于下个阶段的进程的输出管道
    pipe(rpipe);

    if (fork() == 0) {
        close(rpipe[WR]); // 子进程只需要对输入管道 rpipe 进行读，而不需要写
        close(lpipe[RD]); // 这是父进程的输入管道，用不到

        primes(rpipe);    // 子进程以父进程的输出管道作为输入，开始进行下一个阶段的处理
    } else {
        close(rpipe[RD]); // 父进程只需要对子进程的输入管道进行写而不需要读，所以关掉父进程的读文件描述符
        int data;
        // 从左管道读取数据
	while(read(lpipe[RD], &data, INT_SIZE) == INT_SIZE && data != -1)
            if (data % first) // 将无法整除的数据传递入右管道
                write(rpipe[WR], &data, INT_SIZE);

        data = -1;
        write(rpipe[WR], &data, INT_SIZE);
        wait(0);
    }

    exit(0);
}

int
main(int argc, char *argv[])
{
    int p[2];
    pipe(p);

    if (fork() == 0) {
        close(p[WR]);
        primes(p);
    } else {
        close(p[RD]);

        int i;
        for (i = 2; i <= 35; ++i)
            write(p[WR], &i, INT_SIZE);
        i = -1;
        write(p[WR], &i, INT_SIZE); // 将 -1 作为结尾标志

        wait(0);
    }

    exit(0);
}