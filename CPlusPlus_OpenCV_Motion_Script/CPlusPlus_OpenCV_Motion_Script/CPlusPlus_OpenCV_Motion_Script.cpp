// Setup tutorial from:
// https://www.opencv-srf.com/2017/11/install-opencv-with-visual-studio.html
// Code base from:
// https://gist.github.com/six519/6d2beee53038ebe8abd98063abfdad86

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/video/tracking.hpp>
#include <opencv2/core/ocl.hpp>
#include <iostream>
#include <windows.h>
//#include <unistd.h> // some projects may need to add this include, I did not need to


using namespace cv;
using namespace std;

int main()
{
    Mat gray1, gray2, frameDelta, threshImage, frame0, frame1, frame2;
    vector<vector<Point> > contours;
    VideoCapture camera(0); // Open camera

    // Set video size to 512x288
    camera.set(3, 512);
    camera.set(4, 288);

    Sleep(3);

    // Capture the first image for frame 1 and frame 2
    camera.read(frame1);
    camera.read(frame2);

    while (true)
    {
        camera.read(frame1);

        Sleep(3); // Give a slight delay so frame 1 and frame 2 can be different

        camera.read(frame2);

        // Convert frame 1 to grayscale
        cvtColor(frame1, gray1, COLOR_BGR2GRAY); // Converts frame 1 to "gray2"
        GaussianBlur(gray1, gray1, Size(21, 21), 0);

        // Convert frame 2 to grayscale
        cvtColor(frame2, gray2, COLOR_BGR2GRAY); // Converts frame 2 to "gray2"
        GaussianBlur(gray2, gray2, Size(21, 21), 0);

        // Compute difference between frame 1 and frame 2
        absdiff(gray1, gray2, frameDelta); // Compares gray 1 to gray 2
        threshold(frameDelta, threshImage, 25, 255, THRESH_BINARY);

        dilate(threshImage, threshImage, Mat(), Point(-1, -1), 2);
        findContours(threshImage, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

        for (int i = 0; i < contours.size(); i++) {
            if (contourArea(contours[i]) < 500) {
                continue;
            }

            putText(frame2, "Motion Detected", Point(10, 20), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(0, 0, 255), 2);
            cout << "Motion Detected" << ".\n";
        }

        //imshow("previous Camera", frame1); // uncomment to display frame 1
        imshow("Main Camera", frame2); // display frame 2

        //imshow("previous gray", gray1); // uncomment to display grayscale frame 1
        //imshow("Main gray", gray2); // uncomment to display grayscale frame 2

        //imshow("Delta", frameDelta); // uncomment to display the image comparison

        if (waitKey(1) == 27) {
            //exit if ESC is pressed
            break;
        }

    }

    return 0;
}
