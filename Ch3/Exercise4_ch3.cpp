#include "PPPheaders.h"

// Ask the user for a sequence of values that represent the distance between two neighboring cities along a given route
/* 
Then calculate the sum of all distances, the smallest amnd largest distances between two neighboring cities and the
average distance travelled
*/
int main()
{
    vector <double> distances; 

    for (double distance; cin >> distance;)
        distances.push_back(distance);
    ranges::sort(distances); // sort the list of distances

    double sum = 0.0;

    for (double x : distances)
        sum += x; // The sum of all distances
    
    cout << "The sum of all distances: " << sum << '\n';
    cout << "The smallest distance between two neighboring cities: " << distances[0] << '\n';
    cout << "The largest distance between two neighboring cities: " << distances [distances.size() - 1] << '\n';
    cout << "The mean distance between two neighboring cities: " << sum / distances.size() << '\n';

}