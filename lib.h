/**
 * File              : lib.h
 * Author            : luvjoey1996
 * Date              : 18.01.2020
 * Last Modified Date: 20.01.2020
 * Last Modified By  : luvjoey1996
 */

#ifndef __URING_EXAMPLE_H
#define __URING_EXAMPLE_H
#include<liburing.h>
#include<unistd.h>
#include<sys/socket.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<string.h>
#include<errno.h>
#include<stdio.h>
#include<stdbool.h>

extern struct io_uring ring;

int listen_on_addr(char *, int);
void echo_handler();

void http_handler();

#endif
