#include <server.hpp>
#include "image_converter.hpp"
#include "base64.h"

cv::Mat server::recieve_image()
{
    // master buffer for the entire image
    std::string imageBuffer{ };
    // buffer to store the length of the message
    std::string imgLength{  };

    // reserve data plus some extra so we prevent unnecessary reallocations 
    imgLength.reserve(128 + 16);
    imageBuffer.reserve(128 + 16);

    bool lengthFound = false;
    do {
        // raw buffer for the recv function
        std::array<char, 128> buffer{ };
        
        size_t numread = read(client_fd, buffer.data(), buffer.size());

        // go through each byte that we read and add it to the lengthToReceiveStr
        // if we find a \n line character than we know we have received the length
        // data, if so then copy the rest of the buffer to the master buffer
        // now lengthToReceiveStr has the length of the image and master_buffer
        // has some of the image data
        for (size_t i = 0; i < numread; ++i) {
            imgLength.push_back(buffer[i]);

            if (buffer[i] == '\n') {
                std::move(buffer.begin() + i + 1, buffer.begin() + numread, std::back_inserter(imageBuffer));
                lengthFound = true;
                break;
            }
        }
    } while (!lengthFound);

    // parse the string length to an int
    size_t totalImgLength = std::stoll(imgLength);
    std::cout << "NETWRK: Image with len: " << totalImgLength << " receiving" << std::endl;

    // reserve more data so we dont do unnecessary reallocs
    imageBuffer.reserve(totalImgLength + 32);

    // if we haven't received the full image, keep receiving more
    // data until our master_buffer has the full image
    while (imageBuffer.length() < totalImgLength) {
        std::array<char, 1024 * 64> buffer{ };

        size_t numread = read(client_fd, buffer.data(), buffer.size());
        
        std::move(buffer.begin(), buffer.begin() + numread, std::back_inserter(imageBuffer));
    }

    // decode the base64 string that we received to the actual image data 
    std::string decoded_img = base64_decode(imageBuffer);
    std::vector<unsigned char> raw_img{ decoded_img.begin(), decoded_img.end() };
    cv::Mat image = cv::imdecode(raw_img, cv::IMREAD_COLOR);

    std::cout << "NETWRK: Received image" << std::endl;

    // return the decoded image
    return image;
}

void server::run()
{
    std::cout << "MainServer: Main server started" << std::endl; 
    // addrlen = sizeof(serverAddr); 

    // Creating socket file descriptor 
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
    { 
        perror("socket failed"); 
        exit(EXIT_FAILURE); 
    } 
    
    // // Forcefully attaching socket to the port 8080 
    // if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 
    //                                             &opt, sizeof(opt))) 
    // { 
    //     perror("setsockopt"); 
    //     exit(EXIT_FAILURE); 
    // } 

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

    std::cout << "MainServer: Socket server setup. Waiting for connections" << std::endl; 

    // if we get a new connection request
    int clientAddrSize = sizeof(clientAddr);
    if((client_fd = accept(server_fd, (struct sockaddr *)&clientAddr, (socklen_t *)&clientAddrSize)))
    {
        cout << "Client Connected!" << std::endl;
    }

}