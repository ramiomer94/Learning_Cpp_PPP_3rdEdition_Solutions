#include "PPPheaders.h"

// Function to compute the weighted sum of price[i] * weight[i]
double weighted_sum(const vector<double>& price, const vector<double>& weight){

    // Ensure that both vectors have the same size
    if(price.size() != weight.size()) error("equal size vectors expected");

    double sum = 0;
    for(int i = 0; i < price.size(); ++i)
        sum += (price[i] * weight[i]);
    
    return sum; // Return the final computed weighted sum
}

int main(){

    //just an example
    try{
        vector<double> prices = {10, 20, 20.5, 30, 0, 100.234, 15.2};
        vector<double> weight = {0.4, 1.0, 1.5, 2.7, 10, 5.2, 15.03};
        double index = weighted_sum(prices, weight);
        cout << "the sum of all price[i]*weight[i] is: " << index << '\n';
    } catch(exception& e){
        cerr << e.what() << '\n';
    }
}