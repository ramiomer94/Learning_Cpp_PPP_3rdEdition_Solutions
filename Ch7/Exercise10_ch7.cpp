#include "PPPheaders.h"

// Return the largest element in a non-empty vector<double>
double maxv(const vector<double>& v){
    if(v.size() == 0) error("non-empty vector expected");

    double max_value = v[0];
    for(const double& element : v)
        if(element > max_value)
            max_value = element; // update when we find a larger value
    
    return max_value;
}


int main(){

    //just an example
    vector<double> test_v = {100,-10,20.234, 22.14, 100, 0, 0.6, 100.23, -1000, 100, 20.4, -0.5};
    double max = maxv(test_v);

    cout << max << '\n';
}