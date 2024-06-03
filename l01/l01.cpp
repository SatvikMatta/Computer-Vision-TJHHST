// Satvik Matta
// 9/9/2022

#include <iostream>
#include <fstream>
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
        double xt, yt;
    public:
        IPoint(int x, int y) {
            this->x = x;
            this->y = y;
            this->xt = x/800.0;
            this->yt = y/800.0;
        }
        IPoint() {
            double d = random_double();
            double d2 = random_double();
            this->xt = d;
            this->yt = d2;
            convert();
        }
        int get_x() {
            return x;
        }
        int get_y() {
            return y;
        }
        double get_xt() {
            return xt;
        }
        double get_yt() {
            return yt;
        }
        void convert(){
            double d = (this->xt) * n;
            double d2 = (this->yt) * m;
            this->x = round(d);
            this->y = round(d2);
        }
};

 void illuminate(int i,int j, int array[n][m] ){
    if (i > 0 && j > 0 && i < n && j < m){
        array[i][j] = 0;
    }
    else{
        cout << "Skipped: Out of bounds" << endl;
    }
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
    myfile.open ("triangle.ppm");
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
//Rasterized circle
void drawcircle(int xc, int yc, double r, int array[m][n]){
    int xmax = (int)(r * 0.70710678); //118
    int y = r;
    int y2 = y*y;
    int ty = (2*y)-1;
    int y2_new = y2;

    for(int x = 0; x <= xmax+2; x++){
        if((y2 - y2_new) >= ty){
            y2 -= ty;
            y -=1;
            ty -= 2;
        }
        illuminate(xc+x,yc+y,array);
        illuminate(xc+x,yc-y,array);
        illuminate(xc-x,yc+y,array);
        illuminate(xc-x,yc-y,array);
        illuminate(xc+y,yc+x,array);
        illuminate(xc+y,yc-x,array);
        illuminate(xc-y,yc+x,array);
        illuminate(xc-y,yc-x,array);
        y2_new -= (2*x)-3;
        
    }

}

void drawcircumcircle(IPoint p1, IPoint p2, IPoint p3, int array[m][n]){
    double d = 2*(p1.get_xt()*(p2.get_yt()-p3.get_yt()) + p2.get_xt()*(p3.get_yt()-p1.get_yt()) + p3.get_xt()*(p1.get_yt()-p2.get_yt()));
    double x = ((p1.get_xt()*p1.get_xt() + p1.get_yt()*p1.get_yt())*(p2.get_yt()-p3.get_yt()) + (p2.get_xt()*p2.get_xt() + p2.get_yt()*p2.get_yt())*(p3.get_yt()-p1.get_yt()) + (p3.get_xt()*p3.get_xt() + p3.get_yt()*p3.get_yt())*(p1.get_yt()-p2.get_yt()))/d;
    double y = ((p1.get_xt()*p1.get_xt() + p1.get_yt()*p1.get_yt())*(p3.get_xt()-p2.get_xt()) + (p2.get_xt()*p2.get_xt() + p2.get_yt()*p2.get_yt())*(p1.get_xt()-p3.get_xt()) + (p3.get_xt()*p3.get_xt() + p3.get_yt()*p3.get_yt())*(p2.get_xt()-p1.get_xt()))/d;
    int x1 = round(x*m);
    int y1 = round(y*n);
    double radius  = sqrt((x-p1.get_xt())*(x-p1.get_xt()) + (y-p1.get_yt())*(y-p1.get_yt()));
    double r = radius*m;
    drawcircle(x1,y1,r,array);
}

double getnineradius(IPoint p1, IPoint p2, IPoint p3){
    double d = 2*(p1.get_xt()*(p2.get_yt()-p3.get_yt()) + p2.get_xt()*(p3.get_yt()-p1.get_yt()) + p3.get_xt()*(p1.get_yt()-p2.get_yt()));
    double x = ((p1.get_xt()*p1.get_xt() + p1.get_yt()*p1.get_yt())*(p2.get_yt()-p3.get_yt()) + (p2.get_xt()*p2.get_xt() + p2.get_yt()*p2.get_yt())*(p3.get_yt()-p1.get_yt()) + (p3.get_xt()*p3.get_xt() + p3.get_yt()*p3.get_yt())*(p1.get_yt()-p2.get_yt()))/d;
    double y = ((p1.get_xt()*p1.get_xt() + p1.get_yt()*p1.get_yt())*(p3.get_xt()-p2.get_xt()) + (p2.get_xt()*p2.get_xt() + p2.get_yt()*p2.get_yt())*(p1.get_xt()-p3.get_xt()) + (p3.get_xt()*p3.get_xt() + p3.get_yt()*p3.get_yt())*(p2.get_xt()-p1.get_xt()))/d;
    double radius  = sqrt((x-p1.get_xt())*(x-p1.get_xt()) + (y-p1.get_yt())*(y-p1.get_yt()));
    double r = radius*m/2;
    return r;
}

void drawincirlce(IPoint p1, IPoint p2, IPoint p3, int array[m][n]){
    double a = sqrt((p2.get_xt()-p3.get_xt())*(p2.get_xt()-p3.get_xt()) + (p2.get_yt()-p3.get_yt())*(p2.get_yt()-p3.get_yt()));
    double b = sqrt((p1.get_xt()-p3.get_xt())*(p1.get_xt()-p3.get_xt()) + (p1.get_yt()-p3.get_yt())*(p1.get_yt()-p3.get_yt()));
    double c = sqrt((p1.get_xt()-p2.get_xt())*(p1.get_xt()-p2.get_xt()) + (p1.get_yt()-p2.get_yt())*(p1.get_yt()-p2.get_yt()));
    double xc = (a*p1.get_xt() + b*p2.get_xt() + c*p3.get_xt())/(a+b+c);
    double yc = (a*p1.get_yt() + b*p2.get_yt() + c*p3.get_yt())/(a+b+c);
    double s = (a+b+c)/2;
    double radius = sqrt((s-a)*(s-b)*(s-c)/s);
    int x1 = round(xc*m);
    int y1 = round(yc*n);
    double r = radius*m;

    drawcircle(x1,y1,r,array);
}



void drawNinePointCircle(IPoint p1, IPoint p2, IPoint p3, int array[m][n]){
    double a = sqrt((p2.get_xt()-p3.get_xt())*(p2.get_xt()-p3.get_xt()) + (p2.get_yt()-p3.get_yt())*(p2.get_yt()-p3.get_yt()));
    double b = sqrt((p1.get_xt()-p3.get_xt())*(p1.get_xt()-p3.get_xt()) + (p1.get_yt()-p3.get_yt())*(p1.get_yt()-p3.get_yt()));
    double c = sqrt((p1.get_xt()-p2.get_xt())*(p1.get_xt()-p2.get_xt()) + (p1.get_yt()-p2.get_yt())*(p1.get_yt()-p2.get_yt()));
    //find angles using law of cosines
    double alpha = acos((b*b + c*c - a*a)/(2*b*c));
    double beta = acos((a*a + c*c - b*b)/(2*a*c));
    double gamma = acos((a*a + b*b - c*c)/(2*a*b));
    //find the orthocenter
    double xc = (p1.get_xt()*tan(alpha) + p2.get_xt()*tan(beta) + p3.get_xt()*tan(gamma))/(tan(alpha) + tan(beta) + tan(gamma));
    double yc = (p1.get_yt()*tan(alpha) + p2.get_yt()*tan(beta) + p3.get_yt()*tan(gamma))/(tan(alpha) + tan(beta) + tan(gamma));
    //find the radius
    double radius = getnineradius(p1,p2,p3);
    //find the center of the circle
    double x1 = xc*m;
    double y1 = yc*n;

    double d = 2*(p1.get_xt()*(p2.get_yt()-p3.get_yt()) + p2.get_xt()*(p3.get_yt()-p1.get_yt()) + p3.get_xt()*(p1.get_yt()-p2.get_yt()));
    double x = ((p1.get_xt()*p1.get_xt() + p1.get_yt()*p1.get_yt())*(p2.get_yt()-p3.get_yt()) + (p2.get_xt()*p2.get_xt() + p2.get_yt()*p2.get_yt())*(p3.get_yt()-p1.get_yt()) + (p3.get_xt()*p3.get_xt() + p3.get_yt()*p3.get_yt())*(p1.get_yt()-p2.get_yt()))/d;
    double y = ((p1.get_xt()*p1.get_xt() + p1.get_yt()*p1.get_yt())*(p3.get_xt()-p2.get_xt()) + (p2.get_xt()*p2.get_xt() + p2.get_yt()*p2.get_yt())*(p1.get_xt()-p3.get_xt()) + (p3.get_xt()*p3.get_xt() + p3.get_yt()*p3.get_yt())*(p2.get_xt()-p1.get_xt()))/d;
    double x2 = x*m;
    double y2= y*n;
    
    int xc1 = round((x1+x2)/2);
    int yc1 = round((y1+y2)/2);
    //draw the circle
    drawcircle(xc1,yc1,radius,array);

    //calculate slope between orthocenter and circumcenter
    double slope = (yc-y)/(xc-x);
    //calculate the y-intercept
    double yint = yc - slope*xc;
    double max = slope + yint;
    // drawline(0,m,round(yint*m),round(max*m),array);


    
}

int main(){    
    srand(time(NULL));
    int v[n][m];
    for(int i = 0; i < n; i++){
        for(int j = 0; j < m; j++){
            v[i][j] = 1;
        }
    }

    IPoint p1 = IPoint();
    IPoint p2 = IPoint();
    IPoint p3 = IPoint();


    // cout<<"p1.x = "<<p1.get_x()<<endl;
    // cout<<"p1.y = "<<p1.get_y()<<endl;
    // cout<<"p2.x = "<<p2.get_x()<<endl;
    // cout<<"p2.y = "<<p2.get_y()<<endl;
    // cout<<"p3.x = "<<p3.get_x()<<endl;
    // cout<<"p3.y = "<<p3.get_y()<<endl;
    // drawline(p1.get_x(),p2.get_x(),p1.get_y(),p2.get_y(),v);
    // drawline(p2.get_x(),p3.get_x(),p2.get_y(),p3.get_y(),v);
    // drawline(p3.get_x(),p1.get_x(),p3.get_y(),p1.get_y(),v);
    drawline(0,800,1408,-1994,v);
    // drawcircumcircle(p1,p2,p3,v);
    // drawincirlce(p1,p2,p3,v);
    // drawNinePointCircle(p1,p2,p3,v);
    createppm(v);

    return 0;
}