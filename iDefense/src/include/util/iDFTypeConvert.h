//
//  iDFTypeConvert.h
//  iDFUtil
//
//  Created by Axis on 16/3/4.
//  Copyright © 2016年 Axis. All rights reserved.
//

#ifndef iDFTypeConvert_h
#define iDFTypeConvert_h

#include <CoreFoundation/CoreFoundation.h>
#include <sys/types.h>
#include <string>

#include "common.h"

/**
 *  there are two common field, 
 *  one is pb_type, which is basic cell of protobuf;
 *  the other is plist_type_t, which is basic cell of CF object
 *  the basic cell contain uint32_t, int32_t, std::string, 
 */
//lhs---> value, rhs--->CFDictionaryRef
#define conver_from_pb_to_simplecf(key)                \
            convert(lhs.key(), (char*)#key, rhs)

//lhs--> directory, rhs-->simple obj
#define convert_from_simplecf_to_pb(key, type)        \
        do {                                          \
            char *ckey = (char*)#key;                        \
            type value;                               \
            if(convert(lhs, ckey, value)){            \
                rhs.set_##key(value);                 \
            }                                         \
        }while(0)

//#define convert_from_pb_to_dict()
//#define conver_from_dict_to_pb()

namespace idf {

    /// those function is aimed to the target that convert between pb and c++ object
    /// types table of protobuf is here: https://developers.google.com/protocol-buffers/docs/proto
    /// but it is too complicated, so i just handle the following types;
    template <typename T>   class TypeTrail {};

    //int32_t
    template <>
    class TypeTrail<int32_t>
    {
    public:
        typedef int32_t         pb_type;
        typedef CFNumberRef     plist_type_t;
        
        static CFTypeID type_id;
        static CFNumberType sub_num_type;
    };
    CFTypeID TypeTrail<int32_t>::type_id=CFNumberGetTypeID();
    CFNumberType TypeTrail<int32_t>::sub_num_type = kCFNumberSInt32Type;

    //uint32_t
    template<>
    class TypeTrail<uint32_t>
    {
    public:
        typedef uint32_t     pb_type;
        typedef CFNumberRef  plist_type_t;
        
        static CFTypeID      type_id;
        static CFNumberType  sub_num_type;
    };
    CFTypeID TypeTrail<uint32_t>::type_id = CFNumberGetTypeID();
    CFNumberType TypeTrail<uint32_t>::sub_num_type = kCFNumberSInt32Type;

    //string
    template<>
    class TypeTrail<std::string>
    {
    public:
        typedef std::string pb_list;
        typedef CFStringRef plist_type_t;
        
        static CFTypeID type_id;
    };
    CFTypeID TypeTrail<std::string>::type_id = CFStringGetTypeID();
    
    //for handle <dict> tag
    template<>
    class TypeTrail<CFDictionaryRef>
    {
    public:
        typedef CFDictionaryRef   plist_type_t;
        static CFTypeID type_id;
    };
    CFTypeID TypeTrail<CFDictionaryRef>::type_id = CFDictionaryGetTypeID();

    //for handle <array> tag
    template<>
    class TypeTrail<CFArrayRef>
    {
    public:
        typedef CFArrayRef      plist_type_t;
        static  CFTypeID        type_id;
    };
    CFTypeID TypeTrail<CFArrayRef>::type_id = CFArrayGetTypeID();

#pragma mark - basic convert
    /*
     * if the following function return true, it means the function is successfully executed;
     * if false, the function occur error.
     */
    //Number
    template<typename T>
    bool convert(T lhs, CFNumberRef& rhs);      //int32_t, uint32_t---->CFNumberRef
    
    template<typename T>
    bool convert(CFNumberRef lhs, T& rhs);      //CFnumberRef ----> int32_t, uint32_t
    
    //Dictionary
    template<typename T>
    bool convert(T value, char* key, CFMutableDictionaryRef &dict);       //put a value map to ${dict}
    
    template<typename T>
    bool convert(CFDictionaryRef dict, char* key, T& values);       //put a value map to object
    
    bool convert(std::string lhs, CFStringRef& rhs);

    bool convert(CFStringRef lhs, std::string& rhs);
    
#pragma mark - start to implements the function declararction
    /**
     *  type is uint32_t, int32_t
     *
     *  @param lhs
     *  @param rhs
     *
     *  @return true if succeed, or false if failed
     */
    template<typename T>
    bool convert(T lhs, CFNumberRef& rhs)       //int32_t, uint32_t---->CFNumberRef
    {
//        TypeTrail<T> value;     //get the type info of {typename T}
        if(TypeTrail<T>::type_id != CFNumberGetTypeID())
            return false;
        
        rhs = CFNumberCreate(kCFAllocatorDefault, TypeTrail<T>::sub_num_type, &lhs);
        if(rhs==NULL)
            return false;
        
        return true;
    }
    
    template<typename T>
    bool convert(CFNumberRef lhs, T& rhs)
    {
        if(TypeTrail<T>::type_id != CFNumberGetTypeID())
            return false;
    
        if(CFNumberGetValue(lhs, TypeTrail<T>::sub_num_type, &rhs)==false)
            return false;
        
        return true;
    }
    
    template<typename T>
    bool convert(T value, char* key, CFMutableDictionaryRef &dict)
    {
        if(!key) return false;
        
        CFStringRef cfkey = CFStringCreateWithCString(kCFAllocatorDefault, key, kCFStringEncodingUTF8);
        if(!cfkey)
            return false;
        
        typedef typename TypeTrail<T>::plist_type_t plist_type_t;
        plist_type_t plist_value;
        if(!convert(value, plist_value))
        {
            CFRelease(cfkey);
            return false;
        }

        CFDictionaryAddValue(dict, cfkey, (const plist_type_t)plist_value); //add value to dict
        
        SAFE_CF_RELEASE(cfkey);
        return true;
    }
    
    template<typename T>    //T is protobuf type
    bool convert(CFDictionaryRef dict, char* key, T& value)
    {
        if(!key)
            return false;
        
        CFStringRef cfkey = CFStringCreateWithCString(kCFAllocatorDefault, key, kCFStringEncodingUTF8);
        if(!cfkey)
            return false;
        
        CFTypeRef type = CFDictionaryGetValue(dict, cfkey);
        if(!type)
        {
            CFRelease(cfkey);
            return false;
        }
        
        typedef typename TypeTrail<T>::plist_type_t plist_type_t;
        
        if((CFGetTypeID(type)!=(TypeTrail<T>::type_id)) ||
           !convert((plist_type_t&)type, value))
        {
            CFRelease(cfkey);
            return false;
        }
        
        return true;
    }
    
    bool convert(CFStringRef lhs, std::string& rhs)
    {
        char buffer[CFStringGetLength(lhs)+1];
        bzero(buffer, sizeof(buffer));
        
        if(!CFStringGetCString(lhs, buffer, sizeof(buffer), kCFStringEncodingUTF8))
        {
            return false;
        }
        
        rhs=buffer;
        
        return true;
    }
    
    bool convert(std::string lhs, CFStringRef& rhs)
    {
        const char * str = lhs.c_str();
        
        rhs = CFStringCreateWithCString(kCFAllocatorDefault, str, kCFStringEncodingUTF8);
        if(!rhs)
            return false;
        
        return true;
    }
}



#endif /* iDFTypeConvert_h */
