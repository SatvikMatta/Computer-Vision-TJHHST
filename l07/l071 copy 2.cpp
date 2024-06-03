//Satvik Matta
//March 10, 2023

// This program will read in an image and use openCV to find the coins in the image
// and then draw a circle around them.

//It will also classify the coins as pennies, nickels, dimes, quarters, half dollars, and dollars.

#include <iostream>
#include <fstream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;

void image3(){
    //Read in the image
    cv::Mat image = cv::imread("coins3__.jpg", cv::IMREAD_COLOR);
    //convert to grayscale
    cv::Mat gray;
    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);

    //write the grayscale image to a file
    cv::imwrite("imageg.jpg", gray);

    //blur the image with a 5x5 kernel
    cv::Mat blur;
    cv::GaussianBlur(gray, blur, cv::Size(5,5), 0);

    //erode the image
    cv::Mat erode;
    cv::erode(blur, erode, cv::Mat(), cv::Point(-1,-1), 15);

    //dilate the image
    cv::Mat dilate;
    cv::dilate(erode, dilate, cv::Mat(), cv::Point(-1,-1), 15);

    cv::Mat contours;
    cv::Canny(blur, contours, 70, 200);
    cv::imwrite("imagef.jpg", contours);

    // //HoughCircles
    std::vector<cv::Vec3f> circles;
    cv::HoughCircles(dilate, circles, cv::HOUGH_GRADIENT, 2, 50, 121, 61, 75, 180);

    //draw the circles
    for (size_t i = 0; i < circles.size(); i++){
        cv::Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
        int radius = cvRound(circles[i][2]);
        cv::circle(image, center, radius, cv::Scalar(0, 0, 255), 3);
    }

    cv::imwrite("imageCircles.jpg", image);

    int average_radius = 0;
    for(int i = 0; i < circles.size(); i++){
        average_radius += circles[i][2];
    }
    average_radius = average_radius / circles.size();

    cout << "Average radius: " << average_radius << endl;

    //classify the coins by ratio to the average radius
    //make a vector of the coins for each coin
    vector<cv::Vec3f> coinP;
    vector<cv::Vec3f> coinN;
    vector<cv::Vec3f> coinD;
    vector<cv::Vec3f> coinQ;
    vector<cv::Vec3f> coinDol;
    for(int i = 0; i < circles.size(); i++){
        int x = circles[i][0];
        int y = circles[i][1];
        int b = image.at<cv::Vec3b>(y,x)[0];
        int g = image.at<cv::Vec3b>(y,x)[1];
        int r = image.at<cv::Vec3b>(y,x)[2];
        if(circles[i][2] < average_radius * 1.01 && r > (g+b)/2 ){
            if(r> 120){
                coinP.push_back(circles[i]);
            }
            else if(r > 60 && r < (g+b)/2 + 10){
                coinN.push_back(circles[i]);
            }
            else if(g < 100 && b < 100){
                coinP.push_back(circles[i]);
            }
            else{
                coinN.push_back(circles[i]);
            }
        }
        else if(circles[i][2] < average_radius * 0.9  ){
            coinD.push_back(circles[i]);
        }
        else if(circles[i][2] < average_radius * 1.134){
            coinN.push_back(circles[i]);
        }
        else if(circles[i][2] < average_radius * 1.3){
            coinQ.push_back(circles[i]);
        }
        else{
            coinDol.push_back(circles[i]);
        }
    }
    //draw the circles for each coin type in a different color
    //pennies are red
    for (size_t i = 0; i < coinP.size(); i++){
        cv::Point center(cvRound(coinP[i][0]), cvRound(coinP[i][1]));
        int radius = cvRound(coinP[i][2]);
        cv::circle(image, center, radius, cv::Scalar(0, 0, 255), 3);
    }
    //nickels are yellow
    for (size_t i = 0; i < coinN.size(); i++){
        cv::Point center(cvRound(coinN[i][0]), cvRound(coinN[i][1]));
        int radius = cvRound(coinN[i][2]);
        cv::circle(image, center, radius, cv::Scalar(0, 255, 255), 3);
    }
    //dimes are blue
    for (size_t i = 0; i < coinD.size(); i++){
        cv::Point center(cvRound(coinD[i][0]), cvRound(coinD[i][1]));
        int radius = cvRound(coinD[i][2]);
        cv::circle(image, center, radius, cv::Scalar(255, 0, 0), 3);
    }
    //quarters are purple
    for (size_t i = 0; i < coinQ.size(); i++){
        cv::Point center(cvRound(coinQ[i][0]), cvRound(coinQ[i][1]));
        int radius = cvRound(coinQ[i][2]);
        cv::circle(image, center, radius, cv::Scalar(255, 0, 255), 3);
    }
    //dollars are pink
    for (size_t i = 0; i < coinDol.size(); i++){
        cv::Point center(cvRound(coinDol[i][0]), cvRound(coinDol[i][1]));
        int radius = cvRound(coinDol[i][2]);
        cv::circle(image, center, radius, cv::Scalar(255, 192, 203), 3);
    }
    
    cv::imwrite("imageCoins.jpg", image);

    //print the number of each coin and the total value
    cout << "Pennies: " << coinP.size() << endl;
    cout << "Nickels: " << coinN.size() << endl;
    cout << "Dimes: " << coinD.size() << endl;
    cout << "Quarters: " << coinQ.size() << endl;
    cout << "Half Dollars" << 0 << endl;
    cout << "Dollars: " << coinDol.size() << endl;
    cout << "Total value: $" << coinP.size() * 0.01 + coinN.size() * 0.05 + coinD.size() * 0.1 + coinQ.size() * 0.25 + coinDol.size() << endl;

    //print the same to a text file named "results.txt"
    ofstream myfile;
    myfile.open("results.txt");
    myfile << "Pennies: " << coinP.size() << endl;
    myfile << "Nickels: " << coinN.size() << endl;
    myfile << "Dimes: " << coinD.size() << endl;
    myfile << "Quarters: " << coinQ.size() << endl;
    myfile << "Half Dollars" << 0 << endl;
    myfile << "Dollars: " << coinDol.size() << endl;
    myfile << "Total value: $" << coinP.size() * 0.01 + coinN.size() * 0.05 + coinD.size() * 0.1 + coinQ.size() * 0.25 + coinDol.size() << endl;
    myfile.close();
}

void image2(){
    //Read in the image
    cv::Mat image = cv::imread("coinsHarder2-2018__.jpg", cv::IMREAD_COLOR);
    //convert to grayscale
    cv::Mat gray;
    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);

    //write the grayscale image to a file
    cv::imwrite("imageg.jpg", gray);

    //blur the image with a 5x5 kernel
    cv::Mat blur;
    cv::GaussianBlur(gray, blur, cv::Size(5,5), 0);

    //erode the image
    cv::Mat erode;
    cv::erode(blur, erode, cv::Mat(), cv::Point(-1,-1), 15);

    //dilate the image
    cv::Mat dilate;
    cv::dilate(erode, dilate, cv::Mat(), cv::Point(-1,-1), 20);

    cv::Mat contours;
    cv::Canny(blur, contours, 70, 200);
    cv::imwrite("imagef.jpg", contours);

    // //HoughCircles
    std::vector<cv::Vec3f> circles;
    cv::HoughCircles(dilate, circles, cv::HOUGH_GRADIENT, 2, 50, 100, 70, 70, 130);

    //draw the circles
    for (size_t i = 0; i < circles.size(); i++){
        cv::Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
        int radius = cvRound(circles[i][2]);
        cv::circle(image, center, radius, cv::Scalar(0, 0, 255), 3);
    }

    cv::imwrite("imageCircles.jpg", image);

    int average_radius = 0;
    for(int i = 0; i < circles.size(); i++){
        average_radius += circles[i][2];
    }
    average_radius = average_radius / circles.size();

    cout << "Average radius: " << average_radius << endl;

    //classify the coins by ratio to the average radius
    //make a vector of the coins for each coin
    vector<cv::Vec3f> coinP;
    vector<cv::Vec3f> coinN;
    vector<cv::Vec3f> coinD;
    vector<cv::Vec3f> coinQ;
    vector<cv::Vec3f> coinDol;
    for(int i = 0; i < circles.size(); i++){
        int x = circles[i][0];
        int y = circles[i][1];
        int b = image.at<cv::Vec3b>(y,x)[0];
        int g = image.at<cv::Vec3b>(y,x)[1];
        int r = image.at<cv::Vec3b>(y,x)[2];
        if(circles[i][2] < average_radius * 1.01 && r > (g+b)/2 ){
            if(r> 120){
                coinP.push_back(circles[i]);
            }
            else if(r > 60 && r < (g+b)/2 + 10){
                coinN.push_back(circles[i]);
            }
            else if(g < 100 && b < 100){
                coinP.push_back(circles[i]);
            }
            else{
                coinN.push_back(circles[i]);
            }
        }
        else if(circles[i][2] < average_radius * 0.9  ){
            coinD.push_back(circles[i]);
        }
        else if(circles[i][2] < average_radius * 1.134){
            coinN.push_back(circles[i]);
        }
        else if(circles[i][2] < average_radius * 1.3){
            coinQ.push_back(circles[i]);
        }
        else{
            coinDol.push_back(circles[i]);
        }
    }
    //draw the circles for each coin type in a different color
    //pennies are red
    for (size_t i = 0; i < coinP.size(); i++){
        cv::Point center(cvRound(coinP[i][0]), cvRound(coinP[i][1]));
        int radius = cvRound(coinP[i][2]);
        cv::circle(image, center, radius, cv::Scalar(0, 0, 255), 3);
    }
    //nickels are yellow
    for (size_t i = 0; i < coinN.size(); i++){
        cv::Point center(cvRound(coinN[i][0]), cvRound(coinN[i][1]));
        int radius = cvRound(coinN[i][2]);
        cv::circle(image, center, radius, cv::Scalar(0, 255, 255), 3);
    }
    //dimes are blue
    for (size_t i = 0; i < coinD.size(); i++){
        cv::Point center(cvRound(coinD[i][0]), cvRound(coinD[i][1]));
        int radius = cvRound(coinD[i][2]);
        cv::circle(image, center, radius, cv::Scalar(255, 0, 0), 3);
    }
    //quarters are purple
    for (size_t i = 0; i < coinQ.size(); i++){
        cv::Point center(cvRound(coinQ[i][0]), cvRound(coinQ[i][1]));
        int radius = cvRound(coinQ[i][2]);
        cv::circle(image, center, radius, cv::Scalar(255, 0, 255), 3);
    }
    //dollars are pink
    for (size_t i = 0; i < coinDol.size(); i++){
        cv::Point center(cvRound(coinDol[i][0]), cvRound(coinDol[i][1]));
        int radius = cvRound(coinDol[i][2]);
        cv::circle(image, center, radius, cv::Scalar(255, 192, 203), 3);
    }
    
    cv::imwrite("imageCoins.jpg", image);

    //print the number of each coin and the total value
    cout << "Pennies: " << coinP.size() << endl;
    cout << "Nickels: " << coinN.size() << endl;
    cout << "Dimes: " << coinD.size() << endl;
    cout << "Quarters: " << coinQ.size() << endl;
    cout << "Half Dollars" << 0 << endl;
    cout << "Dollars: " << coinDol.size() << endl;
    cout << "Total value: $" << coinP.size() * 0.01 + coinN.size() * 0.05 + coinD.size() * 0.1 + coinQ.size() * 0.25 + coinDol.size() << endl;

    //print the same to a text file named "results.txt"
    ofstream myfile;
    myfile.open("results.txt");
    myfile << "Pennies: " << coinP.size() << endl;
    myfile << "Nickels: " << coinN.size() << endl;
    myfile << "Dimes: " << coinD.size() << endl;
    myfile << "Quarters: " << coinQ.size() << endl;
    myfile << "Half Dollars" << 0 << endl;
    myfile << "Dollars: " << coinDol.size() << endl;
    myfile << "Total value: $" << coinP.size() * 0.01 + coinN.size() * 0.05 + coinD.size() * 0.1 + coinQ.size() * 0.25 + coinDol.size() << endl;
    myfile.close();
}

void image1(string filename, int minRadius, int maxRadius, int minDist, string result, string imageg, string imageCoins, string imageCircle, string imagef, int lt, int ht, int thres){
    //Read in the image
    cv::Mat image = cv::imread("image.jpg", cv::IMREAD_COLOR);
    //convert to grayscale
    cv::Mat gray;
    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);

    //write the grayscale image to a file
    cv::imwrite("imageg.jpg", gray);

    //blur the image with a 5x5 kernel
    cv::Mat blur;
    cv::GaussianBlur(gray, blur, cv::Size(5,5), 0);

    //erode the image
    cv::Mat erode;
    cv::erode(blur, erode, cv::Mat(), cv::Point(-1,-1), 15);

    //dilate the image
    cv::Mat dilate;
    cv::dilate(erode, dilate, cv::Mat(), cv::Point(-1,-1), 15);

    cv::Mat contours;
    cv::Canny(blur, contours, 70, 200);
    cv::imwrite("imagef.jpg", contours);

    // //HoughCircles
    std::vector<cv::Vec3f> circles;
    cv::HoughCircles(dilate, circles, cv::HOUGH_GRADIENT, 2, 50, 100, 100, 70, 130);

    //draw the circles
    for (size_t i = 0; i < circles.size(); i++){
        cv::Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
        int radius = cvRound(circles[i][2]);
        cv::circle(image, center, radius, cv::Scalar(255, 0, 0), 3);
    }

    cv::imwrite("imageCircles.jpg", image);

    int average_radius = 0;
    for(int i = 0; i < circles.size(); i++){
        average_radius += circles[i][2];
    }
    average_radius = average_radius / circles.size();

    cout << "Average radius: " << average_radius << endl;

    //classify the coins by ratio to the average radius
    //make a vector of the coins for each coin
    vector<cv::Vec3f> coinP;
    vector<cv::Vec3f> coinN;
    vector<cv::Vec3f> coinD;
    vector<cv::Vec3f> coinQ;
    vector<cv::Vec3f> coinDol;
    for(int i = 0; i < circles.size(); i++){
        int x = circles[i][0];
        int y = circles[i][1];
        int b = image.at<cv::Vec3b>(y,x)[0];
        int g = image.at<cv::Vec3b>(y,x)[1];
        int r = image.at<cv::Vec3b>(y,x)[2];
        if(circles[i][2] < average_radius * 1.01 && r > g && r > b){
            if(r> 120){
                coinP.push_back(circles[i]);
            }
            else if(g < 90 && b < 90){
                coinP.push_back(circles[i]);
            }
            else{
                coinN.push_back(circles[i]);
            }
        }
        else if(circles[i][2] < average_radius * 0.9  ){
            coinD.push_back(circles[i]);
        }
        else if(circles[i][2] < average_radius * 1.134){
            coinN.push_back(circles[i]);
        }
        else if(circles[i][2] < average_radius * 1.3){
            coinQ.push_back(circles[i]);
        }
        else{
            coinDol.push_back(circles[i]);
        }
    }
    //draw the circles for each coin type in a different color
    //pennies are red
    for (size_t i = 0; i < coinP.size(); i++){
        cv::Point center(cvRound(coinP[i][0]), cvRound(coinP[i][1]));
        int radius = cvRound(coinP[i][2]);
        cv::circle(image, center, radius, cv::Scalar(0, 0, 255), 3);
    }
    //nickels are yellow
    for (size_t i = 0; i < coinN.size(); i++){
        cv::Point center(cvRound(coinN[i][0]), cvRound(coinN[i][1]));
        int radius = cvRound(coinN[i][2]);
        cv::circle(image, center, radius, cv::Scalar(0, 255, 255), 3);
    }
    //dimes are blue
    for (size_t i = 0; i < coinD.size(); i++){
        cv::Point center(cvRound(coinD[i][0]), cvRound(coinD[i][1]));
        int radius = cvRound(coinD[i][2]);
        cv::circle(image, center, radius, cv::Scalar(255, 0, 0), 3);
    }
    //quarters are purple
    for (size_t i = 0; i < coinQ.size(); i++){
        cv::Point center(cvRound(coinQ[i][0]), cvRound(coinQ[i][1]));
        int radius = cvRound(coinQ[i][2]);
        cv::circle(image, center, radius, cv::Scalar(255, 0, 255), 3);
    }
    //dollars are pink
    for (size_t i = 0; i < coinDol.size(); i++){
        cv::Point center(cvRound(coinDol[i][0]), cvRound(coinDol[i][1]));
        int radius = cvRound(coinDol[i][2]);
        cv::circle(image, center, radius, cv::Scalar(255, 192, 203), 3);
    }
    
    cv::imwrite("imageCoins.jpg", image);

    //print the number of each coin and the total value
    cout << "Pennies: " << coinP.size() << endl;
    cout << "Nickels: " << coinN.size() << endl;
    cout << "Dimes: " << coinD.size() << endl;
    cout << "Quarters: " << coinQ.size() << endl;
    cout << "Half Dollars" << 0 << endl;
    cout << "Dollars: " << coinDol.size() << endl;
    cout << "Total value: $" << coinP.size() * 0.01 + coinN.size() * 0.05 + coinD.size() * 0.1 + coinQ.size() * 0.25 + coinDol.size() << endl;

    //print the same to a text file named "results.txt"
    ofstream myfile;
    myfile.open("results.txt");
    myfile << "Pennies: " << coinP.size() << endl;
    myfile << "Nickels: " << coinN.size() << endl;
    myfile << "Dimes: " << coinD.size() << endl;
    myfile << "Quarters: " << coinQ.size() << endl;
    myfile << "Half Dollars" << 0 << endl;
    myfile << "Dollars: " << coinDol.size() << endl;
    myfile << "Total value: $" << coinP.size() * 0.01 + coinN.size() * 0.05 + coinD.size() * 0.1 + coinQ.size() * 0.25 + coinDol.size() << endl;
    myfile.close();
}

void part1(bool image11, bool image21, bool image31, int argc, char *argv[]){
    if(image11){
            string inputfile = "image.jpg"; //
            int lt = 100; //
            int ht = 200; //
            string imfName = "imagef.jpg";  //
            string imCirName = "imageCircles.jpg"; //
            string imCoinName = "imageCoins.jpg"; //
            string imResName = "results.txt"; //
            string imgName = "imageg.jpg"; //
            int minR = 70; //
            int maxR = 100; //
            int minDist = 100; //
            int thresh = 100; //
            // -ht 90 -minR 75 -maxR 170 -circlethresh 70 -dist 110
            for (int i = 0; i < argc; i++){
                if(strcmp(argv[i], "-f") == 0){
                    inputfile = argv[i+1];
                }
                else if(strcmp(argv[i], "-lt") == 0){
                    lt = atoi(argv[i+1]);
                }
                else if(strcmp(argv[i], "-ht") == 0){
                    ht = atoi(argv[i+1]);
                }
                else if(strcmp(argv[i], "-imf") == 0){
                    imfName = argv[i+1];
                }
                else if(strcmp(argv[i], "-imCir") == 0){
                    imCirName = argv[i+1];
                }
                else if(strcmp(argv[i], "-imCoin") == 0){
                    imCoinName = argv[i+1];
                }
                else if(strcmp(argv[i], "-imRes") == 0){
                    imResName = argv[i+1];
                }
                else if(strcmp(argv[i], "-minR") == 0){
                    minR = atoi(argv[i+1]);
                }
                else if(strcmp(argv[i], "-maxR") == 0){
                    maxR = atoi(argv[i+1]);
                }
                else if(strcmp(argv[i], "-dist") == 0){
                    minDist = atoi(argv[i+1]);
                }
                else if(strcmp(argv[i], "-tresh") == 0){
                    thresh = atoi(argv[i+1]);
                }
                else if(strcmp(argv[i], "-img") == 0){
                    imgName = argv[i+1];
                }
            }
        image1();
    }
    else if(image21){
            string inputfile = "image.jpg";
            int lt = 100;
            int ht = 200;
            string imfName = "imagef.jpg";
            string imCirName = "imageCircles.jpg";
            string imCoinName = "imageCoins.jpg";
            string imResName = "results.txt";
            string imgName = "imageg.jpg";
            int minR = 70;
            int maxR = 100;
            int minDist = 100;
            int thresh = 100;
            // -ht 90 -minR 75 -maxR 170 -circlethresh 70 -dist 110
            for (int i = 0; i < argc; i++){
                if(strcmp(argv[i], "-f") == 0){
                    inputfile = argv[i+1];
                }
                else if(strcmp(argv[i], "-lt") == 0){
                    lt = atoi(argv[i+1]);
                }
                else if(strcmp(argv[i], "-ht") == 0){
                    ht = atoi(argv[i+1]);
                }
                else if(strcmp(argv[i], "-imf") == 0){
                    imfName = argv[i+1];
                }
                else if(strcmp(argv[i], "-imCir") == 0){
                    imCirName = argv[i+1];
                }
                else if(strcmp(argv[i], "-imCoin") == 0){
                    imCoinName = argv[i+1];
                }
                else if(strcmp(argv[i], "-imRes") == 0){
                    imResName = argv[i+1];
                }
                else if(strcmp(argv[i], "-minR") == 0){
                    minR = atoi(argv[i+1]);
                }
                else if(strcmp(argv[i], "-maxR") == 0){
                    maxR = atoi(argv[i+1]);
                }
                else if(strcmp(argv[i], "-dist") == 0){
                    minDist = atoi(argv[i+1]);
                }
                else if(strcmp(argv[i], "-tresh") == 0){
                    thresh = atoi(argv[i+1]);
                }
                else if(strcmp(argv[i], "-img") == 0){
                    imgName = argv[i+1];
                }
            }
        image2();
    }
    else if(image31){
            string inputfile = "image.jpg";
            int lt = 100;
            int ht = 200;
            string imfName = "imagef.jpg";
            string imCirName = "imageCircles.jpg";
            string imCoinName = "imageCoins.jpg";
            string imResName = "results.txt";
            string imgName = "imageg.jpg";
            int minR = 70;
            int maxR = 100;
            int minDist = 100;
            int thresh = 100;
            // -ht 90 -minR 75 -maxR 170 -circlethresh 70 -dist 110
            for (int i = 0; i < argc; i++){
                if(strcmp(argv[i], "-f") == 0){
                    inputfile = argv[i+1];
                }
                else if(strcmp(argv[i], "-lt") == 0){
                    lt = atoi(argv[i+1]);
                }
                else if(strcmp(argv[i], "-ht") == 0){
                    ht = atoi(argv[i+1]);
                }
                else if(strcmp(argv[i], "-imf") == 0){
                    imfName = argv[i+1];
                }
                else if(strcmp(argv[i], "-imCir") == 0){
                    imCirName = argv[i+1];
                }
                else if(strcmp(argv[i], "-imCoin") == 0){
                    imCoinName = argv[i+1];
                }
                else if(strcmp(argv[i], "-imRes") == 0){
                    imResName = argv[i+1];
                }
                else if(strcmp(argv[i], "-minR") == 0){
                    minR = atoi(argv[i+1]);
                }
                else if(strcmp(argv[i], "-maxR") == 0){
                    maxR = atoi(argv[i+1]);
                }
                else if(strcmp(argv[i], "-dist") == 0){
                    minDist = atoi(argv[i+1]);
                }
                else if(strcmp(argv[i], "-tresh") == 0){
                    thresh = atoi(argv[i+1]);
                }
                else if(strcmp(argv[i], "-img") == 0){
                    imgName = argv[i+1];
                }
        }
        image3();
    }
}



int main(int argc, char *argv[]){
    part1(false, false, true, argc, argv);
    return 0;
}

