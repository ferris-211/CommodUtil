//
//  LPCommon.h
//  WalktourPackControl
//
//  Created by Ferris on 2018/9/10.
//  Copyright © 2018年 DingLi. All rights reserved.
//

#ifndef LPCommon_h
#define LPCommon_h

#include <unistd.h>
#include <stdlib.h>

#ifndef __stdcall
#define __stdcall
#endif

#define msleep(x) usleep((x)*1000)
#define fsleep(x) usleep((int)(x)*1000 * 1000)
#define LPRelease(x) {if(x != NULL){delete x;x = NULL;}}

#endif /* LPCommon_h */
