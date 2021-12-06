#include <single_thread_server.hpp>

int main()
{
    singleThreadServer server;

    server.run();
    server.create_thread();
}