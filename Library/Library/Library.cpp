#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <windows.h>
#include <thread>

using namespace cv;
using namespace std;

int mouseX = 1;
Mat concatenatedImage;

void onMouse(int event, int x, int y, int flags, void* userdata) {
    static auto start_time = chrono::steady_clock::now();

    if (event == EVENT_MOUSEMOVE) {
        auto current_time = chrono::steady_clock::now();
        auto elapsed_time = chrono::duration_cast<chrono::seconds>(current_time - start_time);

        if (elapsed_time.count() >= 2 && y > 500) {
            mouseX = x;
            start_time = current_time;
        }
    }
}

void webcamThread() {
    VideoCapture cap(1); // Change the camera index as needed
    if (!cap.isOpened()) {
        cerr << "Camera problem" << endl;
        return;
    }

    namedWindow("Main", WINDOW_NORMAL);

    bool isObjectHeld = false;
    int holdStartTime = 0;
    const int holdThreshold = 2000; // 2 seconds in milliseconds
    bool rightClickPerformed = false;

    const int smoothingWindowSize = 10;
    vector<Point> cursorPositions;
    cursorPositions.reserve(smoothingWindowSize);
    bool validRedContour = false; // Flag to track valid red contour

    while (true) {
        Mat frame;
        cap >> frame;

        /********************Detection du rouge***************************/
        Mat hsvFrame;
        cvtColor(frame, hsvFrame, COLOR_BGR2HSV);

        Scalar lowerRed(90 - 10, 70, 50);
        Scalar upperRed(90 + 10, 255, 255);

        Mat redMask;
        inRange(hsvFrame, cv::Scalar(0, 100, 100), cv::Scalar(10, 255, 255), redMask);
        inRange(hsvFrame, cv::Scalar(160, 100, 100), cv::Scalar(179, 255, 255), redMask);

        vector<vector<Point>> contours;
        findContours(redMask, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

        Point smoothedCursorPosition(0, 0);
        /********************End Detection du rouge***************************/

        validRedContour = false; // Reset the flag

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

                validRedContour = true; // Set the flag if there's a valid red contour
            }
        }

        if (validRedContour) {
            // Draw the red dot only if there's a valid red contour
            SetCursorPos(GetSystemMetrics(SM_CXSCREEN) - GetSystemMetrics(SM_CXSCREEN) * smoothedCursorPosition.x / frame.cols * 2,
                GetSystemMetrics(SM_CYSCREEN) * smoothedCursorPosition.y / frame.rows * 2);

            circle(frame, smoothedCursorPosition, 10, Scalar(0, 0, 255), -1);
        }

        imshow("Main", frame);
        int key = waitKey(1);

        if (key == 27) // Press "Esc" to exit
            break;
    }
}

void imageConcatenationThread() {
    string folderPath = "C:/Users/abzdr/Downloads/";
    vector<string> imageFiles;
    glob(folderPath + "*.jpg", imageFiles);

    if (imageFiles.empty()) {
        cerr << "No images found in the folder." << endl;
        return;
    }

    int targetWidth = 1000 / imageFiles.size();
    int targetHeight = 100;

    vector<Mat> resizedImages;

    for (const string& filename : imageFiles) {
        Mat img = imread(filename);
        if (!img.empty()) {
            // Preserve aspect ratio and use bilinear interpolation
            double aspectRatio = static_cast<double>(targetHeight) / img.rows;
            resize(img, img, Size(0, 0), aspectRatio, aspectRatio, INTER_LINEAR);
            resizedImages.push_back(img);
        }
        else {
            cerr << "Error reading image: " << filename << endl;
        }
    }

    if (resizedImages.empty()) {
        cerr << "No valid images to concatenate." << endl;
        return;
    }

    hconcat(resizedImages, concatenatedImage);
    while (true) {
        int selectedImageIndex = mouseX / (1000 / imageFiles.size());
        Mat selectedImage = (selectedImageIndex >= 0 && selectedImageIndex < imageFiles.size()) ? resizedImages[selectedImageIndex] : Mat::zeros(targetHeight, targetWidth, CV_8UC3);

        // Preserve aspect ratio for the selected image
        double aspectRatio = static_cast<double>(500) / selectedImage.rows;
        resize(selectedImage, selectedImage, Size(1000, 500), aspectRatio, aspectRatio, INTER_LINEAR);

        resize(concatenatedImage, concatenatedImage, Size(1000, 100));
        Mat stackedImage;
        vconcat(selectedImage, concatenatedImage, stackedImage);
        imshow("Concatenated Images", stackedImage);
        moveWindow("Concatenated Images", 20, 20);

        setMouseCallback("Concatenated Images", onMouse, nullptr);
        if (waitKey(1) == 27) // Press "Esc" to exit
            break;
    }
}

int main() {
    thread webcam(webcamThread);
    thread imageConcatenation(imageConcatenationThread);

    webcam.join();
    imageConcatenation.join();

    return 0;
}
