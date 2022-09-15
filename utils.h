#ifndef JSOL_U
#define JSOL_U

std::vector<std::string> split(std::string str, char Delimiter);
void printImage(const std::string label, cv::Mat image, const int resize);
int get_distance_euclid(cv::Point a, cv::Point b);

class eyeDetectorRoi {
private:
    cv::CascadeClassifier eyeCascade;//detector
    cv::CascadeClassifier faceCascade;

public:
    eyeDetectorRoi(std::string eyeCascadePath, std::string faceCascadePath);
    cv::Mat detect(cv::Mat image);
};

class eyeDirectionFinder {
private:
    cv::CascadeClassifier eyeCascade;
    enum class DIRECTION { fail, ur, u, ul, r, p, l, dr, d, dl, sr, sl };
    const std::string DIRECTIONtoString[12] = { "fail", "ur", "u", "ul", "r", "p", "l", "dr", "d", "dl", "sr", "sl" };//lookup table

    const int DFLEX = 15;
    const int UFLEX = 38;
    const int LRFLEX = 55;
    const int IGNORE_RATIO = 6; 
    const int PAD = 50;
    const int MIN_EYE_RATIO = 7;
    const cv::Scalar MIN_SCALAR_PUPIL = cv::Scalar(10, 10, 10);
    const cv::Scalar MAX_SCALAR_PUPIL= cv::Scalar(25, 25, 25);
    const cv::Scalar MIN_SCALAR_GAZE = cv::Scalar(180, 165, 140);
    const cv::Scalar MAX_SCALAR_GAZE = cv::Scalar(250, 230, 210);

    int adjustDetectedEyes(std::vector<cv::Rect>& eyes, cv::Mat& image);//return -1 on fail, return 0 on success
    cv::Point getCenterPoint(const cv::Mat& roi);//return cv::Point(-1,0) on fail
    int findDirectionByVector(const cv::Point& pupilVector);

public:
    eyeDirectionFinder(std::string eyeCascadePath);
    std::string findDirection(cv::Mat image);
};

#endif