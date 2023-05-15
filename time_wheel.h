#pragma once

#include <time.h>

#include <vector>
#include <functional>
#include <tuple>

/* 单级时间轮定时器 */
// todo 实现多级定时器
class time_wheel
{
public:
    using task_type = std::function<void()>;

private:
    int time_fd;                                   /* 备用timer_creat_fd */
    int cur_sec;                                   /* 当前时间轮的下标 */
    std::vector<std::vector<task_type>> fun_timer; /* 时间轮 seconds,task */

    // int cur_min; /* 多级时间轮分针下标 备用 */
    // std::tuple<int, int, int> clock_;
    // int sec;
    // int min;
    // int hour;
    // std::vector<std::vector<std::vector<task_type>>> fun_timer;

public:
    time_wheel() : time_fd(-1), cur_sec(0), fun_timer(60) {}

    /// @brief 向时间轮中注册不超过60s的定时任务
    /// @param fun 任务
    /// @param time 定时时间
    void regist_task(const task_type &fun, int time)
    {
        // std::cout << time % 60 << std::endl;
        fun_timer[time % 60].push_back(fun);
    }
    

    /// @brief 每过一秒tick一次 移动一次时间轮下标
    void tick()
    {
        ++cur_sec;
        for (auto &f : fun_timer[cur_sec])
        {
            f();
        }
        cur_sec = cur_sec % 60;
    }

    ~time_wheel() = default;
};





// Test


// time_wheel ti;
// ti.regist_task(fff, 1);
// ti.regist_task(fff2, 10);
// sleep(1);
// ti.tick(); // 1s
// cout << "1s" << endl;
// sleep(1);
// ti.tick();
// cout << "2s" << endl;
// sleep(1);
// ti.tick();
// cout << "3s" << endl;
// sleep(1);
// ti.tick();
// cout << "4s" << endl;
// sleep(1);
// ti.tick();
// cout << "5s" << endl;
// sleep(1);
// ti.tick();
// cout << "6s" << endl;
// sleep(1);
// ti.tick();
// cout << "7s" << endl;
// sleep(1);
// ti.tick();
// cout << "8s" << endl;
// sleep(1);
// ti.tick();
// cout << "9s" << endl;
// sleep(1);
// ti.tick();
// cout << "10s" << endl;
// sleep(1);
// ti.tick();
// cout << "11s" << endl;
