//
//  uninitialized_psd.h
//  mini_stl
//
//  Created by pengsida on 16/8/31.
//  Copyright © 2016年 pengsida. All rights reserved.
//

#ifndef uninitialized_psd_h
#define uninitialized_psd_h

#ifndef iterator_traits_psd_h
#include "iterator_traits_psd.h"
#endif

#ifndef type_traits_psd_h
#include "type_traits_psd.h"
#endif

#ifndef construct_psd_h
#include "construct_psd.h"
#endif

#ifndef _LIBCPP_CSTRING
#include <cstring>
#endif

namespace mini_stl
{
    inline void fill(unsigned char* first, unsigned char* last, const unsigned char& value)
    {
        unsigned char tmp = value;
        memset(first, tmp, last - first);
    }
    
    inline void fill(signed char* first, signed char* last, const signed char& value)
    {
        signed char tmp = value;
        memset(first, static_cast<unsigned char>(tmp), last - first);
    }
    
    inline void fill(char* first, char* last, const char& value)
    {
        char tmp = value;
        memset(first, static_cast<unsigned char>(tmp), last - first);
    }
    
    template<typename ForwardIterator, typename Size, typename T>
    inline ForwardIterator fill_n(ForwardIterator first, Size n, const T& value)
    {
        for(; n > 0; --n, ++first)
            *first = value;
        return first;
    }
    
    template<typename Size>
    inline unsigned char* fill_n(unsigned char* first, Size n, const unsigned char& value)
    {
        fill(first, first + n, value);
        return first + n;
    }
    
    template<typename Size>
    inline signed char* fill_n(signed char* first, Size n, const signed char& value)
    {
        fill(first, first + n, value);
        return first + n;
    }
    
    template<typename Size>
    inline char* fill_n(char* first, Size n, const char& value)
    {
        fill(first, first + n, value);
        return first + n;
    }
    
    template<typename ForwardIterator, typename Size, typename T>
    inline ForwardIterator __uninitialized_fiil_n_aux(ForwardIterator first, Size n, const T& value, false_type)
    {
        for(; n > 0; --n, ++first)
            construct(&*first, value);
        return first;
    }
    
    template<typename ForwardIterator, typename Size, typename T>
    inline ForwardIterator __uninitialized_fill_n_aux(ForwardIterator first, Size n, const T& value, true_type)
    {
        return fill_n(first, n, value);
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
