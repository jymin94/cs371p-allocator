// ------------------------------
// projects/allocator/Allocator.h
// Copyright (C) 2015
// Glenn P. Downing
// ------------------------------

#ifndef Allocator_h
#define Allocator_h

// --------
// includes
// --------

#include <cassert>   // assert
#include <cstddef>   // ptrdiff_t, size_t
#include <new>       // bad_alloc, new
#include <stdexcept> // invalid_argument
#include <cmath> //abs

// ---------
// Allocator
// ---------
using namespace std;
template <typename T, std::size_t N>
class Allocator {
    public:
        // --------
        // typedefs
        // --------

        typedef T                 value_type;

        typedef std::size_t       size_type;
        typedef std::ptrdiff_t    difference_type;

        typedef       value_type*       pointer;
        typedef const value_type* const_pointer;

        typedef       value_type&       reference;
        typedef const value_type& const_reference;

    public:
        // -----------
        // operator ==
        // -----------

        friend bool operator == (const Allocator&, const Allocator&) {
            return true;}                                              // this is correct

        // -----------
        // operator !=
        // -----------

        friend bool operator != (const Allocator& lhs, const Allocator& rhs) {
            return !(lhs == rhs);}

    private:
        // ----
        // data
        // ----

        char a[N];
        
        // -----
        // valid
        // -----

        /**
         * O(1) in space
         * O(n) in time
         * <your documentation>
         */
        bool valid () const {
            int index = 0;
            while (index < N) {
                int left_value = (*this)[index];
             //   cout << index << ": " << left_value << endl;
                index += 4 + abs(left_value);
                int right_value = (*this)[index];
                assert(left_value != 0 && right_value != 0);
               // cout << index << ": " << right_value << endl;
                if (left_value != right_value) {
                    //cout << left_value << " != " << right_value << endl;
                    return false;
                }
                index += 4;
            }
            return true;}

        /**
         * O(1) in space
         * O(1) in time
         * <your documentation>
         * https://code.google.com/p/googletest/wiki/AdvancedGuide#Private_Class_Members
         */
       // FRIEND_TEST(TestAllocator2, index);
        
        // FOR NICOLE'S TEST
        FRIEND_TEST(TestAllocator2, index_1);
        FRIEND_TEST(TestAllocator2, index_2);
        FRIEND_TEST(TestAllocator2, index_3);

        FRIEND_TEST(TestAllocator2, default_constructor_1);
        FRIEND_TEST(TestAllocator2, default_constructor_2);
        FRIEND_TEST(TestAllocator2, default_constructor_3);

        FRIEND_TEST(AllocateFixture, allocate_1);
        FRIEND_TEST(AllocateFixture, allocate_2);
        FRIEND_TEST(AllocateFixture, allocate_3);

        FRIEND_TEST(DeallocateFixture, deallocate_1);
        FRIEND_TEST(DeallocateFixture, deallocate_2);
        FRIEND_TEST(DeallocateFixture, deallocate_3);

        FRIEND_TEST(TestAllocator2, valid_1);
        FRIEND_TEST(TestAllocator2, valid_2);
        FRIEND_TEST(TestAllocator2, valid_3);

        int& operator [] (int i) {
            return *reinterpret_cast<int*>(&a[i]);}

    public:
        // ------------
        // constructors
        // ------------

        /**
         * O(1) in space
         * O(1) in time
         * throw a bad_alloc exception, if N is less than sizeof(T) + (2 * sizeof(int))
         */
        Allocator () {
            if (N < sizeof(T) + (2 * sizeof(int)))
                throw bad_alloc();
            (*this)[0] = N-8; // replace!
            (*this)[N-4] = N-8;
           
            // <your code>
            assert(valid());}

        // Default copy, destructor, and copy assignment
        // Allocator  (const Allocator&);
        // ~Allocator ();
        // Allocator& operator = (const Allocator&);
/*
        // get_first_sentinel
        char* get_sentinel_start (void* p) {
            char* sentinel = (char*)p;

        }
*/

        // --------
        // allocate
        // --------

        /**
         * O(1) in space
         * O(n) in time
         * after allocation there must be enough space left for a valid block
         * the smallest allowable block is sizeof(T) + (2 * sizeof(int))
         * choose the first block that fits
         * throw a bad_alloc exception, if n is invalid
         */
        pointer allocate (size_type n) {
            if (n == 0)
                return nullptr;
            difference_type alloc_size = n * sizeof(T);
            if (N < sizeof(T) + (2 * sizeof(int)))
                throw bad_alloc();
            int index = 0;
            assert ((*this)[index] != 0 && (*this)[N-4] != 0);
            while (index <= N - 4) {
                if ((*this)[index] >= 0 && abs((*this)[index]) >= alloc_size) {
                    int first_sentinel_value = (*this)[index];
                    assert (first_sentinel_value != 0);
                    int end_idx = index + first_sentinel_value + 4;
                    (*this)[index] = -1 * alloc_size;
                    index += 4;
                    pointer old_index = (pointer)&(*this)[index];
                    index += n * sizeof(T);
                    (*this)[index] = -1 * alloc_size;
                    index += 4;
                    pointer return_value = old_index; 
                  
                    if (index <= N-4) {
                        (*this)[index] = (N - index) - 8;
                        (*this)[end_idx] = (N - index)- 8;
                    }
                    return return_value; 
                }
                else if ((*this)[index] < 0) {
                    index += abs((*this)[index]) + 8;
                    if (index > N-4 || abs((*this)[index]) < alloc_size) {
                        throw bad_alloc();
                    }
                }
                else 
                    throw bad_alloc();
            }
            assert(valid());
            throw bad_alloc();}      // if there wasn't enough space
       
        // ---------
        // construct
        // ---------

        /**
         * O(1) in space
         * O(1) in time
         */
        void construct (pointer p, const_reference v) {
            new (p) T(v);                               // this is correct and exempt
            assert(valid());}                           // from the prohibition of new

        // ----------
        // deallocate
        // ----------

        /**
         * O(1) in space
         * O(1) in time
         * after deallocation adjacent free blocks must be coalesced
         * throw an invalid_argument exception, if p is invalid
         * <your documentation>
         */
        void deallocate (pointer p, size_type) {
            if (p == 0)
                throw invalid_argument("p is wrong");
            size_t addr = reinterpret_cast<size_t>(p);
            size_t this_addr = reinterpret_cast<size_t>(this);
            
            int index = addr - this_addr;
            index -= 4;
            if ((*this)[index] >= 0)
                throw bad_alloc();
            int first_sent_idx = index;
            (*this)[first_sent_idx] = abs((*this)[first_sent_idx]);
            index += 4 + (*this)[index];
            int end_sent_idx = index;
            (*this)[end_sent_idx] = abs((*this)[index]);
            assert ((*this)[first_sent_idx] == (*this)[index]);
            int right_co_idx = index + 4;
            // coalescing to the right 
            while (right_co_idx < N - 8 && (*this)[right_co_idx] > 0) {
                assert((*this)[first_sent_idx] > 0);
                (*this)[first_sent_idx] += 8 + (*this)[right_co_idx];
                right_co_idx += 4 + (*this)[right_co_idx];
                (*this)[right_co_idx] = (*this)[first_sent_idx];
                end_sent_idx = right_co_idx;
                right_co_idx += 4;
            }
            // coalescing to the left  
            int front_index = first_sent_idx - 4;
            while (front_index > 8 && (*this)[front_index] > 0) {
                assert((*this)[end_sent_idx] > 0);
                front_index -= (4 + (*this)[front_index]);
                (*this)[front_index] += 8 + (*this)[first_sent_idx];
                (*this)[end_sent_idx] = (*this)[front_index]; 
                front_index -= 4;
            }
           assert(valid());}

        // -------
        // destroy
        // -------

        /**
         * O(1) in space
         * O(1) in time
         */
        void destroy (pointer p) {
            p->~T();               // this is correct
            assert(valid());}

        /**
         * O(1) in space
         * O(1) in time
         * <your documentation>
         */
        const int& operator [] (int i) const {
            return *reinterpret_cast<const int*>(&a[i]);}};

#endif // Allocator_h
