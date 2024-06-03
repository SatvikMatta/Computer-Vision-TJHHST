// Satvik Matta
// 5/23/2023

#include <stdio.h>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <math.h>
#include <cmath>
#include <fstream>

using namespace cv;
using namespace std;

vector<Point> perspectiveProjection(vector<Mat> currPoints, vector<double> frame, vector<double> eye){
    vector<Point> pointH;
    for(int i = 0; i < (int) currPoints.size(); i++){
        double t = (frame[0]-eye[0]);
        t/= (currPoints[i].at<double>(0,0) - eye[0]);
        double u = (t*currPoints[i].at<double>(0,1)) + eye[1] - (eye[1]*t);
        double v = (t*currPoints[i].at<double>(0,2)) + eye[2] - (eye[2]*t);
        pointH.push_back(Point(u,v));
    }
    return pointH;
}

void frame(VideoWriter out, vector<Mat> pointsS, vector<Mat> pointsT, ofstream &myfile, bool count, ofstream &myfile2){
    Mat currentFrame = Mat::zeros(600,800,CV_64FC3);
    map<int, vector<int>> sMap;
    map<int, vector<int>> tMap;
    vector<double> plane = {500,0,0};
    vector<double> eye = {800,0,0};

    sMap[0] = {2,4,1};
    sMap[1] = {0,5,3};
    sMap[2] = {6,0,3};
    sMap[3] = {7,1,2};
    sMap[4] = {0,6,5};
    sMap[5] = {1,7,4};
    sMap[6] = {7,2,4};
    sMap[7] = {5,6,3};
    tMap[0] = vector<int>{1,2,3};
    tMap[1] = vector<int>{0,2,3};
    tMap[2] = vector<int>{1,0,3};
    tMap[3] = vector<int>{1,2,0};
    vector<Point> twopointT = perspectiveProjection(pointsT, plane, eye);
    vector<Point> twopointS = perspectiveProjection(pointsS, plane, eye);
    for(int i=0; i < (int)pointsS.size(); i++){
        // twopointS.push_back(Point((pointsS[i].at<double>(0,0) * 100) + 400, (pointsS[i].at<double>(0,1) * 100) + 300));
        twopointS[i] = Point(twopointS[i].x + 400, twopointS[i].y + 300);
        if (i < (int)pointsS.size() - 1 && count){
            myfile << "(" << ((pointsS[i].at<double>(0,0)) + 400) << "," << ((pointsS[i].at<double>(0,1)) + 300) << "," <<((pointsS[i].at<double>(0,1)))<< "), ";
            myfile2 << "(" << (twopointS[i].x+400) << "," << (twopointS[i].y + 300) << "), ";
        }
        else if(count){
            myfile << "(" << ((pointsS[i].at<double>(0,0)) + 400) << "," << ((pointsS[i].at<double>(0,1)) + 300) << "," << ((pointsS[i].at<double>(0,1)))<< ")" << endl;
            myfile2 << "(" << (twopointS[i].x+400) << "," << (twopointS[i].y + 300) << ")" << endl;
        }
    }
    for(int i=0; i < (int)pointsT.size(); i++){
        // twopointT.push_back(Point((pointsT[i].at<double>(0,0) * 150) + 400, (pointsT[i].at<double>(0,1) * 150) + 300));
        twopointT[i] = Point(twopointT[i].x + 400, twopointT[i].y + 300);
    }
    for(int i = 0; i<(int)pointsS.size(); i++){
        //[] for auto add key, .at() for no auto add key
        vector<int> s = sMap.at(i);
        for(int j = 0; j < (int)s.size(); j++){
            int c = s.at(j);
            line(currentFrame, twopointS.at(i), twopointS.at(c), Scalar(255,0,0), 2, LINE_AA);
        }
    }
    for(int i = 0; i < (int)pointsS.size(); i++){
        circle(currentFrame, twopointS.at(i), 5, Scalar(0,0,255), -1);
    }
    for(int i=  0; i < (int)pointsT.size(); i++){
        circle(currentFrame, twopointT.at(i), 5, Scalar(0,0,255), -1);
    }
    for(int i = 0; i<(int)pointsT.size(); i++){
        vector<int> t = tMap.at(i);
        for(int j = 0; j < (int)t.size(); j++){
            int c = t.at(j);
            line(currentFrame, twopointT.at(i), twopointT.at(c), Scalar(0,255,0), 2, LINE_AA);
        }
    }
    Mat image;
    currentFrame.convertTo(image, CV_8UC3);
    out.write(image);
}

void transformation(vector<Mat> points, double theta){
    double radians = theta * (M_PI / 180);
    double md[] = {1,0,0,0,0, cos(radians), sin(radians), 0, 0, -1*sin(radians), cos(radians), 0, 0, 0, 0, 1};
    Mat rotation = Mat(4,4,CV_64FC1, md); 
    for(int i = 0; i < (int)points.size(); i++){
        points[i] = rotation*points[i];
    }
    double md2[] = {cos(radians), 0, -1*sin(radians), 0, 0, 1, 0, 0, sin(radians), 0, cos(radians), 0, 0, 0, 0, 1};
    Mat rotation2 = Mat(4,4,CV_64FC1, md2);
    for(int i = 0; i < (int)points.size(); i++){
        points[i] = rotation2*points[i];
    }
}

void part2(){
    ofstream myfile;
    myfile.open("coordinates.txt");
    ofstream myfile2;
    myfile2.open("coordinates2d.txt");
    vector<Mat> pointsS;
    Mat temp = (Mat_<double>(4,1) << 1,1,1,1);
    pointsS.push_back((Mat_<double>(4,1) << 1,1,1,1));
    pointsS.push_back((Mat_<double>(4,1) << 1,1,-1,1));
    pointsS.push_back((Mat_<double>(4,1) << 1,-1,1,1));
    pointsS.push_back((Mat_<double>(4,1) << 1,-1,-1,1));
    pointsS.push_back((Mat_<double>(4,1) << -1,1,1,1));
    pointsS.push_back((Mat_<double>(4,1) << -1,1,-1,1));
    pointsS.push_back((Mat_<double>(4,1) << -1,-1,1,1));
    pointsS.push_back((Mat_<double>(4,1) << -1,-1,-1,1));

    for(int i = 0; i < (int)pointsS.size(); i++){
        pointsS[i].at<double>(0,0) = pointsS[i].at<double>(0,0) * 170;
        pointsS[i].at<double>(0,1) = pointsS[i].at<double>(0,1) * 170;
        pointsS[i].at<double>(0,2) = pointsS[i].at<double>(0,2) * 170;
    }

    vector<Mat> pointsT;
    pointsT.push_back((Mat_<double>(4,1) << (1/sqrt(3)), 0, 0, 1));
    pointsT.push_back((Mat_<double>(4,1) << 0, 0, (2/sqrt(6)), 1));
    pointsT.push_back((Mat_<double>(4,1) << ((-1*sqrt(3))/6), 0.5, 0, 1));
    pointsT.push_back((Mat_<double>(4,1) << ((-1*sqrt(3))/6), -0.5, 0, 1));

    for(int i=0; i< (int)pointsT.size(); i++){
        pointsT[i].at<double>(0,0) = pointsT[i].at<double>(0,0) * 175;
        pointsT[i].at<double>(0,1) = pointsT[i].at<double>(0,1) * 175;
        pointsT[i].at<double>(0,2) = pointsT[i].at<double>(0,2) * 175;
    }

    VideoWriter output;
    output.open("rotation.avi", VideoWriter::fourcc('M','J','P','G'), 30, Size(800,600), true);
    bool count = true;
    for(int i = 0; i<200; i++){
        transformation(pointsS, 3);
        transformation(pointsT, 3);
        if(i == 4){
            count = false;
        }
        frame(output, pointsS, pointsT, myfile, count, myfile2);
    }
}

int main(int argc, char** argv){
    part2();
    return 0;
}