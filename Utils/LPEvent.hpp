//
//  LPEvent.hpp
//  WalktourPackControl
//
//  Created by Ferris on 2018/9/10.
//  Copyright © 2018年 DingLi. All rights reserved.
//

#ifndef LPEvent_hpp
#define LPEvent_hpp

#include <stdio.h>
#include "threadpool.h"
#include <pthread.h>
#include "LPCommon.h"

typedef enum
{
    LP_EVENT_TYPE_READ = 0,
    LP_EVENT_TYPE_WRITE,
    LP_EVENT_TYPE_ERROR
}CLPEventType;

typedef void (__stdcall *LPEventHanler)(void *event, void* userInfo);

typedef struct{
    LPEventHanler handler;
    void *userinfo;
} LPEventHandlerContext;

class CLPEvent {
public:
    void setEventHandler(LPEventHanler handler,void *userInfo);
    void setCancleHandler(LPEventHanler handler,void *userInfo);
    void resume();
    void suspend();
    void cancel();
    int getHandel();
    static CLPEvent *createEvent(int type,int handle,threadpool_t *pool);
    static void destoryEvent(CLPEvent *);
    void ProcessEvent(int state);
private:
    CLPEvent(int type,int handle,threadpool_t *pool);
    ~CLPEvent();
private:
    int nHandle;
    int nType;
    threadpool_t *pPool;
    LPEventHandlerContext handlerCtx;
    LPEventHandlerContext cancelCtx;
    int nState;
    pthread_mutex_t mutex_lock;
};


#endif /* LPEvent_hpp */
