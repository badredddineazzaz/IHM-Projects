#include <opencv2/opencv.hpp>
#include <windows.h>

using namespace std;
using namespace cv;

int main() {
    VideoCapture cap(1); // Change the camera index as needed
    if (!cap.isOpened()) {
        cerr << "Camera problem" << endl;
        return -1;
    }

    Mat frame;

    namedWindow("Main", WINDOW_NORMAL);

    bool isObjectHeld = false;
    int holdStartTime = 0;
    const int holdThreshold = 2000; // 2 seconds in milliseconds
    bool rightClickPerformed = false; // Flag to track if the right-click action was performed

    const int smoothingWindowSize = 10;
    vector<Point> cursorPositions;
    cursorPositions.reserve(smoothingWindowSize);

    while (true) {
        cap >> frame;
        /********************Detection du rouge***************************/

        Mat hsvFrame;
        cvtColor(frame, hsvFrame, COLOR_BGR2HSV);

        Scalar lowerRed(160, 50, 50);
        Scalar upperRed(180, 255, 255);

        Mat redMask;
        inRange(hsvFrame, lowerRed, upperRed, redMask);

        vector<vector<Point>> contours;
        findContours(redMask, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

        Point smoothedCursorPosition(0, 0);
        /********************End Detection du rouge***************************/


        for (const auto& contour : contours) {
            if (contourArea(contour) > 1000) {
                Moments moments = cv::moments(contour);
                Point center(moments.m10 / moments.m00, moments.m01 / moments.m00);

                cursorPositions.push_back(center);
                /********************Smoothing***************************/
                if (cursorPositions.size() > smoothingWindowSize) {
                    cursorPositions.erase(cursorPositions.begin());
                }

                for (const auto& position : cursorPositions) {
                    smoothedCursorPosition += position;
                }
                smoothedCursorPosition.x /= cursorPositions.size();
                smoothedCursorPosition.y /= cursorPositions.size();

                SetCursorPos(GetSystemMetrics(SM_CXSCREEN) - GetSystemMetrics(SM_CXSCREEN) * smoothedCursorPosition.x / frame.cols * 0.5,
                    GetSystemMetrics(SM_CYSCREEN) * smoothedCursorPosition.y / frame.rows * 0.5);

                circle(frame, smoothedCursorPosition, 10, Scalar(0, 0, 255), -1);
                /********************End Smoothing***************************/

                /********************Double Click***************************/
                // Check for object hold
                if (!isObjectHeld) {
                    holdStartTime = cv::getTickCount();
                    isObjectHeld = true;
                }
                else {
                    double elapsedTime = (cv::getTickCount() - holdStartTime) / cv::getTickFrequency() * 1000; // in milliseconds

                    if (elapsedTime >= holdThreshold && !rightClickPerformed) {
                        // Perform a right-click action
                        mouse_event(MOUSEEVENTF_RIGHTDOWN, 0, 0, 0, 0);
                        mouse_event(MOUSEEVENTF_RIGHTUP, 0, 0, 0, 0);
                        rightClickPerformed = true;
                    }
                }

                /********************End Double Click***************************/

                // Break out of the loop after processing the first valid contour
                break;
            }
        }

        imshow("Main", frame);

        if (waitKey(1) == 27) {
            break;
        }
    }

    cap.release();
    destroyAllWindows();

    return 0;
}
