//
//  LPDataBuffer.hpp
//  WalktourPackControl
//
//  Created by Ferris on 2018/9/4.
//  Copyright © 2018年 DingLi. All rights reserved.
//

#ifndef LPDataBuffer_hpp
#define LPDataBuffer_hpp

#include <stdio.h>

//数据块缓存

class CLPDataBuffer {
private:
    char *pBuffer;
    size_t nBufferLength;
    int nCapacity;
    int nItemCount;
    int nItemSize;
public:
    /**
     数据块缓存构造函数
     @param itemSize 块大小
     @param capacity 块预开辟空间
     */
    CLPDataBuffer(int itemSize = 1,int capacity = 10);
    //释构函数
    ~CLPDataBuffer();
    
    /**
     数据块大小

     @return int
     */
    int getItemSize();
    
    
    /**
     数据块个数

     @return int
     */
    int getItemCount();
    
    
    /**
     重置块个数（已开辟内存不变化）

     @return 队列开始指针
     */
    char *reset();
    
    /**
     重置块个数（当内存不足时，申请新的内存空间）
     @param count 数据块个数
     @return 队列开始指针，返回空时代表新申请的内存无效
     */
    char *resetCount(int count);
    
    
    /**
     获取数据块队列指针

     @return 指针
     */
    char *getBytes();
    
    
    /**
     获取数据块

     @param index 数据块下标
     @return 指针
     */
    char *getItem(int index);
    
    
    /**
     添加新的数据块

     @param item 数据块指针
     */
    void addItem(char *item);
    
    
    /**
     添加多个数据块

     @param item 指针
     @param count 个数
     */
    void addItems(char *item,int count);
    
    
    /**
     删除某个数据块

     @param index 数据块下标
     */
    void removeItem(int index);
    
    
    /**
     删除批量数据块

     @param fromIndex 开始指针
     @param count 数量
     */
    void removeItems(int fromIndex,int count);
    
    
    /**
     重置内存，根据实际的数量回收内存

     @return 指针,返回空代表不成功
     */
    char * revert();
};


#endif /* LPDataBuffer_hpp */
