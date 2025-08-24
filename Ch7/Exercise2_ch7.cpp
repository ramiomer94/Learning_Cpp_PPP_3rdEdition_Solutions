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


int main(){
    try{
        // just an example
        vector<int> list = {1,2,3,4,5,6,7,8,9};
        print("Just a list of integers: ", list);
        
    }catch(exception& e){
        cerr << e.what() << '\n';
    }
}
