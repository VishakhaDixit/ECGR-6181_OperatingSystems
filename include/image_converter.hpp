
#include <opencv2/core/mat.hpp>

namespace convertors
{
    /// Converts an image from color to grayscale
    cv::Mat convert_to_grayscale(cv::Mat in);
    cv::Mat convert_to_negative(cv::Mat in);
    cv::Mat gaussian_blur(cv::Mat in);
    cv::Mat flip90_image(cv::Mat in);
}