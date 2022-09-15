#include <iostream>
#include <filesystem>
#include <vector>
#include <string>
#include <cmath>
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <sstream>
#include "utils.h"

std::vector<std::string> split(std::string str, char Delimiter) {
    std::istringstream iss(str);             // istringstream�� str�� ��´�.
    std::string buffer;                      // �����ڸ� �������� ����� ���ڿ��� ������� ����

    std::vector<std::string> result;

    // istringstream�� istream�� ��ӹ����Ƿ� getline�� ����� �� �ִ�.
    while (std::getline(iss, buffer, Delimiter)) {
        result.push_back(buffer);       // ����� ���ڿ��� vector�� ����
    }

    return result;
}

void printImage(const std::string label, cv::Mat image, const int resize = 1) {
    if (resize != 1)
        cv::resize(image, image, cv::Size(image.cols / resize, image.rows / resize), 0, 0, 0);
    cv::imshow(label, image);
    cv::waitKey();
    cv::destroyAllWindows();
}

int get_distance_euclid(cv::Point a, cv::Point b) {
    return std::sqrt(std::pow(a.x - b.x, 2) + std::pow(a.y - b.y, 2));
}

eyeDetectorRoi::eyeDetectorRoi(std::string eyeCascadePath, std::string faceCascadePath) {
    std::cout << "Making detector for eye roi...\n";
    eyeCascade.load(eyeCascadePath);
    faceCascade.load(faceCascadePath);
}

cv::Mat eyeDetectorRoi::detect(cv::Mat image) {
    cv::Mat imageGray;
    std::vector<cv::Rect> eyes;//save detected objects
    std::vector<cv::Rect> faces;

    std::vector<cv::Point> centers;//for calculate ROI
    cv::Mat outputImage;

    //preprocessing
    cv::cvtColor(image, imageGray, cv::COLOR_BGR2GRAY);
    cv::equalizeHist(imageGray, imageGray);

    //detecting
    std::cout << "\n\nDetecting eyes...\n";
    eyeCascade.detectMultiScale(imageGray, eyes, 1.01, 3, 0 | cv::CASCADE_SCALE_IMAGE, cv::Size(image.rows / 5, image.rows / 5));
    if (eyes.size() != 2) {
        //retry eye detection on face roi
        std::cout << "Trying to handling exception by face detection...\n";
        faceCascade.detectMultiScale(imageGray, faces, 1.1, 3, 0 | cv::CASCADE_SCALE_IMAGE, cv::Size(image.rows / 3, image.rows / 3));//ignore too small face
        if (faces.size() == 1) {//well detected
            std::cout << "Success for face detection & detecting eyes in face ROI...\n";
            eyeCascade.detectMultiScale(imageGray(faces[0]), eyes, 1.01, 11, 0 | cv::CASCADE_SCALE_IMAGE, cv::Size(faces[0].width / 7, faces[0].width / 7));//min for nose
            for (auto& catched_eye : eyes) {//adjust location of face to eye vector.
                catched_eye.x += faces[0].x;
                catched_eye.y += faces[0].y;
            }
        }
        else {//fail to face detection
            std::cerr << "*****Fail! We cannot detect face properly. we detected " << faces.size() << " faces*****\n";
            return cv::Mat();//������ ���� mat�� ��ȯ
        }

        if (eyes.size() != 2) {//fail to eye detection again (in face roi)
            std::cerr << "*****Fail! We cannot detect eye properly. We detected" << eyes.size() << " eyes*****\n";
            return cv::Mat();
        }
        else {//apply race xy
            std::cout << "Exception is catched!!!\n";
        }
    }

    //calculate roi
    for (int j = 0; j < eyes.size(); j++)//eyes.size() is 2.
        centers.push_back(std::move(cv::Point(eyes[j].x + eyes[j].width / 2, eyes[j].y + eyes[j].height / 2)));
    const auto avg_center = cv::Point((centers[0].x + centers[1].x) / 2, (centers[1].y + centers[1].y) / 2);
    const int d = get_distance_euclid(centers[0], centers[1]);
    const float T = 21;//ROI ���� ���̺� ���

    auto start_point = cv::Point(avg_center.x - 18 * d / T, avg_center.y - 12 * d / T);//use of T
    auto end_point = cv::Point(avg_center.x + 18 * d / T, avg_center.y + 12 * d / T);

    try {
        outputImage = image(cv::Rect(start_point.x, start_point.y, end_point.x - start_point.x, end_point.y - start_point.y));
    }
    catch (std::exception& e) {
        std::cerr << "*****Fail! Eye space in original image is not enough for getting ROI region!*****\n";
        return cv::Mat();
    }

    std::cout << "Success!\n";
    return outputImage;
};

eyeDirectionFinder::eyeDirectionFinder(std::string eyeCascadePath) {
    std::cout << "Making finder for calculate direction of eye...\n";
    eyeCascade.load(eyeCascadePath);
}

int eyeDirectionFinder::adjustDetectedEyes(std::vector<cv::Rect>& eyes, cv::Mat& image) {
    int ltCount = 0;
    int rtCount = 0;
    for (int i = 0; i < eyes.size(); i++) {
        cv::Rect& eye = eyes.at(i);

        //1. ignore y's outerline as much as IGNORE_RATIO
        if (eye.y < image.rows / IGNORE_RATIO || eye.y>image.rows / IGNORE_RATIO * (IGNORE_RATIO - 1)) {
            eyes.erase(eyes.begin() + i);
            i--;//������ ���� shift����
            std::cout << "removed 1 eye by IGNORE_RATIO\n";
            continue;
        }

        //2. erase same roi
        if (eye.x < image.cols / 2) {
            if (rtCount >= 1) {
                eyes.erase(eyes.begin() + i);
                i--;
                std::cout << "removed 1 eye by DUPLICATION\n";
            }
            rtCount++;
        } else {
            if (ltCount >= 1) {
                eyes.erase(eyes.begin() + i);
                i--;
                std::cout << "removed 1 eye by same ROI\n";
            }
            ltCount++;
        }
    
        //3. make roi to 2:3 ratio
        eye.y -= eye.width / 3 - eye.height / 2;
        eye.height += 2 * (eye.width / 3 - eye.height / 2);
    }
    //4. use only one eye
    if (eyes.size() == 1) {
        eyes.push_back(eyes[0]);
    }

    //5. exception (no eyes)
    if (eyes.size() != 2) {
        std::cout << "****FAIL****" << eyes.size() << "\n";
        return -1;
    }
    return 0;
}

cv::Point eyeDirectionFinder::getCenterPoint(const cv::Mat& roi) {
    cv::Point roiCenter=cv::Point(-1,0);//-1 for express error

    int count = 0;
    for (int i = 0; i < roi.rows; i++) {
        for (int j = 0; j < roi.cols; j++) {
            if (roi.at<uchar>(i, j) == 255) {
                count++;
                roiCenter.x += j;
                roiCenter.y += i;
            }
        }
    }

    if (count != 0) {
        roiCenter.x += 1;//remove -1 for express error
        roiCenter.x /= count;
        roiCenter.y /= count;
    } else if(count == 0) {
        std::cerr << "*****Cannot calculate center point*****\n";
        //-1,0 use x for express error
    }
    return roiCenter;
}

int eyeDirectionFinder::findDirectionByVector(const cv::Point& pupilVector) {
    if (pupilVector.x<-LRFLEX && pupilVector.y>UFLEX)
        return static_cast<int>(DIRECTION::ur);
    else if (pupilVector.x < -LRFLEX && pupilVector.y < -DFLEX)
        return static_cast<int>(DIRECTION::dr);
    else if (pupilVector.x < -LRFLEX)
        return static_cast<int>(DIRECTION::r);
    else if (pupilVector.x > LRFLEX && pupilVector.y > UFLEX)
        return static_cast<int>(DIRECTION::ul);
    else if (pupilVector.x > LRFLEX && pupilVector.y < -DFLEX)
        return static_cast<int>(DIRECTION::dl);
    else if (pupilVector.x > LRFLEX)
        return static_cast<int>(DIRECTION::l);
    else if (pupilVector.y > UFLEX)
        return static_cast<int>(DIRECTION::u);
    else if (pupilVector.y < -DFLEX)
        return static_cast<int>(DIRECTION::d);
    else if (pupilVector.x<LRFLEX && pupilVector.x>-LRFLEX && pupilVector.y<UFLEX && pupilVector.y>-DFLEX)
        return static_cast<int>(DIRECTION::p);
    return static_cast<int>(DIRECTION::fail);
}

std::string eyeDirectionFinder::findDirection(cv::Mat image) {
    std::cout << "\n\nFinding direction of eyes...\n";
    std::vector<cv::Rect> eyes;

    //1. opencv eye detection
    eyeCascade.detectMultiScale(image, eyes, 1.01, 2, 0 | cv::CASCADE_SCALE_IMAGE, cv::Size(image.rows / MIN_EYE_RATIO, image.cols / MIN_EYE_RATIO));
    
    //2. adjust error of detected eye
    if (adjustDetectedEyes(eyes, image) !=0) {
        std::cerr << "*****Fail! We cannot detect eye properly. We finally detect" << eyes.size() << " eyes*****\n";
        //printImage("failed", image, 4);
        std::cout<<"[result] "<< DIRECTIONtoString[static_cast<int>(DIRECTION::fail)]<<"\n";
        return DIRECTIONtoString[static_cast<int>(DIRECTION::fail)];
    }

    //3. calculate pupilVector
    cv::Point pupilCenter, gazeCenter;
    cv::Point avgVector = cv::Point(0, 0);//Rt & Lt 's avg vector
    cv::Mat pupilRoi, gazeRoi;;

    for (auto& eye : eyes) {
        cv::Mat partialEye = image(eye);//detected eye region

        //3-1. get pupilRoi by color threshold
        cv::inRange(partialEye, MIN_SCALAR_PUPIL, MAX_SCALAR_PUPIL, pupilRoi);//����3

        //3-2. judge spielmann by using pupilRoi's brightness
        if (sum(pupilRoi)[0] == 0) {
            if (eye.x < image.cols / 2) {
                std::cout << "[result] " << DIRECTIONtoString[static_cast<int>(DIRECTION::sr)] << "\n";
                return DIRECTIONtoString[static_cast<int>(DIRECTION::sr)];
            }
            else {
                std::cout << "[result] " << DIRECTIONtoString[static_cast<int>(DIRECTION::sl)] << "\n";
                return DIRECTIONtoString[static_cast<int>(DIRECTION::sl)];
            }
        }

        //3-3. get gazeRoi by color threshold
        cv::inRange(partialEye, MIN_SCALAR_GAZE, MAX_SCALAR_GAZE, gazeRoi);


        //3-4. get centerPoint of pupilRoi
        std::cout << "calculating center of pupil...\n";
        pupilCenter = getCenterPoint(pupilRoi);
        if (pupilCenter.x == -1) {//if cannot check center of pupil
            return DIRECTIONtoString[static_cast<int>(DIRECTION::fail)];
        }

        //3-5. get centerPoitn of gazeRoi
        std::cout << "calculating center of gaze...\n";
        gazeCenter = getCenterPoint(gazeRoi);
        if (gazeCenter.x == -1) {
            return DIRECTIONtoString[static_cast<int>(DIRECTION::fail)];
        }

        //3-6. calculate pupilVector & save each vector for avgVector
        cv::Point vector = cv::Point(pupilCenter.x - gazeCenter.x, gazeCenter.y - pupilCenter.y);//����_��и������.
        std::cout << "each vector: " << vector << "\n";
        avgVector.x += vector.x;
        avgVector.y += vector.y;
    }

    //4. get avgPupilVector about two eyes.
    avgVector.x /= 2;
    avgVector.y /= 2;
    std::cout << "avg vector: " << avgVector << "\n";
    return DIRECTIONtoString[findDirectionByVector(avgVector)];
};