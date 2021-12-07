#include "server.hpp"
#include <atomic>

#define MULTI_SERVER_PORT 8081

class multiThreadServer : public server
{
    public:
        multiThreadServer() = default;
        ~multiThreadServer()  = default;

        void run();

        void create_server_sock(int port);
        
        int wait_for_client();

        void execute_thread(int id, int client_fd);

        void create_thread(int client_fd);

        void destroy_thread();

    private:
        static int threadId;
        struct ThreadData
        {
            std::atomic_bool finished;
            std::thread thread;
        };

        std::mutex mMutex;
        std::unordered_map<int, ThreadData> mThreads;

        struct sockaddr_in serverAddr, clientAddr;
        int server_fd;
        int addrlen;
};