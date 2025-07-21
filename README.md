# Eye-movement-detection-algorithm-for-brain-abnormality-diagnosis

**Affiliation:**  
CHOI JIWOONG, Student Researcher at Medical Device R&D Lab, Gachon University College of Medicine  
Collaborated with Ophthalmology Professor at Seoul National University Bundang Hospital

---

## Project Overview

This project presents an image processing algorithm to detect eye movement for early diagnosis of brain and inner ear abnormalities, such as vertigo and strabismus, particularly focusing on children. Early diagnosis is critical because conventional diagnosis requires specialized equipment (e.g., video oculography, Frenzel glasses) and cooperation from patients, which is difficult for young children.

Our algorithm uses general cameras without specialized hardware and applies Haar Cascade classifiers and gaze vector analysis to detect eye regions and estimate gaze direction.

---

## Abstract

Dizziness caused by abnormalities in the inner ear or brain affects approximately 25% of adults, and half of these individuals suffer difficulties in daily life. Direct diagnosis is challenging, so observing eye movements is essential for early detection. However, diagnosis currently requires professional devices and patient cooperation, which is especially hard for young children (aged 3-4), who are at risk of permanent damage from eye movement disorders such as strabismus.

This project proposes an effective image processing algorithm and machine learning approach to measure eye movement using ordinary cameras, facilitating early diagnosis and treatment.

---

## Project Structure

- `README.md` (this file)  
- `Untitled14.ipynb` — Testing notebook  
- `eval.cpp` — Evaluation of detection accuracy on dataset  
- `main.cpp` — Main testing program  
- `trainImage 세트별 데이터 코드.cpp` — Code for handling dataset by sets  
- `utils.cpp` — Utility function implementations  
- `utils.h` — Utility function and class declarations  

---

## Key Components

### 1. **eyeDetectorRoi**  
- Extracts eye Region of Interest (ROI) from a face image using Haar Cascade classifiers.  
- Method: `.detect(cv::Mat image)` returns a cropped Mat image of the eye ROI or an empty image if detection fails.  
- Handles exception cases by detecting the face first if eye detection fails.  

### 2. **eyeDirectionFinder**  
- Determines one of nine gaze directions (e.g., up-right, left, center) from the eye ROI image.  
- Method: `.findDirection(cv::Mat image)` returns gaze direction as a string.  
- Uses pupil and gaze color thresholds and vector calculations to estimate gaze direction.  
- Provides access to the average gaze vector used in the detection.  

---

## Usage Example

- Prepare Haar cascade classifier XML files (e.g., `haarcascade_eye_tree_eyeglasses.xml`).  
- Initialize `eyeDetectorRoi` and `eyeDirectionFinder` with the classifier path.  
- Use `.detect()` to extract eye ROI from a face image.  
- Use `.findDirection()` to obtain the gaze direction from the eye ROI.

Refer to `main.cpp` and `eval.cpp` for implementation details and examples.

---

## Example Output (from evaluation)

For each gaze direction label (e.g., "ur", "dl"), the program tests detection accuracy on corresponding images and outputs results such as:  
```
ur(100 images) Testing... Result: 95/100 (95%)
dl(100 images) Testing... Result: 90/100 (90%)
...
Finish!
```


---

## Dependencies

- OpenCV (with `objdetect`, `highgui`, and `imgproc` modules)  
- C++17 (for `<filesystem>`)  

---

## Contact

If you have questions or want to collaborate, please contact the Medical Device R&D Lab, Gachon University College of Medicine.

---

Thank you for your interest in this eye movement detection algorithm project aimed at improving early diagnosis for brain and inner ear abnormalities.

