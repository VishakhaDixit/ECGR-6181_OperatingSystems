#include "server.hpp"
#include <condition_variable>

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

        //Mutex for getting connection file descriptor
        std::mutex     connLock;

        //Queue to store connection file descriptor for incoming connections
        std::queue<int> connQueue;

        //Signal to notify sleeping threads of availability of connections
        std::condition_variable mSignal;

        struct sockaddr_in serverAddr, clientAddr;
        int server_fd;
        int addrlen;
};