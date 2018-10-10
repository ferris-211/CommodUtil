//
//  LPDate.hpp
//  WalktourPackControl
//
//  Created by Ferris on 2018/10/9.
//  Copyright © 2018年 DingLi. All rights reserved.
//

#ifndef LPDate_hpp
#define LPDate_hpp

#include <stdio.h>

#pragma pack(1)

typedef struct _LPTimeval
{
    int     tv_sec;
    int     tv_usec;
}LPTimeval,*LPTimevalRef;

#pragma pack()

class CLPDate {
private:
    LPTimeval tvSince1970;
public:
    CLPDate(LPTimevalRef time);
    ~CLPDate();
    static CLPDate *dateNow();
    static CLPDate *date(int offsetSEC);
    void getTimeSince1970(LPTimevalRef time);
    void toFormat(void *buffer,size_t len, const char *format);
};

#endif /* LPDate_hpp */
