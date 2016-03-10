
//
//  main.cpp
//  testipc
//
//  Created by Axis on 16/3/2.
//  Copyright © 2016年 Axis. All rights reserved.
//

#include <iostream>
#include "iDFIPC.h"
#include "iDFLog.h"
#include "iDFPlugin.h"
#include <sys/uio.h>
#include "iDFConfig.h"
#include "iDFTypeConvert.h"
#include "iDFLock.h"
//#include "iDFTypeConvert.h"

int main(int argc, const char * argv[]) {
    pid_t pid = fork();
    
//    idf::iDFConfig<idf::MutexLock> config("/Users/axis/PycharmProjects/iDefense/iDefense/iDefense/src/lib/iDFUtil/Tests/res/test.plist");
//    std::string group="test1";
//    idf::LogMessage obj;
//    config.get(group, obj);
    
//    obj.set_debug_level(500);
//    obj.set_max_size(5000);
//    
//    config.set(group, obj);
//    config.save();
    
    if(pid<0)
    {
        printf("error");
    }
    else if(pid==0)
    {
        sleep(5);   //wait 5 second for server start
        printf("child, pid = %d", getpid());
        
        idf::ipc::iDFIPC *ipc = new idf::ipc::iDFIPC();
        
        int sock = ipc->connect("6661");
        printf("\nsock=%d\n", sock);
        char buf[1024] = {0};
        strcpy(buf, "client");
        
        idf::ipc::RequestHeader header;
        header.cmd=1;
        
        char teststr[] = "hello world";
//        header.version="";
        
        ipc->send(sock, &header, sizeof(header), teststr, sizeof(teststr));
        
        printf("quit now");
//        write(sock, buf, strlen(buf));
    }
    else
    {
        printf("parent, pid=%d", getpid());
        fflush(stdout);
        
        idf::ipc::iDFIPC *ipc = new idf::ipc::iDFIPC();
        int sock = ipc->listen("6661");
        sock = ipc->accept(sock);
        
        char buf[1024];
        bzero(buf, 1024);
        
        idf::ipc::RequestHeader header;
        int header_len = sizeof(header);
        
        void * ptr=nullptr;
        int msg_len = 0;

        ipc->recv(sock, &header, header_len, &ptr, &msg_len);
        
//        struct iovec h = *((struct iovec*)buf);
//        printf("\n\n\n %ld  %s\n\nEND\n", size, (char*)test[0].iov_base);
        
//        while(1) {}
    }

    return 0;
}
