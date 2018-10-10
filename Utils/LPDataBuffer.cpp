//
//  LPDataBuffer.cpp
//  WalktourPackControl
//
//  Created by Ferris on 2018/9/4.
//  Copyright © 2018年 DingLi. All rights reserved.
//

#include "LPDataBuffer.hpp"
#include <stdlib.h>
#include <string>

CLPDataBuffer::CLPDataBuffer(int itemSize,int capacity)
{
    nItemSize = itemSize;
    nCapacity = capacity;
    if (nCapacity == 0) {
        nCapacity = 1;
    }
    reset();
}

CLPDataBuffer::~CLPDataBuffer()
{
    reset();
    free(pBuffer);
}

int CLPDataBuffer::getItemSize()
{
    return nItemSize;
}

int CLPDataBuffer::getItemCount()
{
    return nItemCount;
}

char * CLPDataBuffer::reset()
{
    return resetCount(0);
}

char * CLPDataBuffer::resetCount(int count)
{
    size_t dataLen = nItemSize * count;
    if (dataLen == 0) {
        dataLen = (nCapacity * nItemSize);
    }
    size_t newLen = nBufferLength;
    while (newLen < dataLen) {
        newLen += (nCapacity * nItemSize);
    }
    if (nBufferLength == 0) {
        pBuffer = (char *)malloc(newLen);
        nItemCount = count;
        nBufferLength = newLen;
    }
    else if (newLen > nBufferLength) {
        char *tmp = (char *)realloc(pBuffer, newLen);
        if (tmp != NULL) {
            pBuffer = tmp;
            nItemCount = count;
            nBufferLength = newLen;
        }
        else
        {
            //无法生成新的内存;
            return NULL;
        }
    }
    else
    {
        nItemCount = count;
    }
    return pBuffer;
}

char * CLPDataBuffer::getBytes()
{
    return pBuffer;
}

char * CLPDataBuffer::getItem(int index)
{
    if (index < nItemCount) {
        size_t n = nItemSize * index;
        char *data = pBuffer;
        data += n;
        return data;
    }
    return NULL;
}

void CLPDataBuffer:: addItem(char *item)
{
    int count = this->getItemCount();
    if(this->resetCount(count + 1) != NULL)
    {
        int offset = count * nItemSize;
        memcpy(pBuffer + offset, item, nItemSize);
    }
}

void CLPDataBuffer::addItems(char *item, int count)
{
    int oldCount = this->getItemCount();
    if(this->resetCount(count + oldCount) != NULL)
    {
        int offset = count * nItemSize;
        memcpy(pBuffer + offset, item, nItemSize * count);
    }
}

void CLPDataBuffer::removeItem(int index)
{
    if (index >= 0 && index < nItemCount) {
        
        if ((index + 1) == nItemCount) {//最后一个
            //什么都不用做
        }
        else//前面的一个，需要把后面的移位
        {
            int srcPos = index * nItemSize;
            int descPos = (index + 1)  * nItemSize;
            int len = nItemCount * nItemSize - descPos;
            char *tmp = (char *)malloc(len);
            memcpy(tmp, pBuffer + descPos, len);
            memcpy(pBuffer + srcPos, tmp, len);
            free(tmp);
        }
        nItemCount -= 1;
    }
}

void CLPDataBuffer::removeItems(int fromIndex, int count)
{
    if (fromIndex >= 0 && fromIndex < nItemCount) {
        if ((count + fromIndex) >= nItemCount) {
            nItemCount = fromIndex;
        }
        else
        {
            int srcPos = fromIndex * nItemSize;
            int descPos = (fromIndex + count)  * nItemSize;
            int len = nItemCount * nItemSize - descPos;
            char *tmp = (char *)malloc(len);
            memcpy(tmp, pBuffer + descPos, len);
            memcpy(pBuffer + srcPos, tmp, len);
            free(tmp);
            nItemCount -= count;
        }
    }
}

char * CLPDataBuffer::revert()
{
    if (pBuffer) {
        size_t realLen = nItemCount * nItemSize;
        size_t i = nCapacity * nItemSize;
        while (i < realLen) {
            i+= (nCapacity * nItemSize);
        }
        if (i != nBufferLength) {
            char *tmp = (char *)realloc(pBuffer, i);
            if (tmp == NULL) {
                return tmp;
            }
            pBuffer = tmp;
            nBufferLength = i;
        }
    }
    return pBuffer;
}
