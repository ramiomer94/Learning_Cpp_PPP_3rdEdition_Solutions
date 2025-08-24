#include "PPPheaders.h"


/*
 * Prints a vector of integers with a label.
 * label: A string that will be printed before the list.
 * v: The vector of integers to be printed.
 */
void print(const string& label, const vector<int>& v){

    cout << label << '\n';
    for(const int& x: v)
        cout << x << ' ';
    
    cout << '\n';

}

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

int main(){
    try{
        // just an example
        int x = 3;
        int y = 6;
        int n = 10;
        vector<int> list;
        string label = "fibonacci list:\n ";

        fibonacci(x,y,list,n); 

        print(label, list);
        
    }catch(exception& e){
        cerr << e.what() << '\n';
    }
}
