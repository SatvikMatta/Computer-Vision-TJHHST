//Satvik Matta
//November 10, 2022
#include <iostream>
#include <fstream>
#include <math.h>
#include <iomanip> // for setprecision
#include <string>
#include <list>
#include <vector>
#include <algorithm>
#include <climits>

using namespace std;

const int render = 800;
//open file to write
ofstream fout("results.txt");
string inputFile = "points.txt";

// Create a Point Class
class Point
{
private:
    double x;
    double y;

public:
    Point(double x, double y)
    {
        this->x = x;
        this->y = y;
    }
    // default constructor
    Point()
    {
        this->x = 0.0;
        this->y = 0.0;
    }
    double getX()
    {
        return x;
    }
    double distance(Point p)
    {
        return sqrt(((p.getX() - x) * (p.getX() - x)) + ((p.getY() - y) * (p.getY() - y)));
    }
    double getY()
    {
        return y;
    }
    double getXCon(){
        return round(x*render);
    }
    double getYCon(){
        return round(y*render);
    }
    void setX(double x)
    {
        this->x = x;
    }
    void setY(double y)
    {
        this->y = y;
    }
    // tostring to print the point
    void tostring()
    {
        cout << "(" << x << "," << y << ")" << endl;
    }
    void rtostring()
    {
        fout << "(" << x << "," << y << ")" << endl;
    }
    // bool Point::operator<(const Point &p) const
    // {
    //     if (x < p.x)
    //     {
    //         return true;
    //     }
    //     else if (x == p.x)
    //     {
    //         if (y < p.y)
    //         {
    //             return true;
    //         }
    //     }
    //     return false;
    // }
};

// Create a Line Class
class Line
{
private:
    Point p1;
    Point p2;
    double m;
    double b;
    bool undefined;
public:
    Line(Point p1, Point p2){
        this->p1 = p1;
        this->p2 = p2;
        if(p1.getX() == p2.getX()){
            undefined = true;
        }
        else{
            undefined = false;
            m = (p2.getY() - p1.getY())/(p2.getX() - p1.getX());
            b = p1.getY() - m*p1.getX();
        }
    }
    Line(Point p, double m, double undefined){
        this->p1 = p;
        this->m = m;
        this->undefined = undefined;
        if(undefined){
            p2.setX(p.getX());
            p2.setY(p.getY() + 1);
        }
        else{
            p2.setX(p.getX() + 1);
            p2.setY(m*p2.getX() + b);
            b = p.getY() - m*p.getX();
        }
    }
    double getslope(){
        return m;
    }
    double getintercept(){
        return b;
    }
    Point getYintercept(){
        return Point(0,b);
    }
    Point getYmax(){
        return Point(1, m + b);
    }
    bool isUndefined(){
        return undefined;
    }
    Point getP1(){
        return p1;
    }
    Point getP2(){
        return p2;
    }
    double getY(double x){
        return m*x + b;
    }
};


// Create a Renderer Class
class Render
{
private:
    int array[render][render];

public:
    Render()
    {
        for (int i = 0; i < render; i++)
        {
            for (int j = 0; j < render; j++)
            {
                array[i][j] = 1;
            }
        }
    }
    void illuminate(int x, int y)
    {
        if (x > 0 && y > 0 && x < render && y < render){
            array[x][y] = 0;
        }
        // else{
        //     cout << "Skipped: Out of bounds" << endl;
        // }
    }
    void colilluminate(int x, int y, int col){
       if (x > 0 && y > 0 && x < render && y < render){
            array[x][y] = col;
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
    void coldrawcircle(int xc, int yc, double r, int col)
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
            colilluminate(xc + x, yc + y, col);
            colilluminate(xc + x, yc - y, col);
            colilluminate(xc - x, yc + y, col);
            colilluminate(xc - x, yc - y, col);
            colilluminate(xc + y, yc + x, col);
            colilluminate(xc + y, yc - x, col);
            colilluminate(xc - y, yc + x, col);
            colilluminate(xc - y, yc - x, col);
            y2_new -= (2 * x) - 3;
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
        int negy1 = render - 1 - y1;
        int negy2 = render - 1 - y2;

        int dx = x2 - x1;
        int dy = negy2 - negy1;
        int j = negy1;
        int error = dy - dx;
        for (int i = x1; i < x2; i++)
        {
            illuminate(i, render - 1 - j);
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
    int negx1 = render-1 - x1;
    int negx2 = render-1 - x2;

    int dy = y2 - y1;
    int dx = negx2 - negx1;
    int j = negx1;
    int error = dx - dy;
    for (int i = y1; i < y2; i++) {
        illuminate(render-1-j, i);
        if (error >= 0) {
            j++;
            error = error - dy;
        }
        error = error + dx;
    }
    
}

    void createppm(string name)
    {
        ofstream myfile;
        myfile.open(name + string(".ppm"));
        myfile << "P3\n"
               << render << " " << render << "\n1\n";
        for (int i = 0; i < render; ++i)
        {
            for (int j = 0; j < render; ++j)
            {
                if (array[i][j] == 0)
                {
                    myfile << "0 0 0 ";
                }
                else if (array[i][j] == 2){
                    myfile <<"1 0 0 ";
                }
                else
                {
                    myfile << "1 1 1 ";
                }
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

class Result
{
    private:
        Point p1;
        Point p2;
        double distance;
    public:
        Result()
        {
            p1 = Point(0, 0);
            p2 = Point(0, 0);
            distance = 10;
        }
        Result(Point p1, Point p2, double distance)
        {
            this->p1 = p1;
            this->p2 = p2;
            this->distance = distance;
        }
        Point getP1()
        {
            return p1;
        }
        Point getP2()
        {
            return p2;
        }
        double getDistance()
        {
            return distance;
        }
};

double random_double()
{
    return (double)rand() / (double)RAND_MAX;
}


void part0(){
    srand(time(NULL));
    cout << "Do you want to generate points (y/n)?" << endl;
    char c;
    cin >> c;
    if (c == 'y')
    {
        cout << "How many points?" << endl;
        int n;
        cin >> n;
        string name = "points";
        ofstream myfile;
        myfile.open(name + string(".txt"));
        myfile << setprecision(23) << fixed;
        for (int i = 0; i < n-1; i++)
        {
            double x = random_double();
            double y = random_double();
            myfile << x << " " << y << endl;
        }
        double x = random_double();
        double y = random_double();
        myfile << x << " " << y;
        myfile.close();
    }
    else{
        return;
    }
}

void part1(){
    cout << setprecision(23) << fixed;
    fout << setprecision(23) << fixed;
    //create a list of points from the file
    list<Point> points;
    ifstream myfile("points.txt");
    string line;
    fout << "Number of Points: ";
    while (getline(myfile, line))
    {
        double x, y;
        int i = line.find(" ");
        x = stod(line.substr(0, i));
        y = stod(line.substr(i + 1, line.length()));
        Point p(x, y);
        points.push_back(p);
    }
    myfile.close();
    cout << points.size() << endl;
    fout << points.size() << endl;
    fout << endl;
    //find closest pair of points in list
    //make iterator for list
    Point *p1 = new Point();
    Point *p2 = new Point();
    int count = 0;
    double min = 2.0;
    clock_t start,end;
    start = clock();
    for (list<Point>::iterator i = points.begin(); i != points.end(); i++)
    {
        
        for (list<Point>::iterator j = next(i,1); j != points.end(); j++)
        {
            double dist = i->distance(*j);
            if (dist < min)
            {
                min = dist;
                *p1 = *i;
                *p2 = *j;
            }
            count++;
        }
    }
    end = clock();
    // cout << count << endl;
    fout << "Brute Force:" << endl;
    cout << "The closest pair of points is: " << endl;
    p1->tostring();
    p2->tostring();
    cout << "The distance between them is: " << min << endl;
    cout << "The time taken is: " << (double)(end-start)/CLOCKS_PER_SEC << endl;
    fout << "The closest pair of points is: " << endl;
    p1->rtostring();
    p2->rtostring();
    fout << "The distance between them is: " << min << endl;
    fout << "The time taken is: " << (double)(end-start)/CLOCKS_PER_SEC << endl;
    fout << endl;


    //draw all the points in the list
    Render r = Render();
    for (list<Point>::iterator i = points.begin(); i != points.end(); i++)
    {
        r.drawcircle(i->getXCon(), i->getYCon(), 3);
        r.drawcircle(i->getXCon(), i->getYCon(), 2);

    }
    r.coldrawcircle(p1->getXCon(), p1->getYCon(), 3, 2);
    r.coldrawcircle(p1->getXCon(), p1->getYCon(), 2, 2);
    r.coldrawcircle(p2->getXCon(), p2->getYCon(), 3, 2);
    r.coldrawcircle(p2->getXCon(), p2->getYCon(), 2, 2);
    r.createppm("points");

}

bool compareX(Point p1, Point p2)
{
    return p1.getX() < p2.getX();
}

Result findclosest(vector<Point> &points, int b, int e){
    if (e-b == 1){
        double distance = points[b].distance(points[e]);
        return Result(points[b], points[e], distance);
    }
    if (e-b == 2){
        double distance1 = points[b].distance(points[b+1]);
        double distance2 = points[b].distance(points[e]);
        double distance3 = points[b+1].distance(points[e]);
        if (distance1 < distance2 && distance1 < distance3){
            return Result(points[b], points[b+1], distance1);
        }
        else if (distance2 < distance1 && distance2 < distance3){
            return Result(points[b], points[e], distance2);
        }
        else{
            return Result(points[b+1], points[e], distance3);
        }
    }
    int mid = (int)((b+e)/2);
    Result left = findclosest(points, b, mid);
    Result right = findclosest(points, mid+1, e);
    Result result;
    if (left.getDistance() < right.getDistance()){
        result = left;
    }
    else{
        result = right;
    }
    double d = result.getDistance();
    int strips = 0;
    int stripe = points.size();
    for (int i = mid; i >= b; i--){
        if (points[mid].getX() - points[i].getX() < d){
            strips=i;
        }
        else{
            break;
        }
    }
    for (int i = mid; i <= e; i++){
        if (points[i].getX() - points[mid].getX() < d){
            stripe=i;
        }
        else{
            break;
        }
    }

    for (int i = strips; i <= mid; i++){
        for (int j = mid; j < stripe+1; j++){
            double distance = points[i].distance(points[j]);
            if (distance < d && i!=j){
                result = Result(points[i], points[j], distance);
            }
        }
    }
    return result;
}

void part2(){
    cout << setprecision(23) << fixed;
    fout << setprecision(23) << fixed;
    //create a list of points from the file
    list<Point> pointss;
    ifstream myfile(inputFile);
    string line;
    while (getline(myfile, line))
    {
        double x, y;
        int i = line.find(" ");
        x = stod(line.substr(0, i));
        y = stod(line.substr(i + 1, line.length()));
        Point p(x, y);
        pointss.push_back(p);
    }
    myfile.close();
    cout << pointss.size() << endl;
    //sort the list by x coordinate
    // pointss.sort(compareX);
    //find closest pair of points in list   //print out the list
    // for (list<Point>::iterator i = points.begin(); i != points.end(); i++)
    // {
    //     i->tostring();
    // }
    std::vector<Point> points;
    std::copy(pointss.begin(), pointss.end(), back_inserter(points));
    clock_t start,end;
    start = clock();
    sort(points.begin(), points.end(), compareX);
    
    Result r = findclosest(points, 0, points.size()-1);
    end = clock();
    double time = (double)(end-start)/CLOCKS_PER_SEC;
    cout << "The closest pair of points is: " << endl;
    r.getP1().tostring();
    r.getP2().tostring();
    cout << "The distance between them is: " << r.getDistance() << endl;
    cout << "Time: " << time << endl;
    fout << "Recursive: " << endl;
    fout << "The closest pair of points is: " << endl;
    r.getP1().rtostring();
    r.getP2().rtostring();
    fout << "The distance between them is: " << r.getDistance() << endl;
    fout << "Time: " << time << endl;
    fout << endl;
}
bool compareY(Point p1, Point p2)
{
    return p1.getY() < p2.getY();
}

Result rfindclosest(vector<Point> &points, int b, int e){
    if (e-b == 1){
        double distance = points[b].distance(points[e]);
        return Result(points[b], points[e], distance);
    }
    if (e-b == 2){
        double distance1 = points[b].distance(points[b+1]);
        double distance2 = points[b].distance(points[e]);
        double distance3 = points[b+1].distance(points[e]);
        if (distance1 < distance2 && distance1 < distance3){
            return Result(points[b], points[b+1], distance1);
        }
        else if (distance2 < distance1 && distance2 < distance3){
            return Result(points[b], points[e], distance2);
        }
        else{
            return Result(points[b+1], points[e], distance3);
        }
    }
    int mid = (int)((b+e)/2);
    Result left = rfindclosest(points, b, mid);
    Result right = rfindclosest(points, mid+1, e);
    Result result;
    if (left.getDistance() < right.getDistance()){
        result = left;
    }
    else{
        result = right;
    }
    double d = result.getDistance();
    vector<Point> strip;
    for (int i = b; i <= e; i++){
        if (abs(points[i].getX() - points[mid].getX()) < d){
            strip.push_back(points[i]);
        }
    }
    unsigned long int uid = strip.size();
    int auid = uid & INT_MAX;
    sort(strip.begin(), strip.end(), compareY);
    for (int i = 0; i < auid; i++){
        for (int j = i+1; j < auid && j < i+15; j++){
            double distance = strip[i].distance(strip[j]);
            if (distance < d && i!=j){
                result = Result(strip[i], strip[j], distance);
            }
        }
    }


    return result;
}

void part3(){
    cout << setprecision(23) << fixed;
    //create a list of points from the file
    list<Point> pointss;
    ifstream myfile(inputFile);
    string line;
    while (getline(myfile, line))
    {
        double x, y;
        int i = line.find(" ");
        x = stod(line.substr(0, i));
        y = stod(line.substr(i + 1, line.length()));
        Point p(x, y);
        pointss.push_back(p);
    }
    myfile.close();
    cout << pointss.size() << endl;
    //sort the list by x coordinate
    // pointss.sort(compareX);
    //find closest pair of points in list   //print out the list
    // for (list<Point>::iterator i = points.begin(); i != points.end(); i++)
    // {
    //     i->tostring();
    // }
    std::vector<Point> points;
    std::copy(pointss.begin(), pointss.end(), back_inserter(points));
    clock_t start,end;
    start = clock();
    sort(points.begin(), points.end(), compareX);
    
    Result r = rfindclosest(points, 0, points.size()-1);
    end = clock();
    double time = (double)(end-start)/CLOCKS_PER_SEC;
    cout << "The closest pair of points is: " << endl;
    r.getP1().tostring();
    r.getP2().tostring();
    cout << "The distance between them is: " << r.getDistance() << endl;
    cout << "Time: " << time << endl;
    fout << "Fully Recursive: " << endl;
    fout << "The closest pair of points is: " << endl;
    r.getP1().rtostring();
    r.getP2().rtostring();
    fout << "The distance between them is: " << r.getDistance() << endl;
    fout << "Time: " << time << endl;
    fout.close();
}

int main(){
    part0();
    // part1();
    part2();
    part3();
    return 0;
}