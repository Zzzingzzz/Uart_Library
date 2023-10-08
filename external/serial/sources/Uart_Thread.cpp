#include "Uart_Thread.hpp"

/**
 * @brief 读串口线程函数
*/
void Uart_Thread::Thread_Read_Uart()
{
    while (1)
    {
        /*是否退出线程*/ 
        if(this->flag_thread_read_uart == false)
        {
            break;
        }

        /*堵塞读取串口*/
        ReadBuffer();

        /*读取到串口后将数据送入队列*/
        PushreadBuffToQueue();

        /*从队列从获取正确的数据*/
        uint8_t aligned_data[uart_length] = {0};
        if(GetAlignedFromQueue(aligned_data) != -1)
        {
            /*从队列中获取正确的数据成功*/
            if(aligned_data[2] == 'S')
            {
                uint8_t flag1 = aligned_data[3];
                uint8_t flag2 = aligned_data[8];
                uint16_t X=0, Y=0;
                memcpy(&X, &aligned_data[4], 2);
                memcpy(&Y, &aligned_data[6], 2);

                printf("flag1 %d X %d Y %d flag2 %d\n", flag1, X, Y, flag2);

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
    uint32_t X=0;
    while(1)
    {
        /*是否退出线程*/ 
        if(this->flag_thread_write_uart == false)
        {
            break;
        }

        memset(writeBuff, 0, sizeof(writeBuff));

        writeBuff[0] = '?';
        writeBuff[1] = '!';
        writeBuff[2] = 0x01;
        writeBuff[uart_length-1] = '!';

        X++;
        memcpy(&writeBuff[3], &X, 4);

        // 写入串口
        WriteBuffer();
        ShowWriteBuff();

        // 休眠100ms
        usleep(10000);
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