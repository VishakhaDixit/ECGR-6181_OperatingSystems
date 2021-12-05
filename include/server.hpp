#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fstream>
#include <unistd.h> 
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
        void run();
        cv::Mat recieve_image();
        void process_image();
        void send_image();

    private:
        struct sockaddr_in address;
        int server_fd, new_socket;
        int addrlen;

};