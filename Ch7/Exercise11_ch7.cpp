#include "PPPheaders.h"

/*
    I prefer the struct method because it:
	    •	Groups related results in one object (better organization)
	    •	Makes the function signature cleaner (fewer parameters)
	    •	Is easier to maintain if more statistics are added later

*/

// Struct to hold statistical results
struct Stats{
    double smallest; // Smallest value in the vector
    double largest;  // Largest value in the vector
    double mean;     // Average value of the vector
    double median;   // Median value of the vector
};

// Function to calculate smallest, largest, mean, and median of a vector
// Takes the vector by value so we can sort it to calculate the median without modifying the caller's data
Stats calculate_stats(vector<double> v){
    if(v.size() == 0) error("non-empty vector expected");

    Stats result; // Struct to store the results

    // --- Find smallest value ---
    double min_val = v[0];
    for(const double& element : v)
        if(element < min_val)
            min_val = element;
    result.smallest = min_val;

    // --- Find largest value ---
    double max_val = v[0];
    for(const double& element : v)
        if(element > max_val)
            max_val = element; // update when we find a larger value
    result.largest = max_val;

    // --- Calculate mean (average) ---
    double sum = 0.0;
    for(const double& x : v)
        sum += x;
    result.mean = sum/v.size();

    // --- Calculate median ---
    // Sort the vector to get middle elements in order
    sort(v.begin(), v.end());
    // Odd number of elements: middle element is the median
    if(v.size()%2 != 0) result.median = v[v.size()/2];
    // Even number of elements: average the two middle elements
    else result.median = (v[v.size()/2] + v[v.size()/2 - 1])/2;

    return result;
}

int main(){
    vector<double> test_v = {12.5, -3.8, 45.2, 0.0, 7.7,
                             100.3, -25.4, 18.9, 3.14, 88.8,
                              42.0, -10.5, 56.6, 72.9, 5.5};
    
    // Calculate stats for the test vector                        
    Stats r = calculate_stats(test_v);

    cout << "The smallest element is: " << r.smallest
         << "\nThe largest element is: " << r.largest
         << "\nThe mean is: " << r.mean
         << "\nThe median is: " << r.median << '\n';
    
}