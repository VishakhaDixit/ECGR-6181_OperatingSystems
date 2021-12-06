#include <server.hpp>
#include "image_converter.hpp"

int main()
{
    server simpleServer;
    simpleServer.run();
    cv::Mat serverImg = simpleServer.recieve_image();
    cv::Mat greyImg = simpleServer.process_image(serverImg);
    simpleServer.send_image(greyImg);
    //cv::imwrite("server_greyImage.png", greyImg);
}