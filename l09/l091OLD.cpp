#include <stdio.h>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <math.h>
#include <cmath>
#include <fstream>
#include <opencv2/calib3d.hpp>

using namespace std;
using namespace cv;

void part1() {
    // Goal of this program is to find a chessboard in a video and draw a cube on top of it
    // First we need to find the chessboard in the video

    // Make the Cube
    Mat cube = Mat::zeros(8, 3, CV_64F);
    cube.at<double>(0, 0) = 1;
    cube.at<double>(0, 1) = 1;
    cube.at<double>(0, 2) = -2;

    cube.at<double>(1, 0) = 1;
    cube.at<double>(1, 1) = -1;
    cube.at<double>(1, 2) = -2;

    cube.at<double>(2, 0) = -1;
    cube.at<double>(2, 1) = 1;
    cube.at<double>(2, 2) = -2;

    cube.at<double>(3, 0) = -1;
    cube.at<double>(3, 1) = -1;
    cube.at<double>(3, 2) = -2;

    cube.at<double>(4, 0) = 1;
    cube.at<double>(4, 1) = 1;
    cube.at<double>(4, 2) = 0;

    cube.at<double>(5, 0) = 1;
    cube.at<double>(5, 1) = -1;
    cube.at<double>(5, 2) = 0;

    cube.at<double>(6, 0) = -1;
    cube.at<double>(6, 1) = 1;
    cube.at<double>(6, 2) = 0;

    cube.at<double>(7, 0) = -1;
    cube.at<double>(7, 1) = -1;
    cube.at<double>(7, 2) = 0;

    // Make the projection matrix
    vector<vector<Point2f>> imagePoints; // img

    vector<vector<Point3f>> objectPoints; // obj

    // create corner matrix and one for optical flow
    vector<Point2f> corners;
    vector<Point2f> corners2; // cornerOF

    vector<Point2f> tempcorners; // out

    vector<Point3f> chessboardPoints; // objp
    chessboardPoints.push_back(Point3f(1, 1, 0));
    chessboardPoints.push_back(Point3f(-1, 1, 0));
    chessboardPoints.push_back(Point3f(1, -1, 0));
    chessboardPoints.push_back(Point3f(-1, -1, 0));

    // create a vector of vectors for img and obj for calibrate camera
    vector<vector<Point2f>> imgCamera;
    vector<vector<Point3f>> objectCamera;

    // create previous frame matrix
    Mat prevFrame;
    int frameCount = 0;

    // Read in the video
    VideoCapture cap("withChessBoard.MOV");
    VideoWriter video("vr.avi", cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), 30, Size(800, 600), true);

    if (!cap.isOpened()) {
        cout << "Error opening video stream or file" << endl;
        return;
    }

    while (cap.isOpened()) {
        Mat currentFrame;
        cap.read(currentFrame);
        if (currentFrame.empty()) {
            break;
        }
        // rotate the frame 90 degrees
        transpose(currentFrame, currentFrame);
        flip(currentFrame, currentFrame, 1);

        // convert to grayscale
        Mat grayFrame;
        cvtColor(currentFrame, grayFrame, COLOR_BGR2GRAY);

        // clear tempcorners for later use
        tempcorners.clear();

        // find the chessboard corners
        bool hasCorners = findChessboardCorners(grayFrame, Size(4, 4), tempcorners, CALIB_CB_ADAPTIVE_THRESH | CALIB_CB_NORMALIZE_IMAGE | CALIB_CB_FAST_CHECK);

        // check if it has corners
        // clear old corners
        corners.clear();

        if (hasCorners) {
            // refine the corners
            cornerSubPix(grayFrame, corners, Size(11, 11), Size(-1, -1), TermCriteria(TermCriteria::EPS + TermCriteria::COUNT, 30, 0.1));

            // add the refined corners
            corners.push_back(tempcorners[16]);
            corners.push_back(tempcorners[18]);
            corners.push_back(tempcorners[30]);
            corners.push_back(tempcorners[32]);
        } else {
            // use optical flow to find the corners
            vector<unsigned char> status;
            vector<float> err;
            corners2.clear(); // clear corners2 before assigning new points
            if(!prevFrame.empty()){
            calcOpticalFlowPyrLK(prevFrame, currentFrame, corners, corners2, status, err, Size(15, 15), 2, TermCriteria(TermCriteria::COUNT + TermCriteria::EPS, 10, 0.03));
            }
            // set the corners to the corners found by optical flow
            corners = corners2;
        }

        // add the corners to the imagePoints and chessboardPoints to objectPoints
        objectPoints.push_back(chessboardPoints);
        imagePoints.push_back(corners);

        // camera calibration points added every 7 frames
        if (frameCount % 7 == 0) {
            imgCamera.push_back(corners);
            objectCamera.push_back(chessboardPoints);
        }

        // increase frame count
        frameCount++;

        // keep track of previous frame
        prevFrame = currentFrame.clone();
    }
    // release the video capture
    cap.release();

    // calibrate the camera
    Mat cameraMatrix, distCoeffs, rvecs, tvecs;

    calibrateCamera(objectCamera, imgCamera, prevFrame.size(), cameraMatrix, distCoeffs, rvecs, tvecs);

    vector<Point3f> cubePoints;
    for (int i = 0; i < cube.rows; i++) {
        cubePoints.push_back(cv::Point3f(cube.at<double>(i, 0), cube.at<double>(i, 1), cube.at<double>(i, 2)));
    }

    // create edges for the cube
    vector<Point2f> edges;
    for (int i = 0; i < cubePoints.size() - 1; i++) {
        for (int j = i + 1; j < cubePoints.size(); j++) {
            double distance = cv::norm(cubePoints[i] - cubePoints[j]);
            if (std::abs(distance - 2.0) < 1e-6) {
                edges.push_back(Point2i(i, j));
            }
        }
    }

    // capture the video again
    cap = VideoCapture("withChessBoard.MOV");
    frameCount = 0;

    while (cap.isOpened()) {
        Mat currentFrame;
        cap.read(currentFrame);
        if (currentFrame.empty()) {
            break;
        }
        // rotate the frame 90 degrees
        transpose(currentFrame, currentFrame);
        flip(currentFrame, currentFrame, 1);
        // solve Perspective-n-Point | source: https://docs.opencv.org/4.x/d5/d1f/calib3d_solvePnP.html 
        solvePnP(objectPoints[frameCount], imagePoints[frameCount], cameraMatrix, distCoeffs, rvecs, tvecs);
        // project the points
        vector<Point2f> projectedPoints;
        projectPoints(cubePoints, rvecs, tvecs, cameraMatrix, distCoeffs, projectedPoints);
        for (int i = 0; i < edges.size(); i++) {
            cv::line(currentFrame, projectedPoints[edges[i].x], projectedPoints[edges[i].y], cv::Scalar(0, 0, 255), 2);
        }
        // write the video
        video.write(currentFrame);
        // increase frame count
        frameCount++;
    }
    // release the video capture
    cap.release();
    // release the video writer
    video.release();
}

int main() {
    part1();
    return 0;
}






// #include <stdio.h>
// #include <iostream>
// #include <opencv2/opencv.hpp>
// #include <opencv2/highgui/highgui.hpp>
// #include <math.h>
// #include <cmath>
// #include <fstream>
// #include <opencv2/calib3d.hpp>

// using namespace std;
// using namespace cv;

// void part1(){
//     //Goal of this program is to find a chessboard in a video and draw a cube on top of it
//     //First we need to find the chessboard in the video

//     //Make the Cube
//     Mat cube = Mat::zeros(8, 3, CV_64F);
//     cube.at<double>(0, 0) = 1;
//     cube.at<double>(0, 1) = 1;
//     cube.at<double>(0, 2) = -2;

//     cube.at<double>(1, 0) = 1;
//     cube.at<double>(1, 1) = -1;
//     cube.at<double>(1, 2) = -2;

//     cube.at<double>(2, 0) = -1;
//     cube.at<double>(2, 1) = 1;
//     cube.at<double>(2, 2) = -2;

//     cube.at<double>(3, 0) = -1;
//     cube.at<double>(3, 1) = -1;
//     cube.at<double>(3, 2) = -2;

//     cube.at<double>(4, 0) = 1;
//     cube.at<double>(4, 1) = 1;
//     cube.at<double>(4, 2) = 0;
    
//     cube.at<double>(5, 0) = 1;
//     cube.at<double>(5, 1) = -1;
//     cube.at<double>(5, 2) = 0;

//     cube.at<double>(6, 0) = -1;
//     cube.at<double>(6, 1) = 1;
//     cube.at<double>(6, 2) = 0;

//     cube.at<double>(7, 0) = -1;
//     cube.at<double>(7, 1) = -1;
//     cube.at<double>(7, 2) = 0;

//     //Make the projection matrix
//     vector<vector<Point2f>> imagePoints; //img

//     vector<vector<Point3f>> objectPoints; //obj

//     //create corner matrix and one for optical flow
//     vector<Point2f> corners;
//     vector<Point2f> corners2; //cornerOF

//     vector<Point2f> tempcorners; //out

//     vector<Point3f> chessboardPoints; //objp
//     chessboardPoints.push_back(Point3f(1, 1, 0));
//     chessboardPoints.push_back(Point3f(-1, 1, 0));
//     chessboardPoints.push_back(Point3f(1, -1, 0));
//     chessboardPoints.push_back(Point3f(-1, -1, 0));

//     //create a vector of vectors for img and obj for calibrate camera
//     vector<vector<Point2f>> imgCamera;
//     vector<vector<Point3f>> objectCamera;


//     //create previous frame matrix
//     Mat prevFrame;
//     int frameCount = 0;

    
//     //Read in the video
//     VideoCapture cap("withChessBoard.MOV");
//     VideoWriter video("vr.avi", cv::VideoWriter::fourcc('M','J','P','G'), 30, Size(800,600), true); 

//     if(!cap.isOpened()){
//         cout << "Error opening video stream or file" << endl;
//         return;
//     }
//     while(cap.isOpened()){
//         Mat currentFrame;
//         cap.read(currentFrame);
//         if(currentFrame.empty()){
//             break;
//         }
//         //rotate the frame 90 degrees
//         transpose(currentFrame, currentFrame);
//         flip(currentFrame, currentFrame, 1);

//         //convert to grayscale
//         Mat grayFrame;
//         cvtColor(currentFrame, grayFrame, COLOR_BGR2GRAY);

//         //clear tempcorners for later use
//         tempcorners.clear();

//         //find the chessboard corners
//         bool hasCorners = findChessboardCorners(grayFrame, Size(4, 4), tempcorners, CALIB_CB_ADAPTIVE_THRESH & CALIB_CB_NORMALIZE_IMAGE & CALIB_CB_FAST_CHECK);

//         //check if it has corners
//         if(hasCorners){
//             //refine the corners
//             cornerSubPix(grayFrame, corners, Size(11, 11), Size(-1, -1), TermCriteria(TermCriteria::EPS + TermCriteria::COUNT, 30, 0.1));

//             //clear old corners
//             corners.clear();
//             corners.push_back(tempcorners[16]);
//             corners.push_back(tempcorners[18]);
//             corners.push_back(tempcorners[30]);
//             corners.push_back(tempcorners[32]);

//         }
//         else{
//             //use optical flow to find the corners
//             vector<unsigned char> status;
//             vector<float> err;
//             calcOpticalFlowPyrLK(prevFrame, currentFrame, corners, corners2, status, err, Size(15, 15), 2, TermCriteria(TermCriteria::COUNT + TermCriteria::EPS, 10, 0.03));
//             //set the corners to the corners found by optical flow
//             corners.clear();
//             corners = corners2;
//         }

//         //add the corners to the imagePoints and chessboardPoints to objectPoints
//         objectPoints.push_back(chessboardPoints);
//         imagePoints.push_back(corners);

//         //camera calibration points added every 7 frames
//         if(frameCount % 7 == 0){
//             imgCamera.push_back(corners);
//             objectCamera.push_back(chessboardPoints);
//         }

//         //increae frame count
//         frameCount++;

//         //keep track of previous frame
//         prevFrame = currentFrame.clone();
//     }
//     //release the video capture
//     cap.release();

//     //calibrate the camera
//     Mat cameraMatrix, distCoeffs, rvecs, tvecs;

//     calibrateCamera(objectCamera, imgCamera, prevFrame.size(), cameraMatrix, distCoeffs, rvecs, tvecs);

//     vector<Point3f> cubePoints;
//     for(int i = 0; i < cube.rows; i++){
//         cubePoints.push_back(cv::Point3f(cube.at<double>(i, 0), cube.at<double>(i, 1), cube.at<double>(i, 2)));
//     }

//     //create edges for the cube
//     vector<Point2f> edges;
//     for (int i = 0; i < cubePoints.size() - 1; i++) {
//     for (int j = i + 1; j < cubePoints.size(); j++) {
//         double distance = cv::norm(cubePoints[i] - cubePoints[j]);
//         if (distance == 2) {
//             edges.push_back(Point2i(i, j));
//         }
//     }

//     //capture the video again
//     cap = VideoCapture("withChessBoard.MOV");
//     frameCount = 0;



//     while(cap.isOpened()){
//         Mat currentFrame;
//         cap.read(currentFrame);
//         if(currentFrame.empty()){
//             break;
//         }
//         //rotate the frame 90 degrees
//         transpose(currentFrame, currentFrame);
//         flip(currentFrame, currentFrame, 1);
//         //solve Perspective-n-Point | source: https://docs.opencv.org/4.x/d5/d1f/calib3d_solvePnP.html 
//         solvePnP(objectPoints[frameCount], imagePoints[frameCount], cameraMatrix, distCoeffs, rvecs, tvecs);
//         //project the points
//         vector<Point2f> projectedPoints;
//         projectPoints(cubePoints, rvecs, tvecs, cameraMatrix, distCoeffs, projectedPoints);
//         for (int i = 0; i < edges.size(); i++) {
//             cv::line(currentFrame, projectedPoints[edges[i].x], projectedPoints[edges[i].y], cv::Scalar(0, 0, 255), 2);
//         }
//         //write the video
//         video.write(currentFrame);
//         //increase frame count
//         frameCount++;
//     }
//     //release the video capture
//     cap.release();
//     //release the video writer
//     video.release();
// }
// }

// int main(){
//     part1();
//     return 0;
// }