//
//  uninitialized_psd.h
//  mini_stl
//
//  Created by pengsida on 16/8/31.
//  Copyright © 2016年 pengsida. All rights reserved.
//

#ifndef uninitialized_psd_h
#define uninitialized_psd_h

#include "iterator_traits_psd.h"
#include "type_traits_psd.h"
#include "construct_psd.h"
#include "algorithm_psd.h"

// 对象已分配内存空间且还未初始化时使用

namespace mini_stl
{
    // uninitialized_copy
    
    template<typename InputIterator, typename ForwardIterator>
    inline ForwardIterator __uninitialized_copy_aux(InputIterator first, InputIterator last, ForwardIterator start, true_type)
    {
        return copy(first, last, start);
    }
    
    template<typename InputIterator, typename ForwardIterator>
    inline ForwardIterator __uninitialized_copy_aux(InputIterator first, InputIterator last, ForwardIterator start, false_type)
    {
        for(; first != last; ++start, ++first)
            construct(&*start, *first);
    }
    
    template<typename InputIterator, typename ForwardIterator, typename T_forward_iterator>
    inline ForwardIterator __uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator start, T_forward_iterator*)
    {
        typedef typename type_traits<T_forward_iterator>::is_POD_type is_POD;
        return __uninitialized_copy_aux(first, last, start, is_POD());
    }
    
    template<typename InputIterator, typename ForwardIterator>
    inline ForwardIterator uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator start)
    {
        return __uninitialized_copy(first, last, start, __value_type(start));
    }
    
    
    ///////////////////////////////////////
    // uninitialized_fill
    
    template<typename ForwardIterator, typename T>
    inline void __uninitialized_fill_aux(ForwardIterator first, ForwardIterator last, const T& value, true_type)
    {
        fill(first, last, value);
    }
    
    template<typename ForwardIterator, typename T>
    inline void __uninitialized_fill_axu(ForwardIterator first, ForwardIterator last, const T& value, false_type)
    {
        for(; first != last; ++first)
            construct(&*first, value);
    }
    
    template<typename ForwardIterator, typename T, typename T_iterator>
    inline void __uninitialized_fill(ForwardIterator first, ForwardIterator last, const T& value, T_iterator*)
    {
        typedef typename type_traits<T_iterator>::is_POD_type is_POD;
        __uninitialized_fill_aux(first, last, value, is_POD());
    }
    
    template<typename ForwardIterator, typename T>
    inline void uninitialized_fill(ForwardIterator first, ForwardIterator last, const T& value)
    {
        __uninitialized_fill(first, last, value, __value_type(first));
    }
    
    template<typename ForwardIterator, typename Size, typename T>
    inline ForwardIterator __uninitialized_fill_n_aux(ForwardIterator first, Size n, const T& value, true_type)
    {
        return fill_n(first, n, value);
    }
    
    template<typename ForwardIterator, typename Size, typename T>
    inline ForwardIterator __uninitialized_fiil_n_aux(ForwardIterator first, Size n, const T& value, false_type)
    {
        for(; n > 0; --n, ++first)
            construct(&*first, value);
        return first;
    }
    
    template<typename ForwardIterator, typename Size, typename T_value, typename T_iterator>
    inline ForwardIterator __uninitialized_fill_n(ForwardIterator first, Size n, const T_value& value, T_iterator*)
    {
        typedef typename type_traits<T_iterator>::is_POD_type is_POD;
        return __uninitialized_fill_n_aux(first, n, value, is_POD());
    }
    
    template<typename ForwardIterator, typename Size, typename T>
    inline ForwardIterator uninitialized_fill_n(ForwardIterator first, Size n, const T& value)
    {
        return __uninitialized_fill_n(first, n, value, __value_type(first));
    }
}

#endif /* uninitialized_psd_h */
