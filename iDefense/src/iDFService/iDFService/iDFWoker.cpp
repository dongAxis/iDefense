//	_ _____        __
//	(_)  __ \      / _|
//	_| |  | | ___| |_ ___ _ __  ___  ___
//	| | |  | |/ _ \  _/ _ \ '_ \/ __|/ _ \
//	| | |__| |  __/ ||  __/ | | \__ \  __/
//	|_|_____/ \___|_| \___|_| |_|___/\___|
//
//  iDFWoker.cpp
//  iDFService
//
//  Created by Axis on 16/3/9.
//  Copyright © 2016年 Axis. All rights reserved.
//

#include "iDFWoker.hpp"
#include <util/common.h>

void iDFWorker::clean(int* client_sock, void **out, void **in)
{
    if(*client_sock>0)
    {
        this->ipc->close(*client_sock);
        *client_sock=-1;
    }
    
    if(*out)
    {
        free(*out);
        *out=NULL;
    }
    
    if(*in)
    {
        free(*in);
        *in=NULL;
    }
    
    return ;
}

void iDFWorker::run()
{
    int client_sock=-1;
    RequestHeader req = {};
    ResponseHeader res = {};
    int req_len __unused = -1;
    int res_len __unused = -1;
    void * in_data=NULL;
    void * out_data=NULL;
    int in_len=0;
    int out_len = 0;
    int error_code = -1;
    
    while (!this->is_stop())
    {
        
        this->clean(&client_sock, &out_data, &in_data);     //clean all the memory
        
        if(server==NULL)
        {
            break;
        }
        
        if((client_sock=this->server->get_client_sock())<0)
        {
            usleep(50000);  //10ms
            continue;
        }
        
        printf("[start working]\n\n\n");
        fflush(stdout);
        
        error_code=ipc->recv(client_sock, &req, sizeof(req), &in_data, &in_len);
        if(error_code<0)
        {
            printf("error code is %d", error_code);
            fflush(stdout);
        }
        
        int cmd = req.cmd;
        PluginTypeID plugin_id = GET_PLUGIN_ID(cmd);
        
        if(plugin_id==IDF_PLUGIN_ALL)
        {
            switch (cmd) {
                case IDF_START:
                    this->mgr->run_all_plugins();
                    break;
                case IDF_STOP:
                    this->mgr->stop_all_plugins();
                    break;
                default:
                    printf("unknown cmd");
                    fflush(stdout);
                    break;
            }
        }
        else
        {
            iDFPluginWrapper *plugin = NULL;
            IDF_ERRORCODE error_code = IDF_SUCCESS;
            error_code = this->mgr->get_plugin(plugin_id, &plugin);
            
            if(IDF_IS_ERROR(error_code) || !plugin || !plugin->is_valid() || !plugin->is_run())
            {
                printf("the plugin dose not run or is invalid, plugin is is %d", plugin_id);
                fflush(stdout);
                continue;
//#warning doese not collect the mmeory
            }
            
            error_code = plugin->send_command(cmd, in_data, in_len, &out_data, &out_len);
//            if(IDF_IS_ERROR(error_code))
//            {
//                printf("there is an error when send the command to plugin");
//                fflush(stdout);
//                continue;
//            }
            res.status = error_code;
            res.cmd = cmd;
//            
//            if(out_data==NULL)
//            {
//                printf("there is no any data to send, quit now!");
//                continue;
//            }
            
//            // test code
//            out_data = new char[100];
//            bzero(out_data, 100*sizeof(char));
//            
//            strcpy((char*)out_data, "test");
//            
//            out_len = strlen((char*)out_data);
//            // test code
            
            
            int ret=-1;
            if((ret = ipc->send(client_sock, &res, sizeof(res), out_data, out_len))<0)
            {
                printf("there is error to send the command, error code is %d", ret);
                fflush(stdout);
                continue;
            }
        }//else
    }
    
    this->clean(&client_sock, &out_data, &in_data);     //clean all the memory
    printf("pid=%p, stop completely\n\n", this->ppid);
}

void iDFWorker::stop()
{
    printf("pid=%p is stop\n\n", this->ppid);
    this->m_stop=true;
}