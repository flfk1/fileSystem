#include "function.h"
void initSys(){
    printf("initSys 执行了\n");
}
void sysStart(){
    printf("sysStart 执行了\n");
}
int my_read(int fd,int len){
	if (fd < 0 || fd >= MAXOPENFILE) {
		printf("文件描述符错误");
        return -1;
	}
	else {
		char* text = (char*)malloc(sizeof(char) * len);
		do_read(fd, len, text);
        return 0;
	}
}
int do_read(int fd, int len, char* text) {
    if (openfilelist[fd].topenfile == FREE) {//判断是否已经关闭
        printf("read: cannot read to fd ‘%d’: fd %d is already close\n", fd, fd);
        return -1;
    }
    else {
        if (openfilelist[fd].attribute == 0) {//判断如果是目录
            printf("read: cannot read to fd ‘%d’: fd %d is a directory\n", fd, fd);
            return -1;
        }
        char buff[BLOCKSIZE];
        openfilelist[fd].count = openfilelist[fd].first;
        while (openfilelist[fd].count != END && len > 0) {
            if (openfilelist[fd].count * BLOCKSIZE <= 0)
                fseek(DISK, 0, SEEK_SET);
            else
                fseek(DISK, openfilelist[fd].count * BLOCKSIZE, SEEK_SET);
                fread(buff, BLOCKSIZE, 1, DISK);
                if (len < BLOCKSIZE)
                    strncpy(text, buff, len);
                else
                    strcat(text, buff);
            len -= strlen(buff);
            strcat(text, buff);
            fputs(buff, stdout);
            openfilelist[fd].count = FAT1[openfilelist[fd].count].id;
        }
        return 0;
}
