#include "multi_thread_server.hpp"

void multiThreadServer::run()
{
    create_server_sock();
}

void multiThreadServer::stop()
{
    mThread.join();
}

void multiThreadServer::create_thread()
{
     while(1)
    {
        wait_for_client();
        mThread = std::thread{&server::execute_thread, this};
        mThread.join();
    }
}