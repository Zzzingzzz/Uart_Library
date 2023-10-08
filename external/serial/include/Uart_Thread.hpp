#ifndef __UART_THREAD_HPP
#define __UART_THREAD_HPP

#include "Uart.hpp"

#include <thread>   /*多线程*/
#include <mutex>    /*互斥锁*/

class Uart_Thread:  public Uart
{
    private:
        /*读串口线程*/
        std::thread thread_read_uart;
        /*写串口线程*/
        std::thread thread_write_uart;

        /*写串口线程锁*/
        std::mutex mutex_write_uart;

        /*是否开启读串口线程*/
        bool flag_thread_read_uart = false;
        /*是否开启写串口线程*/
        bool flag_thread_write_uart = false;

        /**
         * @brief 读串口线程函数
        */
        void Thread_Read_Uart();

        /**
         * @brief 写串口线程函数
        */
        void Thread_Write_Uart();

    public:

        /*是否显示原始读串口帧数据*/
        bool enable_show_read = false;
        /*是否显示原始写串口帧数据*/
        bool enable_show_write = false;

        /**
         * @brief 开启读串口线程
        */
        void Enable_Thread_Read_Uart();

        /**
         * @brief 开启写串口线程
        */
        void Enable_Thread_Write_Uart();

        /**
         * @brief 关闭读串口线程
        */
        void Disable_Thread_Read_Uart();

        /**
         * @brief 关闭写串口线程
        */
        void Disable_Thread_Write_Uart();

        /**
         * @brief 任务1发送串口模板
         * @param X 任务1执行完毕后的数据
        */
        void Mission1_Send(uint32_t X);

        /**
         * @brief 任务2发送串口模板
         * @param X 任务2执行完毕后的数据
        */
        void Mission2_Send(uint32_t X);
};

#endif
