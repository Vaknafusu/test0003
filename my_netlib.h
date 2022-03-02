#ifndef __my_netlib_h_
#define __my_netlib_h_
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>			/* See NOTES */
#include <sys/socket.h>
#include <errno.h>
#include <sys/select.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <signal.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>
#include <time.h>
#include <ctype.h>


#define ser_port 8888
#define my_port ser_port
#define buf_size 1024
#define MAX_EVENTS 1024

struct myevent_s {
	int fd;													//要监听的文件描述符  
	int events;												//对应的监听事件  
	void *arg;												//泛型参数  
	void (*call_back)(int fd, int events, void *arg);       //回调函数  
	int status;												//是否在监听:1->在红黑树上(监听), 0->不在(不监听)  
	char buf[buf_size];
	int len;
	long last_active;										//记录每次加入红黑树 g_efd 的时间值  
};

int Socket(int domain, int type, int protocol);
int Bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
int Listen(int sockfd, int backlog);
int Accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
int Connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
int Select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout);
int Inet_pton(int af, const char *src, void *dst);
int Epoll_create(int size);
int Epoll_ctl(int epfd, int op, int fd, struct epoll_event *event);
int Epoll_wait(int epfd, struct epoll_event *events, int maxevents, int timeout);
void do_sigchild(int num);
int socket_init();
void eventadd(int efd, int events, struct myevent_s *ev);
void eventset(struct myevent_s *ev, int fd, void (*call_back)(int, int, void *), void *arg);
void add_myevent(int fd, int epo_evente, void (*call_back)(int, int, void *), void *arg, struct myevent_s *my_event);
void acceptconn(int lfd, int events, void *arg);
void eventdel(int efd, struct myevent_s *ev);
void senddata(int fd, int events, void *arg);
void recvdata(int fd, int events, void *arg);
int get_line(int cfd, char *buf, int size);
void sendto_b(struct myevent_s *ev, int bfd);
void send_http(int bfd, int no, char *disp, const char *type);
void send_error(int cfd, int status, char *title, char *text);
const char *get_file_type(const char *name);
void encode_str(char* to, int tosize, const char* from);

extern int epoll_fd;
extern struct myevent_s g_events[MAX_EVENTS+1];
#endif
