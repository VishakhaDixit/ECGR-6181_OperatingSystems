#include "thread_pool_priority_server.hpp"

int main()
{
    std::cout << "Creating worker threads with lower priority" << std::endl;
    threadPoolPriorityServer srv;
    srv.run();
    srv.serverThread.join();
}