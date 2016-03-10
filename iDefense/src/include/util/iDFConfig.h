//
//  iDFConfig.h
//  iDFUtil
//
//  Created by Axis on 16/3/4.
//  Copyright © 2016年 Axis. All rights reserved.
//

#ifndef iDFConfig_h
#define iDFConfig_h

#include "iDFLock.h"
#include "iDFException.h"
#include "iDFTypeConvert.h"
//#include "IDF_LogMessage.pb.h"
#include "IDF_LogMessage.h"

#include <CoreFoundation/CoreFoundation.h>
#include "common.h"
#include <unistd.h>
#include <fstream>
#include <cstring>

namespace idf
{
    template<typename L>
    class iDFConfig
    {
    public:
        iDFConfig(std::string config_path);
        virtual ~iDFConfig()
        {
            
        }
        
        inline bool isValid()
        {
            idf::LockGuard<L> guard(mutex);
            return is_valid;
        }
        
        template<typename pb>
        bool get(std::string& group, pb & obj);
        
        template<typename pb>
        bool set(std::string& group, pb obj);
        
        bool save();
    private:
        std::string config_path;
        L mutex;
//        std::ifstream ifs;
        CFMutableDictionaryRef plist;
        bool is_valid;
    };
    
    template<typename L>
    iDFConfig<L>::iDFConfig(std::string config_path)  : config_path(config_path), is_valid(false)
    {
        if(config_path=="") throwf("config path is NULL");
        
        idf::LockGuard<L> guard(mutex);     //不准写~
        
        if(access(config_path.c_str(), F_OK)!=0)
        {
            throwf("%s dose not existed", config_path.c_str());
        }
        
        std::ifstream ifs(config_path);  //read file
        if(!ifs)
            throwf("file init error");
        if(!ifs.is_open()) throwf("file open error");
//        this->ifs = ifs;
        
        uint64_t length = ifs.seekg(0, std::ios::end).tellg();
        char content[length+1];
        bzero(content, sizeof(content));
        if(!ifs.seekg(0, std::ios::beg).read(content, length))
        {
            ifs.close();
            throwf("read config file error");
        }
        
        CFReadStreamRef stream = CFReadStreamCreateWithBytesNoCopy(kCFAllocatorDefault, (const UInt8 *)content, length, kCFAllocatorNull);
        if(!stream || !CFReadStreamOpen(stream))
        {
            ifs.close();
            throwf("get config or open stream failed");
        }
        
        CFPropertyListFormat format;
        CFErrorRef error;
        CFPropertyListRef plist = CFPropertyListCreateWithStream(kCFAllocatorDefault, stream, length, kCFPropertyListMutableContainers, &format, &error);
        
//        CFTypeID = //CFTYp
        
        if(!plist ||
           CFGetTypeID(plist)!=CFDictionaryGetTypeID() ||
           format!=kCFPropertyListXMLFormat_v1_0)
        {
            CFReadStreamClose(stream);
            SAFE_CF_RELEASE(stream);
            throwf("failed");
        }
        
        this->plist=(CFMutableDictionaryRef)plist;
        
        CFReadStreamClose(stream);
        SAFE_CF_RELEASE(stream);
        
        ifs.close();    //close file now!
        
        is_valid = true;
    }
    
    template<typename L>
    template<typename pb>
    bool iDFConfig<L>::get(std::string& group, pb &obj)
    {
        CFStringRef key;
        
        if(!isValid() || group=="" ||
           (key = CFStringCreateWithCString(NULL, group.c_str(), kCFStringEncodingUTF8))) return false;

        SAFE_CF_RELEASE(key);
        
        CFTypeRef type = CFDictionaryGetValue(this->plist, key);
        if(!type || CFGetTypeID(type)!=CFDictionaryGetTypeID()) return false;
        
        return convert((CFDictionaryRef)type, obj);
    }
    
    template<typename L>
    template<typename pb>
    bool iDFConfig<L>::set(std::string& group, pb obj)
    {
        CFStringRef key;
        
        if(!isValid() || group=="" ||
           (key = CFStringCreateWithCString(NULL, group.c_str(), kCFStringEncodingUTF8))==NULL) return false;
        
        SAFE_CF_RELEASE(key);
        
        //1. convert the pb object to dict
        CFMutableDictionaryRef dict = CFDictionaryCreateMutable(kCFAllocatorDefault, 0, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
        if(!dict || !convert(obj, dict))
        {
            return false;
        }
        
        //2. write the pb to the dict
        idf::LockGuard<L> Lock(this->mutex);
        CFStringRef cfkey = CFStringCreateWithCString(kCFAllocatorDefault, group.c_str(), kCFStringEncodingUTF8);
        if(!cfkey || !CFDictionaryContainsKey(this->plist, cfkey))
        {
            SAFE_CF_RELEASE(cfkey);
            SAFE_CF_RELEASE(dict);
            
            return false;
        }
        
        CFDictionaryRemoveValue(this->plist, cfkey);
        CFDictionaryAddValue(this->plist, cfkey, dict);

        SAFE_CF_RELEASE(dict);
        SAFE_CF_RELEASE(cfkey);
        
        return true;
    }
    
    template<typename L>
    bool iDFConfig<L>::save()
    {
        //1. dict ---> xml
        if(!isValid())
            return false;
        
        CFErrorRef error;
        CFDataRef new_data = CFPropertyListCreateData(kCFAllocatorDefault, (CFPropertyListRef)this->plist, kCFPropertyListXMLFormat_v1_0, kCFPropertyListMutableContainers, &error);
        if(!new_data) return false;
        
//        LockGuard<L> guard(this->mutex);
        
        std::ofstream ofs(config_path, std::ios_base::trunc | std::ios_base::out);
        
        if(!ofs.is_open())
        {
            SAFE_CF_RELEASE(new_data);
            throwf("file exception");
        }
        
        const UInt8 *c_data = CFDataGetBytePtr(new_data);
        
        if(!c_data ||
           !ofs.seekp(std::ios::beg) ||
           !ofs.write((char*)c_data, strlen((char*)c_data)))
        {
            SAFE_CF_RELEASE(new_data);
            throwf("file write failed");
        }
        
        SAFE_CF_RELEASE(new_data);
        
        ofs.close();
        
        return true;
    }
}


#endif /* iDFConfig_h */
