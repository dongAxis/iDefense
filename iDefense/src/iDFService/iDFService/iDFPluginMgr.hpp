//	_ _____        __
//	(_)  __ \      / _|
//	_| |  | | ___| |_ ___ _ __  ___  ___
//	| | |  | |/ _ \  _/ _ \ '_ \/ __|/ _ \
//	| | |__| |  __/ ||  __/ | | \__ \  __/
//	|_|_____/ \___|_| \___|_| |_|___/\___|
//
//  iDFPluginMgr.hpp
//  iDFService
//
//  Created by Axis on 16/3/8.
//  Copyright © 2016年 Axis. All rights reserved.
//

#ifndef iDFPluginMgr_hpp
#define iDFPluginMgr_hpp

#include <vector>
#include <util/iDFCommandSet.h>
#include <util/iDFLock.h>

#include "iDFPluginWrapper.hpp"

class iDFPluginMgr
{
public:
    __attribute__((always_inline)) iDFPluginMgr()
    {
        idf::LockGuard<idf::MutexLock> guard(lock);
        
        plugins.clear();        //clear that array
    }
    
    IDF_ERRORCODE add_plugin(std::string path);
    IDF_ERRORCODE remove_plugin(PluginTypeID id);
    IDF_ERRORCODE run_all_plugins();
    IDF_ERRORCODE stop_all_plugins();
    IDF_ERRORCODE stop_plugin(PluginTypeID id);
    IDF_ERRORCODE get_plugin(PluginTypeID id, iDFPluginWrapper **wrapper);
    std::vector<iDFPluginWrapper*> get_all_plugins()
    {
        return this->plugins;
    }
private:
    std::vector<iDFPluginWrapper*> plugins;
    idf::MutexLock lock;
};

#endif /* iDFPluginMgr_hpp */
