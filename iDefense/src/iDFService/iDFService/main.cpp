//	_ _____        __
//	(_)  __ \      / _|
//	_| |  | | ___| |_ ___ _ __  ___  ___
//	| | |  | |/ _ \  _/ _ \ '_ \/ __|/ _ \
//	| | |__| |  __/ ||  __/ | | \__ \  __/
//	|_|_____/ \___|_| \___|_| |_|___/\___|
//
//  main.cpp
//  iDFService
//
//  Created by Axis on 16/3/8.
//  Copyright © 2016年 Axis. All rights reserved.
//

#include <CoreFoundation/CoreFoundation.h>
#include <vector>
#include <stdio.h>
#include <cstring>
#include <cstdlib>
#include <util/iDFException.h>
#include <util/iDFConfig.h>
#include <util/iDFThread.h>
#include <util/common.h>
#include <util/iDFCommandSet.h>
#include <signal.h>

#include "iDFPluginMgr.hpp"
#include "iDFServer.hpp"
#include "iDFWoker.hpp"

iDFServer *server=NULL;

bool registerPlugin(char *own_port, iDFPluginMgr& mgr)
{
    std::vector<iDFPluginWrapper*> plugins = mgr.get_all_plugins();
    
    std::vector<iDFPluginWrapper*>::const_iterator begin = plugins.begin();
    std::vector<iDFPluginWrapper*>::const_iterator end = plugins.end();
    
    std::vector<int> type_id;
    type_id.clear();
    
    for(;begin!=end;begin++)
    {
        iDFPluginWrapper* wrapper = (iDFPluginWrapper*)(*begin);
        type_id.push_back(wrapper->get_plugin_id());
    }
    
    printf("hereherherher\n");
    
    if(type_id.empty()) return false;
    
    if(strcmp(own_port, NAMING_SERVER_PORT)==0)
    {
        begin = plugins.begin();
        
        for(;begin!=end; begin++)
        {
            if((*begin)->get_plugin_id()==IDF_PLUGIN_NAMINGSERVICE)
            {
                for (int i=0; i<type_id.size(); i++)
                {
                    struct register_data data={};
                    data.type = type_id[i];
                    bzero(data.node, sizeof(data.node));
                    strncpy(data.node, own_port, sizeof(data.node));
                    
                    (*begin)->send_command(IDF_NAMINGSERVICE_REGISTER_PLUGIN, &data, sizeof(data), NULL, NULL);
                }
                
                return true;
            }
        }
        
        printf("some plugin use the naming server's port but do not contain naming server");
        
        return false;
    }
    else
    {
        printf("i am here");
        
        idf::ipc::iDFIPC ipc;
        idf::ipc::RequestHeader header={};
        
        header.cmd = IDF_NAMINGSERVICE_REGISTER_PLUGIN;
        
        for(int i=0; i<type_id.size(); i++)
        {
            struct register_data data={};
            data.type = type_id[i];
            strncpy(data.node, own_port, strlen(own_port));
            
            int sock = ipc.connect(NAMING_SERVER_PORT);
            int code = ipc.send(sock, &header, sizeof(header), &data, sizeof(data));
            sleep(1);
            printf("\n\n\ncode is %d\n\n\n", code);
            ipc.close(sock);
        }
    }
    
    return true;
}

void signal_handle(int sig)
{
    printf("server is doing down");
    
    if(server)
        server->stop();
    
    fflush(stdout);
}

/**
 *  argv[1]
 *
 *  @param argc <#argc description#>
 *  @param argv <#argv description#>
 *
 *  @return <#return value description#>
 */
int main(int argc, char *argv[])
{
    if (argc<5) {
        exit(-1);
    }
    
    char *port;
    char *namespace_port;
    
    if(strcmp(argv[1], "-p")==0)
    {
        port = argv[2];     //own port
    }
    else
    {
        throwf("failed");
        exit(-1);
    }
    
    if(strcmp(argv[3], "-np")==0)
    {
        namespace_port = argv[4];
    }
    
    uint32_t lib_nums = argc-5;
    if(lib_nums<=0)
    {
        printf("params invalid, there are no any plugins");
        exit(-1);
    }
    
    //0. handle signal
    signal(SIGABRT, &signal_handle);
    signal(SIGINT, &signal_handle);
    signal(SIGQUIT, &signal_handle);
    signal(SIGABRT, &signal_handle);
    signal(SIGKILL, &signal_handle);
    
    //1. store all the plugin
    iDFPluginMgr mgr;
    for (int i=0; i<lib_nums; i++)
    {
        mgr.add_plugin(argv[5+i]);
    }
    
    //1.5 register plugin
    int code = registerPlugin(port, mgr);
    if(!code)
    {
#warning CG
        printf("register plugin failed");
        exit(-1);
    }
    
    //2. construct the unix domain file
//    std::string sock_path = "/var/tmp/sock.";
//    sock_path+=port;
//    
//    printf("%s", sock_path.c_str());
//    
//    if(access(sock_path.c_str(), F_OK)==0)
//    {
//        unlink(sock_path.c_str());
//    }
    
    //3. create server
    server = new iDFServer(std::string(port));
    
    //5. init all the plugin
    IDF_ERRORCODE error_code=IDF_SUCCESS;
    
    //----------------test code--------------
    
    error_code = mgr.run_all_plugins();
    if(IDF_IS_ERROR(error_code))
    {
//        goto CG;
    }
    
    //---------------test code----------
    
    //4 create worker
    std::vector<idf::iDFThread*> threads;
    threads.clear();
    
    iDFWorker worker(server, 0, &mgr);
    for(int i=0; i<5; i++)  //this is a assumption
    {
        threads.push_back(new iDFThread(&worker, false, priority_default));
    }
    
    server->run();
    
    printf("[-]serverv down\n");
    fflush(stdout);
    
CG:
    
    for(int i=0; i<5; i++)
    {
        threads[i]->stop();
        threads[i]->join();
        
        delete  threads[i];
        threads[i]=NULL;
    }
    threads.clear();
    
    error_code = mgr.stop_all_plugins();
    //remove all the plugin
    
    exit(0);
}