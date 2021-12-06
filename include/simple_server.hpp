#include "server.hpp"

class simpleServer : public server
{
    public:
        simpleServer() = default;
        ~simpleServer()  = default;

        void run();
};