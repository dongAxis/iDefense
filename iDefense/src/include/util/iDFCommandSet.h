//
//  iDFCommandSet.h
//  iDFUtil
//
//  Created by Axis on 16/3/3.
//  Copyright © 2016年 Axis. All rights reserved.
//

#ifndef iDFCommandSet_h
#define iDFCommandSet_h

//#if IDF_PLUGIN_MAX>8
//#error "the number of plugin is 8 at most"
//#endif

enum PluginSet
{
    IDF_PLUGIN1=(1<<0),
    IDF_PLUGIN2=(1<<1),
    IDF_PLUGIN_TASKMANAGER=(1<<2),
    IDF_PLUGIN_NOTIFICATION=(1<<3),
    IDF_PLUGIN_DB=(1<<4),
    IDF_PLUGIN_NAMINGSERVICE=(1<<5),
    IDF_PLUGIN_ALL=(1<<6),
    
    IDF_PLUGIN_MAX
};
typedef int PluginTypeID;
#define IS_VALID_PLUGIN_TYPE_ID(x)   ((x)>=0 && (x)<IDF_PLUGIN_MAX)


//command
#warning test command, we will modify it when the plugin is ready
enum PLuginCommand
{
    IDF_PLUGIN1_ACTION1 = (1 << IDF_PLUGIN1) << 8,
    IDF_PLUGIN1_ACTION2,
    
    
    IDF_PLUGIN2_ACTION1 = (1 << IDF_PLUGIN2) << 8,
    IDF_PLUGIN2_ACTION2,
    
    IDF_TASKMANAGER_ADD_TASK = (IDF_PLUGIN_TASKMANAGER<<8),
    IDF_TASKMANAGER_GET_STATUS,
    IDF_TASKMANAGER_SET_STATUS,
    IDF_TASKMANAGER_REMOVE_TASK,
    
    IDF_NAMINGSERVICE_REGISTER_PLUGIN = (IDF_PLUGIN_NAMINGSERVICE << 8),
    IDF_NAMINGSERVICE_CHECK_PLUGIN_ADDR,
    IDF_NAMINGSERVICE_UNREGISTER_PLUGIN,
    IDF_NAMINGSERVICE_GET_ALL_INFO,
    
    IDF_START = (1<<IDF_PLUGIN_ALL)<<8,
    IDF_STOP,
};

enum PluginTask
{
    IDF_TASK_TEST=1,
    
    IDF_TASK_MAX
};
typedef int PluginTaskType;
#define IS_VALID_TASK(x) ((x)>0 && (x)<IDF_TASK_MAX)

#endif /* iDFCommandSet_h */
