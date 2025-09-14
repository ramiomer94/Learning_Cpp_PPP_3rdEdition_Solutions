#include "PPPheaders.h"


// checks if a string contains only alphabetic characters (optional not a requirement in the exercise)
bool is_name(const string& n){
    for (char ch : n){
        if(!isalpha(ch)) return false; // return false if any non-alphabetic character found
    }
    return true;
}



//--------------------------------------------------------------------------------------------------------------------------------

// class to hold pairs of (name, age) values
// implements reading names, reading ages, printing, and sorting
class Name_pairs{
    public:
            void read_names();      // read a list of names from user input
            void read_ages();       // read an age for each name
            void sort();            // sort names alphabetically and reorder ages to match  

            // Accessors (to expose private data for operators)
            const vector<string>& names_list() const {return name;}
            const vector<double>& ages_list () const {return age;}
            int names_list_size() const {return name.size();}
            int ages_list_size() const {return age.size();}    
    private:
            vector<string> name;    // stores list of names
            vector<double> age;     // stores corresponding ages

};

//--------------------------------------------------------------------------------------------------------------------------------

// reads a list of names from input until the user enters "quit"
// only accepts alphabetic names (checked with is_name())
void Name_pairs::read_names(){

    cout << "Please enter a list of names. When done enter 'quit': \n";
    string input_name;
    while(cin >> input_name){
        if(!is_name(input_name)){ // Validate: must be alphabetic
             cout << "Please enter a valid name.\n";
             continue;
        }
        if(input_name == "quit") return;
        name.push_back(input_name); // Store valid name
    }

}

//--------------------------------------------------------------------------------------------------------------------------------

// reads an age for each name already stored in the vector
// prompts user to input an age corresponding to each name
void Name_pairs::read_ages(){
    cout << "Please enter the age for each name you see on the screen.\n";
    double input_age = 0;
    
    if(name.empty()) { // If no names were entered, display a message stating that and do nothing after
            cout << "No names to display.\n";
            return;
    }

   for(string n : name){
        cout << n << ':';   // Prompt for age of the current name
        cin >> input_age;
        if (!cin) error("invalid age value. double expected"); // error if input is not a double
        age.push_back(input_age);
        
    }
    
}

//--------------------------------------------------------------------------------------------------------------------------------

// sorts the names alphabetically and reorders the ages so that each age still corresponds to the correct person
void Name_pairs::sort(){

    vector<string> name_before_sorting = name; // save the original order of names
    vector<double> ages_after_sorting;

    std::sort(name.begin(), name.end()); // sort the names alphabetically

    for(int i = 0; i < age.size(); ++i){
        for(int j = 0; j < age.size(); ++j){
            if(name[i] == name_before_sorting[j]){      // Find matching name in original order
                ages_after_sorting.push_back(age[j]);   // Assign corresponding age
                break;
            }
        }
    }
    age = ages_after_sorting;      // Replace with reordered ages
}

//--------------------------------------------------------------------------------------------------------------------------------

// Operator overload: <<
// Prints Name_pairs in the form (name, age) per line
// Validates that both vectors are of equal length before printing
ostream& operator<< (ostream& os, const Name_pairs& np){
    if(np.names_list_size() != np.ages_list_size()) 
        error("non-matching lists. every name entry needs a corresponding age entry.");
    for(int i = 0; i < np.names_list_size(); ++i){
        os << '(' << np.names_list()[i] << ", " << np.ages_list()[i] << ")\n";
    }
    return os;
}

//--------------------------------------------------------------------------------------------------------------------------------

// Operator overload: ==
// Compares two Name_pairs objects for equality by checking vector sizes
// and ensuring all names and ages match in order
bool operator== (const Name_pairs& np1, const Name_pairs& np2){

    if(np1.ages_list_size() != np2.ages_list_size() || np1.names_list_size() != np2.names_list_size())
        return false;
    for(int i = 0; i < np1.names_list_size(); ++i){
        if(np1.names_list()[i] != np2.names_list()[i] || np1.ages_list()[i] != np2.ages_list()[i])
            return false;
    }
    return true;
}

//--------------------------------------------------------------------------------------------------------------------------------

// Operator overload: !=
// Simply defined as the negation of ==
bool operator!= (const Name_pairs& np1, const Name_pairs& np2){
    return !(np1 == np2);
}

// Test the Name_pairs class
int main(){

    try{
        Name_pairs np1;
        np1.read_names();
        np1.read_ages();
        cout << np1;
        Name_pairs np2;
        np2.read_names();
        np2.read_ages();
        cout << np2;

        if(np1 == np2) cout << "same list";
        if(np1 != np2) cout << "different list";

        np1.sort();
        np2.sort();
        cout << np1;
        cout << np2;

        if(np1 == np2) cout << "same list";
        if(np1 != np2) cout << "different list";
        
        return 0;
    } catch (exception& e){
        cerr << e.what() << '\n';
    }
}

