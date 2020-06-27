/*
 * Copyright (C) Jray
 * Created:2020.03.25
 * Last improved:2020.06.01 
 */

#include "CPUemulator.h"

/*读取指令*/
short readInstruction(FILE *fp, short *memory, int beginPlace)
{
    short ch = 0, tmp, i = 0, bb = 0;

    while ((tmp = fgetc(fp)) != EOF)
    {
        if (tmp != ' ' && tmp != '\n')
        {
            ch = ch * 2 + (tmp - '0');
            bb++;
        } /*过滤空格回车，并将二进制指令转为十进制*/

        if (8 == bb)
        {
            bb = 0;
            memory[beginPlace + i++] = ch;
            ch = 0;
        } /*保存十进制指令到内存的代码段*/
    }

    return 0;
}
/*取指令*/
short takeInstruction(short *memory, Register *reg, int beginPlace)
{
    reg->ir[0] = memory[reg->ip + beginPlace];
    reg->ir[1] = memory[reg->ip + 1 + beginPlace]; /*从内存读取指令*/

    reg->ip += 4; /*指令计数器+4*/
    return 0;
}
/*运行指令*/
short runInstruction(threadSarameter *sarameter, Register *reg, short *stop)
{
    while (!(*stop))
    {
        takeInstruction(sarameter->memory, reg, sarameter->memoryPlace); /*取指令*/

        short flag = reg->ir[0];

        switch (reg->ir[0])
        { /*分析指令*/
        case COMMAND_SHUTDOWN:
            shutdownEmu(stop);
            break;
        case COMMAND_CONVERT:
            convert(sarameter->memory, reg, sarameter->memoryPlace);
            break;
        case COMMAND_PLUS:
        case COMMAND_SUBTRACT:
        case COMMAND_MULTIPLY:
        case COMMAND_DIVIDE:
        case COMMAND_AND:
        case COMMAND_OR:
            culculatefun(sarameter->memory, reg, reg->ir[0] - 1, sarameter->memoryPlace);
            break;
        case COMMAND_NOT:
            not(sarameter->memory, reg, sarameter->memoryPlace);
            break;
        case COMMAND_COMPARE:
            compare(sarameter->memory, reg, sarameter->memoryPlace);
            break;
        case COMMAND_GOTO:
            mygoto(sarameter->memory, reg, sarameter->memoryPlace, sarameter->threadNumber, sarameter->printMutex);
            break;
        case COMMAND_IN:
            in(sarameter->memory, reg, 0, sarameter->printMutex);
            break;
        case COMMAND_OUT:
            out(sarameter->memory, reg, sarameter->memoryPlace, sarameter->threadNumber);
            break;
        case COMMAND_LOCK:
            lockMemory(sarameter->memory, reg, sarameter->hMutex, sarameter->memoryPlace);
            break;
        case COMMAND_UNLOCK:
            unlockMemory(sarameter->memory, reg, sarameter->hMutex, sarameter->memoryPlace);
            break;
        case COMMAND_SLEEP:
            sleepThread(sarameter->memory, reg, sarameter->memoryPlace);
            break;
        default:
            exit(1);
        }

        if (flag != 10)
        {
            everyStepPrint(reg, 0, sarameter->threadNumber, sarameter->printMutex); /*每步输出*/
        }
    }
    return 0;
}
