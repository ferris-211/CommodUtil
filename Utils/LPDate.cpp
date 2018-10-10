//
//  LPDate.cpp
//  WalktourPackControl
//
//  Created by Ferris on 2018/10/9.
//  Copyright © 2018年 DingLi. All rights reserved.
//

#include "LPDate.hpp"
#include <time.h>
#include <sys/time.h>
#include <string.h>

CLPDate:: CLPDate(LPTimevalRef time)
{
    if (time == NULL) {
        struct timeval us;
        gettimeofday(&us,NULL);
        tvSince1970.tv_sec = (int)us.tv_sec;
        tvSince1970.tv_usec = us.tv_usec;
    }
    else
    {
        memcpy(&tvSince1970, time, sizeof(LPTimeval));
    }
}

CLPDate:: ~CLPDate(){
    
}

CLPDate * CLPDate:: dateNow()
{
    struct timeval us;
    gettimeofday(&us,NULL);
    LPTimeval tv;
    tv.tv_sec = (int)us.tv_sec;
    tv.tv_usec = us.tv_usec;
    return new CLPDate(&tv);
}

CLPDate * CLPDate:: date(int offsetSEC)
{
    struct timeval us;
    gettimeofday(&us,NULL);
    LPTimeval tv;
    tv.tv_sec = (int)us.tv_sec + offsetSEC;
    tv.tv_usec = us.tv_usec;
    return new CLPDate(&tv);
}

void CLPDate:: getTimeSince1970(LPTimevalRef time)
{
    memcpy(time, &tvSince1970, sizeof(LPTimeval));
}

void CLPDate:: toFormat(void *buffer,size_t len, const char *format)
{
    struct tm t;
    time_t sec = tvSince1970.tv_sec;
    strftime((char *)buffer, len, format, localtime_r(&sec, &t));
}
