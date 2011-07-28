#include <opencv2/opencv.hpp>
#include <vector>

using std::vector;
using namespace cv;

bool record = false;                                              //setting variables
bool mouseclick = false;                                         
double alpha;
double beta;

Mat dst;
Mat firstframe;
vector<Mat> frames;
vector<Point2f> points;
CvPoint a;

void mousecallback(int event, int x, int y, int, void* )          //mouse call back
{
    switch( event ){
        case CV_EVENT_MOUSEMOVE:
            break;
            
		case CV_EVENT_LBUTTONDOWN:                                //save and print point after click
            Point seed = Point(x,y);
            a = Point(x,y);
            printf("x:, %d y: %d",a.x ,a.y);
            std::cout<<"-----"<<std::endl;
            mouseclick = true;
            break;
    }
}

int main(int, char**)
{   VideoCapture cap(0); 
    if(!cap.isOpened())  
    return -1;                                                    //test the camera 
    
    //------------------recording----------------------
    
    namedWindow("Video");                                         //create a window
        
    while(record == false)
    {
         if(waitKey('r') == 82) record =true;                     //start the record
    }
    
    while(record == true)
    {
        Mat frame;
        cap.read(frame);
        imshow("Video", frame);
        frames.push_back(frame.clone());                          //recording (push_back)
        if(waitKey('s') == 83) record =false;                     //finish the record
    }
    destroyWindow("Video");
    
    //-------------------select--------------------------
    
    namedWindow("Select");                            
    imshow("Select", frames[0]);                                   //show the first frame of the video 
    
    setMouseCallback("Select", mousecallback);

    while(mouseclick == false){
        waitKey(0);                                                //press any key to continue...
    }
    destroyWindow("Select");
    
    //--------------find features to track---------------
    
    namedWindow("Display Features");
    firstframe = frames[0];

    Mat trackingImage;
    cvtColor(frames[0], trackingImage, CV_RGB2GRAY);
    goodFeaturesToTrack(trackingImage, points, 100, 0.01, 10);      //find good features to track
    
    std::cout<<"-------------------"<<std::endl;
    std::cout<<"--Features found---"<<std::endl;                    //test if features are found
    
    for (int c = 0; c< points.size(); c++){
        circle(firstframe, points[c], 5, Scalar(1.0,0.0,0.0,1.0));  //display features
    }
    imshow("Display Features", firstframe);
    
    waitKey(0);
    
    //--------------track features-----------------------
    vector<Point2f> prevpts;
    vector<Point2f> nextpts;
    vector<uchar> status;
    vector<float> error;                                           //settings
    
    prevpts = points;
    
    for (int t = 0; t< frames.size()-1; t++){
        calcOpticalFlowPyrLK(frames[t], frames[t+1], prevpts, nextpts, status, error);
        
        prevpts = nextpts;
        
        for (int c = 0; c< prevpts.size(); c++){
            circle(frames[t+1], prevpts[c], 5, Scalar(1.0,0.0,0.0,1.0)); 
            char winName[16];
            sprintf(winName, "test %d",t+1);
            namedWindow(winName);
            imshow(winName, frames[t+1]);
        }                                                         //TESTING
     }
    
    waitKey(0);
    //-------------------blending------------------------
   
    Mat prevBlend = Mat::zeros(frames[0].size(), frames[0].type()); //create prevBlend = frames[0] which will be used later in blending
    
    for (int f = 0; f < frames.size(); f++) {
        alpha = 1.0 / (f + 1); 
        beta = 1.0 - alpha;                                        //change alpha & beta
        
        Mat frame = frames[f];                                     //get the frame
        
        /*  PRINT 
        printf("alpha: %f, beta: %f\n", alpha ,beta);
        printf("src1.type: %d, src2.type: %d; src1.depth: %d\n", frame.type(), prevBlend.type(), frame.depth());  //check
        */                                                      //PRINT(just for testing)
        
        addWeighted( frame, alpha, prevBlend, beta, 0.0, dst);     //blending
        
        prevBlend = dst;                                           //save blended picture
        
        /*  TEST
        char winName[16];
        sprintf(winName, "Test %d", f);
        namedWindow(winName);    //create new window for testing
        imshow(winName, dst);   //test 
         */                                                      //TEST(just for testing)
    }
    std::cout<<"--Finished blending---"<<std::endl;
    
    //------------------display--------------------------
    namedWindow("Blend");                                          //create new window for blended picture
    
    imwrite("/Users/yanling/Documents/autopan/blend.png", dst);    //save blended image
    
    imshow( "Blend", dst);                                         //show blended image
    
    if(waitKey(0) == 81) exit(0);
    
    return 0;
}
