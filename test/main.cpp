#include <iostream>
#include "../src/coc.hpp"

int main() {
    auto myStruct = coc::Open("my_struct.coc");
    if (!myStruct) return EXIT_FAILURE;

    auto foo = myStruct->Int["Foo"];
    if (foo) std::cout << "Foo: " << *foo << '\n';

    auto bar = myStruct->Struct["Bar"];
    if (bar) {
        auto baz = bar->String["Baz"];
        if (baz) std::cout << "Bar.Baz: " << *baz << '\n';
    }

    auto math = coc::Open("math.coc");
    if (!math) return EXIT_FAILURE;

    auto fibonacci = math->Array.Int["Fibonacci"];
    if (fibonacci) {
        for (int &num : *fibonacci) {
            std::cout << num << ", ";
        }
    }

    std::cin.get();
    return 0;
}