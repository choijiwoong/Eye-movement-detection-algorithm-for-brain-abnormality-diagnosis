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
    const std::string dataPath = "trainImage/";
    std::vector<std::vector<std::string>> answers = std::vector<std::vector<std::string>>(12);
    const std::string DIRECTIONtoString[12] = { "fail", "ur", "u", "ul", "r", "p", "l", "dr", "d", "dl", "sr", "sl" };//lookup table
    std::map < std::string, int > StringtoDIRECTION = std::map < std::string, int >({
        {"fail", 0}, {"ur" , 1}, {"u" , 2}, {"ul" , 3}, {"r" , 4}, {"p" , 5}, {"l" , 6}, {"dr" , 7}, {"d" , 8}, {"dl" , 9}, {"sr" , 10}, {"sl" , 11}
        });
    for (const auto& entry : std::filesystem::directory_iterator(dataPath)) {
        std::string contentName = entry.path().filename().string();
        std::vector<std::string> fileNameElement = split(contentName, '_');
        answers[StringtoDIRECTION[split(fileNameElement[1], '.')[0]]].push_back(contentName);
    }

    const std::string harEyePath = "C:/Program Files/opencv/sources/data/haarcascades/haarcascade_eye_tree_eyeglasses.xml";
    eyeDirectionFinder finder = eyeDirectionFinder(harEyePath);

    for (int i = 1; i < 12; i++) {//fill...sl
        auto& element = answers[i];
        int totalLen = element.size();
        int correct = 0;
        std::cout << DIRECTIONtoString[i] << "(" << element.size() << "개)" << "를 테스트하는중...";
        for (auto& elem : element) {
            if (finder.findDirection(cv::imread(dataPath + elem)) == DIRECTIONtoString[i])
                correct++;
        }
        std::cout << " 결과: " << correct << "/" << totalLen << " (" << float(correct) / totalLen * 100 << "%)\n";
    }

    //****test****
    std::cout << "\nFinish!\n";
    return 0;
}