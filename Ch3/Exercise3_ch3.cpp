#include "PPPheaders.h"

// compute mean and median temperatures  
int main()   
{
    vector<double> temps; // temperatures
    for (double temp; cin>>temp;) // read into temp
        temps.push_back(temp); // put temp into vector

    // compute mean temperature: 
    double sum = 0;
    for (double x : temps)
        sum += x;
    cout << "Average temperature: " << sum/temps.size() << '\n';

// compute median temperature:
    ranges::sort(temps); // sort the temperatures 
    
    if (temps.size() % 2 != 0) //odd number of elements
        cout << "Median temperature: " << temps[temps.size()/2] << '\n';
    else // even number of elements
        cout << "Median temperature: " << (temps[temps.size()/2 - 1] + temps[temps.size()/2])/ 2.0 << '\n';
    
}