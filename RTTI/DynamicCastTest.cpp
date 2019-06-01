#include <gtest/gtest.h>
#include "DynamicCastObject.h"

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
//______________________________________________________________________________________________________________________
TEST(correctness, ref_test) // NOLINT
{
    try
    {
        DynamicCastObject b;
        Base *a1 = b.fast_cast<Base>();
        Base *a2 = dynamic_cast<Base *>(&b);
        EXPECT_EQ(a1, a2);
    } catch (std::bad_cast &e)
    {
        std::cout << e.what() << std::endl;
    }
}

TEST(correctness, const_ref_test) // NOLINT
{
    DynamicCastObject b;
    try
    {
        Base const *a1 = b.fast_cast<Base>();
        Base const *a2 = dynamic_cast<Base const *>(&b);
        EXPECT_EQ(a1, a2);
    } catch (std::bad_cast &e)
    {
        std::cout << e.what() << std::endl;
    }
}
//______________________________________________________________________________________________________________________
TEST(correctness, void_test) //NOLINT
{
    Base b;
    try
    {
        void *a1 = b.fast_cast<void>();
        void *a2 = dynamic_cast<void *>(&b);
        EXPECT_EQ(a1, a2);
    } catch (std::bad_cast &e)
    {
        std::cout << e.what() << std::endl;
    }
}

TEST(correctness, const_void_test) //NOLINT
{
    Base b;
    try
    {
        void const *a1 = b.fast_cast<void>();
        void const *a2 = dynamic_cast<void const *>(&b);
        EXPECT_EQ(a1, a2);
    } catch (std::bad_cast &e)
    {
        std::cout << e.what() << std::endl;
    }
}
//______________________________________________________________________________________________________________________
