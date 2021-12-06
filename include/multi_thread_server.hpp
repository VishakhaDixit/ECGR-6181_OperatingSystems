#include "server.hpp"

class multiThreadServer : public server
{
    public:
        multiThreadServer() = default;
        ~multiThreadServer()  = default;

        void run();

        void create_thread();

        void stop();

    private:
        std::thread mThread;
};