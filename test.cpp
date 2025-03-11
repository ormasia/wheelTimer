#include "wheel_timer.h"
#include <iostream>
#include <vector>
#include <cassert>

using namespace ORMAR;

void testImmediateTask() {
    wheelTimer timer(10, 100);
    bool executed = false;
    
    timer.addTimer([&executed]() { executed = true; }, 0);
    timer.tick();
    
    assert(executed && "Immediate task should execute on first tick");
}

void testDelayedSingleTask() {
    wheelTimer timer(10, 100);
    bool executed = false;
    
    timer.addTimer([&executed]() { executed = true; }, 100);
    timer.tick(); // 0 -> 1 (processed slot 0)
    timer.tick(); // 1 -> 2 (processed slot 1)
    
    assert(executed && "Delayed task should execute after 2 ticks");
}

void testMultipleTasksOrder() {
    wheelTimer timer(10, 100);
    std::vector<bool> results = {false, false, false};
    
    // 添加不同延迟的任务
    timer.addTimer([&results]() { results[0] = true; }, 0);
    timer.addTimer([&results]() { results[1] = true; }, 100);
    timer.addTimer([&results]() { results[2] = true; }, 200);
    
    // 验证执行顺序
    timer.tick(); // 执行立即任务
    assert(results[0] && "First task should execute immediately");
    
    timer.tick(); // 执行100ms任务
    assert(results[1] && "Second task should execute after 1 tick");
    
    timer.tick(); // 执行200ms任务
    assert(results[2] && "Third task should execute after 2 ticks");
}

int main() {
    std::cout << "Running wheelTimer tests...\n";
    
    testImmediateTask();
    testDelayedSingleTask();
    testMultipleTasksOrder();
    
    std::cout << "All tests passed successfully!\n";
    return 0;
}