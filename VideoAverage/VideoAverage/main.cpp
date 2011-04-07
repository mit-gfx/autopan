#include <iostream>
#include <opencv2/opencv.hpp>

int main (int argc, const char * argv[])
{
    // Some arbitrary width/height for testing.
    int width = 720, height = 480;

    // Create an image.
    // IPL_DEPTH_32F tells it to use 32-bit floats for the image, so we don't run out of precision
    IplImage* image = cvCreateImage(cvSize(width, height), IPL_DEPTH_32F, 3);
    
    // insert code here...
    // Delete the above as necessary
    std::cout << "Hello, World!\n";
    std::cout << "Image size: " << image->width << "x" << image->height << "\n";
    return 0;
}
