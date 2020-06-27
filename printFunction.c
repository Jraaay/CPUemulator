#include "CPUemulator.h" 

/*每步输出*/
short everyStepPrint(Register *reg, short place, int threadCount, HANDLE printMutex)
{
    WaitForSingleObject(printMutex, INFINITE);
    printf("id = %d\nip = %d\nflag = %d\nir = %d\nax1 = %d ax2 = %d ax3 = %d ax4 = %d\nax5 = %d ax6 = %d ax7 = %d ax8 = %d\n", threadCount + 1, (reg->ip + place + threadCount * THREAD_MEMORY_SIZE), reg->flag, connectByte(0, 0, reg->ir[0], reg->ir[1]), reg->ax[0], reg->ax[1], reg->ax[2], reg->ax[3], reg->ax[4], reg->ax[5], reg->ax[6], reg->ax[7]);
    ReleaseMutex(printMutex);
    return 0;
}

/*最终输出*/
short finalPrint(short *memory)
{
    printf("\ncodeSegment :\n");
    int i, j;
    for (i = 0; i < 16; i++)
    {
        for (j = 0; j < 8; j++)
        {
            printf("%d%c", connectByte(memory[(i * 8 + j) * 4], memory[(i * 8 + j) * 4 + 1], memory[(i * 8 + j) * 4 + 2], memory[(i * 8 + j) * 4 + 3]), 7 == j ? '\n' : ' ');
        }
    } /*输出代码段内存*/
    printf("\ndataSegment :\n");
    for (i = 0; i < 16; i++)
    {
        for (j = 0; j < 16; j++)
        {
            printf("%d%c", (short)connectByte(0, 0, memory[(i * 16 + j) * 2 + 16384], memory[(i * 16 + j) * 2 + 16384 + 1]), 15 == j ? '\n' : ' ');
        }
    } /*输出数据段内存*/
    return 0;
}
