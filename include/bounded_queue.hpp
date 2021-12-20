#pragma once

#include <queue>
#include <mutex>
#include <condition_variable>
#include <iostream>

#define MAX_CAPACITY 6
using namespace std;

class SharedQueue
{
public:
    SharedQueue() = default;
    ~SharedQueue() = default;

    int& front();
    void pop_front();

    bool push_back(const int& item);

    int size();

private:
    std::deque<int> conqueue_;
    std::mutex mutex_;
    std::condition_variable cond_;
};