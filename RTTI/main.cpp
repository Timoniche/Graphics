#include <iostream>
#include <cassert>

#include "DynamicCastObject.h"

struct Base1 : virtual DynamicCastObject
{
    Base1()
    {
        this->tie(this);
    }
};

struct Base2 : virtual DynamicCastObject
{
    Base2()
    {
        this->tie(this);
    }
};

struct MD : Base1, Base2
{
    MD()
    {
        this->tie(this);
    }
};

int main()
{
    auto md = new MD();
    auto base2 = dynamic_cast<Base2*>(md);
    std::cerr << md << std::endl;
    std::cerr << base2 << std::endl;

    return 0;
}