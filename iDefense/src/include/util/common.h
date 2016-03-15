//
//  common.h
//  iDFUtil
//
//  Created by Axis on 16/3/2.
//  Copyright © 2016年 Axis. All rights reserved.
//

#ifndef common_h
#define common_h

#include "iDFCommandSet.h"

#undef __ON__
#define __ON__      1

#undef __OFF__
#define __OFF__     0

#define NAMING_SERVER_PORT  "1001"

#define SAFE_CF_RELEASE(x)            \
        do {                          \
            if((x)) CFRelease((x));   \
        }while(0);

#define GET_PLUGIN_ID(cmd)  ((cmd)>>8)

//register data
struct register_data
{
    PluginTypeID type;
    char node[32];
};

struct task_percent
{
    int task_id;
    int percent; //0--100
    char task_desc[128];
};

struct check_status
{
    PluginTypeID type;
};

#endif /* common_h */
