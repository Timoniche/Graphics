#include <gtest/gtest.h>
#include "DynamicCastObject.h"

namespace
{
    /*
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
     */

    class Base : public DynamicCastObject
    {
    public:
        Base()
        { this->tie(this); }
    };
}

//______________________________________________________________________________________________________________________
TEST(correctness, pointer_test) // NOLINT
{
    try
    {
        DynamicCastObject b;
        Base *a1 = b.fast_cast<Base>();
        Base *a2 = fast_cast<Base>(&b);
        Base *a3 = dynamic_cast<Base *>(&b);
        EXPECT_EQ(a3, a1);
        EXPECT_EQ(a3, a2);
    } catch (std::bad_cast &e)
    {
        std::cout << e.what() << std::endl;
    }
}

TEST(correctness, const_pointer_test) // NOLINT
{
    DynamicCastObject const b;
    try
    {
        Base const *a1 = b.fast_cast<Base>();
        Base const *a2 = fast_cast<Base>(&b);
        Base const *a3 = dynamic_cast<Base const *>(&b);
        EXPECT_EQ(a3, a1);
        EXPECT_EQ(a3, a2);
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
        void *a2 = fast_cast<void>(&b);
        void *a3 = dynamic_cast<void *>(&b);
        EXPECT_EQ(a3, a1);
        EXPECT_EQ(a3, a2);
    } catch (std::bad_cast &e)
    {
        std::cout << e.what() << std::endl;
    }
}

TEST(correctness, const_void_test) //NOLINT
{
    Base const b;
    try
    {
        void const *a1 = b.fast_cast<void>();
        void const *a2 = fast_cast<void>(&b);
        void const *a3 = dynamic_cast<void const *>(&b);
        EXPECT_EQ(a3, a1);
        EXPECT_EQ(a3, a2);
    } catch (std::bad_cast &e)
    {
        std::cout << e.what() << std::endl;
    }
}
//______________________________________________________________________________________________________________________
TEST(correctness, ref_test_bad_cast) //NOLINT
{
    DynamicCastObject b;
    try
    {
        Base a1 = fast_cast<Base>(b);
        Base a2 = dynamic_cast<Base &>(b);
        EXPECT_EQ(a2, a1);
    } catch (std::bad_cast &e)
    {
        std::cout << e.what() << std::endl;
    }
}

TEST(correctness, ref_test) //NOLINT
{
    Base b;
    try
    {
        DynamicCastObject a1 = fast_cast<DynamicCastObject>(b);
        DynamicCastObject a2 = dynamic_cast<DynamicCastObject &>(b);
        EXPECT_EQ(a2, a1);
    } catch (std::bad_cast &e)
    {
        std::cout << e.what() << std::endl;
    }
}
//______________________________________________________________________________________________________________________
