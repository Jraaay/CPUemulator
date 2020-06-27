/*
 * Copyright (C) Jray
 * Created:2020.03.25
 * Last improved:2020.06.01 
 */

#include "CPUemulator.h"

/*初始化线程参数*/
threadSarameter *initSarameter(HANDLE *hMutex, int threadCount, short *memory, HANDLE printMutex)
{
    threadSarameter *sarameter = (threadSarameter *)malloc(sizeof(threadSarameter)); /*为线程参数分配内存*/
    sarameter->memory = memory;                                                      /*初始化内存参数*/
    sarameter->memoryPlace = threadCount * THREAD_MEMORY_SIZE;
    sarameter->threadNumber = threadCount;
    sarameter->hMutex = hMutex;
    sarameter->printMutex = printMutex;
    return sarameter;
}

/*初始化互斥对象数组*/
HANDLE *inithMutex(void)
{
    HANDLE *hMutex = (HANDLE *)malloc(sizeof(HANDLE) * (16384 / 2)); /*为互斥对象数组分配内存*/
    int i;
    for (i = 0; i < 16384 / 2; i++)
    {
        hMutex[i] = NULL;
    } /*初始化互斥对象数组*/
    return hMutex;
}

/*初始化寄存器*/
Register *initRegister(void)
{
    Register *reg = (Register *)malloc(sizeof(Register)); /*分配内存*/

    reg->ip = 0;
    reg->ir[0] = 0;
    reg->ir[1] = 0;
    reg->flag = 0;

    int i;
    for (i = 0; i < 8; i++)
    {
        reg->ax[i] = 0;
    } /*寄存器置零*/

    return reg; /*返回寄存器地址*/
}

/*初始化内存*/
short *initMemory(void)
{
    short *memory = (short *)malloc(sizeof(short) * 32 * 1024); /*分配内存*/

    int i;
    for (i = 0; i < 32 * 1024; i++)
    {
        memory[i] = 0;
    } /*内存置零*/

    memory[16384] = 0;
    memory[16385] = 100;

    return memory; /*返回内存地址*/
}
