/**
 * FileName:      xargs.c
 * Author:        Yuxxxxxxxxxx
 * Version :      V1.0
 * Date:          2024.7.31
 * Description:   Reads line by line from standard input and executes a command 
                  for each line, passing the line as an argument to the command.
 * CopyRight (c)  2023-2024 All Right Reseverd
 */
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

// �������б�ִ��ĳ������
void
run(char* program, char** args)
{
    if (fork() == 0) {
        exec(program, args);
        exit(0);
    }

    return;
}

int
main(int argc, char* argv[])
{
    char buf[2048];                // ����ʱʹ�õ��ڴ��
    char* p = buf, * last_p = buf; // ��ǰ�����Ľ�������ʼָ��
    char* argsbuf[128];            // ȫ�������б��ַ���ָ�����飬���� argv ���� ���Ĳ����� stdin ����Ĳ���
    char** args = argsbuf;         // ָ�� argsbuf �е�һ���� stdin ����Ĳ���

    for (int i = 1; i < argc; ++i) { // �� argv �ṩ�Ĳ������뵽���յĲ����б���
        *args++ = argv[i];
    }

    char** pa = args; // ��ʼ�������
    while (read(0, p, 1) != 0) {
        if (*p == '\n' || *p == ' ') {
            // ����һ���������
            // �Կո�ָ����� `echo hello world`���� hello �� world ��Ϊһ������
            *p = '\0';
            *(pa++) = last_p;
            last_p = p + 1;

            if (*p == '\n') {
                // ����һ�����
                *p = 0;                // �����б�ĩβ�� null ��ʶ�б����
                run(argv[1], argsbuf); // ִ�����һ��ָ��
                pa = args;             // ���ö������ָ�룬׼��������һ��
            }
        }

        p++;
    }

    if (pa != args) { // ������һ�в��ǿ���
        *p = '\0';
        *(pa++) = last_p;
        *pa = 0;
        run(argv[1], argsbuf);
    }

    while (wait(0) != -1); // ѭ���ȴ������ӽ�����ɣ�ÿһ�� wait(0) �ȴ�һ��

    exit(0);
}