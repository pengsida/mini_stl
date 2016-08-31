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
        // placement new：只是operator new重载的一个版本。它并不分配内存，只是返回指向已经分配好的某段内存的一个指针。因此不能删除它，但需要调用对象的析构函数。
        new ((void*)p) T1(value);
    }
    
    template<typename T1>
    inline void construct(T1 * p)
    {
        //placement new：只是operator new重载的一个版本。它并不分配内存，只是返回指向已经分配好的某段内存的一个指针。因此不能删除它，但需要调用对象的析构函数。
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
