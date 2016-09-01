//
//  allocator_psd.h
//  mini_stl
//
//  Created by pengsida on 16/8/28.
//  Copyright © 2016年 pengsida. All rights reserved.
//

#ifndef allocator_psd_h
#define allocator_psd_h

#include "alloc_psd.h"
#include "alloc_default_psd.h"
#include "construct_psd.h"
#include "uninitialized_psd.h"

namespace mini_stl
{
    template<typename T, typename Alloc>
    class allocator
    {
    public:
        typedef size_t size_type;
        typedef ptrdiff_t difference_type;
        typedef T value_type;
        typedef T* pointer;
        typedef const T* const_pointer;
        typedef T& reference;
        typedef const T& const_reference;
        
    public:
        allocator(){}
        static value_type* allocate(size_t n);
        static value_type* allocate();
        static void deallocate(pointer p, size_t n);
        static void deallocate(pointer p);
        static value_type* reallocate(pointer p, size_t old_sz, size_t new_sz);
    };
    
    template<typename T, typename Alloc>
    typename allocator<T, Alloc>::value_type* allocator<T, Alloc>::allocate(size_t n)
    {
        return 0 == n ? 0 : static_cast<pointer>(Alloc::allocate(n * sizeof(value_type)));
    }
    
    template<typename T, typename Alloc>
    typename allocator<T, Alloc>::value_type* allocator<T, Alloc>::allocate()
    {
        return static_cast<pointer>(Alloc::allocator(sizeof(value_type)));
    }
    
    template<typename T, typename Alloc>
    void allocator<T, Alloc>::deallocate(pointer p, size_t n)
    {
        Alloc::deallocate(p, n * sizeof(value_type));
    }
    
    template<typename T, typename Alloc>
    void allocator<T, Alloc>::deallocate(pointer p)
    {
        Alloc::deallocate(p, sizeof(value_type));
    }
    
    template<typename T, typename Alloc>
    typename allocator<T, Alloc>::value_type* allocator<T, Alloc>::reallocate(pointer p, size_t old_sz, size_t new_sz)
    {
        Alloc::reallocate(p, old_sz, new_sz);
    }
}

#endif /* allocator_psd_h */
