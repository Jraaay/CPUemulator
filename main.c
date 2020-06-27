#include "CPUemulator.h"

short main(void)
{
    HANDLE *hMutex = inithMutex();
    short *memory = initMemory(); /*初始化内存*/
    HANDLE hThread[THREAD_COUNT]; /*创建线程句柄数组*/
    HANDLE printMutex = CreateMutex(NULL, FALSE, NULL);
    threadSarameter *sarameter[THREAD_COUNT]; /*创建线程参数结构体数组*/
    int i;
    for (i = 0; i < THREAD_COUNT; i++)
    {
        sarameter[i] = initSarameter(hMutex, i, memory, printMutex);                     /*初始化线程参数结构体*/
        hThread[i] = (HANDLE)_beginthreadex(NULL, 0, mainThread, sarameter[i], 0, NULL); /*创建线程*/
    }
    for (i = 0; i < THREAD_COUNT; i++)
    {
        WaitForSingleObject(hThread[i], INFINITE); /*等待线程结束*/
        CloseHandle(hThread[i]);                   /*关闭线程*/
        free(sarameter[i]);                        /*释放线程参数结构体内存*/
    }
    finalPrint(memory); /*最终输出*/
    free(memory);       /*释放内存*/
    free(hMutex);
    return 0;
    //system("pause");
}

/*单个线程的主函数*/
unsigned __stdcall mainThread(void *pArguments)
{
    threadSarameter *sarameter = (threadSarameter *)pArguments;
    FILE *fp = fopen(FILE_NAME, "r");                               /*获取文件流*/
    readInstruction(fp, sarameter->memory, sarameter->memoryPlace); /*读取指令*/
    fclose(fp);                                                     /*关闭文件流*/
    Register *reg = initRegister();                                 /*初始化寄存器*/
    short stop = 0;                                                 /*设置停机状态*/
    runInstruction(sarameter, reg, &stop);                          /*运行指令*/
    free(reg);                                                      /*释放寄存器内存*/
    return 0;
}
