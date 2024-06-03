#include <stdio.h>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <math.h>
#include <cmath>

using namespace std;

using namespace cv;

vector<Point> perspectiveProjection(vector<Mat> currPoints, vector<double> frame, vector<double> eye){
    vector<Point> pointHolder;
    for(int x = 0; x < currPoints.size(); x++){
        double t = (frame[0] - eye[0]) / (currPoints[x].at<double>(0,0) - eye[0]);
        //double u = (t*currPoints[x].at<double>(0,1)) + ((1-t)*eye[1]);
        //double v = (t*currPoints[x].at<double>(0,2)) + ((1-t)*eye[2]);
        double u = (t*currPoints[x].at<double>(0,1)) + eye[1] - (eye[1]*t);
        double v = (t*currPoints[x].at<double>(0,2)) + eye[2] - (eye[2]*t);
        pointHolder.push_back(Point(u,v));
    }
    return pointHolder;
}

void frame(VideoWriter outputVideo, vector<Mat> points, vector<Mat> pointsTri){
    Mat currFrame = cv::Mat::zeros(600,800,CV_64FC3);
    vector<double> plane = vector<double>{500, 0, 0};
    vector<double> eye = vector<double>{800, 200, 50};
    //dictionary referring to which points touch which
    map<int,vector<int> > squareMap;
    map<int,vector<int> > triangleMap;
    triangleMap[0] = vector<int>{1,2,3};
    triangleMap[1] = vector<int>{0,2,3};
    triangleMap[2] = vector<int>{1,0,3};
    triangleMap[3] = vector<int>{1,2,0};
    squareMap[0] = vector<int>{4,3,1};
    squareMap[1] = vector<int>{0,2,5};
    squareMap[2] = vector<int>{1,3,6};
    squareMap[3] = vector<int>{7,2,0};
    squareMap[4] = vector<int>{5,7,0};
    squareMap[5] = vector<int>{1,6,4};
    squareMap[6] = vector<int>{5,7,2};
    squareMap[7] = vector<int>{4,6,3};
    vector<Point> twoDPointsSquare;
    vector<Point> twoDPointsTri;
    twoDPointsSquare = perspectiveProjection(points, plane, eye);
    twoDPointsTri = perspectiveProjection(pointsTri, plane, eye);
    //square
    for(int x = 0; x < (int)points.size(); x++){
    //    twoDPointsSquare.push_back(Point((points[x].at<double>(0,0) + 400), (points[x].at<double>(0,1) + 300)));
        twoDPointsSquare[x] = Point(twoDPointsSquare[x].x + 400, twoDPointsSquare[x].y + 300);
    }
    //triangle
    for(int x = 0; x < (int)pointsTri.size(); x++){
    //    twoDPointsTri.push_back(Point((pointsTri[x].at<double>(0,0) + 400), (pointsTri[x].at<double>(0,1) + 300)));
        twoDPointsTri[x] = Point(twoDPointsTri[x].x + 400, twoDPointsTri[x].y + 300);
    }
    //square
    for(int x = 0; x < (int)twoDPointsSquare.size(); x++){
        vector<int> tangentIndices = squareMap.at(x); 
        for(int y = 0; y < (int)tangentIndices.size(); y++){
            int currIdx = tangentIndices.at(y); //get the current index we need to draw the line with
            line(currFrame, twoDPointsSquare.at(x), twoDPointsSquare.at(currIdx), Scalar(218,165,32), 2, LINE_AA);
        }
    }
    //triangle
    for(int x = 0; x < (int)twoDPointsTri.size(); x++){
        vector<int> tangentIndices = triangleMap.at(x); 
        for(int y = 0; y < (int)tangentIndices.size(); y++){
            int currIdx = tangentIndices.at(y); //get the current index we need to draw the line with
            line(currFrame, twoDPointsTri.at(x), twoDPointsTri.at(currIdx), Scalar(105,105,105), 2, LINE_AA);
        }
    }
    Mat image8Bit;
    currFrame.convertTo(image8Bit, CV_8U);
    cvtColor(image8Bit, image8Bit, COLOR_BGR2RGB);
    outputVideo << image8Bit;
}

void transformation(vector<Mat> points, double degrees){ //function to rotate the points in the matrix
    double radians = (M_PI/180) * degrees;
    double mydata[]= {1,0,0,0,0, cos(radians), sin(radians), 0, 0, -1*sin(radians), cos(radians), 0, 0, 0, 0, 1};
    Mat rotationMatrixX = Mat(4,4,CV_64FC1, mydata); //transformation on the x-axis
    for(int x = 0; x < (int)points.size(); x++){
        points[x] = rotationMatrixX*points[x];
    }
    double mydata2[] = {cos(radians), 0, -1*sin(radians), 0, 0, 1, 0, 0, sin(radians), 0, cos(radians), 0, 0, 0, 0, 1}; //transformation on the y-axis
    Mat rotationMatrixX2 = Mat(4,4,CV_64FC1, mydata2);
    for(int x = 0; x < (int)points.size(); x++){
        points[x] = rotationMatrixX2*points[x];
    }
}

void part2(){   
    //create the points of squares
    
    vector<Mat> pointsSquare{(Mat_<double>(4,1) << -1, -1, -1, 1), (Mat_<double>(4,1) << 1, -1, -1, 1), (Mat_<double>(4,1) << 1, -1, 1, 1), (Mat_<double>(4,1) << -1, -1, 1, 1), (Mat_<double>(4,1) << -1, 1, -1, 1), (Mat_<double>(4,1) << 1, 1, -1, 1), (Mat_<double>(4,1) << 1, 1, 1, 1), (Mat_<double>(4,1) << -1, 1, 1, 1)};
    for(int x = 0; x < pointsSquare.size(); x++){
        pointsSquare[x].at<double>(0,0) = (pointsSquare[x].at<double>(0,0) * 100);
        pointsSquare[x].at<double>(0,1) = (pointsSquare[x].at<double>(0,1) * 100);
        pointsSquare[x].at<double>(0,2) = (pointsSquare[x].at<double>(0,2) * 100);
    }
    
    vector<Mat> pointsTriangle{(Mat_<double>(4,1) << (1/sqrt(3)), 0, 0, 1), (Mat_<double>(4,1) << 0, 0, (2/sqrt(6)), 1), (Mat_<double>(4,1) << ((-1*sqrt(3))/6), 0.5, 0, 1), (Mat_<double>(4,1) << ((-1*sqrt(3))/6), -0.5, 0, 1)};
    
    for(int x = 0; x < pointsTriangle.size(); x++){
        pointsTriangle[x].at<double>(0,0) = (pointsTriangle[x].at<double>(0,0) * 150);
        pointsTriangle[x].at<double>(0,1) = (pointsTriangle[x].at<double>(0,1) * 150);
        pointsTriangle[x].at<double>(0,2) = (pointsTriangle[x].at<double>(0,2) * 150);
    }
    //make the video with 200 frames
    VideoWriter outputVideo;
    outputVideo.open("rotation.avi", VideoWriter::fourcc('M','J','P','G'), 30, Size(800,600), true);
    for(int x = 0; x < 200; x++){ //two-hundred frames
        transformation(pointsSquare, 2); //transform the square
        transformation(pointsTriangle, 2); //transform the triangle
        frame(outputVideo, pointsSquare, pointsTriangle);
    }
    outputVideo.release();
}

int main(){
    part2();
}