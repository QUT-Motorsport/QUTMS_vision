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

// Blue hsv values

int b_low_h=114;
int b_high_h=125;
int b_low_s=180;
int b_high_s=255;
int b_low_v=17;
int b_high_v=117;

int b_iLastX = -1; 
int b_iLastY = -1;

// Ideas: - use findContours() after thresholding image
// contourArea(), similar to moments calculation

int main()
{

    // Create a VideoCapture object and open the input file
    // If the input is the web camera, pass 0 instead of the video file name.
    // If an external camera is attached pass 1
    
    // Choose to read a video file or read a from webcam
    
    VideoCapture cap("vid.mp4");
    //VideoCapture cap(0); 

    // Create Mat files for storing image information
    Mat imgOriginal, imgHSV, b_imgHSV;
    
    // Create windows to display stuff
    namedWindow("Video Capture", WINDOW_NORMAL);
    namedWindow("Object Detection", WINDOW_NORMAL);
    
    // Create a while loop for processing images which will result in a video

    while((char)waitKey(1)!='q'){
         
        cap.read(imgOriginal);

        // Add sleep below if you need to slow down frame rate, this will give the vision system more time to process data
        //usleep(5000);

        // move captured image into imgOriginal
        cap>>imgOriginal;
        if(imgOriginal.empty())
            break;
        //-- Detect the object based on HSV Range Values
        cvtColor(imgOriginal, imgHSV, COLOR_BGR2HSV);
        cvtColor(imgOriginal, b_imgHSV, COLOR_BGR2HSV);

        //Create a black image with the size as the camera output. This will be used for placing lines and annotations on the screen
        Mat imgLines = Mat::zeros( imgOriginal.size(), CV_8UC3 );

        // Create threshold Mat files to store image data
        Mat imgThresholded, b_imgThresholded;

        // Threshold colour to only display yellow with HSV ranges
        inRange(imgHSV,Scalar(y_low_h,y_low_s,y_low_v), Scalar(y_high_h,y_high_s,y_high_v),imgThresholded);

        //morphological opening (removes small noise objects from the foreground)
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

        double b_dM01 = b_oMoments.m01; //Spatial moment
        double b_dM10 = b_oMoments.m10; //Spatial moment
        double b_dArea = b_oMoments.m00; //Spatial moment

        
        
        // if the area <= 10000, I consider that the there are no object in the image and it's because of the noise, the area is not zero 
        if (dArea > 100000 && b_dArea > 100000) //100000
            {
            
            // Printing statements for debugging
            //cout << b_dArea << endl;
            //cout << dArea << endl;

            //calculate the position of the ball - see https://docs.opencv.org/3.4/d8/d23/classcv_1_1Moments.html#a8b1b4917d1123abc3a3c16b007a7319b
            // for equations

            int y_posX = dM10 / dArea;
            int y_posY = dM01 / dArea; 

            int b_posX = b_dM10 / b_dArea;
            int b_posY = b_dM01 / b_dArea;        
                    
            if (b_posX >= 0 && b_posY >= 0 && y_posX >= 0 && y_posY >= 0)
            {
                //Draw a red line from the previous point to the current point
                line(imgLines, Point(y_posX, y_posY), Point(b_posX, b_posY), Scalar(0,0,255), 2);
                
                // Circle parameters
                int thickness = -1;
                int lineType = 1;
                int x_center = (b_posX + y_posX)/2;
                int y_center = (b_posY + y_posY)/2;

                //Draw small dot indicating centre point between cones
                circle(imgLines,Point(x_center, y_center),5.0,Scalar( 0, 255, 0 ),thickness,lineType);

            }

            //y_iLastX = y_posX;
            //y_iLastY = y_posY;

            }
        
        // Create annotations on screen
        Mat imgTracking;
        imgTracking = imgOriginal + imgLines;

        // Resize images (occasionally too big to fit on computer screen)
        resize(imgThresholded, imgThresholded, cv::Size(900,600));
        resize(b_imgThresholded, b_imgThresholded, cv::Size(900,600));
        resize(imgTracking, imgTracking, cv::Size(900,600));
        resize(imgHSV, imgHSV, cv::Size(900,600));

        // Combine yellow and blue thresholded binary images for visual purposes
        imgThresholded = imgThresholded + b_imgThresholded;
        //-- Show the frames
        //imshow("Video Capture",imgOriginal);
        imshow("Object Detection",imgThresholded);
        //imshow("detection yellow", b_imgThresholded);
        //imshow("hsv img", imgHSV);
        imshow("Original with lines", imgTracking);


    }
    
    // Close everything
    return 0;
    
}