#include "PPPheaders.h"

/*
 * Generates a Fibonacci-like sequence and stores it in a vector.
 * x: First starting number of the sequence.
 * y: Second starting number of the sequence.
 * v: Vector where the generated sequence will be stored.
 * n: Total number of elements to generate.
 */
void fibonacci(int x, int y, vector<int>& v, int n){

     // Clear vector if it already contains data
    if(!v.empty()) v.clear();

    // Validate that 'n' (the size of the Fibonacci list) is a positive integer
    if(n <= 0) error (to_string(n), " expected positive int");

    
    v.push_back(x); // Push the first starting number into the sequence
    if(n==1) return; // If only one number is requested, stop here

    // Push the second starting number into the sequence
    v.push_back(y);

    // each term is the sum of the two preceding ones
    for(int i = 2; i < n; ++i){
        v.push_back(v[i-2]+v[i-1]);
    }
}

/*
 * Calculates the largest Fibonacci-like number that can be stored in an int.
 * 
 * Starts with x = 1 and y = 2, generating short Fibonacci sequences (n = 3) 
 * and repeatedly advancing to the next two numbers until adding them would 
 * cause integer overflow.
*/

int main(){
    try{
       int x = 1;
       int y = 2;
       int n = 3;
       vector<int> list; // Holds the generated Fibonacci sequence
       int last_element = 0.0; // Tracks the last valid Fibonacci number before overflow

       while(true){
            // Check for potential overflow before calculating the next term:
            // If y > INT_MAX - x, then x + y would overflow.
            if (y > INT_MAX - x) break;
            fibonacci(x,y,list,n);

            // Update x and y to the last two terms of the sequence
            x = list[list.size()-2];
            y = list[list.size()-1];

            // Store the last valid number before overflow
            last_element = y;    
       }

       // Output the system's maximum int value and the approximation
       cout << "The maximum number 'int' type can hold is: " << INT_MAX
            << "\nThe approximation using fibonacci is: "<< last_element << '\n';
       
    }catch(exception& e){
        cerr << e.what() << '\n';
    }
}
