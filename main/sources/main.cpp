#include "main.hpp"

using namespace std;

// 串口线程结构体
Uart_Thread uart;

/*任务1*/
void thread1();

/*任务2*/
void thread2();

int main()
{
    // uart初始化
    uart.InitSerialPort("/dev/ttyUSB0");

    // 开启读写串口线程
    uart.Enable_Thread_Read_Uart();

    /*写串口方法一：开始写串口线程，在while1中一直发送信息，但会占用较多系统资源*/
    // uart.Enable_Thread_Write_Uart();

    /*写串口方法二：在不同的线程任务中，每完成一次任务发送一次串口，
      但要记得写发送函数中使用互斥锁，防止多线程间任务串口错位*/
    thread th1(thread1);
    thread th2(thread2);

    /*主线程休眠*/
    sleep(100000000);

    return 0;
}

/*任务1*/
void thread1()
{
    uint32_t X = 0;
    while (1)
    {
        X++;
        // to do ...

        /*完成任务1后，发送一次串口*/
        uart.Mission1_Send(X);

        /*睡眠4s*/
        sleep(4);
    }
}

/*任务2*/
void thread2()
{
    uint32_t X = 0;
    while (1)
    {
        X++;
        // to do ...

        /*完成任务2后，发送一次串口*/
        uart.Mission2_Send(X);

        /*睡眠2s*/
        sleep(2);
    }
}