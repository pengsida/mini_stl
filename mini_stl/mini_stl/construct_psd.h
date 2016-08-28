//
//  construct_psd.h
//  mini_stl
//
//  Created by pengsida on 16/8/28.
//  Copyright © 2016年 pengsida. All rights reserved.
//

#ifndef construct_psd_h
#define construct_psd_h

#ifndef _LIBCPP_NEW
#include <new>
#endif

#ifndef iterator_traits_psd_h
#include "iterator_traits_psd.h"
#endif

#ifndef type_traits_psd_h
#include "type_traits_psd.h"
#endif

namespace mini_stl
{
    template<typename T1, typename T2>
    inline void construct(T1 * p, const T2 & value)
    {
        new ((void*)p) T1(value);
    }
    
    template<typename T1>
    inline void construct(T1 * p)
    {
        new ((void*)p) T1();
    }
    
    template<typename T>
    inline void destroy(T * p)
    {
        p->~T();
    }
    
    template<typename ForwardIterator>
    inline void __destroy_aux(ForwardIterator first, ForwardIterator last, true_type)
    {
    }
    
    template<typename ForwardIterator>
    inline void __destroy_aux(ForwardIterator first, ForwardIterator last, false_type)
    {
        for(; first != last; ++first)
            destroy(&*first);
    }
    
    template<typename ForwardIterator, typename T>
    inline void __destroy(ForwardIterator first, ForwardIterator last, T*)
    {
        typedef typename type_traits<T>::has_trivial_destructor trivial_destructor;
        __destroy_aux(first, last, trivial_destructor());
    }
    
    template<typename ForwardIterator>
    inline void destroy(ForwardIterator first, ForwardIterator last)
    {
        __destroy(first, last, __value_type(first));
    }
}

#endif /* construct_psd_h */
