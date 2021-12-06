#include <server.hpp>

int main()
{
    server simpleServer;
    simpleServer.run();
    cv::Mat serverImg = simpleServer.recieve_image();
    cv::imwrite("serverImage.png", serverImg);
}