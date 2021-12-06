#include "server.hpp"

class singleThreadServer : public server
{
    public:
        singleThreadServer() = default;
        ~singleThreadServer()  = default;

        void run();

        void create_thread();

        void stop();

    private:
        std::thread mThread;
};