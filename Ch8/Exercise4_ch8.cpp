#include "PPPheaders.h"

// A single (name, age) record
class Name_pair{
    public:
            Name_pair (string nn) : n{nn}, a{0}{}
            Name_pair(double aa, string nn) : a{aa}, n{nn} {}
            double age() const {return a;}
            string name() const {return n;}
            void change_age(double aa) {a = aa;};

    private:
            double a = 0;   // age
            string n = "";  // name

};

//--------------------------------------------------------------------------------------------------------------------------------

// Lowercase a copy of a string
string to_lowercase(string n){
    for(char& ch : n)
        ch = tolower(static_cast<unsigned char>(ch));
    return n;
}

// True if a duplicate (case-insensitive) name exists in vector
bool is_duplicate(const vector<Name_pair>& v, string n){
    for(const Name_pair& np : v )
        if (to_lowercase(np.name()) == to_lowercase(n)) return true;
    return false;
}

// Validate that a "name" contains only letters or hyphens
bool is_name(string n){
    for(char ch : n)
        if (!isalpha(ch) && ch != '-') return false;

    return true;
}

//--------------------------------------------------------------------------------------------------------------------------------

// Holds a list of (name, age) and supports read/sort/print/compare
class Name_pairs{
    public:
            void read_names();      // read a list of names from user input
            void read_ages();       // read an age for each name
            void sort();            // sort by name (case-insensitive)

            // Accessors (to expose private data for operators)
            const vector<Name_pair>& names_ages_list() const {return name_age_pair;}   
    private:
            vector<Name_pair> name_age_pair;   

};

//--------------------------------------------------------------------------------------------------------------------------------

// Read names until the user types "stop" (case-insensitive).
// Rejects non-conforming names and duplicates.
void Name_pairs::read_names(){

    cout << "Please enter a list of names. When done enter 'stop': \n";
    string input_name;
    while(cin >> input_name){
        string lower = to_lowercase(input_name);
        if(lower == "stop") return;

        // Validate format; reprompt until a valid name is given or 'stop'
        while(!is_name(input_name)){ 
             cout << input_name << " is not a valid name. Use only letters and '-'.\n";
             cout << "Enter a valid name (or 'stop' to finish):\n";
             if(!(cin >> input_name)) return;
             if (to_lowercase(input_name) == "stop") return;
        }

        // Reject duplicates (case-insensitive)
        if(is_duplicate(names_ages_list(), input_name)) {
            cout << " is a duplicate. Only the first entry is kept; duplicates are discarded.\n";
            continue;
        }

        // Store valid name with default age 0 (to be filled later)
        Name_pair np{input_name};
        name_age_pair.push_back(np); // Store valid name
    }

}

//--------------------------------------------------------------------------------------------------------------------------------

// reads an age for each name already stored in the vector
// prompts user to input an age corresponding to each name
void Name_pairs::read_ages(){
    
    if(name_age_pair.empty()) { // If no names were entered, display a message stating that and do nothing after
            cout << "Sorry, no names in the list to read ages for!!\n";
            return;
    }

    cout << "Please enter the age for each name you see on the screen:\n";
    double input_age = 0;

   for(Name_pair& np : name_age_pair){
        cout << np.name() << ':';   // Prompt for age of the current name
        cin >> input_age;
        if (!cin) error("invalid age value. double expected"); // error if input is not a double
        
        // Handle negative input gracefully
        while(input_age < 0){
            cout << "Bad age. Expected positive double. Enter a valid age for " << np.name() << ":\n";
            cin >> input_age;
            if (!cin) error("invalid age value. double expected"); // error if input is not a double
        }
        np.change_age(input_age);
    }
    
}

//--------------------------------------------------------------------------------------------------------------------------------

// Define an order for Name_pair by name (case-insensitive).
bool operator< (const Name_pair& np1, const Name_pair& np2){
    return np1.name() < np2.name();
}

// sorts the names alphabetically and reorders the ages so that each age still corresponds to the correct person
void Name_pairs::sort(){
    if(name_age_pair.empty()) cout << "There is nothing to sort. List is empty\n";
    std::sort(name_age_pair.begin(), name_age_pair.end()); // sort the names alphabetically
    
}

//--------------------------------------------------------------------------------------------------------------------------------

// Operator overload: <<
// Prints Name_pairs in the form (name, age) per line
// Prints a friendly message if the list is empty.
ostream& operator<< (ostream& os, const Name_pairs& nps){
    const vector<Name_pair>& pairs = nps.names_ages_list();
    if(pairs.empty()) os << "There is no names in the list to print.\n";
    for(const Name_pair& np: pairs)
        os << '(' << np.name() << ',' << np.age() << ")\n";
    
    return os;
}

//--------------------------------------------------------------------------------------------------------------------------------

// Operator overload: ==
// Equality: lists must be the same size and each (name, age) must match at the same index.
// Name comparison is case-insensitive, consistent with duplicates/sort.
bool operator== (const Name_pairs& np1, const Name_pairs& np2){

    const vector<Name_pair>& pairs1 = np1.names_ages_list();
    const vector<Name_pair>& pairs2 = np2.names_ages_list();
    if(pairs1.size() != pairs2.size())
        return false;
    for(int i = 0; i < pairs1.size(); ++i){

        if(to_lowercase(pairs1[i].name()) != to_lowercase(pairs2[i].name()) || pairs1[i].age() != pairs2[i].age())
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

//--------------------------------------------------------------------------------------------------------------------------------

// Test the Name_pairs class
int main(){

    try{

        cout << "\n\n----------Producing The First Name_Age Pairs List----------\n\n";
        Name_pairs np1;
        np1.read_names();
        np1.read_ages();
        cout << np1;
        cout << "\n\n----------Producing The Second Name_Age Pairs List----------\n\n";
        Name_pairs np2;
        np2.read_names();
        np2.read_ages();
        cout << np2;

        cout << "\n\n----------Comparing The Two Lists (No Sorting)----------\n\n";
        if(np1 == np2) cout << "same list\n";
        if(np1 != np2) cout << "different list\n";

        cout << "\n\n----------Printing The Two Lists After Sorting----------\n\n";
        np1.sort();
        np2.sort();
        cout << np1;
        cout << np2;
        
        cout << "\n\n----------Comparing The Two Lists (After Sorting)----------\n\n";
        if(np1 == np2) cout << "same list\n";
        if(np1 != np2) cout << "different list\n";
        
        return 0;
    } catch (exception& e){
        cerr << e.what() << '\n';
    }
}

