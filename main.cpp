#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

using namespace std;

double objective_function(const vector<double>& X){
    double x = X.at(0);
    double y = X.at(1);
    double z = X.at(2);
    return -x * y * z;
}

double equality_constraint(const vector<double>& X){
    double x = X.at(0);
    double y = X.at(1);
    double z = X.at(2);
    return 2.0 * (x * y + x * z + y * z) - 1.0;
}

vector<double> inequality_constraints(const vector<double>& X){
    double x = X.at(0);
    double y = X.at(1);
    double z = X.at(2);
    vector<double> h = {-x, -y, -z};
    return h;
}

double calculatePenalty(const vector<double>& X) {
    double inequalityPenalty = 0.0;
    vector<double> inequalities = inequality_constraints(X);
    for (const auto& g_i : inequalities) {
        inequalityPenalty += pow(max(0.0, g_i), 2);
    }
    
    double equalityPenalty = pow(equality_constraint(X), 2);
    
    return inequalityPenalty + equalityPenalty;
}

double penaltyFunction(const vector<double>& X, double r) {
    return objective_function(X) + (1.0/r) * calculatePenalty(X);
}

void PrintValuesWith(const vector<double>& X){
    cout<< "With Variables: ";
    for(auto i : X){cout<<i<<"; ";}
    cout<<"\n";
    cout<< "f(X) value: "<< objective_function(X)<<"\n";
    cout<< "g(X) value: "<< equality_constraint(X)<<"\n";
    cout<< "hi(X) result: \n";
    vector<double> inequalities = inequality_constraints(X);
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
    
    cout << "f(X): " << objective_function(X) << "\n";
    cout << "b(X): " << calculatePenalty(X) << "\n\n";
    
    cout << "B(X,r) values with different r values:\n";
    for (double r : rValues) {
        cout << "when r = " << r << ", B(X,r) = " << penaltyFunction(X, r) << "\n";
    }
    cout << "\n";
}

int main() {
    vector<double> X0 = {0,0,0};
    vector<double> X1 = {1,1,1};
    vector<double> Xm = {0.8, 0.1, 0.8};

    cout << "X0:\n";
    PrintValuesWith(X0);
    
    cout << "X1:\n";
    PrintValuesWith(X1);
    
    cout << "Xm\n";
    PrintValuesWith(Xm);
    
    analyzePenalty(X0);
    analyzePenalty(X1);
    analyzePenalty(Xm);
    
    return 0;
}