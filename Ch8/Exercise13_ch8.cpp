#include "PPPheaders.h"

/* Normalize a fraction (n/d):
  - forbid zero denominator
  - force denominator positive (carry sign to numerator)
  - reduce by the greatest common denominator (gcd) to lowest terms
*/
void normalize(long int& n, long int& d){
    if(d == 0) error("Denominator = 0");
    if(d < 0) {
        n = -n;
        d = -d;
    }
    long int g = gcd(n,d);     // The greatest common denominator
    if(g != 1) {
        n /= g;
        d /= g;
    }
}


class Rational {
    public:
            Rational(){}                                                // default: 0/1
            Rational(long int n, long int d);                           // if rational a/-b, this 2-arg constructor makes it -a/b
            Rational(long int n) : numerator{n}, denominator{1}{}       // 1-arg constructor to produce an int --> n/1
           
            long int get_numerator() const { return numerator; }
            long int get_denominator() const { return denominator; }
            void change_numerator(const long int& n) { numerator = n; }
            void change_denominator(const long int& d);

            // copy assignment
            Rational& operator= (const Rational& other){
                if(this == &other) return *this;

                numerator = other.get_numerator();
                denominator = other.get_denominator();
                return *this;
            }

    private:
            long int numerator = 0;
            long int denominator = 1;
};

// 2-arg constructor enforces nonzero denominator and a positive denominator.
Rational::Rational(long int n, long int d) : numerator{n}, denominator{d} {
    if(d == 0) error("Denominator = 0");
    if(d < 0) {denominator = -denominator; numerator = -numerator;}     // num/-den --> -num/den
}

void Rational::change_denominator(const long int& d) { 
    if (d == 0) error("Denominator = 0");
    if(d < 0) { denominator = -denominator; numerator = -numerator; } // ensure a positive denominator
    else denominator = d;
}


ostream& operator<< (ostream& os, const Rational& r){
    if(r.get_numerator() == 0)      return os << 0;                     //if numerator = 0 , print 0
    if(r.get_denominator() == 1)    return os << r.get_numerator();     //if denominator = 1, print just an int numerator
    return os << r.get_numerator() << '/' << r.get_denominator();       // otherwise print num/den
}


Rational operator+ (const Rational& x , const Rational& y){
    
    long int a = x.get_numerator(); long int b = x.get_denominator();   //x = a/b
    long int c = y.get_numerator(); long int d = y.get_denominator();   //y = c/d


    long int g = gcd(b, d);                                             // greatest common denominator
    long int l = b/g * d;                                               // l is the least common multiple
    long int num = a * (l/b) + c * (l/d);                               

    normalize(num,l);                                                   // reduce the result rational to its simplest form
    return Rational{num,l};
}

Rational operator- (const Rational& x , const Rational& y){
    long int a = x.get_numerator(); long int b = x.get_denominator();   //x = a/b
    long int c = y.get_numerator(); long int d = y.get_denominator();   //y = c/d

    long int g = gcd(b, d);
    long int l = b/g * d;
    long int num = a * (l/b) - c * (l/d);
    
    normalize(num,l);
    return Rational{num,l};
}

Rational operator* (const Rational& x , const Rational& y){
    long int a = x.get_numerator(); long int b = x.get_denominator();   //x = a/b
    long int c = y.get_numerator(); long int d = y.get_denominator();   //y = c/d

    // a/b * c/d = a * c / b * d
    long int num = a * c;
    long int den = b * d;

    normalize(num,den);
    return Rational{num, den};
}

Rational operator/ (const Rational& x , const Rational& y){
    if(y.get_numerator() == 0) error("Division by zero rational");
    long int a = x.get_numerator(); long int b = x.get_denominator();   //x = a/b
    long int c = y.get_numerator(); long int d = y.get_denominator();   //y = c/d

    // a/b / c/d = a*d/ b*c
    long int num = a * d;
    long int den = b * c;

    normalize(num,den);
    return Rational{num,den};
}

bool operator== (const Rational& x , const Rational& y){
    long int a = x.get_numerator(); long int b = x.get_denominator();   //x = a/b
    long int c = y.get_numerator(); long int d = y.get_denominator();   //y = c/d
    normalize(a,b);
    normalize(c,d);

    //compares the two rationals in their simplest forms
    return (a == c) && (b == d);
}

bool operator!= (const Rational& x , const Rational& y){
    return !(x==y);
}

//convert rational to double by division
double convert_to_double(const Rational& x){
    return static_cast<double>(x.get_numerator()) / static_cast<double>(x.get_denominator());
}


int main(){
    try{
         // --- Construction & normalization ---
        Rational a{1,2};
        Rational b{2,4};
        Rational c{5,-10};
        Rational d{-6,-8};
        Rational e{3};

        cout << "a = " << a << '\n';   // 1/2
        cout << "b = " << b << '\n';   // 2/4
        cout << "c = " << c << '\n';   // -5/10
        cout << "d = " << d << '\n';   // 6/8
        cout << "e = " << e << '\n';   // 3

        // --- Equality / inequality ---
        cout << "a == b ? " << (a == b) << '\n'; // true
        cout << "a != c ? " << (a != c) << '\n'; // true

        // --- Addition / Subtraction ---
        Rational s1 = Rational{1,3} + Rational{1,6};
        cout << "1/3 + 1/6 = " << s1 << '\n';           // 1/2

        Rational s2 = Rational{5,8} + Rational{4,9};
        cout << "5/8 + 4/9 = " << s2 << '\n';           // 77/72

        Rational s3 = Rational{3,4} - Rational{1,2};
        cout << "3/4 - 1/2 = " << s3 << '\n';           // 1/4

        Rational s4 = Rational{5,-2} + Rational{-5,7};
        cout << "5/-2 + -5/7 = " << s4 << '\n';         // -45/14

        // --- Multiplication / Division ---
        Rational m1 = Rational{2,3} * Rational{3,4};
        cout << "2/3 * 3/4 = " << m1 << '\n';           // 1/2

        Rational m2 = Rational{-2,5} * Rational{-5,7};
        cout << "-2/5 * -5/7 = " << m2 << '\n';         // 2/7

        Rational q1 = Rational{5,6} / Rational{5,12};
        cout << "5/6 / 5/12 = " << q1 << '\n';          // 2

        Rational q2 = Rational{-3,8} / Rational{9,16};
        cout << "-3/8 / 9/16 = " << q2 << '\n';

        // --- Conversion to double ---
        Rational quarter{1,4};
        cout << "1/4 as double = " << convert_to_double(quarter) << '\n';   //0.25

        // --- Mutation helpers ---
        Rational mu{4,6};
        mu.change_denominator(-9);                                          // -4/9
        cout << "After changing denominator of 4/6 to -9: " << mu << '\n';

        // --- Divide by zero rational should throw ---
        cout << "Trying division by zero rational...\n";
        Rational boom = Rational{1,2} / Rational{0,5};
        cout << boom << '\n';  // shouldn’t reach here
    
    }catch(exception& e){
        cerr << e.what() << '\n';
    }

}