/*
 * Copyright (C) Jray
 * Created:2020.03.25
 * Last improved:2020.06.01 
 */

#include "CPUemulator.h"

/*连接工具函数*/
int connectByte(short a, short b, short c, short d)
{
    int byte[4] = {a, b, c, d};
    int value = 0;
    int i;

    for (i = 0; i < 4; i++)
    {
        value <<= 8; /*移位*/
        value |= (byte[i] & 0xff);
    }
    return value; /*返回值*/
}

/*切割工具函数*/
short apart(short n, short a, short b)
{
    int byte[4], i, offset;

    for (i = 0; i < 16 / (b - a); i++)
    {
        offset = 16 - (i + 1) * (b - a);                                                           /*计算偏移量*/
        byte[i] = 4 == b - a ? (char)(((n >> offset) & 0xff) % 16) : (char)((n >> offset) & 0xff); /*分别存储*/
    }
    return byte[a / (b - a)]; /*返回值*/
}

/*获取立即数*/
short getImmediate(short *memory, Register *reg, int beginPlace)
{
    return connectByte(0, 0, memory[reg->ip - 2 + beginPlace], memory[reg->ip - 1 + beginPlace]); /*返回立即数*/
}
