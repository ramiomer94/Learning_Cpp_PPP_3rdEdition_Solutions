#include <iostream>
#include <stdexcept>

void divide(int a, int b) {
    if (b == 0)
        throw std::runtime_error("Can't divide by zero!");
    std::cout << "Result: " << a / b << "\n";
}

int main() {
    try {
        divide(10, 0);
        std::cout << "This line won't run if exception thrown.\n";
    } catch (std::runtime_error& e) {
        std::cerr << "Caught: " << e.what() << "\n";
    }
    std::cout << "Program continues after error is handled.\n";
    return 0;
}