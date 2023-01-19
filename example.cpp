//
//  example.cpp
//  VeryBigInteger
//
//  Created by Roman Podymov on 19/01/2023.
//  Copyright © 2023 VeryBigInteger. All rights reserved.
//

#include "verybiginteger.h"

int main() {
    VeryBigInteger<100> A(10);
    VeryBigInteger<100> B(20);
    VeryBigInteger<100> C = operator+<100>(A,B);
    C.Print();
    
    return 0;
}
