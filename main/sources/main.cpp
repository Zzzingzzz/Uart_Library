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
Uart_Thread uart2("/dev/ttyUSB1", true, false);

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
    // thread th2(thread2);

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
    timeval tt1, tt2;
    gettimeofday(&tt1, NULL);

    float X = 0.0;
    int interval = 1000; // 1000hz
    while (1)
    {
        /* 获取当前时间戳 */
        timeval start_time;
        gettimeofday(&start_time, NULL);

        X += 1.0;
        uart.Mission_Send(Uart_Thread_Space::Mission_test, &uart, X);

        /* 计算发送所花费的时间 */
        timeval end_time;
        gettimeofday(&end_time, NULL);
        int elapsed_time = (end_time.tv_sec - start_time.tv_sec) * 1e6 + end_time.tv_usec - start_time.tv_usec;

        /* 如果发送时间小于时间间隔，则休眠相应的时间 */
        if (elapsed_time < interval)
        {
            usleep(interval - elapsed_time);
        }

        COUT_RED_START;
        std::cout<<"X: "<<X<<std::endl;
        COUT_COLOR_END;

        if (X >= 30000)
            break;
    }

    gettimeofday(&tt2, NULL);

    /*运行时间*/
    float diff_time = 1e3 * (tt2.tv_sec - tt1.tv_sec) + (tt2.tv_usec - tt1.tv_usec) / 1000.0f;
    std::cout << "sent time " << diff_time << std::endl;
}

// /*任务2*/
// void thread2()
// {
//     int X = 0;
//     double Y = 0.0;
//     while (1)
//     {
//         X++;
//         Y += 0.2;
//         // to do ...

//         /*完成任务2后，发送一次串口，记得赋值时使用强制类型转换保证数据类型一致*/
//         uart.Mission_Send(Uart_Thread_Space::Mission2_Assignment, &uart, (uint16_t)X, (float)Y);

//         /*睡眠2s*/
//         sleep(2);
//     }
// }