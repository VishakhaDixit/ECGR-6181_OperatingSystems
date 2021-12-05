#include <server.hpp>

cv::Mat server::recieve_image()
{
    ofstream fp;
    char *memBlock;
    memBlock = new char[256];

    // if we get a new connection request
    new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
    if (new_socket == -1)
    { 
        perror("accept"); 
        exit(EXIT_FAILURE); 
    }
    int readImgSize;
    read(new_socket, &readImgSize, sizeof(readImgSize));

    fp.open("server_rcvImg.png");

    if(fp.is_open())
    {
        while(readImgSize >= 256)
        {
            read(socket, memBlock, 256);
            fp.write(memBlock, 256);

            readImgSize -= 256;   
        }

        read(socket, memBlock, readImgSize);
        fp.write(memBlock, readImgSize);
        fp.close();
        //@TODO: Close socket connection in both client and server.
    }

    cv::Mat rcvImage = imread("server_rcvImg.png", cv::IMREAD_UNCHANGED);

    return rcvImage;
}

void server::run()
{
    std::cout << "MainServer: Main server started" << std::endl; 
    int opt = 1; 
    int addrlen = sizeof(address); 

    // Creating socket file descriptor 
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
    { 
        perror("socket failed"); 
        exit(EXIT_FAILURE); 
    } 
    
    // Forcefully attaching socket to the port 8080 
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 
                                                &opt, sizeof(opt))) 
    { 
        perror("setsockopt"); 
        exit(EXIT_FAILURE); 
    } 

    address.sin_family = AF_INET; 
    address.sin_addr.s_addr = INADDR_ANY; 
    address.sin_port = htons( PORT ); 
    
    // Forcefully attaching socket to the port 8080 
    if (bind(server_fd, (struct sockaddr *)&address,  
                                sizeof(address))<0) 
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

    std::cout << "MainServer: Socket server setup. Waiting for connections" << std::endl;

}