#include "PPPheaders.h"


// a program to solve quadratic equations. A quadratic equation is of the form ax2 + bx + c = 0.
int main(){
    cout << "This program finds the real root of a quadratic equation."
         << "\nThe user needs only to enter the coefficients a,b&c of the equation they wish to solve.\n"
         << "Please enter the values of 'a', 'b' & 'c': ";
    
    double a;
    double b;
    double c;

    cin >> a >> b >> c;

    // Check if it's actually a quadratic equation
    if (a == 0){
        cout << "This is not a quadratic equation (a must not be zero).\n";
        return 0;
    }
    // Check for complex roots: if discriminant < 0, the roots are complex
    if (pow(b,2) < (4 * a * c )){

        cout << "The roots of this equation are complex. This program find only the real root of a quadratic equation.\n"
             << "To get the complex roots use the <complex> library";
        return 0;
    }
    double sqrt_in_numerator = sqrt(pow(b,2) - (4 * a * c));
    double first_result = ((-b) + sqrt_in_numerator) / (2 * a);
    double second_result =  ((-b) - sqrt_in_numerator)/(2 * a);

    // Handle the case of a repeated root
    if (first_result == second_result)
        cout << "The equation has one real root and it is x == " << first_result << '\n';
    else
        cout << "x == " << first_result << " or x == " << second_result << '\n';
}