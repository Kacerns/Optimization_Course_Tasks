#include <iostream>
#include <iomanip>
#include <cmath>
#include <vector>
#include <limits>
#include <random>
#include<algorithm>

using namespace std;

double S1 = 100;
double S2 = 100;
double S3 = 1 - S1 - S2;

class Gradient{
    public:
    vector<double> Results;

    Gradient(vector<double> X){
        double x1 = X.at(0);
        double x2 = X.at(1);
        double coefficient = -0.125;

        double Calc_With_x1 = coefficient * (x1 - pow(x2, 2) - 2*x1*x2);
        double Calc_With_x2 = coefficient * (x1 - pow(x1, 2) - 2*x1*x2);

        Results.push_back(Calc_With_x1);
        Results.push_back(Calc_With_x2);
    }

    Gradient& operator*(double multiplier){
        Results.at(0) = Results.at(0)*multiplier;
        Results.at(1) = Results.at(1)*multiplier;
        return *this;
    }

    Gradient& operator+(vector<double> &X){
        if(X.size() == 2){
            X.at(0) = X.at(0) + Results.at(0);
            X.at(1) = X.at(1) + Results.at(1);
        }
        return *this;
    }

};

void gradient_descent(vector<double> &X){
    int iterations = 0;
    while(S1 - X.at(0) > 0.000004 || S2 - X.at(1) > 0.000004){
        S1 = X.at(0);
        S2 = X.at(1);
        S3 = 1 - S1 - S2;

        cout<<X.at(0)<<" "<<X.at(1)<<endl;
        Gradient f(X);
        double gamma = -3.0;
        f = f*gamma;
        f + X;
        iterations++;
    }
    cout<<"Iterations: "<<iterations<<endl;
}

int main(){
    vector<double> smh {1,1};
    gradient_descent(smh);
    return 0;
}
