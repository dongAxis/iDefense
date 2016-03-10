//
//  iDFThread.h
//  iDFUtil
//
//  Created by Axis on 16/3/2.
//  Copyright © 2016年 Axis. All rights reserved.
//

#ifndef iDFThread_h
#define iDFThread_h

#include <pthread.h>

#include "iDFException.h"

#pragma GCC visibility push(default)

namespace idf
{
    enum Priority
    {
        priority_low=0,
        priority_default,
        priority_high
    };
    
    class iDFRunable
    {
    public:
        __attribute__((always_inline)) iDFRunable() { this->m_stop = true; }
        
        virtual void run()=0;
        virtual void stop()=0;
        
    protected:
        bool m_stop;
    };
    
    class iDFThread : public iDFRunable
    {
        typedef void * (*ThreadFunc_ptr)(void*);
    private:
        iDFThread operator=(const iDFThread& rhs);
        iDFThread(const iDFThread& rhs);
    public:
        inline iDFThread(ThreadFunc_ptr cb, void *arg, bool detach=false, enum Priority priority=priority_default)
            : cb(cb), cb_arg(arg), is_deatch(detach), pri(priority)
        {
            if(cb==nullptr) throwf("callback function is null");
            
            runable=NULL;
            init();
        }
        
        explicit inline iDFThread(iDFRunable *runable, bool detach=false, enum Priority priority=priority_default)
             :runable(runable), is_deatch(detach), pri(priority)
        {
            if(!runable) throwf("failed");
            
            cb=NULL;
            init();
        }
        
        virtual void run() override;
        virtual void stop() override;
        
        bool handleSignal();
        void handleAttr(pthread_attr_t *attr);
        void init();
        bool isExisted();
        void join();
        
        inline pthread_t getPid() { return this->pid; }
        
    private:
        ThreadFunc_ptr cb;
        void* cb_arg;
        bool is_deatch;
        enum Priority pri;
        pthread_t pid;
        iDFRunable *runable;
    };
}

#pragma GCC visibility pop

#endif /* iDFThread_h */
