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
            // <your code>
            return true;}

        /**
         * O(1) in space
         * O(1) in time
         * <your documentation>
         * https://code.google.com/p/googletest/wiki/AdvancedGuide#Private_Class_Members
         */
        FRIEND_TEST(TestAllocator2, index);
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
            (*this)[N-1] = N-8;
           
            // <your code>
            assert(valid());}

        // Default copy, destructor, and copy assignment
        // Allocator  (const Allocator&);
        // ~Allocator ();
        // Allocator& operator = (const Allocator&);

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
            // <your code>
            // get alloc_size = n * sizeof(T)
            // go through the array (from the beginning) + check whether the first positive sentinel is >= alloc_size
            // put in a new sentinel
            //cout << "n = " << n << endl; 
            cout << "START ADDRESS PLS " << this << endl;
            difference_type alloc_size = n * sizeof(T);
            int index = 0;
            while (index < N - 4) {
                if ((*this)[index] >= 0 && abs((*this)[index]) > alloc_size + 8) {
                    
                    (*this)[index] = -1 * alloc_size;
                    cout << index << ": " << &(*this)[index] << endl;
                    index += 4;
                    //index++;
                    pointer old_index = (pointer)&(*this)[index];
                    index += n * sizeof(int);
                    //index += sizeof(int) + alloc_size;
                    (*this)[index] = -1 * alloc_size;
                    cout << index << ": " << &(*this)[index] << endl;
                    //index += 4;
                    index++;
                    size_t wtf = 4;
                    pointer return_value = old_index; 
                   
                    (*this)[index] -= (alloc_size + 8); // LEL IS DIS RITE DOE
                    (*this)[N-1] -= (alloc_size + 8);
                    cout << "hello pointer is " <<  return_value << endl;
                    return return_value; 
                }
                else if (abs((*this)[index]) > alloc_size + 8 || (*this)[index] < 0) {
                    index += abs((*this)[index]);
                }
            }
            assert(valid());
            return 0;} // if there wasn't enough space
            //return nullptr;}             // replace!

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
            // <your code>
            
            // given p, we want to get the address that p points to (no matter what type of pointer it is)
            //void* address = static_cast<void *>(p);
            //cout << "is this an address.." << address << endl;
            
            cout << "deallocate got address " <<  p << endl;
            // fuq 
            cout << *(p) << endl;
            // p points at the actual data, so go back 4 bytes to get to the sentinel 1st_sentinel_begin
            assert(*(p) < 0);
            // then change the value to positive 
            // then skip forward the value of the sentinel to get to the other sentinel 1st_sentinel_end
            // then change that value to positive

            // skip forward 4 bytes to check for another free space (another positive sentinel)
                //(there should either be a negative or positive sentinel there)
            
            //if there is a positive sentinel 2nd_sentinel_begin, then 1st_sentinel_begin = positive value of that 2nd_sentinel_begin + value of 1st_sentinel_begin + 8 (for absorbing the 1st_sentinel_end and 2nd_sentinel_begin of the other free block)
            // from there skip forward the value of 2nd_sentinel_begin
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
