#include "thread_pool_server.hpp"
#include <condition_variable>

class threadPoolPriorityServer : public threadPoolServer
{
    public:
        threadPoolPriorityServer(int maxThreads = 4);
        ~threadPoolPriorityServer() = default;

        void run();

        void server_thread();
        
        std::thread serverThread;

};