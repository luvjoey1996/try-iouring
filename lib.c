/**
 * File              : lib.c
 * Author            : luvjoey1996
 * Date              : 18.01.2020
 * Last Modified Date: 20.01.2020
 * Last Modified By  : luvjoey1996
 */

#include"lib.h"
#include"aco.h"
#include"aco_assert_override.h"
#include<http_parser.h>

int listen_on_addr(char *ip, int port)
{
    int listenfd, ret;
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd < 0) {
        fprintf(stderr, "socket\n");
        goto ERR;
    }
    struct sockaddr_in serv_addr;
    bzero(&serv_addr, sizeof(serv_addr));
    inet_aton(ip, &(serv_addr.sin_addr));
    serv_addr.sin_port = htons(port);
    ret = bind(listenfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr));
    if (ret < 0) {
        fprintf(stderr, "bind\n");
        goto ERR;
    }
    ret = listen(listenfd, 128);
    if (ret < 0) {
        fprintf(stderr, "listen\n");
        goto ERR;
    }
    return listenfd;
ERR:
    fprintf(stderr, "%s\n", strerror(errno));
    exit(1);
}

void echo_handler() {
    aco_t *co = aco_get_co();
    int *iretp = (int *) ((uintptr_t)co->arg);
    int clientfd = *iretp;
    struct iovec vec;
    char *base = calloc(1024, sizeof(char));
    vec.iov_base = base;
    struct io_uring_sqe *sqe;
    while (true) {
        vec.iov_len = 1024;
        sqe = io_uring_get_sqe(&ring);
        io_uring_prep_readv(sqe, clientfd, &vec, 1, 0);
        io_uring_sqe_set_data(sqe, &clientfd);
        io_uring_submit(&ring);
        aco_yield();
        int res = *iretp;
        if (res <= 0) {
            goto EXIT;
        }
        vec.iov_len = res;
        sqe = io_uring_get_sqe(&ring);
        io_uring_prep_writev(sqe, clientfd, &vec, 1, 0);
        io_uring_sqe_set_data(sqe, &clientfd);
        io_uring_submit(&ring);
        aco_yield();
    }
    goto EXIT;
EXIT:
    close(clientfd);
    aco_exit();
}

void http_handler() 
{
    aco_t *co = aco_get_co();
    int *iretp = (int *) ((uintptr_t) co->arg);
    int clientfd = *iretp;
    struct iovec vec;
    char *base = calloc(1024, sizeof(char));
    vec.iov_base = base;
    while (true) {
        vec.iov_len = 1024;
        struct io_uring_sqe *sqe;
        sqe = io_uring_get_sqe(&ring);
        io_uring_prep_readv(sqe, clientfd, &vec, 1, 0);
        io_uring_sqe_set_data(sqe, &clientfd);
        io_uring_submit(&ring);
        aco_yield();
        int res = *iretp;
        if (res < 0) {
            goto EXIT;
        }
        memset(vec.iov_base, 0, 1024);
        sprintf(vec.iov_base, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nDate: Mon, 28 Feb 2011 10:38:19 GMT\r\nTransfer-Encoding: chunked\r\nServer: Myserver\r\n\r\n%x\r\n%s\r\n0\r\n\r\n", strlen("<html><body><p>Hello world!</p></body></html>"),"<html><body><p>Hello world!</p></body></html>");
        vec.iov_len = strlen(vec.iov_base);
        sqe = io_uring_get_sqe(&ring);
        io_uring_prep_writev(sqe, clientfd, &vec, 1, 0);
        io_uring_sqe_set_data(sqe, &clientfd);
        io_uring_submit(&ring);
        aco_yield();
        goto EXIT;
    }
EXIT:
    close(clientfd);
    free(vec.iov_base);
    aco_exit();
}

