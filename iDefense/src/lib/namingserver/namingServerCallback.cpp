//	_ _____        __
//	(_)  __ \      / _|
//	_| |  | | ___| |_ ___ _ __  ___  ___
//	| | |  | |/ _ \  _/ _ \ '_ \/ __|/ _ \
//	| | |__| |  __/ ||  __/ | | \__ \  __/
//	|_|_____/ \___|_| \___|_| |_|___/\___|
//
//  namingServerCallback.cpp
//  namingserver
//
//  Created by Axis on 16/3/10.
//  Copyright © 2016年 Axis. All rights reserved.
//

#include "namingServerCallback.hpp"
#include <util/iDFLock.h>
#include <vector>
#include <cstring>

//struct register_data plugins[32];
static std::vector<struct register_data*> plugins;
static idf::RWLock rw_lock;

IDF_ERRORCODE get_all_register_plugin(int cmd, void* in_data, uint64_t in_len, void** out_data, uint64_t *out_len)
{
    idf::LockGuard<idf::RWLock> guard(rw_lock, /*is_read=*/true);
    
    //iterator
    std::vector<struct register_data*>::const_iterator begin = plugins.begin();
    std::vector<struct register_data*>::const_iterator end = plugins.end();
    
    std::string str="";
    
    for(;begin!=end; begin++)
    {
        struct register_data* data = *begin;
        str+=data->type;
        str+=" ";
        
        str+=data->node;
        str+=",";
    }
    
    if(*out_data==NULL)
        *out_data = new char[strlen(str.c_str())+1];      //create heap
    
//    strncpy(*out_data, <#const char *#>, <#size_t#>)
    strncpy((char*)(*out_data), str.c_str(), (size_t)strlen(str.c_str()));
    
    *out_len = strlen(str.c_str());
    
    return IDF_SUCCESS;
}

IDF_ERRORCODE register_plugin_cb(int cmd, void* in_data, uint64_t in_len, void** out_data, uint64_t *out_len)
{
    fflush(stdout);
    if(in_data==NULL) return IDF_INVALID_PARAM;
    
    if(cmd!=IDF_NAMINGSERVICE_REGISTER_PLUGIN)
    {
//        if(in_data)
//            delete (struct register_data*)in_data;
        return IDF_COMMAND_NOT_RIGHT;
    }
    
    idf::LockGuard<idf::RWLock> guard(rw_lock, /*is_read=*/false);
    
    struct register_data* tmp_data =  new register_data(); //(struct register_data*)in_data;
    tmp_data->type =((struct register_data*)in_data)->type;
    
    printf("register;s node is %s\n\n", ((struct register_data*)in_data)->node);
    
    strncpy(tmp_data->node, ((struct register_data*)in_data)->node, sizeof(tmp_data->node));
    
    //iterator
    std::vector<struct register_data*>::const_iterator begin = plugins.begin();
    std::vector<struct register_data*>::const_iterator end = plugins.end();
    
//    for(int i=0; i<len; i++)
    for(;begin!=end; begin++)
    {
        struct register_data* data = *begin;
        if(data->type==tmp_data->type)
        {
//            delete tmp_data;
//            in_data=tmp_data=NULL;
            return IDF_PLUGIN_EXISTED;
        }
    }
    
    plugins.push_back(tmp_data);
    
    printf("[add plugin info]\n\n");
    printf("PluginTypeID: %d\n", tmp_data->type);
    printf("node: %s", tmp_data->node);
    
//    delete in_data;
    
    return IDF_SUCCESS;
}

IDF_ERRORCODE unregister_plugin_cb(int cmd, void* in_data, uint64_t in_len, void** out_data, uint64_t *out_len)
{
    if(in_data==NULL) return IDF_INVALID_PARAM;
    
    PluginTypeID type_id = *(PluginTypeID*)in_data;
    
    idf::LockGuard<idf::RWLock> guard(rw_lock, /*is_read=*/false);
    
    std::vector<struct register_data*>::const_iterator begin = plugins.begin();
    std::vector<struct register_data*>::const_iterator end = plugins.end();
    
    for(;begin!=end; begin++)
    {
        struct register_data* data = *begin;
        if(data->type==type_id)
        {
            plugins.erase(begin);
            delete data;
            return IDF_SUCCESS;
        }
    }
    
    return IDF_PLUGIN_NON_EXISTED;
}

IDF_ERRORCODE check_plugin_addr_cb(int cmd, void* in_data, uint64_t in_len, void** out_data, uint64_t *out_len)
{
    if(in_data==NULL) return IDF_INVALID_PARAM;
    
    PluginTypeID type_id = *(PluginTypeID*)in_data;
    
    idf::LockGuard<idf::RWLock> guard(rw_lock, /*is_read=*/true);
    
//    struct register_data* tmp_data = (struct register_data*)in_data;
    
    //iterator
    std::vector<struct register_data*>::const_iterator begin = plugins.begin();
    std::vector<struct register_data*>::const_iterator end = plugins.end();
    
    for(;begin!=end; begin++)
    {
        struct register_data* data = *begin;
        if(data->type==type_id)
        {
            if(*out_data==NULL)
                *out_data = new char[32];
            bzero(*out_data, sizeof(char)*32);
            strncpy((char*)(*out_data), (const char*)data->node, sizeof(char)*32);      //get node
            
            *out_len = strlen((char*)(*out_data));
            
//            delete (PluginTypeID*)in_data;
//            in_data = NULL;
            return IDF_SUCCESS;
        }
    }
    
//    if(in_data)
//    {
//        delete (PluginTypeID*)in_data;
//        in_data=NULL;
//    }
    
    return IDF_PLUGIN_NON_EXISTED;
}