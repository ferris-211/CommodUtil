//
//  LPTimer.hpp
//  WalktourPackControl
//
//  Created by Ferris on 2018/9/11.
//  Copyright © 2018年 DingLi. All rights reserved.
//

#ifndef LPTimer_hpp
#define LPTimer_hpp

#include <stdio.h>
#include "LPDate.hpp"

typedef void (*PCLPTimerFire)(void *timer);

//用毫秒作为间隔单位
class CLPTimer {
private:
    int state;
    int cut;                     //循环执行次数, -1表示一直执行
    LPTimeval tv_fire;           //运行的具体时间到秒
    int nInterval;               //还需要等待的毫秒数
    void *pUserInfo;
    PCLPTimerFire fFire;
    CLPTimer(CLPDate *fireDate, int interval,PCLPTimerFire fire,bool repeat = false,void *userinfo = NULL);
    ~CLPTimer();
public:
    
    /**
     创建定时器

     @param interval 时间间隔
     @param fire 触发回调
     @param repeat 是否重复
     @param userinfo 用户
     @return timer对象
     */
    static CLPTimer *scheduleTimer(int interval,PCLPTimerFire fire,bool repeat = false,void *userinfo = NULL);
    
    
    /**
     创建定时器

     @param fireDate 首次触发时间
     @param interval 时间间隔
     @param fire 触发回调
     @param repeat 是否重复
     @param userinfo 用户
     @return timer对象
     */
    static CLPTimer *scheduleTimerFire(CLPDate *fireDate, int interval,PCLPTimerFire fire,bool repeat = false,void *userinfo = NULL);
    
    
    /**
     销毁定时器
     @param timer 将要被销毁对象
     */
    static void destoryTimer(CLPTimer *timer);
    
    
    /**
     取消定时器
     */
    void invalidate();
    
    
    /**
     定时器是否进行中

     @return bool
     */
    bool validate();
    
    
    /**
     触发时间，（new 对象，用户需要自己管理CLPDate的生命周期）

     @return CLPDate
     */
    CLPDate *fireDate();
    
    
    /**
     用户附加信息

     @return void *
     */
    void *userInfo();
    
    //处理逻辑
    int timerRun(void *arg);
    void timerDealloc(int stat);
};

#endif /* LPTimer_hpp */
