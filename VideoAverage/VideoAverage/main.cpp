#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;


int main (int argc, const char * argv[])
{
    double alpha = 0.5 ; double beta = 0.0; double input;
    
    Mat src1, src2, dst;
    // argv[1] is the path to the first image
    // argv[2] is the path to the second image
    std::cout<<"Simple Linear Blender"<<std::endl;
    std::cout<<"* Enter alpha [0-1]: ";
    std::cin>>input;
    
    if( input >= 0 && input <= 1 )
    { alpha = input; }
    
    src1 = imread( "/Users/yanling/Documents/autopan/cat.jpg");
    src2 = imread( "/Users/yanling/Documents/autopan/dog.jpg");
    
    if( !src1.data ) { printf("Error loading src1 \n"); return -1; }
    if( !src2.data ) { printf("Error loading src2 \n"); return -1; }
    
    namedWindow("Linear Blend", 1);
    
    beta = ( 1.0 - alpha );
    addWeighted( src1, alpha, src2, beta, 0.0, dst);
    
    imwrite("/Users/yanling/Documents/autopan/blend.png", dst);
    
    imshow( "Linear Blend", dst );
    
    waitKey(0);
    return 0;
}
    

