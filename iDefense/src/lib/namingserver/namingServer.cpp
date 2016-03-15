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
#include <util/iDFCommandSet.h>
#include <util/iDFLog.h>

#include "namingServer.h"
#include "namingServerCallback.hpp"

//extern std::vector<struct register_data*> plugins;
//extern idf::RWLock rw_lock;

iDFLogContext *context;

IDF_ERRORCODE initPlugin()
{
    printf("[+]%s\n", __FUNCTION__);
    fflush(stdout);
    
    context = new iDFLogContext("/var/log/iDF/nameserver.log");
    
    return 0;
}


IDF_ERRORCODE startPlugin()
{
    printf("[+]%s\n", __FUNCTION__);
    fflush(stdout);
    
    IDF_ERROR("%s", "nimei  test log");
    
    return 0;
}

IDF_ERRORCODE stopPlugin()
{
    printf("[+]%s\n", __FUNCTION__);
    fflush(stdout);
    
    if(context) delete context;
    
    return 0;
}


IDF_ERRORCODE sendCommand(int cmd, void* in_data, uint64_t in_len, void** out_data, uint64_t *out_len)
{
    printf("[+]%s\n", __FUNCTION__);
    fflush(stdout);
    
    for(int i=0; i<5; i++)
    {
        if(handler[i].cmd==cmd)
        {
            return (*handler[i].ptr)(cmd, in_data, in_len, out_data, out_len);
        }
    }
    
    return IDF_PLUGIN_NON_EXISTED;
}

PluginTypeID getPluginId()
{
    return IDF_PLUGIN_NAMINGSERVICE;
}

Handler handler[] =
{
    {
        .cmd=IDF_NAMINGSERVICE_REGISTER_PLUGIN,
        .desc=(char*)"register the plugin",
        .ptr = &register_plugin_cb
    },
    {
        .cmd=IDF_NAMINGSERVICE_CHECK_PLUGIN_ADDR,
        .desc=(char*)"check plugin's address",
        .ptr = &check_plugin_addr_cb
    },
    {
        .cmd=IDF_NAMINGSERVICE_UNREGISTER_PLUGIN,
        .desc=(char*)"unregister plugin",
        .ptr = &unregister_plugin_cb
    },
    {
        .cmd=IDF_NAMINGSERVICE_GET_ALL_INFO,
        .desc=(char*)"get all plugin desc",
        .ptr = &get_all_register_plugin
    },
    {
        .cmd=IDF_NAMINGSERVICE_CHECK_STATUS,
        .desc=(char*)"check plugin status",
        .ptr=check_plugin_status
    }
};