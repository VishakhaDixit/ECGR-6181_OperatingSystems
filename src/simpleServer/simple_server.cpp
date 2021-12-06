#include "simple_server.hpp"

void simpleServer::run()
{
    create_server_sock();
    wait_for_client();

    cv::Mat serverImg = recieve_image();
    cv::Mat greyImg   = process_image(serverImg);
    send_image(greyImg);
}

void simpleServer::create_thread()
{}