#include <gtest/gtest.h>
#include "DynamicCast.h"

namespace
{
    struct A
    {
        A() = default;
        virtual ~A() = default;
        char a = 'a';
    };

    struct B : A
    {
        B() = default;
        ~B() override = default;
        char b = 'b';
    };

    class Base : public DynamicCastObject
    {
    public:
        Base()
        { this->tie(this); }
    };
}


TEST(correctness, ref_test) // NOLINT
{
    DynamicCastObject b;
    Base *a1 = b.fast_cast<Base>();
    Base *a2 = dynamic_cast<Base *>(&b);
    EXPECT_EQ(a1, a2);
}

TEST(correctness, const_ref_test) // NOLINT
{
    DynamicCastObject b;
    Base const *a1 = b.fast_cast<Base>();
    Base const *a2 = dynamic_cast<Base const *>(&b);
    EXPECT_EQ(a1, a2);
}