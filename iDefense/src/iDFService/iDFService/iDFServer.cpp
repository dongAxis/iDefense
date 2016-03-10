//	_ _____        __
//	(_)  __ \      / _|
//	_| |  | | ___| |_ ___ _ __  ___  ___
//	| | |  | |/ _ \  _/ _ \ '_ \/ __|/ _ \
//	| | |__| |  __/ ||  __/ | | \__ \  __/
//	|_|_____/ \___|_| \___|_| |_|___/\___|
//
//  iDFServer.cpp
//  iDFService
//
//  Created by Axis on 16/3/8.
//  Copyright © 2016年 Axis. All rights reserved.
//

#include <util/iDFIPC.h>
#include "iDFServer.hpp"

void iDFServer::run()
{
    if(sock_path=="") return ;
    
    this->m_stop=false;
    
    int sock = this->ipc.listen(sock_path.c_str());
    if(sock<=0)
    {
        return ;
    }
    
    server_socket = sock;
    
    while (!this->m_stop)
    {
        int new_socket = this->ipc.accept(sock);
        
        if(new_socket<0) continue;
        
        {
            idf::LockGuard<MutexLock> guard(vec_mutex);
            sock_array.push_back(new_socket);
        }
    }
}

void iDFServer::stop()
{
    printf("nimei\n\n\n");
    fflush(stdout);
    
    this->m_stop=true;
    
    {
        idf::LockGuard<MutexLock> guard(vec_mutex);
        
        sock_array.clear();
        
        if(server_socket>0)
        {
            this->ipc.close(server_socket);
            server_socket=-1;
        }
    }
}