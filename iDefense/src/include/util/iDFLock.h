//
//  iDFLock.h
//  iDFUtil
//
//  Created by Axis on 16/3/2.
//  Copyright © 2016年 Axis. All rights reserved.
//

#ifndef iDFLock_h
#define iDFLock_h

#include <pthread.h>
#include <sys/errno.h>
#include <unistd.h>
#include <iostream>

#include "iDFException.h"

namespace idf
{
    /*
     Lock Guard
     */
    template<typename T>
    class LockGuard
    {
    public:
        inline LockGuard(T& mutex) : m_mutex(mutex)
        {
//            std::cout << "begin" << std::endl;
            m_mutex.lock();
        }
        
        inline ~LockGuard()
        {
//            std::cout << "end" << std::endl;
            m_mutex.unlock();
        }
        
    private:
        LockGuard(T* mutex);    //copy constructor
        LockGuard();            //constructior
        
    private:
        T& m_mutex;
    };
    
    class NullLock
    {
    public:
        virtual void lock() {}
        virtual void unlock() {}
    };
    
    //mutex
    class MutexLock : public NullLock
    {
    public:
        MutexLock();
        ~MutexLock();
        void lock() override;
        void unlock() override;
        
    private:
        pthread_mutex_t mutex;
        
        // lock cann not copy value
        MutexLock(MutexLock& lock);
        MutexLock operator =(MutexLock& lock);
    };
    
    inline MutexLock::MutexLock()
    {
        int error_code = pthread_mutex_init(&this->mutex, NULL);
        for(; error_code==EAGAIN; error_code=pthread_mutex_init(&this->mutex, NULL))
        {
            usleep(1000);
        }
        
        if(error_code!=0) idf::throwf("init mutex in %s of %s, line %d", __FUNCTION__, __FILE__, __LINE__);
    }
    
    inline MutexLock::~MutexLock()
    {
        int error_code = pthread_mutex_destroy(&this->mutex);   //destroy mutex lock
        printf("error_code = %d", error_code);
    }
    
    inline void MutexLock::lock()
    {
        pthread_mutex_lock(&this->mutex);
    }
    
    inline void MutexLock::unlock()
    {
        pthread_mutex_unlock(&this->mutex);
    }
}

#endif /* iDFLock_h */
