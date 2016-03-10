//	_ _____        __
//	(_)  __ \      / _|
//	_| |  | | ___| |_ ___ _ __  ___  ___
//	| | |  | |/ _ \  _/ _ \ '_ \/ __|/ _ \
//	| | |__| |  __/ ||  __/ | | \__ \  __/
//	|_|_____/ \___|_| \___|_| |_|___/\___|
//
//  iDFPluginMgr.cpp
//  iDFService
//
//  Created by Axis on 16/3/8.
//  Copyright © 2016年 Axis. All rights reserved.
//

#include "iDFPluginMgr.hpp"

IDF_ERRORCODE iDFPluginMgr::add_plugin(std::string path)
{
    idf::LockGuard<idf::MutexLock> Guard(this->lock);
    
    std::vector<iDFPluginWrapper*>::const_iterator begin=this->plugins.begin();
    std::vector<iDFPluginWrapper*>::const_iterator end = this->plugins.end();
    
    for (; begin!=end; begin++)
    {
        if((*begin)->is_valid()&&(*begin)->get_plugin_path()==path) return IDF_PLUGIN_EXISTED;
    }
    
    iDFPluginWrapper *wrapper = new iDFPluginWrapper(path);
    
    if(!wrapper || !wrapper->is_valid())
    {
        return IDF_PLUGIN_OR_CALLBACK_INVALID;
    }
    
    plugins.push_back(wrapper);
    return IDF_SUCCESS;
}

IDF_ERRORCODE iDFPluginMgr::remove_plugin(PluginTypeID id)
{
    idf::LockGuard<idf::MutexLock> Guard(this->lock);
    
    std::vector<iDFPluginWrapper*>::iterator begin=this->plugins.begin();
    std::vector<iDFPluginWrapper*>::iterator end = this->plugins.end();
    
    for(;begin!=end;begin++)
    {
        iDFPluginWrapper * plugin = *begin;
        
        if(plugin->get_plugin_id()==id)
        {
            if(IDF_IS_ERROR(this->stop_plugin(id)))
            {
                return IDF_PLUGIN_CANNOT_STOP;
            }
            
            this->plugins.erase(begin);
            delete *begin;
            
            return IDF_SUCCESS;
        }
    }
    
    return IDF_PLUGIN_NO_PLUGIN;
}

IDF_ERRORCODE iDFPluginMgr::stop_plugin(PluginTypeID id)
{
    idf::LockGuard<idf::MutexLock> Guard(this->lock);
    
    std::vector<iDFPluginWrapper*>::iterator begin=this->plugins.begin();
    std::vector<iDFPluginWrapper*>::iterator end = this->plugins.end();
    
    for(;begin!=end;begin++)
    {
        iDFPluginWrapper * plugin = *begin;
        
        if(plugin->get_plugin_id()==id)
        {
            if(plugin->is_run())
            {
//                plugin->stop();
                for (int i=0; i<30; i++)
                {
                    IDF_ERRORCODE error_code = plugin->stop();
                    if(IDF_IS_ERROR(error_code)) continue;
                    else
                        return IDF_SUCCESS;
                }
                return IDF_PLUGIN_CANNOT_STOP;
            }
            printf("plugin dose not run");
            return IDF_SUCCESS;
        }
    }
    return IDF_PLUGIN_NO_PLUGIN;
}

IDF_ERRORCODE iDFPluginMgr::run_all_plugins()
{
    idf::LockGuard<idf::MutexLock> Guard(this->lock);
    
    std::vector<iDFPluginWrapper*>::iterator begin=this->plugins.begin();
    std::vector<iDFPluginWrapper*>::iterator end = this->plugins.end();
    
    for(; begin!=end; begin++)
    {
        iDFPluginWrapper * plugin = *begin;
        if(!plugin->is_valid() || plugin->is_run())
        {
            printf("skip the plugin, %d", plugin->get_plugin_id());
            continue;
        }
        
        IDF_ERRORCODE error_code = plugin->init() || plugin->start();
        if(IDF_IS_ERROR(error_code))
        {
            printf("plugin failed to start it~, skip it~, error code is %lld", error_code);
            continue;
        }
    }
    
    return IDF_SUCCESS;
}

IDF_ERRORCODE iDFPluginMgr::stop_all_plugins()
{
    idf::LockGuard<idf::MutexLock> Guard(this->lock);
    
    std::vector<iDFPluginWrapper*>::iterator begin=this->plugins.begin();
    std::vector<iDFPluginWrapper*>::iterator end = this->plugins.end();
    
    for(;begin != end; begin++)
    {
        iDFPluginWrapper * plugin = *begin;
        
        if(!plugin->is_valid() || !plugin->is_run())
        {
            printf("skip the plugin, %d", plugin->get_plugin_id());
            continue;
        }
        
        IDF_ERRORCODE error_code = plugin->stop();
        if(IDF_IS_ERROR(error_code))
        {
            printf("plugin failed to start it~, skip it~, error code is %lld", error_code);
            continue;
        }
    }
    
    return IDF_SUCCESS;
}

IDF_ERRORCODE iDFPluginMgr::get_plugin(PluginTypeID id, iDFPluginWrapper **wrapper)
{
    idf::LockGuard<idf::MutexLock> Guard(this->lock);
    
    std::vector<iDFPluginWrapper*>::iterator begin=this->plugins.begin();
    std::vector<iDFPluginWrapper*>::iterator end = this->plugins.end();
    
    for(;begin != end; begin++)
    {
        iDFPluginWrapper * plugin = *begin;
        
        if(plugin->get_plugin_id()==id)
        {
            *wrapper = plugin;
            return IDF_SUCCESS;
        }
    }
    
    return IDF_PLUGIN_NO_PLUGIN;
}