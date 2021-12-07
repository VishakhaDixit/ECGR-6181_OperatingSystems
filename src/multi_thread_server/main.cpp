#include <multi_thread_server.hpp>

int main()
{
    std::cout << "Multi threaded server started" << std::endl;
    multiThreadServer srv;
    srv.run();
}