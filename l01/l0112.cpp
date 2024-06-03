#include <iostream>
#include <string>
#include <fstream>
#include<cstdlib>
#include<ctime>
#include<cmath>
#include<math.h>
using namespace std;

const int m = 800;
const int n = 800;

double random_double(){
    return (double)rand() / (double)RAND_MAX;
}

class IPoint {
    private:
        int x, y;
    public:
        IPoint(int x, int y) {
            this->x = x;
            this->y = y;
        }
        IPoint() {
            double d = random_double()*n;
            double d2 = random_double()*m;
            this->x = round(d);
            this->y = round(d2);
        }
        int get_x() {
            return x;
        }
        int get_y() {
            return y;
        }
};

 void illuminate(int i,int j, int array[n][m] ){
    array[i][j] = 0;
}


void bres1(int x1, int y1, int x2, int y2, int array[m][n]){
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
        illuminate(i,j,array);
        if(error >= 0){
            j++;
            error = error - dx;
        }
        error = error + dy;
    }
}

void bres2(int x1, int y1, int x2, int y2, int array[m][n]){
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
        illuminate(i, m-1-j, array);
        if (error >= 0) {
            j++;
            error = error - dx;
        }
        error = error + dy;
    }
    
}

void bres3(int x1, int y1, int x2, int y2, int array[m][n]){
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
        illuminate(j,i,array);
        if(error >= 0){
            j++;
            error = error - dy;
        }
        error = error + dx;
    }
}

void bres4(int x1, int y1, int x2, int y2, int array[m][n]){
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

    int dy = y2 - y1;
    int dx = negx2 - negx1;
    int j = negx1;
    int error = dx - dy;
    for (int i = y1; i < y2; i++) {
        illuminate(m-1-j, i, array);
        if (error >= 0) {
            j++;
            error = error - dy;
        }
        error = error + dx;
    }
    
}

void createppm(int v[m][n]){
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
}

void drawstraight(int x1, int x2, int y1, int y2, int array[m][n]){
    if(x1 == x2){
        if(y1 > y2){
            int temp = y1;
            y1 = y2;
            y2 = temp;
        }
        for(int i = y1; i <= y2; i++){
            illuminate(x1,i,array);
        }
    }
    else if(y1 == y2){
        if(x1 > x2){
            int temp = x1;
            x1 = x2;
            x2 = temp;
        }
        for(int i = x1; i <= x2; i++){
            illuminate(i,y1,array);
        }
    }
    
}

void  drawline(int x1, int x2, int y1, int y2, int array[m][n]){
    if(x1 == x2 || y1 == y2){
        drawstraight(x1,x2,y1,y2,array);
        return;
    }
    int dx = x2 - x1;
    int dy = y2 - y1;
    if(abs(dx) >= abs(dy)){
        if ((double)dx/dy > 0){
            bres1(x1,y1,x2,y2,array);
        }
        else{
            bres2(x1,y1,x2,y2,array);
        
        }
    }
    else if (abs(dx) < abs(dy)){
        if ((double)dx/dy > 0){
            bres3(x1,y1,x2,y2,array);
        }
        else{
            bres4(x1,y1,x2,y2,array);
        }
    }
    illuminate(x1,y1,array);
    illuminate(x2,y2,array);
    
}
//Find circumcenter of triangle
int** circumcenter(IPoint p1, IPoint p2, IPoint p3){
    int** arr = new int*[2];
    arr[0] = new int[2];
    arr[1] = new int[2];

    return arr;
}


int main(){    
    // srand(time(NULL));
    int v[n][m];
    for(int i = 0; i < n; i++){
        for(int j = 0; j < m; j++){
            v[i][j] = 1;
        }
    }
    // IPoint p1 = IPoint();
    // IPoint p2 = IPoint();
    // IPoint p3 = IPoint();

    IPoint p1 = IPoint();
    IPoint p2 = IPoint();
    IPoint p3 = IPoint();


    cout<<"p1.x = "<<p1.get_x()<<endl;
    cout<<"p1.y = "<<p1.get_y()<<endl;
    cout<<"p2.x = "<<p2.get_x()<<endl;
    cout<<"p2.y = "<<p2.get_y()<<endl;
    cout<<"p3.x = "<<p3.get_x()<<endl;
    cout<<"p3.y = "<<p3.get_y()<<endl;



    drawline(p1.get_x(),p2.get_x(),p1.get_y(),p2.get_y(),v);
    drawline(p2.get_x(),p3.get_x(),p2.get_y(),p3.get_y(),v);
    drawline(p3.get_x(),p1.get_x(),p3.get_y(),p1.get_y(),v);
    createppm(v);

    return 0;
}