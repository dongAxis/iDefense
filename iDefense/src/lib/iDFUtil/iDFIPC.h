//
//  iDFIPC.h
//  iDFUtil
//
//  Created by Axis on 16/3/2.
//  Copyright © 2016年 Axis. All rights reserved.
//

#ifndef iDFIPC_h
#define iDFIPC_h

#include "Visibility_push.h"

namespace idf
{
    namespace ipc
    {
#warning 4 bytes alignment, maybe 8 bytes?
//#pragma pack(4)
        struct RequestHeader
        {
            int cmd;
            char version[32];
        };
        
        struct ResponseHeader
        {
            int cmd;
            char version[32];
        };
//#pragma unpack()
        
        class iDFIPC
        {
        public:
            int listen(const char *node) const;
            int connect(const char *node) const;
            int accept(int sock);
            void close(int sock);
            int send(int sock, const void* header, int headerLen, const void* msg, int msgLen);
            int recv(int sock, void* header, int headerLen, void** msg, int* msgLen);  //header len always same
        };
    }
}



#include "Visibility_pop.h"

#endif /* iDFIPC_h */
