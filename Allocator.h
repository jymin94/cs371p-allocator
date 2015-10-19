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
            //    cout << index << ": " << left_value << endl;
                index += 4 + abs(left_value);
                int right_value = (*this)[index];
              //  cout << index << ": " << right_value << endl;
                if (left_value != right_value)
                    return false;
                index += 4;
            }
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
            // <your code>
            // get alloc_size = n * sizeof(T)
            // go through the array (from the beginning) + check whether the first positive sentinel is >= alloc_size
            // put in a new sentinel
            //cout << "n = " << n << endl; 

            //cout << "START ADDRESS PLS " << this << endl;
            difference_type alloc_size = n * sizeof(T);
            int index = 0;
            while (index <= N - 4) {
                if ((*this)[index] >= 0 && abs((*this)[index]) > alloc_size + 8) {
                    //int* end_sentinel = (int*)&(*this)[index];
                    //end_sentinel += 1 + (*end_sentinel)/sizeof(int);
                    
 //                   char* end_sentinel = (char*)&(*this)[index];
//                    end_sentinel += 4 + (*this)[index];
//                   cout << "END: " << static_cast<void*>(end_sentinel) << endl;
                    int first_sentinel_value = (*this)[index];
                    assert (first_sentinel_value != 0);
                    //int*  end_first_sentinel = &(*this)[index + first_sentinel_value + 4];
                    //cout << end_first_sentinel << endl;
                    int end_idx = index + first_sentinel_value + 4;
                    //cout << &(*this)[end_idx] << endl;
                   // cout << &(*this)[N-4] << endl;
                    //cout << "N is " << N << endl;
                    //cout << "alloc_size: " << alloc_size << endl;
                    (*this)[index] = -1 * alloc_size;
                    //cout << index << ": " << (*this)[index] << endl;
                    index += 4;
                    //index++;
                    pointer old_index = (pointer)&(*this)[index];
                    index += n * sizeof(T);
                    //index += sizeof(int) + alloc_size;
                    (*this)[index] = -1 * alloc_size;
                    //cout << index << ": " << (*this)[index] << endl;
                    index += 4;
                    pointer return_value = old_index; 
                   
                    (*this)[index] = N - (alloc_size + 16); // LEL IS DIS RITE DOE
                    //cout << index  << ": " << (*this)[index] << endl;
                    (*this)[end_idx] = N - (alloc_size + 16);
                    //cout << end_idx << ": " << (*this)[end_idx] << endl;
                    //cout << "hello pointer is " <<  return_value << endl;
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
            // given p, we want to get the address that p points to (no matter what type of pointer it is)
            //void* address = static_cast<void *>(p);
            //cout << "is this an address.." << address << endl;
            int* address = (int*)p; // bc we're at a sentinel which is type int
             // p points at the actual data, so go back 4 bytes to get to the sentinel 1st_sentinel_begin
            address--;
            int* first_sentinel_p = address;
            *first_sentinel_p = abs(*first_sentinel_p);
            address += 1 + *address/sizeof(int);
            int* end_sentinel_p = address; 
            *address = abs(*address); 
            assert (*first_sentinel_p == *address);
            address++;
            //PLS TEST THE COALESCES
            // COALESCE BACK
            if (address < (int*)&(*this)[N] && *address > 0) {
                *first_sentinel_p += *address + 8;
                address+= 1 + *address/sizeof(int);
                *address = *first_sentinel_p;
            }
            
            address = (int*)p;
            // COALESCE FRONT
            if (--address > (int*)&(*this) && *address > 0) {
                *end_sentinel_p += *address + 8;
                address -= (1 + *address/sizeof(int));
                *address = *end_sentinel_p;
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
