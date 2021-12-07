#include "image_converter.hpp"

#include <thread>
#include <iostream>
#include <chrono>

#include <opencv2/opencv.hpp>

namespace convertors
{
    cv::Mat convert_to_grayscale(cv::Mat in) 
    {
        // create an empty image of the same size and
        // type
        cv::Mat ret{ 
            in.rows,
            in.cols,
            in.type()
        };

        // convert the color to grayscale
        cv::cvtColor(in, ret, cv::COLOR_RGB2GRAY);

        return ret;
    }

    cv::Mat convert_to_negative(cv::Mat in)
    {
        cv::Mat grey = convert_to_grayscale(in);
        return 255 - grey;
    }

    cv::Mat gaussian_blur(cv::Mat in)
    {
        cv::Mat ret
        {
            in.rows,
            in.cols,
            in.type()
        };
        cv::GaussianBlur(in, ret, cv::Size(9,9), 0);
        return ret;
    }

    cv::Mat flip90_image(cv::Mat in)
    {
        cv::Mat ret;
        cv::transpose(in, ret);
        cv::flip(ret,ret,1);

        return ret;
    }

    cv::Mat select_filter(cv::Mat in, uint8_t filter)
    {
        cv::Mat ret;

        switch (filter)
        {
            case 1:
                ret = convert_to_grayscale(in);
                return ret;
            case 2:
                ret = convert_to_negative(in);
                return ret;
            case 3:
                ret = gaussian_blur(in);
                return ret;
            case 4:
                ret = flip90_image(in);
                return ret;
            default:
                return ret;
        }
    }
}