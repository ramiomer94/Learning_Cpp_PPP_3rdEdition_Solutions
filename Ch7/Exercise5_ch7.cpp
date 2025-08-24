#include "PPPheaders.h"

//pass-by-value function
/*
 * Function: reverse
 * -----------------
 * Reverses the elements of a vector<int> and returns a new reversed vector.
 * 
 * Parameters:
 *  - v: vector<int> passed by value (a copy is made).
 * 
 * Returns:
 *  - A new vector<int> containing the elements of v in reverse order.
 * 
 * Notes:
 *  - Because v is passed by value, changes do NOT affect the caller’s vector.
 */
vector<int> reverse (vector<int> v){
    if(v.empty()) error("expected size array of ints > 0");

    int end = v.size()-1;
    for(int start = 0;  start < v.size()/2; ++start)
        swap(v[start], v[end-start]);
    
    return v;
}

//pass-by-reference function
/*
 * Function: reverse_by_reference
 * ------------------------------
 * Reverses the elements of a vector<int> in place.
 * 
 * Parameters:
 *  - v: vector<int>& passed by reference (no copy is made).
 * 
 * Returns:
 *  - void (modifies v directly).
 * 
 * Notes:
 *  - Because v is passed by reference, changes persist in the caller’s vector.
 */
void reverse_by_reference (vector<int>& v){
    if(v.empty()) error("expected size array of ints > 0");

    int end = v.size()-1;
    for(int start = 0; start < v.size()/2; ++start)
        swap(v[start], v[end-start]);

}

/*
 * Function: print
 * ---------------
 * Prints the elements of a vector<int> with a label.
 * 
 * Parameters:
 *  - label: string to display before the vector.
 *  - v: const vector<int>& to print (read-only).
 */
void print(const string& label, const vector<int>& v){

    cout << label << ' ';
    for(const int& x: v)
        cout << x << ' ';
    
    cout << '\n';
}



int main(){

    // Test 1: reverse (pass-by-value)
    cout << "Testing reverse function-Argument passed by value: \n";
    vector<int> v1 = {1,2,3,4,5,6,7,8};
    vector<int> v2 = {1,2,3,4,5,6,7,8,9};

    print("v1: ", v1);
    vector<int> v1_reversed = reverse(v1);
    print("v1_reversed", v1_reversed);

    print("v2: ", v2);
    vector<int> v2_reversed = reverse(v2);
    print("v2_reversed: ", v2_reversed);

    cout << "//----------------------------------------------------------------------------------------------------------\n";

    // Test 2: reverse_by_reference (pass-by-reference)
    cout << "Testing reverse_by_reference function-Argument passed by reference: \n";
    vector<int> v3 = {1,2,3,4,5,6,7,8};
    vector<int> v4 = {1,2,3,4,5,6,7,8,9};

    print("v3: ", v3);
    reverse_by_reference(v3);
    print("After reversing v3: ", v3);

 
    print("v4: ", v4);
    reverse_by_reference(v4);
    print("After reversing v4: ", v4);

}