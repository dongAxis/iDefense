//
//  iDFException.h
//  iDFUtil
//
//  Created by Axis on 16/3/2.
//  Copyright © 2016年 Axis. All rights reserved.
//_ _____        __
//(_)  __ \      / _|
//_| |  | | ___| |_ ___ _ __  ___  ___
//| | |  | |/ _ \  _/ _ \ '_ \/ __|/ _ \
//| | |__| |  __/ ||  __/ | | \__ \  __/
//|_|_____/ \___|_| \___|_| |_|___/\___|

#ifndef iDFException_h
#define iDFException_h

#include "Visibility_push.h"

extern "C" {

    void throwf(const char *format, ...);
}

#include "Visibility_pop.h"

#endif /* iDFException_h */
