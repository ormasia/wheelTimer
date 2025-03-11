#ifndef TIME_WHEEL_H
#define TIME_WHEEL_H

#include <vector>
#include <list>
#include <functional>
#include <mutex>

namespace ORMAR
{
    class timerTask
    {
    public:
        timerTask(std::function<void()> cb, int rotation) : cb_(cb), rotation(rotation) {}
        int rotation; // 剩余的圈数
        std::function<void()> cb_;
    };

    class wheelTimer
    {
        friend class timerTask;

    private:
        std::vector<std::list<timerTask>> slots;
        int interval;    // 每个槽的时间间隔
        int slotsNum;    // 槽数
        int currentSlot; // 当前槽
        std::mutex mtx;

    public:
        // 槽数，每个槽的时间间隔
        wheelTimer(int slotsNum, int interval);
        void addTimer(std::function<void()> cb, int timeout);
        void tick();
        //todo 实现不需要内核态的时间轮转，在函数内部调用tick()，实现定时器触发任务；
        void start();
        ~wheelTimer();
    };

}
#endif