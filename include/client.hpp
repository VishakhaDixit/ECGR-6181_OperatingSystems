#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fstream>
#include <unistd.h> 
#include <opencv4/opencv2/opencv.hpp>

#define IP   "127.0.0.1"

using namespace std;

namespace client
{
    void convert_image(string path, uint8_t filter_choice, int port);

    void send_image(int socket, string img, uint8_t filter_choice);

    cv::Mat receive_image(int socket);

    void display_image(cv::Mat img, string windowName);
}