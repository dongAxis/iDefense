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
#include <util/common.h>
#include <sys/errno.h>

#if IDF_PLUGIN_MAX>8
#error "the number of plugin is 8 at most"
#endif

int check_status();
int get_all_plugin();
//command
#warning test command, we will modify it when the plugin is ready

typedef int (*CommandPtr)(void);
struct command_map
{
    char *prefix;
    CommandPtr ptr;
} CommandMap[] =
{
    {
        .prefix="check-status",
        .ptr=&check_status
    },
    {
        .prefix="get-all-plugin",
        .ptr=&get_all_plugin
    }
};

static int num;
static const char **argv_ptr;

CommandPtr findCallBack(char* command)
{
    for(int i=0; i<2; i++)
    {
        if(strcmp(CommandMap[i].prefix, command)==0)
        {
            return CommandMap[i].ptr;
        }
    }
    
    return NULL;
}

int get_all_plugin()
{
    if(num!=2)
    {
        return -1;
    }
    
    int error_code = -1;
    idf::ipc::iDFIPC *ipc = new idf::ipc::iDFIPC();
    idf::ipc::RequestHeader header;
    idf::ipc::ResponseHeader res;
    struct check_status status;
    char *test;
    int plugin_id=-1;
    void *msg=NULL;
    int msgLen = 0;
    
    int sock = ipc->connect(NAMING_SERVER_PORT);
    if(sock<0)
    {
        error_code=-1;
        goto GC;
    }
    
//    plugin_id = atoi(argv_ptr[2]);
//    status.type = plugin_id;
//    void *msg=NULL;
    header.cmd=IDF_NAMINGSERVICE_GET_ALL_INFO;
    
    error_code = ipc->send(sock, &header, sizeof(header), NULL, 0);
    if(error_code<0)
    {
        goto GC;
    }
    
    error_code=ipc->recv(sock, &res, sizeof(res), &msg, &msgLen);
    
   test  = (char*)msg;
    
    printf("[+] error_code is %d, node is %s \n", res.status, test);
GC:
    if(ipc) delete ipc;
    
    return error_code;
}


int check_status()
{
    if(num!=3)
    {
        return -1;
    }
    
    int error_code = -1;
    idf::ipc::iDFIPC *ipc = new idf::ipc::iDFIPC();
    idf::ipc::RequestHeader header;
    idf::ipc::ResponseHeader res;
    struct check_status status;
    int plugin_id=-1;
    void *msg=NULL;
    int msgLen = 0;
    
    int sock = ipc->connect(NAMING_SERVER_PORT);
    if(sock<0)
    {
        error_code=-1;
        goto GC;
    }
    
    plugin_id = atoi(argv_ptr[2]);
    status.type = plugin_id;
    header.cmd=IDF_NAMINGSERVICE_CHECK_STATUS;
    
    error_code = ipc->send(sock, &header, sizeof(header), &status, sizeof(status));
    if(error_code<0)
    {
        goto GC;
    }
    
    error_code=ipc->recv(sock, &res, sizeof(res), &msg, &msgLen);
    printf("[+] error_code is %d \n", res.status);
GC:
    if(ipc) delete ipc;
    
    return error_code;
}

int main(int argc, const char * argv[]) {
    
    
    if(argc<=1) return -1;
    
    const char* command=argv[1];
    CommandPtr handler = findCallBack((char*)command);
    
    if(handler==NULL) return -1;
    
    num = argc;
    argv_ptr = argv;
    
    return (*handler)();
    
//    idf::ipc::iDFIPC *ipc = new idf::ipc::iDFIPC();
//    
//    int sock = ipc->connect("1001");
//    printf("\nsock=%d\n", sock);
//    char buf[1024] = {0};
//    strcpy(buf, "client");
//    
//    idf::ipc::RequestHeader header;
//    header.cmd=IDF_NAMINGSERVICE_GET_ALL_INFO;
//    
//    int teststr = IDF_PLUGIN_NAMINGSERVICE;
//    //        header.version="";
//    
//    ipc->send(sock, &header, sizeof(header), (const void*)&teststr, sizeof(teststr));
//    
//    idf::ipc::ResponseHeader res;
//    void *msg=NULL;
//    int len = -1;
//    
//    int ret = ipc->recv(sock, &res, sizeof(res), &msg, &len);
//    printf("\n\nret = %d\n\n, msg=%s", ret, (char*)msg);
//    
//    printf("quit now");
//    //        write(sock, buf, strlen(buf));
//    return 0;
}
