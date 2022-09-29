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

[Abstract]
Dizziness, which occurs when there is an abnormality in the inner ear or brain, 25% of the adult population experiences vertigo, and half of them experience difficulty in physical activity or daily life due to dizziness. Since it is impossible to directly identify abnormalities in the inner ear or brain, early diagnosis through eye movement observation is important. However, it takes a considerable amount of time for a patient to timely diagnose, treat, and prevent a disease because professional equipment such as a video ocular diagram and Frenzel glasses are required for diagnosis. In addition, in the case of children aged 3-4 years, who develop binocular vision the fastest, eye movement diseases such as strabismus can cause permanent damage, but cooperation with the examination process is difficult. In this paper, we propose and compare an effective image processing algorithm that can measure eye movement in children and an algorithm using machine learning focusing on the use in general cameras without speialized equipment. Using this, it is expected that strabismus can be diagnosed and treated early by early diagnosis and treatment of inner ear or brain abnormalities, and simple eye movement measurement in children.
