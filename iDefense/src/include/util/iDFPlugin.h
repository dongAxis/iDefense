//
//  iDFPlugin.h
//  iDFUtil
//
//  Created by Axis on 16/3/3.
//  Copyright © 2016年 Axis. All rights reserved.
//

#ifndef iDFPlugin_h
#define iDFPlugin_h

#include "Visibility_push.h"
#include "iDFErrorCode.h"
#include "iDFCommandSet.h"

namespace idf
{
    class IDFPlugin
    {
        typedef IDF_ERRORCODE (*CommCB_ptr)(int cmd, void* in_data, uint64_t in_len, void** out_data, uint64_t *out_len);
        typedef struct handler
        {
            int cmd;
            char* desc[1024];
            CommCB_ptr ptr;
        }Handler;
        
    public:
        virtual IDF_ERRORCODE initPlugin()=0;
        virtual IDF_ERRORCODE startPlugin()=0;
        virtual IDF_ERRORCODE stopPlugin()=0;
        virtual IDF_ERRORCODE sendCommand(int cmd)=0;
        
    protected:
        Handler *handler;
    };
}

#include "Visibility_pop.h"


#endif /* iDFPlugin_h */
