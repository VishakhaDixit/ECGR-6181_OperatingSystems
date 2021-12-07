#include <client.hpp>

int main() 
{
    uint8_t filter_choice;

    cout << std::endl << "1. Convert Image to Grayscale";
    cout << std::endl << "2. Convert Image Negative";
    cout << std::endl << "3. Apply 9x9 Gaussian Blur";
    cout << std::endl << "4. Flip Image by 90 Degree";
    cout << std::endl << "Enter your choice: ";
    
    cin >> filter_choice;

    client::convert_image("../test/Lenna.png", filter_choice);
}