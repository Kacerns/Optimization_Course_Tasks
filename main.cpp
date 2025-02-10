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
    int Iterations = 20;
    double L;
    double xm;
    for(int i = 0; i<Iterations; ++i){
        L = r-l;
        xm = (l+r)/2;

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

void GoldenRatio(double l, double r){
    double L = r - l;
    double Ratio = 0.61803;
    double x1 = r - Ratio*L;
    double x2 = l + Ratio*L;

    for(int i = 0; i<20; ++i){
        if(f(x2)<f(x1)){
            l = x1;
            L = r - l;
            x1 = x2;
            x2 = l + Ratio*L;
        }
        else{
            r = x2;
            L = r - l;
            x2 = x1;
            x1 = r - Ratio*L;
        }
    }
    cout<<l<<endl;
    cout<<r<<endl;
}


int main(int argc, char *argv[]){

    OneDimensionalIntervalDivision(0, 10);
    GoldenRatio(0,10);

    return 0;
}