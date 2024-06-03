//Satvik Matta
//March 3, 2023
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>
#include <limits>
#include <vector>
#include <cstring>

using namespace std;

class Point {
    private:
        double x;
        double y;
    public:
        Point(double x, double y) {
            this -> x = x;
            this -> y = y;
        }
        double getX() {
            return x;
        }
        double getY() {
            return y;
        }
        bool coinEquals(Point p2) {
            return (x <= p2.getX()+85 && x >= p2.getX()-85 && y <= p2.getY()+85 && y >= p2.getY()-85);
        }
};

class Render {
    private:
        vector<vector<vector<int>>> pixels;
        vector<vector<int>> inc;
        vector<vector<vector<int>>> originialImage;
        int width; // number of columns
        int height; // number of height
        int maxVal;
    public:
        Render(string filename) {
            ifstream fin(filename);
            string temp;
            fin >> temp;
            fin >> width;
            fin >> height;
            fin >> maxVal;
            pixels.resize(height);
            inc.resize(height);
            originialImage.resize(height);
            for(int i = 0; i < height; i++){
                pixels[i].resize(width);
                inc[i].resize(width);
                originialImage[i].resize(width);
                for(int j = 0; j < width; j++){
                    pixels[i][j].resize(3);
                    inc[i][j] = 0;
                    originialImage[i][j].resize(3);
                }
            }
            for(int i = 0; i < height; i++){
                for(int j = 0; j < width; j++){
                    for(int k = 0; k < 3; k++){
                        fin >> pixels[i][j][k];
                        originialImage[i][j][k] = pixels[i][j][k];
                    }
                }
            }
        }
    void createPPM(string filename) {
        ofstream outfile(filename);
        outfile << "P3 " << endl;
        outfile << width << " " << height << endl;
        outfile << maxVal << endl;
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                outfile << pixels[i][j][0] << " " << pixels[i][j][1] << " " << pixels[i][j][2] << " " << endl;
            }
        }
        outfile.close();
    }
    void illuminate(int x, int y, int r, int g, int b, bool increment) {
        if (x >= 0 && y >= 0 && x < width && y < height) {
            if (increment){
                inc[y][x] = inc[y][x]+1;
            } 
            else {
                pixels[y][x][0] = r;
                pixels[y][x][1] = g;
                pixels[y][x][2] = b;
            }
        }
    }
    void bres1(int x1, int y1, int x2, int y2, int r, int g, int b, bool increment){
            if (x1 > x2)
            {
                int temp = x1;
                x1 = x2;
                x2 = temp;
                temp = y1;
                y1 = y2;
                y2 = temp;
            }
            int dx = x2 - x1;
            int dy = y2 - y1;
            int j = y1;
            int error = dy - dx;

            for (int i = x1; i < x2; i++)
            {
                illuminate(i, j, r, g, b, increment);
                if (error >= 0)
                {
                    j++;
                    error = error - dx;
                }
                error = error + dy;
            }
        }

    void bres2(int x1, int y1, int x2, int y2, int r, int g, int b, bool increment){
        if (x1 > x2)
        {
            int temp = x1;
            x1 = x2;
            x2 = temp;
            temp = y1;
            y1 = y2;
            y2 = temp;
        }
        int negy1 = (int)inc[0].size() - 1 - y1;
        int negy2 = (int)inc[0].size() - 1 - y2;

        int dx = x2 - x1;
        int dy = negy2 - negy1;
        int j = negy1;
        int error = dy - dx;
        for (int i = x1; i < x2; i++)
        {
            illuminate(i, (int)inc[0].size() - 1 - j, r, g, b, increment);
            if (error >= 0)
            {
                j++;
                error = error - dx;
            }
            error = error + dy;
        }
    }
    void bres3(int x1, int y1, int x2, int y2, int r, int g, int b, bool increment){
        if (y1 > y2){
            int temp = x1;
            x1 = x2;
            x2 = temp;
            temp = y1;
            y1 = y2;
            y2 = temp;
        }
        int dx = x2 - x1;
        int dy = y2 - y1;
        int j = x1;
        int error = dx - dy;

        for (int i = y1; i < y2; i++)
        {
            illuminate(j, i, r, g, b, increment);
            if (error >= 0)
            {
                j++;
                error = error - dy;
            }
            error = error + dx;
        }
    }

    void bres4(int x1, int y1, int x2, int y2, int r, int g, int b, bool increment){
        if (y1 > y2) {
            int temp = x1;
            x1 = x2;
            x2 = temp;
            temp = y1;
            y1 = y2;
            y2 = temp;
        }
        int negx1 = (int)inc.size()-1 - x1;
        int negx2 = (int)inc.size()-1 - x2;

        int dy = y2 - y1;
        int dx = negx2 - negx1;
        int j = negx1;
        int error = dx - dy;
        for (int i = y1; i < y2; i++) {
            illuminate((int)inc.size()-1-j, i, r, g, b, increment);
            if (error >= 0) {
                j++;
                error = error - dy;
            }
            error = error + dx;
        }
        
    }
    void drawstraight(int x1, int x2, int y1, int y2, int r, int g, int b, bool increment)
    {
        if (x1 == x2)
        {
            if (y1 > y2)
            {
                int temp = y1;
                y1 = y2;
                y2 = temp;
            }
            for (int i = y1; i <= y2; i++)
            {
                illuminate(x1, i, r, g, b, increment);
            }
        }
        else if (y1 == y2)
        {
            if (x1 > x2)
            {
                int temp = x1;
                x1 = x2;
                x2 = temp;
            }
            for (int i = x1; i <= x2; i++)
            {
                illuminate(i, y1, r, g, b, increment);
            }
        }
    }

    void drawline(int x1, int x2, int y1, int y2, int r, int g, int b, bool increment)
    {
        if (x1 == x2 || y1 == y2)
        {
            drawstraight(x1, x2, y1, y2, r, g, b, increment);
            return;
        }
        int dx = x2 - x1;
        int dy = y2 - y1;
        if (abs(dx) >= abs(dy))
        {
            if ((double)dx / dy > 0)
            {
                bres1(x1, y1, x2, y2, r, g, b, increment);
            }
            else
            {
                bres2(x1, y1, x2, y2, r, g, b, increment);
            }
        }
        else if (abs(dx) < abs(dy))
        {
            if ((double)dx / dy > 0)
            {
                bres3(x1, y1, x2, y2, r, g, b, increment);
            }
            else
            {
                bres4(x1, y1, x2, y2, r, g, b, increment);
            }
        }
        illuminate(x1, y1, r, g, b, increment);
        illuminate(x2, y2, r, g, b, increment);
    }
    void nLin(Point point1, Point point2, int r, int g, int b, bool increment) {
        double b1 = 0.0;
        double slope = (point1.getY() - point2.getY()) / (point1.getX() - point2.getX());
        if (isfinite(slope)) {
            b1 = point1.getY() - (slope * point1.getX());
            Point point3 = Point((0 - b1) / slope, 0);
            Point point4 = Point((height - b1) / slope, height);
            drawline(point3.getX(), point4.getX(), point3.getY(), point4.getY(), r, g, b, increment);
        } else {
            Point point3 = Point(point2.getX(), 0);
            Point point4 = Point(point2.getX(), height);
            drawline(point3.getX(), point4.getX(), point3.getY(), point4.getY(), r, g, b, increment);
        }
    }

    void drawcircle(int xc, int yc, double rad, int r, int g, int b, bool increment)
    {
        int xmax = (int)(rad * 0.70710678);
        int y = rad;
        int y2 = y * y;
        int ty = (2 * y) - 1;
        int y2_new = y2;

        for (int x = 0; x <= xmax + 2; x++)
        {
            if ((y2 - y2_new) >= ty)
            {
                y2 -= ty;
                y -= 1;
                ty -= 2;
            }
            illuminate(xc + x, yc + y, r, g, b, increment);
            illuminate(xc + x, yc - y, r, g, b, increment);
            illuminate(xc - x, yc + y, r, g, b, increment);
            illuminate(xc - x, yc - y, r, g, b, increment);
            illuminate(xc + y, yc + x, r, g, b, increment);
            illuminate(xc + y, yc - x, r, g, b, increment);
            illuminate(xc - y, yc + x, r, g, b, increment);
            illuminate(xc - y, yc - x, r, g, b, increment);
            y2_new -= (2 * x) - 3;
        }
    }

    void hystersis(int row, int col, vector<vector<int>> &array) {
        if (row < 0 || row >= height || col < 0 || col >= width || array[row][col] == 1 || array[row][col] == 0) {
            return;
        }
        array[row][col] = 1;
        hystersis(row + 1, col, array);
        hystersis(row + 1, col + 1, array);
        hystersis(row, col + 1, array);
        hystersis(row - 1, col, array);
        hystersis(row - 1, col - 1, array);
        hystersis(row, col - 1, array);
        hystersis(row + 1, col - 1, array);
        hystersis(row - 1, col + 1, array);
    }
    vector<vector<double>> canny(int threshold1, int threshold2, string filename2, string filename3, string filename4, string filename5, string filename6, string filename7) {
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                int temp = (pixels[i][j][0] + pixels[i][j][1] + pixels[i][j][2]) / 3;
                pixels[i][j][0] = temp;
                pixels[i][j][1] = temp;
                pixels[i][j][2] = temp;
            }
        }

        // create black and white image
        ofstream bwImage;
        bwImage.open("imageg.ppm");
        bwImage << "P3" << endl;
        bwImage << width << " " << height << endl;
        bwImage << "255" << endl;
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                bwImage << pixels[i][j][0] << " " << pixels[i][j][1] << " " << pixels[i][j][2] << " ";
            }
            bwImage << endl;
        }

        bwImage.close();
        cout << "Black and white image created" << endl;

        vector<vector<vector<int>>> temp;
        vector<vector<int>> angles(height, vector<int>(width, 0));
        vector<vector<double>> angles2(height, vector<double>(width, 0.0));
        vector<vector<int>> magnitudes(height, vector<int>(width, 0));
        copy(pixels.begin(), pixels.end(), back_inserter(temp));
        vector<Point> places;
        vector<vector<int>> tempImage(height, vector<int>(width, 0));
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                tempImage[i][j] = pixels[i][j][0];
            }
        }
        double tempA = 0.0;
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                if (i == 0 || j == 0 || j == width - 1 || i == height - 1) {
                    tempImage[i][j] = 0;
                } else {
                    double gX = (temp[i-1][j-1][0]*-1 + temp[i][j-1][0]*-2 + temp[i+1][j-1][0]*-1 + temp[i-1][j+1][0] + temp[i][j+1][0]*2 + temp[i+1][j+1][0]);
                    double gY = (temp[i-1][j-1][0]*-1 + temp[i-1][j][0]*-2 + temp[i-1][j+1][0]*-1 + temp[i+1][j-1][0] + temp[i+1][j][0]*2 + temp[i+1][j+1][0]);
                    int tempm = sqrt(pow(gX, 2) + pow(gY, 2));
                    if (tempm > threshold2) {
                        tempImage[i][j] = 3;
                        places.push_back(Point(j, i));
                    } else if (tempm > threshold1) {
                        tempImage[i][j] = 2;
                    } else {
                        tempImage[i][j] = 0;
                    }
                    magnitudes[i][j] = tempm;
                    angles2[i][j] = atan2(gY, gX);
                    tempA = angles2[i][j]*180 / M_PI;
                    if (tempA < 0) {
                        tempA = tempA + 180;
                    }
                    if (tempA >= 0 && tempA <= 22.5) {
                        angles[i][j] = 0;
                    } else if (tempA > 22.5 && tempA <= 67.5) {
                        angles[i][j] = 45;
                    } else if (tempA > 67.5 && tempA <= 112.5) {
                        angles[i][j] = 90;
                    } else if (tempA > 112.5 && tempA <= 157.5) {
                        angles[i][j] = 135;
                    } else if (tempA > 157.5 && tempA <= 180) {
                        angles[i][j] = 0;
                    }
                }
            }
        }
        for (int i = 0; i < (int) places.size(); i++) {
            Point p = places[i];
            hystersis(p.getY(), p.getX(), tempImage);
        }

        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                pixels[i][j][0] = tempImage[i][j];
                pixels[i][j][1] = tempImage[i][j];
                pixels[i][j][2] = tempImage[i][j];
                if (pixels[i][j][0] == 2) {
                    pixels[i][j][0] = 0;
                    pixels[i][j][1] = 0;
                    pixels[i][j][2] = 0;
                }
            }
        }
        int temp2 = maxVal;
        maxVal = 1;
        createPPM("image1.ppm");
        cout << "image1.ppm created" << endl;
        maxVal = temp2;
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                if (i != 0 && j != 0 && j != width-1 && i != height-1) {
                    int value = magnitudes[i][j];
                    tempA = angles[i][j];
                    if (tempA == 0) {
                        if (value >= magnitudes[i][j+1] && value >= magnitudes[i][j-1]) {
                            value = 1;
                        } else {
                            value = 0;
                        }
                    } else if (tempA == 135) {
                        if (value >= magnitudes[i-1][j+1] && value >= magnitudes[i+1][j-1]) {
                            value = 1;
                        } else {
                            value = 0;
                        }
                    } else if (tempA == 90) {
                        if (value >= magnitudes[i+1][j] && value >= magnitudes[i-1][j]) {
                            value = 1;
                        } else {
                            value = 0;
                        }
                    } else {
                        if (value >= magnitudes[i+1][j+1] && value >= magnitudes[i-1][j-1]) {
                            value = 1;
                        } else {
                            value = 0;
                        }
                    }
                    pixels[i][j][0] = value;
                    pixels[i][j][1] = value;
                    pixels[i][j][2] = value;
                } else {
                    pixels[i][j][0] = 1;
                    pixels[i][j][1] = 1;
                    pixels[i][j][2] = 1;
                }
            }
        }
        temp2 = maxVal;
        maxVal = 1;
        createPPM("image2.ppm");
        cout << "Created image2.ppm" << endl;
        maxVal = temp2;
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                if (tempImage[i][j] == 1 && pixels[i][j][0] == 1) {
                    pixels[i][j][0] = 1;
                    pixels[i][j][1] = 1;
                    pixels[i][j][2] = 1;
                } else {
                    pixels[i][j][0] = 0;
                    pixels[i][j][1] = 0;
                    pixels[i][j][2] = 0;
                }
            }
        }
        temp2 = maxVal;
        maxVal = 1;
        createPPM(filename2);
        cout << "Created imagef.ppm" << endl;
        maxVal = temp2;
        return angles2;
    }
    vector<Point> findCenter(int threshold1, int threshold2, int votes, int numSquares, string filename2, string filename3, string filename4, string filename5, string filename6, string filename7){
        vector<vector<int>>edges(height, vector<int> (width, 0));
        vector <int> sq;
        vector <Point> centers;
        vector<vector<vector <int>>> original;
        vector <vector<int>> addedValues(height, vector<int> (width, 0));
        copy(pixels.begin(), pixels.end(), back_inserter(original));
        vector<vector<double>> angles = canny(threshold1, threshold2, filename2, filename3, filename4, filename5, filename6, filename7);
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                edges[i][j] = pixels[i][j][0];
            }
        }
        int sqN = sqrt(numSquares);
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                if (edges[i][j] == 1) {
                    nLin(Point(j, i), Point(((double)j)+width*cos(angles[i][j]), ((double)i)+width*sin(angles[i][j])), 255, 0,0, true);
                }
            }
        }
        int max = 0;
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                if (inc[i][j] > max) {
                    max = inc[i][j];
                }
            }
        }
        ofstream outfile(filename3);
        outfile << "P3 " << endl;
        outfile << width << " " << height << endl;
        outfile << maxVal << endl;
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                outfile << inc[i][j] << " " << inc[i][j] << " " << inc[i][j] << " ";
            }
        }
        outfile.close();
        cout<< "created imagev.ppm" << endl;
        for (int l = 0; l < sqN; l++) {
            for (int k = 0; k < sqN; k++) {
                int averageAmount = 0;
                for (int i = 0; i < height / sqN; i++) {
                    for (int j = 0; j < width / sqN; j++) {
                        averageAmount += inc[i + ((height / sqN) * l)][j + ((width / sqN) * k)];
                    }
                }
                sq.push_back((averageAmount / ((height / sqN) * (width / sqN))) * 1.3);
            }
        }
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                illuminate(j, i, original[i][j][0], original[i][j][1], original[i][j][2], false);
            }
        }
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                if (inc[i][j] >= votes && abs(((original[i][j][0] + original[i][j][1] + original[i][j][2]) / 3) - 200) > 20 && abs(((original[i][j][0] +original[i][j][1] + original[i][j][2]) / 3) - 170) > 20) {
                    if (sq[floor(i / (height / sqN)) * sqN + ceil(j / (width / sqN))] > votes) {
                        if (inc[i][j] > sq[floor(i / (height / sqN)) * sqN + ceil(j / (width / sqN))]) {
                            centers.push_back(Point(j, i));
                            drawcircle(j, i, 2, 255, 0, 0, false);
                            drawcircle(j, i, 3, 255, 0, 0, false);
                            drawcircle(j, i, 4, 255, 0, 0, false);
                            illuminate(j, i, 255, 0, 0, false);
                        }
                    } else {
                        centers.push_back(Point(j, i));
                        drawcircle(j, i, 2, 255, 0, 0, false);
                        drawcircle(j, i, 3, 255, 0, 0, false);
                        drawcircle(j, i, 4, 255, 0, 0, false);
                        illuminate(j, i, 255, 0, 0, false);
                    }
                }
            }
        }
        createPPM(filename4);
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                pixels[i][j][0] = edges[i][j];
                pixels[i][j][1] = edges[i][j];
                pixels[i][j][2] = edges[i][j];
            }
        }
        cout << "Found Candidate Centers: " << centers.size() << endl;
        return centers;
    }
    double pCircle(int cx, int cy, int r) {
        double tots = 0;
        double children = 0;
        int xmax = (int)(r * 0.70710678);
        int y = r;
        int y2 = y * y;
        int ty = (2 * y) - 1;
        int y2_new = y2;
        for (int x = 0; x <= xmax + 2; x++) {
            if ((y2 - y2_new) >= ty) {
                y2 -= ty;
                y -= 1;
                ty -= 2;
            }
            int tempcx = cx + x;
            int tempcy = cy + y;
            if (tempcx < width && tempcx >= 0 && tempcy < height && tempcy >= 0 && pixels[tempcy][tempcx][0] == 1) {
                children++;
            }
            tempcy = cy - y;
            if (tempcx < width && tempcx >= 0 && tempcy < height && tempcy >= 0 && pixels[tempcy][tempcx][0] == 1) {
                children++;
            }
            tempcx = cx - x;
            tempcy = cy + y;
            if (tempcx < width && tempcx >= 0 && tempcy < height && tempcy >= 0 && pixels[tempcy][tempcx][0] == 1) {
                children++;
            }
            tempcy = cy - y;
            if (tempcx < width && tempcx >= 0 && tempcy < height && tempcy >= 0 && pixels[tempcy][tempcx][0] == 1) {
                children++;
            }
            tempcx = cx + y;
            tempcy = cy + x;
            if (tempcx < width && tempcx >= 0 && tempcy < height && tempcy >= 0 && pixels[tempcy][tempcx][0] == 1) {
                children++;
            }
            tempcy = cy - x;
            if (tempcx < width && tempcx >= 0 && tempcy < height && tempcy >= 0 && pixels[tempcy][tempcx][0] == 1) {
                children++;
            }
            tempcx = cx - y;
            tempcy = cy + x;
            if (tempcx < width && tempcx >= 0 && tempcy < height && tempcy >= 0 && pixels[tempcy][tempcx][0] == 1) {
                children++;
            }
            tempcy = cy - x;
            if (tempcx < width && tempcx >= 0 && tempcy < height && tempcy >= 0 && pixels[tempcy][tempcx][0] == 1) {
                children++;
            }
            tots += 8;
            y2_new -= (2 * x) - 3;
        }
        return children / tots;
    }
    void add(vector<Point> &rcs, vector<Point> &com, Point p, vector<vector<double>> &percent, int number, int size) {
        for (int i = 0; i < (int) rcs.size(); i++) {
            if (p.coinEquals(rcs[i])) {
                if (percent[number][size] > percent[com[i].getX()][com[i].getY()]) {
                    com[i] = Point(number, size);
                    rcs[i] = p;
                }
                return;
            }
        }
        rcs.push_back(p);
        com.push_back(Point(number, size));
    }
    void findCoins(int threshold1, int threshold2, int votes, int numSquares, double circlePercentage, int smallestRadius, string filename2, string filename3, string filename4, string filename5, string filename6, string filename7, bool detect) {
        int coinSize = 180-smallestRadius;
        int start = smallestRadius;
        vector<Point> centers = findCenter(threshold1, threshold2, votes, numSquares, filename2, filename3, filename4, filename5, filename6, filename7);
        vector<vector<double>> percent(centers.size(), vector<double> (coinSize, 0));
        double max = 0;
        vector<Point> rc;
        vector<Point> com;
        double amount = 0;
        int coinP = 0;
        int coinN = 0;
        int coinD = 0;
        int coinQ = 0;
        int coinH = 0;
        vector<int> cTypes;
        for (int num = 0; num < (int)centers.size(); num++) {
            for (int place = start; place < coinSize + start; place++) {
                percent[num][place - start] = pCircle(centers[num].getX(), centers[num].getY(), place);
            }
        }
        for (int i = 0; i < (int)centers.size(); i++) {
            for (int j = 0; j < coinSize; j++) {
                if (percent[i][j] > max) {
                    max = percent[i][j];
                }
            }
        }
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                pixels[i][j][0] = originialImage[i][j][0];
                pixels[i][j][1] = originialImage[i][j][1];
                pixels[i][j][2] = originialImage[i][j][2];
            }
        }
        for (int i = 0; i < (int) centers.size(); i++) {
            for (int j = 0; j < coinSize; j++) {
                if (percent[i][j] > circlePercentage * max) {
                    add(rc, com, centers[i], percent, i, j);
                }
            }
        }

        for (int i = 0; i < (int) rc.size(); i++) {
            int midRadius = (com[i].getY() + start) / 2;
            int tempR = 0;
            int tempB = 0;

            for (int k = -midRadius; k <= midRadius; k++) {
                for (int l = -midRadius; l <= midRadius; l++) {
                    tempR += pixels[rc[i].getY() + k][rc[i].getX() + l][0];
                    tempB += pixels[rc[i].getY() + k][rc[i].getX() + l][2];
                }
            }

            tempR /= (5 * midRadius * midRadius);
            tempB /= (5 * midRadius * midRadius);
            // cout << tempR << " " << tempG << " " << tempB << endl;
            if (com[i].getY() + start >= 150) {
                if (tempR > 1.3 * tempB) {
                    rc.erase(rc.begin() +i);
                    com.erase(com.begin() +i);
                    i--;
                    amount+=0;
                } else if (detect) {
                amount += 1;
                cTypes.push_back(5);
                coinH += 1;
                }
                else{
                    cTypes.push_back(6);
                }
            } else if (com[i].getY() + start >= 103) {
                if (tempR > 1.3 * tempB) {
                    rc.erase(rc.begin() +i);
                    com.erase(com.begin() +i);
                    i--;
                    amount+=0;
                } else {
                    amount += 0.25;
                    cTypes.push_back(1);
                    coinQ += 1;
                }
            }
            else if (com[i].getY() + start > 80 && com[i].getY() + start < 103) {
                if (tempR > 1.3 * tempB) {
                    amount += 0.01;
                    cTypes.push_back(3);
                    coinP += 1;
                } else {
                    amount += 0.05;
                    cTypes.push_back(2);
                    coinN += 1;
                }
            } else {
                if (tempR > 1.3 * tempB) {
                    amount += 0.01;
                    cTypes.push_back(3);
                    coinP += 1;
                } else {
                    if(detect){
                        coinD += 1;
                        amount += 0.1;
                        cTypes.push_back(4);
                    }
                    else{
                        cTypes.push_back(6);
                    }
                    
                }
            }
        }
        ofstream out(filename5);
        for (int i = 0; i < (int) rc.size(); i++) {
            if (cTypes[i] == 1 || cTypes[i] == 2 || cTypes[i] == 3 || cTypes[i] == 4 || cTypes[i] == 5) {
                drawcircle(rc[i].getX(), rc[i].getY(), com[i].getY()+start, 255, 0, 0, false);
                drawcircle(rc[i].getX(), rc[i].getY(), com[i].getY()+start+1, 255, 0, 0, false);
                drawcircle(rc[i].getX(), rc[i].getY(), com[i].getY()+start-1, 255, 0, 0, false);
                drawcircle(rc[i].getX(), rc[i].getY(), 2, 255, 0, 0, false);
                drawcircle(rc[i].getX(), rc[i].getY(), 3, 255, 0, 0, false);
                drawcircle(rc[i].getX(), rc[i].getY(), 4, 255, 0, 0, false);
                illuminate(rc[i].getX(), rc[i].getY(), 255, 0, 0, false);
            }
            else {
                //do nothing
                //used to trash coins
            }
        }
        createPPM(filename5);
        cout <<"created imageCircles.ppm" << endl;
        cout <<"Classified Found Coins" << endl;
        for (int i = 0; i < (int) rc.size(); i++) { //5 is dollar, 1 is quarter, 2 is nickel, 3 is penny, 4 is dime, 6 is get rid of
            if (cTypes[i] == 1) {
                drawcircle(rc[i].getX(), rc[i].getY(), com[i].getY()+start, 128, 0, 128, false);
                drawcircle(rc[i].getX(), rc[i].getY(), com[i].getY()+start+1, 128, 0, 128, false);
                drawcircle(rc[i].getX(), rc[i].getY(), com[i].getY()+start-1, 128, 0, 128, false);
            } else if (cTypes[i] == 2) {
                drawcircle(rc[i].getX(), rc[i].getY(), com[i].getY()+start, 255, 255, 0, false);
                drawcircle(rc[i].getX(), rc[i].getY(), com[i].getY()+start+1, 255, 255, 0, false);
                drawcircle(rc[i].getX(), rc[i].getY(), com[i].getY()+start-1, 255, 255, 0, false);
            } else if (cTypes[i] == 3) {
                drawcircle(rc[i].getX(), rc[i].getY(), com[i].getY()+start, 255, 0, 0, false);
                drawcircle(rc[i].getX(), rc[i].getY(), com[i].getY()+start+1, 255, 0, 0, false);
                drawcircle(rc[i].getX(), rc[i].getY(), com[i].getY()+start-1, 255, 0, 0, false);
            } else if (cTypes[i] == 4) {
                drawcircle(rc[i].getX(), rc[i].getY(), com[i].getY()+start, 0, 0, 255, false);
                drawcircle(rc[i].getX(), rc[i].getY(), com[i].getY()+start+1, 0, 0, 255, false);
                drawcircle(rc[i].getX(), rc[i].getY(), com[i].getY()+start-1, 0, 0, 255, false);
            } else if(cTypes[i] == 5) {//236, 67, 248
                drawcircle(rc[i].getX(), rc[i].getY(), com[i].getY()+start, 236, 67, 248, false);
                drawcircle(rc[i].getX(), rc[i].getY(), com[i].getY()+start+1, 236, 67, 248, false);
                drawcircle(rc[i].getX(), rc[i].getY(), com[i].getY()+start-1, 236, 67, 248, false);
            }
            else {
                //do nothing
                //used to trash coins
            }
        }
        cout << "Half Dollar Coins: " << 0 << endl;
        cout << "Dollar Coins: " << coinH << endl;
        cout << "Quarters: " << coinQ << endl;
        cout << "Dimes: " << coinD << endl;
        cout << "Nickels: " << coinN << endl;
        cout << "Pennies: " << coinP << endl;
        cout << "Total Sum: $" << amount << endl;

        ofstream outfile(filename7);
        outfile << "Half Dollar Coins: " << 0 << endl;
        outfile << "Dollar Coins: " << coinH << endl;
        outfile << "Quarters: " << coinQ << endl;
        outfile << "Dimes: " << coinD << endl;
        outfile << "Nickels: " << coinN << endl;
        outfile << "Pennies: " << coinP << endl;
        outfile << "Total Sum: $" << amount << endl;
        outfile.close();

        createPPM(filename6);
        cout << "Created imageCoins.ppm" << endl;
    }
};

void part3(int argc, char *argv[]) {
    
    int threshold1 = 70;
    int threshold2 = 200;
    int vote = 60;
    int sq = 2400;
    double percent = 0.31;
    int smallestRadius = 70;
    string filename = "image.ppm";
    string filename2 = "imagef.ppm";
    string filename3 = "imagev.ppm";
    string filename4 = "imageCC.ppm";
    string filename5 = "imageCircles.ppm";
    string filename6 = "imageCoins.ppm";
    string filename7 = "results.txt";
    bool image3 = false;

    // ./l062 -f myimg.ppm -lt 150 -ht 200 -ff myimagef.ppm -TC 300 -fv myimagev.ppm -fcc myimageCC.ppm -TCircle 55 -fCi myimageCircles.ppm -fCo myimageCoins.ppm -fR myresults.txt
    for(int i = 0; i < argc; i++){

        if(strcmp(argv[i], "-f") == 0){
            filename = argv[i+1];
        }
        else if(strcmp(argv[i], "-i3") == 0){
            if(strcmp(argv[i+1], "true") == 0){
                image3 = true;
            }
            else if(strcmp(argv[i+1], "t") == 0){
                image3 = true;
            }
            if(image3){
                threshold1 = 70;
                threshold2 = 100;
                vote = 75;
                sq = 169;
                percent = 0.31;
                smallestRadius = 70;
            }
        }
        else if(strcmp(argv[i], "-lt") == 0){
            threshold1 = stoi(argv[i+1]);
        }
        else if(strcmp(argv[i], "-ht") == 0){
            threshold2 = stoi(argv[i+1]);
        }
        else if(strcmp(argv[i], "-ff") == 0){
            filename2 = argv[i+1];
        }
        else if(strcmp(argv[i], "-TC") == 0){
            vote = stoi(argv[i+1]);
        }
        else if(strcmp(argv[i], "-fv") == 0){
            filename3 = argv[i+1];
        }
        else if(strcmp(argv[i], "-fcc") == 0){
            filename4 = argv[i+1];
        }
        else if(strcmp(argv[i], "-TCircle") == 0){
            percent = stod(argv[i+1])/100.0;
        }
        else if(strcmp(argv[i], "-fCi") == 0){
            filename5 = argv[i+1];
        }
        else if(strcmp(argv[i], "-fCo") == 0){
            filename6 = argv[i+1];
        }
        else if(strcmp(argv[i], "-fR") == 0){
            filename7 = argv[i+1];
        }
        else if(strcmp(argv[i], "-sR") == 0){
            smallestRadius = stoi(argv[i+1]);
        }
        else if(strcmp(argv[i], "-sQ") == 0){
            sq = stoi(argv[i+1]);
        }
        else if(strcmp(argv[i], "-sR") == 0){
            smallestRadius = stoi(argv[i+1]);
        }
        else{
            //do nothing
        }  
    }
    cout << "image: " << filename << endl; 
    Render render = Render(filename);
    render.findCoins(threshold1, threshold2, vote, sq, percent, smallestRadius, filename2, filename3, filename4, filename5, filename6, filename7, image3);
}

int main(int argc, char *argv[]) {
    part3(argc, argv);
}