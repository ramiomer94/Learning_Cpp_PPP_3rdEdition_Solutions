#include "PPPheaders.h"


double ctok(double c) { // converts Celsius to Kelvin
    double k = c + 273.25; // k was type int storing c + 273.25 as an integer. fix define k as type double 
    return k;              //error: return int is a compile time error. we return a value in the form of a variable or an expression
                           //       Also the function in the function declaration statement returns a double value not an int value;         
}

int main() {

    double c = 0; // declare input variable
    cin >> c; // Error: cin >> d d is not a declared variable and we want to store our input in c
    double k = ctok(c); //Error the argument for ctok is of type double so we replace the string "c" with the variable of type double c
    cout << k << '\n' ; // Errors: 1- Cout is a syntax error character out method is cout with lower case c
                        //         2- '/n' is not the special character newline so it is treated by the compiler as a string with 
                        //             two characters / and n . Newline character is represented by \n
}