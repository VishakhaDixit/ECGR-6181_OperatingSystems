/**************************
 *
 * @file    client.cpp
 *
 * @brief   
 *
 * @date    Nov 29, 2021
 *
 * @author  Vishakha Dixit
 *
 **************************/

#include "client.hpp"
#include "base64.h"

namespace client
{
    void get_grayscale(std::string path)
    {
        int server = 0;
        struct sockaddr_in serverAddr;

        server = socket(AF_INET, SOCK_STREAM, 0);

        if(server < 0)
        {
            cout << "\n Socket Creation Failed \n";
            return;
        }

        serverAddr.sin_family = AF_INET;
        serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
        serverAddr.sin_port = htons(PORT);

        // if(inet_pton(AF_INET, IP, &serverAddr.sin_addr) <= 0)
        // {
        //     cout << "Address is not supported \n";
        //     return;
        // }

        if(connect(server, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
        {
            cout << "Connection Failed \n";
            return ;
        }

        send_image(server, path);
        cv::Mat grayImg = receive_image(server);

        // display_image(image);
        // display_image(grayImg);
    }

    void send_image(int socket, string img) 
    {
        cv::Mat cvImg = cv::imread(img);
        std::vector<unsigned char> buf;
        cv::imencode(".png", cvImg, buf);
        auto base64_png = reinterpret_cast<const unsigned char*>(buf.data());
        std::string encode_png = base64_encode(base64_png, buf.size());

        std::string message = std::to_string(encode_png.length()) + "\n" + encode_png;
        send(socket, message.data(), message.length(), 0);

        // int file_size = cvImg.total() * cvImg.elemSize();
        // cout<<"file size: "<<file_size<<std::endl;
        // send(socket, &file_size , sizeof(file_size), 0);
        cout << "Image was sent" << std::endl;
        close(socket);
    }

    cv::Mat receive_image(int socket) 
    {
        int readImgSize;
        char *memBlock;
        ofstream fp;

        memBlock = new char[256];

        read(socket, &readImgSize, sizeof(readImgSize));

        fp.open("rcvImg.png");

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
        }

        cv::Mat rcvImage = imread("rcvImg.png", cv::IMREAD_UNCHANGED);

        return rcvImage;
    }

    void display_image(cv::Mat img, string windowName)
    {
        cv::String window= windowName;    //Name of the window
        cv::namedWindow(window);          // Create a window
        cv::imshow(window, img);          // Show our image inside the created window.
        cv::waitKey(0);
    }
}