# C Object Configuration (COC) | C++ Version 0.1.0 (2024-10-22)
## Overview
This is version 0.1.0 of the C++ library which is made to be compatible with ***C Object Configuration I***. Please refer to [C-Object-Configuration](https://github.com/C-Object-Configuration) for documentation on actual ***C Object Configuration***.
## Getting Started
Using the library is fairly straightforward. In order to stay organized, this whole library is provided within one single namespace, ***coc***. The namespace has three major components, ***coc::Load***, ***coc::Open*** and ***coc::Structure***.
___
***coc::Load(std::string data):*** this function loads some ***C Object Configuration*** data.
```cpp
#include <coc.hpp>

int main() {
	// Passing a 'C Object Configuration' struct as an 'std::string'
	std::optional<coc::Structure> myStruct = coc::Load("{ int Foo = 42 }");
	return 0;
}
```
___
***coc::Open(std::string path):*** this function is a ***wrapper*** around ***coc::Load***, it opens an actual ***.coc*** file, reads its contents and proceeds to call ***coc::Load*** by itself.
```cpp
#include <coc.hpp>

int main() {
	// Passing a filepath to a 'C Object Configuration' struct,
	// relative to the current working directory
	std::optional<coc::Structure> myStruct = coc::Open("my_struct.coc");
	return 0;
}
```
___
The key similarity here are the functions return type, ***std::optional&lt;coc::Structure&gt;.*** The use of ***std::optional*** allows for easy data retrieval, and safety when your ***coc*** code is either invalid, or ***.coc*** file unable to be located.
___
Let's check if our data is valid or not. We use ***auto*** in place of ***std::optional&lt;coc::Structure&gt;*** for code readability. We check if our ***myStruct*** variable ***has*** a value or ***not***. If we fail to open our ***my_struct.coc*** file, we ***return EXIT_FAILURE***.
```cpp
#include <iostream>
#include <coc.hpp>

int main() {
	// Passing a filepath to a 'C Object Configuration' struct,
	// relative to the current working directory
	auto myStruct = coc::Open("my_struct.coc");

	if (!myStruct.has_value()) {
		std::cout << "myStruct is NOT valid\n";
		return EXIT_FAILURE;
	}
	std::cout << "myStruct is valid\n";

	return 0;
}
```
___
The next step would be to load some data from our struct. This is our current ***C Object Configuration*** struct, located within our ***my_struct.coc*** file.
```c
int Foo = 42
```
A ***coc::Structure*** has one ***member struct*** for each data type. So we would retrieve an ***integer*** using the ***Int*** member. Each one of these type members then has their own ***std::optional&lt;*** _type_ ***&gt; Get(std::string_view key)*** method, where the ***key*** is the identifier from your ***coc*** code, capital ***F*** in this instance.
```cpp
#include <iostream>
#include <coc.hpp>

int main() {
	// Passing a filepath to a 'C Object Configuration' struct,
	// relative to the current working directory
	auto myStruct = coc::Open("my_struct.coc");

	if (!myStruct.has_value()) {
		std::cout << "myStruct is NOT valid\n";
		return EXIT_FAILURE;
	}
	std::cout << "myStruct is valid\n";

	auto foo = myStruct.value().Int.Get("Foo");
	// If foo has a value, print it
	if (foo.has_value()) {
		int i = foo.value();
		std::cout << "Foo: " << i << '\n';
	}

	return 0;
}
```
We use ***value()*** to access the ***underlying*** variable of an ***std::optional***. We retrieve our ***Foo*** from ***myStruct***, and if it exists, we print the underlying value of ***foo***, again using ***value().***
___
***std::optional*** has an ***explicit boolean conversion***.
```cpp
// Both lines yield the same outcome
if (foo.has_value)
if (foo)
```
Both the ***->*** and ***\**** operators are overloaded, and can be used in place of ***value().***
```cpp
auto foo = myStruct->Int.Get("Foo");
int i = *foo;
```
The ***[]*** operator is overloaded by each ***type*** member. Combining all of these ***syntactic sugar workflows*** might look something like this.
```cpp
#include <iostream>
#include <coc.hpp>

int main() {
	auto myStruct = coc::Open("my_struct.coc");
	if (!myStruct) return EXIT_FAILURE;

	auto foo = myStruct->Int["Foo"];
	if (foo) std::cout << "Foo: " << *foo << '\n';

	return 0;
}
```
___
## Nested Structures
Accessing struct members within another struct is exactly the same as any other type, but let's go through it just for clarity.
___
Our expanded ***my_struct.coc*** file.
```c
int Foo = 42

struct Bar = {
	string Baz = "hello world"
}
```
___
Let's open and read the file. If ***myStruct*** doesn't exist, we exit our program. We try to load our ***Bar*** struct, if it does exist, we proceed to load ***Bar***'s string ***Baz***. And once again, if ***Baz*** does indeed exist, we print it to our console.
```cpp
#include <iostream>
#include <coc.hpp>

int main() {
	auto myStruct = coc::Open("my_struct.coc");
	if (!myStruct) return EXIT_FAILURE;

	auto bar = myStruct->Struct["Bar"];
	if (bar) {
		auto baz = bar->String["Baz"];
		if (baz) std::cout << "Bar.Baz: " << *baz << '\n';
	}

	return 0;
}
```
## Arrays
All arrays within a ***coc::Structure*** are stored under a special ***Array*** member. This ***Array*** then in turn stores all different types separately, the same as non array/single variables. The respective getter method returns an ***std::vector*** of the respective type.
___
New ***math.coc*** file.
```c
int Fibonacci = [
	0, 1, 1, 2, 3, 5, 8, 13, 21, 34
]
```
We open ***math.coc***. We retrieve ***Fibonacci***, then iterate through it and print it's contents. Of course, don't forget the necessary error checking, in case file loading or parsing is invalid.
```cpp
#include <iostream>
#include <coc.hpp>

int main() {
	auto math = coc::Open("math.coc");
	if (!math) return EXIT_FAILURE;

	auto fibonacci = math->Array.Int["Fibonacci"];
	if (fibonacci) {
		for (int &num : *fibonacci) {
			std::cout << num << ", ";
		}
	}

	return 0;
}
```