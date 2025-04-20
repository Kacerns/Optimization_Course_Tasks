#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

double objective_function(const vector<double>& X) {
    double x = X.at(0);
    double y = X.at(1);
    double z = X.at(2);
    return -x * y * z;
}

double equality_constraint(const vector<double>& X) {
    double x = X.at(0);
    double y = X.at(1);
    double z = X.at(2);
    return 2.0 * (x * y + x * z + y * z) - 1.0;
}

vector<double> inequality_constraints(const vector<double>& X) {
    double x = X.at(0);
    double y = X.at(1);
    double z = X.at(2);
    vector<double> h = {-x, -y, -z};
    return h;
}

int main() {
    vector<double> initial_point = {0.1, 0.1, 0.1};
    
    cout << "Optimizavimo uždavinys:\n";
    cout << "Minimizuoti f(X) = -x*y*z\n";
    cout << "Kai g(X) = 2(x*y + x*z + y*z) - 1 = 0\n";
    cout << "Ir h1(X) = -x ≤ 0, h2(X) = -y ≤ 0, h3(X) = -z ≤ 0\n";
    
    return 0;
}