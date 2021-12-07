
#include <opencv2/core/mat.hpp>

namespace convertors
{
    cv::Mat convert_to_grayscale(cv::Mat in);
    cv::Mat convert_to_negative(cv::Mat in);
    cv::Mat gaussian_blur(cv::Mat in);
    cv::Mat flip90_image(cv::Mat in);

    cv::Mat select_filter(cv::Mat in, uint8_t filter);
}