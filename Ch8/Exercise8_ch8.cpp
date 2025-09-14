#include "PPPheaders.h"

//------------------------------------ Utility validation functions ------------------------------------//

// Validate an author's name:
// - Non-empty
// - First and last chars must be alphabetic
// - Allowed: alphabetic, '-', '\'', ' '
// - No consecutive non-alphabetic characters
bool is_name(const string& n) {
    if (n.empty()) return false;

    // First and last must be alphabetic
    if (!isalpha(static_cast<unsigned char>(n.front())) || !isalpha(static_cast<unsigned char>(n.back()))) return false;

    for (int i = 0; i < n.size(); ++i) {
        unsigned char ch = n[i];

        // Must be alphabetic, '-', '\'', or space
        if (!isalpha(ch) && ch != '-' && ch != '\'' && ch != ' ')
            return false;

        // Reject consecutive non-alphabetic characters
        if (i > 0) {
            unsigned char prev = n[i-1];
            if (!isalpha(prev) && !isalpha(ch)) return false;
        }
    }

    return true;
}

// Return true if s consists only of digits
bool all_digits(const string& s){
    for(unsigned char ch : s)
        if(!isdigit(ch)) return false;
    return true;
}

// A valid card number must be exactly 9 digits
bool is_cardnumber(const string& n){
    if(n.size()!= 9) return false;
    if(!all_digits(n)) return false;
    return true;
}

//---------------------------------------------------------------------------------------------------------------------------------

// Represents a library user (first + last name)
class User{
    public:
            User(string fname, string lname) : first_name{fname}, last_name{lname}{
                if(!is_name(fname) || !is_name(lname)) error("Invalid user's name");
            }
            const string& user_first_name() const {return first_name;}
            const string& user_last_name() const {return last_name;}
            void change_first_name(const string& n);
            void change_last_name(const string& n);
    private:
            string first_name;
            string last_name;
};

// Validate before assigning new first name
void User::change_first_name(const string& n) {
    if(!is_name(n)) error("Invalid first name. Can't change user's firt name");
    first_name = n;
}

// Validate before assigning new last name
void User::change_last_name(const string& n) {
    if(!is_name(n)) error("Invalid last name. Can't change user's last name");
    last_name = n;
}

//---------------------------------------------------------------------------------------------------------------------------------

// Represents a library patron (user + card number + fees)
class Patron{
    public:
            Patron(const User& usern, string cardn);
            Patron(const User& usern, string cardn, double fee_owed);

            const string& get_cardnumber() const {return card_number;}
            const string& get_user_first_name() const {return user_name.user_first_name();}
            const string& get_user_last_name() const {return user_name.user_last_name();}
            double get_fee_owed() const {return fee;}
            bool is_fee_owed() const { return fee > 0.0;}

            void charge_fee(double amount){fee = amount;} // overwrite fee balance
            
    private:
            User user_name;
            string card_number = "000000000";
            double fee = 0; // default: no fee

};

// valid user + valid 9-digit card, fee defaults to 0
Patron::Patron(const User& usern, string cardn) : user_name{usern}, card_number{cardn} {
    if(!is_cardnumber(cardn)) error("Invalid card number. Expected a 9-digit char array");
}

// same as above, but with explicit fee
Patron::Patron(const User& usern, string cardn, double fee_owed) : user_name{usern}, card_number{cardn}, fee{fee_owed} {
    if(fee < 0) error("Invalid fee charged. Expected 0 or > 0 double.");
    if(!is_cardnumber(cardn)) error("Invalid card number. Expected a 9-digit char array");
}

//---------------------------------------------------------------------------------------------------------------------------------

// test tracker
int passed = 0, failed = 0;
void check(const char* label, bool ok) {
    if (ok) { cout << "[PASS] " << label << "\n"; ++passed; }
    else    { cout << "[FAIL] " << label << "\n"; ++failed; }
}

int main() {
    // ---- is_name() basic positives ----
    check("is_name: Alice",            is_name("Alice"));
    check("is_name: Mary-Jane",        is_name("Mary-Jane"));
    check("is_name: O'Neil",           is_name("O'Neil"));
    check("is_name: John Doe",         is_name("John Doe"));

    // ---- is_name() basic negatives ----
    check("is_name: empty -> false",   !is_name(""));
    check("is_name: starts '-' -> false", !is_name("-John"));
    check("is_name: ends '-' -> false",   !is_name("John-"));
    check("is_name: double space -> false", !is_name("John  Doe"));

    // ---- is_cardnumber() ----
    check("card: 9 digits OK",         is_cardnumber("012345678"));
    check("card: too short",          !is_cardnumber("12345678"));
    check("card: too long",           !is_cardnumber("0123456789"));
    check("card: has letter",         !is_cardnumber("12345a789"));

    // ---- User construction (OK) ----
    {
        User u{"Alice","Smith"};
        check("User ctor ok", u.user_first_name()=="Alice" && u.user_last_name()=="Smith");
    }

    // ---- User construction (should throw) ----
    {
        bool threw = false;
        try { User u{"-Alice","Smith"}; }
        catch (exception& e) { threw = true; }
        check("User ctor: bad first name throws", threw);
    }
    {
        bool threw = false;
        try { User u{"Alice","Smith-"}; }
        catch (exception& e) { threw = true; }
        check("User ctor: bad last name throws", threw);
    }

    // ---- User setters ----
    {
        User u{"Alice","Smith"};
        u.change_first_name("Mary-Jane");
        check("User change_first_name ok", u.user_first_name()=="Mary-Jane");
    }
    {
        bool threw = false;
        try {
            User u{"Alice","Smith"};
            u.change_last_name("Doe-");
        } catch (exception& e) { threw = true; }
        check("User change_last_name bad -> throws", threw);
    }

    // ---- Patron construction ----
    {
        User u{"John","Doe"};
        Patron p{u, "000000123"};
        check("Patron ctor ok (no fee)",
              p.get_user_first_name()=="John" &&
              p.get_user_last_name()=="Doe" &&
              p.get_cardnumber()=="000000123" &&
              !p.is_fee_owed() && p.get_fee_owed()==0.0);
    }

    // ---- Patron bad card -> throws ----
    {
        bool threw = false;
        try { User u{"John","Doe"}; Patron p{u, "123"}; }
        catch (exception& e) { threw = true; }
        check("Patron ctor bad card throws", threw);
    }

    // ---- Patron fee cases ----
    {
        User u{"Ana","Lopez"};
        Patron p{u, "123456789", 5.5};
        check("Patron ctor with fee", p.is_fee_owed() && p.get_fee_owed()==5.5);
    }
    {
        bool threw = false;
        try { User u{"Ana","Lopez"}; Patron p{u, "123456789", -1.0}; }
        catch (exception& e) { threw = true; }
        check("Patron ctor negative fee throws", threw);
    }
    {
        User u{"Sam","Hill"};
        Patron p{u, "987654321"};
        p.charge_fee(12.75);
        check("Patron charge_fee updates", p.is_fee_owed() && p.get_fee_owed()==12.75);
    }

    // ---- Copy independence (Patron holds its own User copy) ----
    {
        User u{"Mia","Gray"};
        Patron p{u, "111222333"};
        u.change_first_name("Changed");
        check("Patron holds copy of User", p.get_user_first_name()=="Mia");
    }

    // ---- Temporary User (works with const& params) ----
    {
        Patron p{ User{"Rami","Omer"}, "012345678" };
        check("Patron accepts temporary User",
              p.get_user_first_name()=="Rami" && p.get_cardnumber()=="012345678");
    }

    // ---- Summary ----
    cout << "\n=== SUMMARY ===\nPassed: " << passed << "\nFailed: " << failed << "\n";
    return failed ? 1 : 0;
}