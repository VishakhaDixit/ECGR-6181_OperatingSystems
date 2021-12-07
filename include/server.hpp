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

    protected:

        virtual void create_server_sock(int port) = 0;
        virtual int  wait_for_client() = 0;
        pair<cv::Mat, uint8_t> recieve_image(int client_fd);
        cv::Mat process_image(cv::Mat img, uint8_t filterCh);
        void    send_image(int client_fd, cv::Mat cvImg);
};