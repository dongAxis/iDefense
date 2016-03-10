//	_ _____        __
//	(_)  __ \      / _|
//	_| |  | | ___| |_ ___ _ __  ___  ___
//	| | |  | |/ _ \  _/ _ \ '_ \/ __|/ _ \
//	| | |__| |  __/ ||  __/ | | \__ \  __/
//	|_|_____/ \___|_| \___|_| |_|___/\___|
//
//  namingServerCallback.hpp
//  namingserver
//
//  Created by Axis on 16/3/10.
//  Copyright © 2016年 Axis. All rights reserved.
//

#ifndef namingServerCallback_hpp
#define namingServerCallback_hpp

#include <cstdio>
#include <sys/types.h>
#include <util/iDFErrorCode.h>
#include <util/iDFCommandSet.h>
#include <util/common.h>

IDF_ERRORCODE register_plugin_cb(int cmd, void* in_data, uint64_t in_len, void** out_data, uint64_t *out_len);
IDF_ERRORCODE check_plugin_addr_cb(int cmd, void* in_data, uint64_t in_len, void** out_data, uint64_t *out_len);
IDF_ERRORCODE unregister_plugin_cb(int cmd, void* in_data, uint64_t in_len, void** out_data, uint64_t *out_len);
IDF_ERRORCODE get_all_register_plugin(int cmd, void* in_data, uint64_t in_len, void** out_data, uint64_t *out_len);

#endif /* namingServerCallback_hpp */
