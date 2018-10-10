//
//  LPError.hpp
//  WalktourPackControl
//
//  Created by Ferris on 2018/9/4.
//  Copyright © 2018年 DingLi. All rights reserved.
//

#ifndef LPError_hpp
#define LPError_hpp

#include <stdio.h>

class CLPError {
    
public:
    CLPError(int code, char *domain, size_t len);
    ~CLPError();
    const char *getDomain();
    int getCode();
    
private:
    char *pBuffer;
    int nCode;
};

#endif /* LPError_hpp */
