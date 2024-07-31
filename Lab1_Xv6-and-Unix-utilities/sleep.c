/**
 * FileName:      sleep.c
 * Author:        Yuxxxxxxxxxx
 * Version :      V1.0
 * Date:          2024.7.30
 * Description:   The sleep command pauses for a user-specified number of ticks by invoking the sleep system call.
 * CopyRight (c)  2023-2024 All Right Reseverd
 */
#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
    if(argc < 2){
        fprintf(2, "Usage: sleep num\n");
        exit(1);
    }

    sleep(atoi(argv[1]));

    exit(0);
}