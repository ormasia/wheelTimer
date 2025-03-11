#include "wheel_timer.h"
#include <iostream>

ORMAR::wheelTimer::wheelTimer(int slotsNum, int interval) : slotsNum(slotsNum), interval(interval), currentSlot(0)
{
    slots.resize(slotsNum);
}

void ORMAR::wheelTimer::addTimer(std::function<void()> cb, int timeout)
{
    if (timeout < 0)
    {
        return;
    }
    int ticksNum = timeout / interval;//需要经历多少个槽
    int rotation = ticksNum / slotsNum;//需要经历多少圈
    int slot = (currentSlot + ticksNum) % slotsNum;//最终落在哪个槽
    // slots[slot].push_back(timerTask(cb, ticks, rotation));
    slots[slot].emplace_back(cb, rotation);
}

/**
 * @brief 触发该时间槽的所有定时任务,rotation--
 */
void ORMAR::wheelTimer::tick()
{
    std::lock_guard<std::mutex> lock(mtx);
    std::cout << "执行任务" << std::endl;
    std::list<timerTask> &bucket = slots[currentSlot];
    for (auto it = bucket.begin(); it != bucket.end();)
    {
        if (it->rotation > 0)
        {
            it->rotation--;
            ++it;
            continue;
        }
        if (it->rotation == 0)
        {
            it->cb_();
            it = bucket.erase(it);
        }
    }
    currentSlot = (currentSlot + 1) % slotsNum;
}

ORMAR::wheelTimer::~wheelTimer() {}
