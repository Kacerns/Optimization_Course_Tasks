#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

using namespace std;

double objectiveFunction(const vector<double>& X){
    double x = X.at(0);
    double y = X.at(1);
    double z = X.at(2);
    return -x * y * z;
}

double equalityConstraint(const vector<double>& X){
    double x = X.at(0);
    double y = X.at(1);
    double z = X.at(2);
    return 2.0 * (x * y + x * z + y * z) - 1.0;
}

vector<double> inequalityConstraints(const vector<double>& X){
    double x = X.at(0);
    double y = X.at(1);
    double z = X.at(2);
    vector<double> h = {-x, -y, -z};
    return h;
}

double calculatePenalty(const vector<double>& X){
    double inequalityPenalty = 0.0;
    vector<double> inequalities = inequalityConstraints(X);
    for (const auto& g_i : inequalities) {
        inequalityPenalty += pow(max(0.0, g_i), 2);
    }
    
    double equalityPenalty = pow(equalityConstraint(X), 2);
    
    return inequalityPenalty + equalityPenalty;
}

double penaltyFunction(const vector<double>& X, double r){
    return objectiveFunction(X) + (1.0/r) * calculatePenalty(X);
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

    Vertice(vector<double> Source, double r){
        Points = Source;
        Value = penaltyFunction(Points, r);
        Lifetime = 0;
    }
};

class Simplex {
    public:
        vector<Vertice> vertices;
        double r_penalty;
        double alpha = 0.0;

        double expand = 2.0;
        double shrink = 0.5;
        double reverse = -0.5;
        
        Simplex(vector<double> X, double a, double r) {
            alpha = a;
            r_penalty = r;
            vertices.push_back(Vertice(X, r_penalty));
            
            double delta1 = ((sqrt(X.size() + 1) + X.size() - 1) / (X.size() * sqrt(2))) * alpha;
            double delta2 = ((sqrt(X.size() + 1) - 1) / (X.size() * sqrt(2))) * alpha;
            
            for(size_t i = 0; i < X.size(); ++i) {
                vector<double> Points = X;
                for(size_t j = 0; j < X.size(); ++j) {
                    if(j == i) {
                        Points[j] = X[j] + delta1;
                    } else {
                        Points[j] = X[j] + delta2;
                    }
                }
                vertices.push_back(Vertice(Points, r_penalty));
            }
        }
    
        void sort_Ascending() {
            sort(vertices.begin(), vertices.end(), [](const Vertice &a, const Vertice &b) {
                return a.Value < b.Value;
            });
        }
    
        vector<double> centroid() {
            vector<double> center(vertices[0].Points.size(), 0.0);
            
            for(size_t i = 0; i < vertices.size() - 1; ++i) {
                for(size_t j = 0; j < vertices[0].Points.size(); ++j) {
                    center[j] += vertices[i].Points[j];
                }
            }
            
            for(size_t j = 0; j < center.size(); ++j) {
                center[j] /= (vertices.size() - 1);
            }
            
            return center;
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
            for(size_t i = 0; i < vertices.size(); ++i) {
                for(size_t j = i + 1; j < vertices.size(); ++j) {
                    double diff = 0;
                    for(size_t k = 0; k < vertices[i].Points.size(); ++k) {
                        diff += abs(vertices[i].Points[k] - vertices[j].Points[k]);
                    }
                    max_diff = max(max_diff, diff);
                }
            }
            return max_diff < 0.0004 * vertices[0].Points.size();
        }

        void shrink_function(){
            for(int i = 1; i < vertices.size(); i++) {
                vector<double> points_to_change = vertices.at(i).Points;
                for(int j = 0; j < vertices[0].Points.size(); j++) {
                    points_to_change.at(j) = vertices.at(0).Points.at(j) + 0.5*(points_to_change.at(j) - vertices.at(0).Points.at(j));
                }
                vertices.at(i) = Vertice(points_to_change, r_penalty);
            }
        }
    };

void PrintValuesWith(const vector<double>& X){
    cout<< "With Variables: ";
    for(auto i : X){cout<<i<<"; ";}
    cout<<"\n";
    cout<< "f(X) value: "<< objectiveFunction(X)<<"\n";
    cout<< "g(X) value: "<< equalityConstraint(X)<<"\n";
    cout<< "hi(X) result: \n";
    vector<double> inequalities = inequalityConstraints(X);
    for(int i = 0; i<3; i++){
        cout<<"h"<<i<<": "<<inequalities[i];
        inequalities[i]<=0? cout<<" <= 0 \n" : cout<<" failed because >= 0 \n";
    }
}

void analyzePenalty(const vector<double>& X){
    cout<< "With Variables: ";
    for(auto i : X){cout<<i<<"; ";}
    cout<<"\n";
    
    vector<double> rValues = {100.0, 10.0, 1.0, 0.1, 0.01};
    
    cout << "f(X): " << objectiveFunction(X) << "\n";
    cout << "b(X): " << calculatePenalty(X) << "\n\n";
    
    cout << "B(X,r) values with different r values:\n";
    for (double r : rValues) {
        cout << "when r = " << r << ", B(X,r) = " << penaltyFunction(X, r) << "\n";
    }
    cout << "\n";
}

void simplex_deformation(vector<double> X){
    
    double r = 100;
    for(int out = 0; out<7;++out){
        cout << "=== Penalty parameter r = " << r << " ===" << "\n";
        int iterations = 0;
        Simplex square(X,0.2,r);
        while(true && iterations < 1000){
            square.sort_Ascending();
            
            if (square.Stop()) {
                cout << "Convergence reached!" << "\n";
                break;
            }
            
            vector<double> centroid = square.centroid();

            int worst_index = square.vertices.size() - 1;
            
            vector<double> reflection_point(X.size(), 0.0);
            for (size_t i = 0; i < X.size(); i++) {
                reflection_point[i] = centroid[i] + (centroid[i] - square.vertices[worst_index].Points[i]);
            }
            
            Vertice reflection(reflection_point, r);
            
            if (square.vertices[0].Value <= reflection.Value && 
                reflection.Value < square.vertices[worst_index-1].Value) {
                square.vertices[worst_index] = reflection;
            }
            else if (reflection.Value < square.vertices[0].Value) {
                vector<double> expanded_point(X.size(), 0.0);
                for (size_t i = 0; i < X.size(); i++) {
                    expanded_point[i] = centroid[i] + square.expand * (reflection_point[i] - centroid[i]);
                }
                
                Vertice expanded(expanded_point, r);
                if (expanded.Value < reflection.Value) {
                    square.vertices[worst_index] = expanded;
                } else {
                    square.vertices[worst_index] = reflection;
                }
            }
            else if (reflection.Value < square.vertices[worst_index].Value) {
                vector<double> contracted_point(X.size(), 0.0);
                for (size_t i = 0; i < X.size(); i++) {
                    contracted_point[i] = centroid[i] + square.shrink * (reflection_point[i] - centroid[i]);
                }
                
                Vertice contracted(contracted_point, r);
                if (contracted.Value < reflection.Value) {
                    square.vertices[worst_index] = contracted;
                } else {
                    square.shrink_function();
                }
            }
            else {
                vector<double> contracted_inside_point(X.size(), 0.0);
                for (size_t i = 0; i < X.size(); i++) {
                    contracted_inside_point[i] = centroid[i] + square.shrink * 
                        (square.vertices[worst_index].Points[i] - centroid[i]);
                }
                
                Vertice contracted_inside(contracted_inside_point, r);
                if (contracted_inside.Value < square.vertices[worst_index].Value) {
                    square.vertices[worst_index] = contracted_inside;
                } else {
                    square.shrink_function();
                }
            }
            for(auto it:square.vertices){it.Lifetime++;}
            iterations++;
        }
        cout<< "Simplex with input: ("<<X.at(0)<<", "<<X.at(1)<<", "<<X.at(2)<<")"<<"\n";
        cout<< "r value: " << r <<"\n";
        cout<<"Iterations: "<<iterations<<"\n";
        cout<<"Resulting Point: "<<square.vertices.at(0).Points.at(0)<<", "<<square.vertices.at(0).Points.at(1)<<", "<<square.vertices.at(0).Points.at(2)<<"\n";
        //cout<<"Function was called: "<<func_call_count<<" times"<<"\n";
        //func_call_count = 0;
        cout<<"\n";

        X = square.vertices[0].Points;
        r *= 0.1;
    }
}

int main() {
    vector<double> X0 = {0,0,0};
    vector<double> X1 = {1,1,1};
    vector<double> Xm = {0.8, 0.1, 0.8};

    // cout << "X0:\n";
    // PrintValuesWith(X0);
    
    // cout << "X1:\n";
    // PrintValuesWith(X1);
    
    // cout << "Xm\n";
    // PrintValuesWith(Xm);
    
    // analyzePenalty(X0);
    // analyzePenalty(X1);
    // analyzePenalty(Xm);
    
    simplex_deformation(Xm);

    return 0;
}