#include <stdio.h>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d.hpp>
// Defining the dimensions of checkerboard
int CHECKERBOARD[2]{7,7};
int main()
{
  cv::VideoWriter outputVideo;  
  std::vector<cv::Point2f> corner_pts;
  bool success;
  int counter = 0;  
  cv::VideoCapture cap1("withChessBoard.MOV");
  // Check if camera opened successfully
  if(!cap1.isOpened()){
    std::cout << "Error opening video stream or file" << std::endl;
    return -1;
  }  
  std::cout << "Number of Frames: " + std::to_string((int)cap1.get(cv::CAP_PROP_FRAME_COUNT)) << std::endl;
  outputVideo.open("vr.avi", cv::VideoWriter::fourcc('M','J','P','G'), 30, cv::Size(1920,1080), true);
  std::cout << "Avi File Opened: " << std::endl;    
  std::vector<cv::Point3f> objp1;
  std::vector<cv::Point3f> cubePts;  
  for(int i = -3; i<= 3; i++)
  {
    for(int j = -3; j<= 3; j++)
      objp1.push_back(cv::Point3f(j,i,0));
  }
    
  cubePts.push_back(cv::Point3f(-1,-1,0));
  cubePts.push_back(cv::Point3f(1,-1,0));
  cubePts.push_back(cv::Point3f(1,-1,-2));
  cubePts.push_back(cv::Point3f(-1,-1,-2));
  cubePts.push_back(cv::Point3f(-1,1,0));
  cubePts.push_back(cv::Point3f(1,1,0));
  cubePts.push_back(cv::Point3f(1,1,-2));
  cubePts.push_back(cv::Point3f(-1,1,-2));
    
  std::map<int,std::vector<int> > squareMap;
  squareMap[0] = std::vector<int>{4,3,1};
  squareMap[1] = std::vector<int>{0,2,5};
  squareMap[2] = std::vector<int>{1,3,6};
  squareMap[3] = std::vector<int>{7,2,0};
  squareMap[4] = std::vector<int>{5,7,0};
  squareMap[5] = std::vector<int>{1,6,4};
  squareMap[6] = std::vector<int>{5,7,2};
  squareMap[7] = std::vector<int>{4,6,3};  
    
  double distance;  
  //for(int x = 0; x < 100; x++){
  while(1){    
    std::vector<std::vector<cv::Point3f> > objpoints;
    std::vector<std::vector<cv::Point2f> > imgpoints;  
    cv::Mat frame;
    cv::Mat gray;
    std::cout << std::to_string(counter) + ": Entered While Loop" << std::endl;
    cap1 >> frame;
    if (frame.empty()){
      break;
    }
    cv::cvtColor(frame,gray,cv::COLOR_BGR2GRAY);
    success = cv::findChessboardCorners(gray, cv::Size(CHECKERBOARD[0], CHECKERBOARD[1]), corner_pts, cv::CALIB_CB_ADAPTIVE_THRESH + cv::CALIB_CB_NORMALIZE_IMAGE + cv::CALIB_CB_FAST_CHECK);
    objpoints.push_back(objp1);
    imgpoints.push_back(corner_pts);   
    std::cout << "Found corners\n";
    if(success)
    {  
      std::vector<cv::Point2f> imagePoints, jacobian;
      cv::Mat currR, currT;  
      std::vector<cv::Point2f> drawCornerPoints = std::vector<cv::Point2f>{corner_pts[16], corner_pts[18], corner_pts[30], corner_pts[32]};   
      cv::Mat cameraMatrix,distCoeffs,R,T;
      cv::calibrateCamera(objpoints, imgpoints, cv::Size(gray.rows,gray.cols), cameraMatrix, distCoeffs, R, T);  
      cv::solvePnP(objp1, corner_pts, cameraMatrix, distCoeffs, currR, currT, false);
      cv::projectPoints(cubePts, R, T, cameraMatrix, distCoeffs, imagePoints);
      if(counter == 0){
          distance = cv::sqrt(cv::pow(drawCornerPoints[0].x - drawCornerPoints[1].x,2) + cv::pow(drawCornerPoints[0].y - drawCornerPoints[1].y,2));
      }  
      imagePoints[3] = cv::Point2f(imagePoints[0].x, imagePoints[0].y - distance);
      imagePoints[2] = cv::Point2f(imagePoints[1].x, imagePoints[1].y - distance); 
      imagePoints[6] = cv::Point2f(imagePoints[5].x, imagePoints[5].y - distance); 
      imagePoints[7] = cv::Point2f(imagePoints[4].x, imagePoints[4].y - distance); 
      std::cout << "Image Points: ";
      for(int y = 0; y < imagePoints.size(); y++){
          std::cout << "(" + std::to_string(imagePoints[y].x) + ", " + std::to_string(imagePoints[y].y) + "), ";
      }
      std::cout << "\n";
      cv::drawChessboardCorners(frame, cv::Size(2, 2), drawCornerPoints, success);
      for(int x = 0; x < (int)imagePoints.size(); x++){
            std::vector<int> tangentIndices = squareMap.at(x); 
            for(int y = 0; y < (int)tangentIndices.size(); y++){
                int currIdx = tangentIndices.at(y); //get the current index we need to draw the line with
                line(frame, imagePoints.at(x), imagePoints.at(currIdx), cv::Scalar(255,255,255), 2, cv::LINE_AA);
            }
      }  
    }
    outputVideo << frame;
    std::cout << "Check works" << std::endl;
    counter += 1;
    cv::waitKey(1);
  }  
  
  return 0;
}
