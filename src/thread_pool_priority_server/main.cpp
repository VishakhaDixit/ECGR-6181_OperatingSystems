#include "thread_pool_priority_server.hpp"

int main()
{
    threadPoolPriorityServer srv;
    srv.run();
    srv.serverThread.join();
}