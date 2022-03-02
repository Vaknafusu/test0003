#include <my_netlib.h>
/*
struct sockaddr_in {
	sa_family_t    sin_family; address family: AF_INET
	in_port_t	   sin_port;   port in network byte order 
	struct in_addr sin_addr;   internet address 
};

struct in_addr {
	uint32_t	   s_addr;	   address in network byte order 
};

*/

/*
socket(int domain, int type, int protocol);
domain:
	AF_INET 用IPv4的地址
	AF_INET6 与上面类似，不过是来用IPv6的地址
	AF_UNIX 本地协议，使用在Unix和Linux系统上，一般都是当客户端和服务器在同一台及其上的时候使用
type:
	SOCK_STREAM 这个协议是按照顺序的、可靠的、数据完整的基于字节流的连接。这是一个使用最多的socket类型，这个socket是使用TCP来进行传输。
	SOCK_DGRAM 这个协议是无连接的、固定长度的传输调用。该协议是不可靠的，使用UDP来进行它的连接。
	SOCK_SEQPACKET该协议是双线路的、可靠的连接，发送固定长度的数据包进行传输。必须把这个包完整的接受才能进行读取。
	SOCK_RAW socket类型提供单一的网络访问，这个socket类型使用ICMP公共协议。（ping、traceroute使用该协议）
	SOCK_RDM 这个类型是很少使用的，在大部分的操作系统上没有实现，它是提供给数据链路层使用，不保证数据包的顺序
protocol:
	传0 表示使用默认协议。
返回值：
	成功：返回指向新创建的socket的文件描述符，失败：返回-1，设置errno
*/
int Socket(int domain, int type, int protocol)
{
	int ret;
	ret = socket(domain, type, protocol);
	if(ret == -1)
	{
		perror("socket error:");
		exit(1);
	}
	return ret;
}

/*
bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
sockfd：
	socket文件描述符
addr:
	构造出IP地址加端口号
addrlen:
	sizeof(addr)长度
返回值：
	成功返回0，失败返回-1, 设置errno
	
*/
int Bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
	int ret;
	ret = bind(sockfd, addr, addrlen);
	if(ret == -1)
	{
		perror("bind error:");
		exit(1);
	}
	return ret;
}

/*
int listen(int sockfd, int backlog)
sockfd:
	socket文件描述符
backlog:
	排队建立3次握手队列和刚刚建立3次握手队列的链接数和
*/
int Listen(int sockfd, int backlog)
{
	int ret;
	ret = listen(sockfd, backlog);
	if(ret == -1)
	{
		perror("listen error:");
		exit(1);
	}
	return ret;
}

/*
int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen)
sockdf:
	socket文件描述符
addr:
	传出参数，返回链接客户端地址信息，含IP地址和端口号
addrlen:
	传入传出参数（值-结果）,传入sizeof(addr)大小，函数返回时返回真正接收到地址结构体的大小
返回值：
	成功返回一个新的socket文件描述符，用于和客户端通信，失败返回-1，设置errno
*/
int Accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen)
{
	int ret;
again:
	if ( (ret = accept(sockfd, addr, addrlen)) < 0) 
	{
		if ((errno == ECONNABORTED) || (errno == EINTR))
		{
			goto again;
		}else
		{
			perror("listen error:");
			exit(1);
		}
	}
		return ret;

}

/*
int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
sockdf:
	socket文件描述符
addr:
	传入参数，指定服务器端地址信息，含IP地址和端口号
addrlen:
	传入参数,传入sizeof(addr)大小
返回值：
	成功返回0，失败返回-1，设置errno
*/
int Connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
	int ret;
	ret = connect(sockfd, addr, addrlen);
	if(ret == -1)
	{
		perror("connect error:");
		exit(1);
	}
	return ret;
}

int Inet_pton(int af, const char *src, void *dst)
{
	int ret; 
	ret = inet_pton(af, src, dst);
	if(ret != 1)
	{
		perror("inet_pton error:");
		exit(1);
	}
	return ret;

}
/*
int select(int nfds, fd_set *readfds, fd_set *writefds,
			fd_set *exceptfds, struct timeval *timeout);

	nfds: 		监控的文件描述符集里最大文件描述符加1，因为此参数会告诉内核检测前多少个文件描述符的状态
	readfds：	监控有读数据到达文件描述符集合，传入传出参数
	timeout：	定时阻塞监控时间，3种情况
				1.NULL，永远等下去
				2.设置timeval，等待固定时间
				3.设置timeval里时间均为0，检查描述字后立即返回，轮询
	void FD_CLR(int fd, fd_set *set); 	//把文件描述符集合里fd清0
	int FD_ISSET(int fd, fd_set *set); 	//测试文件描述符集合里fd是否置1
	void FD_SET(int fd, fd_set *set); 	//把文件描述符集合里fd位置1
	void FD_ZERO(fd_set *set); 			//把文件描述符集合里所有位清0
*/
int Select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout)
{
	int ret;
	ret = select(nfds, readfds, writefds, exceptfds, timeout);
	if(ret == -1)
	{
		perror("select error:");
		exit(1);
	}
	return ret;
}

int Epoll_create(int size)
{
	int ret;
	ret = epoll_create(size);
	if(ret == -1)
	{
		perror("epoll_create error:");
		exit(1);
	}
	return ret;
}
/*
int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event)
		op：表示动作，用3个宏来表示：
			EPOLL_CTL_ADD (注册新的fd到epfd)，
			EPOLL_CTL_MOD (修改已经注册的fd的监听事件)，
			EPOLL_CTL_DEL (从epfd删除一个fd)；
		event：	告诉内核需要监听的事件

		struct epoll_event {
			__uint32_t events; 
			epoll_data_t data; 
		};
		typedef union epoll_data {
			void *ptr;
			int fd;
			uint32_t u32;
			uint64_t u64;
		} epoll_data_t;

		EPOLLIN ：	表示对应的文件描述符可以读（包括对端SOCKET正常关闭）
		EPOLLOUT：	表示对应的文件描述符可以写
		EPOLLPRI：	表示对应的文件描述符有紧急的数据可读（这里应该表示有带外数据到来）
		EPOLLERR：	表示对应的文件描述符发生错误
		EPOLLHUP：	表示对应的文件描述符被挂断；
		EPOLLET： 	将EPOLL设为边缘触发(Edge Triggered)模式，这是相对于水平触发(Level Triggered)而言的
		EPOLLONESHOT：只监听一次事件，当监听完这次事件之后，如果还需要继续监听这个socket的话，
			需要再次把这个socket加入到EPOLL队列里
*/
int Epoll_ctl(int epfd, int op, int fd, struct epoll_event *event)
{
	int ret;
	ret = epoll_ctl(epfd, op, fd, event);
	if(ret == -1)
	{
		perror("epoll_ctl error:");
		exit(1);
	}
	return ret;	
}
/*
	int epoll_wait(int epfd, struct epoll_event *events, int maxevents, int timeout)
		events：		用来存内核得到事件的集合，
		maxevents：	告之内核这个events有多大，这个maxevents的值不能大于创建epoll_create()时的size，
		timeout：	是超时时间
			-1：	阻塞
			0：	立即返回，非阻塞
			>0：	指定毫秒
		返回值：	成功返回有多少文件描述符就绪，时间到时返回0，出错返回-1
*/
int Epoll_wait(int epfd, struct epoll_event *events, int maxevents, int timeout)
{
	int ret;
	ret = epoll_wait(epfd, events, maxevents, timeout);
    if(ret == -1)
	{
		perror("epoll_wait error:");
		exit(1);
	}
	return ret;              
}
/*
信号捕获，子进程回收
*/
void do_sigchild(int num)
{
	while (waitpid(0, NULL, WNOHANG) > 0)
		;
}
/*
套接字初始化
*/
int socket_init()
{
	struct sigaction newact;
	int opt = 1;
	int sfd;
	struct sockaddr_in my_addr;

	newact.sa_handler = do_sigchild;
	sigemptyset(&newact.sa_mask);
	newact.sa_flags = 0;
	sigaction(SIGCHLD, &newact, NULL); //信号捕捉，自动回收子进程

	sfd = Socket(AF_INET, SOCK_STREAM, 0);

	setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)); //端口复用

	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(my_port);
	my_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	Bind(sfd, (struct sockaddr *)&my_addr, sizeof(my_addr));

	Listen(sfd, 10);
	return sfd;
}     
void add_myevent(int fd, int epo_evente, void (*call_back)(int, int, void *), void *arg, struct myevent_s *my_event)
{
	struct epoll_event epo_eve;

	epo_eve.events = epo_evente;

	my_event->events = epo_evente;
	my_event->fd = fd;
	my_event->status = 1;
	my_event->last_active = time(NULL);
	my_event->arg = arg;
	my_event->call_back = call_back;
	my_event->len = 0;
	memset(my_event->buf, 0, sizeof(my_event->buf)); 

	epo_eve.data.ptr = my_event;
	Epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &epo_eve);
	printf("event add OK [fd=%d], events[%0X]\n", my_event->fd, epo_evente);
}         
void eventset(struct myevent_s *ev, int fd, void (*call_back)(int, int, void *), void *arg)  
{  
    ev->fd = fd;  
    ev->call_back = call_back;  
    ev->events = 0;  
    ev->arg = arg;  
    ev->status = 0;  
    memset(ev->buf, 0, sizeof(ev->buf));  
    ev->len = 0;  
    ev->last_active = time(NULL);                       //调用eventset函数的时间  
  
    return;  
}  

void eventadd(int efd, int events, struct myevent_s *ev)  
{  
    struct epoll_event epv = {0, {0}};  
    int op;  
    epv.data.ptr = ev;  
    epv.events = ev->events = events;       //EPOLLIN 或 EPOLLOUT  
  
    if (ev->status == 0) {                                          //已经在红黑树 epoll_fd 里  
        op = EPOLL_CTL_ADD;                 //将其加入红黑树 epoll_fd, 并将status置1  
        ev->status = 1;  
    }  
	if (epoll_ctl(efd, op, ev->fd, &epv) < 0)                       //实际添加/修改  
        printf("event add failed [fd=%d], events[%d]\n", ev->fd, events);  
    else  
        printf("event add OK [fd=%d], op=%d, events[%0X]\n", ev->fd, op, events);  
  
    return ;  
}  

void eventdel(int efd, struct myevent_s *ev)  
{  
    struct epoll_event epv = {0, {0}};  
  
    if (ev->status != 1)                                        //不在红黑树上  
        return ;  
  
    //epv.data.ptr = ev;  
    epv.data.ptr = NULL;  
    ev->status = 0;                                             //修改状态  
    epoll_ctl(efd, EPOLL_CTL_DEL, ev->fd, &epv);                //从红黑树 efd 上将 ev->fd 摘除  
  
    return ;  
} 
//send_error(cfd, 404, "Not Found", "");
void send_error(int cfd, int status, char *title, char *text)
{
	char buf[4096] = {0};

	sprintf(buf, "%s %d %s\r\n", "HTTP/1.1", status, title);
	sprintf(buf+strlen(buf), "Content-Type:%s\r\n", "text/html");
	sprintf(buf+strlen(buf), "Content-Length:%d\r\n", -1);
	sprintf(buf+strlen(buf), "Connection: close\r\n");
	send(cfd, buf, strlen(buf), 0);
	send(cfd, "\r\n", 2, 0);

	memset(buf, 0, sizeof(buf));

	sprintf(buf, "<html><head><title>%d %s</title></head>\n", status, title);
	sprintf(buf+strlen(buf), "<body bgcolor=\"#cc99cc\"><h2 align=\"center\">%d %s</h4>\n", status, title);
	sprintf(buf+strlen(buf), "%s\n", text);
	sprintf(buf+strlen(buf), "<hr>\n</body>\n</html>\n");
	send(cfd, buf, strlen(buf), 0);
	
	return ;
}

void acceptconn(int lfd, int events, void *arg)
{
	struct sockaddr_in cin;
	socklen_t len = sizeof(cin);
	int cfd, i;

	if ((cfd = accept(lfd, (struct sockaddr *)&cin, &len)) == -1) {
		if (errno != EAGAIN && errno != EINTR) {
		}
		printf("%s: accept, %s\n", __func__, strerror(errno));
		return ;
	}

	do{
		for (i = 0; i < MAX_EVENTS; i++)								//从全局数组g_events中找一个空闲元素  
			if (g_events[i].status == 0)								//类似于select中找值为-1的元素  
			break;													//跳出 for  

		if (i == MAX_EVENTS) {
		printf("%s: max connect limit[%d]\n", __func__, MAX_EVENTS);
		break;														//跳出do while(0) 不执行后续代码  
		}

	int flag = 0;
		if ((flag = fcntl(cfd, F_SETFL, O_NONBLOCK)) < 0) {				//将cfd也设置为非阻塞  
		printf("%s: fcntl nonblocking failed, %s\n", __func__, strerror(errno));
			break;
		}

		/* 给cfd设置一个 myevent_s 结构体, 回调函数 设置为 recvdata */
		eventset(&g_events[i], cfd, recvdata, &g_events[i]);
		eventadd(epoll_fd, EPOLLIN, &g_events[i]);		//将cfd添加到红黑树epoll_fd中,监听读事件  
		//add_myevent(cfd, EPOLLIN, recvdata, &g_events[i], &g_events[i]);
	} while(0);

	printf("new connect [%s:%d][time:%ld], pos[%d]\n",
			inet_ntoa(cin.sin_addr), ntohs(cin.sin_port), g_events[i].last_active, i);
	return ;
}

int get_line(int cfd, char *buf, int size)
{
    int i = 0;
    char c = '\0';
    int n;
    while ((i < size-1) && (c != '\n')) {  
        n = recv(cfd, &c, 1, 0);
        if (n > 0) {     
            if (c == '\r') {            
                n = recv(cfd, &c, 1, MSG_PEEK);
                if ((n > 0) && (c == '\n')) {              
                    recv(cfd, &c, 1, 0);
                } else {                       
                    c = '\n';
                }
            }
            buf[i] = c;
            i++;
        } else {      
            c = '\n';
        }
    }
    buf[i] = '\0';
    
    if (n == -1)
    	i = n;

    return i;
}

void recvdata(int fd, int events, void *arg)  
{  
    struct myevent_s *ev = (struct myevent_s *)arg;  
    int len;  
	char file[1024];
	char method[16], protocol[16];
	char c = '.';
  	len = get_line(fd, ev->buf, sizeof(ev->buf));
	if(len <= 0)
	{
		close(ev->fd); 
	}else
	{
		//printf("%s",ev->buf);
		sscanf(ev->buf, "%[^ ] %[^ ] %[^ ]", method, file, protocol);
		//printf("%s",file);
		while((len = get_line(fd, ev->buf, sizeof(ev->buf)))>0)
		{     
			//printf("%s",ev->buf);
		}

		
		eventdel(epoll_fd, ev);        //将该节点从红黑树上摘除  
		
		sprintf(ev->buf,"%c%s",c,file);
		ev->len = strlen(ev->buf);  
		ev->buf[ev->len] = '\0';                                //手动添加字符串结束标记  
		printf("C[%d]:%s\n", fd, ev->buf);  
		ev->call_back = senddata;  
		eventadd(epoll_fd, EPOLLOUT, ev);                      //将fd加入红黑树epoll_fd中,监听其写事件
	} 
    return;  
}

const char *get_file_type(const char *name)
{
    char* dot;

    // 自右向左查找‘.’字符, 如不存在返回NULL
    dot = strrchr(name, '.');   
    if (dot == NULL)
        return "text/plain; charset=utf-8";
    if (strcmp(dot, ".html") == 0 || strcmp(dot, ".htm") == 0)
        return "text/html; charset=utf-8";
    if (strcmp(dot, ".jpg") == 0 || strcmp(dot, ".jpeg") == 0)
        return "image/jpeg";
    if (strcmp(dot, ".gif") == 0)
        return "image/gif";
    if (strcmp(dot, ".png") == 0)
        return "image/png";
    if (strcmp(dot, ".css") == 0)
        return "text/css";
    if (strcmp(dot, ".au") == 0)
        return "audio/basic";
    if (strcmp( dot, ".wav" ) == 0)
        return "audio/wav";
    if (strcmp(dot, ".avi") == 0)
        return "video/x-msvideo";
    if (strcmp(dot, ".mov") == 0 || strcmp(dot, ".qt") == 0)
        return "video/quicktime";
    if (strcmp(dot, ".mpeg") == 0 || strcmp(dot, ".mpe") == 0)
        return "video/mpeg";
    if (strcmp(dot, ".vrml") == 0 || strcmp(dot, ".wrl") == 0)
        return "model/vrml";
    if (strcmp(dot, ".midi") == 0 || strcmp(dot, ".mid") == 0)
        return "audio/midi";
    if (strcmp(dot, ".mp3") == 0)
        return "audio/mpeg";
    if (strcmp(dot, ".ogg") == 0)
        return "application/ogg";
    if (strcmp(dot, ".pac") == 0)
        return "application/x-ns-proxy-autoconfig";

    return "text/plain; charset=utf-8";
}

/*
 *  这里的内容是处理%20之类的东西！是"解码"过程。
 *  %20 URL编码中的‘ ’(space)
 *  %21 '!' %22 '"' %23 '#' %24 '$'
 *  %25 '%' %26 '&' %27 ''' %28 '('......
 *  相关知识html中的‘ ’(space)是&nbsp
 */
void encode_str(char* to, int tosize, const char* from)
{
    int tolen;

    for (tolen = 0; *from != '\0' && tolen + 4 < tosize; ++from) {    
        if (isalnum(*from) || strchr("/_.-~", *from) != (char*)0) {      
            *to = *from;
            ++to;
            ++tolen;
        } else {
            sprintf(to, "%%%02x", (int) *from & 0xff);
            to += 3;
            tolen += 3;
        }
    }
    *to = '\0';
}

void sendto_b(struct myevent_s *ev, int bfd)
{
	int fd;
	int len;
	int ret;
	int i;
	struct stat sbuf;
	char path[1024] = {0};
	char buf[buf_size] = {0};
	struct dirent** ptr;
	char enstr[1024] = {0};

	ret = stat(ev->buf, &sbuf);
	if (ret != 0) 
	{
		send_error(bfd, 404, "Not Found", "没有这个文件");
		perror("stat");	
	}
	if(S_ISREG(sbuf.st_mode))
	{
		send_http(bfd, 200, "ok", get_file_type(ev->buf));
	
		fd = open(ev->buf,O_RDONLY);
		if(fd == -1)
		{
			perror("open error");
		}
		while((len = read(fd, ev->buf, sizeof(ev->buf)))>0)
		{
			//printf("%s",ev->buf);
			len = send(bfd, ev->buf, len, 0);
			if(len == -1)
			{
				if(errno != EAGAIN && errno != 	EINTR)
					perror("send error");
			}
		}
		close(fd);
	}
	if(S_ISDIR(sbuf.st_mode))//如果是目录
	{
		send_http(bfd, 200, "ok", " Content-Type: text/plain; charset=iso-8859-1");
		
		sprintf(buf, "<html><head><title>目录名: %s</title></head>", ev->buf);
    	sprintf(buf+strlen(buf), "<body><h1>当前目录: %s</h1><table>", ev->buf);
		
    	int num = scandir(ev->buf, &ptr, NULL, alphasort);
		 
		for(i = 0; i < num; ++i) 
		{
			char* name = ptr[i]->d_name;

			// 拼接文件的完整路径
			sprintf(path, "%s/%s", ev->buf, name);
			//printf("path = %s ===================\n", path);
			struct stat st;
			stat(path, &st);

			// 编码生成 %E5 %A7 之类的东西
			encode_str(enstr, sizeof(enstr), name);
			
			// 如果是文件
			if(S_ISREG(st.st_mode)) {       
				sprintf(buf+strlen(buf), 
						"<tr><td><a href=\"%s\">%s</a></td><td>%ld</td></tr>",
						enstr, name, (long)st.st_size);
			} else if(S_ISDIR(st.st_mode)) {		// 如果是目录       
				sprintf(buf+strlen(buf), 
						"<tr><td><a href=\"%s/\">%s/</a></td><td>%ld</td></tr>",
						enstr, name, (long)st.st_size);
			}
			ret = send(bfd, buf, strlen(buf), 0);
			if (ret == -1) {
				if (errno == EAGAIN) {
					perror("send error:");
					continue;
				} else if (errno == EINTR) {
					perror("send error:");
					continue;
				} else {
					perror("send error:");
					exit(1);
				}
			}
			memset(buf, 0, sizeof(buf));
			// 字符串拼接
   		}

    sprintf(buf+strlen(buf), "</table></body></html>");
    send(bfd, buf, strlen(buf), 0);

    //printf("dir message send OK!!!!\n");
	}
	
}
void send_http(int bfd, int no, char *disp, const char *type)
{
	char buf[2048];
	sprintf(buf, "HTTP/1.1 %d %s\r\n", no, disp);  
   	send(bfd, buf, strlen(buf), 0);  
      
    sprintf(buf, "Content-Type: %s\r\n", type);  
    sprintf(buf+strlen(buf), "Content-Length:-1\r\n");  
    send(bfd, buf, strlen(buf), 0);  

    send(bfd, "\r\n", 2, 0);
}
void senddata(int fd, int events, void *arg)  
{  
    struct myevent_s *ev = (struct myevent_s *)arg;  

    sendto_b(ev, fd); 
    eventdel(epoll_fd, ev);                                //从红黑树epoll_fd中移除  
      
        close(ev->fd);                                      //关闭链接    
    
  
    return ;
}  