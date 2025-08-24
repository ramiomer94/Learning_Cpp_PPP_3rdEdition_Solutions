#include "PPPheaders.h"


// Function declaration: checks whether a number is prime
bool isPrime(int x);

int main(){
    cout << "Please enter the number of the first n prime numbers you would like me to display. n: ";
    int n;
    
    while (cin >> n) {

        if (n > 0) { // Check for valid input (positive number only)

            vector<int> primes; 
            int n_count = 0; // Counter for how many primes have been found

            const int upper_limit = n * (log(n) * log(n)); // Estimate upper limit using approximation n * (log(n))^2
            int adjust_limit = 1; // Factor to increase the range if not enough primes are found

            while(n_count < n){ // Loop until we've found n prime numbers
                for(int i = 2; i <= (upper_limit * adjust_limit); ++i)
                    if(isPrime(i)){
                        primes.push_back(i);
                        ++n_count;
                        if(n_count == n) break; // Stop when we reach n primes
                    }
                ++adjust_limit; // Expand the range if not enough primes were found
            }

            for (int prime : primes)
                cout << prime << "  ";

            
            cout << "\n\nPlease enter the number of the first n prime numbers you would like me to display. n: ";
        } else {
            cout << "Invalid Input!! n must be positive integer bigger than zero." <<
            "\nPlease enter the number of the first n prime numbers you would like me to display. n: ";
            continue;
        }
    }
}


// Function to check whether a given number x is prime
bool isPrime (int x){
    if (x <= 1)
        return false; // 1 and lower are not prime
    else
        for(int i = 2; i * i <= x; ++i)
            if (x % i == 0)
                return false;

    return true;
}