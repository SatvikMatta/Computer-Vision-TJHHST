#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include<cstdlib>
#include<ctime>
using namespace std;



class IPoint {
    public:
        int x, y;
};


int n = 200;
int m = n;

vector<vector<int> > illuminate(int i,int j, vector<vector<int> > v){
    v[i][j] = 0;
    return v;
}

//Bresenham's line algorithm 1
vector<vector<int> > bres1(int x1, int y1, int x2, int y2, vector<vector<int> > v){
    if(x1 > x2){
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

    for(int i = x1; i < x2; i++){
        v = illuminate(i,j,v);
        if(error >= 0){
            j++;
            error = error - dx;
        }
        error = error + dy;
    }
    return v;
}


vector<vector<int> > line(int x1, int y1, int x2, int y2, vector<vector<int> > v){

    if (x1 == x2) {
        if (y1 > y2) {
            int temp = y1;
            y1 = y2;
            y2 = temp;
        }

        for (int i = y1; i <= y2; i++) {
            v = illuminate(x1, i, v);
        }
        return v;
    }

    else if (y1 == y2)
    {
        if (x1 > x2) {
            int temp = x1;
            x1 = x2;
            x2 = temp;
        }

        for (int i =x1; i <= x2; i++) {
            v = illuminate(i, y1, v);
        }
        return v;
    }
    
    else{
        cout << "Not a straight line" << endl;
    }
    return v;

}

//Bresenham's line algorithm 2 DA is x-axis
vector<vector<int> > bres2(int x1, int y1, int x2, int y2, vector<vector<int> > v){
    if (x1 > x2) {
        int temp = x1;
        x1 = x2;
        x2 = temp;
        temp = y1;
        y1 = y2;
        y2 = temp;
    }
    int negy1 = m-1 - y1;
    int negy2 = m-1 - y2;

    int dx = x2 - x1;
    int dy = negy2 - negy1;
    int j = negy1;
    int error = dy - dx;
    for (int i = x1; i < x2; i++) {
        v = illuminate(i, m-1-j, v);
        if (error >= 0) {
            j++;
            error = error - dx;
        }
        error = error + dy;
    }
    return v;

}

//Bresenham's line algorithm 3 DA is y-axis
vector<vector<int> > bres3(int x1, int y1, int x2, int y2, vector<vector<int> > v){
    if(y1 > y2){
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

    for(int i = y1; i < y2; i++){
        v = illuminate(j,i,v);
        if(error >= 0){
            j++;
            error = error - dy;
        }
        error = error + dx;
    }
    return v;
}

//Bresenham's line algorithm 4 DA is y-axis
vector<vector<int> > bres4(int x1, int y1, int x2, int y2, vector<vector<int> > v){
    if (y1 > y2) {
        int temp = x1;
        x1 = x2;
        x2 = temp;
        temp = y1;
        y1 = y2;
        y2 = temp;
    }
    int negx1 = m-1 - x1;
    int negx2 = m-1 - x2;

    int dx = negx2 - negx1;
    int dy = y2 - y1;
    int j = x1;
    int error = dx - dy;
    for (int i = y1; i < y2; i++) {
        v = illuminate(m-1-j,i, v);
        if (error >= 0) {
            j++;
            error = error - dy;
        }
        error = error + dx;
    }
    return v;

}

double random_double(){
    return (double)rand() / (double)RAND_MAX;
}

vector<vector<int> > draw_line(IPoint p1, IPoint p2, vector<vector<int> > v, double dx, double dy){
    if ( dx == 0 || dy == 0) {
        v = line(p1.x, p1.y, p2.x, p2.y, v);
    }
    if (abs(dx) > abs(dy)) {
        double slope = (double)dy / dx;
        if (slope>0) {
            v = bres1(p1.x, p1.y, p2.x, p2.y, v);
        }
        else {
            v = bres2(p1.x, p1.y, p2.x, p2.y, v);
        }
    }
    else {
        double slope = (double)dy / dx;
        if (slope>0) {
            v = bres3(p1.x, p1.y, p2.x, p2.y, v);
        }
        else {
            v = bres4(p1.x, p1.y, p2.x, p2.y, v);
        }
    }
    return v;
}

int main(){    
    srand(time(NULL));
    vector<vector<int> > v(n, std::vector<int>(m, 1));
    IPoint p1;
    IPoint p2;
    IPoint p3;
    
    // p1.x = random_double()*n; 
    // p1.y = random_double()*m;
    // p2.x = random_double()*n;
    // p2.y = random_double()*m;
    // p3.x = random_double()*n;
    // p3.y = random_double()*m;

    p1.x = 136;
    p1.y = 71;
    p2.x = 86;
    p2.y = 52;
    p3.x = 6;
    p3.y = 150;
    
    
    double dx = p2.x - p1.x;
    double dy = p2.y - p1.y;
    v = draw_line(p1, p2, v, dx, dy);
    
    double dx2 = p3.x - p2.x;
    double dy2 = p3.y - p2.y;
    v = draw_line(p2, p3, v, dx2, dy2);
    // double dx3 = p1.x - p3.x;
    // double dy3 = p1.y - p3.y;
    // v = draw_line(p1, p3, v, dx3, dy3);



    for (int i = 0; i < m; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            std::cout << v[i][j] << ' ';
        }
        std::cout << std::endl;
    }

    cout<<"p1.x = "<<p1.x<<endl;
    cout<<"p1.y = "<<p1.y<<endl;
    cout<<"p2.x = "<<p2.x<<endl;
    cout<<"p2.y = "<<p2.y<<endl;
    // cout<<"p3.x = "<<p3.x<<endl;
    // cout<<"p3.y = "<<p3.y<<endl;

    //create ppm file from array
    ofstream myfile;
    myfile.open ("l011.ppm");
    myfile << "P3\n" << n << " " << m << "\n1\n";
    for (int i = 0; i < m; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            myfile << v[i][j] << " ";
            myfile << v[i][j] << " ";
            myfile << v[i][j] << " ";
        }
        myfile << std::endl;
    }
    myfile.close();
    return 0;
    
}