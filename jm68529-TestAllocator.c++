// ------------------------------------
// projects/allocator/TestAllocator1.c++
// Copyright (C) 2015
// Glenn P. Downing
// ------------------------------------

// --------
// includes
// --------

#include <algorithm> // count
#include <memory>    // allocator

#include "gtest/gtest.h"

#include "Allocator.h"

// --------------
// TestAllocator1
// --------------

template <typename A>
struct TestAllocator1 : testing::Test {
    // --------
    // typedefs
    // --------

    typedef          A                  allocator_type;
    typedef typename A::value_type      value_type;
    typedef typename A::difference_type difference_type;
    typedef typename A::pointer         pointer;};

typedef testing::Types<
            std::allocator<int>,
            std::allocator<double>,
            Allocator<int,    100>,
            Allocator<double, 100> >
        my_types_1;

TYPED_TEST_CASE(TestAllocator1, my_types_1);

TYPED_TEST(TestAllocator1, test_1) {
    typedef typename TestFixture::allocator_type  allocator_type;
    typedef typename TestFixture::value_type      value_type;
    typedef typename TestFixture::difference_type difference_type;
    typedef typename TestFixture::pointer         pointer;

          allocator_type  x;
    const difference_type s = 1;
    const value_type      v = 2;
    const pointer         p = x.allocate(s);
  
    if (p != nullptr) {
        x.construct(p, v);
        ASSERT_EQ(v, *p);
        x.destroy(p);
        x.deallocate(p, s);}}

TYPED_TEST(TestAllocator1, test_10) {
    typedef typename TestFixture::allocator_type  allocator_type;
    typedef typename TestFixture::value_type      value_type;
    typedef typename TestFixture::difference_type difference_type;
    typedef typename TestFixture::pointer         pointer;

          allocator_type  x;
    const difference_type s = 10;
    const value_type      v = 2;
    const pointer         b = x.allocate(s);
    if (b != nullptr) {
        pointer e = b + s;
        pointer p = b;
        try {
            while (p != e) {
                x.construct(p, v);
                ++p;}}
        catch (...) {
            while (b != p) {
                --p;
                x.destroy(p);}
            x.deallocate(b, s);
            throw;}
        ASSERT_EQ(s, std::count(b, e, v));
        while (b != e) {
            --e;
            x.destroy(e);}
        x.deallocate(b, s);}}

// --------------
// TestAllocator2
// --------------

TEST(TestAllocator2, const_index) {
    const Allocator<int, 100> x;
    ASSERT_EQ(x[0], 92);}

TEST(TestAllocator2, index) {
    Allocator<int, 100> x;
    ASSERT_EQ(x[0], 92);}

// --------------
// TestAllocator3
// --------------

template <typename A>
struct TestAllocator3 : testing::Test {
    // --------
    // typedefs
    // --------

    typedef          A                  allocator_type;
    typedef typename A::value_type      value_type;
    typedef typename A::difference_type difference_type;
    typedef typename A::pointer         pointer;};

typedef testing::Types<
            Allocator<int,    100>,
            Allocator<double, 100> >
        my_types_2;

TYPED_TEST_CASE(TestAllocator3, my_types_2);

TYPED_TEST(TestAllocator3, test_1) {
    typedef typename TestFixture::allocator_type  allocator_type;
    typedef typename TestFixture::value_type      value_type;
    typedef typename TestFixture::difference_type difference_type;
    typedef typename TestFixture::pointer         pointer;

          allocator_type  x;
    const difference_type s = 1;
    const value_type      v = 2;
    const pointer         p = x.allocate(s);
    if (p != nullptr) {
        x.construct(p, v);
        ASSERT_EQ(v, *p);
        x.destroy(p);
        x.deallocate(p, s);}}

TYPED_TEST(TestAllocator3, test_10) {
    typedef typename TestFixture::allocator_type  allocator_type;
    typedef typename TestFixture::value_type      value_type;
    typedef typename TestFixture::difference_type difference_type;
    typedef typename TestFixture::pointer         pointer;

          allocator_type  x;
    const difference_type s = 10;
    const value_type      v = 2;
    const pointer         b = x.allocate(s);
    if (b != nullptr) {
        pointer e = b + s;
        pointer p = b;
        try {
            while (p != e) {
                x.construct(p, v);
                ++p;}}
        catch (...) {
            while (b != p) {
                --p;
                x.destroy(p);}
            x.deallocate(b, s);
            throw;}
        ASSERT_EQ(s, std::count(b, e, v));
        while (b != e) {
            --e;
            x.destroy(e);}
        x.deallocate(b, s);}}


TEST(ProjAllocator, ConstructorDefault) {
    const Allocator<int, 200> x;
    ASSERT_EQ(x[0], 192);
    ASSERT_EQ(x[196], 192);
}

// constructs with an N not divisible by 4
TEST(ProjAllocator, ConstructorOdd) {
    const Allocator<double, 201> x;
    ASSERT_EQ(x[0], 193);
    ASSERT_EQ(x[197], 193);
}

TEST(ProjAllocator, ConstructorTooSmall) {
    try {
        Allocator<int, 9> x;
        ASSERT_TRUE(false);
    }
    catch (...) {
        ASSERT_TRUE(true);
    }
}

TEST(ProjAllocator, Valid1) {
    Allocator<int, 64> x;
    ASSERT_TRUE(x.valid());
}

TEST(ProjAllocator, Valid2) {
    Allocator<int, 64> x;
    int *a = x.allocate(1);
    int *b = x.allocate(1);
    x.deallocate(b, 4);
    x.deallocate(a, 4);
    ASSERT_TRUE(x.valid());
}

TEST(ProjAllocator, ValidFalse) {
    Allocator <int, 100> x;
    x[97] = -2;
    ASSERT_FALSE(x.valid());
}

TEST(ProjAllocator, Valid3) {
    Allocator<int, 100> x;
    x[0] = -1;
    ASSERT_FALSE(x.valid());
}

TEST(ProjAllocator, AllocateDeallocate1) {
    Allocator<double, 24> x;
    double *a = x.allocate(2);
    x.deallocate(a, 2);
    ASSERT_EQ(x[0], 16);
    ASSERT_EQ(x[20], 16);
}

TEST(ProjAllocator, AllocateDeallocate2) {
    Allocator<double, 100> x;
    double *a = x.allocate(1);
    double *b = x.allocate(1);
    x.deallocate(b, 2);
    ASSERT_EQ(x[16], 76);
    x.deallocate(a, 2);
    ASSERT_EQ(x[0], 92);
    ASSERT_EQ(x[96], 92);
}

TEST(ProjAllocator, AllocateNoSpace) {
    Allocator<int, 28> x;
    x.allocate(1);
    x.allocate(1);
    ASSERT_THROW({x.allocate(1);}, std::bad_alloc);
}

TEST(ProjAllocator, AllocateBigger) {
    Allocator<int, 44> x;
    x.allocate(1);
    x.allocate(1);
    x.allocate(1);
    ASSERT_EQ(x[24], -12);
    ASSERT_EQ(x[40], -12);
}

TEST(ProjAllocator, AllocateDoubles) {
    Allocator<double, 100> x;
    x.allocate(10);
    ASSERT_EQ(x[0], -92);
    ASSERT_EQ(x[96], -92);
}

TEST(ProjAllocator, DeallocateWrongPointer1) {
    Allocator<double, 100> x;
    ASSERT_THROW({x.deallocate(0, 5);}, std::invalid_argument);
}


TEST(ProjAllocator, DeallocateWrongPointer2) {
    Allocator<int, 100> x;
    int *a = x.allocate(2);
    ++a;
    ASSERT_THROW({x.deallocate(a, 5);}, std:: invalid_argument);
}

TEST(ProjAllocator, Deallocate1) {
    Allocator<int, 64> x;
    x.allocate(1);
    int *a = x.allocate(2);
    x.deallocate(a, 2);
    ASSERT_EQ(x[12], 44);
    ASSERT_EQ(x[60], 44);
}
