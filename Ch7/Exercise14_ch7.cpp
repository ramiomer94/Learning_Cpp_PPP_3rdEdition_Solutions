#include "PPPheaders.h"

/*
    Can we declare a non-reference function argument const (e.g., void f(const int);)? 
    Ans: Yes

    What might that mean?
    Ans: passing a copy of a variable as a constant 
    
    Why might we want to do that?
    Ans: to make sure that the copy is not accidentally modified inside the function

    Why don’t people do that often?
    Ans: copying big arguments can be expensive and slows down the execution of the code

*/
double convert (const double conversion_factor, double x){
    double y = x * conversion_factor;
    return y;
}

int main(){
    double conversion_factor = 15.2;
    double input = 20;
    cout << convert(conversion_factor, input) << '\n';
}