/*****************************************************************************
*   Markerless AR desktop application.
******************************************************************************
*   by Khvedchenia Ievgen, 5th Dec 2012
*   http://computer-vision-talks.com
******************************************************************************
*   Ch3 of the book "Mastering OpenCV with Practical Computer Vision Projects"
*   Copyright Packt Publishing 2012.
*   http://www.packtpub.com/cool-projects-with-opencv/book
*****************************************************************************/

////////////////////////////////////////////////////////////////////
// File includes:
#include "ARDrawingContext.hpp"
#include "MarkerDetectionFacade.hpp"
#include "DebugHelpers.hpp"

////////////////////////////////////////////////////////////////////
// Standard includes:
#include <opencv2/opencv.hpp>
#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>

/**
 * Processes a recorded video or live view from web-camera and allows you to adjust homography refinement and 
 * reprojection threshold in runtime.
 */
void processVideo( CameraCalibration& calibration, cv::VideoCapture& capture);

/**
 * Processes single image. The processing goes in a loop.
 * It allows you to control the detection process by adjusting homography refinement switch and 
 * reprojection threshold in runtime.
 */
void processSingleImage( CameraCalibration& calibration, const cv::Mat& image);

/**
 * Performs full detection routine on camera frame and draws the scene using drawing context.
 * In addition, this function draw overlay with debug information on top of the AR window.
 * Returns true if processing loop should be stopped; otherwise - false.
 */
bool processFrame(const cv::Mat& cameraFrame, std::auto_ptr<MarkerDetectionFacade>& detector, ARDrawingContext& drawingCtx);

int main(int argc, const char * argv[])
{
    // Change this calibration to yours:
    CameraCalibration calibration(545.31565719766058f, 545.31565719766058f, 326.0f, 183.5f);
    

    std::cout << "Input image not specified" << std::endl;
    std::cout << "Usage: markerless_ar_demo  [filepath to recorded video or image]" << std::endl;



    if (argc == 1)
    {
        processVideo(calibration, cv::VideoCapture());
    }
    else if (argc == 2)
    {
        std::string input = argv[1];
        cv::Mat testImage = cv::imread(input);
        if (!testImage.empty())
        {
            processSingleImage( calibration, testImage);
        }
        else 
        {
            cv::VideoCapture cap;
            if (cap.open(input))
            {
                processVideo( calibration, cap);
            }
        }
    }
    else
    {
        std::cerr << "Invalid number of arguments passed" << std::endl;
        return 1;
    }

    return 0;
}

void processVideo(CameraCalibration& calibration, cv::VideoCapture& capture)
{
    // Grab first frame to get the frame dimensions
    cv::Mat currentFrame, srcFrame;  
    capture >> srcFrame;
	resize(srcFrame, currentFrame, cvSize(653, 368));
    // Check the capture succeeded:
    if (currentFrame.empty())
    {
        std::cout << "Cannot open video capture device" << std::endl;
        return;
    }

    cv::Size frameSize(currentFrame.cols, currentFrame.rows);

	std::auto_ptr<MarkerDetectionFacade> detector = createMarkerDetection(calibration);

    ARDrawingContext drawingCtx("Markerless AR", frameSize, calibration);

    bool shouldQuit = false;
    do
    {
        capture >> srcFrame;
        if (srcFrame.empty())
        {
            shouldQuit = true;
            continue;
        }
		resize(srcFrame, currentFrame, cvSize(653,368));
        shouldQuit = processFrame(currentFrame, detector, drawingCtx);
    } while (!shouldQuit);
}

void processSingleImage(CameraCalibration& calibration, const cv::Mat& image)
{
    cv::Size frameSize(image.cols, image.rows);
    std::auto_ptr<MarkerDetectionFacade> detector = createMarkerDetection(calibration);
    ARDrawingContext drawingCtx("Markerless AR", frameSize, calibration);

    bool shouldQuit = false;
    do
    {
        shouldQuit = processFrame(image, detector, drawingCtx);
    } while (!shouldQuit);
}

bool processFrame(const cv::Mat& cameraFrame, std::auto_ptr<MarkerDetectionFacade>& detector, ARDrawingContext& drawingCtx)
{
    // Clone image used for background (we will draw overlay on it)
    cv::Mat img = cameraFrame.clone();

    // Draw information:
//    if (pipeline.m_patternDetector.enableHomographyRefinement)
//        cv::putText(img, "Pose refinement: On   ('h' to switch off)", cv::Point(10,15), CV_FONT_HERSHEY_PLAIN, 1, CV_RGB(0,200,0));
//    else
//        cv::putText(img, "Pose refinement: Off  ('h' to switch on)",  cv::Point(10,15), CV_FONT_HERSHEY_PLAIN, 1, CV_RGB(0,200,0));

//    cv::putText(img, "RANSAC threshold: " + ToString(pipeline.m_patternDetector.homographyReprojectionThreshold) + "( Use'-'/'+' to adjust)", cv::Point(10, 30), CV_FONT_HERSHEY_PLAIN, 1, CV_RGB(0,200,0));

    // Set a new camera frame:
    drawingCtx.updateBackground(img);

    // Find a pattern and update it's detection status:
	BGRAVideoFrame cameraImage(cameraFrame);
    drawingCtx.isPatternPresent = detector->processFrame(cameraImage);

	std::vector<Transformation> vec_trans = detector->getTransformations();
	if(!vec_trans.empty())
	{
		// Update a pattern pose:
		drawingCtx.patternPose = vec_trans[0];

		// Request redraw of the window:
		drawingCtx.updateWindow();
	}

    // Read the keyboard input:
    int keyCode = cv::waitKey(5); 

    bool shouldQuit = false;
    //if (keyCode == '+' || keyCode == '=')
    //{
    //    pipeline.m_patternDetector.homographyReprojectionThreshold += 0.2f;
    //    pipeline.m_patternDetector.homographyReprojectionThreshold =/* std::*/min(10.0f, pipeline.m_patternDetector.homographyReprojectionThreshold);
    //}
    //else if (keyCode == '-')
    //{
    //    pipeline.m_patternDetector.homographyReprojectionThreshold -= 0.2f;
    //    pipeline.m_patternDetector.homographyReprojectionThreshold = /*std::*/max(0.0f, pipeline.m_patternDetector.homographyReprojectionThreshold);
    //}
    //else if (keyCode == 'h')
    //{
    //    pipeline.m_patternDetector.enableHomographyRefinement = !pipeline.m_patternDetector.enableHomographyRefinement;
    //}
    //else if (keyCode == 27 || keyCode == 'q')
    //{
    //     shouldQuit = true;
    //}

    return shouldQuit;
}


