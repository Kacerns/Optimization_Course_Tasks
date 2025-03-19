#include <iostream>
#include <iomanip>
#include <cmath>
#include <vector>
#include <limits>
#include <random>
#include<algorithm>
#include <fstream>
#include <sstream>

using namespace std;

double S1 = 100;
double S2 = 100;
double S3 = 1 - S1 - S2;

class Simplex{
    public:
    vector<pair<vector<double>, int>> vertices;

    Simplex(vector<double> X, double alpha){
        vertices.push_back({X, 0});
        double delta1 = ((sqrt(X.size()+1) + X.size() - 1)/(X.size()*sqrt(2)))*alpha;
        double delta2 = ((sqrt(X.size()+1) - 1)/(X.size()*sqrt(2)))*alpha;
        for(int i = 0; i < X.size(); ++i){
            vector<double> vertice;
            vertice.reserve(2);
            for(int j = 0; j < X.size(); ++i){
                if(j==i){vertice.at(j) = X.at(j) + delta1;}
                else{vertice.at(j) = X.at(j) + delta2;}
            }
            vertices.push_back({vertice, 0});
        }
    }
};

class Gradient{
    public:
    vector<double> Results;

    Gradient(vector<double> X){
        double x1 = X.at(0);
        double x2 = X.at(1);
        double coefficient = -0.125;

        double Calc_With_x1 = coefficient * x2 * (1 - 2*x1 - x2);
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
            X.at(0) += Results.at(0);
            X.at(1) += Results.at(1);
        }
        return *this;
    }

};

double function(vector<double> X){
    if(X.size() == 2){
        return (-1.0/8.0) * X.at(0) * X.at(1) * (1-X.at(0)-X.at(1));
    }
}
double minimization_function(double gamma, vector<double> X){
    Gradient g(X);
    g * gamma;
    return function({X.at(0) + (-1*g.Results.at(0)), X.at(1) + (-1*g.Results.at(1))});
}

double GoldenRatio(double l, double r, vector<double> X){

    int it_count = 0;
    double L = r - l;
    double Ratio = 0.61803;
    double x1 = r - Ratio*L;
    double x2 = l + Ratio*L;
    double epsilon = 1;

    double fx1 = minimization_function(x1, X);
    double fx2 = minimization_function(x2, X);
    

    while(epsilon > 0.0001){
        if(fx2<fx1){
            l = x1;
            L = r - l;
            x1 = x2;
            fx1 = fx2;
            x2 = l + Ratio*L;
            fx2 = minimization_function(x2, X);
        }
        else{
            r = x2;
            L = r - l;
            x2 = x1;
            fx2 = fx1;
            x1 = r - Ratio*L;
            fx1 = minimization_function(x1, X);
        }
        it_count++;
        epsilon = r-l;
    }
    // cout<<"Function Call Count: "<<func_call_count<<endl;
    return (l+r)/2.0;
    // func_call_count = 0;
}

void gradient_descent(vector<double> &X){
    int iterations = 0;
    while(abs(S1 - X.at(0)) > 0.000004 || abs(S2 - X.at(1)) > 0.000004){
        S1 = X.at(0);
        S2 = X.at(1);
        S3 = 1 - S1 - S2;

        cout<<X.at(0)<<" "<<X.at(1)<<endl;
        Gradient f(X);
        double gamma = -3.5;
        f = f*gamma;
        f + X;
        iterations++;
    }
    cout<<"Iterations: "<<iterations<<endl;
}

void fast_descent(vector<double> &X){
    int iterations = 0;
    while(abs(S1 - X.at(0)) > 0.000004 || abs(S2 - X.at(1)) > 0.000004){
        S1 = X.at(0);
        S2 = X.at(1);
        S3 = 1 - S1 - S2;

        cout<<X.at(0)<<" "<<X.at(1)<<endl;
        Gradient f(X);
        double grad_norm = sqrt(pow(f.Results.at(0), 2) + pow(f.Results.at(1), 2));
        double r = 1.0 / grad_norm;
        if (r < 0.1) r = 0.1;
        if (r > 10.0) r = 10.0;
        double gamma = GoldenRatio(0.0, r, X);
        f = (f*gamma);
        f = f*(-1.0);
        f + X;
        iterations++;
    }
    cout<<"Iterations: "<<iterations<<endl;
}


int main(){
    vector<double> smh {0.1,0.8};
    // gradient_descent(smh);
    vector<double> smd {1,1};
    fast_descent(smd);

    return 0;
}
