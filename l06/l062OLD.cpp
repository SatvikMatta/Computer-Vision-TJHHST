//Satvik Matta
//Febuary 10, 2022
#include <fstream>
#include <math.h>
#include <string>
#include <vector>
#include <iostream>
#include <cstring>
#include <sstream>
using namespace std;

class Point {
    private:
        double x;
        double y;
    public:
        Point() {
            this -> x = 0;
            this -> y = 0;
        }
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
        void setX(double x) {
            this -> x = x;
        }
        void setY(double y) {
            this -> y = y;
        }
        double calcDistance(Point pt2) {
            return sqrt(pow((pt2.getX() - x), 2) + pow((pt2.getY() - y), 2));
        }
        double calcDistanceNS(Point pt2) {
            return pow((pt2.getX() - x), 2) + pow((pt2.getY() - y), 2);
        }
        string toString() {
            ostringstream temp;
            temp << fixed << setprecision(23) << "(" << x << "," << y << ")";
            return temp.str();
        }
        bool coinEquals(Point p2) {
            if (x <= p2.getX() + 85 && x >= p2.getX() - 85 && y <= p2.getY() + 85 && y >= p2.getY() - 85) {
                return false;
            } else {
                return true;
            }
        }
};


// Create a Renderer Class
class Render
{
private:
    vector<vector<int>> array;

public:
    Render(vector<vector<int>> array)
    {
        this->array = array;
    }
    vector<vector<int>> getarray()
    {
        return array;
    }
    void illuminate(int x, int y)
    {
        if (x >= 0 && y >= 0 && x < (int) array.size() && y < (int) array[0].size()){
            array[x][y] += 1;
        }
        // else{
        //     cout << "Skipped: Out of bounds" << endl;
        // }
    }
    void color(int x, int y, int c)
    {
        if (x >= 0 && y >= 0 && x < (int) array.size() && y < (int) array[0].size()){
            array[x][y] = c;
        }
    }
    void drawcircle(int xc, int yc, double r)
    {
        int xmax = (int)(r * 0.70710678);
        int y = r;
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
            illuminate(xc + x, yc + y);
            illuminate(xc + x, yc - y);
            illuminate(xc - x, yc + y);
            illuminate(xc - x, yc - y);
            illuminate(xc + y, yc + x);
            illuminate(xc + y, yc - x);
            illuminate(xc - y, yc + x);
            illuminate(xc - y, yc - x);
            y2_new -= (2 * x) - 3;
        }
    }
    void coldrawcircle(int xc, int yc, double r, int c)
    {
        int xmax = (int)(r * 0.70710678);
        int y = r;
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
            color(xc + x, yc + y, c);
            color(xc + x, yc - y, c);
            color(xc - x, yc + y, c);
            color(xc - x, yc - y, c);
            color(xc + y, yc + x, c);
            color(xc + y, yc - x, c);
            color(xc - y, yc + x, c);
            color(xc - y, yc - x, c);
            y2_new -= (2 * x) - 3;
        }
    }
    int getmax(){
        int maxvalue = 0;
        for (int i = 0; i < (int) array.size(); i++){
            for (int j = 0; j < (int) array[0].size(); j++){
                if (array[i][j] > maxvalue){
                maxvalue = array[i][j];
                }   
            }
        }
        return maxvalue;
    }
    void bres1(int x1, int y1, int x2, int y2)
    {
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
            illuminate(i, j);
            if (error >= 0)
            {
                j++;
                error = error - dx;
            }
            error = error + dy;
        }
    }

    void bres2(int x1, int y1, int x2, int y2)
    {
        if (x1 > x2)
        {
            int temp = x1;
            x1 = x2;
            x2 = temp;
            temp = y1;
            y1 = y2;
            y2 = temp;
        }
        int negy1 = (int)array[0].size() - 1 - y1;
        int negy2 = (int)array[0].size() - 1 - y2;

        int dx = x2 - x1;
        int dy = negy2 - negy1;
        int j = negy1;
        int error = dy - dx;
        for (int i = x1; i < x2; i++)
        {
            illuminate(i, (int)array[0].size() - 1 - j);
            if (error >= 0)
            {
                j++;
                error = error - dx;
            }
            error = error + dy;
        }
    }

    void bres3(int x1, int y1, int x2, int y2)
    {
        if (y1 > y2)
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
        int j = x1;
        int error = dx - dy;

        for (int i = y1; i < y2; i++)
        {
            illuminate(j, i);
            if (error >= 0)
            {
                j++;
                error = error - dy;
            }
            error = error + dx;
        }
    }

void bres4(int x1, int y1, int x2, int y2){
    if (y1 > y2) {
        int temp = x1;
        x1 = x2;
        x2 = temp;
        temp = y1;
        y1 = y2;
        y2 = temp;
    }
    int negx1 = (int)array.size()-1 - x1;
    int negx2 = (int)array.size()-1 - x2;

    int dy = y2 - y1;
    int dx = negx2 - negx1;
    int j = negx1;
    int error = dx - dy;
    for (int i = y1; i < y2; i++) {
        illuminate((int)array.size()-1-j, i);
        if (error >= 0) {
            j++;
            error = error - dy;
        }
        error = error + dx;
    }
    
}

    void createppm(string name, int max)
    {
        ofstream myfile;
        myfile.open(name + string(".ppm"));
        myfile << "P3\n"
               << (int) array[0].size() << " " << (int) array.size() << "\n" << max << "\n";
        for (int i = 0; i < (int)array.size(); ++i)
        {
            for (int j = 0; j < (int) array[0].size(); ++j)
            {
                myfile << array[i][j] << " ";
                myfile << array[i][j] << " ";
                myfile << array[i][j] << " ";
            }
            myfile << std::endl;
        }
        myfile.close();
    }

    void drawstraight(int x1, int x2, int y1, int y2)
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
                illuminate(x1, i);
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
                illuminate(i, y1);
            }
        }
    }

    void drawline(int x1, int x2, int y1, int y2)
    {
        if (x1 == x2 || y1 == y2)
        {
            drawstraight(x1, x2, y1, y2);
            return;
        }
        int dx = x2 - x1;
        int dy = y2 - y1;
        if (abs(dx) >= abs(dy))
        {
            if ((double)dx / dy > 0)
            {
                bres1(x1, y1, x2, y2);
            }
            else
            {
                bres2(x1, y1, x2, y2);
            }
        }
        else if (abs(dx) < abs(dy))
        {
            if ((double)dx / dy > 0)
            {
                bres3(x1, y1, x2, y2);
            }
            else
            {
                bres4(x1, y1, x2, y2);
            }
        }
        illuminate(x1, y1);
        illuminate(x2, y2);
    }
};

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

int checkifedge(int i, int j, vector<vector<int>> &matrix){
    if (i < 0 || i >= (int) matrix.size() || j < 0 || j >=  (int) matrix[0].size()){
        return 0;
        cout << "out of bounds" << endl;
    }
    if(matrix[i][j]==1){
        return 1;
    }
    return 0;
}

double percentageCircle(int cx, int cy, int r, int width, int height, vector<vector<int>> &image) {
        double totalPixels = 0;
        double capturedPixels = 0;
        int hux = 0;
        int huy = 0;
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
            hux = cx + x;
            huy = cy + y;
            if (hux < width && hux >= 0 && huy < height && huy >= 0 && image[huy][hux] == 1) {
                capturedPixels++;
            }
            huy = cy - y;
            if (hux < width && hux >= 0 && huy < height && huy >= 0 && image[huy][hux] == 1) {
                capturedPixels++;
            }
            hux = cx - x;
            huy = cy + y;
            if (hux < width && hux >= 0 && huy < height && huy >= 0 && image[huy][hux] == 1) {
                capturedPixels++;
            }
            huy = cy - y;
            if (hux < width && hux >= 0 && huy < height && huy >= 0 && image[huy][hux] == 1) {
                capturedPixels++;
            }
            hux = cx + y;
            huy = cy + x;
            if (hux < width && hux >= 0 && huy < height && huy >= 0 && image[huy][hux] == 1) {
                capturedPixels++;
            }
            huy = cy - x;
            if (hux < width && hux >= 0 && huy < height && huy >= 0 && image[huy][hux] == 1) {
                capturedPixels++;
            }
            hux = cx - y;
            huy = cy + x;
            if (hux < width && hux >= 0 && huy < height && huy >= 0 && image[huy][hux] == 1) {
                capturedPixels++;
            }
            huy = cy - x;
            if (hux < width && hux >= 0 && huy < height && huy >= 0 && image[huy][hux] == 1) {
                capturedPixels++;
            }
            totalPixels += 8;
            y2_new -= (2 * x) - 3;
        }
        return capturedPixels / totalPixels;
}

void coindetect(vector<vector<int>> &imagef, vector<Point> &centers, string inputfile){
    cout << "center size: " << centers.size() << endl;
    int coinsize = 180-70;
    int start = 70;
    vector<vector<double>> percent(centers.size(), vector<double>(coinsize,0));
    double max= 0;
    vector<Point> com;
    vector<Point> realCenter;
    int width;
    int height;
    int m;
    double amount = 0;
    int gR = 0;
    int gB = 0;
    int coinP = 0;
    int coinN = 0;
    int coinD = 0;
    int coinQ = 0;
    int coinS = 0;
    vector<int> cType;

    vector<vector<vector<int>>> pixels;
    vector<vector<vector<int>>> oimage;
    ifstream fin(inputfile);
    string temp;
    fin >> temp;
    fin >> width;
    fin >> height;
    fin >> m;
    pixels.resize(height);
    for(int i = 0; i < height; i++){
        pixels[i].resize(width);
        for(int j = 0; j < width; j++){
            pixels[i][j].resize(3);
        }
    }
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            for(int k = 0; k < 3; k++){
                fin >> pixels[i][j][k];
            }
        }
    }

    for (int num = 0; num < (int)centers.size(); num++) {
            for (int p = start; p < coinsize + start; p++) {
                percent[num][p - start] = percentageCircle(centers[num].getX(), centers[num].getY(), p, width, height, imagef);
        }
    }
    for(int i = 0; i < (int)centers.size(); i++){
        for(int j = 0; j < coinsize; j++){
            if(percent[i][j] > max){
                max = percent[i][j];
            }
        }
    }
    cout << "max: " << max << endl;
    cout << "percent size: " << percent.size() << endl;
    //print out percent matrix to a txt file
    // ofstream fout("percent.txt");
    // for(int i = 0; i < (int)percent.size(); i++){
    //     for(int j = 0; j < coinsize; j++){
    //         fout << percent[i][j] << " ";
    //     }
    //     fout << endl;
    // }
    int count = 0;
    for(int i = 0; i<(int)centers.size(); i++){
        for(int j = 0; j< coinsize; j++){
            if(percent[i][j] >= .3 * max){
                for(int k = 0; k<realCenter.size();k++){
                    if(centers[i].coinEquals(realCenter[k])){
                        count++;
                        if(percent[i][j] > percent[com[k].getX()][com[k].getY()]){
                            com[k] = Point(i,j);
                            realCenter[k] = centers[i];
                        }
                        break;
                    }
                }
                realCenter.push_back(centers[i]);
                com.push_back(Point(i,j));
            }
        }
    }
    cout << "real center size: " << realCenter.size() << endl;
    cout << "count: " << count << endl;
    cout << "com size: " << com.size() << endl;
    for(int i = 0; i < (int)realCenter.size(); i++){
        // cout << "i: " << i << endl;
        int throwy = (com[i].getY() + start) / 2;
        // cout << "throwy: " << throwy << endl;
        gR = (pixels[realCenter[i].getY()][realCenter[i].getX()][0]+pixels[realCenter[i].getY()+throwy][realCenter[i].getX()][0]+pixels[realCenter[i].getY()][realCenter[i].getX()+throwy][0]+pixels[realCenter[i].getY()-throwy][realCenter[i].getX()][0]+pixels[realCenter[i].getY()][realCenter[i].getX()-throwy][0])/5;
        gB = (pixels[realCenter[i].getY()][realCenter[i].getX()][2]+pixels[realCenter[i].getY()+throwy][realCenter[i].getX()][2]+pixels[realCenter[i].getY()][realCenter[i].getX()+throwy][2]+pixels[realCenter[i].getY()-throwy][realCenter[i].getX()][2]+pixels[realCenter[i].getY()][realCenter[i].getX()-throwy][2])/5;
        if(com[i].getY() + start >= 150){
            if(gR > 1.3 * gB){
                realCenter.erase(realCenter.begin() + i);
                com.erase(com.begin() + i);
                i--;
            }
            else{
                amount+=1;
                cType.push_back(5);
                coinS += 1;
            }
        }
        else if(com[i].getY() + start >= 103){
            if(gR > 1.3 * gB){
                realCenter.erase(realCenter.begin() + i);
                com.erase(com.begin() + i);
                i--;
            }
            else{
                amount+=.25;
                cType.push_back(1);
                coinQ += 1;
            }
        }
        else if(com[i].getY() > 80 ){
            if(gR > 1.3 * gB){
                amount += .01;
                cType.push_back(3);
                coinP += 1;
            }
            else{
                amount+=.05;
                cType.push_back(2);
                coinN += 1;
            }
        }
        else{
            if(gR > 1.3 * gB){
                amount += .01;
                cType.push_back(3);
                coinP += 1;
            }
            else{
                amount+=.1;
                cType.push_back(4);
                coinD += 1;
            }
        }
    }
    cout << "real center size: " << realCenter.size() << endl;
    vector<vector<int>> circlem;
    for(int i = 0; i < height; i++){
        vector<int> row;
        for(int j = 0; j < width; j++){
            row.push_back(0);
        }
        circlem.push_back(row);
    }
    Render circlemap(circlem);
    for(int i = 0; (int)realCenter.size(); i++){
        if(cType[i] == 1){
            circlemap.coldrawcircle(realCenter[i].getX(), realCenter[i].getY(), com[i].getY() + start, -1);
            circlemap.coldrawcircle(realCenter[i].getX(), realCenter[i].getY(), com[i].getY() + start+1, -1);
            circlemap.coldrawcircle(realCenter[i].getX(), realCenter[i].getY(), com[i].getY() + start-1, -1);
        }
        else if(cType[i] == 2){
            circlemap.coldrawcircle(realCenter[i].getX(), realCenter[i].getY(), com[i].getY() + start, -2);
            circlemap.coldrawcircle(realCenter[i].getX(), realCenter[i].getY(), com[i].getY() + start+1, -2);
            circlemap.coldrawcircle(realCenter[i].getX(), realCenter[i].getY(), com[i].getY() + start-1, -2);
        }
        else if(cType[i] == 3 ){
            circlemap.coldrawcircle(realCenter[i].getX(), realCenter[i].getY(), com[i].getY() + start, -3);
            circlemap.coldrawcircle(realCenter[i].getX(), realCenter[i].getY(), com[i].getY() + start+1, -3);
            circlemap.coldrawcircle(realCenter[i].getX(), realCenter[i].getY(), com[i].getY() + start-1, -3);
        }
        else if(cType[i] == 4){
            circlemap.coldrawcircle(realCenter[i].getX(), realCenter[i].getY(), com[i].getY() + start, -4);
            circlemap.coldrawcircle(realCenter[i].getX(), realCenter[i].getY(), com[i].getY() + start+1, -4);
            circlemap.coldrawcircle(realCenter[i].getX(), realCenter[i].getY(), com[i].getY() + start-1, -4);
        }
        else if(cType[i] == 5){
            circlemap.coldrawcircle(realCenter[i].getX(), realCenter[i].getY(), com[i].getY() + start, -5);
            circlemap.coldrawcircle(realCenter[i].getX(), realCenter[i].getY(), com[i].getY() + start+1, -5);
            circlemap.coldrawcircle(realCenter[i].getX(), realCenter[i].getY(), com[i].getY() + start-1, -5);
        }
    }
    circlem = circlemap.getarray();
    for(int i = 0; i<height; i++){
        for(int j = 0; j<width; j++){ // quarter: 255, 192, 200, nickel: 255, 255, 0, dime: blue, penny: red, dollar: green
            if(circlem[i][j] == -1){
                pixels[i][j][0] = 255;
                pixels[i][j][1] = 192;
                pixels[i][j][2] = 200;
            }
            else if(circlem[i][j] == -2){
                pixels[i][j][0] = 255;
                pixels[i][j][1] = 255;
                pixels[i][j][2] = 0;
            }
            else if(circlem[i][j] == -3){
                pixels[i][j][0] = 255;
                pixels[i][j][1] = 0;
                pixels[i][j][2] = 0;
            }
            else if(circlem[i][j] == -4){
                pixels[i][j][0] = 0;
                pixels[i][j][1] = 0;
                pixels[i][j][2] = 255;
            }
            else if(circlem[i][j] == -5){
                pixels[i][j][0] = 0;
                pixels[i][j][1] = 255;
                pixels[i][j][2] = 0;
            }
        }
    }
    cout << "Amount: $" << amount << endl;
    cout << "Number of Quarters: " << coinQ << endl;
    cout << "Number of Nickels: " << coinN << endl;
    cout << "Number of Dimes: " << coinD << endl;
    cout << "Number of Pennies: " << coinP << endl;
    cout << "Number of Dollars: " << coinS << endl;

    //convert pixels to ppm
    ofstream output;
    output.open("coins.ppm");
    output << "P3" << endl;
    output << width << " " << height << endl;
    output << "255" << endl;
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            output << pixels[i][j][0] << " " << pixels[i][j][1] << " " << pixels[i][j][2] << " ";
        }
        output << endl;
    }
    output.close();
    cout << "coins.ppm created" << endl;
    return;
}

void midpointdetect(vector<vector<int>> &gx, vector<vector<int>> &gy, vector<vector<int>> &imagef, int width, int height, string inputfile, int threshold, bool pArgs){
    int maxcoinsize = 125;
    if (pArgs){
        if(height > width){
            maxcoinsize = height/2;
        }
        else{
            maxcoinsize = width/2;
        }
    }
    vector<vector<int>> voting;
    for (int i = 0; i < height; i++){
        vector<int> row;
        for (int j = 0; j < width; j++){
            row.push_back(0);
        }
        voting.push_back(row);
    }
    Render votemap = Render(voting);

    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            if(imagef[i][j] == 1){
                double slope;
                if (gx[i][j] == 0){
                    slope = 20000;
                }
                else{
                    slope = (double)gy[i][j]/(double)gx[i][j];
                }
                int minx = j - maxcoinsize;
                int maxx = j + maxcoinsize;
                int miny = i - (int)(maxcoinsize * slope);
                int maxy = i + (int)(maxcoinsize * slope);
                votemap.drawline(miny, maxy, minx, maxx);
            }
        }
    }
    int maxvalue = votemap.getmax();
    cout << "Max Value: " << maxvalue << endl;
    votemap.createppm("imagev", maxvalue);
    cout << "Finished Voting Map" << endl;
    vector<vector<int>> middle;
    for (int i = 0; i < height; i++){
        vector<int> row;
        for (int j = 0; j < width; j++){
            row.push_back(0);
        }
        middle.push_back(row);
    }
    int count = 0;
    voting = votemap.getarray(); 
    vector<Point> centers;
    Render middlemap = Render(middle);
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            int thres = threshold;
            if (i > 0.4*height && i < 0.9*height && j < 0.4*width && pArgs == false)
            {
                thres = 1.8*threshold;
            }
            if (i > 0.4*height && i < 0.9*height && j > 0.4*width && pArgs == false)
            {
                thres = 1.5*threshold;
            }
            if(voting[i][j] > thres){
                centers.push_back(Point(j,i));
                middlemap.drawcircle(i, j, 2);
                middlemap.drawcircle(i, j, 3);
                count++;
            }
        }
    }
    middle = middlemap.getarray();
    cout << "Number of coins: " << count << endl;
    //Load initial r,g,b image from image.ppm
    vector<vector<int>> r;
    vector<vector<int>> g;
    vector<vector<int>> b;
    for(int i = 0; i < height; i++){
        vector<int> row;
        for(int j = 0; j < width; j++){
            row.push_back(0);
        }
        r.push_back(row);
        g.push_back(row);
        b.push_back(row);
    }
    ifstream infile(inputfile);
    string line;
    int width1, height1, maxval1;
    infile >> line;
    infile >> width1 >> height1 >> maxval1;

    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            infile >> r[i][j];
            infile >> g[i][j];
            infile >> b[i][j];
        }
    }
    infile.close();
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            if(middle[i][j] >= 1){
                r[i][j] = 255;
                g[i][j] = 0;
                b[i][j] = 0;
            }
        }
    }
    ofstream outfile("imageCC.ppm");
    outfile << "P3" << endl;
    outfile << width1 << " " << height1 << endl;
    outfile << maxval1 << endl;
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            outfile << r[i][j] << " " << g[i][j] << " " << b[i][j] << " ";
        }
        outfile << endl;
    }
    
    outfile.close();

    cout << "Finished Midpoint Detection and Overlay" << endl;

    coindetect(imagef, centers, inputfile);
}

void canny(int argc, char *argv[]){

    // cout << "Part 3" << endl;

    string filename = "image.ppm";
    int lt = 70;
    int ht = 200;
    string outputfilename = "image1.ppm";
    string grayfilename = "imageg.ppm";
    string sobelfilename = "images.ppm";
    string image2 = "image2.ppm";
    string finalfilename = "imagef.ppm";
    int threshold = 22;
    bool pArgs = false;


    //parse command line
    for (int i = 0; i < argc; i++){
        //print argvi
//         cout << argv[i] << endl;
//         cout<< "i: " << i << endl;
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
        else if (strcmp(argv[i], "-TC") == 0){
            threshold = atoi(argv[i+1]);
            pArgs = true;
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
    
    cout << "Finished Intensity Matrix" << endl;


    //put image into 2d vector
    vector<vector<int>> image = imggray;
    vector<vector<int>> sobel;
    vector<vector<int>> gx;
    vector<vector<int>> gy;
    for (int i = 0; i < height; i++){
        vector<int> row;
        vector<int> rowx;
        vector<int> rowy;
        for (int j = 0; j < width; j++){
            if (i == 0 || i == height - 1 || j == 0 || j == width - 1){
                row.push_back(0);
                rowx.push_back(0);
                rowy.push_back(0);
            }
            else{
                int gx1 = -1*image[i-1][j-1] + -2*image[i][j-1] - image[i+1][j-1] + image[i-1][j+1] + 2*image[i][j+1] + image[i+1][j+1];
                int gy1 = -1*image[i-1][j-1] + -2*image[i-1][j] - image[i-1][j+1] + image[i+1][j-1] + 2*image[i+1][j] + image[i+1][j+1];
                int g = sqrt(gx1*gx1 + gy1*gy1);
                row.push_back(g);
                rowx.push_back(gx1);
                rowy.push_back(gy1);
            }
        }
        sobel.push_back(row);
        gx.push_back(rowx);
        gy.push_back(rowy);
    }

    // cout << "sobel" << endl;
     cout << "Finished Sobel" << endl;

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
     cout << "Finished Double Threshold and Hysteresis" << endl;

    //create theta matrix
    vector<vector<double>> theta;
    for (int i = 0; i < height; i++){
        vector<double> row2;
        for (int j = 0; j < width; j++){
            if (i == 0 || i == height - 1 || j == 0 || j == width - 1){
                row2.push_back(0);
            }
            else{
                double gx1 = gx[i][j];
                double gy1 = gy[i][j];
                double g = atan2(gy1, gx1);
                row2.push_back(g);
            }
        }
        theta.push_back(row2);
    }

    //convert theta matrix to degrees
    for (int i = 0; i < (int)theta.size(); i++) {
        for (int j = 0; j < (int)theta[i].size(); j++) {
            theta[i][j] = theta[i][j] * 180 / M_PI;
        }
    }
    
     cout << "Finished Creating Theta Matrix" << endl;
    
    // cout << "theta matrix" << endl;

    vector<vector<double>> angles = theta;
    for (int i = 0; i < (int)theta.size(); i++) {
        for (int j = 0; j < (int)theta[i].size(); j++) {
            double angle = theta[i][j];
            if (angle >=0.0 && angle < 45.0){
                if(angle <= 22.5){
                    angles[i][j] = 0;
                }
                else{
                    angles[i][j] = 45;
                }
            }
            else if (angle >= 45.0 && angle < 90.0){
                if(angle <= 67.5){
                    angles[i][j] = 45;
                }
                else{
                    angles[i][j] = 90;
                }
            }
            else if (angle >= 90.0 && angle < 135.0){
                if(angle <= 112.5){
                    angles[i][j] = 90;
                }
                else{
                    angles[i][j] = 135;
                }
            }
            else if (angle >= 135.0 && angle < 180.0){
                if(angle <= 157.5){
                    angles[i][j] = 135;
                }
                else{
                    angles[i][j] = 180;
                }
            }
            else if(angle > -180.0 && angle <= -135.0){
                if(angle > -157.5){
                    angles[i][j] = -135;
                }
                else{
                    angles[i][j] = 180;
                }
            }
            else if(angle > -135.0 && angle <= -90.0){
                if(angle > -112.5){
                    angles[i][j] = -90;
                }
                else{
                    angles[i][j] = -135;
                }
            }
            else if(angle > -90.0 && angle <= -45.0){
                if(angle > -67.5){
                    angles[i][j] = -45;
                }
                else{
                    angles[i][j] = -90;
                }
            }
            else if(angle > -45.0 && angle <= 0.0){
                if(angle > -22.5){
                    angles[i][j] = 0;
                }
                else{
                    angles[i][j] = -45;
            }
            }

        }
    }

    // cout << "theta matrix transform" << endl;
    cout << "Finished Theta Matrix Transform" << endl;
    
    vector<vector<int>> image21;
    image = sobel;
    theta = angles;
    //non-maximum suppression
    for (int i = 0; i < height; i++){
        vector<int> row3;
        for (int j = 0; j < width; j++){
            if (i == 0 || i == height - 1 || j == 0 || j == width - 1){
                row3.push_back(0);
            }
            else if(theta[i][j]== 180 || theta[i][j] == 0){
                if (image[i][j] >= image[i][j-1] && image[i][j] >= image[i][j+1]){
                    row3.push_back(1);
                }
                else{
                    row3.push_back(0);
                }
            }
            else if (theta[i][j] == -45 || theta[i][j] == 135){
                if(image[i][j] >= image[i-1][j-1] && image[i][j] >= image[i+1][j+1]){
                    row3.push_back(1);
                }
                else{
                    row3.push_back(0);
                }
            }
            else if (theta[i][j] == 45 || theta[i][j] == -135){
                if(image[i][j] >= image[i-1][j+1] && image[i][j] >= image[i+1][j-1]){
                    row3.push_back(1);
                }
                else{
                    row3.push_back(0);
                }
            }
            else if (theta[i][j] == 90 || theta[i][j] == -90){
                if(image[i][j] >= image[i-1][j] && image[i][j] >= image[i+1][j]){
                    row3.push_back(1);
                }
                else{
                    row3.push_back(0);
                }
            }
            
        }
        image21.push_back(row3);
    }

//     cout << "image21 dimensions: " << image21.size() << " " << image21[0].size() << endl;
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
    cout << "Finished Non-Maximum Suppression" << endl;

    // check if image1 and image2 and see if each pixel is the same
    // if it is the same, then it is an edge pixel
    // if it is not the same, then it is not an edge pixel
    vector<vector<int>> image3;
    for (int i = 0; i < height; i++){
        vector<int> row3;
        for (int j = 0; j < width; j++){
            // cout << "i: " << i << " j: " << j << endl;
            if ((dhyst[i][j] == 2 || dhyst[i][j] == 3) && image21[i][j] == 1){
                row3.push_back(1);
            }
            else{
                row3.push_back(0);
            }
        }
        image3.push_back(row3);
    }

//     cout << "image3 created" << endl;

    // create image3.ppm
    ofstream myfile3;
    myfile3.open(finalfilename);
    maxval = 1;
    myfile3 << line << endl;
    myfile3 << width << " " << height << endl;
    myfile3 << maxval << endl;
    for (int i = 0; i < height; i++){
        for (int j = 0; j < width; j++){
            if (image3[i][j] == 1){
                myfile3 << 1 << " " << 1 << " " << 1 << " ";
            }
            else{
                myfile3 << 0 << " " << 0 << " " << 0 << " ";
            }
        }
        myfile3 << endl;
    }
    myfile3.close();
    myfile2.close();
    
    cout << "Finished Final Image" << endl;
    midpointdetect(gx, gy, image3, width, height, filename, threshold, pArgs);

}



int main(int argc, char *argv[]){
    canny(argc, argv);
    return 0;
}