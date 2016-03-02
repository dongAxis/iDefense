//
//  iDFThread.cpp
//  iDFUtil
//
//  Created by Axis on 16/3/2.
//  Copyright © 2016年 Axis. All rights reserved.
//
#include <signal.h>
#include <stdio.h>
#include <string>

#include "iDFThread.h"

using namespace idf;

static void signal_handler(int signo)
{
    printf("signo is %d", signo);
}

static void* ThreadCallback(void* thread)
{
    ((iDFThread*)thread)->run();
    
    return nullptr;
}

bool iDFThread::handleSignal()
{
    struct sigaction sig = {};
    sig.sa_handler = signal_handler;
    sigemptyset(&sig.sa_mask);      //set empty for signal no
    sigdelset(&sig.sa_mask, SIGINT);
    sigdelset(&sig.sa_mask, SIGHUP);
    sigdelset(&sig.sa_mask, SIGTERM);
    
    if(sigaction(SIGUSR1, &sig, NULL)!=0)
    {
        return false;
    }
    return true;
}

void iDFThread::handleAttr(pthread_attr_t *attr)
{
    //1.init attr
    if(pthread_attr_init(attr)!=0) throwf("init attr failed");
    
    //2. set detach or disatach mode
    if(pthread_attr_setdetachstate(attr, this->is_deatch?PTHREAD_CREATE_DETACHED:PTHREAD_CREATE_JOINABLE)!=0)
        throwf("set thread status of detach");
    
    if(this->pri==priority_default) return ;    //if the priority is default, just return, we can not set the priority
    
    //3. get sche status struct
    struct sched_param sched_param = {};
    int error_code = pthread_attr_getschedparam(attr, &sched_param);
    
    //4. get thread's policy
    int policy=-1;
    error_code |= pthread_attr_getschedpolicy(attr, &policy);
    
    if(error_code) throwf("can not get sched policy value");
    
    //5. get min and max policy value & set it to the value
    int min = sched_get_priority_min(policy);
    int max = sched_get_priority_max(policy);
    
    if(this->pri==priority_low)
    {
        sched_param.sched_priority = min;
    }
    else
    {
        sched_param.sched_priority = max;
    }
    pthread_attr_setschedparam(attr, &sched_param);
    
    return ;
}

void iDFThread::init()
{
    /*
     1. handle signal;
     2. init pthread attr;
     3. create thread but run it.
     */
    
    this->handleSignal();       //handle signal
    
    pthread_attr_t attr;
    try
    {
        //1. handle attr
        this->handleAttr(&attr);    //handle attr
        
        //2,. create thread, we need to call function named "run"
        int error_code = pthread_create(&this->pid, &attr, ThreadCallback, this);
        if(error_code!=0) throwf("pthread_create failed, now we will throw it");
        
        pthread_attr_destroy(&attr);
    }
    catch (std::string exception)
    {
        pthread_attr_destroy(&attr);
    }
}

#warning "can not use this F, dose no test it"
bool iDFThread::isExisted()
{
    int ret = pthread_kill(this->pid, 0)==0;
    if(ret==0 && this->is_deatch) this->pid=0;
    
    return ret;
}

void iDFThread::join()
{
    if(!this->is_deatch)
    {
        pthread_join(this->pid, nullptr);
    }
}

void iDFThread::run()
{
    if(this->cb)
    {
        (this->cb)(this->cb_arg);
    }
    else if(this->runable)
    {
        this->runable->run();
    }
}


void iDFThread::stop()
{
    if(this->pid!=0)
    {
        this->m_stop=true;
    }
    
    if(this->runable) this->runable->stop();
    
}