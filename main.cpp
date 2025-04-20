#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

// Tikslo funkcija: minimizuojame -x*y*z (maksimizuojame tūrį)
double objective_function(const vector<double>& X) {
    double x = X[0];
    double y = X[1];
    double z = X[2];
    return -x * y * z;
}

// Lygybinio apribojimo funkcija: 2(xy + xz + yz) - 1 = 0
double equality_constraint(const vector<double>& X) {
    double x = X[0];
    double y = X[1];
    double z = X[2];
    return 2.0 * (x * y + x * z + y * z) - 1.0;
}

// Nelygybinių apribojimų funkcijos: -x ≤ 0, -y ≤ 0, -z ≤ 0
vector<double> inequality_constraints(const vector<double>& X) {
    double x = X[0];
    double y = X[1];
    double z = X[2];
    vector<double> h = {-x, -y, -z};
    return h;
}

int main() {
    // Pradinis taškas - galima naudoti įvairius optimizavimo algoritmus
    vector<double> initial_point = {0.1, 0.1, 0.1};
    
    cout << "Optimizavimo uždavinys:\n";
    cout << "Minimizuoti f(X) = -x*y*z\n";
    cout << "Kai g(X) = 2(x*y + x*z + y*z) - 1 = 0\n";
    cout << "Ir h1(X) = -x ≤ 0, h2(X) = -y ≤ 0, h3(X) = -z ≤ 0\n";
    
    return 0;
}