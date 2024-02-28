/**
 * @file main2.cpp
 * @author your name (you@domain.com)
 * @brief 没啥卵用的测试代码
 * @version 0.1
 * @date 2023-10-12
 *
 * @copyright Copyright (c) 2023
 *
 */

// #include <stdio.h>      /*标准输入输出定义*/
// #include <stdlib.h>     /*标准函数库定义*/
// #include <unistd.h>     /*Unix 标准函数定义*/
// #include <fcntl.h>      /*文件控制定义*/
// #include <errno.h>      /*错误号定义*/
// #include <stdint.h>     /*标准整数类型*/

// #include <iostream>     /*标准输入输出定义*/
// #include <string>       /*对字符串的操作函数和类型定义*/
// #include <cstring>      /*对字符串的操作函数和字符处理函数*/

// #include <sys/ioctl.h>  /*I/O控制函数*/
// #include <sys/types.h>  /*基本数据类型*/
// #include <sys/stat.h>   /*文件状态的结构和常量*/
// #include <sys/termios.h>    /*PPSIX 终端控制定义*/

// int main() {
//     int fd;
//     struct termios options;
//     fd_set rfds;

//     // 打开串口设备
//     fd = open("/dev/pts/5", O_RDWR | O_NOCTTY | O_NDELAY);
//     if (fd < 0) {
//         perror("open");
//         return -1;
//     }

//     // 配置串口参数
//     tcgetattr(fd, &options);
//     options.c_cflag = B9600 | CS8 | CLOCAL | CREAD;
//     options.c_iflag = IGNPAR;
//     options.c_oflag = 0;
//     options.c_lflag = 0;
//     options.c_cc[VTIME] = 0;
//     options.c_cc[VMIN] = 1;
//     tcsetattr(fd, TCSANOW, &options);

//     while (1) {
//         // 使用 select 函数监听串口文件描述符的可读事件
//         FD_ZERO(&rfds);
//         FD_SET(fd, &rfds);
//         select(fd + 1, &rfds, NULL, NULL, NULL);

//         // 读取串口数据
//         uint8_t buf[10];
//         int n = read(fd, buf, sizeof(buf));
//         if (n > 0) {
//             printf("Received n:%d\n", n);
//             for(int i=0; i<n; i++)
//             {
//                 printf("%x ", buf[i]);
//             }
//             printf("\n");
//         }
//     }

//     // 关闭串口设备
//     close(fd);

//     return 0;
// }

#include "main.hpp"

Uart_Thread uart;

void mission_assignment(Uart_Thread *uart_ptr, uint32_t X)
{
    /*为写串口缓冲区赋值*/
    uart_ptr->writeBuff[2] = 0x01;
    memcpy(&uart_ptr->writeBuff[3], &X, 4);

    printf("hello1\n");
}

class Fork
{
public:
    /**
     * @brief 任务发送串口模板函数
     *
     * @tparam Args 函数指针的参数
     * @param assignment_func 为writeBuff赋值的函数
     * @param args assignment_func的参数
     */
    template <typename... Args>
    void Mission222_Send(void (*Assignment_Func)(Uart_Thread *, Args...), Uart_Thread *res_uart, Args... args);
};

template <typename... Args>
void Fork::Mission222_Send(void (*Assignment_Func)(Uart_Thread *, Args...), Uart_Thread *res_uart, Args... args)
{
    // /*清空写串口缓冲区*/
    // ClearWriteBuff();

    /*为写串口缓冲区赋值*/
    Assignment_Func(res_uart, args...);

    printf("hello2\n");

    // /*给写入串口进行上锁保护，并写入串口*/
    // std::lock_guard<std::mutex> res_lock_write_uart(mutex_write_uart);
    // WriteBuffer();
}

Fork forkew;

int main()
{
    int X = 32;

    uart.Mission_Send(mission_assignment, (uint32_t)X);

    forkew.Mission222_Send(mission_assignment, &uart, (uint32_t)X);
}