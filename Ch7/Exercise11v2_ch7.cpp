#include "PPPheaders.h"

/*
    I prefer the struct method because it:
	    •	Groups related results in one object (better organization)
	    •	Makes the function signature cleaner (fewer parameters)
	    •	Is easier to maintain if more statistics are added later

*/


// Function to calculate smallest, largest, mean, and median
// The results are returned through reference arguments
// The vector is passed by value so we can sort it without affecting the original
void cal_result(vector<double> v, double& smallest, double& largest, double& mean, double& median){

    if(v.size() == 0) error("non-empty vector expected");

    // --- Find the smallest element ---
    double min_val = v[0];
    for(const double& element : v)
        if(element < min_val)
            min_val = element; // Update if we find a smaller value
    smallest = min_val; // Store in reference parameter

    // --- Find the largest element ---
    double max_val = v[0];
    for(const double& element : v)
        if(element > max_val)
            max_val = element; // update when we find a larger value
    largest = max_val;

    // --- Calculate the mean (average) ---
    double sum = 0.0;
    for(const double& x : v)
        sum += x;
    mean = sum/v.size();

    // --- Calculate the median ---
    // Sort vector so that middle values are easy to find
    sort(v.begin(), v.end());
    // Odd number of elements → middle element is the median
    if(v.size()%2 != 0) median = v[v.size()/2];
    // Even number of elements → median is average of two middle elements
    else median = (v[v.size()/2] + v[v.size()/2 - 1])/2;
}

int main(){
    vector<double> test_v = {12.5, -3.8, 45.2, 0.0, 7.7,
                             100.3, -25.4, 18.9, 3.14, 88.8,
                              42.0, -10.5, 56.6, 72.9, 5.5};
    
    // Variables to store results
    double smallest = 0.0;
    double largest = 0.0;
    double mean = 0.0;
    double median = 0.0;
    
    cal_result(test_v, smallest, largest, mean, median);

    cout << "The smallest element is: " << smallest
         << "\nThe largest element is: " << largest
         << "\nThe mean is: " << mean
         << "\nThe median is: " << median << '\n';
    
}