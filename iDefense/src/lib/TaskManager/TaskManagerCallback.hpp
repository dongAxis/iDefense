//	_ _____        __
//	(_)  __ \      / _|
//	_| |  | | ___| |_ ___ _ __  ___  ___
//	| | |  | |/ _ \  _/ _ \ '_ \/ __|/ _ \
//	| | |__| |  __/ ||  __/ | | \__ \  __/
//	|_|_____/ \___|_| \___|_| |_|___/\___|
//
//  TaskManagerCallback.hpp
//  TaskManager
//
//  Created by Axis on 16/3/10.
//  Copyright © 2016年 Axis. All rights reserved.
//

#ifndef TaskManagerCallback_hpp
#define TaskManagerCallback_hpp

#include <cstdio>
#include <sys/types.h>
#include <util/iDFErrorCode.h>

IDF_ERRORCODE taskmanager_add_task_callback(int cmd, void* in_data, uint64_t in_len, void** out_data, uint64_t *out_len);
IDF_ERRORCODE taskmanager_remove_task_callback(int cmd, void* in_data, uint64_t in_len, void** out_data, uint64_t *out_len);
IDF_ERRORCODE taskmanager_get_status_callback(int cmd, void* in_data, uint64_t in_len, void** out_data, uint64_t *out_len);
IDF_ERRORCODE taskmanager_set_status_callback(int cmd, void* in_data, uint64_t in_len, void** out_data, uint64_t *out_len);

#endif /* TaskManagerCallback_hpp */
