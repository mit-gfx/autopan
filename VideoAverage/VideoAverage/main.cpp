#include <opencv2/opencv.hpp>
#include <vector>

using std::vector;
using namespace cv;

bool record = false;

int main(int, char**)
{
    vector<Mat> frames;
    
    VideoCapture cap(0); 
    if(!cap.isOpened())  
    return -1;
    
    Mat edges;
    namedWindow("video",1);
        
    while(record == false)
    {
         if(waitKey('r') >= 0) record =true;
    }
    
    while(record == true)
    {
        Mat frame;
        cap.read(frame);
        imshow("video", frame);
        frames.push_back(frame);
        
        if(waitKey('s') >=0 ) record =false;
    }
    
    for (int f = 0; f < frames.size(); f++) {
        Mat frame = frames[f];
        // blend together, analyze, etc., etc.
    }
  
    return 0;
}
