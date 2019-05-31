#include <iostream>
#include <cassert>

struct Base { virtual ~Base(); };
struct Derived:  Base {};

void func(Base& base)
{
    Derived& derived = static_cast<Derived&>(base); // Fails
}

int main()
{

}