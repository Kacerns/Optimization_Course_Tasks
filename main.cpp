#include<iomanip>
#include<iostream>
#include<algorithm>
#include<cmath>
#include<vector>

using namespace std;

// function to analyse
double f(double x){
    double a = 1;
    double b = 8;
    return (pow(pow(x, 2)-a,2)/b)-1;
}
// One Dimensional Optimisation Interval Division Method
void OneDimensionalIntervalDivision(double l, double r){
    int Iterations = 10;
    double L;
    double Xm;
    for(int i = 0; i<Iterations; ++i){
        double L = r-l;
        double xm = (l+r)/2;

        double x1 = l + L/4;
        double x2 = r - L/4;

        if(f(x1) < f(xm)){
            r = xm;
            xm = x1;
        }
        else if(f(x2) < f(xm)){
            l = xm;
            xm = x2;
        }
        else{
            l = x1;
            r = x2;
        }
        // implement epsilon here;
        }
    cout<<l<<endl;
    cout<<r<<endl;
    }


int main(int argc, char *argv[]){

    OneDimensionalIntervalDivision(0, 10);

    return 0;
}