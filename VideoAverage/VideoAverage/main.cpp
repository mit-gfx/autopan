#include <opencv2/opencv.hpp>
#include <vector>

using std::vector;
using namespace cv;

bool record = false;                                              //setting variables
bool mouseclick = false;                                         
double alpha;
double beta;

Mat dst;
Mat dst2;
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
{  
    //-----------------choose mode---------------------
    string input;
    std::cout<<"-------------------"<<std::endl;
    std::cout<<"----SELECT MODE----"<<std::endl; 
    std::cout<<"(please enter 'camera' or 'video')"<<std::endl;
    std::cin>>input;
    
    if (input == "camera") {
           
      VideoCapture cap(0); 
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
      }else{
        
      
    }
    //--------------find features to track---------------
    
    Mat firstframe;
    firstframe = frames[0];
    
    Mat trackingImage;
    cvtColor(frames[0], trackingImage, CV_RGB2GRAY);
    goodFeaturesToTrack(trackingImage, points, 300, 0.01, 10);      //find good features to track
    
    std::cout<<"-------------------"<<std::endl;
    std::cout<<"--Features found---"<<std::endl;                    //test if features are found
    
    for (int c = 0; c< points.size(); c++){
        circle(firstframe, points[c], 5, Scalar(1.0,0.0,0.0,1.0));  //display features
    }
    
    //-------------------select--------------------------
    
    namedWindow("Select");                            
    imshow("Select", firstframe);                                   //show the first frame of the video 
    
    setMouseCallback("Select", mousecallback);

    while(mouseclick == false){
        waitKey(0);                                                //press any key to continue...
    }
    destroyWindow("Select");

    //--------------pick 3 best points--------------------
    double disf = 1000, diss = 1000, dist = 1000;
    int fpts, spts, tpts;
    for (int i = 0; i < points.size(); i++) {
        double distance = sqrt((points[i].x-a.x)*(points[i].x-a.x) + (points[i].y-a.y)*(points[i].y-a.y));
        if (disf > distance) {
            disf = distance;
            fpts = i;
        }
    }
    for (int i = 0; i < points.size(); i++) {
        double distance = sqrt((points[i].x-a.x)*(points[i].x-a.x) + (points[i].y-a.y)*(points[i].y-a.y));
        if (diss > distance) {
            diss = distance;
            if (i != fpts) {
                spts = i;
            }
        }
    }
    for (int i = 0; i < points.size(); i++) {
        double distance = sqrt((points[i].x-a.x)*(points[i].x-a.x) + (points[i].y-a.y)*(points[i].y-a.y));
        if (dist > distance) {
            dist = distance;
            if (i != fpts) {
                if (i != spts) {
                    tpts = i;
                }
            }
        }
    }

    printf("first point: %d, second point: %d, third point: %d ", fpts, spts, tpts);
    std::cout<<"------------------"<<std::endl;

    //--------------track features & warp image-----------
    vector<Point2f> prevpts;
    vector<Point2f> nextpts;
    vector<uchar> status;
    vector<float> error;
    vector<Mat> result;//settings
    prevpts = points; 

    Mat transform;
    Mat warpedframe;
    Point2f prevSelectedPts[3], nextSelectedPts[3];

    for (int t = 0; t< frames.size()-1; t++){
        calcOpticalFlowPyrLK(frames[t], frames[t+1], prevpts, nextpts, status, error);  //track feature

        prevSelectedPts[0] = prevpts[fpts]; prevSelectedPts[1] = prevpts[spts]; prevSelectedPts[2] = prevpts[tpts];
        nextSelectedPts[0] = nextpts[fpts]; nextSelectedPts[1] = nextpts[spts]; nextSelectedPts[2] = nextpts[tpts];  //the points need to be improved
        circle(frames[t], nextpts[fpts], 5, Scalar(1.0,0.0,0.0,1.0));
        
        // compute x,y difference between next point 0 and prev point 0
        // construct an affine transformation matrix of that:
        //  1  0  dx
        //  0  1  dy
        
        //transform = getAffineTransform(nextSelectedPts, prevSelectedPts); 
        
        transform = Mat::eye(2, 3, CV_32F);
        transform.at<float>(0, 2) = points[fpts].x-nextpts[fpts].x;
        transform.at<float>(1, 2) = points[fpts].y-nextpts[fpts].y;
        
        cout << "Transform " << t << ":\n" << transform << "\n";
        cout << "prev points: "
             << prevSelectedPts[0] << prevSelectedPts[1] << prevSelectedPts[2] << "\n";
        cout << "next points: "
             << nextSelectedPts[0] << nextSelectedPts[1] << nextSelectedPts[2] << "\n";
        
        warpAffine(frames[t], warpedframe, transform, frames[t].size());                   //warp image
        result.push_back(warpedframe.clone());
        
        prevpts = nextpts; 
        
        
        ///*
        for (int c = 0; c< frames.size()-1; c++){
            char winName[16];
            sprintf(winName, "test %d",t+1);
            namedWindow(winName);
            imshow(winName, warpedframe);                                          
        }//*/   //TEST WARPEDFRAME

        /*for (int c = 0; c< prevpts.size(); c++){
            circle(frames[t+1], prevpts[c], 5, Scalar(1.0,0.0,0.0,1.0)); 
            char winName[16];
            sprintf(winName, "test %d",t+1);
            namedWindow(winName);
            imshow(winName, frames[t+1]);
        }                                                       */  //TEST IF FEATURES ARE TRACKED
     }
        
    waitKey(0);
    
    //-------------------blending------------------------
   
    Mat prevBlend = Mat::zeros(result[0].size(), result[0].type()); //create prevBlend = frames[0] which will be used later in blending
    Mat prevBlend2 = Mat::zeros(frames[0].size(), result[0].type());
    
    //blend the warped frame
    for (int f = 0; f < result.size(); f++) {
        alpha = 1.0 / (f + 1); 
        beta = 1.0 - alpha;                                         //change alpha & beta

        Mat frame = result[f];                                      //get the frame

        /*  PRINT 
        printf("alpha: %f, beta: %f\n", alpha ,beta);
        printf("src1.type: %d, src2.type: %d; src1.depth: %d\n", frame.type(), prevBlend.type(), frame.depth());  //check
        */    //PRINT(just for testing)

        addWeighted( frame, alpha, prevBlend, beta, 0.0, dst);      //blending
        
        prevBlend = dst;                                            //save blended picture

        /*  TEST
        char winName[16];
        sprintf(winName, "Test %d", f);
        namedWindow(winName);    //create new window for testing
        imshow(winName, dst);   //test 
         */    //TEST (just for testing)
    }
    
    //blend the original frame
    for (int f = 0; f < frames.size(); f++) {
        alpha = 1.0 / (f + 1); 
        beta = 1.0 - alpha;                                         //change alpha & beta
        
        Mat frame = frames[f];                                      //get the frame
        
        /*  PRINT 
         printf("alpha: %f, beta: %f\n", alpha ,beta);
         printf("src1.type: %d, src2.type: %d; src1.depth: %d\n", frame.type(), prevBlend.type(), frame.depth());  //check
         */    //PRINT(just for testing)
        
        addWeighted( frame, alpha, prevBlend2, beta, 0.0, dst2);      //blending
        
        prevBlend2 = dst2;                                            //save blended picture
        
        /*  TEST
         char winName[16];
         sprintf(winName, "Test %d", f);
         namedWindow(winName);    //create new window for testing
         imshow(winName, dst);   //test 
         */    //TEST (just for testing)
    }

    std::cout<<"--Finished blending---"<<std::endl;
    
    //------------------display--------------------------
    namedWindow("Blend(result)");                                           //create new window for blended picture
    
    imwrite("/Users/yanling/Documents/autopan/blend.png", dst);     //save blended "RESULT" image
    
    imshow( "Blend(result)", dst);                                          //show blended "RESULT" image
    
    namedWindow("Blend(original)");                                           //create new window for blended picture
    
    imwrite("/Users/yanling/Documents/autopan/blend2.png", dst2);     //save blended "ORIGINAL" image
    
    imshow( "Blend(original)", dst2);                                          //show blended "ORIGINAL" image

    
    if(waitKey(0) == 81) exit(0);
    
    return 0;
}
