//
//  iDFLog.h
//  iDFUtil
//
//  Created by Axis on 16/3/3.
//  Copyright © 2016年 Axis. All rights reserved.
//

#ifndef iDFLog_h
#define iDFLog_h
#include "Visibility_push.h"


#include <string>
#include <sys/stat.h>

#include "iDFException.h"
#include "iDFLock.h"

enum LogLevel
{
    IDF_ERROR=0,
    IDF_WARNING,
    IDF_INFO,
    IDF_DEBUG
};

#warning   我还没想好 怎么设计  先放一放
#if DEBUG

#define DEBUG_ERROR()

#else

#endif

class iDFLog
{
public:
    iDFLog sharedInstance();
private:
    iDFLog();
    iDFLog(iDFLog& right);
    iDFLog(enum LogLevel level, std::string config_path)
        : log_level(level), pid(getpid()), config_path(config_path)
    {
        idf::LockGuard<idf::MutexLock> Guard(mutex);
    }
    
private:
    std::string config_path;
    std::string log_path;
    enum LogLevel log_level;
    int pid;
    idf::MutexLock mutex;
};

#include "Visibility_pop.h"

#endif /* iDFLog_h */
