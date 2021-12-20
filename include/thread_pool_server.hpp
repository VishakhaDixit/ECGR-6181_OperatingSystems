#include "server.hpp"
#include <condition_variable>
#include "bounded_queue.hpp"

#define THREAD_POOL_SERVER_PORT 8082

class threadPoolServer : public server
{
    public:
        threadPoolServer(int maxThreads = 3);
        ~threadPoolServer()  = default;

        void run();

        void create_server_sock(int port);
        
        int wait_for_client();

        void execute_thread(int id);

        void destroy_thread();

    protected:

        std::vector<std::thread> mThreads;

        //Queue to store connection file descriptor for incoming connections
        SharedQueue queue;

        struct sockaddr_in serverAddr, clientAddr;
        int server_fd;
        int addrlen;
};