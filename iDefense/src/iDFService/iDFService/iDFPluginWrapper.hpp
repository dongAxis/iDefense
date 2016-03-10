//	_ _____        __
//	(_)  __ \      / _|
//	_| |  | | ___| |_ ___ _ __  ___  ___
//	| | |  | |/ _ \  _/ _ \ '_ \/ __|/ _ \
//	| | |__| |  __/ ||  __/ | | \__ \  __/
//	|_|_____/ \___|_| \___|_| |_|___/\___|
//
//  iDFPluginWrapper.hpp
//  iDFService
//
//  Created by Axis on 16/3/8.
//  Copyright © 2016年 Axis. All rights reserved.
//

#ifndef iDFPluginWrapper_hpp
#define iDFPluginWrapper_hpp

#include <string>
#include <dlfcn.h>
#include <sys/types.h>
#include <util/iDFErrorCode.h>
#include <util/iDFConfigStatus.h>
#include <util/iDFCommandSet.h>

class iDFPluginWrapper
{
    typedef IDF_ERRORCODE (*init_plugin_ptr)();
    typedef IDF_ERRORCODE (*start_plugin_ptr)();
    typedef IDF_ERRORCODE (*stop_plugin_ptr)();
    typedef IDF_ERRORCODE (*send_command_plugin_ptr)(int cmd, void* in_data, uint64_t in_len, void** out_data, uint64_t *out_len);
    typedef PluginTypeID (*get_plugin_id_ptr)();
public:
    inline iDFPluginWrapper(std::string path) :
        plugin_path(path), isvalid(false), isrun(false)
    {
        if(access(plugin_path.c_str(), F_OK)!=0)
        {
            return;
        }
        
        void * handle = dlopen(plugin_path.c_str(), RTLD_LAZY);
        if(!handle)
        {
            return ;
        }
        
        //reslove symbols
        init_ptr=(init_plugin_ptr)dlsym(handle, "initPlugin");
        if(!init_ptr)
            return;
        
#if RUN_TEST_CODE
        (*init_ptr)();
#endif
        
        start_ptr = (start_plugin_ptr)dlsym(handle, "startPlugin");
        if(!start_ptr)
            return ;
 
#if RUN_TEST_CODE
        (*start_ptr)();
#endif
        
        stop_ptr = (stop_plugin_ptr)dlsym(handle, "stopPlugin");
        if(!stop_ptr)
            return ;
        
#if RUN_TEST_CODE
//        (*stop_ptr)();
#endif
        
        send_ptr = (send_command_plugin_ptr)dlsym(handle, "sendCommand");
        if(!send_ptr)
            return ;
#if RUN_TEST_CODE
        (*send_ptr)(1, NULL, 0, NULL, NULL);
#endif
        
        get_pluginid = (get_plugin_id_ptr)dlsym(handle, "getPluginId");
        if(!get_pluginid)
            return ;
        
        isvalid=true;
    }
    
    __attribute__((always_inline)) bool is_valid()
    {
        return isvalid;
    }
    
    __attribute__((always_inline)) IDF_ERRORCODE init()
    {
        if (!is_valid() || !init_ptr) {
            return IDF_PLUGIN_OR_CALLBACK_INVALID;
        }
        
        return (*init_ptr)();
    }
    
    __attribute__((always_inline)) IDF_ERRORCODE start()
    {
        if(!is_valid() || !start_ptr)
        {
            return IDF_PLUGIN_OR_CALLBACK_INVALID;
        }
        
        isrun=true;
        
        return (*start_ptr)();
    }
    
    __attribute__((always_inline)) IDF_ERRORCODE stop()
    {
        if(!is_valid() || !stop_ptr)
        {
            return IDF_PLUGIN_OR_CALLBACK_INVALID;
        }
        
        isrun=false;
        
        return (*stop_ptr)();
    }
    
    __attribute__((always_inline)) IDF_ERRORCODE send_command(int cmd, void* in_data, int in_len, void** out_data, int *out_len)
    {
        if(!is_valid() || !send_ptr)
        {
            return IDF_PLUGIN_OR_CALLBACK_INVALID;
        }
        
        return (*send_ptr)(cmd, in_data, in_len, out_data, (uint64_t*)out_len);
    }
    
    __attribute__((always_inline)) std::string get_plugin_path() { return this->plugin_path; }
    
    __attribute__((always_inline)) PluginTypeID get_plugin_id()
    {
        if(!is_valid() || !get_pluginid)
        {
            return IDF_PLUGIN_MAX;
        }
        
        return (*get_pluginid)();
    }
    
    __attribute__((always_inline)) bool is_run() { return isrun; }
    
    virtual ~iDFPluginWrapper()
    {
        isvalid=false;
    }
    
private:
    std::string plugin_path;
    bool isvalid;
    bool isrun;
    init_plugin_ptr init_ptr;
    start_plugin_ptr start_ptr;
    stop_plugin_ptr stop_ptr;
    send_command_plugin_ptr send_ptr;
    get_plugin_id_ptr get_pluginid;
};

#endif /* iDFPluginWrapper_hpp */
