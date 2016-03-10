//	_ _____        __
//	(_)  __ \      / _|
//	_| |  | | ___| |_ ___ _ __  ___  ___
//	| | |  | |/ _ \  _/ _ \ '_ \/ __|/ _ \
//	| | |__| |  __/ ||  __/ | | \__ \  __/
//	|_|_____/ \___|_| \___|_| |_|___/\___|
//
//  iDFWoker.hpp
//  iDFService
//
//  Created by Axis on 16/3/9.
//  Copyright © 2016年 Axis. All rights reserved.
//

#ifndef iDFWoker_hpp
#define iDFWoker_hpp

#include <util/iDFThread.h>
#include <string>
#include <vector>
#include <util/iDFIPC.h>
#include <util/iDFLock.h>
#include "iDFServer.hpp"
#include "iDFPluginMgr.hpp"

using namespace idf;
using namespace idf::ipc;

class iDFWorker : public iDFRunable
{
public:
    iDFWorker(const iDFServer *server, int id, iDFPluginMgr* mgr)
            :server(server), is_valid(false), id(id), mgr(mgr), ppid(pthread_self())
    {
        this->m_stop=false;
        
        if(server==NULL)
            return ;
        
        ipc = new iDFIPC();
        if(ipc==NULL) return ;
        
        is_valid=true;
    }
    
    virtual ~iDFWorker()
    {
        if(!is_stop())
            stop();
        
        if(ipc)
        {
            delete ipc;
            ipc=NULL;
        }
    }
    
    virtual void run();
    virtual void stop();
    
    __attribute__((always_inline)) bool is_stop()
    {
        return this->m_stop;
    }
    
    void clean(int* client_sock, void **out, void **in);
    
private:
    const iDFServer *server;
    bool is_valid;
    iDFIPC *ipc;
    int id;
    iDFPluginMgr *mgr;
    pthread_t ppid;
};

#endif /* iDFWoker_hpp */
