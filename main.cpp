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

int func_call_count = 0;
double S1 = 100;
double S2 = 100;
double S3 = 1 - S1 - S2;

double function(vector<double> X){
    func_call_count++;
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

class Simplex {
    public:
        vector<Vertice> vertices;
        double alpha = 0.0;

        double expand = 2.0;
        double shrink = 0.5;
        double reverse = -0.5;
        
        Simplex(vector<double> X, double a) {
            alpha = a;
            vertices.push_back(Vertice(X));
            
            double delta1 = ((sqrt(X.size() + 1) + X.size() - 1) / (X.size() * sqrt(2))) * alpha;
            double delta2 = ((sqrt(X.size() + 1) - 1) / (X.size() * sqrt(2))) * alpha;
            
            for(size_t i = 0; i < X.size(); ++i) {
                vector<double> Points(X.size());
                for(size_t j = 0; j < X.size(); ++j) {
                    if(j == i) {
                        Points.at(j) = X.at(j) + delta1;
                    } else {
                        Points.at(j) = X.at(j) + delta2;
                    }
                }
                vertices.push_back(Vertice(Points));
            }
        }
    
        void sort_Ascending() {
            sort(vertices.begin(), vertices.end(), [](const Vertice &a, const Vertice &b) {
                return a.Value < b.Value;
            });
        }
    
        vector<double> centroid() {
            vector<double> centroid(2);

            for(int i = 0; i < 2; i++){
                centroid.at(i) = vertices.at(0).Points.at(i) + vertices.at(1).Points.at(i);
                centroid.at(i) = centroid.at(i)/2.0;
            }

            return centroid;
        }
    
        bool Exceeded_Lifetime() {
            for(const auto& vertex : vertices) {
                if(vertex.Lifetime > 100) {
                    return true;
                }
            }
            return false;
        }
    
        bool Stop() {
            double max_diff = 0;
            double temp_diff = 0;
            for(size_t i = 0; i < vertices.size(); ++i) {
                for(size_t j = i + 1; j < vertices.size(); ++j) {
                    if(i != j){temp_diff = abs(vertices.at(i).Points.at(0) - vertices.at(j).Points.at(0)) + abs(vertices.at(i).Points.at(1) - vertices.at(j).Points.at(1)); temp_diff /= 2;}
                    max_diff = max(max_diff, temp_diff);
                }
            }
            return max_diff < 0.0004;
        }

        void shrink_function(){
            for(int i = 1; i<vertices.size(); i++){
                vector<double> points_to_change = vertices.at(i).Points;
                for(int j = 0; j<2; j++){
                    points_to_change.at(j) = vertices.at(0).Points.at(j) + 0.5*(points_to_change.at(j) - vertices.at(0).Points.at(j));
                }
                vertices.at(i) = Vertice(points_to_change);
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
        func_call_count +=2;

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
    

    while(epsilon > 0.01){
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

void gradient_descent(vector<double> X){
    int iterations = 0;
    cout<< "gradient descent with input: ("<<X.at(0)<<", "<<X.at(1)<<")"<<endl;
    while(abs(S1 - X.at(0)) > 0.000004 || abs(S2 - X.at(1)) > 0.000004){
        S1 = X.at(0);
        S2 = X.at(1);
        S3 = 1 - S1 - S2;

        Gradient f(X);
        double gamma = -3.5;
        f = f*gamma;
        f + X;
        iterations++;
    }
    cout<<"Iterations: "<<iterations<<endl;
    cout<<"Resulting Point: "<<X.at(0)<<" "<<X.at(1)<<endl;
    cout<<"Function was called: "<<func_call_count<<" times"<<endl;
    cout<<endl;
    func_call_count = 0;
}

void fast_descent(vector<double> X){
    int iterations = 0;
    cout<< "Fast descent with input: ("<<X.at(0)<<", "<<X.at(1)<<")"<<endl;
    while(abs(S1 - X.at(0)) > 0.000004 || abs(S2 - X.at(1)) > 0.000004){
        S1 = X.at(0);
        S2 = X.at(1);
        S3 = 1 - S1 - S2;

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
    cout<<"Resulting Point: "<<X.at(0)<<" "<<X.at(1)<<endl;
    cout<<"Function was called: "<<func_call_count<<" times"<<endl;
    cout<<endl;
    func_call_count = 0;
}

void simplex_deformation(vector<double> X){
    int iterations = 0;
    Simplex triangle(X,0.2);
    while(true && iterations < 1000){
        triangle.sort_Ascending();

        if(triangle.Stop()){break;}

        vector<double> centroid = triangle.centroid();
        vector<double> reflection_point(2, 0.0);

        for(int i = 0; i < 2; i++){
            reflection_point.at(i) = centroid.at(i) + (centroid.at(i) - triangle.vertices.at(2).Points.at(i));
        }

        Vertice Reflection(reflection_point);

        if(triangle.vertices.at(0).Value <= Reflection.Value && Reflection.Value < triangle.vertices.at(1).Value){
            triangle.vertices.at(2) = Reflection;
        }else if(Reflection.Value < triangle.vertices.at(0).Value){
            vector<double> expanded_point(2, 0.0);
            for(int i = 0; i < 2; i++){
                expanded_point.at(i) = centroid.at(i) + triangle.expand*(Reflection.Points.at(i)-centroid.at(i));
            }
            Vertice expanded(expanded_point);
            if(expanded.Value < Reflection.Value){
                triangle.vertices.at(2) = expanded;
            }else{triangle.vertices.at(2) = Reflection;}
        }else if(Reflection.Value < triangle.vertices.at(2).Value){
            vector<double> contracted_point(2, 0.0);
            for(int i = 0; i < 2; i++){
                contracted_point.at(i) = centroid.at(i) + triangle.shrink*(Reflection.Points.at(i)-centroid.at(i));
            }
            Vertice contracted(contracted_point);
            if(contracted.Value < Reflection.Value){
                triangle.vertices.at(2) = contracted;
            }else{triangle.shrink_function();}
        }else{
            vector<double> contracted_inside_point(2, 0.0);
            for(int i = 0; i < 2; i++){
                contracted_inside_point.at(i) = centroid.at(i) + triangle.shrink*(triangle.vertices.at(2).Points.at(i)-centroid.at(i));
            }
            Vertice contracted_inside(contracted_inside_point);
            if(contracted_inside.Value < Reflection.Value){
                triangle.vertices.at(2) = contracted_inside;
            }else{triangle.shrink_function();}
        }
        for(auto it:triangle.vertices){it.Lifetime++;}
        iterations++;
    }
    cout<< "Simplex with input: ("<<X.at(0)<<", "<<X.at(1)<<")"<<endl;
    cout<<"Iterations: "<<iterations<<endl;
    cout<<"Resulting Point: "<<triangle.vertices.at(0).Points.at(0)<<", "<<triangle.vertices.at(0).Points.at(1)<<endl;
    cout<<"Function was called: "<<func_call_count<<" times"<<endl;
    func_call_count = 0;
    cout<<endl;
}


int main(){
    vector<double> smh {1,1};
    gradient_descent(smh);
    fast_descent(smh);
    simplex_deformation(smh);
    return 0;
}
