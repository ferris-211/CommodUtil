//
//  LPError.cpp
//  WalktourPackControl
//
//  Created by Ferris on 2018/9/4.
//  Copyright © 2018年 DingLi. All rights reserved.
//

#include "LPError.hpp"
#include <string>

CLPError:: CLPError(int code , char *domain, size_t length)
{
    nCode = code;
    pBuffer = NULL;
    if (domain) {
        size_t len = length + 1;
        pBuffer = (char *)malloc(len);
        memset(pBuffer, 0, len);
        memcpy(pBuffer, domain, length);
    }
}

CLPError:: ~CLPError()
{
    if (pBuffer != NULL) {
        free(pBuffer);
    }
}

int CLPError::getCode()
{
    return nCode;
}

const char * CLPError::getDomain()
{
    return pBuffer;
}
