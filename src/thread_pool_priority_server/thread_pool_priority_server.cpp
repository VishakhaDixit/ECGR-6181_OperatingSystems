#include "thread_pool_priority_server.hpp"

#include <thread>
#include <iostream>

#include <pthread.h>
#include <sched.h>

threadPoolPriorityServer::threadPoolPriorityServer(int maxThreads)
    : threadPoolServer{ maxThreads }
{
    // Set the priorities
    pthread_attr_t tattr;
    sched_param param;
    int core = 0, num_cores = 4;
    cpu_set_t cpuset;
    int policy = SCHED_FIFO;

    CPU_ZERO(&cpuset);
    CPU_SET(core, &cpuset);

    pthread_attr_init(&tattr);
    pthread_attr_getschedparam(&tattr, &param);
    pthread_attr_setinheritsched(&tattr, PTHREAD_EXPLICIT_SCHED);
    pthread_attr_setschedpolicy(&tattr, policy);
    param.sched_priority = param.sched_priority - 1;
    pthread_attr_setschedparam(&tattr, &param);

    for (auto &t : mThreads) 
    {
        pthread_setschedparam(t.native_handle(), policy, &param);
        if (pthread_setaffinity_np(t.native_handle(), sizeof(cpu_set_t), &cpuset) != 0) 
        {
            cout << endl << "can't set thread CPU core";
        }
        else
            cout << "Set thread to core: " << core << endl;
    
        core = (core + 1) % num_cores;
    }
}

void threadPoolPriorityServer::run()
{
    create_server_sock(THREAD_POOL_PRIORITY_SERVER_PORT);
    serverThread = std::thread{ &threadPoolPriorityServer::server_thread, this }; 
    pthread_attr_t tattr;
    sched_param param;
    int policy = SCHED_FIFO;

    pthread_attr_init(&tattr);
    pthread_attr_getschedparam(&tattr, &param);
    pthread_attr_setinheritsched(&tattr, PTHREAD_EXPLICIT_SCHED);
    pthread_attr_setschedpolicy(&tattr, policy);
    param.sched_priority = param.sched_priority + 1;
    pthread_attr_setschedparam(&tattr, &param);

    pthread_setschedparam(serverThread.native_handle(), policy, &param);
}

void threadPoolPriorityServer::server_thread()
{
    std::cout << std::endl << "Creating server thread with higher priority" << std::endl;
    while (true)
    {
        int client_fd = wait_for_client();
        if(client_fd != -1)
        {
            if(!queue.push_back(client_fd))
            {
                cout << endl << "QUEUE IS FULL!! CANNOT ACCEPT ANYMORE REQUESTS......" << endl;
            }
        }
    }
}