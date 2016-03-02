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
    
    while(1)
    {
        if((newClient=::accept(sock, (struct sockaddr*)&addr, &len))>0)
        {
            printf("%s=%d", "newClient", newClient);
            int on = 1;
            if(setsockopt(newClient, SOL_SOCKET, SO_NOSIGPIPE, &on, sizeof(on))<0)
            {
                return -1;
            }
            break;
        }
       else
       {
           printf("errono code is %d", errno);
           if(errno==EINTR) continue;
       }
    }
    
    return newClient;
}

void idf::ipc::iDFIPC::close(int sock)
{
    if(sock>0)
        ::close(sock);
}

