#include "bounded_queue.hpp"

    int& SharedQueue::front()
    {
        std::unique_lock<std::mutex> mlock(mutex_);
        while (conqueue_.empty())
        {
            cout << endl << "QUEUE IS EMPTY !!! NOTHING TO PROCESS....." << endl;
            cond_.wait(mlock);
        }
        return conqueue_.front();
    }

    void SharedQueue::pop_front()
    {
        std::unique_lock<std::mutex> mlock(mutex_);
        while (conqueue_.empty())
        {
            cout << endl << "Queue is empty, nothing to process !!!" << endl;
            cond_.wait(mlock);
        }
        conqueue_.pop_front();
    }   

    bool SharedQueue::push_back(const int& item)
    {
        if(size() < MAX_CAPACITY)
        {
            std::unique_lock<std::mutex> mlock(mutex_);
            conqueue_.push_back(item);
            mlock.unlock();     // unlock before notificiation to minimize mutex con
            cond_.notify_one(); // notify one waiting thread
            return true;
        }
        else
        {
            return false;
        }
    }  

    int SharedQueue::size()
    {
        std::unique_lock<std::mutex> mlock(mutex_);
        int size = conqueue_.size();
        mlock.unlock();
        return size;
    }