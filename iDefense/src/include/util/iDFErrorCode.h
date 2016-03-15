//
//  iDFErrorCode.h
//  iDFUtil
//
//  Created by Axis on 16/3/2.
//  Copyright © 2016年 Axis. All rights reserved.
//

#ifndef iDFErrorCode_h
#define iDFErrorCode_h

typedef int IDF_ERRORCODE;

#define IDF_IS_ERROR(x) ((x)<IDF_SUCCESS)

#define IDF_SUCCESS     0

#define IDF_PLUGIN_OR_CALLBACK_INVALID      IDF_SUCCESS-1
#define IDF_PLUGIN_EXISTED                  IDF_SUCCESS-2
#define IDF_PLUGIN_CANNOT_STOP              IDF_SUCCESS-3
#define IDF_PLUGIN_NO_PLUGIN                IDF_SUCCESS-3
#define IDF_COMMAND_NOT_RIGHT               IDF_SUCCESS-4
#define IDF_INVALID_PARAM                   IDF_SUCCESS-5
#define IDF_PLUGIN_NON_EXISTED              IDF_SUCCESS-6

#endif /* iDFErrorCode_h */
