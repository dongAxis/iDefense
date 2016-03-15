//	_ _____        __
//	(_)  __ \      / _|
//	_| |  | | ___| |_ ___ _ __  ___  ___
//	| | |  | |/ _ \  _/ _ \ '_ \/ __|/ _ \
//	| | |__| |  __/ ||  __/ | | \__ \  __/
//	|_|_____/ \___|_| \___|_| |_|___/\___|
//
//  iDFLog.cpp
//  iDFUtil
//
//  Created by Axis on 16/3/14.
//  Copyright © 2016年 Axis. All rights reserved.
//
#include <string>
#include <util.h>
#include <fcntl.h>
#include <time.h>
#include "iDFLog.h"
#include "iDFLock.h"

#define LOG_FORMAT   "[time: %s][thread:%p] %s\n"

iDFLogContext::iDFLogContext(std::string path)
            : is_invalid(false), path(path), fd(-1), current_level(IDF_DEBUG)
{
    if(access(path.c_str(), F_OK)!=0)
    {
        printf("Log path do not existed");
        return ;
    }
    
    fd = open(path.c_str(), O_APPEND|O_RDWR);
    if(fd<0)
    {
        printf("Open error! quit now");
    }
    
    is_invalid=true;
}

iDFLogContext::~iDFLogContext()
{
    {
        idf::LockGuard<idf::MutexLock> guard(mutex);
        
        if(!is_invalid)
        {
            return ;
        }
        
        if(fd>0)
        {
            close(fd);
        }
    }
}

bool iDFLogContext::set_debug_level(int level)
{
    idf::LockGuard<idf::MutexLock> guard(mutex);
    
    (current_level=level);
    return true;
}

bool iDFLogContext::write(int level, char* format, char* data)
{
    idf::LockGuard<idf::MutexLock> guard(mutex);
    
    if(current_level<level) return false;
    if(!this->isvalid()) return false;
    
//    std::string param_str="";
//    va_list list;
//    char * str=NULL;
    time_t time_ptr;
    
//    va_start(list, format);
//    
//    str = va_arg(list, char*);
//    for(int i=0; str!=NULL; i++)
//    {
//        str = va_arg(list, char*);
//        param_str+=str;
//        param_str+=",";
//    }
//    va_end(list);
    
    char log[2048];
    char *timer=NULL;
    size_t len=-1;
    bzero(log, sizeof(log));
    
    time(&time_ptr);
    timer = asctime(gmtime(&time_ptr));
    len = strlen(timer);
    if(timer[len-2]=='\r' || timer[len-2]=='\n') timer[len-2]='\0';
    else if(timer[len-1]=='\r' || timer[len-1]=='\n') timer[len-1]='\0';
    
    sprintf(log, LOG_FORMAT, timer, pthread_self(), data);
    
    ::write(fd, log, strlen(log)+1);
    printf("hererhere");
    fflush(stdout);
    fsync(fd);
    
    return true;
}