# Eye-movement-detection-algorithm-for-brain-abnormality-diagnosis
All tools are in utils.h

eyeDetectorRoi is tool for making eye roi image by using face picture.
you can get result Mat image by calling .detect(cv::Mat image) method.
By using harrcascade classifier, it will return proper roi image or empty image when it fails.

eyeDirectionFinder is tool for getting nine gaze direction by using eye roi image.
you can get nine gaze direction by calling .findDirection(cv::Mat image) method.
By using harrcascade classifier and gaze vector, it will return proper direction as string.

if you want to get gaze vector, you can use avgVector(variable) in eyeDirectionFinder::findDirection.
You should enter path of c++ opencv harrcascade classifier. 
check main.cpp for example.
