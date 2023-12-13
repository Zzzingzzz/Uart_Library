/**
 * @file main.cpp
 * @author zzzing (1226196281@qq.com)
 * @brief 串口库主函数，包含一些调用示例
 * @version 2.1
 * @date 2023-10-12
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "main.hpp"

using namespace std;

// 串口线程结构体
Uart_Thread uart("/dev/ttyUSB0", true, true);

/*任务1*/
void thread1();

/*任务2*/
void thread2();

int main()
{
    /*写串口方法一：开始写串口线程，在while1中一直发送信息，但会占用较多系统资源*/
    // uart.Enable_Thread_Write_Uart();

    /*写串口方法二：在不同的线程任务中，每完成一次任务发送一次串口，
      但要记得写发送函数中使用互斥锁，防止多线程间任务串口错位*/
    thread th1(thread1);
    thread th2(thread2);

    // /*主线程休眠*/
    // sleep(100000000);

    /*此处给出使用Vofa JustFloat协议的方法*/
    float tt = 0;
    while (1)
    {
        tt += 0.1;

        /*建立四条曲线*/
        std::vector<float> datas;
        datas.push_back(sin(tt));
        datas.push_back(sin(2 * tt));
        datas.push_back(sin(3 * tt));
        datas.push_back(sin(4 * tt));

        /*发送vofa数据*/
        uart.Mission_Send_Vofa_JustFloat(datas);

        usleep(100000);
    }

    return 0;
}

/*任务1*/
void thread1()
{
    int X = 0;
    while (1)
    {
        X++;
        // to do ...

        /*完成任务1后，发送一次串口，记得赋值时使用强制类型转换保证数据类型一致*/
        uart.Mission_Send(Uart_Thread_Space::Mission1_Assignment, &uart, (uint32_t)X);

        /*睡眠4s*/
        sleep(4);
    }
}

/*任务2*/
void thread2()
{
    int X = 0;
    double Y = 0.0;
    while (1)
    {
        X++;
        Y += 0.2;
        // to do ...

        /*完成任务2后，发送一次串口，记得赋值时使用强制类型转换保证数据类型一致*/
        uart.Mission_Send(Uart_Thread_Space::Mission2_Assignment, &uart, (uint16_t)X, (float)Y);

        /*睡眠2s*/
        sleep(2);
    }
}