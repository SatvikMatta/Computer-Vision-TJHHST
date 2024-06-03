//Satvik Matta
//6/2/2023

#include <stdio.h>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d.hpp>

using namespace cv;
using namespace std;

void part1(){
VideoWriter writer;
vector<Point2f> corners;
bool found;
bool cont = true;
int counter = 0;
VideoCapture cap("withChessBoard.MOV");

if(!cap.isOpened()){
cout << "Error opening video stream or file" << endl;
return;
}

writer.open("vr.avi", VideoWriter::fourcc('M','J','P','G'), 30, Size(1920,1080), true);

vector<Point3f> obj;
vector<Point3f> cube;

for(int i = -3; i< 4; i++){
for(int j = -3; j< 4; j++){
obj.push_back(Point3f(j,i,0));
}
}


cube.push_back(Point3f(-1,-1,0));
cube.push_back(Point3f(1,-1,0));
cube.push_back(Point3f(1,-1,-2));
cube.push_back(Point3f(-1,-1,-2));
cube.push_back(Point3f(-1,1,0));
cube.push_back(Point3f(1,1,0));
cube.push_back(Point3f(1,1,-2));
cube.push_back(Point3f(-1,1,-2));

map<int,vector<int> > cMap;
cMap[0] = vector<int>{4,3,1};
cMap[1] = vector<int>{0,2,5};
cMap[2] = vector<int>{1,3,6};
cMap[3] = vector<int>{7,2,0};
cMap[4] = vector<int>{5,7,0};
cMap[5] = vector<int>{1,6,4};
cMap[6] = vector<int>{5,7,2};
cMap[7] = vector<int>{4,6,3}; 

Mat prevFrame, prevGray;
cap.read(prevFrame);
cvtColor(prevFrame, prevGray, COLOR_BGR2GRAY);

double dist = 0;
Mat frame;
flip(frame, frame, 1);
flip(frame, frame, 0);
Mat gray;
vector<Point2f> cornerPoints;

Mat cameraMatrix;
Mat distCoeffs;
Mat R;
Mat T;

while(cont){

vector<vector<Point2f>> imgPts;
vector<vector<Point3f>> objPts;

cap >> frame;
if(frame.empty()){
cont = false;
break;
}
flip(frame, frame, 1);
flip(frame, frame, 0);

cvtColor(frame, gray, COLOR_BGR2GRAY);
found = findChessboardCorners(gray, Size(7,7), corners, CALIB_CB_ADAPTIVE_THRESH + CALIB_CB_NORMALIZE_IMAGE + CALIB_CB_FAST_CHECK);
objPts.push_back(obj);
imgPts.push_back(corners);



if(found){
vector<Point2f> imgPts2;
vector<Point2f> temp;

Mat rvec, tvec;
cornerPoints.clear();
cornerPoints.push_back(corners[16]);
cornerPoints.push_back(corners[18]);
cornerPoints.push_back(corners[30]);
cornerPoints.push_back(corners[32]);


if(counter % 20 == 0){
calibrateCamera(objPts, imgPts, gray.size(), cameraMatrix, distCoeffs, R, T);
}


solvePnP(obj, corners, cameraMatrix, distCoeffs, rvec, tvec);

projectPoints(cube, rvec, tvec, cameraMatrix, distCoeffs, imgPts2);

if(counter == 0){
dist = norm(cornerPoints[0] - cornerPoints[1]);
}

imgPts2[3] = Point2f(imgPts2[0].x, imgPts2[0].y - dist);
imgPts2[2] = Point2f(imgPts2[1].x, imgPts2[1].y - dist); 
imgPts2[6] = Point2f(imgPts2[5].x, imgPts2[5].y - dist); 
imgPts2[7] = Point2f(imgPts2[4].x, imgPts2[4].y - dist); 

for (const Point2f& point : imgPts2) {
circle(frame, point, 5, Scalar(255, 0, 0), -1);
}

for(int i=0; i < (int)imgPts2.size(); i++){
vector<int> t = cMap.at(i);
for(int j=0; j < (int) t.size(); j++){
line(frame, imgPts2.at(i), imgPts2.at(t.at(j)), Scalar(255,255,255), 2, LINE_AA);
}
}

std::swap(prevGray, gray);


}
else if((int)cornerPoints.size() > 0){
// Track features using Optical Flow
vector<Point2f> nextCorners;
vector<uchar> status;
vector<float> error;

calcOpticalFlowPyrLK(prevGray, gray, cornerPoints, nextCorners, status, error);

vector<Point2f> trackedCorners;
for (size_t i = 0; i < status.size(); i++) {
if (status[i]) {
trackedCorners.push_back(nextCorners[i]);
}
}

cornerPoints = trackedCorners;

Mat rvec, tvec;

vector<Point3f> obj2;
obj2.push_back(obj[16]);
obj2.push_back(obj[18]);
obj2.push_back(obj[30]);
obj2.push_back(obj[32]);

solvePnP(obj2, cornerPoints, cameraMatrix, distCoeffs, rvec, tvec);

vector<Point2f> imgPts2;
projectPoints(cube, rvec, tvec, cameraMatrix, distCoeffs, imgPts2);

imgPts2[3] = Point2f(imgPts2[0].x, imgPts2[0].y - dist);
imgPts2[2] = Point2f(imgPts2[1].x, imgPts2[1].y - dist);
imgPts2[6] = Point2f(imgPts2[5].x, imgPts2[5].y - dist);
imgPts2[7] = Point2f(imgPts2[4].x, imgPts2[4].y - dist);

for (const Point2f& point : imgPts2) {
circle(frame, point, 5, Scalar(255, 0, 0), -1);
}

for(int i=0; i < (int)imgPts2.size(); i++){
vector<int> t = cMap.at(i);
for(int j=0; j < (int) t.size(); j++){
line(frame, imgPts2.at(i), imgPts2.at(t.at(j)), Scalar(255,255,255), 2, LINE_AA);
}
}
std::swap(prevGray, gray);


}
else{
cout << "No corners found" << " " << counter << endl;
}


writer.write(frame);
counter++;
}
cap.release();
writer.release();
}

int main(){
part1();
return 0;
}