#include "server.hpp"
#include <condition_variable>

class threadPoolServer : public server
{
    public:
        threadPoolServer(int maxThreads = 4);
        ~threadPoolServer()  = default;

        void run();

        void create_server_sock();
        
        int wait_for_client();

        void execute_thread(int id);

        void destroy_thread();

    private:

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