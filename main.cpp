#include<iomanip>
#include<iostream>
#include<algorithm>
#include<cmath>
#include<vector>
#include<fstream>

using namespace std;

int func_call_count = 0;

// function to analyse
double f(double x){
    func_call_count++;
    double a = 1;
    double b = 8;
    return (pow(pow(x, 2)-a,2)/b)-1;
}
// derivative of function f
double f_(double x){
    func_call_count++;
    double a = 1;
    double b = 8;
    return (4*x*(pow(x, 2)-a))/b;
}
// double derivative of function f
double f__(double x){
    func_call_count++;
    double a = 1;
    double b = 8;
    return (12*pow(x, 2)-4*a)/b;
}
// One Dimensional Optimisation Interval Division Method
void OneDimensionalIntervalDivision(double l, double r){
    double L;
    double xm = (l+r)/2;
    double epsilon = 1;
    int it_count = 0;
    double x1,x2, up, low;

    double middle = f(xm);


    while(epsilon > 0.0001){
        L = r-l;
        xm = (l+r)/2;

        x1 = l + L/4;

        low = f(x1);
        
        if(low < middle){
            r = xm;
            xm = x1;
            middle = low;
        }
        else{
            x2 = r - L/4;
            up = f(x2);
            if(up < middle){
                l = xm;
                xm = x2;
                middle = up;
            }
            else{
                l = x1;
                r = x2;
            }
        }
        it_count++;
        epsilon = r-l;
    }
    cout<<it_count<<endl;
    cout<<func_call_count<<endl;
    cout<<l<<endl;
    cout<<r<<endl;
    func_call_count = 0;
}

// Golden Ratio Method
void GoldenRatio(double l, double r){
    int it_count = 0;

    double L = r - l;
    double Ratio = 0.61803;
    double x1 = r - Ratio*L;
    double x2 = l + Ratio*L;
    double epsilon = 1;

    double fx2 = f(x2);
    double fx1 = f(x1);

    while(epsilon > 0.0001){
        if(fx2<fx1){
            l = x1;
            L = r - l;
            x1 = x2;
            fx1 = fx2;
            x2 = l + Ratio*L;
            fx2 = f(x2);
        }
        else{
            r = x2;
            L = r - l;
            x2 = x1;
            fx2 = fx1;
            x1 = r - Ratio*L;
            fx1 = f(x1);
        }
        it_count++;
        epsilon = r-l;
    }
    cout<<it_count<<endl;
    cout<<func_call_count<<endl;
    cout<<l<<endl;
    cout<<r<<endl;
    func_call_count = 0;
}

// Newtons Method
void NewtonsMethod(double x){
    double x0 = x+1;
    int it_count = 0;
    while(x0-x > 0.0001){
        x0 = x;
        x = x - f_(x)/f__(x);
        it_count++;
    }
    cout<<it_count<<endl;
    cout<<func_call_count<<endl;
    cout<<x<<endl;
}
int main(int argc, char *argv[]){
    cout<<"Interval Division: "<<endl;
    OneDimensionalIntervalDivision(0, 10);
    cout<<"Golden Ratio: "<<endl;
    GoldenRatio(0,10);
    cout<<"Newtons Method: "<<endl;
    NewtonsMethod(5);
    return 0;
}