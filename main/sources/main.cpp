#include <iostream>

#include "main.hpp"

#include <sys/time.h> 
#include <unistd.h>

using namespace std;

// 串口线程结构体
Uart_Thread uart;

int main()
{
    // uart初始化
    uart.InitSerialPort("/dev/ttyUSB0");

    // 开启读写串口线程
    uart.Enable_Thread_Read_Uart();
    uart.Enable_Thread_Write_Uart();

    while(1)
    {

    }

    return 0;
}