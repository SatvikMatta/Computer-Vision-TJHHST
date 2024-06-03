#include <stdio.h>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <math.h>
#include <cmath>

using namespace std;

using namespace cv;

Point convertToTwoD(Mat_<double> currPoint, Mat_<double> origin, Mat_<double> w1, Mat_<double> w2){
    double d = currPoint.at<double>(0,0);
    double h = currPoint.at<double>(0,1);
    double a = origin.at<double>(0,0);
    double b = w1.at<double>(0,0);
    double c = w2.at<double>(0,0);
    double e = origin.at<double>(0,1);
    double f = w1.at<double>(0,1);
    double g = w2.at<double>(0,1);
    double v = (a-((b*e)/f)+((b*h)/f)-d)/(((b*g)/f)-c);
    double u = (e+(g*v)-h)/(f*-1);
    return Point(u, v);
}
vector<Mat> makePerpWithMagOne(Mat_<double> v1, Mat_<double> v2){
    vector<Mat> holder;
    Mat_<double> newV1 = (v1 - ((v1.dot(v2)/v2.dot(v2))*v2));
    Mat_<double> newV2 = (1/sqrt( pow(v2.at<double>(0,0), 2) + pow(v2.at<double>(0,1), 2) + pow(v2.at<double>(0,2), 2) ) ) * v2;
    newV1 = (1/sqrt( pow(newV1.at<double>(0,0), 2) + pow(newV1.at<double>(0,1), 2) + pow(newV1.at<double>(0,2), 2) ) ) * newV1;
    holder.push_back(newV1);
    holder.push_back(newV2);
    return holder;
}

Mat threeDProjectPoint(Mat_<double> currPoint, Mat_<double> newEye, Mat_<double> a, Mat_<double> n){
    Mat_<double> numerator = a - newEye;
    Mat_<double> denominator = currPoint - newEye;
    double firstScalar = (numerator.dot(n) / denominator.dot(n));
    denominator = (currPoint-newEye) * firstScalar;
    denominator = denominator + newEye;
    return denominator;
}

Mat_<double> calculateW1(vector<Mat> currPoints, vector<double> eye, Mat_<double> a, Mat_<double> n){
    vector<Mat> newCurrPoints; //converts all of the 4x1 homogenous coordinates into 3x1 matrices
    for(int x = 0; x < (int)currPoints.size(); x++){
        newCurrPoints.push_back((Mat_<double>(3,1) << currPoints[x].at<double>(0,0), currPoints[x].at<double>(0,1), currPoints[x].at<double>(0,2)));
    }
    vector<Point> twoDPoints;
    vector<Mat> threeDProjections;
    Mat_<double> newEye = (Mat_<double>(3,1) << eye[0], eye[1], eye[2]); //converts the eye into a matrix
    Mat_<double> origin = (Mat_<double>(3,1) << 0, 0, 0); //sets matrix origin
    origin = threeDProjectPoint(origin, newEye, a, n);
    //project the points onto the current plane
    for(int x = 0; x < (int)newCurrPoints.size(); x++){
        threeDProjections.push_back(threeDProjectPoint(newCurrPoints[x], newEye, a, n));
    }
    Mat_<double> w1 = threeDProjections[1] - threeDProjections[0];
    for(int i = 0; i < (int)threeDProjections.size(); i++){
        cout << threeDProjections[i] << " " << i << endl;
    }
    return w1;
}

Mat_<double> calculateW2(vector<Mat> currPoints, vector<double> eye, Mat_<double> a, Mat_<double> n){
    vector<Mat> newCurrPoints; //converts all of the 4x1 homogenous coordinates into 3x1 matrices
    for(int x = 0; x < (int)currPoints.size(); x++){
        newCurrPoints.push_back((Mat_<double>(3,1) << currPoints[x].at<double>(0,0), currPoints[x].at<double>(0,1), currPoints[x].at<double>(0,2)));
    }
    vector<Point> twoDPoints;
    vector<Mat> threeDProjections;
    Mat_<double> newEye = (Mat_<double>(3,1) << eye[0], eye[1], eye[2]); //converts the eye into a matrix
    Mat_<double> origin = (Mat_<double>(3,1) << 0, 0, 0); //sets matrix origin
    origin = threeDProjectPoint(origin, newEye, a, n);
    //project the points onto the current plane
    for(int x = 0; x < (int)newCurrPoints.size(); x++){
        threeDProjections.push_back(threeDProjectPoint(newCurrPoints[x], newEye, a, n));
    }
    Mat_<double> w2 = threeDProjections[1] - threeDProjections[2];
    return w2;
}

vector<Point> aPerspectiveProjection(vector<Mat> currPoints, vector<double> eye, Mat_<double> a, Mat_<double> n, Mat_<double> w1, Mat_<double> w2){
    vector<Mat> newCurrPoints; //converts all of the 4x1 homogenous coordinates into 3x1 matrices
    for(int x = 0; x < (int)currPoints.size(); x++){
        newCurrPoints.push_back((Mat_<double>(3,1) << currPoints[x].at<double>(0,0), currPoints[x].at<double>(0,1), currPoints[x].at<double>(0,2)));
    }
    vector<Point> twoDPoints;
    vector<Mat> threeDProjections;
    Mat_<double> newEye = (Mat_<double>(3,1) << eye[0], eye[1], eye[2]); //converts the eye into a matrix
    Mat_<double> origin = (Mat_<double>(3,1) << 0, 0, 0); //sets matrix origin
    origin = threeDProjectPoint(origin, newEye, a, n);
    for(int x = 0; x < (int)newCurrPoints.size(); x++){
        Mat tpr = threeDProjectPoint(newCurrPoints[x], newEye, a, n);
        threeDProjections.push_back(tpr);
    }
    vector<Mat> holder = makePerpWithMagOne(w1, w2);
    w1 = holder[0]; //true w1
    w2 = holder[1]; //true w2
    for(int x = 0; x < (int)threeDProjections.size(); x++){
        twoDPoints.push_back(convertToTwoD(threeDProjections[x], origin, w1, w2));
    }
    return twoDPoints;
}

bool check(vector<Mat> currPoints, Mat_<double> a, Mat_<double> n, vector<double> eye){
    vector<Mat> newCurrPoints; //converts all of the 4x1 homogenous coordinates into 3x1 matrices
    for(int x = 0; x < (int)currPoints.size(); x++){
        newCurrPoints.push_back((Mat_<double>(3,1) << currPoints[x].at<double>(0,0), currPoints[x].at<double>(0,1), currPoints[x].at<double>(0,2)));
    }
    Mat_<double> newEye = (Mat_<double>(3,1) << eye[0], eye[1], eye[2]); //converts the eye into a matrix
    for(int x = 0; x < (int)newCurrPoints.size(); x++){
        Mat_<double> temp = newCurrPoints[x] - a;
        double product = temp.dot(n);
        if(product > 0){
            cout << "Does Not Work" << "\n";
            return false; 
        }
    }
    Mat_<double> holder = newEye - a;
    double product2 = holder.dot(n);
    if(product2 < 0){
        cout << "Does Not Work" << "\n";
        return false;
    }
    return true;
}

void frame(VideoWriter outputVideo, vector<Mat> points, vector<Mat> pointsTri, Mat_<double> a, Mat_<double> n, vector<double> eye, Mat_<double> w1, Mat_<double> w2){
    Mat currFrame = cv::Mat::zeros(600,800,CV_64FC3);
    check(points, a, n, eye);
    check(pointsTri, a, n, eye);
    map<int,vector<int> > squareMap;
    map<int,vector<int> > squareMapPrint;
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
    squareMapPrint[0] = vector<int>{4,3,1};
    squareMapPrint[1] = vector<int>{2,5};
    squareMapPrint[2] = vector<int>{3,6};
    squareMapPrint[3] = vector<int>{7};
    squareMapPrint[4] = vector<int>{5,7};
    squareMapPrint[5] = vector<int>{6};
    squareMapPrint[6] = vector<int>{7};
    vector<Point> twoDPointsSquare;
    vector<Point> twoDPointsTri;
    twoDPointsSquare = aPerspectiveProjection(points, eye, a, n, w1, w2);
    twoDPointsTri = aPerspectiveProjection(pointsTri, eye, a, n, w1, w2);
        
    //square
    for(int x = 0; x < (int)points.size(); x++){
        twoDPointsSquare[x] = Point(twoDPointsSquare[x].x + 400, twoDPointsSquare[x].y + 300);
    }
    //triangle
    for(int x = 0; x < (int)pointsTri.size(); x++){
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
    Mat_<double> a = (Mat_<double>(3,1) << 500, 200, 300); //sets matrix a
    Mat_<double> n = (Mat_<double>(3,1) << 5, 2, 3); //sets matrix n
    vector<double> eye = vector<double>{1000, 50, 400};
    Mat_<double> w1 = calculateW1(pointsSquare, eye, a, n);
    Mat_<double> w2 = calculateW2(pointsSquare, eye, a, n);
    //make the video with 200 frames
    VideoWriter outputVideo;
    outputVideo.open("rotation.avi", VideoWriter::fourcc('M','J','P','G'), 30, Size(800,600), true);
    for(int x = 0; x < 200; x++){ //two-hundred frames
        transformation(pointsSquare, 2); //transform the square
        transformation(pointsTriangle, 2); //transform the triangle
        frame(outputVideo, pointsSquare, pointsTriangle, a, n, eye, w1, w2);
    }
    outputVideo.release();
}

int main(){
    part2();
}

