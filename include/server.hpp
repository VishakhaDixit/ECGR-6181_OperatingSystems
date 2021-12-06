#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fstream>
#include <thread>
#include <unistd.h> 
#include "image_converter.hpp"
#include <opencv4/opencv2/opencv.hpp>

#define PORT 8080
#define IP   "127.0.0.1"
#define IMG_PATH "/home/vdixit2/Downloads"

using namespace std;

class server
{
    public:

        server()  = default;
        ~server() = default;

        virtual void run() = 0;
        void    execute_thread();

    protected:

        virtual void create_thread() = 0;

        void    create_server_sock();
        void    wait_for_client();
        cv::Mat recieve_image();
        cv::Mat process_image(cv::Mat img);
        void    send_image(cv::Mat cvImg);

    private:

        struct sockaddr_in serverAddr, clientAddr;
        int server_fd, client_fd;
        int addrlen;
};