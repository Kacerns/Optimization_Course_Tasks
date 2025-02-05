#include<iomanip>
#include<iostream>
#include<algorithm>
#include<vector>

using namespace std;

// One Dimensional Optimization Interval Division Method
template <class Type>
void OneDimensionalIntervalDivision(Type l, Type r){
    int Iterations = 5;
    Type L;
    Type Xm;
    for(int i = 0; i<Iterations; ++i){
        try
        {
            Type L = r-l;
            Type Xm = (l+r)/2;
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
    }



    

}




int main(int argc, char *argv[]){



    return 0;
}