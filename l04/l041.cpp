//Satvik Matta
//December 2, 2022
#include <iostream>
#include <fstream>
#include <math.h>
#include <iomanip> // for setprecision
#include <string>
#include <list>
#include <vector>
using namespace std;

const int render = 400;
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
        return sqrt(pow((p.getX() - x), 2) + pow((p.getY() - y), 2));
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
    bool isRightOf(Point p1, Point p2){
        double x1 = p1.getX();
        double y1 = p1.getY();
        double x2 = p2.getX();
        double y2 = p2.getY();
        double x3 = x;
        double y3 = y;
        double val = (y2-y1)*(x3-x2)-(x2-x1)*(y3-y2);
        if(val == 0){
            return false;
        }
        else if(val > 0){
            return true;
        }
        else{
            return false;
        }
    }
    //distance between a point and a line
    double distance(Point p1, Point p2)
    {
        double x1 = p1.getX();
        double y1 = p1.getY();
        double x2 = p2.getX();
        double y2 = p2.getY();
        double x3 = x;
        double y3 = y;
        double val = abs((y3 - y1) * (x2 - x1) - (y2 - y1) * (x3 - x1));
        double dist = val / sqrt(pow((x2 - x1), 2) + pow((y2 - y1), 2));
        return dist;
    }
    bool operator==(const Point& p) const
    {
        return x == p.x && y == p.y;
    }
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

double random_double()
{
    return (double)rand() / (double)RAND_MAX;
}


void part0(){
    srand(time(NULL));
    // cout << "Do you want to generate points (y/n)?" << endl;
    char c;
    // cin >> c;
    c = 'y';
    if (c == 'y')
    {
        
        string name = "points";
        ofstream myfile;
        myfile.open(name + string(".txt"));
        myfile << setprecision(23) << fixed;
        for (int i = 0; i < 59; i++)
        {
            double x = random_double();
            double y = random_double();
            myfile << x << " " << y << endl;
        }
        double x = random_double();
        double y = random_double();
        myfile << x << " " << y << endl;
        myfile.close();
    }
    else{
        return;
    }
}
double testSide(Point P, Point Q, Point R){
    return (Q.getX() - P.getX()) * (R.getY() - P.getY()) - (R.getX() - P.getX()) * (Q.getY() - P.getY());
}

//FindHull
void findHull(vector<Point> &Sk, Point P, Point Q, vector<Point> &Hull){
    if (Sk.size() == 0){
        return;
    }
    double max = Sk[0].distance(P, Q);
    int index = 0;
    for (int i = 1; i < (int) Sk.size(); i++){
        double dist = Sk[i].distance(P, Q);
        if (dist > max){
            max = dist;
            index = i;
        }
    }

    // Hull.push_back(Sk[index]);

    Point R = Sk[index];
    
    //add R to hull between P and Q
    for (int i = 0; i < (int) Hull.size(); i++){
        if (Hull[i].getX() == P.getX() && Hull[i].getY() == P.getY()){
            Hull.insert(Hull.begin() + i + 1, R);
            break;
        }
    }
    
    // vector<Point> S0; //Points inside triangle PRQ
    vector<Point> S1; // Points on the right side of PR
    vector<Point> S2; // Points on the right side of RQ
    for (int i = 0; i < (int) Sk.size(); i++){
        if(i != index){
            double side = testSide(P, R, Sk[i]);
            if (side > 0){
                S1.push_back(Sk[i]);
            }
            else{
                side = testSide(R, Q, Sk[i]);
                if (side > 0){
                    S2.push_back(Sk[i]);
                }
            }
        }
    }
    findHull(S1, P, R, Hull);
    findHull(S2, R, Q, Hull);

    return;
}



//QuickHull
vector<Point> QuickHull(vector<Point> &points){
    vector<Point> convexHull;
    if (points.size() < 3){
        return points;
    }
    Point minPoint = points[0];
    Point maxPoint = points[0];
    for (int i = 0; i < (int) points.size(); i++){
        if (points[i].getX() < minPoint.getX()){
            minPoint = points[i];
        }
        if (points[i].getX() > maxPoint.getX()){
            maxPoint = points[i];
        }
        if (points[i].getX() == minPoint.getX()){
            if (points[i].getY() < minPoint.getY()){
                minPoint = points[i];
            }
        }
        if (points[i].getX() == maxPoint.getX()){
            if (points[i].getY() > maxPoint.getY()){
                maxPoint = points[i];
            }
        }
    }
    convexHull.push_back(minPoint);
    convexHull.push_back(maxPoint);
    convexHull.push_back(minPoint);
    vector<Point> S1;
    vector<Point> S2;
    vector<Point> S0;
    for (int i = 0; i < (int) points.size(); i++){
        double side = testSide(minPoint, maxPoint, points[i]);
        if (side < 0){
            S1.push_back(points[i]);
        }
        else if (side > 0){
            S2.push_back(points[i]);
        }
        else{
            S0.push_back(points[i]);
        }
    }
    if(S1.empty() || S2.empty()){
        for (int i = 0; i < (int) S0.size(); i++){
            convexHull.push_back(S0[i]);
        }
    }

    findHull(points, minPoint, maxPoint, convexHull);
    // cout << convexHull.size() << endl;
    findHull(points, maxPoint, minPoint, convexHull);
    // cout << convexHull.size() << endl;
    return convexHull;

}

void part1(){
    part0();
    //read in points from points.txt
    cout << setprecision(23) << fixed;
    //create a list of points from the file
    list<Point> pointss;
    ifstream myfile("points.txt");
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
    // cout << pointss.size() << endl;

    vector<Point> points;
    copy(pointss.begin(), pointss.end(), back_inserter(points));

    vector<Point> convexHull = QuickHull(points);
    
    
    Render render;
    //draw the points
    for (int i = 0; i < (int) points.size(); i++){
        render.drawcircle(points[i].getXCon(), points[i].getYCon(), 2);
        render.drawcircle(points[i].getXCon(), points[i].getYCon(), 3);
    }
    //Remove duplicates from convex hull
    for (int i = 0; i < (int) convexHull.size(); i++){
        for (int j = i + 1; j < (int) convexHull.size(); j++){
            if (convexHull[i].getX() == convexHull[j].getX() && convexHull[i].getY() == convexHull[j].getY()){
                convexHull.erase(convexHull.begin() + j);
                j--;
            }
        }
    }
    convexHull.push_back(convexHull[0]);
    //draw the convex hull points
    for (int i = 0; i < (int) convexHull.size(); i++){
        render.coldrawcircle(convexHull[i].getXCon(), convexHull[i].getYCon(), 2,2);
        render.coldrawcircle(convexHull[i].getXCon(), convexHull[i].getYCon(), 3,2);
    }
    //draw the lines
    for (int i = 0; i < (int) convexHull.size() - 1; i++){
        render.drawline(convexHull[i].getXCon(), convexHull[i + 1].getXCon(), convexHull[i].getYCon(), convexHull[i + 1].getYCon());
    }
    render.createppm("quickhull");

}
int main(){
    part1();
    return 0;
}