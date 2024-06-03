//Satvik Matta
//December 2, 2022
#include <fstream>
#include <math.h>
#include <string>
#include <vector>
#include <cstring>
using namespace std;

ofstream intensitymatrix(ifstream &infile){
    ofstream outfile;
    outfile.open("imageg.ppm");
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
    intensitymatrix(infile);
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
    intensitymatrix(infile);
    ifstream infile2("imageg.ppm");
    sobel(infile2);
    ifstream infile3("images.ppm");
    thres2(infile3, lt, ht, outputfilename);
}
int main(int argc, char *argv[]){
    // part1();
    part2(argc, argv);
    return 0;
}