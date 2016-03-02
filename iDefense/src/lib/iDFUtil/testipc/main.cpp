//
//  main.cpp
//  testipc
//
//  Created by Axis on 16/3/2.
//  Copyright © 2016年 Axis. All rights reserved.
//

#include <iostream>
#include "iDFIPC.h"

int main(int argc, const char * argv[]) {
    pid_t pid = fork();
    
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
        write(sock, buf, strlen(buf));
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
        int size = read(sock, buf, sizeof(buf));
        
//        XCTAssert(strcmp(buf, "client")==0, "error");
        printf("\n\n\n%s\n\nEND\n", buf);
        
        while(1) {}
    }

    return 0;
}
