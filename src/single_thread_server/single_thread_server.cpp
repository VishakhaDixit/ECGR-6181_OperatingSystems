#include "single_thread_server.hpp"

void singleThreadServer::run()
{
    create_server_sock();
}

void singleThreadServer::stop()
{
    mThread.join();
}

void singleThreadServer::create_thread()
{
     while(1)
    {
        wait_for_client();
        mThread = std::thread{&server::execute_thread, this};
        mThread.join();
    }
}