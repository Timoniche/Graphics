#include <gtest/gtest.h>
#include "DCOBits.h"

namespace
{
    class Base : public DCOBits
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
        DCOBits b;
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
    DCOBits const b;
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
    DCOBits b;
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
        DCOBits a1 = fast_cast<DCOBits>(b);
        DCOBits a2 = dynamic_cast<DCOBits &>(b);
        EXPECT_EQ(a2, a1);
    } catch (std::bad_cast &e)
    {
        std::cout << e.what() << std::endl;
    }
}

TEST(correctness, test_self) //NOLINT
{
    Base b;
    try
    {
        DCOBits* a1 = fast_cast<DCOBits>(&b);
        DCOBits* a2 = dynamic_cast<DCOBits *>(&b);
        EXPECT_EQ(a2, a1);
    } catch (std::bad_cast &e)
    {
        std::cout << e.what() << std::endl;
    }
}
//______________________________________________________________________________________________________________________


