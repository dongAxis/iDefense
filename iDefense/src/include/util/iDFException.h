//
//  iDFException.h
//  iDFUtil
//
//  Created by Axis on 16/3/2.
//  Copyright © 2016年 Axis. All rights reserved.
//

#ifndef iDFException_h
#define iDFException_h

#include <sys/types.h>

namespace idf
{
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
}

#endif /* iDFException_h */
