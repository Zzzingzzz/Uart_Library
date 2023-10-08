#ifndef __UART_HPP
#define __UART_HPP

#include <stdio.h>      /*标准输入输出定义*/
#include <stdlib.h>     /*标准函数库定义*/
#include <unistd.h>     /*Unix 标准函数定义*/
#include <fcntl.h>      /*文件控制定义*/
#include <errno.h>      /*错误号定义*/
#include <stdint.h>     /*标准整数类型*/

#include <iostream>     /*标准输入输出定义*/
#include <string>       /*对字符串的操作函数和类型定义*/
#include <cstring>      /*对字符串的操作函数和字符处理函数*/

#include <sys/ioctl.h>  /*I/O控制函数*/
#include <sys/types.h>  /*基本数据类型*/
#include <sys/stat.h>   /*文件状态的结构和常量*/
#include <sys/termios.h>    /*PPSIX 终端控制定义*/

#include "Queue_T.hpp"  /*自定义queue队列*/

class Uart
{
    private:

        /*每帧数据长度*/ 
        #define uart_length 10

        /*串口编号*/
        int fd = -1; 

    public:

        /*写串口缓冲区*/
        uint8_t writeBuff[uart_length];
        /*读串口缓冲区*/
        uint8_t readBuff[uart_length];  

        /*读线程队列*/
        Queue_T readBuff_queue;

        /**
         * @brief 串口初始化
         * @param dev 打开串口名称
         * @return -1表示打开串口失败，其他表示打开成功
         */
        int InitSerialPort(std::string dev);

        /**
         * @brief 读串口
         */
        int ReadBuffer();

        /**
         * @brief 写串口
         */
        int WriteBuffer();

        /**
         * @brief 打印读到的串口
         */
        void ShowReadBuff();

        /**
         * @brief 打印写的串口
         */
        void ShowWriteBuff();

        /**
         * @brief 将收到的串口帧加入队列
        */
        void PushreadBuffToQueue();

        /**
         * @brief 从队列中提取对齐好的数据
         * @param *pData 提取到的数据的数组指针
         * @return -1表示提取失败，其他表示提取成功
        */
        int8_t GetAlignedFromQueue(uint8_t *pData);

    private:

        /**
         * @brief 打开串口并返回串口设备文件描述
         * @param dev 打开串口名称
         */
        void OpenPort(std::string dev);

        /**
         * @brief 设置串口
         * @param speed 波特率
         * @param flow_ctrl 数据流控制方式
         * @param databits 数据位
         * @param stopbits 停止位
         */
        void SetUp(int speed, int flow_ctrl, int databits, int stopbits);

/****************打印颜色定义****************/
#define COUT_RED_START      std::cout << "\033[1;31m";
#define COUT_GREEN_START    std::cout << "\033[1;32m";
#define COUT_YELLOW_START   std::cout << "\033[1;33m";
#define COUT_BLUE_START     std::cout << "\033[1;34m";
#define COUT_PURPLE_START   std::cout << "\033[1;35m";
#define COUT_CYAN_START     std::cout << "\033[1;36m";
#define COUT_WHITE_START    std::cout << "\033[1;37m";
#define COUT_COLOR_END      std::cout << "\033[0m";

};

#endif