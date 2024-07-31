/**
 * FileName:      find.c
 * Author:        Yuxxxxxxxxxx
 * Version :      V1.0
 * Date:          2024.7.31
 * Description:   Find all files with a specific name in the directory tree.
 * CopyRight (c)  2023-2024 All Right Reseverd
 */
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void
find(char *path, const char *filename)
{
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    if ((fd = open(path, 0)) < 0) {
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }

    if (fstat(fd, &st) < 0) {
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        return;
    }

    // find 的第一个参数必须是目录
    if (st.type != T_DIR) {
        fprintf(2, "usage: find <path> <file>\n");
        return;
    }

    // 防止缓冲区溢出
    if (strlen(path) + 1 + DIRSIZ + 1 > sizeof(buf)) {
        fprintf(2, "find: path too long\n");
        return;
    }

    strcpy(buf, path);
    p = buf + strlen(buf);
    *p++ = '/'; // p指向最后一个'/'之后

    while (read(fd, &de, sizeof(de)) == sizeof(de)) {
	if (de.inum == 0)
            continue;
        memmove(p, de.name, DIRSIZ); //添加路径名称
        p[DIRSIZ] = 0;               //字符串结束标志

        if (stat(buf, &st) < 0){
            fprintf(2, "find: cannot stat %s\n", buf);
            continue;
        }

        // 不能在“.”和“..”目录中递归
        if (st.type == T_DIR
                    && strcmp(p, ".") != 0
                    && strcmp(p, "..") != 0) {
            find(buf, filename);
        } else if (strcmp(filename, p) == 0) {
            printf("%s\n", buf);
        }
    }

    close(fd);
}

int
main(int argc, char *argv[])
{
    if (argc < 3) {
        fprintf(2, "usage: find <path> <file>\n");
        exit(0);
    }

    find(argv[1], argv[2]);
    exit(0);
}