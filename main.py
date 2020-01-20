#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# File              : main.py
# Author            : luvjoey1996
# Date              : 18.01.2020
# Last Modified Date: 18.01.2020
# Last Modified By  : luvjoey1996
from ctypes import *

def main():
    cdll.LoadLibrary('./listen_on_addr.so')
    libc = CDLL("./listen_on_addr.so")
    libc.listen_on_addr
    ip = c_wchar("0.0.0.0")
    p = create_string_buffer(b"0.0.0.0")
    port = c_int(8888)
    libc.listen_on_addr(pointer(ip), port)
    libc.listen_on_addr(pointer(p), port)
    import socket
    sock = socket.fromfd(3, socket.AF_INET, socket.SOCK_STREAM)

main()
