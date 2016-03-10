//	_ _____        __
//	(_)  __ \      / _|
//	_| |  | | ___| |_ ___ _ __  ___  ___
//	| | |  | |/ _ \  _/ _ \ '_ \/ __|/ _ \
//	| | |__| |  __/ ||  __/ | | \__ \  __/
//	|_|_____/ \___|_| \___|_| |_|___/\___|
//
//  iDFServer.hpp
//  iDFService
//
//  Created by Axis on 16/3/8.
//  Copyright © 2016年 Axis. All rights reserved.
//

#ifndef iDFServer_hpp
#define iDFServer_hpp

#include <util/iDFThread.h>
#include <string>
#include <vector>
#include <util/iDFIPC.h>
#include <util/iDFLock.h>

using namespace idf;
using namespace idf::ipc;

class iDFServer : public iDFRunable
{
public:
    iDFServer(std::string path) : sock_path(path), server_socket(-1)
    {
        this->m_stop=false;
        sock_array.clear();
    }
    
    virtual void run();
    virtual void stop();
    
#warning it might have some problems here, do some unit test to verify
    __attribute__((always_inline)) int get_client_sock() const
    {
        {
            idf::LockGuard<MutexLock> guard(vec_mutex);
            
            if(sock_array.empty()) return -1;
            
            std::vector<int>::const_iterator client_sock_iter = sock_array.begin();
            std::vector<int>::const_iterator client_sock_end = sock_array.end();
            if(client_sock_iter==client_sock_end) return -1;
            
            int client_sock = *client_sock_iter;
            sock_array.erase(client_sock_iter);
            
            return client_sock;
        }
    }
    
private:
    iDFIPC ipc;
    std::string sock_path;
    mutable std::vector<int> sock_array;
    mutable idf::MutexLock vec_mutex;
    int server_socket;
};

#endif /* iDFServer_hpp */
