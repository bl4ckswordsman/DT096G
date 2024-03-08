/** @note
* This code has not been graded/approved like main.cpp has.
* It gives results as double even when both base and exponent are integers.
**/

#include <iostream>
#include <type_traits>

// Template struct to calculate power at compile time
template<int Exponent, typename Enable = void>
struct power;

// Partial specialization for Exponent = 0
template<>
struct power<0> {
    // Function to calculate the power
    template<typename BaseType>
    static constexpr BaseType calculatePower(BaseType base) {
        return 1;  // Base case: if exponent is 0, return 1
    }
};

// Partial specialization for non-negative exponents
template<int Exponent>
struct power<Exponent, std::enable_if_t<(Exponent > 0)>> {
    // Function to calculate the power
    template<typename BaseType>
    static constexpr BaseType calculatePower(BaseType base) {
        // If exponent is positive, recursively multiply base
        return base * power<Exponent - 1>::calculatePower(base);
    }
};

// Partial specialization for negative exponents
template<int Exponent>
struct power<Exponent, std::enable_if_t<(Exponent < 0)>> {
    // Function to calculate the power
    static constexpr double calculatePower(double base) {
        // If exponent is negative, calculate the reciprocal of the power with positive exponent
        return 1 / power<-Exponent>::calculatePower(base);
    }
};

int main() {
    constexpr auto static base = 2;
    constexpr auto static exponent = -1;
    constexpr auto static result = power<exponent>::calculatePower(base);

    std::cout << "Calculating " << base << " raised to the power of " << exponent << " = " << result << std::endl;

    return 0;
}