//
//  algorithm_psd.h
//  mini_stl
//
//  Created by pengsida on 16/8/31.
//  Copyright © 2016年 pengsida. All rights reserved.
//

#ifndef algorithm_psd_h
#define algorithm_psd_h

#include "iterator_traits_psd.h"

#ifndef _LIBCPP_CSTRING
#include <cstring>
#endif

namespace mini_stl
{
    // distance
    template<typename InputIterator>
    inline typename iterator_traits<InputIterator>::difference_type distance(InputIterator first, InputIterator last)
    {
        typedef typename iterator_traits<InputIterator>::iterator_category category;
        
    }
    
    
    
    /////////////////////////////////////
    // copy_backward
    
    // (first, last]
    template<typename InputIterator, typename BackwardIterator>
    inline BackwardIterator copy_backward(InputIterator first, InputIterator last, BackwardIterator end)
    {
        for(; last != first; --last, --end)
            *end = *last;
        return end;
    }
    
    
    
    /////////////////////////////////////
    // copy
            
    // [first, last)
    template<typename InputIterator, typename ForwardIterator>
    inline ForwardIterator copy(InputIterator first, InputIterator last, ForwardIterator start)
    {
        for(; first != last; ++start, ++first)
            *start = *first;
        return start;
    }
    
    
    
    //////////////////////////////////////
    // fill
    
    // [first, last)
    template<typename ForwardIterator, typename T>
    inline void fill(ForwardIterator first, ForwardIterator last, const T& value)
    {
        for(; first != last; ++first)
            *first = value;
    }
    
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
    
    // [first, last)
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
}

#endif /* algorithm_psd_h */
