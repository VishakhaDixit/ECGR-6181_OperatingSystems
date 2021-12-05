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

namespace client
{
    void img_transmit(std::string path)
    {
        int serverSocket = 0;
        struct sockaddr_in serverAddr;

        serverSocket = socket(AF_INET, SOCK_STREAM, 0);

        if(serverSocket < 0)
        {
            cout << "\n Socket Creation Failed \n";
            return;
        }

        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(PORT);

        if(inet_pton(AF_INET, IP, &serverAddr.sin_addr) <= 0)
        {
            cout << "Address is not supported \n";
            return;
        }

        if(connect(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
        {
            cout << "Connection Failed \n";
            return;
        }

        send_image(serverSocket, IMG_PATH);
        cv::Mat grayImg = receive_image(serverSocket);

        // display_image(image);
        // display_image(grayImg);
    }

    void send_image(int socket, string img) 
    {
        //open file
        ifstream fp;
        char *memBlock;
        uint8_t remCapacity;
        
        memBlock = new char[256];

        fp.open(img);

        if(fp.is_open())
        {
            //calculate size
            fp.seekg(0, ios_base::end);
            int file_size = fp.tellg();
            
            //send size
            send(socket, &file_size, sizeof(file_size), 0);

            remCapacity = file_size;

            fp.seekg(0, ios::beg);

            while(remCapacity >= 256)
            {
                fp.read(memBlock, 256);

                //send in 256 bytes chunks
                send(socket, memBlock, 256, 0);

                remCapacity -= 256;
            }

            //send remaining bytes of image 
            fp.read(memBlock, remCapacity);
            send(socket, memBlock, remCapacity, 0);

            fp.close();
        }
        
        // debugging symbols
        cout << "Image was sent" << std::endl;
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