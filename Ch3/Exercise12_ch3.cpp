#include "PPPheaders.h"


// Function declaration: checks whether a given number is prime
bool isPrime (int n); 

//program to find all the prime numbers between 1 and 100
int main(){

    // A predefined list of known prime numbers between 1 and 100 (for comparison)
    vector<int> primes = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 
                          47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97};
                        
    // Vector to store prime numbers we will find programmatically
    vector<int> myPrimes = {2}; // Start with 2, the first and only even prime number

    for(int i = 3; i <= 100; i+=2 ){ //Skip even numbers
        if(isPrime(i))
            myPrimes.push_back(i);  
    } 
    
    // Print the list of primes found by the program
    for (int prime : myPrimes)
        cout << prime << " ";
    cout << "\n\n";

    // Print the predefined list of known prime numbers for comparison
    for (int prime : primes)
        cout << prime << " ";
    cout << '\n';

}

// Function definition: checks if an integer n is prime
bool isPrime (int n){
    
    if (n <= 1) 
        return false;
    
    for(int i = 2; i < n; ++i)
        if(n % i == 0)
            return false;
    
    return true;
}