#include <my_netlib.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h> /* See NOTES */
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h> /* superset of previous */
#include <arpa/inet.h>
#include <signal.h>
#include <sys/wait.h>


int epoll_fd;
struct myevent_s g_events[MAX_EVENTS+1];
int main(int argc, char **argv)
{
	
	int sfd, i;
	struct epoll_event epo_eves[1025];
	struct myevent_s *my_event;
	
	sfd = socket_init();

	epoll_fd = Epoll_create(1024);
	my_event = malloc(sizeof(struct myevent_s));
	add_myevent(sfd, EPOLLIN, acceptconn, my_event, my_event);
	while(1){
		int nfd = Epoll_wait(epoll_fd,  epo_eves, 1025, 1000);

		for (i = 0; i < nfd; i++) {

			struct myevent_s *ev = (struct myevent_s *) epo_eves[i].data.ptr;
			if (( epo_eves[i].events & EPOLLIN) && (ev->events & EPOLLIN)) { 
                ev->call_back(ev->fd, epo_eves[i].events, ev->arg);  
                
            }  
            if ((epo_eves[i].events & EPOLLOUT) && (ev->events & EPOLLOUT)) {         //写就绪事件  
				ev->call_back(ev->fd, epo_eves[i].events, ev->arg);  
				
            }  
        }  
     
	}
    /* 退出前释放所有资源 */  
    return 0;
}
