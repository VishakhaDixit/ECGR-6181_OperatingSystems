#include "simple_server.hpp"

void simpleServer::run()
{
    cv::Mat serverImg;
    cv::Mat convertedImg;
    std::pair<cv::Mat, uint8_t> p;

    create_server_sock();
    int clientSock = wait_for_client();

    if(clientSock != -1)
    {
        p = recieve_image(clientSock);

        convertedImg = process_image(p.first, p.second - 48);
        send_image(clientSock, convertedImg);
    }
}

void simpleServer::create_server_sock()
{
    std::cout << "MainServer: Main server started" << std::endl;
    // Creating socket file descriptor 
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
    { 
        perror("socket failed"); 
        exit(EXIT_FAILURE); 
    }

    serverAddr.sin_family = AF_INET; 
    serverAddr.sin_addr.s_addr = INADDR_ANY; 
    serverAddr.sin_port = htons( PORT ); 

    // Forcefully attaching socket to the port 8080 
    if (bind(server_fd, (struct sockaddr *)&serverAddr,  
                                sizeof(serverAddr))<0) 
    { 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    }

    // Listen to on the addr and port
    if (listen(server_fd, 3) < 0) 
    { 
        perror("listen"); 
        exit(EXIT_FAILURE); 
    }
}

int simpleServer::wait_for_client()
{
    std::cout << "MainServer: Socket server setup. Waiting for connections" << std::endl; 

    // if we get a new connection request
    int clientAddrSize = sizeof(clientAddr);
    if((client_fd = accept(server_fd, (struct sockaddr *)&clientAddr, (socklen_t *)&clientAddrSize)))
    {
        cout << "Client Connected!" << std::endl;
        return client_fd;
    }
    else
    {
        cout << "Client Connection invalid" << std::endl;
        return -1;
    }
}