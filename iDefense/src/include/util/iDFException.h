//
//  iDFException.h
//  iDFUtil
//
//  Created by Axis on 16/3/2.
//  Copyright © 2016年 Axis. All rights reserved.
//

#ifndef iDFException_h
#define iDFException_h

#include "Visibility_push.h"

extern "C" {

    void throwf(const char *format, ...);
}

#include "Visibility_pop.h"

#endif /* iDFException_h */
