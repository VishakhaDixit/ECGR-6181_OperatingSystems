#include "server.hpp"

#define SIMPLE_SERVER_PORT 8080

class simpleServer : public server
{
    public:
        simpleServer() = default;
        ~simpleServer()  = default;

        void run();

        void create_server_sock(int port);
        
        int wait_for_client();

    private:

        struct sockaddr_in serverAddr, clientAddr;
        int server_fd, client_fd;
        int addrlen;
};