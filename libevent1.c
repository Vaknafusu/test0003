#include <stdio.h> 
#include <unistd.h> 
#include <stdlib.h> 
#include <sys/types.h> 
#include <sys/stat.h> 
#include <string.h> 
#include <fcntl.h> 
#include <event2/event.h> 
#include <event2/listener.h> 
#include <event2/bufferevent.h> 

#define my_port 8888
void read_cb(struct bufferevent *bev, void *ctx)
{
    char buf[1024];
    size_t ret;
    ret = bufferevent_read(bev, buf, sizeof(buf));
    printf("%s",buf);
    bufferevent_write(bev, buf, ret);
}
void write_cb(struct bufferevent *bev, void *ctx)
{
    printf("write finish\r\n");
}
void listener_cb(struct evconnlistener *listener, evutil_socket_t fd, struct sockaddr *addr, int socklen, void *arg)
{
    struct bufferevent *ev;
    struct event_base *base = arg;
    ev = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE);
    bufferevent_setcb(ev, read_cb, write_cb, NULL, NULL);
    bufferevent_enable(ev, EV_READ);

}

int main(int argc, char **argv)
{
    // 创建个event_base 
    struct event_base* base = NULL;
    base = event_base_new();
    struct sockaddr_in saddr;
    memset(&saddr, 0, sizeof(struct sockaddr_in));
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(my_port);
    saddr.sin_addr.s_addr = htonl(INADDR_ANY);

    struct evconnlistener * listner = evconnlistener_new_bind (	
                                                                base,
                                                                listener_cb, 
                                                                base, 
                                                                LEV_OPT_CLOSE_ON_FREE | LEV_OPT_REUSEABLE,
                                                                -1,
                                                                (struct sockaddr *)&saddr,
                                                                sizeof(saddr));
    event_base_dispatch(base);
    evconnlistener_free(listner);
    event_base_free(base);
    return 0;
}