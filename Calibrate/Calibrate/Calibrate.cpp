#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <vector>
#include <iostream>

using namespace cv;
using namespace std;

int main(int argc, char** argv) {
    String folderPath = "C:/Users/abzdr/Pictures/Camera Roll";
    vector<String> fileNames;
    glob(folderPath + "/*.jpg", fileNames, false);

    if (fileNames.empty()) {
        cout << "No JPG files found in the specified directory." << endl;
        return -1;
    }

    int currentIndex = 0;
    int numFiles = static_cast<int>(fileNames.size());

    namedWindow("Image Viewer", WINDOW_AUTOSIZE);

    while (true) {
        Mat image = imread(fileNames[currentIndex]);
        if (image.empty()) {
            cerr << "Error reading image: " << fileNames[currentIndex] << endl;
            break;
        }

        // Create a grid for thumbnails
        int gridWidth = image.cols * numFiles;
        int gridHeight = 100;  // Height of the thumbnail grid
        Mat grid = Mat::zeros(gridHeight, gridWidth, image.type());

        // Calculate the size of each thumbnail
        int thumbnailWidth = image.cols;
        int thumbnailHeight = gridHeight;

        // Copy thumbnails of all images to the grid
        for (int i = 0; i < numFiles; i++) {
            Mat thumbnail = imread(fileNames[i]);
            if (!thumbnail.empty()) {
                Rect roi(i * thumbnailWidth, 0, thumbnailWidth, thumbnailHeight);
                thumbnail.copyTo(grid(roi));
            }
        }

        // Create a canvas for the combined image
        Mat combinedImage(image.rows + gridHeight, max(image.cols, grid.cols), image.type(), Scalar(255, 255, 255));

        // Copy the loaded image to the top of the canvas
        Rect roiImage(0, 0, image.cols, image.rows);
        image.copyTo(combinedImage(roiImage));

        // Copy the grid to the bottom of the canvas
        Rect roiGrid(0, image.rows, grid.cols, gridHeight);
        grid.copyTo(combinedImage(roiGrid));

        imshow("Image Viewer", combinedImage);

        // Wait for a key event
        int key = waitKey(0);

        if (key == 27) {  // Press ESC to exit
            break;
        }
        else if (key == 81 || key == 63234) {  // Left arrow key
            currentIndex = (currentIndex - 1 + numFiles) % numFiles;
        }
        else if (key == 83 || key == 63235 || key == 262241) {  // Right arrow key
            currentIndex = (currentIndex + 1) % numFiles;
        }
    }

    destroyWindow("Image Viewer");
    return 0;
}
