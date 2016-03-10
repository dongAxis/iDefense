//
//  main.cpp
//  fake_client
//
//  Created by Axis on 16/3/9.
//  Copyright © 2016年 Axis. All rights reserved.
//

#include <iostream>
#include <util/iDFIPC.h>
#include <util/iDFCommandSet.h>

#if IDF_PLUGIN_MAX>8
#error "the number of plugin is 8 at most"
#endif


//command
#warning test command, we will modify it when the plugin is ready



int main(int argc, const char * argv[]) {
    idf::ipc::iDFIPC *ipc = new idf::ipc::iDFIPC();
    
    int sock = ipc->connect("1001");
    printf("\nsock=%d\n", sock);
    char buf[1024] = {0};
    strcpy(buf, "client");
    
    idf::ipc::RequestHeader header;
    header.cmd=IDF_NAMINGSERVICE_GET_ALL_INFO;
    
    int teststr = IDF_PLUGIN_NAMINGSERVICE;
    //        header.version="";
    
    ipc->send(sock, &header, sizeof(header), (const void*)&teststr, sizeof(teststr));
    
    idf::ipc::ResponseHeader res;
    void *msg=NULL;
    int len = -1;
    
    int ret = ipc->recv(sock, &res, sizeof(res), &msg, &len);
    printf("\n\nret = %d\n\n, msg=%s", ret, (char*)msg);
    
    printf("quit now");
    //        write(sock, buf, strlen(buf));
    return 0;
}
