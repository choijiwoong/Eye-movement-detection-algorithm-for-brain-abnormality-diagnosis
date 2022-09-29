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
#include <map>

int main() {
    //****test****
    const std::string dataPath = "trainImageSets/";
    std::vector<std::vector<std::string>> answers = std::vector<std::vector<std::string>>(12);
    const std::string DIRECTIONtoString[12] = { "fail", "ur", "u", "ul", "r", "p", "l", "dr", "d", "dl", "sr", "sl" };//lookup table
    std::map < std::string, int > StringtoDIRECTION = std::map < std::string, int >({
        {"fail", 0}, {"ur" , 1}, {"u" , 2}, {"ul" , 3}, {"r" , 4}, {"p" , 5}, {"l" , 6}, {"dr" , 7}, {"d" , 8}, {"dl" , 9}, {"sr" , 10}, {"sl" , 11}
        });

    const std::string harEyePath = "C:/Program Files/opencv/sources/data/haarcascades/haarcascade_eye_tree_eyeglasses.xml";
    eyeDirectionFinder finder = eyeDirectionFinder(harEyePath);

    for (const auto& entry : std::filesystem::directory_iterator(dataPath)) {//세트별 데이터 담는 디렉토리 오픈
        int totalLen = 9;
        int correct = 0;
        for (const auto& entry2 : std::filesystem::directory_iterator(dataPath+entry.path().filename().string())) {//세트 데이터 오픈
            std::string contentName = entry2.path().filename().string();
            std::vector<std::string> fileNameElement = split(contentName, '_');
            std::string label = split(fileNameElement[1], '.')[0];//_이후, .이전

            std::string res = finder.findDirection(cv::imread(dataPath + entry.path().filename().string() + "/" + entry2.path().filename().string()));
            if (res == label)
                correct++;
        }
        float accuracy = float(correct) / totalLen * 100;
        //std::cout << entry.path().filename().string()<<"result: " << accuracy << "\n";
        if (accuracy < 70) {
            std::cout << "Low accuracy file: " << entry.path().filename().string() << "("<<accuracy<<"%)\n";
            //std::cin.get();
        }
    }

    
    std::cout << "\nFinish!\n";
    return 0;
}