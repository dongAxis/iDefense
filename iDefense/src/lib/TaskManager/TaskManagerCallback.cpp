//	_ _____        __
//	(_)  __ \      / _|
//	_| |  | | ___| |_ ___ _ __  ___  ___
//	| | |  | |/ _ \  _/ _ \ '_ \/ __|/ _ \
//	| | |__| |  __/ ||  __/ | | \__ \  __/
//	|_|_____/ \___|_| \___|_| |_|___/\___|
//
//  TaskManagerCallback.cpp
//  TaskManager
//
//  Created by Axis on 16/3/10.
//  Copyright © 2016年 Axis. All rights reserved.
//

#include "TaskManagerCallback.hpp"
#include <vector>
#include <util/common.h>
#include <util/iDFLock.h>
#include <util/iDFCommandSet.h>

std::vector<struct task_percent*> task;
idf::RWLock rw_lock;

IDF_ERRORCODE taskmanager_add_task_callback(int cmd, void* in_data, uint64_t in_len, void** out_data, uint64_t *out_len)
{
    if(in_data==NULL) return -1;
    
    PluginTaskType task_type = *(PluginTaskType*)in_data;
    if(!IS_VALID_TASK(task_type)) return -1;
    
    idf::LockGuard<idf::RWLock> Guard(rw_lock, /*is_read=*/false);
    std::vector<struct task_percent*>::const_iterator begin = task.begin();
    std::vector<struct task_percent*>::const_iterator end = task.end();
    
    for(; begin!=end; begin++)
    {
        struct task_percent * tmp_task = *begin;
        
        if(tmp_task->task_id==task_type)
        {
            return -1;
        }
    }
    
    struct task_percent* single_task = new struct task_percent();
    single_task->task_id=task_type;
    single_task->percent=0;
    bzero(single_task->task_desc, sizeof(single_task->task_desc));
    task.push_back(single_task);
    
    return IDF_SUCCESS;
}

IDF_ERRORCODE taskmanager_remove_task_callback(int cmd, void* in_data, uint64_t in_len, void** out_data, uint64_t *out_len)
{
    if(in_data==NULL) return -1;
    
    PluginTaskType task_type = *(PluginTaskType*)in_data;
    if(!IS_VALID_TASK(task_type)) return -1;
    
    idf::LockGuard<idf::RWLock> Guard(rw_lock, /*is_read=*/false);
    std::vector<struct task_percent*>::const_iterator begin = task.begin();
    std::vector<struct task_percent*>::const_iterator end = task.end();
    
    for (; begin!=end; begin++)
    {
        struct task_percent * tmp_task = *begin;
        
        if(tmp_task->task_id==task_type)
        {
            delete *begin;
            task.erase(begin);
            return IDF_SUCCESS;
        }
    }
    
    return -1;
}

IDF_ERRORCODE taskmanager_get_status_callback(int cmd, void* in_data, uint64_t in_len, void** out_data, uint64_t *out_len)
{
    if(in_data==NULL) return -1;
    
    PluginTaskType task_type = *(PluginTaskType*)in_data;
    if(!IS_VALID_TASK(task_type)) return -1;
    
    idf::LockGuard<idf::RWLock> Guard(rw_lock, /*is_read=*/true);
    std::vector<struct task_percent*>::const_iterator begin = task.begin();
    std::vector<struct task_percent*>::const_iterator end = task.end();
    
    for (; begin!=end; begin++)
    {
        struct task_percent * tmp_task = *begin;
        
        if(tmp_task->task_id==task_type)
        {
            if(*out_data==NULL)
                *out_data = new struct task_percent();
            
            struct task_percent *ptr = (struct task_percent*)*out_data;
            
            ptr->task_id = task_type;
            ptr->percent = tmp_task->percent;
            strncpy(ptr->task_desc, tmp_task->task_desc, sizeof(ptr->task_desc));
            
            *out_len = sizeof(struct task_percent);
            
            return IDF_SUCCESS;
        }
        
    }
    
    return -1;
}

IDF_ERRORCODE taskmanager_set_status_callback(int cmd, void* in_data, uint64_t in_len, void** out_data, uint64_t *out_len)
{
    if(in_data==NULL) return -1;
    
    struct task_percent * update_task = (struct task_percent *)in_data;
    if(!IS_VALID_TASK(update_task->task_id)) return -1;
    
    idf::LockGuard<idf::RWLock> Guard(rw_lock, /*is_read=*/false);
    std::vector<struct task_percent*>::const_iterator begin = task.begin();
    std::vector<struct task_percent*>::const_iterator end = task.end();
    
    for (; begin!=end; begin++)
    {
        struct task_percent * tmp_task = *begin;
        
        if(tmp_task->task_id==update_task->task_id)
        {
            tmp_task->percent = update_task->percent;
            if(strlen(update_task->task_desc)>0)
                strncpy(tmp_task->task_desc, update_task->task_desc, sizeof(tmp_task->task_desc));
            
            return IDF_SUCCESS;
        }
    }
    
    return -1;
}