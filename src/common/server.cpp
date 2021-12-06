#include <server.hpp>
#include "image_converter.hpp"
#include "base64.h"

cv::Mat server::recieve_image(int client_fd)
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

cv::Mat server::process_image(cv::Mat img)
{
    cv::Mat greyImg = convertors::convert_to_grayscale(img);
    return greyImg;
}

void server::send_image(int client_fd, cv::Mat cvImg) 
{
    std::vector<unsigned char> buf;
    cv::imencode(".png", cvImg, buf);
    auto base64_png = reinterpret_cast<const unsigned char*>(buf.data());
    std::string encode_png = base64_encode(base64_png, buf.size());

    std::string message = std::to_string(encode_png.length()) + "\n" + encode_png;
    send(client_fd, message.data(), message.length(), 0);

    cout << "Server:Image was sent" << std::endl;
    close(client_fd);
}
