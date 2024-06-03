// import opencv and things for io
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <iterator>

// import the namespace
using namespace std;
using namespace cv;

Mat rotationX(double thetha){
    thetha = thetha * 3.14159265358979323846 / 180;
    Mat R_mat = Mat(4, 4, CV_64F, Scalar::all(0));
    R_mat.at<double>(0, 0) = 1;
    R_mat.at<double>(1, 1) = cos(thetha);
    R_mat.at<double>(1, 2) = -sin(thetha);
    R_mat.at<double>(2, 1) = sin(thetha);
    R_mat.at<double>(2, 2) = cos(thetha);
    R_mat.at<double>(3, 3) = 1;
    return R_mat;

}

Mat rotationY(double thetha){
    thetha = thetha * 3.14159265358979323846 / 180;
    Mat R_mat = Mat(4, 4, CV_64F, Scalar::all(0));
    R_mat.at<double>(0,0) = cos(thetha);
    R_mat.at<double>(0,2) = -sin(thetha);
    R_mat.at<double>(1,1) = 1;
    R_mat.at<double>(2,0) = sin(thetha);
    R_mat.at<double>(2,2) = cos(thetha);
    return R_mat;

}

Mat rotationZ(double thetha){
    thetha = thetha * 3.14159265358979323846 / 180;
    Mat R_mat = Mat(4, 4, CV_64F, Scalar::all(0));
    R_mat.at<double>(0,0) = cos(thetha);
    R_mat.at<double>(0,1) = -sin(thetha);
    R_mat.at<double>(1,0) = sin(thetha);
    R_mat.at<double>(1,1) = cos(thetha);
    return R_mat;

}

Mat scale(double Sx, double Sy, double Sz){
    Mat S_mat = Mat(4, 4, CV_64F, Scalar::all(0));
    S_mat.at<double>(0, 0) = Sx;
    S_mat.at<double>(1, 1) = Sy;
    S_mat.at<double>(2, 2) = Sz;
    S_mat.at<double>(3, 3) = 1;
    return S_mat;

}

Mat translation(double Tx, double Ty, double Tz){
    Mat T_mat = Mat(4, 4, CV_64F, Scalar::all(0));
    T_mat.at<double>(0, 0) = 1;
    T_mat.at<double>(0, 3) = Tx;
    T_mat.at<double>(1, 1) = 1;
    T_mat.at<double>(1, 3) = Ty;
    T_mat.at<double>(2, 2) = 1;
    T_mat.at<double>(2, 3) = Tz;
    T_mat.at<double>(3, 3) = 1;
    return T_mat;
}

vector<Mat> cube_rotating(){
    vector<Mat> cube;
    cube.push_back((Mat_ < double > (4, 1) << 1, 1, 1, 1));
    cube.push_back((Mat_ < double > (4, 1) << 1, -1, 1, 1));
    cube.push_back((Mat_ < double > (4, 1) << -1, -1, 1, 1));
    cube.push_back((Mat_ < double > (4, 1) << -1, 1, 1, 1));
    cube.push_back((Mat_ < double > (4, 1) << 1, 1, -1, 1));
    cube.push_back((Mat_ < double > (4, 1) << 1, -1, -1, 1));
    cube.push_back((Mat_ < double > (4, 1) << -1, -1, -1, 1));
    cube.push_back((Mat_ < double > (4, 1) << -1, 1, -1, 1));
    return cube;

}

Mat makeframe(vector<Mat> o){
    Mat frame(600, 800, CV_8UC3, Scalar(0, 0, 0)); //add 399 to x and 299 to y
    vector<Point> points;
    //get the points
    for(int i = 0; i < o.size(); i++){
        points.push_back(Point(o[i].at<double>(0, 0) +399, o[i].at<double>(1, 0)+299));
    }
    // for(Point p: points){
    //     circle(frame, p, 3, Scalar(0,0, 255), -1, 8, 0);
    // }
    //for each point draw a circle of radius 3 and each having a different color
    circle(frame, points[0], 3, Scalar(0, 0, 255), -1, 8, 0);
    circle(frame, points[1], 3, Scalar(0, 255, 0), -1, 8, 0);
    circle(frame, points[2], 3, Scalar(255, 0, 0), -1, 8, 0);
    circle(frame, points[3], 3, Scalar(255, 255, 0), -1, 8, 0);
    circle(frame, points[4], 3, Scalar(255, 0, 255), -1, 8, 0);
    circle(frame, points[5], 3, Scalar(0, 255, 255), -1, 8, 0);
    circle(frame, points[6], 3, Scalar(255, 255, 255), -1, 8, 0);
    circle(frame, points[7], 6, Scalar(255, 255, 255), -1, 8, 0);

    return frame;
}

vector<Mat> addframe(vector<Mat> o, double Tx, double Ty, double Tz, double Sx, double Sy, double Sz, double thetax, double thetay, double thetaz){
    Mat R_mat = rotationY(5);
    Mat T_mat = translation(Tx, Ty, Tz);
    Mat S_mat = scale(Sx, Sy, Sz);
    Mat M_mat = T_mat * R_mat * S_mat;
    Mat O_mat = Mat(4, 4, CV_64F, Scalar::all(0));
    O_mat.at<double>(0, 0) = 1;
    O_mat.at<double>(1, 1) = 1;
    O_mat.at<double>(3, 3) = 1;
    vector<Mat> cube;
    for(int i = 0; i < o.size(); i++){
        Mat temp = M_mat * o[i];
        cube.push_back(O_mat * temp);
    }
    return cube;
}

void part1(){
    vector<Mat> cube = cube_rotating();
    double fps = 30;
    VideoWriter video;
    Size size = Size(800, 600);
    video.open("rotation.avi", VideoWriter::fourcc('M', 'J', 'P', 'G'), fps, size, true);
    ofstream myfile;
    myfile.open("cords.txt");
    cube = addframe(cube, 0, 0, 0, 100, 100, 100, 5, 0, 0);
    for(int i = 0; i < 360; i++){
        cube = addframe(cube, 0, 0, 0, 1, 1, 1, 0, (10), 0);
        // cout << cube[0] << endl;
        // print the cords to a txt file
        for(int j = 0; j < cube.size(); j++){
            myfile << cube[j].at<double>(0, 0) << " " << cube[j].at<double>(1, 0) << " " << cube[j].at<double>(2, 0) << endl;
        }
        myfile << endl;
        Mat frame = makeframe(cube);
        video.write(frame);
    }
    video.release();
}

int main(){
    part1();
    return 0;
}
