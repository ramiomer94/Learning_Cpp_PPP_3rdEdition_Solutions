#include "PPPheaders.h"

class Name_value{
    public:
            string name;
            int value;
            Name_value(string n = "", int val = 0)
                        :name{n}, value{val}{}

};

int main(){

    try {
        cout << "Enter a set of name-and-value pairs, such as Joe 17 and Barbara 22. Terminate input with 'NoName 0': ";
    
        string name;
        int value;
        vector<Name_value> pairs;

        while(cin >> name >> value){

            if(name == "NoName" && value == 0) break;

            Name_value pair {name, value};
            if(pairs.size() == 0)
                pairs.push_back(pair);
            else {
                for(int i = 0; i < pairs.size(); ++i){
                    if(name == pairs[i].name){
                        cout << "Program terminated due to the use of duplicate names.\n";
                        return -1;
                    }
                }
                pairs.push_back(pair);
            }
            
        }

        if (!cin) error("Expected a string and an integer. Ex: John 22\n");
        if (pairs.size() == 0) error("Empty list.\n");

        for(Name_value x: pairs)
            cout << '(' << x.name << ", " << x.value << ")\n"; 


        int val;

        cout << "Enter a value you wish to retrieve the corresponding names for: ";
        while (cin >> val){
            bool found = false;

            cout << "The name(s) associated with the value: " << val << " is/are: ";
            for(int i = 0; i < pairs.size(); ++i) {
                if(val == pairs[i].value){
                    cout << pairs[i].name<< '\n';
                    found = true;
                }
            }

            if(!found) cout << "score not found.\n";
            cout << "Enter a value you wish to retrieve the corresponding names for: ";
        }

        error("Expected an int.\n");
   
    } catch(exception& e){
        cerr << "Error: " << e.what();
    }

}