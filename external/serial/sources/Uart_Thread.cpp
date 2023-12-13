#include "Uart_Thread.hpp"

/**
 * @brief Construct a new Uart_Thread object
 *
 * @param uart_port 串口端口
 * @param enable_thread_read 是否开启读串口线程，默认不开启
 * @param enable_thread_write 是否开启写串口线程，默认不开启
 */
Uart_Thread::Uart_Thread(std::string uart_port, bool enable_thread_read, bool enable_thread_write)
{
    // 串口初始化
    InitSerialPort(uart_port);

    // 开启读串口线程
    if (enable_thread_read)
    {
        Enable_Thread_Read_Uart();
    }

    // 开启写串口线程
    if (enable_thread_write)
    {
        Enable_Thread_Write_Uart();
    }
}

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

#if enable_show_read
        printf("read length %ld\n", read_length);
#endif

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
                uint32_t X = 0;
                memcpy(&X, &aligned_data[3], 4);
#if enable_show_read
                printf("X:%d\n", X);
                printf("Receive Mission 1:\n");
                ShowReadBuff();
#endif
            }
            else if (aligned_data[2] == 0x02)
            {
                /*任务2*/
                uint32_t X = 0;
                memcpy(&X, &aligned_data[4], 4);
#if enable_show_read
                printf("X:%d\n", X);
                printf("Receive Mission 2:\n");
                ShowReadBuff();
#endif
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

        std::queue<uint8_t> local_writeBuff_queue;
        /*堵塞等待条件变量通知*/
        {
            std::unique_lock<std::mutex> lock(mutex_write_uart_queue);
            cv_write_uart_queue.wait(lock, [this]
                                     { return !writeBuff_queue.empty(); });
            local_writeBuff_queue = std::move(writeBuff_queue);
        }

        /*从队列中获取数据*/
        while (!local_writeBuff_queue.empty())
        {
            uint8_t writeBuff[uart_length] = {0};
            size_t index = 0;
            for (index; index < uart_length && !local_writeBuff_queue.empty(); index++)
            {
                writeBuff[index] = local_writeBuff_queue.front();
                local_writeBuff_queue.pop();
            }

            if (index == uart_length)
            {
                WriteBuffer(writeBuff);

#if enable_show_write
                ShowWriteBuff();
#endif
                float usleep_time = 1000000.0 / send_frequency_hz;
                // 休眠usleep_time ms
                usleep(usleep_time);
            }
        }
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
 * @brief 发送兼容Vofa JustFloat协议的串口数据
 *
 * @param data 待发送的浮点数数据
 */
void Uart_Thread::Mission_Send_Vofa_JustFloat(std::vector<float> data)
{
    /*转换Vofa JustFloat协议*/
    const size_t data_size = data.size();
    uint8_t res_writeBuff[4 * (data_size + 1)] = {0};
    for (size_t i = 0; i < data_size; i++)
    {
        memcpy(&res_writeBuff[4 * i], &data[i], 4);
    }
    res_writeBuff[4 * data_size] = 0x00;
    res_writeBuff[4 * data_size + 1] = 0x00;
    res_writeBuff[4 * data_size + 2] = 0x80;
    res_writeBuff[4 * data_size + 3] = 0x7f;

#if enable_show_write
    printf("Mission Vofa Send:");
    for (size_t i = 0; i < 4 * (data_size + 1); i++)
    {
        printf("%x ", res_writeBuff[i]);
    }
    printf("\n");
#endif

    /*给写入串口进行上锁保护*/
    std::lock_guard<std::mutex> res_lock_write_uart(mutex_write_uart);

    WriteVofaJustFloat(res_writeBuff, 4 * (data_size + 1));
}

/**
 * @brief 任务1赋值串口模板函数
 *
 * @param uart_ptr 赋值的串口
 * @param X 任务1执行完毕后的数据
 */
void Uart_Thread_Space::Mission1_Assignment(Uart_Thread *uart_ptr, uint32_t X)
{
    printf("Mission1 Send!\n");

    /*为写串口缓冲区赋值*/
    uart_ptr->writeBuff[2] = 0x01;
    memcpy(&uart_ptr->writeBuff[3], &X, 4);
}

/**
 * @brief 任务2赋值串口模板函数
 *
 * @param uart_ptr 赋值的串口
 * @param X 任务2执行完毕后的数据
 * @param Y 任务2执行完毕后的数据
 */
void Uart_Thread_Space::Mission2_Assignment(Uart_Thread *uart_ptr, uint16_t X, float Y)
{
    printf("Mission2 Send!\n");

    /*为写串口缓冲区赋值*/
    uart_ptr->writeBuff[2] = 0x02;
    memcpy(&uart_ptr->writeBuff[3], &X, 2);
    memcpy(&uart_ptr->writeBuff[5], &Y, 4);
}