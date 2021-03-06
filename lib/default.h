#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <setjmp.h>
#include <signal.h>
#include <dirent.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <errno.h>
#include <math.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAXLINE 8192
#define LISTENQ 1024
#define FILEN 16
#define FILENAME 256

typedef struct {
    int *buf;       // 动态分配的数组指针 
    int n;          // 最大成员数
    int front;      // 第一个项 buf[(front+1)%n]
                    // 是buf的第一个可用项的索引
                    // 移除第一个项时将其加一
                        /*
                         * front n. 正面; 前面; 正前方; 前部;
                         * adj. 前面的; 前部的; 在前的;
                         * v. 面向; 在...前面;
                         * */
    int rear;       // 最后一项 buf[rear%n]
                    // 是buf最后一个可用项后的空项的索引
                    // 添加新项目时将其加一
                        /* 
                         * read adj. 后方的，后面的;
                         * n. 后部; 屁股;
                         * v. 抚养; 养育; 培养;
                         * */
    sem_t mutex;    // 提供互斥的缓冲区访问
    sem_t slots;    // 记录空槽位
    sem_t items;    // 可用项目的数量
} sbuf_t;

/* 创建一个空的，拥有n个空槽的共享缓冲区 */
void sbuf_init(sbuf_t *sp, int n);
/* 清除共享缓冲区 */
void sbuf_deinit(sbuf_t *sp);
/* 添加一个item到共享缓冲区的后面 */
void sbuf_insert(sbuf_t *sp, int item);
/* 移除并且返回共享缓冲区的第一个项目 */
int sbuf_remove(sbuf_t *sp);

/* 彩色化输出 */
void printf_clr(const char *s, const char *color);

/* 将整个字符串数组初始化为null 
 * 第一个参数指明数组维度
 * 后面的参数是数组维度的值
 * 从第一维到最后一维 */
// void Nulltoarray(int n, ...);

int open_clientfd(char *hostname, char *port);
int open_listenfd(char *port);
int recvfile(int fd, char *filename);
int sendfile(int fd, char *filename);
