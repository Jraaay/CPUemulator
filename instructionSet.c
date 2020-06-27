#include "CPUemulator.h"

/*停机指令*/
short shutdownEmu(short *stop)
{
    *stop = 1; /*设置停机状态*/
    return 0;
}

/*输入指令*/
short in(short *memory, Register *reg, int de, HANDLE printMutex)
{
    WaitForSingleObject(printMutex, INFINITE);
    printf("in:\n");                                              /*输出提示符*/
    scanf("%d", &de);                                             /*扫描用户输入*/
    writeRealData(memory, reg, apart(reg->ir[1], 8, 12) - 1, de); /*写入寄存器*/
    ReleaseMutex(printMutex);
    return 0;
}

/*输出指令*/
short out(short *memory, Register *reg, int beginPlace, int threadCount)
{
    printf("id = %d    out: %d\n", threadCount + 1, getRealData(apart(reg->ir[1], 8, 12) - 1, memory, reg, beginPlace)); /*输出指定寄存器、寄存器指向内存*/
    return 0;
}

/*传输指令*/
short convert(short *memory, Register *reg, int beginPlace)
{
    if (0 == apart(reg->ir[1], 12, 16))
    {
        reg->ax[apart(reg->ir[1], 8, 12) - 1] = getImmediate(memory, reg, beginPlace); /*当使用立即数传递时，无条件传入到寄存器而不传入到内存*/
    }
    else
    {
        writeRealData(memory, reg, apart(reg->ir[1], 8, 12) - 1, getRealData(apart(reg->ir[1], 12, 16) - 1, memory, reg, beginPlace)); /*执行传输数据指令*/
    }
    return 0;
}

/*计算模块*/
short realculculate(short a, short b, short config)
{ /*1: +, 2: -, 3: *, 4: /, 5: &&, 6: ||*/
    switch (config)
    {
    case 1:
        return a + b; /*执行+指令*/
    case 2:
        return a - b; /*执行-指令*/
    case 3:
        return a * b; /*执行*指令*/
    case 4:
        return a / b; /*执行/指令*/
    case 5:
        return a && b; /*执行&&指令*/
    case 6:
        return a || b; /*执行||指令*/
    default:
        exit(1);
    }
}
/*计算指令*/
short culculatefun(short *memory, Register *reg, short config, int beginPlace)
{
    writeRealData(memory, reg, apart(reg->ir[1], 8, 12) - 1, realculculate(getRealData(apart(reg->ir[1], 8, 12) - 1, memory, reg, beginPlace), getRealData(apart(reg->ir[1], 12, 16) - 1, memory, reg, beginPlace), config)); /*进行计算、写入寄存器并返回是否成功*/
    return 0;
}

/*否指令*/
short not(short *memory, Register *reg, int beginPlace)
{
    short Reg[2] = {apart(reg->ir[1], 8, 12) - 1, apart(reg->ir[1], 12, 16) - 1}; /*保存寄存器地址*/
    int i;
    for (i = 0; i <= 1; i++)
    {
        if (Reg[i] != -1)
        {
            writeRealData(memory, reg, Reg[i], !getRealData(Reg[i], memory, reg, beginPlace)); /*执行否指令、写入寄存器并返回是否成功*/
        }
    }
    return 0;
}

/*比较指令*/
short compare(short *memory, Register *reg, int beginPlace)
{
    if (getRealData(apart(reg->ir[1], 8, 12) - 1, memory, reg, beginPlace) > getRealData(apart(reg->ir[1], 12, 16) - 1, memory, reg, beginPlace))
    {
        reg->flag = 1;
    }
    else if (getRealData(apart(reg->ir[1], 8, 12) - 1, memory, reg, beginPlace) == getRealData(apart(reg->ir[1], 12, 16) - 1, memory, reg, beginPlace))
    {
        reg->flag = 0;
    }
    else
    {
        reg->flag = -1;
    }
    return 0;
}

/*跳转指令*/
short mygoto(short *memory, Register *reg, int beginPlace, int threadCount, HANDLE printMutex)
{
    short firstReg = apart(reg->ir[1], 8, 12);
    short secondReg = apart(reg->ir[1], 12, 16); /*保存寄存器地址*/

    if (0 == secondReg || (1 == secondReg && 0 == reg->flag) || (2 == secondReg && 1 == reg->flag) || (3 == secondReg && -1 == reg->flag))
    {
        everyStepPrint(reg, getImmediate(memory, reg, beginPlace) - 4, threadCount, printMutex); /*特殊的每步输出*/
        reg->ip += getImmediate(memory, reg, beginPlace) - 4;                                    /*跳转*/
    }
    else
    {
        everyStepPrint(reg, 0, threadCount, printMutex);
    }
    return 0;
}

/*线程休眠*/
short sleepThread(short *memory, Register *reg, int beginPlace)
{
    short sleepTime = getImmediate(memory, reg, beginPlace);
    Sleep(sleepTime / SLEEP_CONST);
    return 0;
}

/*锁定内存*/
short lockMemory(short *memory, Register *reg, HANDLE *hMutex, int beginPlace)
{
    int lockMemoryPlace = getImmediate(memory, reg, beginPlace); /*获取需要锁定的内存位置*/
    if (hMutex[(lockMemoryPlace - 16384) / 2] == NULL)           /*判断是否已经创建互斥对象*/
    {
        hMutex[(lockMemoryPlace - 16384) / 2] = CreateMutex(NULL, TRUE, NULL); /*创建互斥对象并上锁*/
    }
    else
    {
        WaitForSingleObject(hMutex[(lockMemoryPlace - 16384) / 2], INFINITE); /*等待互斥对象被释放并获取控制权*/
    }
    return 0;
}

/*解锁内存*/
short unlockMemory(short *memory, Register *reg, HANDLE *hMutex, int beginPlace)
{
    int unlockMemoryPlace = getImmediate(memory, reg, beginPlace); /*获取需要解锁的内存位置*/
    ReleaseMutex(hMutex[(unlockMemoryPlace - 16384) / 2]);         /*释放互斥对象*/
    return 0; 
}
