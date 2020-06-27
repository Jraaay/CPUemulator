#include "CPUemulator.h" 

/*获取寄存器内容、寄存器指向的内存内容*/
short getRealData(int num, short *memory, Register *reg, int beginPlace)
{
    if (-1 == num)
    {
        return getImmediate(memory, reg, beginPlace); /*返回立即数*/
    }
    else if (num < 4)
    {
        return reg->ax[num]; /*返回寄存器内容*/
    }
    else
    {
        return connectByte(0, 0, memory[reg->ax[num]], memory[reg->ax[num] + 1]); /*返回寄存器指向的内存的内容*/
    }
}

/*将数据写入寄存器、寄存器指向的内存*/
short writeRealData(short *memory, Register *reg, int num, int data)
{
    if (num < 4)
    {
        reg->ax[num] = data; /*写入寄存器*/
    }
    else
    {
        memory[reg->ax[num]] = apart(data, 0, 8);
        memory[reg->ax[num] + 1] = apart(data, 8, 16); /*写入寄存器指向的内存*/
    }
    return 0;
}
