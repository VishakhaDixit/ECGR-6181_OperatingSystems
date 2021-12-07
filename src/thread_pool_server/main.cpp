#include "thread_pool_server.hpp"

int main()
{
    std::cout << std::endl << "Thread pool server started" << std::endl;
    threadPoolServer srv;
    srv.run();
}