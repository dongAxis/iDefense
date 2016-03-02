//
//  Tests.m
//  Tests
//
//  Created by Axis on 16/3/2.
//  Copyright © 2016年 Axis. All rights reserved.
//

#import <XCTest/XCTest.h>

#include <iostream>
#include <string>
#include "iDFLock.h"
#include "iDFThread.h"

using namespace idf;

static void* callback(void* arg)
{
    int i=2;
    while(i--){ usleep(10000); }
    return nullptr;
}

//static void* callback(void* arg)
//{
//    std::cout << "in callback" << std::endl;
//    return nullptr;
//}

@interface Tests : XCTestCase

@end

@implementation Tests

- (void)setUp {
    [super setUp];
    // Put setup code here. This method is called before the invocation of each test method in the class.
}

- (void)tearDown {
    // Put teardown code here. This method is called after the invocation of each test method in the class.
    [super tearDown];
}

- (void)testThread
{
    iDFThread *thread = new iDFThread(&callback, NULL);
    XCTAssert(thread!=nullptr, "thread is null");
    
//    thread->interupt();
    thread->join();
    
}

-(void)testLock
{
    MutexLock test;
    LockGuard<MutexLock> lock(test);
    
    
}


@end
