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

namespace client
{
    void img_transmit(string path);

    void send_image(int socket, string img);

    cv::Mat receive_image(int socket);

    void display_image(cv::Mat img, string windowName);
}