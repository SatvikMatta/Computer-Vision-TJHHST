//Satvik Matta
//5/29/2023

#include <stdio.h>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <math.h>
#include <cmath>
#include <fstream>
#include <string>
#include <sstream>

using namespace cv;
using namespace std;

Point convert2d(Mat_<double> currPoint, Mat_<double> origin, Mat_<double> w1, Mat_<double> w2){
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
    // cout << u << " " << v << endl;
    return Point(u, v);
}

Mat tDPP(Mat_<double> currentPoint, Mat_<double> newEye, Mat_<double> a, Mat_<double> n){
    Mat_<double> numerator = a - newEye;
    Mat_<double> denominator = currentPoint - newEye;
    double firstScalar = (numerator.dot(n) / denominator.dot(n));
    denominator = (currentPoint-newEye) * firstScalar;
    denominator = denominator + newEye;
    // cout << denominator << endl;
    return denominator;
}

vector<Mat> makePerpNormal(Mat_<double> v1, Mat_<double> v2){
    vector<Mat> hold;
    Mat_<double> newV1 = (v1 - ((v1.dot(v2)/v2.dot(v2))*v2));
    Mat_<double> newV2 = (1/sqrt( pow(v2.at<double>(0,0), 2) + pow(v2.at<double>(0,1), 2) + pow(v2.at<double>(0,2), 2) ) ) * v2;
    newV1 = (1/sqrt( pow(newV1.at<double>(0,0), 2) + pow(newV1.at<double>(0,1), 2) + pow(newV1.at<double>(0,2), 2) ) ) * newV1;
    hold.push_back(newV1);
    hold.push_back(newV2);
    // cout << newV1 << endl;
    return hold;
}

Mat_<double> calcW1(vector<Mat> currentPoints, vector<double> eye, Mat_<double> a, Mat_<double> n, ofstream& myfile){
    vector<Mat> newPoints;
    for(int i = 0; i < (int) currentPoints.size(); i++){
        newPoints.push_back((Mat_<double>(3,1) << currentPoints[i].at<double>(0,0), currentPoints[i].at<double>(0,1), currentPoints[i].at<double>(0,2)));
    }
    vector<Mat> threeDproj;
    Mat_<double> newEye = (Mat_<double>(3,1) << eye[0], eye[1], eye[2]); 
    Mat_<double> origin = (Mat_<double>(3,1) << 0, 0, 0);
    origin = tDPP(origin, newEye, a, n);
    for(int i = 0; i<(int)newPoints.size(); i++){
        threeDproj.push_back(tDPP(newPoints[i], newEye, a, n));
    }
    //print out all threeDproj
    // for(int i = 0; i < (int)threeDproj.size(); i++){
    //     cout << threeDproj[i] << " " << i << endl;
    // }
    Mat_<double> w1 = threeDproj[3] - threeDproj[7];
    myfile << "pv1 = " << "(" << threeDproj[3].at<double>(0,0) << ", " << threeDproj[3].at<double>(0,1) << ", " << threeDproj[3].at<double>(0,2) << ")" << endl;
    myfile << "pv2 = " << "(" << threeDproj[7].at<double>(0,0) << ", " << threeDproj[7].at<double>(0,1) << ", " << threeDproj[7].at<double>(0,2) << ")" << endl;
    myfile << "pv3 = " << "(" << threeDproj[2].at<double>(0,0) << ", " << threeDproj[2].at<double>(0,1) << ", " << threeDproj[2].at<double>(0,2) << ")" << endl;
    myfile << endl;
    return w1;
}

//check if wrong
Mat_<double> calcW2(vector<Mat> currentPoints, vector<double> eye, Mat_<double> a, Mat_<double> n){
    vector<Mat> newPoints;
    for(int i = 0; i < (int) currentPoints.size(); i++){
        newPoints.push_back((Mat_<double>(3,1) << currentPoints[i].at<double>(0,0), currentPoints[i].at<double>(0,1), currentPoints[i].at<double>(0,2)));
    }
    vector<Mat> threeDproj;
    Mat_<double> newEye = (Mat_<double>(3,1) << eye[0], eye[1], eye[2]); 
    Mat_<double> origin = (Mat_<double>(3,1) << 0, 0, 0);
    origin = tDPP(origin, newEye, a, n);
    for(int i = 0; i<(int)newPoints.size(); i++){
        threeDproj.push_back(tDPP(newPoints[i], newEye, a, n));
    }
    Mat_<double> w2 = threeDproj[2] - threeDproj[3];
    return w2;
}

vector<Point> aPP(vector<Mat> currentPoints, vector<double> eye, Mat_<double> a, Mat_<double> n, Mat_<double> w1, Mat_<double> w2, ofstream& myfile){
    vector<Mat> newPoints;
    for(int i = 0; i < (int)currentPoints.size(); i++){
        newPoints.push_back((Mat_<double>(3,1) << currentPoints[i].at<double>(0,0), currentPoints[i].at<double>(0,1), currentPoints[i].at<double>(0,2)));
    }
    vector<Point> twoDPoints;
    vector<Mat> threeDProjections;
    Mat_<double> newEye = (Mat_<double>(3,1) << eye[0], eye[1], eye[2]);
    Mat_<double> origin = (Mat_<double>(3,1) << 0, 0, 0);
    origin = tDPP(origin, newEye, a, n);
    for(int i = 0; i < (int)newPoints.size(); i++){
        Mat tpr = tDPP(newPoints[i], newEye, a, n);
        threeDProjections.push_back(tpr);
    }
    vector<Mat> holder = makePerpNormal(w1, w2);
    w1 = holder[0];
    w2 = holder[1];
    for(int x = 0; x < (int)threeDProjections.size(); x++){
        twoDPoints.push_back(convert2d(threeDProjections[x], origin, w1, w2));
    }

    myfile << "The w1 and w2 obtained from a and b (these now are perpendicular and of magnitude 1) are:" << endl;
    myfile << "w1 = " << "(" << w1.at<double>(0,0) << ", " << w1.at<double>(1,0) << ", " << w1.at<double>(2,0) << ")" << endl;
    myfile << "w2 = " << "(" << w2.at<double>(0,0) << ", " << w2.at<double>(1,0) << ", " << w2.at<double>(2,0) << ")" << endl;
    myfile << endl;
    return twoDPoints;
}

vector<Point> aPP(vector<Mat> currentPoints, vector<double> eye, Mat_<double> a, Mat_<double> n, Mat_<double> w1, Mat_<double> w2){
    vector<Mat> newPoints;
    for(int i = 0; i < (int)currentPoints.size(); i++){
        newPoints.push_back((Mat_<double>(3,1) << currentPoints[i].at<double>(0,0), currentPoints[i].at<double>(0,1), currentPoints[i].at<double>(0,2)));
    }
    vector<Point> twoDPoints;
    vector<Mat> threeDProjections;
    Mat_<double> newEye = (Mat_<double>(3,1) << eye[0], eye[1], eye[2]);
    Mat_<double> origin = (Mat_<double>(3,1) << 0, 0, 0);
    origin = tDPP(origin, newEye, a, n);
    for(int i = 0; i < (int)newPoints.size(); i++){
        Mat tpr = tDPP(newPoints[i], newEye, a, n);
        threeDProjections.push_back(tpr);
    }
    vector<Mat> holder = makePerpNormal(w1, w2);
    w1 = holder[0];
    w2 = holder[1];
    for(int x = 0; x < (int)threeDProjections.size(); x++){
        twoDPoints.push_back(convert2d(threeDProjections[x], origin, w1, w2));
    }

    return twoDPoints;
}

bool check(vector<Mat> currentPoints, Mat_<double> a, Mat_<double> n, vector<double> eye){
    vector<Mat> newPoints; 
    for(int i = 0; i < (int)currentPoints.size(); i++){
        newPoints.push_back((Mat_<double>(3,1) << currentPoints[i].at<double>(0,0), currentPoints[i].at<double>(0,1), currentPoints[i].at<double>(0,2)));
    }
    Mat_<double> newEye = (Mat_<double>(3,1) << eye[0], eye[1], eye[2]);
    for(int i = 0; i < (int)newPoints.size(); i++){
        Mat_<double> temp = newPoints[i] - a;
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

void frame(VideoWriter out, vector<Mat> pointsS, vector<Mat> pointsT, Mat_<double> a, Mat_<double> n, vector<double> eye, Mat_<double> w1,  Mat_<double> w2, ofstream &myfile, bool count, ofstream &myfile2, ofstream &myfile3, bool count2){
    Mat currentFrame = Mat::zeros(600,800,CV_64FC3);
    check(pointsS, a, n, eye);
    check(pointsT, a, n, eye);
    map<int, vector<int>> sMap;
    map<int, vector<int>> tMap;


    // vector<double> plane = {500,0,0};
    // vector<double> eye = {800,0,0};

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


    vector<Point> twopointT = aPP(pointsT, eye, a, n, w1, w2);
    vector<Point> twopointS;
    if(count2){
        twopointS = aPP(pointsS, eye, a, n, w1, w2, myfile3);
    }
    else{
        twopointS = aPP(pointsS, eye, a, n, w1, w2);
    }

    if(count2){
        myfile3 << "The center of the cube in first frame and its projection are:" << endl;
        myfile3 << "center  = (" << ((a.at<double>(0,0)) + 400) << "," << ((a.at<double>(0,1)) + 300) << "," <<((a.at<double>(0,1)))<< ")" << endl;
        myfile3 << "p0 = (" << (twopointS[0].x+400) << "," << (twopointS[0].y + 300) << ")" << endl;
        myfile3 << endl;

        myfile3 << "I Finished 8.3!" << endl;
        myfile3 << "This Log File is wayyy to overcomplicated for me to continue implementing it. Srry :(" << endl;
    }

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

    for(int i = 0; i<(int)twopointS.size(); i++){
        //[] for auto add key, .at() for no auto add key
        vector<int> t = sMap.at(i); 
        for(int j = 0; j < (int)t.size(); j++){
            int c = t.at(j);
            line(currentFrame, twopointS.at(i), twopointS.at(c), Scalar(255, 0, 0), 2, LINE_AA);
        }
    }
    for(int i = 0; i < (int)twopointS.size(); i++){
        circle(currentFrame, twopointS.at(i), 5, Scalar(0,0,255), -1);
    }
    for(int i=  0; i < (int)twopointT.size(); i++){
        circle(currentFrame, twopointT.at(i), 5, Scalar(0,0,255), -1);
    }
    for(int i = 0; i<(int)twopointT.size(); i++){
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

vector<string> split(string str, char delimiter){
    vector<string> internal;
    stringstream ss(str);
    string tok;
    while(getline(ss, tok, delimiter)){
        internal.push_back(tok);
    }
    return internal;
}

void part3(int argc, char** argv){
    ofstream myfile;
    myfile.open("coordinates.txt");

    ofstream myfile2;
    myfile2.open("coordinates2d.txt");

    ofstream myfile3;
    myfile3.open("log.txt");

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
        pointsS[i].at<double>(0,0) = pointsS[i].at<double>(0,0) * 150;
        pointsS[i].at<double>(0,1) = pointsS[i].at<double>(0,1) * 150;
        pointsS[i].at<double>(0,2) = pointsS[i].at<double>(0,2) * 150;
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

    // ./l083 -a "(700.5,200.1,100.12)" -n "(1,2,3.12)" -e "(1000,900.23,100.1156)"

    Mat_<double> a = (Mat_<double>(3,1) << 700, 200, 100);
    Mat_<double> n = (Mat_<double>(3,1) << 1.1, 2.1, 3.2);
    vector<double> eye = vector<double>{1000.1, 900.231, 100.156};

    for(int i = 0; i < argc; i++){
        if(strcmp(argv[i], "-a") == 0){
            string atemp = argv[i+1];
            // cout << atemp << endl;
            //remove the parentheses
            atemp.erase(atemp.begin());
            atemp.erase(atemp.end()-1);
            //split the string by commas
            vector<string> astring = split(atemp, ',');
            a.at<double>(0,0) = stod(astring[0]);
            a.at<double>(1,0) = stod(astring[1]);
            a.at<double>(2,0) = stod(astring[2]);
            //print the Mat a
            // cout << "a: " << a << endl;
        }
        if(strcmp(argv[i], "-n") == 0){
            string ntemp = argv[i+1];
            // cout << ntemp << endl;
            //remove the parentheses
            ntemp.erase(ntemp.begin());
            ntemp.erase(ntemp.end()-1);
            //split the string by commas
            vector<string> nstring = split(ntemp, ',');
            n.at<double>(0,0) = stod(nstring[0]);
            n.at<double>(1,0) = stod(nstring[1]);
            n.at<double>(2,0) = stod(nstring[2]);
            //print the Mat n
            // cout << "n: " << n << endl;
        }
        if(strcmp(argv[i], "-e") == 0){
            string etemp = argv[i+1];
            // cout << etemp << endl;
            //remove the parentheses
            etemp.erase(etemp.begin());
            etemp.erase(etemp.end()-1);
            //split the string by commas
            vector<string> estring = split(etemp, ',');
            eye[0] = stod(estring[0]);
            eye[1] = stod(estring[1]);
            eye[2] = stod(estring[2]);
            //print the vector eye
            // cout << "eye: " << eye[0] << " " << eye[1] << " " << eye[2] << endl;
        }
    }

    myfile3 << "The plane defined by (x-a)*n =0 is:" << endl;
    myfile3 << "  a = " << "(" << a.at<double>(0,0) << ", " << a.at<double>(1,0) << ", " << a.at<double>(2,0) << ")" << endl;
    myfile3 << "  n = " << "(" << n.at<double>(0,0) << ", " << n.at<double>(1,0) << ", " << n.at<double>(2,0) << ")" << endl;
    myfile3 << "The eye e is: " << endl << "  e = " << "(" << eye[0] << ", " << eye[1] << ", " << eye[2] << ")" << endl;

    myfile3 << "Vertices I used to create he 2d coordinate system and their projections are: " << endl;
    myfile3 << "v1 = " << "(" << pointsS[3].at<double>(0,0) << ", " << pointsS[3].at<double>(0,1) << ", " << pointsS[3].at<double>(0,2) << ")" << endl;
    myfile3 << "v2 = " << "(" << pointsS[7].at<double>(0,0) << ", " << pointsS[7].at<double>(0,1) << ", " << pointsS[7].at<double>(0,2) << ")" << endl;
    myfile3 << "v3 = " << "(" << pointsS[2].at<double>(0,0) << ", " << pointsS[2].at<double>(0,1) << ", " << pointsS[2].at<double>(0,2) << ")" << endl;
    myfile3 << endl;

    Mat_<double> w1 = calcW1(pointsS, eye, a, n, myfile3);
    Mat_<double> w2 = calcW2(pointsS, eye, a, n);

    myfile3 << "The 2 vectors a and b that are in plane are:" << endl;
    myfile3 << "a = pv1-pv2 = " << "(" << w1.at<double>(0,0) << ", " << w1.at<double>(1,0) << ", " << w1.at<double>(2,0) << ")" << endl;
    myfile3 << "b = pv3-pv1 = " << "(" << w2.at<double>(0,0) << ", " << w2.at<double>(1,0) << ", " << w2.at<double>(2,0) << ")" << endl;
    myfile3 << endl;

    VideoWriter output;
    output.open("rotation.avi", VideoWriter::fourcc('M','J','P','G'), 30, Size(800,600), true);
    bool count = true;
    for(int i = 0; i<200; i++){
        transformation(pointsS, 3);
        transformation(pointsT, 3);
        if(i == 4){
            count = false;
        }
        frame(output, pointsS, pointsT, a, n, eye, w1, w2, myfile, count, myfile2, myfile3, i == 0);
    }
    output.release();
}

int main(int argc, char** argv){
    part3(argc, argv);
    return 0;
}



