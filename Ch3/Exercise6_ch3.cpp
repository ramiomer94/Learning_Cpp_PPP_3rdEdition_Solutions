#include "PPPheaders.h"

// Simple calculator able to handle the four basic math operations (+,-,*,/) on two input values. 
// The user enters three arguments: two double values and a character to represent an operation.

int main(){

    
    double val1;
    double val2;
    char operation;

    cout << "Enter the value of the first operand: ";
    if (!(cin >> val1)){
        cout << "Invalid Input for the first operand\n";
        return 1;
    }
    cout << "Enter the value of the second operand: ";
    if (!(cin >> val2)){
        cout << "Invalid Input for the second operand\n";
        return 1;
    }
    cout << "Choose one of the following operations ('+', '-', '*', '/'): ";
    cin >> operation;

    double result;

    switch (operation){
        case '+':
        result = val1 + val2;
        break;

        case '-':
        result = val1 - val2;
        break;

        case '*':
        result = val1 * val2;
        break;

        case '/':
        if (val2 == 0){
            cout << "ERROR! Devision by Zero!!";
            return 1;
        }
        result = val1 / val2;
        break;

        default:
        cout << "Invalid Inputs!!\n";
        return 1;

    }

    cout << val1 << " " << operation << " " << val2 << " == " << result << '\n';

    return 0;

}