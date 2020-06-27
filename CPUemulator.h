/*
 * Copyright (C) Jray
 * Created:2020.03.25
 * Last improved:2020.06.01 
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <windows.h>
#include <process.h>
/*include区*/

#define FILE_NAME "dict.dic"   /*定义指令所在的文件名*/
#define IR_COUNT 2             /*定义ir寄存器数量*/
#define AX_COUNT 8             /*定义ax寄存器大小*/
#define SLEEP_CONST 1          /*定义休眠函数*/
#define THREAD_COUNT 2         /*定义线程数量*/
#define THREAD_MEMORY_SIZE 256 /*定义每个线程的代码区内存大小*/

#define COMMAND_SHUTDOWN 0 /*00000000*/
#define COMMAND_CONVERT 1  /*00000001*/
#define COMMAND_PLUS 2     /*00000010*/
#define COMMAND_SUBTRACT 3 /*00000011*/
#define COMMAND_MULTIPLY 4 /*00000100*/
#define COMMAND_DIVIDE 5   /*00000101*/
#define COMMAND_AND 6      /*00000110*/
#define COMMAND_OR 7       /*00000111*/
#define COMMAND_NOT 8      /*00001000*/
#define COMMAND_COMPARE 9  /*00001001*/
#define COMMAND_GOTO 10    /*00001010*/
#define COMMAND_IN 11      /*00001011*/
#define COMMAND_OUT 12     /*00001100*/
#define COMMAND_LOCK 13    /*00001101*/
#define COMMAND_UNLOCK 14  /*00001110*/
#define COMMAND_SLEEP 15   /*00001111*/
/* 宏定义区 */

/*定义寄存器*/
typedef struct _CPU_Register
{
    short ip, ir[IR_COUNT], flag, ax[AX_COUNT];
} Register; 

typedef struct _thread_Sarameter
{
    short *memory;
    int memoryPlace;
    int threadNumber;
    HANDLE *hMutex;
    HANDLE printMutex;
} threadSarameter;
/* 结构、类、变量等定义区 */

Register *initRegister(void);                                                                      /*初始化寄存器*/
HANDLE *inithMutex(void);                                                                          /*初始化互斥对象数组*/
short *initMemory(void);                                                                           /*初始化内存*/
unsigned __stdcall mainThread(void *pArguments);                                                   /*单个线程的主函数*/
threadSarameter *initSarameter(HANDLE *hMutex, int threadCount, short *memory, HANDLE printMutex); /*初始化线程参数*/
short readInstruction(FILE *fp, short *memory, int beginPlace);                                    /*读取指令*/
short takeInstruction(short *memory, Register *reg, int beginPlace);                               /*取指令*/
short runInstruction(threadSarameter *sarameter, Register *reg, short *stop);                      /*运行指令*/
short shutdownEmu(short *stop);                                                                    /*停机指令*/
short convert(short *memory, Register *reg, int beginPlace);                                       /*传输指令*/
short realculculate(short a, short b, short config);                                               /*计算模块*/
short culculatefun(short *memory, Register *reg, short config, int beginPlace);                    /*计算指令*/
short not(short *memory, Register *reg, int beginPlace);                                           /*否指令*/
short compare(short *memory, Register *reg, int beginPlace);                                       /*比较指令*/
short in(short *memory, Register *reg, int de, HANDLE printMutex);                                 /*输入指令*/
short out(short *memory, Register *reg, int beginPlace, int threadCount);                          /*输出指令*/
short apart(short ch, short a, short b);                                                           /*切割工具函数*/
short getImmediate(short *memory, Register *reg, int beginPlace);                                  /*获取立即数*/
short mygoto(short *memory, Register *reg, int beginPlace, int threadCount, HANDLE printMutex);    /*跳转指令*/
short lockMemory(short *memory, Register *reg, HANDLE *hMutex, int beginPlace);                    /*锁内存*/
short unlockMemory(short *memory, Register *reg, HANDLE *hMutex, int beginPlace);                  /*解锁内存*/
short everyStepPrint(Register *reg, short place, int threadCount, HANDLE printMutex);              /*每步输出*/
int connectByte(short a, short b, short c, short d);                                               /*连接工具函数*/
short finalPrint(short *memory);                                                                   /*最后输出*/
short getRealData(int num, short *memory, Register *reg, int beginPlace);                          /*获取寄存器内容、寄存器指向的内存内容*/
short writeRealData(short *memory, Register *reg, int num, int data);                              /*将数据写入寄存器、寄存器指向的内存*/
short sleepThread(short *memory, Register *reg, int beginPlace);                                   /*线程休眠*/ 
/* 函数定义区 */
