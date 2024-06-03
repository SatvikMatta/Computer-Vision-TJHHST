//Satvik Matta
//Febuary 10, 2022
#include <cstdlib>
#include <fstream>
#include <cmath>
#include <iterator>
#include <vector>
#include <algorithm>
#include <chrono>
#include <limits>
#include <iomanip>
#include <sstream>
#include <stack>
#include <random>
#include <iostream>
#include <cstring>
#include <string>


using namespace std;

class Point{
    private:
        double x;
        double y;
    public:
        Point(){
            x = 0;
            y = 0;
        }
        Point(double x, double y){
            this->x = x;
            this->y = y;
        }
        double getX(){
            return x;
        }
        double getY(){
            return y;
        }
        void setX(double x){
            this->x = x;
        }
        void setY(double y){
            this->y = y;
        }
        double distance(Point p){
            return sqrt(pow(x - p.getX(), 2) + pow(y - p.getY(), 2));
        }
        double distance_NoSqrt(Point p){
            return pow(x - p.getX(), 2) + pow(y - p.getY(), 2);
        }
        string toString(){
            stringstream ss;
            ss << "(" << x << ", " << y << ")";
            return ss.str();
        }
        bool coinE(Point p){
            int x_diff = abs(x - p.getX());
            int y_diff = abs(y - p.getY());
            return x_diff <= 85 && y_diff <= 85;
        }
};

class Line{
    private:
        Point p1;
        Point p2;
        double x;
        double y;
        double z;
    public:
        Line(Point p1, Point p2){
            this->p1 = p1;
            this->p2 = p2;
            x = -1 * (p2.getY() - p1.getY());
            y = p2.getX() - p1.getX();
            z = x * p1.getX() + y * p1.getY();
        }
        double getXfY(double f){
            if(y==0){
                return p1.getX();
            }
            else if(x==0){
                return -numeric_limits < double > ::infinity();
            }
            else{
                return (z - y * f) / x;
            }
        }
        double getYfX(double f){
            if(x==0){
                return -numeric_limits < double > ::infinity();
            }
            else if(y==0){
                return p1.getY();
            }
            else{
                return (z - x * f) / y;
            }
        }
        Point *getInter(Line l){
            double temp = x * l.getY() - l.getX() * y;
            if(temp == 0){
                return NULL;
            }
            else{
                double xt = (z * l.getY() - y * l.getZ()) / temp;
                double yt = (x * l.getZ() - z * l.getX()) / temp;
                return new Point(xt, yt);
            }
        }
        double getX(){
            return x;
        }
        double getY(){
            return y;
        }
        double getZ(){
            return z;
        }
        Point getP1(){
            return p1;
        }
        Point getP2(){
            return p2;
        }

};

class PPM{
    private:
        vector<vector<vector<int>>> pixels;
        vector<vector<int>> inc;
        vector<vector<vector<int>>> originial;
        vector<vector<int>> gxarray;
        vector<vector<int>> gyarray;
        vector<vector<int>> middle;
        int width;
        int height;
        int max;
    public:
        PPM(int width, int height, int max){
            this->width = width;
            this->height = height;
            this->max = max;
            pixels.resize(height);
            inc.resize(height);
            originial.resize(height);
            for(int i = 0; i < height; i++){
                pixels[i].resize(width);
                inc[i].resize(width);
                originial[i].resize(width);
                for(int j = 0; j < width; j++){
                    pixels[i][j].resize(3);
                    inc[i][j] = 0;
                    originial[i][j].resize(3);
                }
            }

        }
        PPM(string file){
            ifstream fin(file);
            string temp;
            fin >> temp;
            fin >> width;
            fin >> height;
            fin >> max;
            pixels.resize(height);
            inc.resize(height);
            originial.resize(height);
            for(int i = 0; i < height; i++){
                pixels[i].resize(width);
                inc[i].resize(width);
                originial[i].resize(width);
                for(int j = 0; j < width; j++){
                    pixels[i][j].resize(3);
                    inc[i][j] = 0;
                    originial[i][j].resize(3);
                }
            }
            for(int i = 0; i < height; i++){
                for(int j = 0; j < width; j++){
                    for(int k = 0; k < 3; k++){
                        fin >> pixels[i][j][k];
                        originial[i][j][k] = pixels[i][j][k];
                    }
                }
            }
        }
        void setPixel(int x, int y, int r, int g, int b){
            pixels[y][x][0] = r;
            pixels[y][x][1] = g;
            pixels[y][x][2] = b;
        }
        void illuminate(int x, int y){
            if (x >= 0 && y >= 0 && x < (int) inc.size() && y < (int) inc[0].size()){
                inc[x][y] += 1;
            }
        }
        void illuminate2(int x, int y){
            if (x >= 0 && y >= 0 && x < (int) inc.size() && y < (int) inc[0].size()){
                middle[x][y] += 1;
            }
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
        int negy1 = (int)inc[0].size() - 1 - y1;
        int negy2 = (int)inc[0].size() - 1 - y2;

        int dx = x2 - x1;
        int dy = negy2 - negy1;
        int j = negy1;
        int error = dy - dx;
        for (int i = x1; i < x2; i++)
        {
            illuminate(i, (int)inc[0].size() - 1 - j);
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
    int negx1 = (int)inc.size()-1 - x1;
    int negx2 = (int)inc.size()-1 - x2;

    int dy = y2 - y1;
    int dx = negx2 - negx1;
    int j = negx1;
    int error = dx - dy;
    for (int i = y1; i < y2; i++) {
        illuminate((int)inc.size()-1-j, i);
        if (error >= 0) {
            j++;
            error = error - dy;
        }
        error = error + dx;
    }
    
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
        void createPPM(string file){
            ofstream fout(file);
            fout << "P3" << endl;
            fout << width << " " << height << endl;
            fout << max << endl;
            for(int i = 0; i < height; i++){
                for(int j = 0; j < width; j++){
                    for(int k = 0; k < 3; k++){
                        fout << pixels[i][j][k] << " ";
                    }
                }
                fout << endl;
            }
            fout.close();
        }
        void circleDraw(int xc, int yc, int radius, int r, int g, int b){
            int xmax = (int)(radius * 0.70710678);
            int y = radius;
            int y2 = radius * radius;
            int ty = (2*y)-1;
            int y2n = y2;
            for(int x=0; x<=xmax+2; x++){
                if ((y2-y2n) >=ty){
                    y2 -= ty;
                    y--;
                    ty -= 2;

                }
                setPixel(xc+x, yc+y, r, g, b);
                setPixel(xc+x, yc-y, r, g, b);
                setPixel(xc-x, yc+y, r, g, b);
                setPixel(xc-x, yc-y, r, g, b);
                setPixel(xc+y, yc+x, r, g, b);
                setPixel(xc+y, yc-x, r, g, b);
                setPixel(xc-y, yc+x, r, g, b);
                setPixel(xc-y, yc-x, r, g, b);
                y2n -= (x*2)-3;
            }
        }
        void blackwhite(){
            for(int i = 0; i < height; i++){
                for(int j = 0; j < width; j++){
                    int avg = (pixels[i][j][0] + pixels[i][j][1] + pixels[i][j][2]) / 3;
                    pixels[i][j][0] = avg;
                    pixels[i][j][1] = avg;
                    pixels[i][j][2] = avg;
                }
            }
            cout << "Black and White" << endl;
        }
        void hystersis(int r, int c,vector<vector<int>> &array){
            if(r < 0 || r >= height || c < 0 || c >= width){
                return;
            }
            if(array[r][c] == 0){
                return;
            }
            if(array[r][c] == 3 || array[r][c] == 2){
                array[r][c] = 1;
                hystersis(r+1, c, array);
                hystersis(r-1, c, array);
                hystersis(r, c+1, array);
                hystersis(r, c-1, array);
                hystersis(r+1, c+1, array);
                hystersis(r+1, c-1, array);
                hystersis(r-1, c+1, array);
                hystersis(r-1, c-1, array);
            }

        }
        vector<vector<double>> canny(int thres1, int thres2){
            blackwhite();
            createPPM("imagebw.ppm");
            cout << "Canny Start" << endl;
            createPPM("imageg.ppm");
            vector<vector<vector<int>>> temp;
            vector < vector < int >> angles(height, vector < int > (width, 0));
            vector < vector < double >> angles2(height, vector < double > (width, 0.0));
            vector<vector<int>> mag(height, vector <int> (width, 0));
            copy(pixels.begin(), pixels.end(), back_inserter(temp));
            vector<Point> points;
            vector<vector<int>> imagetemp(height, vector<int>(width, 0));
            vector<vector<int>> gxarray(height, vector<int>(width, 0));
            vector<vector<int>> gyarray(height, vector<int>(width, 0));
            for(int i =0; i < height; i++){
                for(int j = 0; j < width; j++){
                    imagetemp[i][j] = pixels[i][j][0];
                }
            }
            double tempA = 0.0;
            for(int i = 0; i < height; i++){
                vector<int> gxrow;
                vector<int> gyrow;
                for(int j = 0; j < width; j++){
                    if(i == 0 || j == 0 || i == height - 1 || j == width - 1){
                        // cout << "i: " << i << " j: " << j << " if state" << endl;
                        imagetemp[i][j] = 0;
                    }
                    else{
                        // cout << "i: " << i << " j: " << j << " else state" << endl;
                        
                        double gx = (temp[i - 1][j - 1][0] * -1 + temp[i][j - 1][0] * -2 + temp[i + 1][j - 1][0] * -1 + temp[i - 1][j + 1][0] + temp[i][j + 1][0] * 2 + temp[i +1][j + 1][0]);
                        gxrow.push_back(gx);
                        double gy = (temp[i - 1][j - 1][0] * -1 + temp[i - 1][j][0] * -2 + temp[i - 1][j + 1][0] * -1 + temp[i + 1][j - 1][0] + temp[i + 1][j][0] * 2 + temp[i +1][j + 1][0]);
                        gyrow.push_back(gy);
                        int magtemp = (int)sqrt((gx * gx) + (gy * gy));
                        // cout << "magtemp: " << magtemp << endl;
                        // cout << "thereshold 1: " << thres1 << endl;
                        // cout << "thereshold 2: " << thres2 << endl;
                        if (magtemp >= thres2){
                            // cout << "Place 1"   << endl;
                            // cout << "i: " << i << " j: " << j << endl;
                            // cout dimensions of imagetemp
                            // cout << "imagetemp size: " << imagetemp.size() << endl;
                            // cout << "imagetemp[0] size: " << imagetemp[0].size() << endl;
                            imagetemp[i][j] = 3;
                            
                            points.push_back(Point(i, j));
                        }
                        else if(magtemp >= thres1){
                            // cout << "Place 2"   << endl;
                            imagetemp[i][j] = 2;
                            
                        }
                        else{
                            // cout << "Place 3"   << endl;
                            imagetemp[i][j] = 0;
                            
                        }
                        // cout <<"Place 4" << endl;
                        // print out mag size
                        // cout << "magtemp size: " << mag.size() << endl;
                        // cout << "magtemp[0] size: " << mag[0].size() << endl;
                        mag[i][j] = magtemp;
                        // cout << "Place 5" << endl;
                        angles2[i][j] = atan2(gy, gx);
                        // cout << "Place 6" << endl;
                        tempA = angles2[i][j] * 180 / M_PI;
                        // cout << "Place 7" << endl;
                        if (tempA < 0) {
                            tempA = tempA + 180;
                        }
                        if (tempA >= 0 && tempA <= 22.5) {
                            // cout << "Place 8" << endl;
                            angles[i][j] = 0;
                        } 
                        else if (tempA > 22.5 && tempA <= 67.5) {
                            // cout << "Place 9" << endl;
                            angles[i][j] = 45;
                        } 
                        else if (tempA > 67.5 && tempA <= 112.5) {
                            // cout << "Place 10" << endl;
                            angles[i][j] = 90;
                        } 
                        else if (tempA > 112.5 && tempA <= 157.5) {
                            // cout << "Place 11" << endl;
                            angles[i][j] = 135;
                        } 
                        else if (tempA > 157.5 && tempA <= 180) {
                            // cout << "Place 12" << endl;
                            angles[i][j] = 0;
                        }
                    }
               }
                gxarray.push_back(gxrow);
                gyarray.push_back(gyrow);
            }
            cout << "finished part a of canny" << endl;
            for (Point p: points){
                hystersis(p.getX(), p.getY(), imagetemp);
            }
            cout << "finished hystereiss of canny" << endl;
            for(int i = 0; i < height; i++){
                for(int j = 0; j < width; j++){
                    if(imagetemp[i][j] == 1){
                        pixels[i][j][0] = 255;
                        pixels[i][j][1] = 255;
                        pixels[i][j][2] = 255;
                    }
                    else{
                        pixels[i][j][0] = 0;
                        pixels[i][j][1] = 0;
                        pixels[i][j][2] = 0;
                    }
                }
            }
            createPPM("image1.ppm");
            for (int i=0; i<height; i++){
                for (int j=0; j<width; j++){
                    if(i!= 0 && j!=0 && i!=height-1 && j!=width-1){
                        int value = mag[i][j];
                        tempA = angles[i][j];
                        if (tempA == 0) {
                            if (value >= mag[i][j + 1] && value >= mag[i][j - 1]) {
                                value = 1;
                            } else {
                                value = 0;
                            }
                        } else if (tempA == 135) {
                            if (value >= mag[i - 1][j + 1] && value >= mag[i + 1][j - 1]) {
                                value = 1;
                            } else {
                                value = 0;
                            }
                        } else if (tempA == 90) {
                            if (value >= mag[i + 1][j] && value >= mag[i - 1][j]) {
                                value = 1;
                            } else {
                                value = 0;
                            }
                        } else {
                            if (value >= mag[i + 1][j + 1] && value >= mag[i - 1][j - 1]) {
                                value = 1;
                            } else {
                                value = 0;
                            }
                        }
                        pixels[i][j][0] = value;
                        pixels[i][j][1] = value;
                        pixels[i][j][2] = value;
                        }
                        else {
                        pixels[i][j][0] = 1;
                        pixels[i][j][1] = 1;
                        pixels[i][j][2] = 1;
                        }
                    }
                }
                int tempu = max;
                max = 1;
                createPPM("image2.ppm");
                max=  tempu;
                for (int i = 0; i<height; i++){
                    for (int j = 0; j<width; j++){
                        if (pixels[i][j][0] == 1 && imagetemp[i][j] == 1){
                            pixels[i][j][0] = 255;
                            pixels[i][j][1] = 255;
                            pixels[i][j][2] = 255;
                        }
                        else {
                            pixels[i][j][0] = 0;
                            pixels[i][j][1] = 0;
                            pixels[i][j][2] = 0;
                        }
                    }
                }
                createPPM("imagef.ppm");
                cout << "Canny Edge Detection Complete" << endl;
                return angles2;
    }
    int getmax(){
        int maxvalue = 0;
        for (int i = 0; i < (int) inc.size(); i++){
            for (int j = 0; j < (int) inc[0].size(); j++){
                if (inc[i][j] > maxvalue){
                maxvalue = inc[i][j];
                }   
            }
        }
        return maxvalue;
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
            illuminate2(xc + x, yc + y);
            illuminate2(xc + x, yc - y);
            illuminate2(xc - x, yc + y);
            illuminate2(xc - x, yc - y);
            illuminate2(xc + y, yc + x);
            illuminate2(xc + y, yc - x);
            illuminate2(xc - y, yc + x);
            illuminate2(xc - y, yc - x);
            y2_new -= (2 * x) - 3;
        }
    }
    void screenLine(Point p1, Point p2, int s, int r, int g, int b, bool inc){
        double slope = 0;
        double b1 = 0;
        slope = (p1.getY() - p2.getY()) / (p1.getX() - p2.getX());
        if(isfinite(slope)){
            b1 = p1.getY() - (slope * p1.getX());
            // cout << "Place 21" << endl;
            drawline(Point((0-b1)/slope, 0).getX(), Point((height-b)/slope, height).getX(), Point((0-b1)/slope, 0).getY(), Point((height-b)/slope, height).getY());
        }
        else{
            // cout << "Place 21" << endl;
            drawline(Point(p2.getX(), 0).getX(), Point(p1.getX(), height).getX(), Point(p2.getX(), 0).getY(), Point(p1.getX(), height).getY());
        }
    }

    vector<Point> findCenters(int thres1, int thres2, int votes, int n){
        int maxcoinsize = 125;
        vector<Point> centers;
        bool pArgs = false;
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
        for(int i = 0; i < height; i++){
            for(int j = 0; j < width; j++){
                if(pixels[i][j][0] == 255){
                    double slope;
                    if (gxarray[i][j] == 0){
                        slope = 20000;
                    }
                    else{
                        slope = (double)gyarray[i][j]/(double)gxarray[i][j];
                    }
                    int minx = j - maxcoinsize;
                    int maxx = j + maxcoinsize;
                    int miny = i - (int)(maxcoinsize * slope);
                    int maxy = i + (int)(maxcoinsize * slope);
                    drawline(miny, maxy, minx, maxx);
                }
            }
        }
        int maxvalue = getmax();
        cout << "Max Value: " << maxvalue << endl;
        int temp = max;
        max  = maxvalue;
        createPPM("imagev.ppm");
        max = temp;
        cout << "Finished Voting Map" << endl;
        for (int i = 0; i < height; i++){
            vector<int> row;
            for (int j = 0; j < width; j++){
                row.push_back(0);
            }
            middle.push_back(row);
        }
        int count = 0;
        voting = inc;
        for(int i = 0; i < height; i++){
            for(int j = 0; j < width; j++){
                int thres = votes;
                if (i > 0.4*height && i < 0.9*height && j < 0.4*width && pArgs == false)
                {
                    thres = 1.8*votes;
                }
                if (i > 0.4*height && i < 0.9*height && j > 0.4*width && pArgs == false)
                {
                    thres = 1.5*votes;
                }
                if(voting[i][j] > thres){
                    centers.push_back(Point(j, i));
                    drawcircle(i, j, 2);
                    drawcircle(i, j, 3);
                    count++;
                }
            }
        }
        cout << "Number of coins: " << count << endl;
        // //Load initial r,g,b image from image.ppm
        // vector<vector<int>> r;
        // vector<vector<int>> g;
        // vector<vector<int>> b;
        // for(int i = 0; i < height; i++){
        //     vector<int> row;
        //     for(int j = 0; j < width; j++){
        //         row.push_back(0);
        //     }
        //     r.push_back(row);
        //     g.push_back(row);
        //     b.push_back(row);
        // }
        // ifstream infile(inputfile);
        // string line;
        // int width1, height1, maxval1;
        // infile >> line;
        // infile >> width1 >> height1 >> maxval1;

        // for(int i = 0; i < height; i++){
        //     for(int j = 0; j < width; j++){
        //         infile >> r[i][j];
        //         infile >> g[i][j];
        //         infile >> b[i][j];
        //     }
        // }
        // infile.close();
        for(int i = 0; i < height; i++){
            for(int j = 0; j < width; j++){
                if(middle[i][j] >= 1){
                    originial[i][j][0] = 255;
                    originial[i][j][1] = 0;
                    originial[i][j][2] = 0;
                }
            }
        }
        ofstream outfile("imageCC.ppm");
        outfile << "P3" << endl;
        outfile << width << " " << height << endl;
        outfile << 255 << endl;
        for(int i = 0; i < height; i++){
            for(int j = 0; j < width; j++){
                outfile << originial[i][j][0] << " " << originial[i][j][1] << " " << originial[i][j][2] << " ";
            }
            outfile << endl;
        }
        
        outfile.close();

        cout << "Finished Midpoint Detection and Overlay" << endl;
        return centers;
    }
    double pCircle(int xc, int yc, int r){
        double total = 0;
        double capturedP = 0;
        int xmax = (int)(r * 0.70710678);
        int y = r;
        int y2 = r * r;
        int ty = (2*y)-1;
        int y2n = y2;
        int tempx = 0;
        int tempy = 0;
        for (int x = 0; x <= xmax + 2; x++) {
            if ((y2 - y2n) >= ty) {
                y2 -= ty;
                y -= 1;
                ty -= 2;
            }
            tempx = xc + x;
            tempy = yc + y;
            if (tempx < width && tempx >= 0 && tempy < height && tempy >= 0 && pixels[tempy][tempx][0] == 1) {
                capturedP++;
            }
            tempy = yc - y;
            if (tempx < width && tempx >= 0 && tempy < height && tempy >= 0 &&  pixels[tempy][tempx][0] == 1) {
                capturedP++;
            }
            tempx = xc - x;
            tempy = yc + y;
            if (tempx < width && tempx >= 0 && tempy < height && tempy >= 0 &&  pixels[tempy][tempx][0] == 1) {
                capturedP++;
            }
            tempy = yc - y;
            if (tempx < width && tempx >= 0 && tempy < height && tempy >= 0 &&  pixels[tempy][tempx][0] == 1) {
                capturedP++;
            }
            tempx = xc + y;
            tempy = yc + x;
            if (tempx < width && tempx >= 0 && tempy < height && tempy >= 0 &&  pixels[tempy][tempx][0] == 1) {
                capturedP++;
            }
            tempy = yc - x;
            if (tempx < width && tempx >= 0 && tempy < height && tempy >= 0 &&  pixels[tempy][tempx][0] == 1) {
                capturedP++;
            }
            tempx = xc - y;
            tempy = yc + x;
            if (tempx < width && tempx >= 0 && tempy < height && tempy >= 0 &&  pixels[tempy][tempx][0] == 1) {
                capturedP++;
            }
            tempy = yc - x;
            if (tempx < width && tempx >= 0 && tempy < height && tempy >= 0 &&  pixels[tempy][tempx][0] == 1) {
                capturedP++;
            }
            total += 8;
            y2n -= (2 * x) - 3;
        }
        return capturedP / total;
    }
    void add(vector<Point> &rc, vector <Point> &com, Point p, vector <vector <double>> &percentage, int n, int s){
        for(long unsigned int i=0; i< rc.size(); i++){
            if(p.coinE(rc[i]))
            {
                if(percentage[n][s] > percentage[com[i].getX()][com[i].getY()]){
                    com[i] = Point(n, s);
                    rc[i] = p;
                }
                return;
            }
        }
        rc.push_back(p);
        com.push_back(Point(n, s));
    }
    void findCoins(int thres1, int thres2, int votes, int n, double childP, int smalPeps){
        int coinsize = 180- smalPeps;
        int start = smalPeps;
        vector<Point> centers = findCenters(thres1, thres2, votes, n);
        vector<vector<double>> percentage(centers.size(), vector<double>(coinsize,0));
        double max = 0;
        vector<Point> rc;
        vector<Point> com;
        double a = 0;
        int gayR = 0;
        int gayG = 0;
        int gayB = 0;
        int coinP = 0;
        int coinN = 0;
        int coinD = 0;
        int coinQ = 0;
        int coinS = 0;
        vector<int> coinTypes;
        for(long unsigned int i=0; i<centers.size(); i++){
            for(int j=start; j<coinsize; j++){
               percentage[i][j-start] = pCircle(centers[i].getX(), centers[i].getY(), j);
            }
        }
        for(long unsigned int i=0; i<centers.size(); i++){
            for(int j=0; j<coinsize; j++){
                if(percentage[i][j] > max){
                    max = percentage[i][j];
                }
            }
        }
        for(int i=0; i<height; i++){
            for(int j=0; j<width; j++){
                pixels[i][j][0] = originial[i][j][0];
                pixels[i][j][1] = originial[i][j][1];
                pixels[i][j][2] = originial[i][j][2];
            }
        }
        for(long unsigned int i=0; i<centers.size(); i++){
            for(int j=0; j<coinsize; j++){
                if(percentage[i][j] > max * childP){
                    add(rc, com, centers[i], percentage, i, j);
                }
            }
        }
        for (long unsigned int i = 0; i < rc.size(); i++) {
            int midRadius = (com[i].getY() + start) / 2;
            gayR = pixels[rc[i].getY()][rc[i].getX()][0] + pixels[rc[i].getY() + midRadius][rc[i].getX()][0] +pixels[rc[i].getY()][rc[i].getX() + midRadius][0] + pixels[rc[i].getY() - midRadius][rc[i].getX()][0] +pixels[rc[i].getY()][rc[i].getX() - midRadius][0];
            gayG = pixels[rc[i].getY()][rc[i].getX()][1] + pixels[rc[i].getY() + midRadius][rc[i].getX()][1] +pixels[rc[i].getY()][rc[i].getX() + midRadius][1] + pixels[rc[i].getY() - midRadius][rc[i].getX()][1] +pixels[rc[i].getY()][rc[i].getX() - midRadius][1];
            gayB = pixels[rc[i].getY()][rc[i].getX()][2] + pixels[rc[i].getY() + midRadius][rc[i].getX()][2] +pixels[rc[i].getY()][rc[i].getX() + midRadius][2] + pixels[rc[i].getY() - midRadius][rc[i].getX()][2] +pixels[rc[i].getY()][rc[i].getX() - midRadius][2];
            gayR = gayR / 5;
            gayG = gayG / 5;
            gayB = gayB / 5;
            if (com[i].getY() + start >= 150) {
                if (gayR > 1.3 * gayB) {
                    rc.erase(rc.begin() +i);
                    com.erase(com.begin() +i);
                    i--;
                    a+=0;
                } else {
                a += 1;
                coinTypes.push_back(5);
                coinS += 1;
            }
            } else if (com[i].getY() + start >= 103) {
                if (gayR > 1.3 * gayB) {
                    rc.erase(rc.begin() +i);
                    com.erase(com.begin() +i);
                    i--;
                    a+=0;
                } else {
                    a += .25;
                    coinTypes.push_back(1);
                    coinQ += 1;
                }
            }
            else if (com[i].getY() + start > 80 && com[i].getY() + start < 103) {
                if (gayR > 1.3 * gayB) {
                    a += .01;
                    coinTypes.push_back(3);
                    coinP += 1;
                } else {
                    a += .05;
                    coinTypes.push_back(2);
                    coinN += 1;
                }
            } else {
                if (gayR > 1.3 * gayB) {
                    a += .01;
                    coinTypes.push_back(3);
                    coinP += 1;
                } else {
                    coinD += 1;
                    a += .1;
                    coinTypes.push_back(4);
                }
            }
        }
        for (long unsigned int i = 0; i < rc.size(); i++) {
            if (coinTypes[i] == 1) {
                circleDraw(rc[i].getX(), rc[i].getY(), com[i].getY() + start, 0,255,0);
                circleDraw(rc[i].getX(), rc[i].getY(), com[i].getY() + start + 1, 0,255,0);
                circleDraw(rc[i].getX(), rc[i].getY(), com[i].getY() + start + 2, 0,255,0);
                circleDraw(rc[i].getX(), rc[i].getY(), 4, 0,255,0);
                circleDraw(rc[i].getX(), rc[i].getY(), 3, 0,255,0);
                circleDraw(rc[i].getX(), rc[i].getY(), 2, 0,255,0);
                circleDraw(rc[i].getX(), rc[i].getY(), 1, 0,255,0);
                setPixel(rc[i].getX(), rc[i].getY(), 0,255,0);
            } else if (coinTypes[i] == 2) {
                circleDraw(rc[i].getX(), rc[i].getY(), com[i].getY() + start, 128,0,128);
                circleDraw(rc[i].getX(), rc[i].getY(), com[i].getY() + start + 1, 128,0,128);
                circleDraw(rc[i].getX(), rc[i].getY(), com[i].getY() + start + 2, 128,0,128);
                circleDraw(rc[i].getX(), rc[i].getY(), 4, 128,0,128);
                circleDraw(rc[i].getX(), rc[i].getY(), 3, 128,0,128);
                circleDraw(rc[i].getX(), rc[i].getY(), 2, 128,0,128);
                circleDraw(rc[i].getX(), rc[i].getY(), 1, 128,0,128);
                setPixel(rc[i].getX(), rc[i].getY(), 128,0,128);
            } else if (coinTypes[i] == 3) {
                circleDraw(rc[i].getX(), rc[i].getY(), com[i].getY() + start, 255,0,0);
                circleDraw(rc[i].getX(), rc[i].getY(), com[i].getY() + start + 1, 255,0,0);
                circleDraw(rc[i].getX(), rc[i].getY(), com[i].getY() + start + 2, 255,0,0);
                circleDraw(rc[i].getX(), rc[i].getY(), 4, 255,0,0);
                circleDraw(rc[i].getX(), rc[i].getY(), 3, 255,0,0);
                circleDraw(rc[i].getX(), rc[i].getY(), 2, 255,0,0);
                circleDraw(rc[i].getX(), rc[i].getY(), 1, 255,0,0);
                setPixel(rc[i].getX(), rc[i].getY(), 255,0,0);
            } else if (coinTypes[i] == 4) {
                circleDraw(rc[i].getX(), rc[i].getY(), com[i].getY() + start, 0,0,255);
                circleDraw(rc[i].getX(), rc[i].getY(), com[i].getY() + start + 1, 0,0,255);
                circleDraw(rc[i].getX(), rc[i].getY(), com[i].getY() + start + 2, 0,0,255);
                circleDraw(rc[i].getX(), rc[i].getY(), 4, 0,0,255);
                circleDraw(rc[i].getX(), rc[i].getY(), 3, 0,0,255);
                circleDraw(rc[i].getX(), rc[i].getY(), 2, 0,0,255);
                circleDraw(rc[i].getX(), rc[i].getY(), 1, 0,0,255);
                setPixel(rc[i].getX(), rc[i].getY(), 0,0,255);
            } else {
                circleDraw(rc[i].getX(), rc[i].getY(), com[i].getY() + start, 255,255,0);
                circleDraw(rc[i].getX(), rc[i].getY(), com[i].getY() + start + 1, 255,255,0);
                circleDraw(rc[i].getX(), rc[i].getY(), com[i].getY() + start + 2, 255,255,0);
                circleDraw(rc[i].getX(), rc[i].getY(), 4, 255,255,0);
                circleDraw(rc[i].getX(), rc[i].getY(), 3, 255,255,0);
                circleDraw(rc[i].getX(), rc[i].getY(), 2, 255,255,0);
                circleDraw(rc[i].getX(), rc[i].getY(), 1, 255,255,0);
                setPixel(rc[i].getX(), rc[i].getY(), 255,255,0);
            }
        }
        cout << coinS << " Silver Dollars" << endl;
        cout << coinQ << " Quarters" << endl;
        cout << coinD << " Dimes" << endl;
        cout << coinN << " Nickels" << endl;
        cout << coinP << " Pennies" << endl;
        cout << "Total: $" << a << endl;
        cout << "Total Coins: " << coinS + coinQ + coinD + coinN + coinP << endl;
        ofstream myfile;
        myfile.open("results.txt");
        myfile << coinS << " Silver Dollars" << endl;
        myfile << coinQ << " Quarters" << endl;
        myfile << coinD << " Dimes" << endl;
        myfile << coinN << " Nickels" << endl;
        myfile << coinP << " Pennies" << endl;
        myfile << "Total: $" << a << endl;
        myfile << "Total Coins: " << coinS + coinQ + coinD + coinN + coinP << endl;
        myfile.close();
        createPPM("coins.ppm");
    }
};

void part2(){
    cout << "Part 2" << endl;
    PPM *image = new PPM("image.ppm");
    image->findCenters(80, 150,65, 169);
    delete image;
}

int main(){
    cout << "Part Main" << endl;
    part2();
    return 0;
}
