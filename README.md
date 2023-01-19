# VeryBigInteger
Big integer implementation based on `std::bitset`. Just include "verybiginteger.h" to your project.

# How to use?

Create `VeryBigInteger` instance with the following code:
  
    VeryBigInteger<100> A(10);
    
Then create the second object and sum them:

    VeryBigInteger<100> B(20);
    VeryBigInteger<100> C = operator+<100>(A,B);
    C.Print();
    
All other operations are similar.

# Example with CMake

1. `mkdir build-dir`
2. `cd build-dir`
3. `cmake ..`
4. `make`
