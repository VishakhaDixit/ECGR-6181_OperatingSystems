#include "thread_pool_server.hpp"
#include <condition_variable>

#define THREAD_POOL_PRIORITY_SERVER_PORT 8083

class threadPoolPriorityServer : public threadPoolServer
{
    public:
        threadPoolPriorityServer(int maxThreads = 4);
        ~threadPoolPriorityServer() = default;

        void run();

        void server_thread();
        
        std::thread serverThread;

};