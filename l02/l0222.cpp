// Satvik Matta
// Date: 10/6/2022
#include <iostream>
#include <fstream>
#include <math.h>
#include <iomanip>
#include <string>
using namespace std;

const int render = 800;

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

double random_double()
{
    return (double)rand() / (double)RAND_MAX;
}

double area(double x1, double y1, double x2, double y2, double x3, double y3)
{
    return abs((x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2)) / 2.0);
}

bool intriangle(double x1, double y1, double x2, double y2, double x3, double y3, double x, double y)
{
    double A = area(x1, y1, x2, y2, x3, y3);
    double A1 = area(x, y, x2, y2, x3, y3);
    double A2 = area(x1, y1, x, y, x3, y3);
    double A3 = area(x1, y1, x2, y2, x, y);
    return (abs((A - (A1 + A2 + A3))) < (render / 100000.0));
}

bool check(double x, double y, ofstream &out, double x1, double x2, double x3, double y1, double y2, double y3)
{
    return intriangle(x1, y1, x2, y2, x3, y3, x, y) || intriangle(x, y, x2, y2, x3, y3, x1, y1) || intriangle(x1, y1, x, y, x3, y3, x2, y2) || intriangle(x1, y1, x2, y2, x, y, x3, y3);
}

void part1()
{
    srand(time(NULL));
    double x1 = random_double();
    double y1 = random_double();
    double x2 = random_double();
    double y2 = random_double();
    double x3 = random_double();
    double y3 = random_double();
    // Generate 4th Random Point
    double x = random_double();
    double y = random_double();
    ofstream myfile;

    myfile << fixed << showpoint;
    myfile << setprecision(17);
    cout << fixed << showpoint;
    cout << setprecision(17);

    cout << "(" << x1 << ',' << y1 << ") , (" << x2 << ',' << y2 << ") , "
         << "(" << x3 << ',' << y3 << ")" << endl;

    myfile.open("log.txt");
    myfile << "(" << x1 << ',' << y1 << ") , (" << x2 << ',' << y2 << ") , "
           << "(" << x3 << ',' << y3 << ")" << endl;
    // if(check(x,y,myfile,x1,x2,x3,y1,y2,y3) == false){
    //     cout << "Testing the Point: (" << x << "," << y << ")" << endl;
    //     myfile << "Testing the Point: (" << x << "," << y << ")" << endl;
    // }
    while (check(x, y, myfile, x1, x2, x3, y1, y2, y3))
    {
        cout << "Testing the Point: (" << x << "," << y << ")" << endl;
        myfile << "Testing the Point: (" << x << "," << y << ")" << endl;
        x = ((double)rand() / RAND_MAX);
        y = ((double)rand() / RAND_MAX);
    }
    cout << "Successful Point: (" << x << "," << y << ")" << endl;
    myfile << "Successful Point: (" << x << "," << y << ")" << endl;
    myfile.close();

    // Make Points.txt
    myfile.open("points.txt");
    myfile << "(" << x1 << ',' << y1 << ") , (" << x2 << ',' << y2 << ") , "
           << "(" << x3 << ',' << y3 << ") , (" << x << ',' << y << ")";
    myfile.close();
}

double distance(double x1, double y1, double x2, double y2)
{
    return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}

double intercept(Line l1, Line l2){
    double m1 = l1.getslope();
    double m2 = l2.getslope();
    double b1 = l1.getintercept();
    double b2 = l2.getintercept();
    double x = (b2-b1)/(m1-m2);
    return x;

}

double findsquare(Point p1, Point p2, Point p3, Point p4, double posorneg, Render render, string name)
{
    Line l1 = Line(p1, p2);
    render.drawline(l1.getYintercept().getXCon(), l1.getYmax().getXCon(), l1.getYintercept().getYCon(), l1.getYmax().getYCon());
    Line pl1 = Line(p3, (1/l1.getslope())*-1, false);
    render.drawline(pl1.getYintercept().getXCon(), pl1.getYmax().getXCon(), pl1.getYintercept().getYCon(), pl1.getYmax().getYCon());

    double dist = distance(p1.getX(), p1.getY(), p2.getX(), p2.getY());
    double x = p3.getX() + (dist/sqrt(1+pow(pl1.getslope(),2))) * posorneg;
    cout << x << endl;
    Point e = Point(x, pl1.getY(x));
    // render.drawcircle(e.getXCon(), e.getYCon(), 3);
    cout << "E: " << e.getXCon() << "," << e.getYCon() << endl;
    Line l2 = Line(p4, e); //s
    render.drawline(l2.getYintercept().getXCon(), l2.getYmax().getXCon(), l2.getYintercept().getYCon(), l2.getYmax().getYCon());

    Line pl2 = Line(p1, (1/l2.getslope())*-1, false);//s1
    render.drawline(pl2.getYintercept().getXCon(), pl2.getYmax().getXCon(), pl2.getYintercept().getYCon(), pl2.getYmax().getYCon());

    Line pl22 = Line(p2, (1/l2.getslope())*-1, false);//s4
    render.drawline(pl22.getYintercept().getXCon(), pl22.getYmax().getXCon(), pl22.getYintercept().getYCon(), pl22.getYmax().getYCon());

    Line pl3 = Line(p3, (1/pl2.getslope())*-1, false);//s
    render.drawline(pl3.getYintercept().getXCon(), pl3.getYmax().getXCon(), pl3.getYintercept().getYCon(), pl3.getYmax().getYCon());

    Point v1 = Point(intercept(pl2, pl3), pl2.getY(intercept(pl2, pl3)));
    Point v2 = Point(intercept(pl22, pl3), pl22.getY(intercept(pl22, pl3)));
    Point v3 = Point(intercept(pl22, l2), pl22.getY(intercept(pl22, l2)));
    Point v4 = Point(intercept(pl2, l2), pl2.getY(intercept(pl2, l2)));

    render.drawcircle(v1.getXCon(), v1.getYCon(), 3);
    render.drawcircle(v2.getXCon(), v2.getYCon(), 3);
    render.drawcircle(v3.getXCon(), v3.getYCon(), 3);
    render.drawcircle(v4.getXCon(), v4.getYCon(), 3);

    double dist1 = distance(v1.getX(), v1.getY(), v2.getX(), v2.getY());
    double dist2 = distance(v2.getX(), v2.getY(), v3.getX(), v3.getY());
    double dist3 = distance(v3.getX(), v3.getY(), v4.getX(), v4.getY());
    double dist4 = distance(v4.getX(), v4.getY(), v1.getX(), v1.getY());
    double dist5 = distance(v1.getX(), v1.getY(), v3.getX(), v3.getY());
    double dist6 = distance(v2.getX(), v2.getY(), v4.getX(), v4.getY());
    double min = dist1;
    if(dist2 < min){
        min = dist2;
    }
    if(dist3 < min){
        min = dist3;
    }
    if(dist4 < min){
        min = dist4;
    }
    if(dist5 < min){
        min = dist5;
    }
    if(dist6 < min){
        min = dist6;
    }
    // cout << "Min: " << min << endl;

    render.createppm(name);
    return 0.0;
}

double findSquareNoRender(Point p1, Point p2, Point p3, Point p4, double posorneg, ofstream &out){
    Line l1 = Line(p1, p2);
    Line pl1 = Line(p3, (1/l1.getslope())*-1, false);
    double dist = distance(p1.getX(), p1.getY(), p2.getX(), p2.getY());
    double x = p3.getX() + (dist/sqrt(1+pow(pl1.getslope(),2))) * posorneg;
    // cout << x << endl;
    Point e = Point(x, pl1.getY(x));
    // cout << "E: " << e.getXCon() << "," << e.getYCon() << endl;
    Line l2 = Line(p4, e);
    Line pl2 = Line(p1, (1/l2.getslope())*-1, false);
    Line pl22 = Line(p2, (1/l2.getslope())*-1, false);
    Line pl3 = Line(p3, (1/pl2.getslope())*-1, false);

    Point v1 = Point(intercept(pl2, pl3), pl2.getY(intercept(pl2, pl3)));
    Point v2 = Point(intercept(pl22, pl3), pl22.getY(intercept(pl22, pl3)));
    Point v3 = Point(intercept(pl22, l2), pl22.getY(intercept(pl22, l2)));
    Point v4 = Point(intercept(pl2, l2), pl2.getY(intercept(pl22, l2)));

    double dist1 = distance(v1.getX(), v1.getY(), v2.getX(), v2.getY());
    double dist2 = distance(v2.getX(), v2.getY(), v3.getX(), v3.getY());
    double dist3 = distance(v3.getX(), v3.getY(), v4.getX(), v4.getY());
    double dist4 = distance(v4.getX(), v4.getY(), v1.getX(), v1.getY());
    double dist5 = distance(v1.getX(), v1.getY(), v3.getX(), v3.getY());
    double dist6 = distance(v2.getX(), v2.getY(), v4.getX(), v4.getY());

    double min = dist1;
    if(dist2 < min){
        min = dist2;
    }
    if(dist3 < min){
        min = dist3;
    }
    if(dist4 < min){
        min = dist4;
    }
    if(dist5 < min){
        min = dist5;
    }
    if(dist6 < min){
        min = dist6;
    }
    // cout << "Min: " << min << endl;
    double area = pow(min,2);
    out << "(" << v1.getX() << "," << v1.getY() << ") , (" << v2.getX() << "," << v2.getY() << ") , (" << v3.getX() << "," << v3.getY() << ") , (" << v4.getX() << "," << v4.getY() << ") Area = " << area << endl;
    return area;
}

void part2(){
    // Read from points.txt (0,0) , (0,1) , (1,0) , (0.5,0.5)
    ifstream myfile;
    // myfile >> fixed >> showpoint;
    // myfile >> setprecision(17);
    cout << fixed << showpoint;
    cout << setprecision(17);
    myfile.open("points.txt");
    double x1, y1, x2, y2, x3, y3, x, y;
    char c;
    myfile >> c >> x1 >> c >> y1 >> c >> c >> c >> x2 >> c >> y2 >> c >> c >> c >> x3 >> c >> y3 >> c >> c >> c >> x >> c >> y;
    myfile.close();
    // cout << "(" << x1 << ',' << y1 << ") , (" << x2 << ',' << y2 << ") , " << "(" << x3 << ',' << y3 << ") , (" << x << ',' << y << ")" << endl;
    Point p1 = Point(x1, y1);
    Point p2 = Point(x2, y2);
    Point p3 = Point(x3, y3);
    Point p4 = Point(x, y);
    p1.tostring();
    p2.tostring();
    p3.tostring();
    p4.tostring();
    //Render
    Render render = Render();
    render.drawcircle(p1.getXCon(), p1.getYCon(), 3);
    render.drawcircle(p2.getXCon(), p2.getYCon(), 3);
    render.drawcircle(p3.getXCon(), p3.getYCon(), 3);
    render.drawcircle(p4.getXCon(), p4.getYCon(), 3);
    ofstream myfile2;
    myfile2 << fixed << showpoint;
    myfile2 << setprecision(17);
    myfile2.open("output.txt");
    myfile2 << "(" << x1 << ',' << y1 << ") , (" << x2 << ',' << y2 << ") , " << "(" << x3 << ',' << y3 << ") , (" << x << ',' << y << ")" << endl;

    int l = 1;
    double minarea  = findSquareNoRender(p1, p2, p3, p4, 1, myfile2);
    double area = findSquareNoRender(p1, p2, p3, p4, -1, myfile2);

    double area2 = findSquareNoRender(p1, p3, p2, p4, 1, myfile2);
    double area3 = findSquareNoRender(p1, p3, p2, p4, -1, myfile2);

    double area4 = findSquareNoRender(p1, p4, p2, p3, 1, myfile2);
    double area5 = findSquareNoRender(p1, p4, p2, p3, -1, myfile2);

    if(area < minarea){
        minarea = area;
        l=2;
    }
    if(area2 < minarea){
        minarea = area2;
        l=3;
    }
    if(area3 < minarea){
        minarea = area3;
        l=4;
    }
    if(area4 < minarea){
        minarea = area4;
        l=5;
    }
    if(area5 < minarea){
        minarea = area5;
        l=6;
    }

    // cout << "Min Area: " << minarea << endl;

    if(l==1){
        cout << "Line 1" << endl;
        findsquare(p1, p2, p3, p4, 1, render, "output");
    }
    else if(l==2){
        cout << "Line 2" << endl;
        findsquare(p1, p2, p3, p4, -1, render, "output");
    }
    else if(l==3){
        cout << "Line 3" << endl;
        findsquare(p1, p3, p2, p4, 1, render, "output");
    }
    else if(l==4){
        cout << "Line 4" << endl;
        findsquare(p1, p3, p2, p4, -1, render, "output");
    }
    else if(l==5){
        cout << "Line 5" << endl;
        findsquare(p1, p4, p2, p3, 1, render, "output");
    }
    else if(l==6){
        cout << "Line 6" << endl;
        findsquare(p1, p4, p2, p3, -1, render, "output");
    }


    // double a1 = findsquare(p1, p2, p3, p4, 1, render, "part2a");
    // double a2 = findsquare(p1, p2, p3, p4, -1, render, "part2b");

    // double a3 = findsquare(p1, p3, p2, p4, 1, render, "part2c");
    // double a4 = findsquare(p1, p3, p2, p4, -1, render, "part2d");

    // double a5 = findsquare(p1, p4, p2, p3, 1, render, "part2e");
    // double a6 = findsquare(p1, p4, p2, p3, -1, render, "part2f");
    
    
    

}

int main()
{
    // part1();
    part2();
    return 0;
}