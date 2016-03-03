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

enum LogLevel
{
    IDF_ERROR=0,
    IDF_WARNING,
    IDF_INFO,
    IDF_DEBUG
};

class iDFLog
{
public:
    iDFLog(std::string path, enum LogLevel level)
        :log_path(path), log_level(level)
    {
        
    }
    
private:
    std::string log_path;
    enum LogLevel log_level;
};

#include "Visibility_pop.h"

#endif /* iDFLog_h */
