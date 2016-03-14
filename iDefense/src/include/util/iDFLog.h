//
//  iDFLog.h
//  iDFUtil
//
//  Created by Axis on 16/3/14.
//  Copyright © 2016年 Axis. All rights reserved.
//

#ifndef iDFLog_h
#define iDFLog_h

#include "iDFLock.h"
#include <stdio.h>
enum iDFLogLevel
{
    IDF_ERROR,
    IDF_WARNING,
    IDF_INFO,
    IDF_DEBUG,
};

class iDFLogContext;
extern iDFLogContext *context;

#if DEBUG

#define IDF_ERROR(format, data)    \
        context->write(IDF_ERROR, (format), data);

#define IDF_WARNING()
#define IDF_INFO()
#define IDF_DEBUG()

#else

#define IDF_ERROR()
#define IDF_WARNING()
#define IDF_INFO()
#define IDF_DEBUG()

#endif

class iDFLogContext
{
public:
    iDFLogContext(std::string path);
    ~iDFLogContext();
    bool isvalid() {return is_invalid;}
    bool set_debug_level(int level);
    bool write(int level, char* format, char* data);
private:
    bool is_invalid;
    std::string path;
    idf::MutexLock mutex;
    int fd;
    int current_level;
};


#endif /* iDFLog_h */
