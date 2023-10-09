#include "Uart_Thread.hpp"

/**
 * @brief 读串口线程函数
 */
void Uart_Thread::Thread_Read_Uart()
{
    while (1)
    {
        /*是否退出线程*/
        if (this->flag_thread_read_uart == false)
        {
            break;
        }

        /*使用 select 函数堵塞，监听串口文件描述符的可读事件*/
        Select();

        /*堵塞读取串口*/
        ssize_t read_length = ReadBuffer();

        printf("read length %ld\n", read_length);

        /*读取到串口后将数据送入队列*/
        PushreadBuffToQueue(read_length);

        /*从队列从获取正确的数据*/
        uint8_t aligned_data[uart_length] = {0};
        if (GetAlignedFromQueue(aligned_data) != -1)
        {
            /*从队列中获取正确的数据成功*/
            if (aligned_data[2] == 0x01)
            {
                /*任务1*/
                printf("Receive Mission 1:\n");
                ShowReadBuff();
                uint32_t X = 0;
                memcpy(&X, &aligned_data[3], 4);
                printf("X:%d\n", X);
            }
            else if (aligned_data[2] == 0x02)
            {
                /*任务2*/
                printf("Receive Mission 2:\n");
                ShowReadBuff();
                uint32_t X = 0;
                memcpy(&X, &aligned_data[4], 4);
                printf("X:%d\n", X);
            }
        }
        else
        {
            /*从队列中获取正确的数据失败*/

            /*打印错误数据*/
            COUT_RED_START;
            ShowReadBuff();
            COUT_COLOR_END;
        }
    }
}

/**
 * @brief 写串口线程函数
 */
void Uart_Thread::Thread_Write_Uart()
{
    uint32_t X = 0;
    while (1)
    {
        /*是否退出线程*/
        if (this->flag_thread_write_uart == false)
        {
            break;
        }

        /*清空写串口缓冲区*/
        ClearWriteBuff();

        /*为写串口缓冲区赋值*/
        writeBuff[2] = 0x01;

        X++;
        memcpy(&writeBuff[3], &X, 4);

        /*给写入串口进行上锁保护，并写入串口*/
        std::lock_guard<std::mutex> res_lock_write_uart(mutex_write_uart);
        WriteBuffer();

        ShowWriteBuff();

        // 休眠100ms
        usleep(100000);
    }
}

/**
 * @brief 开启读串口线程
 */
void Uart_Thread::Enable_Thread_Read_Uart()
{
    flag_thread_read_uart = true;

    thread_read_uart = std::thread(&Uart_Thread::Thread_Read_Uart, this);
}

/**
 * @brief 开启写串口线程
 */
void Uart_Thread::Enable_Thread_Write_Uart()
{
    flag_thread_write_uart = true;

    thread_write_uart = std::thread(&Uart_Thread::Thread_Write_Uart, this);
}

/**
 * @brief 关闭读串口线程
 */
void Uart_Thread::Disable_Thread_Read_Uart()
{
    flag_thread_read_uart = false;
}

/**
 * @brief 关闭写串口线程
 */
void Uart_Thread::Disable_Thread_Write_Uart()
{
    flag_thread_write_uart = false;
}

/**
 * @brief 任务1发送串口模板
 */
void Uart_Thread::Mission1_Send(uint32_t X)
{
    /*清空写串口缓冲区*/
    ClearWriteBuff();

    /*为写串口缓冲区赋值*/
    writeBuff[2] = 0x01;
    memcpy(&writeBuff[3], &X, 4);

    /*给写入串口进行上锁保护，并写入串口*/
    std::lock_guard<std::mutex> res_lock_write_uart(mutex_write_uart);
    WriteBuffer();

    printf("This is Mission1:\n");
    ShowWriteBuff();
}

/**
 * @brief 任务2发送串口模板
 */
void Uart_Thread::Mission2_Send(uint32_t X)
{
    /*清空写串口缓冲区*/
    ClearWriteBuff();

    /*为写串口缓冲区赋值*/
    writeBuff[2] = 0x02;
    memcpy(&writeBuff[4], &X, 4);

    /*给写入串口进行上锁保护，并写入串口*/
    std::lock_guard<std::mutex> res_lock_write_uart(mutex_write_uart);
    WriteBuffer();

    printf("This is Mission2:\n");
    ShowWriteBuff();
}