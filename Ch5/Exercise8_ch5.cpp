#include "PPPheaders.h"

int factorial(int n);
int permutations(int a, int b);
int combinations(int a, int b);


int main(){

    try{

        while(true){

            cout << "Enter 'C' to calculate combinations or 'P' to calculate permutations or 'X' to quit (case-insensitive): ";

            char function;
            cin >> function;
            if(!cin) error("Expected char.");

            if(isupper(function)) function = tolower(function);
            if(function != 'c'  && function != 'p' && function != 'x') 
                error("Expected 'P, 'C', or 'X' (case insensitive)");

            if(function == 'x') return 0;

            cout << "Enter two non-negative integers 'a' and 'b' (where a ≥ b), separated by space: ";

            int a; int b;
            cin >> a >> b;
            if(!cin) error("Expected int.");

            int result;
            if(function == 'c'){
                result = combinations(a,b);
                cout << "C(" << a << "," << b << ") == " << result << '\n';
            }
            if(function == 'p'){
                result = permutations(a,b);
                cout << "P(" << a << "," << b << ") == " << result << '\n';
            }

        }

    }catch(exception& e){
        cerr << "Error: " << e.what() << '\n';
    }catch(...){
        cerr << "Error: unknown exception" << '\n';
    }

}

int factorial(int n){

    expect([&]{return n >= 0;}, "Factorial for a negative number is undefined.");

    int factorial = 1;
    if(n == 0) return factorial;

    for(int i = 2; i <= n; ++i)
        factorial *= i;
    
    expect([&]{return (factorial >= 1 && factorial <= INT_MAX);}, "Overflow detected: Factorial can only be a positive whole number.");

    return factorial;
}

int permutations(int a, int b){

    expect([&]{return (a >= b && b >= 0);},
         "Expected 'a' and 'b' in permutations P(a,b) non-negative integers. Expected a >= b");

    int numerator = factorial(a);

    int denominator = factorial(a-b);
    if (denominator == 0) error ("Division by zero is undefined");

    int permutations = numerator/denominator;
    expect([&]{return (permutations >= 1 && permutations <= numerator);},
           "Outside the range of permutations. Overflow detected: Range of permutation P(a,b) is [1, a!]");
    
    return permutations;

}

int combinations(int a, int b){

    expect([&]{return (a >= b && b >= 0);},
          "Expected 'a' and 'b' in permutations C(a,b) non-negative integers. Expected a >= b");
    
    int numerator = permutations(a,b);
    int denominator = factorial(b);
    if (denominator == 0) error ("Division by zero is undefined");

    int combinations = numerator/denominator;
    expect([&]{return  (combinations >= 1 && combinations <= static_cast<int>(pow(2,a)));},
         "Outside the range of combinations. Overflow detected: Range of combinations C(a,b) is [1, 2^a]");
    
    return combinations;
}