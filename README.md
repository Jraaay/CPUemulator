# CPUemulator

*这是BUPT(北京邮电大学)SCS(计算机学院)2019-2020学年第二学期大一年级计算机导论课程设计大作业*

其中dict.dic为指令代码存放的文件

本项目的dict.dic为使用两个线程进行100张票的出售。

编译的命令为：`gcc -o CPUemulator.exe CPUemulator.h dataIO.c initialize.c instructionProcess.c instructionSet.c main.c printFunction.c toolFunction.c`

## 指令集
| 指令名称   | 指令                                 | 说明                                                                                           |
|--------|------------------------------------|----------------------------------------------------------------------------------------------|
| 停机指令   | 00000000 00000000 0000000000000000 | 停止程序执行。                                                                                      |
| 数据传送指令 | 00000001 00010000 0000000000000000 | 将一个立即数（后16位）传送至寄存器1                                                                          |
|        | 00000001 00010101 0000000000000000 | 将寄存器5（5、6、7、8号寄存器为地址寄存器）中地址所指向的内存单元（2个字节）的内容传送至寄存器1                                          |
|        | 00000001 01010001 0000000000000000 | 将寄存器1的内容传送至寄存器5中地址所指向的内存单元（2个字节）5、6、7、8号寄存器为地址寄存器）。                                          |
| 算数运算指令 | 00000010 00010000 0000000000000000 | 将寄存器1内的数与一个立即数（后16位）相加，结果保存至寄存器1                                                             |
|        | 00000010 00010101 0000000000000000 | 将寄存器1内的数与寄存器5中地址所指向的内存单元（2个字节）里存的数相加，结果保存至寄存器1                                               |
|        | 00000011 00010000 0000000000000000 | 将寄存器1内的数减去一个立即数（后16位），结果保存至寄存器1                                                              |
|        | 00000011 00010101 0000000000000000 | 将寄存器1内的数减去寄存器5中地址所指向的内存单元（2个字节）里存的数，结果保存至寄存器1                                                |
|        | 00000100 00010000 0000000000000000 | 将寄存器1内的数与一个立即数（后16位）相乘，结果保存至寄存器1                                                             |
|        | 00000100 00010101 0000000000000000 | 将寄存器1内的数与寄存器5中地址所指向的内存单元（2个字节）里存的数相乘，结果保存至寄存器1                                               |
|        | 00000101 00010000 0000000000000000 | 将寄存器1内的数除以（C语言的整数除法）一个立即数（后16位），结果保存至寄存器1                                                    |
|        | 00000101 00010101 0000000000000000 | 将寄存器1内的数除以（C语言的整数除法）寄存器5中地址所指向的内存单元（2个字节）里存的数，结果保存至寄存器1                                      |
| 逻辑运算指令 | 00000110 00010000 0000000000000000 | 将寄存器1内的数与一个立即数（后16位）做逻辑与，结果保存至寄存器1（如果结果为真则保存1，否则保存0）                                         |
|        | 00000110 00010101 0000000000000000 | 将寄存器1内的数与寄存器5中地址所指向的内存单元（2个字节）里存的数做逻辑与，结果保存至寄存器1（如果结果为真则保存1，否则保存0）                           |
|        | 00000111 00010000 0000000000000000 | 将寄存器1内的数与一个立即数（后16位）做逻辑或，结果保存至寄存器1（如果结果为真则保存1，否则保存0）                                         |
|        | 00000111 00010101 0000000000000000 | 将寄存器1内的数与寄存器5中地址所指向的内存单元（2个字节）里存的数做逻辑或，结果保存至寄存器1（如果结果为真则保存1，否则保存0）                           |
|        | 00001000 00010000 0000000000000000 | 将寄存器1内的数做逻辑非，结果保存至寄存器1（如果结果为真则保存1，否则保存0）                                                     |
|        | 00001000 00000101 0000000000000000 | 将寄存器5中地址所指向的内存单元（2个字节）里存的数做逻辑非，结果仍保存至寄存器5中地址所指向的内存单元（如果结果为真则保存1，否则保存0）                       |
| 比较指令   | 00001001 00010000 0000000000000000 | 将寄存器1内的数与一个立即数（后16位）比较，如两数相等，则标志寄存器被修置为0，如寄存器1大，则标志寄存器被置为1，如寄存器1小，则标志寄存器被置为\-1。              |
|        | 00001001 00010101 0000000000000000 | 将寄存器1内的数与寄存器5中地址所指向的内存单元（2个字节）里存的数比较，如两数相等，则标志寄存器被置为0，如寄存器1大，则标志寄存器被置为1，如寄存器1小，则标志寄存器被置为\-1。 |
| 跳转指令   | 00001010 00000000 0000000000000000 | 无条件跳转指令，转移至程序计数器加一个立即数（后16位）处执行。也就是说要修改程序计数器。                                                |
|        | 00001010 00000001 0000000000000000 | 如果标志寄存器内的值为0则转移至程序计数器加一个立即数（后16位）处执行。也就是说要修改程序计数器。                                           |
|        | 00001010 00000010 0000000000000000 | 如果标志寄存器内的值为1则转移至程序计数器加一个立即数（后16位）处执行。也就是说要修改程序计数器。                                           |
|        | 00001010 00000011 0000000000000000 | 如果标志寄存器内的值为\-1则转移至程序计数器加一个立即数（后16位）处执行。也就是说要修改程序计数器。                                         |
| 输入输出指令 | 00001011 00010000 0000000000000000 | 从输入端口读入一个整数并保存在寄存器1中。也就是从键盘读一个整数到寄存器1中。                                                      |
|        | 00001100 00010000 0000000000000000 | 将寄存器1中的数输出到输出端口。也就是将寄存器1中的数以整数的形式输出到显示器上，同时输出一个换行符。                                          |
| 多核版指令  | 00001101 00000000 0000000000000000 | 立即数（后16位）为内存地址，请求互斥对象，用于锁住立即数所指定的内存。                                                         |
|        | 00001110 00000000 0000000000000000 | 立即数（后16位）为内存地址，释放互斥对象，释放掉锁住立即数所指定的内存的互斥对象。与上一条指令对应。                                          |
|        | 00001111 00000000 0000000000000000 | 休眠立即数（后16位）毫秒。                                                                               |
