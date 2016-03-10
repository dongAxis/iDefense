//
//  iDFPlugin.h
//  iDFUtil
//
//  Created by Axis on 16/3/3.
//  Copyright © 2016年 Axis. All rights reserved.
//

#ifndef iDFPlugin_h
#define iDFPlugin_h

#include <cstdio>
#include <sys/types.h>

#include "Visibility_push.h"
#include "iDFErrorCode.h"
#include "iDFCommandSet.h"

#if __cplusplus
extern "C" {
#endif

    typedef IDF_ERRORCODE (*CommCB_ptr)(int cmd, void* in_data, uint64_t in_len, void** out_data, uint64_t *out_len);
    typedef struct handler
    {
        int cmd;
        char* desc[1024];
        CommCB_ptr ptr;
    }Handler;

    IDF_ERRORCODE initPlugin();
    IDF_ERRORCODE startPlugin();
    IDF_ERRORCODE stopPlugin();
    IDF_ERRORCODE sendCommand(int cmd, void* in_data, uint64_t in_len, void** out_data, uint64_t *out_len);
    PluginTypeID getPluginId();

    extern "C" Handler handler[];

#if __cplusplus
}
#endif

#include "Visibility_pop.h"

#endif /* iDFPlugin_h */
