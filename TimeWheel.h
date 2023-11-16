#pragma once

#include <time.h>
#include <unistd.h>
#include <vector>
#include <functional>
#include <tuple>

#include <iostream>

/* 单级时间轮定时器 */
// todo 实现多级定时器
class Alarm
{
public:
    using task = std::tuple<size_t, const std::function<void()>>;

private:
    size_t cur_sec;                           /* 当前时间轮的下标 */
    std::vector<std::vector<task>> fun_timer; /* 时间轮 seconds,task */
    bool quit;

    // int cur_min; /* 多级时间轮分针下标 备用 */
    // std::tuple<int, int, int> clock_;
    // int sec;
    // int min;
    // int hour;
    // std::vector<std::vector<std::vector<task_type>>> fun_timer;

public:
    Alarm() : cur_sec(0), fun_timer(60), quit(false) {}

    /// @brief 向时间轮中注册不超过60s的定时任务
    /// @param fun 任务
    /// @param time 定时时间
    void add_task(size_t time, const std::function<void()> &fun)
    {
        // std::cout << time % 60 << std::endl;
        auto element = std::make_tuple(time, std::move(fun));
        fun_timer[time % 60].push_back(element);
    }

    void add_task_agagin(const task &element)
    {
        size_t future_time = cur_sec + std::get<0>(element);
        fun_timer[future_time % 60].push_back(element);
    }

    /// @brief 每过一秒tick一次 移动一次时间轮下标
    void start()
    {
        while (!quit)
        {
            sleep(1);
            cur_sec = ++cur_sec % 60;
            for (auto &task_element : fun_timer[cur_sec])
            {
                std::get<1>(task_element)();
                add_task_agagin(task_element);
                std::cout << cur_sec << std::endl;
            }
            fun_timer[cur_sec].clear();
        }
    }

    inline void stop()
    {
        quit = true;
    }

    ~Alarm() = default;
};
