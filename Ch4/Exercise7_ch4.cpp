#include "PPPheaders.h"

//Finds all the real roots of a quadratic equation of the form a*x2 + b*x + c = 0
vector<double> quadratic_sln (double a, double b, double c){ 
    //The numerator of the quadratic formula must be a real number: b2 >= 4*a*c 
    expect([&]{return (b * b) >= (4 * a * c);}, 
    "Argument in the sqrt of the discriminant is negative. The equation has non real roots.");

    vector<double> real_roots; //store all real roots in vector real_roots

    double discriminant = sqrt(b*b - 4*a*c);
    double numerator1 = (-b) + discriminant;
    double numerator2 = (-b) - discriminant;
    double denominator = 2 * a;
    double x1 = numerator1/denominator;
    double x2 = numerator2/denominator;

    if (x1 == x2) //If the equation has only one real root
        real_roots.push_back(x1);
    else{ // If the equation has two real roots
        real_roots.push_back(x1);
        real_roots.push_back(x2);
    }

    return real_roots;

}

int main(){

   try {
    double a;
    double b;
    double c;

    cin >> a >> b >> c;

    if(cin) {
        vector<double> sln = quadratic_sln(a,b,c);

        if (sln.size() == 1)
            cout << "The real root of the quadratic equation with coefficients a==" << a << ", b==" << b
                << ", c==" << c << " is: x==" << sln[0] << '\n';
        else
            cout << "The real root of the quadratic equation with coefficients a==" << a << ", b==" << b
                << ", c==" << c << " is: x1==" << sln[0] << ", x2==" << sln[1] << '\n';

    }else
        error("One or more inputs is/are not valid double(s)\n");
   } catch (exception& e){
        cerr << "Exception caught: " << e.what() << '\n';
   }

}