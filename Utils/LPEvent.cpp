//
//  LPEvent.cpp
//  WalktourPackControl
//
//  Created by Ferris on 2018/9/10.
//  Copyright © 2018年 DingLi. All rights reserved.
//

#include "LPEvent.hpp"
#include <sys/select.h>
#include <stdlib.h>


enum CLPEventState
{
    kLPEventState_cancle = -1,
    kLPEventState_suspend = 0,
    kLPEventState_resume,
    kLPEventState_work,
};

#define HANDLE_NULL -1

void CLPEventThread(void *handle)
{
    CLPEvent *event = (CLPEvent *)handle;
    event->ProcessEvent(kLPEventState_work);
}

CLPEvent::CLPEvent(int type,int handle,threadpool_t *pool)
{
    nType = type;
    nHandle = handle;
    pPool = pool;
    nState = kLPEventState_suspend;
    handlerCtx.handler = 0;
    handlerCtx.userinfo = 0;
    cancelCtx.handler = 0;
    cancelCtx.userinfo = 0;
    pthread_mutex_init(&mutex_lock, NULL);
}

CLPEvent::~CLPEvent()
{
    pthread_mutex_lock(&mutex_lock);
    handlerCtx.handler = 0;
    handlerCtx.userinfo = 0;
    cancelCtx.handler = 0;
    cancelCtx.userinfo = 0;
    nState = kLPEventState_cancle;
    nHandle = HANDLE_NULL;
    pthread_mutex_unlock(&mutex_lock);
    pthread_mutex_destroy(&mutex_lock);
}

CLPEvent * CLPEvent::createEvent(int type, int handle, threadpool_t *pool)
{
    CLPEvent *evnet = new CLPEvent(type,handle,pool);
    return evnet;
}

void CLPEvent::destoryEvent(CLPEvent *event)
{
    if (event) {
        delete event;
    }
}

void CLPEvent::setEventHandler(LPEventHanler handler, void *userInfo)
{
    pthread_mutex_lock(&mutex_lock);
    handlerCtx.handler = (handler != NULL)?handler:0;
    handlerCtx.userinfo = (userInfo != NULL)?userInfo:0;
    pthread_mutex_unlock(&mutex_lock);
}

void CLPEvent::setCancleHandler(LPEventHanler handler, void *userInfo)
{
    pthread_mutex_lock(&mutex_lock);
    cancelCtx.handler = (handler != NULL)?handler:0;
    cancelCtx.userinfo = (userInfo != NULL)?userInfo:0;
    pthread_mutex_unlock(&mutex_lock);
}

void CLPEvent::resume()
{
    if (nState < kLPEventState_resume) {
        pthread_mutex_lock(&mutex_lock);
        nState = kLPEventState_resume;
        pthread_mutex_unlock(&mutex_lock);
        ProcessEvent(nState);
    }
}

void CLPEvent::suspend()
{
    pthread_mutex_lock(&mutex_lock);
    nState = kLPEventState_suspend;
    pthread_mutex_unlock(&mutex_lock);
}

void CLPEvent::cancel()
{
    if (nState != kLPEventState_cancle) {
        pthread_mutex_lock(&mutex_lock);
        nState = kLPEventState_cancle;
        pthread_mutex_unlock(&mutex_lock);
    }
}

int CLPEvent::getHandel()
{
    return nHandle;
}

void CLPEvent::ProcessEvent(int state)
{
    if (state == kLPEventState_cancle) {
        if (cancelCtx.handler != 0) {
            cancelCtx.handler(this,cancelCtx.userinfo);
        }
    }
    else if (state == kLPEventState_resume)
    {
        if (pPool != NULL) {
            threadpool_add(pPool, CLPEventThread, this, 0);
        }
    }
    else if (state == kLPEventState_work)
    {
        if (nState == kLPEventState_resume) {
            if (nHandle != HANDLE_NULL) {
                int hs = nHandle;
                int ret = 0;
                fd_set    fds;
                struct timeval tv;
                tv.tv_sec = 0;
                tv.tv_usec = 0;
                FD_ZERO(&fds);
                FD_SET(hs, &fds);
                switch (nType) {
                    case LP_EVENT_TYPE_READ:
                    {
                        ret = select(hs+1, &fds,NULL, NULL, &tv );
                        break;
                    }
                    case LP_EVENT_TYPE_WRITE:
                    {
                        ret = select(hs+1,NULL, &fds, NULL, &tv );
                        break;
                    }
                    case LP_EVENT_TYPE_ERROR:
                    {
                        ret = select(hs+1,NULL, NULL, &fds, &tv );
                        break;
                    }
                    default:
                        break;
                }
                if( ret >  0 && FD_ISSET(hs, &fds))
                {
                    if (handlerCtx.handler != 0) {
                        handlerCtx.handler(this,handlerCtx.userinfo);
                    }
                }
            }
        }
        ProcessEvent(nState);
    }
}
