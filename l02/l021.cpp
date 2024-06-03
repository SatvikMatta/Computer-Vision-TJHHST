//Satvik Matta
//Date: 9/17/2022
#include <iostream>
#include <fstream>
#include<math.h>
#include <iomanip>
using namespace std;

const int render = 800;

double random_double(){
    return (double)rand() / (double)RAND_MAX;
}

double area(double x1, double y1, double x2, double y2, double x3, double y3) {
   return abs((x1*(y2-y3) + x2*(y3-y1)+ x3*(y1-y2))/2.0);
}

bool intriangle(double x1,double y1, double x2, double y2, double x3, double y3, double x, double y) {
   double A = area(x1, y1, x2, y2, x3, y3);
   double A1 = area(x, y, x2, y2, x3, y3);
   double A2 = area(x1, y1, x, y, x3, y3);
   double A3 = area(x1, y1, x2, y2, x, y);
   return (abs((A- (A1 + A2 + A3))) < (render/100000.0));
}

bool check(double x,double y,ofstream &out, double x1, double x2, double x3, double y1, double y2, double y3){
    return intriangle(x1, y1, x2, y2, x3, y3, x, y) || intriangle(x, y, x2, y2, x3, y3, x1, y1) || intriangle(x1, y1, x, y, x3, y3, x2, y2) || intriangle(x1, y1, x2, y2, x, y, x3, y3);
}

void part1(){
    srand(time(NULL));
    double x1 = random_double();
    double y1 = random_double();
    double x2 = random_double();
    double y2 = random_double();
    double x3 = random_double();
    double y3 = random_double();
    //Generate 4th Random Point 
    double x = random_double();
    double y = random_double();
    ofstream myfile;

    myfile << fixed << showpoint;
    myfile << setprecision(17);
    cout << fixed << showpoint;
    cout << setprecision(17);

    cout << "(" << x1 << ',' << y1 << ") , (" << x2 << ',' << y2 << ") , " << "(" << x3 << ',' << y3 << ")" << endl;

    myfile.open("log.txt");
    myfile << "(" << x1 << ',' << y1 << ") , (" << x2 << ',' << y2 << ") , " << "(" << x3 << ',' << y3 << ")" << endl;
    // if(check(x,y,myfile,x1,x2,x3,y1,y2,y3) == false){
    //     cout << "Testing the Point: (" << x << "," << y << ")" << endl;
    //     myfile << "Testing the Point: (" << x << "," << y << ")" << endl;
    // }
    while (check(x,y,myfile,x1,x2,x3,y1,y2,y3)) {
        cout << "Testing the Point: (" << x << "," << y << ")" << endl;
        myfile << "Testing the Point: (" << x << "," << y << ")" << endl;
        x = ((double) rand()/RAND_MAX);
        y = ((double) rand()/RAND_MAX);
    }
    cout << "Successful Point: (" << x << "," << y << ")" << endl;
    myfile << "Successful Point: (" << x << "," << y << ")" << endl;
    myfile.close();

    //Make Points.txt
    myfile.open("points.txt");
    myfile << "(" << x1 << ',' << y1 << ") , (" << x2 << ',' << y2 << ") , " << "(" << x3 << ',' << y3 << ") , (" << x << ',' << y << ")";
    myfile.close();
}

int main(){
    part1();
    return 0;
}