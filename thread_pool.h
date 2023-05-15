#pragma once

#include <pthread.h>

#include <iostream>
#include <functional>

#define QUEUE_MAX_COUNT 5000
#define THREAD_MAX_COUNT 5000

template<typename F>
class thread_pool
{
public:
    using task = std::function<F>;

private:
    pthread_t *pool_ptr;      /* 线程池指针 */
    int pool_size;            /* 线程池大小 */
    task *task_queue;         /* 队列指针 */
    int queue_size;           /* 队列大小 */
    pthread_mutex_t mutex;    /* 互斥量 */
    pthread_cond_t cv;        /* 条件变量 */
    int cur_task_count;       /* 当前任务队列的任务数量 */
    int workers_queue_index;  /* 消费者线程指向的任务队列下标 */
    int add_task_queue_index; /* 生产者线程添加任务下标 */
    bool shutdown_;           /* 关闭线程池标志 */
    int thread_started;       /* 已启动线程数量 */

private:
    /// @brief 消费者线程
    /// @param arg 线程池指针
    /// @return 线程池
    static void *workers(void *arg)
    {
        thread_pool *pool = (thread_pool *)arg;

        while (true)
        {
            // std::cout << "try to get lock..." << pthread_self() << std::endl;
            pthread_mutex_lock(&(pool->mutex));
            // std::cout << "get lock" << pthread_self() << std::endl;

            // 为什么要用while而不用if
            while (pool->cur_task_count == 0 && !(pool->shutdown_))
            {
                // std::cout << "waiting... " << pthread_self() << std::endl;
                pthread_cond_wait(&(pool->cv), &(pool->mutex));
            }

            // 线程池要被关闭了，需要完成手头上所有的任务吗？
            if (pool->shutdown_  && pool->cur_task_count == 0)
            {
                // std::cout << "shutdown!!" << std::endl;
                break;
            }

            // 每个线程都要一个一个来按次序排队领取任务
            pool->task_queue[pool->workers_queue_index]();
            // 当前任务队列减少一个任务
            pool->cur_task_count--;
            pool->workers_queue_index = (pool->workers_queue_index + 1) % pool->queue_size;
            pthread_mutex_unlock(&(pool->mutex));
        }
        pool->thread_started--;
        // 这里的锁一定要记得释放!!!!!!!
        // 否则退出线程池时会导致已经终止的线程一直占有锁
        pthread_mutex_unlock(&(pool->mutex));
        pthread_exit(nullptr);
    }

    /// @brief 等待所有线程运行完毕
    void join_all()
    {
        for (int i = 0; i != pool_size; ++i)
        {
            if (pthread_join(pool_ptr[i], nullptr) != 0)
            {
                std::exception();
            }
        }
    }

    /// @brief 释放线程池资源
    void free_source()
    {
        // join_all();
        delete[] pool_ptr;
        delete[] task_queue;
        pthread_cond_destroy(&cv);
        pthread_mutex_destroy(&mutex);
    }

public:
    thread_pool(int pool_s = 10, int queue_s = 10)
        : pool_ptr(nullptr),
          pool_size(pool_s),
          task_queue(nullptr),
          queue_size(queue_s),
          mutex(PTHREAD_MUTEX_INITIALIZER),
          cv(PTHREAD_COND_INITIALIZER),
          cur_task_count(0),
          workers_queue_index(0),
          add_task_queue_index(0),
          shutdown_(false),
          thread_started(0)
    {
        if (pool_size <= 0 || queue_size <= 0 || pool_size > THREAD_MAX_COUNT || queue_size > QUEUE_MAX_COUNT)
        {
            throw std::exception();
        }

        pool_ptr = new pthread_t[pool_size];
        task_queue = new task[queue_size];

        if (pool_ptr == nullptr || task_queue == nullptr)
        {
            throw std::exception();
        }
    }

    /// @brief 启动线程池
    void start_pool()
    {
        for (int i = 0; i < pool_size; ++i)
        {
            if (pthread_create(&(pool_ptr[i]), nullptr, &workers, this) != 0) // 线程创建失败返回非0
            {
                delete[] pool_ptr;
                throw std::exception();
            }
            ++thread_started;
        }
    }

    /// @brief 生产者线程添加任务
    /// @param t_ 任务
    /// @return 添加情况
    bool add_task(const task &t_)
    {
        pthread_mutex_lock(&mutex);
        task_queue[add_task_queue_index] = t_;
        add_task_queue_index = (add_task_queue_index + 1) % queue_size;
        cur_task_count++;
        pthread_cond_signal(&cv); // 分析pthread_cond_signal和pthread_mutex_unlock调用顺序有什么讲究

        // 添加死循环测试：无法释放mutex锁，因此通知了那些在cv条件变量等待的线程也没有用，因为他们获得不到锁
        //  while (1)
        //  {
        //      ;
        //  }

        pthread_mutex_unlock(&mutex);

        return true;
    }

    /// @brief 关闭线程池
    void close_pool()
    {

        if (shutdown_)
        {
            return;
        }

        shutdown_ = true;
        pthread_mutex_lock(&mutex);

        if (pthread_cond_broadcast(&cv) != 0)
        {
            pthread_mutex_unlock(&mutex);
            throw std::exception();
        }
        pthread_mutex_unlock(&mutex);

        for (int i = 0; i != pool_size; ++i)
        {
            if (pthread_join(pool_ptr[i], nullptr) != 0)
            {
                std::exception();
            }
        }
    }

    ~thread_pool()
    {
        std::cout << "free" << std::endl;
        free_source();
    }
};



//Test


// thread_pool<void()> p;
// p.start_pool();
// p.add_task([]()
//            { cout << 123 << endl; });

// p.close_pool();
// ForwardList<int> list;