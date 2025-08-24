#include "PPPheaders.h"

// This function takes a vector of integers (day_values) and a string (day)
// It prints out the values and computes their sum for a specific day.
void sum_day_values (vector<int> day_values, string day){

    // If the vector is empty, warn the user
    if(day_values.size() == 0) cerr << "No values found in " << day << "'s vector\n\n";
    else {
        // Double check that the 'day' is valid
        if(day != "Monday" && day != "Tuesday" && day != "Wednesday" && day != "Thursday"
          && day != "Friday" && day != "Saturday" && day != "Sunday")
            error("Expected a string day of the week of the format (ex. \"Thursdya\")\n");
    
        int sum = 0;

        cout << day << ": ";
        for(int x: day_values){ // Print each value and compute the sum
            cout << x << " ";
            sum += x;
        }
        cout << " ---> The sum of all the values associated with " << day << " is: " << sum << '\n';
    }
}

int main(){
    cout << "Enter a list of (day of the week, value) pairs-Example: Tuesday 24 mon -10 .... : ";
    int value;
    string day;
    // Separate vectors to hold values for each valid day of the week
    vector<int> monday_values, tuesday_values, wednesday_values, thursday_values, friday_values,saturday_values, sunday_values;
    int rejected_values_counter = 0; // Counter to track unrecognized day inputs

    // Read input pairs continuously until EOF (Ctrl+D/Ctrl+Z) or invalid input
    while(cin >> day >> value){
        // Error if input is corrupted or mismatched
        if(!cin) error ("Expected int for first input and a char* array for the second input.\n");

        // Match input day string to correct day vector, accounting for common synonyms
        if(day == "Monday" || day == "monday" || day == "Mon" || day == "mon")
            monday_values.push_back(value);
        else if(day == "Tuesday" || day == "tuesday" || day == "Tue" || day == "tue" || day == "Tues" || day == "tues")
            tuesday_values.push_back(value);
        else if(day == "Wednesday" || day == "wednesday" || day == "Wed" || day == "wed")
            wednesday_values.push_back(value);
        else if(day == "Thursday" || day == "thursday" || day == "Thu" || day == "thu" || day == "Thurs" || day == "thurs")
            thursday_values.push_back(value);
        else if(day == "Friday" || day == "friday" || day == "Fri" || day == "fri")
            friday_values.push_back(value);
        else if(day == "Saturday" || day == "saturday" || day == "Sat" || day == "sat")
            saturday_values.push_back(value);
        else if(day == "Sunday" || day == "sunday" || day == "Sun" || day == "sun")
            sunday_values.push_back(value);
        else
            ++rejected_values_counter; // Count invalid or misspelled day entries
          
    }

    // Output results for all valid day entries
    cout << "\nThe values associated with each day-of-the-week:\n\n";
    sum_day_values(monday_values, "Monday");
    sum_day_values(tuesday_values, "Tuesday");
    sum_day_values(wednesday_values, "Wednesday");
    sum_day_values(thursday_values, "Thursday");
    sum_day_values(friday_values, "Friday");
    sum_day_values(saturday_values, "Saturday");
    sum_day_values(sunday_values, "Sunday");

    // Show how many values were rejected due to invalid day names
    cout << "\nThe number of rejected values is: " << rejected_values_counter << '\n';
    
}