//
//  iDFCommandSet.h
//  iDFUtil
//
//  Created by Axis on 16/3/3.
//  Copyright © 2016年 Axis. All rights reserved.
//

#ifndef iDFCommandSet_h
#define iDFCommandSet_h

enum PluginSet
{
    IDF_PLUGIN1=0,
    IDF_PLUGIN2,
    IDF_PLUGIN_TASKMANAGER,
    IDF_PLUGIN_NOTIFICATION,
    IDF_PLUGIN_DB,
    
    IDF_PLUGIN_MAX
};

//command
#warning test command, we will modify it when the plugin is ready
enum PLuginCommand
{
    IDF_PLUGIN1_ACTION1 = (1 << 8) << IDF_PLUGIN1,
    IDF_PLUGIN1_ACTION2,
    
    
    IDF_PLUGIN2_ACTION1 = (1 << 8) << IDF_PLUGIN2,
    IDF_PLUGIN2_ACTION2,
};


#endif /* iDFCommandSet_h */
