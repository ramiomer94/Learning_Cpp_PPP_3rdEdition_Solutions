#include "PPPheaders.h"

//pass-by-value function
/*
 * Function: reverse
 * -----------------
 * Reverses the order of elements in a vector<string>.
 * 
 * Parameters:
 *  - v: vector<string> passed by value (creates a copy of the original vector).
 * 
 * Returns:
 *  - A new vector<string> containing the elements of v in reverse order.
 * 
 * Notes:
 *  - The original vector in the calling scope is NOT changed because we pass by value.
 *  - Throws an error if the vector is empty.
 */
vector<string> reverse (vector<string> v){
    if(v.empty()) error("vector must be non-empty");

    int end = v.size()-1;
    for(int start = 0;  start < v.size()/2; ++start)
        swap(v[start], v[end-start]);
    
    return v; // Return reversed copy
}

//pass-by-reference function
/*
 * Function: reverse_by_reference
 * ------------------------------
 * Reverses the order of elements in a vector<string> directly in place.
 * 
 * Parameters:
 *  - v: vector<string>& passed by reference (no copy made).
 * 
 * Returns:
 *  - void (the input vector is modified directly).
 * 
 * Notes:
 *  - The original vector in the calling scope IS changed because we pass by reference.
 *  - Throws an error if the vector is empty.
 */
void reverse_by_reference (vector<string>& v){
    if(v.empty()) error("vector must be non-empty");

    int end = v.size()-1;
    for(int start = 0; start < v.size()/2; ++start)
        swap(v[start], v[end-start]);
}

/*
 * Function: print
 * ---------------
 * Prints the elements of a vector<string> with a descriptive label.
 * 
 * Parameters:
 *  - label: A string to display before the vector contents.
 *  - v: const vector<string>& (read-only vector of strings).
 * 
 * Output Example:
 *  fruits: apple grapes banana ...
 */
void print(const string& label, const vector<string>& v){

    cout << label << ' ';
    for(const string& word: v)
        cout << word << ' ';
    
    cout << '\n';
}


int main(){

    // Testing reverse() - pass-by-value (returns a reversed copy)
    cout << "Testing reverse function-Argument passed by value: \n";
    vector<string> v1 = {"apple", "grapes", "watermelon", "oranges", "bananas", "grape fruits", "pineapples", "tomato", "peach"};
    vector<string> v2 = {"cucumbers", "lettuce", "peppers", "onions", "spinach", "parsley", "mint", "cabbage"};

    print("v1: ", v1);
    vector<string> v1_reversed = reverse(v1); // Original remains unchanged
    print("v1_reversed", v1_reversed);

    print("v2: ", v2);
    vector<string> v2_reversed = reverse(v2);
    print("v2_reversed: ", v2_reversed);

    cout << "//----------------------------------------------------------------------------------------------------------";

    // Testing reverse_by_reference() - pass-by-reference (modifies in place)
    cout << "Testing reverse_by_reference function-Argument passed by reference: \n";
    vector<string> v3 = {"apple", "grapes", "watermelon", "oranges", "bananas", "grape fruits", "pineapples", "tomato", "peach"};
    vector<string> v4 = {"cucumbers", "lettuce", "peppers", "onions", "spinach", "parsley", "mint", "cabbage"};


    print("v3: ", v3);
    reverse_by_reference(v3); // Modifies v3 directly
    print("After reversing v3: ", v3);

 
    print("v4: ", v4);
    reverse_by_reference(v4);
    print("After reversing v4: ", v4);

}