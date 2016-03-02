//
//  iDFIPC.cpp
//  iDFUtil
//
//  Created by Axis on 16/3/2.
//  Copyright © 2016年 Axis. All rights reserved.
//

#include "iDFIPC.h"
#include <sys/un.h>
#include <sys/socket.h>
#include <string>
#include <fcntl.h>
#include <sys/errno.h>
#include <sys/stat.h>
#include <sys/uio.h>

int idf::ipc::iDFIPC::listen(const char *node) const
{
    if(node==nullptr)
    {
        return -1;
    }
    
    //1. create socket
    int sock = socket(AF_LOCAL, SOCK_STREAM, 0);    //create socket
    if(sock<0)
    {
        return -1;
    }
    
    //2. set option to reuse the addr
    int on = 1;
    if(setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on))<0)
    {
        ::close(sock);
        return -1;
    }
    
    //3. construct path
    // path is like /var/tmp/port.xxxx
    std::string path = "";
    path+="/var/tmp/";       //root dir
    path+="port.";
    path+=node;
    unlink(path.c_str());       //remove older path
    
    //4. bind the socket & addr
    struct sockaddr_un addr = {};
    addr.sun_family = AF_LOCAL;
    strncpy(addr.sun_path, path.c_str(), sizeof(addr.sun_path)-1>strlen(path.c_str())?strlen(path.c_str()):sizeof(addr.sun_path)-1);
    
    int ret = 0;
    if((ret = ::bind(sock, (const struct sockaddr*)&addr, (socklen_t)SUN_LEN(&addr)))<0)
    {
        printf("socket bind failed, errno=%d", errno);
        ::close(sock);
        return -1;
    }
    
    chmod(addr.sun_path, 0x777);
    //5. listen
    if((ret =::listen(sock, 5))<0)
    {
        printf("sock listen failed");
        ::close(sock);
        return -1;
    }
    
    return sock;
}

int idf::ipc::iDFIPC::connect(const char *node) const
{
    if(node==nullptr)
    {
        return -1;
    }
    
    //1. create socket
    int sock = socket(AF_LOCAL, SOCK_STREAM, 0);
    if(sock<0) return -1;
    
    //2. no handle broken pipe
    int on = 1;
    if(setsockopt(sock, SOL_SOCKET, SO_NOSIGPIPE, &on, sizeof(on))<0)
    {
        ::close(sock);
        return -1;
    }
    
    //3. construct path
    std::string path = "";
    path+="/var/tmp/";
    path+="port.";
    path+=node;
//    unlink(path.c_str());
    
    //4. connect
    struct sockaddr_un addr = {};
    addr.sun_family = AF_LOCAL;
    strncpy(addr.sun_path, path.c_str(), sizeof(addr.sun_path)-1>strlen(path.c_str())?strlen(path.c_str()):sizeof(addr.sun_path)-1);
    
    if(::connect(sock, (struct sockaddr*)&addr, (socklen_t)SUN_LEN(&addr))<0)
    {
        printf("failed, errno=%d", errno);
        ::close(sock);
        return -1;
    }
    
    return sock;
}

int idf::ipc::iDFIPC::accept(int sock)
{
    struct sockaddr_un addr;
    int newClient = -1;
    socklen_t len = sizeof(addr);
    
//    while(1)
//    {
        if((newClient=::accept(sock, (struct sockaddr*)&addr, &len))>0)
        {
            printf("%s=%d", "newClient", newClient);
            int on = 1;
            if(setsockopt(newClient, SOL_SOCKET, SO_NOSIGPIPE, &on, sizeof(on))<0)
            {
                return -1;
            }
//            break;
        }
       else
       {
           printf("errono code is %d", errno);
//           if(errno==EINTR) continue;
       }
//    }
    
    return newClient;
}

#warning [TODO] need to UT
int idf::ipc::iDFIPC::send(int sock, const void* header, int header_len, const void* msg, int msg_len)
{
    printf("111111");
    if(sock<0 || header==nullptr || msg==nullptr) return -1;
    
    printf("2222");
    
    struct iovec iovec[3];
    bzero(iovec, sizeof(iovec));    //fill zero
    
    int totalLen = header_len+msg_len;
    
    printf("header_len=%d, msg_len=%d, total = %d", header_len, msg_len, totalLen);
    
    iovec[0].iov_base = (char*)&totalLen;
    iovec[0].iov_len = sizeof(totalLen);
    iovec[1].iov_base = (void*)header;
    iovec[1].iov_len = header_len;
    iovec[2].iov_base = (void*)msg;
    iovec[2].iov_len = msg_len;
    
    size_t send_size = -1;
    while(true)
    {
        printf("333");
#define DIM(x)      (sizeof((x)) / sizeof((x)[0]))
        send_size = writev(sock, iovec, 3);
#undef DIM
        
        printf("\n send_size=%ld, totalLen=%d", send_size, totalLen);
        
        int errcode = errno;
        
        if(send_size>0) break;   //maybe successful
        
        if(errcode==EINTR) continue;    //it will triggle by int3 or other system trap
    }
    
    size_t actual_size = iovec[0].iov_len+totalLen;
    if(send_size!=actual_size)
    {
        printf("[ERROR] size dose not same ~ ");
        return -1;
    }
    
    return 0;
}

#warning [TODO] need to UT
int idf::ipc::iDFIPC::recv(int sock, void* header, int header_len, void** msg, int* msg_len)
{
    struct iovec vect_header;
    
    //1. read header
    size_t size;
    while (1)
    {
    size = readv(sock, &vect_header, 1);
    if(size!=sizeof(struct iovec))
    {
        printf("failed, size is %ld", size);
    }
    else break;
    }
    
    printf("size is %d", *(int*)vect_header.iov_base);  //this is the total size
    
    //2. read the last 2 block vector
    struct iovec vect_block[2];
    bzero(vect_block, sizeof(vect_block));
    size = readv(sock, vect_block, 2);
    if(size!=*(int*)vect_header.iov_base-sizeof(int)) return -1;
    
    *msg_len = (int)(size-header_len);
    
    
    return 0;
}

void idf::ipc::iDFIPC::close(int sock)
{
    if(sock>0)
        ::close(sock);
}

