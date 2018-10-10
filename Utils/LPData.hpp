//
//  LPData.hpp
//  WalktourPackControl
//
//  Created by Ferris on 2018/9/3.
//  Copyright © 2018年 DingLi. All rights reserved.
//

#ifndef LPData_hpp
#define LPData_hpp

#include <stdio.h>

class CLPData {
private:
    void *pBuffer;
    size_t nBufferLength;
    
public:
    CLPData(void *pData,size_t length);
    ~CLPData();
    size_t getLength();
    void *getBytes();
    CLPData *subData(size_t from,size_t to);
};

#endif /* LPData_hpp */
