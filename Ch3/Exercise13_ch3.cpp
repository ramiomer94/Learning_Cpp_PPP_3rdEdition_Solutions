#include "PPPheaders.h"


// Function to return the square of a number
int square (int x) {return x * x;}

// Finding the list of prime numbers between 1 and 100 using the “Sieve of Eratosthenes” method.
int main(){

    // Vector to mark if a number is prime ('P') or not ('N')
    vector<char> isPrime;

    for (int i = 0; i < 100; ++i)
        isPrime.push_back('P'); // Initially assume all numbers are prime
    
    isPrime[0] = 'N'; //1 is not a prime number

    // Apply Sieve of Eratosthenes logic
    for (int i = 1; square(i+1) <= 100; ++i) // Start from the first prime (2), and eliminate its multiples
        if (isPrime[i] == 'P')
            for (int j = square(i+1); j < 100; j += (i+1)) // Mark all multiples of (i+1) starting from its square as not prime
                isPrime[j] = 'N';
    
    // Output all numbers marked as prime
    for (int i = 2; i < 100; ++i)
        if (isPrime[i] == 'P')
            cout << i << " ";
    
    cout << '\n';

}