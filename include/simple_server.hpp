#include "server.hpp"

class simpleServer : public server
{
    public:
        simpleServer() = default;
        ~simpleServer()  = default;

        void run();

        void create_server_sock();
        
        int wait_for_client();

    private:

        struct sockaddr_in serverAddr, clientAddr;
        int server_fd, client_fd;
        int addrlen;
};