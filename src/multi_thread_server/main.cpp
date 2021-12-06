#include <multi_thread_server.hpp>

int main()
{
    multiThreadServer server;

    server.run();
    server.create_thread();
}