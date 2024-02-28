#include <iostream>
#include <type_traits>

// Template struct to calculate power at compile time
template<int Exponent, typename Enable = void>
struct power;

// Partial specialization for non-negative exponents
template<int Exponent>
struct power<Exponent, std::enable_if_t<(Exponent >= 0)>> {
    // Function to calculate the power
    template<typename BaseType>
    static constexpr BaseType calculatePower(BaseType base) {
        if constexpr (Exponent == 0) {      // Base case: if exponent is 0, return 1
            return 1;
        } else {       // If exponent is positive, recursively multiply base
            return base * power<Exponent - 1>::calculatePower(base);
        }
    }
};

// Partial specialization for negative exponents
template<int Exponent>
struct power<Exponent, std::enable_if_t<(Exponent < 0)>> {
    // Function to calculate the power
    template<typename BaseType>
    static constexpr BaseType calculatePower(BaseType base) {        // If exponent is negative, recursively divide 1 by base
        return 1 / base * power<Exponent + 1>::calculatePower(base);
    }
};

int main() {
    constexpr auto static base = 2.0;
    constexpr auto static exponent = 3;
    constexpr auto static  result = power<exponent>::calculatePower(base);

    std::cout << "Calculating " << base << " raised to the power of " << exponent << " = " << result << std::endl;

    return 0;
}