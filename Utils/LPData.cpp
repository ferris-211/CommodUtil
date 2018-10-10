//
//  LPData.cpp
//  WalktourPackControl
//
//  Created by Ferris on 2018/9/3.
//  Copyright © 2018年 DingLi. All rights reserved.
//

#include "LPData.hpp"
#include <stdlib.h>
#include <string>

#define DataBufferLength 1024

CLPData::CLPData(void *pData,size_t length)
{
    nBufferLength = length;
    if (nBufferLength > 0) {
        pBuffer = malloc(nBufferLength);
        memcpy(pBuffer, pData, nBufferLength);
    }
}

CLPData::~CLPData()
{
    free(pBuffer);
}

size_t CLPData::getLength()
{
    return nBufferLength;
}

void * CLPData::getBytes()
{
    return pBuffer;
}

CLPData * CLPData::subData(size_t from, size_t to)
{
    if (from < nBufferLength && to < nBufferLength) {
        if (from < to) {
            size_t length = to - from;
            char *src = (char *)pBuffer + from;
            CLPData *rtn = new CLPData(src,length);
            return rtn;
        }
    }
    return NULL;
}

