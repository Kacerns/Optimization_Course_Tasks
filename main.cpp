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

double function(vector<double> X){
    return (-1.0/8.0) * X.at(0) * X.at(1) * (1-X.at(0)-X.at(1));
}

class Vertice{
    public:
    vector<double> Points;
    double Value;
    int Lifetime;

    Vertice(){
        Value = 0;
        Lifetime = 0;
    }

    Vertice(vector<double> Source){
        Points = Source;
        Value = function(Points);
        Lifetime = 0;
    }
};

class Simplex{
    public:
    vector<Vertice> vertices;
    double alpha;

    double expand = 2.0;
    double shrink = 0.5;
    double reverse = -0.5;
       
    int best_index = 0;

    Simplex(vector<double> X, double a){
        alpha = a;

        vertices.push_back(Vertice(X));
        double delta1 = ((sqrt(X.size()+1) + X.size() - 1)/(X.size()*sqrt(2)))*alpha;
        double delta2 = ((sqrt(X.size()+1) - 1)/(X.size()*sqrt(2)))*alpha;
        for(int i = 0; i < X.size(); ++i){
            vector<double> Points{0,0};
            for(int j = 0; j < X.size(); ++j){
                if(j==i){Points.at(j) = X.at(j) + delta1;}
                else{Points.at(j) = X.at(j) + delta2;}
            }
            vertices.push_back(Vertice(Points));
        }
    }

    int get_WorstPoint_index() {
        int worst_index = 0;
        for (int i = 0; i < vertices.size(); ++i) {
            if (vertices.at(i).Value > vertices.at(worst_index).Value) {
                worst_index = i;
            }
        }
        return worst_index;
    }

    void set_new_point(int worst_index){
        Vertice Xdeformed;

        vector<double> midpoint {0.0,0.0};
        int n = vertices.size();

        for(int i = 0; i < n; ++i){
            if(i != worst_index){
                midpoint.at(0)+=vertices.at(i).Points.at(0);
                midpoint.at(1)+=vertices.at(i).Points.at(1);
            }
        }
        for(int i = 0; i < 2; i++){midpoint.at(i) = midpoint.at(i)/(n-1);}

        vector<double> Point_xnew = vertices.at(worst_index).Points;
        for(int i = 0; i < 2; i++){Point_xnew.at(i) =  midpoint.at(i) + (midpoint.at(i)-Point_xnew.at(i));}
        Vertice Xnew(Point_xnew);

        int second_best_index = 0;        
        best_index = 0;

        for(int i = 0; i<vertices.size(); ++i){
            if(vertices.at(i).Value < vertices.at(best_index).Value){second_best_index = best_index; best_index = i;}
        }

        vector<double> Point_Deformed = vertices.at(worst_index).Points;
        if(vertices.at(best_index).Value < Xnew.Value && Xnew.Value < vertices.at(second_best_index).Value){
            Xdeformed = Xnew;
        }else if(Xnew.Value < vertices.at(best_index).Value){
            for(int i = 0; i < 2; i++){Point_Deformed.at(i) = midpoint.at(i) + expand * (midpoint.at(i) - Point_Deformed.at(i));}
        }else if(Xnew.Value > vertices.at(worst_index).Value){
            for(int i = 0; i < 2; i++){Point_Deformed.at(i) = midpoint.at(i) + shrink * (midpoint.at(i) - Point_Deformed.at(i));}
        }else{
            for(int i = 0; i < 2; i++){Point_Deformed.at(i) = midpoint.at(i) + reverse * (midpoint.at(i) - Point_Deformed.at(i));}
        }

        Xdeformed = Vertice(Point_Deformed);
        for(auto it:vertices){it.Lifetime++;}
        vertices.at(worst_index) = Xdeformed;
    }

    bool Exceeded_Lifetime(){
        for(auto it:vertices){if(it.Lifetime>100){return true;}}
        return false;
    }

    bool Stop(){
        double max_diff = 0;
        for (int i = 0; i < vertices.size(); ++i) {
            for (int j = i + 1; j < vertices.size(); ++j) {
                max_diff = max(max_diff, abs(vertices[i].Value - vertices[j].Value));
            }
        }
        return max_diff < 0.000004;
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
        f = f*gamma;
        f = f*(-1.0);
        f + X;
        iterations++;
    }
    cout<<"Iterations: "<<iterations<<endl;
}

void simplex_deformation(vector<double> X){
    int iterations = 0;
    Simplex triangle(X,1);
    while(!triangle.Stop()){
        cout<<triangle.vertices.at(0).Points.at(0)<<" "<<triangle.vertices.at(0).Points.at(1)<<endl;
        cout<<triangle.vertices.at(1).Points.at(0)<<" "<<triangle.vertices.at(1).Points.at(1)<<endl;
        cout<<triangle.vertices.at(2).Points.at(0)<<" "<<triangle.vertices.at(2).Points.at(1)<<endl;
        int worst_point = triangle.get_WorstPoint_index();
        triangle.set_new_point(worst_point);
        if(triangle.Exceeded_Lifetime()){
            triangle = Simplex(triangle.vertices.at(triangle.best_index).Points, triangle.alpha*0.5);
        }
        iterations++;
    }
    cout<<triangle.vertices.at(0).Points.at(0)<<" "<<triangle.vertices.at(0).Points.at(1)<<endl;
    cout<<triangle.vertices.at(1).Points.at(0)<<" "<<triangle.vertices.at(1).Points.at(1)<<endl;
    cout<<triangle.vertices.at(2).Points.at(0)<<" "<<triangle.vertices.at(2).Points.at(1)<<endl;
}


int main(){
    // vector<double> smh {0.1,0.8};
    // // gradient_descent(smh);
    // vector<double> smd {1,1};
    // fast_descent(smd);

    vector<double> sma{1,1};
    simplex_deformation(sma);
    return 0;
}
