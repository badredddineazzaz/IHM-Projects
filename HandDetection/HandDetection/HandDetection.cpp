#include <opencv2/opencv.hpp>

int main() {
    cv::VideoCapture cap(1); // Ouvre la webcam, 0 pour la première caméra
    if (!cap.isOpened()) {
        std::cerr << "Erreur: Impossible d'ouvrir la caméra." << std::endl;
        return -1;
    }

    cv::CascadeClassifier handCascade;
    if (!handCascade.load("C:/Users/abzdr/Downloads/aGest.xml")) {
        std::cerr << "Erreur: Impossible de charger le fichier de cascade." << std::endl;
        return -1;
    }

    cv::Mat frame;

    while (true) {
        cap >> frame; // Capturer une image depuis la caméra

        std::vector<cv::Rect> hands;
        cv::Mat gray;

        cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
        cv::equalizeHist(gray, gray);

        handCascade.detectMultiScale(gray, hands, 1.1, 5, 0, cv::Size(30, 30));

        for (const auto& hand : hands) {
            cv::Point center(hand.x + hand.width / 2, hand.y + hand.height / 2);
            cv::circle(frame, center, 5, cv::Scalar(0, 0, 255), -1); // Dessine un point rouge

            // Print or display the X and Y coordinates of the red dot
            std::cout << "X: " << center.x << " Y: " << center.y << std::endl;
        }

        cv::imshow("Main avec point rouge", frame);

        if (cv::waitKey(1) == 27) { // Quitte la boucle si la touche 'ESC' est enfoncée
            break;
        }
    }

    cap.release();
    cv::destroyAllWindows();

    return 0;
}
