//
//  iDFException.cpp
//  iDFUtil
//
//  Created by Axis on 16/3/3.
//  Copyright © 2016年 Axis. All rights reserved.
//
#include <cstdio>
#include <sys/types.h>
#include <stdarg.h>
#include "iDFException.h"

void throwf(const char *format, ...)
{
    va_list list;
    char *p=nullptr;
    
    va_start(list, format);
    vasprintf(&p, format, list);
    va_end(list);
    
    const char* t = p;
    throw t;
}