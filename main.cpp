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
#include <time.h>

int main() {
    const std::string dataPath = "data\\3_4";//3+4
    const std::string harEyePath = "C:/Program Files/opencv/sources/data/haarcascades/haarcascade_eye_tree_eyeglasses.xml";
    const std::string harFacePath = "C:/Program Files/opencv/sources/data/haarcascades/haarcascade_frontalface_alt2.xml";
    cv::Mat image;
    std::string result;
    int noDup = 0;

    eyeDirectionFinder finder = eyeDirectionFinder(harEyePath);
    for (const auto& entry : std::filesystem::directory_iterator(dataPath)) {
        std::string contentName = entry.path().filename().string();
        std::vector<std::string> fileNameElement = split(contentName, '_');

        //get only JPG
        const auto ext = entry.path().extension();
        if (ext == ".jpg" || ext == ".JPG")
            image = std::move(cv::imread(entry.path().string()));
        else
            continue;

        result = finder.findDirection(image);
        if (result == "fail") {//check fail
            std::cout << "final result is fail\n";
        }

        //handle result
        if (fileNameElement.size() > 2) {
            std::cout << fileNameElement[1] + "_" + std::to_string(noDup)+ "_" + result<<"\n";
            //cv::imwrite(fileNameElement[1] + std::to_string(noDup) + "_" + std::to_string(noDup)+"_" + result + ".JPG", image);
            //printImage(fileNameElement[1] + "_" + result, image, 4);
        } else {//형식에 맞지않는 파일이름인 경우
            std::cout << split(fileNameElement[0], '.')[0] +"_" + std::to_string(noDup) + "_" + result << "\n";
            //cv::imwrite(split(fileNameElement[0], '.')[0] + "_"+std::to_string(noDup) + "_" + result + ".JPG", image);
            //printImage(split(fileNameElement[0], '.')[0] + "_" + result, image, 4);
        }
        noDup++;
    }
    std::cout << "\nFinish!\n";
    return 0;
}