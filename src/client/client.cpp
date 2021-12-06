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

        if(connect(server, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
        {
            cout << "Connection Failed \n";
            return ;
        }

        send_image(server, path);
        cv::Mat grayImg = receive_image(server);

        cv::imwrite("client_grayscale.png", grayImg);
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

        cout << "Image was sent" << std::endl;
    }

    cv::Mat receive_image(int socket) 
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
            
            size_t numread = read(socket, buffer.data(), buffer.size());

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

            size_t numread = read(socket, buffer.data(), buffer.size());
            
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

    void display_image(cv::Mat img, string windowName)
    {
        cv::String window= windowName;    //Name of the window
        cv::namedWindow(window);          // Create a window
        cv::imshow(window, img);          // Show our image inside the created window.
        cv::waitKey(0);
    }
}