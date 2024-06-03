//Satvik Matta
//December 2, 2022
#include <fstream>
#include <math.h>
#include <string>
#include <vector>
#include <iostream>
#include <cstring>
using namespace std;

ofstream intensitymatrix(ifstream &infile, string filename){
    ofstream outfile;
    outfile.open(filename);
    string line;
    int width, height, maxval;
    infile >> line;
    infile >> width >> height >> maxval;
    outfile << line << endl;
    outfile << width << " " << height << endl;
    outfile << maxval << endl;
    //convert to grayscale
    for (int i = 0; i < height; i++){
        for (int j = 0; j < width; j++){
            int r, g, b;
            infile >> r >> g >> b;
            int intensity = (r + g + b) / 3;
            outfile << intensity << " " << intensity << " " << intensity << " ";
        }
        outfile << endl;
    }
    outfile.close();
    return outfile;

}

//sobel filter on a grayscale image
void sobel(ifstream &infile){
    ofstream outfile;
    outfile.open("images.ppm");
    string line;
    int width, height, maxval;
    infile >> line;
    infile >> width >> height >> maxval;
    outfile << line << endl;
    outfile << width << " " << height << endl;
    outfile << maxval << endl;
    //put image into 2d vector
    vector<vector<int>> image;
    for (int i = 0; i < height; i++){
        vector<int> row;
        for (int j = 0; j < width; j++){
            int r, g, b;
            infile >> r >> g >> b;
            row.push_back(r);
        }
        image.push_back(row);
    }
    vector<vector<int>> sobel;
    for (int i = 0; i < height; i++){
        vector<int> row;
        for (int j = 0; j < width; j++){
            if (i == 0 || i == height - 1 || j == 0 || j == width - 1){
                row.push_back(0);
            }
            else{
                int gx = -1*image[i-1][j-1] + -2*image[i][j-1] - image[i+1][j-1] + image[i-1][j+1] + 2*image[i][j+1] + image[i+1][j+1];
                int gy = -1*image[i-1][j-1] + -2*image[i-1][j] - image[i-1][j+1] + image[i+1][j-1] + 2*image[i+1][j] + image[i+1][j+1];
                int g = sqrt(gx*gx + gy*gy);
                row.push_back(g);
            }
        }
        sobel.push_back(row);
    }
    //make sobel into ppm
    for (int i = 0; i < height; i++){
        for (int j = 0; j < width; j++){
            outfile << sobel[i][j] << " " << sobel[i][j] << " " << sobel[i][j] << " ";
        }
        outfile << endl;
    }
    outfile.close();
}

ofstream thres(ifstream &infile){
    ofstream outfile;
    outfile.open("imagem.ppm");
    string line;
    int width, height, maxval;
    infile >> line;
    infile >> width >> height >> maxval;
    maxval = 1;
    outfile << line << endl;
    outfile << width << " " << height << endl;
    outfile << maxval << endl;
    for (int i = 0; i < height; i++){
        for (int j = 0; j < width; j++){
            int r, g, b;
            infile >> r >> g >> b;
            if (r > 60){
                outfile << 1 << " " << 1 << " " << 1 << " ";
            }
            else{
                outfile << 0 << " " << 0 << " " << 0 << " ";
            }
        }
        outfile << endl;
    }
    outfile.close();
    return outfile;
}

//recursive hysterisis
// 1= weak edge, 2= strong edge, 0= not edge
void hysterisis(vector<vector<int>> &matrix, int i, int j){
    if (i < 0 || i >= (int) matrix.size() || j < 0 || j >=  (int) matrix[0].size()){
        return;
    }
    if(matrix[i][j]==3 || matrix[i][j]==0){
        return;
    }
    matrix[i][j] = 3;
    hysterisis(matrix, i-1, j-1);
    hysterisis(matrix, i-1, j);
    hysterisis(matrix, i-1, j+1);
    hysterisis(matrix, i, j-1);
    hysterisis(matrix, i, j+1);
    hysterisis(matrix, i+1, j-1);
    hysterisis(matrix, i+1, j);
    hysterisis(matrix, i+1, j+1);
}

//double threshold
// 1= weak edge, 2= strong edge, 0= not edge
void thres2(ifstream &infile, int lt, int ht, string filename){
    ofstream outfile;
    outfile.open(filename);
    string line;
    int width, height, maxval;
    infile >> line;
    infile >> width >> height >> maxval;
    maxval = 1;
    outfile << line << endl;
    outfile << width << " " << height << endl;
    outfile << maxval << endl;
    vector<vector<int>> matrix;
    for (int i = 0; i < height; i++){
        vector<int> row;
        for (int j = 0; j < width; j++){
            int r, g, b;
            infile >> r >> g >> b;
            if (r > ht){
                row.push_back(2);
            }
            else if (r > lt){
                row.push_back(1);
            }
            else{
                row.push_back(0);
            }
        }
        matrix.push_back(row);
    }
    for (int i = 0; i < height; i++){
        for (int j = 0; j < width; j++){
            if (matrix[i][j] == 2){
                hysterisis(matrix, i, j);
            }
        }
    }
    //convert all 1's to 0's and all 3's to 1's and all 2's to 0's and make ppm
    for (int i = 0; i < height; i++){
        for (int j = 0; j < width; j++){
            if (matrix[i][j] == 1){
                outfile << 0 << " " << 0 << " " << 0 << " ";
            }
            else if (matrix[i][j] == 3){
                outfile << 1 << " " << 1 << " " << 1 << " ";
            }
            else{
                outfile << 0 << " " << 0 << " " << 0 << " ";
            }
        }
        outfile << endl;
    }
    outfile.close();
}

void part1(){
    ifstream infile("image.ppm");
    intensitymatrix(infile, "imageg.ppm");
    ifstream infile2("imageg.ppm");
    sobel(infile2);
    ifstream infile3("images.ppm");
    thres(infile3);
}



void part2(int argc, char *argv[]){
    string filename = "image.ppm";
    int lt = 50;
    int ht = 80;
    string outputfilename = "image1.ppm";
    //parse command line
    for (int i = 1; i < argc; i++){
        if (strcmp(argv[i], "-f") == 0){
            filename = argv[i+1];
        }
        else if (strcmp(argv[i], "-lt") == 0){
            lt = atoi(argv[i+1]);
        }
        else if (strcmp(argv[i], "-ht") == 0){
            ht = atoi(argv[i+1]);
        }
        else if (strcmp(argv[i], "-o") == 0){
            outputfilename = argv[i+1];
        }
    }
    ifstream infile(filename);
    intensitymatrix(infile, "imageg.ppm");
    ifstream infile2("imageg.ppm");
    sobel(infile2);
    ifstream infile3("images.ppm");
    thres2(infile3, lt, ht, outputfilename);
}

void part3(int argc, char *argv[]){

    // cout << "Part 3" << endl;

    string filename = "image.ppm";
    int lt = 50;
    int ht = 80;
    string outputfilename = "image1.ppm";
    string grayfilename = "imageg.ppm";
    string sobelfilename = "images.ppm";
    string image2 = "image2.ppm";
    string finalfilename = "imagef.ppm";


    //parse command line
    for (int i = 0; i < argc; i++){
        //print argvi
        cout << argv[i] << endl;
        cout<< "i: " << i << endl;
        if (strcmp(argv[i], "-f") == 0){
            filename = argv[i+1];
        }
        else if (strcmp(argv[i], "-lt") == 0){
            lt = atoi(argv[i+1]);
        }
        else if (strcmp(argv[i], "-ht") == 0){
            ht = atoi(argv[i+1]);
        }
        else if (strcmp(argv[i], "-of") == 0){
            outputfilename = argv[i+1];
        }
        else if (strcmp(argv[i], "-fg") == 0){
            grayfilename = argv[i+1];
        }
        else if (strcmp(argv[i], "-f2") == 0){
            image2 = argv[i+1];
        }
        else if (strcmp(argv[i], "-ff") == 0){
            finalfilename = argv[i+1];
        }
    }
    //print
    cout << filename << endl;
    //intesity matrix
    ifstream infile(filename);
    ofstream outfile;
    outfile.open(grayfilename);
    string line;
    int width, height, maxval;
    infile >> line;
    infile >> width >> height >> maxval;
    outfile << line << endl;
    outfile << width << " " << height << endl;
    outfile << maxval << endl;
    vector<vector<int>> imggray;
    //convert to grayscale
    // cout << "start of for loop" << endl;
    for (int i = 0; i < height; i++){
        vector<int> rowtemp;
        for (int j = 0; j < width; j++){
            int r, g, b;
            infile >> r >> g >> b;
            int intensity = (r + g + b) / 3;
            outfile << intensity << " " << intensity << " " << intensity << " ";
            // cout <<  "i = " << i  << " j = " << j << endl;
            rowtemp.push_back(intensity);
        }
        outfile << endl;
        imggray.push_back(rowtemp);
    }
    outfile.close();

    //print step
    // cout << "intensity matrix" << endl;


    //put image into 2d vector
    vector<vector<int>> image = imggray;
    vector<vector<int>> sobel;
    for (int i = 0; i < height; i++){
        vector<int> row;
        for (int j = 0; j < width; j++){
            if (i == 0 || i == height - 1 || j == 0 || j == width - 1){
                row.push_back(0);
            }
            else{
                int gx = -1*image[i-1][j-1] + -2*image[i][j-1] - image[i+1][j-1] + image[i-1][j+1] + 2*image[i][j+1] + image[i+1][j+1];
                int gy = -1*image[i-1][j-1] + -2*image[i-1][j] - image[i-1][j+1] + image[i+1][j-1] + 2*image[i+1][j] + image[i+1][j+1];
                int g = sqrt(gx*gx + gy*gy);
                row.push_back(g);
            }
        }
        sobel.push_back(row);
    }

    // cout << "sobel" << endl;

    //double threshold and hysteresis
    outfile.open(outputfilename);
    maxval = 1;
    outfile << line << endl;
    outfile << width << " " << height << endl;
    outfile << maxval << endl;
    vector<vector<int>> matrix;
    for (int i = 0; i < height; i++){
        vector<int> row;
        for (int j = 0; j < width; j++){
            int r;
            r = sobel[i][j];
            if (r > ht){
                row.push_back(2);
            }
            else if (r > lt){
                row.push_back(1);
            }
            else{
                row.push_back(0);
            }
        }
        matrix.push_back(row);
    }
    for (int i = 0; i < height; i++){
        for (int j = 0; j < width; j++){
            if (matrix[i][j] == 2){
                hysterisis(matrix, i, j);
            }
        }
    }
    //convert all 1's to 0's and all 3's to 1's and all 2's to 0's and make ppm
    for (int i = 0; i < height; i++){
        for (int j = 0; j < width; j++){
            if (matrix[i][j] == 1){
                outfile << 0 << " " << 0 << " " << 0 << " ";
            }
            else if (matrix[i][j] == 3){
                outfile << 1 << " " << 1 << " " << 1 << " ";
            }
            else{
                outfile << 0 << " " << 0 << " " << 0 << " ";
            }
            
        }
        outfile << endl;
    }
    outfile.close();

    vector<vector<int>> dhyst = matrix;

    // cout << "double threshold and hysteresis" << endl;

    //create theta matrix
    vector<vector<double>> theta;
    for (int i = 0; i < height; i++){
        vector<double> row2;
        for (int j = 0; j < width; j++){
            if (i == 0 || i == height - 1 || j == 0 || j == width - 1){
                row2.push_back(0);
            }
            else{
                int gx = -1*image[i-1][j-1] + -2*image[i][j-1] - image[i+1][j-1] + image[i-1][j+1] + 2*image[i][j+1] + image[i+1][j+1];
                int gy = -1*image[i-1][j-1] + -2*image[i-1][j] - image[i-1][j+1] + image[i+1][j-1] + 2*image[i+1][j] + image[i+1][j+1];
                double g = atan2(gy, gx);
                row2.push_back(g);
            }
        }
        theta.push_back(row2);
    }

    //convert theta matrix to degrees
    for (int i = 0; i < theta.size(); i++) {
        for (int j = 0; j < theta[i].size(); j++) {
            theta[i][j] = theta[i][j] * 180 / M_PI;
        }
    }
    
    // cout << "theta matrix" << endl;

    vector<vector<double>> angles = theta;
    for (int i = 0; i < theta.size(); i++) {
        for (int j = 0; j < theta[i].size(); j++) {
            double angle = theta[i][j];
            double absolute = (angle > 0) ? angle : -1*angle;
            bool isNegative = angle < 0;
            if (absolute >= 0 && absolute <= 22.5) {
                if (angle == -22.5) angles[i][j] = -45;
                else angles[i][j] = 0;
            }
            else if (absolute > 22.5 && absolute <= 67.5) {
                if (angle == -67.5) angles[i][j] = -45; //-135
                else angles[i][j] = (((int)(isNegative)*(-2))+1)*45;
            }
            else if (absolute > 67.5 && absolute <= 112.5) {
                if (angle == -112.5) angles[i][j] = -90; //-180
                else angles[i][j] = (((int)(isNegative)*(-2))+1)*90;
            }
            else if (absolute > 112.5 && absolute <= 157.5) {
                if (angle == -157.5) angles[i][j] = -135; //-135
                else angles[i][j] = (((int)(isNegative)*(-2))+1)*135;
            }
            else {
                angles[i][j] = 180;
            }

        }
    }


    // cout << "theta matrix transform" << endl;
    
    vector<vector<int>> image21;
    image = sobel;
    //non-maximum suppression
    for (int i = 0; i < height; i++){
        vector<int> row3;
        for (int j = 0; j < width; j++){
            int angle = angles[i][j];
            int curr, p1, p2, maxp = 0;
            curr = sobel[i][j];
            if (angle == 0 || angle == 180) {
                p1 = sobel[i][j-1];
                p2 = sobel[i][j+1];
                maxp = max(p1, p2);
            } else if (angle == 45 || angle == -135) {
                p1 = sobel[i-1][j+1];
                p2 = sobel[i+1][j-1];
                maxp = max(p1, p2);
            } else if (angle == 90 || angle == -90) {
                p1 = sobel[i-1][j];
                p2 = sobel[i+1][j];
                maxp = max(p1, p2);
            } else {
                p1 = sobel[i-1][j-1];
                p2 = sobel[i+1][j+1];
                maxp = max(p1, p2);
            }
            int k = (curr >= maxp) ? 0 : 1;
            row3.push_back(k);
        //     if (i == 0 || i == height - 1 || j == 0 || j == width - 1){
        //         row3.push_back(0);
        //     }
        //     else if(theta[i][j]== 180 || theta[i][j] == 0){
        //         if (image[i][j] >= image[i][j-1] && image[i][j] >= image[i][j+1]){
        //             row3.push_back(1);
        //         }
        //         else{
        //             row3.push_back(0);
        //         }
        //     }
        //     else if (theta[i][j] == 135 || theta[i][j] == -45){
        //         if(image[i][j] >= image[i-1][j-1] && image[i][j] >= image[i+1][j+1]){
        //             row3.push_back(1);
        //         }
        //         else{
        //             row3.push_back(0);
        //         }
        //     }
        //     else if (theta[i][j] == 45 || theta[i][j] == -135){
        //         if(image[i][j] >= image[i-1][j+1] && image[i][j] >= image[i+1][j-1]){
        //             row3.push_back(1);
        //         }
        //         else{
        //             row3.push_back(0);
        //         }
        //     }
        //     else if (theta[i][j] == 90){
        //         if(image[i][j] >= image[i-1][j] && image[i][j] >= image[i+1][j]){
        //             row3.push_back(1);
        //         }
        //         else{
        //             row3.push_back(1);
        //         }
        //     }
            
        }
        image21.push_back(row3);
    }

    cout << "image21 dimensions: " << image21.size() << " " << image21[0].size() << endl;
    // cout << "image1 dimensions: " << image1.size() << " " << image1[0].size() << endl;

    // cout << "non-maximum suppression" << endl;

    //create image2.ppm
    ofstream myfile2;
    myfile2.open(image2);
    maxval = 1;
    myfile2 << line << endl;
    myfile2 << width << " " << height << endl;
    myfile2 << maxval << endl;
    for (int i = 0; i < height; i++){
        for (int j = 0; j < width; j++){
            if (image21[i][j] == 1){
                myfile2 << 1 << " " << 1 << " " << 1 << " ";
            }
            else{
                myfile2 << 0 << " " << 0 << " " << 0 << " ";
            }
        }
        myfile2 << endl;
    }

    // cout << "image2.ppm created" << endl;

    //check if image1 and image2 and see if each pixel is the same
    //if it is the same, then it is an edge pixel
    //if it is not the same, then it is not an edge pixel
    // vector<vector<int>> image3;
    // for (int i = 0; i < height; i++){
    //     vector<int> row3;
    //     for (int j = 0; j < width; j++){
    //         // cout << "i: " << i << " j: " << j << endl;
    //         if (matrix[i][j] == image21[i][j] && matrix[i][j] == 1){
    //             row3.push_back(1);
    //         }
    //         else{
    //             row3.push_back(0);
    //         }
    //     }
    //     image3.push_back(row3);
    // }

    ofstream outfile2("image6.ppm");
    outfile2 << "P3 " << dhyst[0].size() << " " << dhyst.size() << " " << 1 << endl;
    for (int i = 0; i < (int)(dhyst.size()); i++) {
        for (int j = 0; j < (int)(dhyst[0].size()); j++) {
            if (dhyst[i][j] == 3) {
                outfile2 << 1 << " " << 1 << " " << 1 << " ";
            } else {
                outfile2 << 0 << " " << 0 << " " << 0 << " ";
            }
        }
        outfile2 << endl;
    }
    outfile2.close();

    cout << "image3 created" << endl;

    //create image3.ppm
    ofstream myfile3;
    // myfile3.open(finalfilename);
    // maxval = 1;
    // myfile3 << line << endl;
    // myfile3 << width << " " << height << endl;
    // myfile3 << maxval << endl;
    // for (int i = 0; i < height; i++){
    //     for (int j = 0; j < width; j++){
    //         if (image3[i][j] == 1){
    //             myfile3 << 1 << " " << 1 << " " << 1 << " ";
    //         }
    //         else{
    //             myfile3 << 0 << " " << 0 << " " << 0 << " ";
    //         }
    //     }
    //     myfile3 << endl;
    // }
    // myfile3.close();
    myfile2.close();
    
    cout << "image3.ppm created" << endl;
}

int main(int argc, char *argv[]){
    // part1();
    // part2(argc, argv);
    part3(argc, argv);
    return 0;
}