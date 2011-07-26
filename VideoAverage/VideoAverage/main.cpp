#include <opencv2/opencv.hpp>
#include <vector>

using std::vector;
using namespace cv;

bool record = false;
double alpha = 1.0;
double beta = 0.0;
Mat dst;

int main(int, char**)
{
    vector<Mat> frames;
    
    VideoCapture cap(0); 
    if(!cap.isOpened())  
    return -1;      //test the camera
    
    namedWindow("Video");  //create a window
        
    while(record == false)
    {
         if(waitKey('r') == 82) record =true;    //start the record
    }
    
    while(record == true)
    {
        Mat frame;
        cap.read(frame);
        imshow("Video", frame);
        frames.push_back(frame);
        if(waitKey('s') == 83) record =false;   //end the record
    }
    
    //blending
    Mat prevBlend = Mat::zeros(frames[0].size(), frames[0].type());
    
    for (int f = 0; f < frames.size(); f++) {
        Mat frame = frames[f];
        printf("src1.type: %d, src2.type: %d; src1.depth: %d\n", frame.type(), prevBlend.type(), frame.depth());  //check
        addWeighted( frame, alpha, prevBlend, beta, 0.0, dst);
        prevBlend = dst;
           
        alpha = 1.0 / (f + 2);
        beta = 1.0 - alpha;
    }

    imwrite("/Users/yanling/Documents/autopan/blend.png", dst);    //save blended image
    
    imshow( "Video", dst);  //show blended image
    
    return 0;
}
