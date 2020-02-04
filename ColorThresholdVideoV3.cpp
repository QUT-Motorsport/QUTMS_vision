#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>
#include <stdlib.h>
using namespace std;
using namespace cv;

#include <unistd.h>

unsigned int microseconds;



// Yellow cone hsv values

int y_low_h=8;
int y_high_h=38;
int y_low_s=176;
int y_high_s=255;
int y_low_v=108;
int y_high_v=155;

int y_iLastX = -1; 
int y_iLastY = -1;

int b_low_h=114;
int b_high_h=125;
int b_low_s=180;
int b_high_s=255;
int b_low_v=17;
int b_high_v=117;

int b_iLastX = -1; 
int b_iLastY = -1;



int main()
{

      // Create a VideoCapture object and open the input file
    // If the input is the web camera, pass 0 instead of the video file name
    
    // Choose to read a video file or read a from webcam
    
    VideoCapture cap("vid.mp4");
    //VideoCapture cap(0); 

    Mat imgOriginal, imgHSV, b_imgHSV;
    
    namedWindow("Video Capture", WINDOW_NORMAL);
    namedWindow("Object Detection", WINDOW_NORMAL);
    
    
    //Create a black image with the size as the camera output
    //Mat imgLines = Mat::zeros( imgOriginal.size(), CV_8UC3 );
    
    while((char)waitKey(1)!='q'){
         
        cap.read(imgOriginal);

        //usleep(5000);

        cap>>imgOriginal;
        if(imgOriginal.empty())
            break;
        //-- Detect the object based on RGB Range Values
        cvtColor(imgOriginal, imgHSV, COLOR_BGR2HSV);
        cvtColor(imgOriginal, b_imgHSV, COLOR_BGR2HSV);

        

        Mat imgLines = Mat::zeros( imgOriginal.size(), CV_8UC3 );

        Mat imgThresholded, b_imgThresholded;

        inRange(imgHSV,Scalar(y_low_h,y_low_s,y_low_v), Scalar(y_high_h,y_high_s,y_high_v),imgThresholded);

        //morphological opening (removes small objects from the foreground)
        erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
        dilate( imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) ); 

        //morphological closing (removes small holes from the foreground)
        dilate( imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) ); 
        erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
        
        //Calculate the moments of the thresholded image
        Moments oMoments = moments(imgThresholded);

        double dM01 = oMoments.m01;
        double dM10 = oMoments.m10;
        double dArea = oMoments.m00;


        // BLUE
        inRange(b_imgHSV,Scalar(b_low_h,b_low_s,b_low_v), Scalar(b_high_h, b_high_s, b_high_v), b_imgThresholded);
        //morphological opening (removes small objects from the foreground)
        erode(b_imgThresholded, b_imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
        dilate( b_imgThresholded, b_imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) ); 

        //morphological closing (removes small holes from the foreground)
        dilate( b_imgThresholded, b_imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) ); 
        erode(b_imgThresholded, b_imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
        
        //Calculate the moments of the thresholded image
        Moments b_oMoments = moments(b_imgThresholded);

        double b_dM01 = b_oMoments.m01;
        double b_dM10 = b_oMoments.m10;
        double b_dArea = b_oMoments.m00;   

        
        
        // if the area <= 10000, I consider that the there are no object in the image and it's because of the noise, the area is not zero 
        if (dArea > 100000 && b_dArea > 100000) //100000
            {
            
            //cout << b_dArea << endl;
            //cout << dArea << endl;
            //calculate the position of the ball
            int y_posX = dM10 / dArea;
            int y_posY = dM01 / dArea; 

            int b_posX = b_dM10 / b_dArea;
            int b_posY = b_dM01 / b_dArea;        
                    
            if (b_posX >= 0 && b_posY >= 0 && y_posX >= 0 && y_posY >= 0)
            {
                //Draw a red line from the previous point to the current point
                line(imgLines, Point(y_posX, y_posY), Point(b_posX, b_posY), Scalar(0,0,255), 2);
                
                int thickness = -1;
                int lineType = 1;
                int x_center = (b_posX + y_posX)/2;
                int y_center = (b_posY + y_posY)/2;

                circle(imgLines,Point(x_center, y_center),5.0,Scalar( 0, 255, 0 ),thickness,lineType);

            }

            //y_iLastX = y_posX;
            //y_iLastY = y_posY;

            }

        


        
        Mat imgTracking;
        imgTracking = imgOriginal + imgLines;

        resize(imgThresholded, imgThresholded, cv::Size(900,600));
        resize(b_imgThresholded, b_imgThresholded, cv::Size(900,600));
        resize(imgTracking, imgTracking, cv::Size(900,600));
        resize(imgHSV, imgHSV, cv::Size(900,600));

        imgThresholded = imgThresholded + b_imgThresholded;
        //-- Show the frames
        //imshow("Video Capture",imgOriginal);
        imshow("Object Detection",imgThresholded);
        //imshow("detection yellow", b_imgThresholded);
        //imshow("hsv img", imgHSV);
        imshow("Original with lines", imgTracking);


    }
    

    return 0;
    
}