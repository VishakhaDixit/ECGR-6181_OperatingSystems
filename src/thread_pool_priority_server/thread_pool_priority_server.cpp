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
    int policy = SCHED_FIFO;

    pthread_attr_init(&tattr);
    pthread_attr_getschedparam(&tattr, &param);
    pthread_attr_setinheritsched(&tattr, PTHREAD_EXPLICIT_SCHED);
    pthread_attr_setschedpolicy(&tattr, policy);
    param.sched_priority = param.sched_priority - 1;
    pthread_attr_setschedparam(&tattr, &param);

    for (auto &t : mThreads) {
        pthread_setschedparam(t.native_handle(), policy, &param);
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
            connQueue.push(client_fd);
            mSignal.notify_one();
        }
    }
}