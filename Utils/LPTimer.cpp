//
//  LPTimer.cpp
//  WalktourPackControl
//
//  Created by Ferris on 2018/9/11.
//  Copyright © 2018年 DingLi. All rights reserved.
//

#include "LPTimer.hpp"
#include "LPCommon.h"
#include <pthread.h>
#include <vector>
#include <sys/time.h>
#include <climits>

typedef std::vector<CLPTimer *> CLPTimerList;

// 当前链表对象管理器
struct stlist {
    pthread_mutex_t lock;         //加锁用的
    int status;                   //0表示停止态, 1表示主线程loop运行态
    pthread_t tid;                //主循环线程id, 0表示没有启动
    CLPTimerList list;            //队列
};

enum CLPTimerState
{
    kUnkonw             = 0,
    kInit,     // If set, timer has been init
    kQueuing,  // If set , timer is queuing
    kDealloc,  // If set, the timer is being deallocated
};

// 定时器对象的单例, 最简就是最复杂
static struct stlist _st = {PTHREAD_MUTEX_INITIALIZER,0,0,{}};

// 得到等待的时间,毫秒, <=0的时候头时间就可以执行了
static inline int _sleeptime(struct stlist* st)
{
    int interval = INT_MAX;
    int tv;
    CLPTimer *tm;
    CLPTimerList::iterator it = st->list.begin();
    int state = 0;
    while (it != st->list.end())
    {
        tm = *it;
        tv = tm->timerRun(&state);
        if (tv == 0 || !tm->validate()) {
            it = st->list.erase(it);
            tm->timerDealloc(state);
        }
        else
        {
            if (interval > tv) {
                interval = tv;
            }
            it++;
        }
    }
    //如果都为无效值
    if (interval == INT_MAX) {
        interval = 0;
    }
    return interval;
}

// 运行的主loop,基于timer管理器
static void* _stlist_loop(struct stlist* st)
{
    int nowt;
    //设置线程属性, 默认线程属性 允许退出线程
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL); //设置立即取消
    pthread_detach(pthread_self()); //设置线程分离,自销毁
    
    // 正常轮询,检测时间
    while (!st->list.empty()) {
        pthread_testcancel(); //添加测试取消点
        pthread_mutex_lock(&st->lock);
        nowt = _sleeptime(st);
        pthread_mutex_unlock(&st->lock);
        if (nowt > 0) {
            msleep(nowt);
        }
    }
    // 已经运行结束
    st->status = 0;
    return NULL;
}

// 添加链表对象, 返回true表示插入的是头结点, 当你执行的时候需要全额加锁
static void _stlist_add(struct stlist* st,CLPTimer *timer)
{
    pthread_mutex_lock(&st->lock);
    st->list.push_back(timer);
    // 看是否需要取消线程
    if(st->status == 1 && _sleeptime(st) <= 0){
        pthread_cancel(st->tid);
        st->status = 0;
    }
    // 这个时候重新开启线程
    if(st->status == 0){
        pthread_create(&st->tid, NULL, (void* (*)(void*))_stlist_loop, st);
        st->status = 1; //延迟真实运行态
    }
    pthread_mutex_unlock(&st->lock);
}

CLPTimer::CLPTimer(CLPDate *fireData, int interval,PCLPTimerFire fire,bool repeat,void *userinfo)
{
    if (fireData == NULL) {
        fireData = CLPDate:: dateNow();
        fireData->getTimeSince1970(&tv_fire);
        int sec,usec;
        sec = interval /1000;
        usec = (interval - sec * 1000) * 1000;
        tv_fire.tv_sec += sec;
        tv_fire.tv_usec += usec;
    }
    else
    {
        fireData->getTimeSince1970(&tv_fire);
    }
    this->nInterval = interval;
    this->fFire = fire;
    this->cut = repeat? -1 : 1;
    this->pUserInfo = userinfo;
    this->state = kInit;
}

CLPTimer::~CLPTimer()
{
    this->state = kUnkonw;
}

CLPTimer * CLPTimer::scheduleTimer( int interval, PCLPTimerFire fire,bool repeat,void *userinfo)
{
    CLPTimer *timer = new CLPTimer(NULL,interval,fire,repeat,userinfo);
     _stlist_add(&_st,timer);
    timer->state = kQueuing;
    return timer;
}

CLPTimer * CLPTimer::scheduleTimerFire(CLPDate *fireDate, int interval,PCLPTimerFire fire,bool repeat,void *userinfo)
{
    CLPTimer *timer = new CLPTimer(fireDate,interval,fire,repeat,userinfo);
    _stlist_add(&_st,timer);
    timer->state = kQueuing;
    return timer;
}

void CLPTimer::destoryTimer(CLPTimer *timer)
{
    if (timer) {
        if (timer->validate()) {
            timer->state = kDealloc;
            timer->invalidate();
        }
        else
        {
            if (timer->state == kQueuing) {
                timer->state = kDealloc;
            }
            else
            {
                timer->invalidate();
                delete timer;
            }
        }
    }
}

void CLPTimer::invalidate()
{
    if (this->cut != 0) {
        this->cut = 0;
    }
}

bool CLPTimer::validate()
{
    return (this->cut != 0);
}

CLPDate * CLPTimer::fireDate()
{
    return (new CLPDate(&this->tv_fire));
}

void * CLPTimer::userInfo()
{
    return pUserInfo;
}

int CLPTimer::timerRun(void *arg)
{
    int *status = (int *)arg;
    //如果为无效直接返回
    if (this->cut == 0) {
        *status = this->state;
        return 0;
    }
    //开始执行
    struct timeval tv_sys;
    gettimeofday(&tv_sys, NULL);
    int offset = (1000*(tv_fire.tv_sec - tv_sys.tv_sec) + (tv_fire.tv_usec - tv_sys.tv_usec)/1000);
    if (offset <= 0) {
        if (fFire != NULL) {
            fFire(this);
        }
        *status = this->state;
        if (this->cut == 1) {
            this->cut = 0;
            return 0;
        }
        //这里需要重新组织数据
        if (this->cut > 0) {
            this->cut -= 1;
        }
        //更新当前时间
        int sec = nInterval/1000;
        int usec = (nInterval - sec *1000) * 1000;
        tv_fire.tv_sec = tv_sys.tv_sec + sec;
        tv_fire.tv_usec = tv_sys.tv_usec + usec;
        return nInterval;
    }
    *status = this->state;
    return offset;
}

void CLPTimer::timerDealloc(int stat)
{
    if (stat == kDealloc) {
        this->state = kUnkonw;
        delete this;
    }
    else
    {
        this->state = kInit;
    }
}
