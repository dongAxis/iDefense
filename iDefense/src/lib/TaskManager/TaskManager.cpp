//	_ _____        __
//	(_)  __ \      / _|
//	_| |  | | ___| |_ ___ _ __  ___  ___
//	| | |  | |/ _ \  _/ _ \ '_ \/ __|/ _ \
//	| | |__| |  __/ ||  __/ | | \__ \  __/
//	|_|_____/ \___|_| \___|_| |_|___/\___|
//
//  namingServer.cpp
//  namingserver
//
//  Created by Axis on 16/3/8.
//  Copyright © 2016年 Axis. All rights reserved.
//

#include <util/iDFPlugin.h>
#include <stdio.h>
#include <vector>
#include <util/iDFLock.h>
#include <sys/errno.h>
#include <util/iDFCommandSet.h>

#include "TaskManagerCallback.hpp"

extern std::vector<struct task_percent*> task;
extern idf::RWLock rw_lock;

bool is_init=false;
bool is_run=true;

IDF_ERRORCODE initPlugin()
{
    printf("[+]%s\n", __FUNCTION__);
    fflush(stdout);
    
    {
        idf::LockGuard<idf::RWLock> Guard(rw_lock);
        task.clear();
    }
    
    is_init=true;
    is_run=false;
    
    return 0;
}


IDF_ERRORCODE startPlugin()
{
    if(!is_init)
    {
        printf("init failed");
        return -1;
    }
    
    printf("[+]%s\n", __FUNCTION__);
    fflush(stdout);
    
    is_run=true;
    
    return 0;
}

IDF_ERRORCODE stopPlugin()
{
    printf("[+]TASKManager %s\n", __FUNCTION__);
    fflush(stdout);
    
    is_init=false;
    is_run=false;
    
    return 0;
}


IDF_ERRORCODE sendCommand(int cmd, void* in_data, uint64_t in_len, void** out_data, uint64_t *out_len)
{
    printf("[+]%s\n", __FUNCTION__);
    fflush(stdout);
    
    if(!is_run || !is_init)
    {
        return -1;
    }
    
    for(int i=0; i<2; i++)
    {
        if(handler[i].cmd==cmd)
        {
            (*handler[i].ptr)(cmd, in_data, in_len, out_data, out_len);
        }
    }
    
    return 0;
}

PluginTypeID getPluginId()
{
    return IDF_PLUGIN_TASKMANAGER;
}

Handler handler[] =
{
    {
        .cmd=IDF_TASKMANAGER_ADD_TASK,
        .desc=(char*)"register the plugin",
        .ptr = &taskmanager_add_task_callback
    },
    {
        .cmd=IDF_TASKMANAGER_GET_STATUS,
        .desc=(char*)"check plugin's address",
        .ptr = taskmanager_get_status_callback
    },
    {
        .cmd=IDF_TASKMANAGER_SET_STATUS,
        .desc=(char*)"unregister plugin",
        .ptr = taskmanager_set_status_callback
    },
    {
        .cmd=IDF_TASKMANAGER_REMOVE_TASK,
        .desc=(char*)"unregister plugin",
        .ptr = taskmanager_remove_task_callback
    }
};