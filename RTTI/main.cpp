#include <iostream>
#include <cassert>

#include "DynamicCastObject.h"

using std::cout;

struct Test
{
    static size_t gen_id()
    {
        cout << "was here\n";
        static size_t _id = 0;
        return ++_id;
    }

    template<typename T>
    struct Type
    {
        static size_t id()
        {
            static const size_t type_id = gen_id();
            return type_id;
        }
    };
};
int main()
{
    Test::Type<int>::id();
    Test::Type<int>::id();
    return 0;
}