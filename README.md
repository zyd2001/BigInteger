# BigInteger
A arbitrary precision integer C++ library.
## Version 0.1.0

### Basic functionality: 
Support construct from int, max signed type, std::string, C-style null-terminated string

Have almost all operation that native integer type has.

Arithmatic: Addition, Substraction, Multiplication, Division, Modulo.

Logic: Shift, Not, And, Or, Xor. All logic operations resemble 2's complement logic operation.

No increment decrement and assign operator yet.

Very limited or none optimization for algorithms. All operation use basic algorithm.

### Build

without test: `cmake . -Bbuild && cmake --build build`

with test: 
1. Execute `git submodule update --init` 
2. Then `cmake . -DBIGINTEGER_TEST=on -Bbuild && cmake --build build` 
3. Then change to `build` and run `ctest`