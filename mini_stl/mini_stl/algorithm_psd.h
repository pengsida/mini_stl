//
//  algorithm_psd.h
//  mini_stl
//
//  Created by pengsida on 16/8/31.
//  Copyright © 2016年 pengsida. All rights reserved.
//

#ifndef algorithm_psd_h
#define algorithm_psd_h

#include "type_traits_psd.h"
#include "iterator_traits_psd.h"
#include <cstring>

namespace mini_stl
{
    // swap
    
    template<typename ForwardIterator, typename T>
    inline void __iter_swap(ForwardIterator a, ForwardIterator b, T*)
    {
        T tmp = *a;
        *a = *b;
        *b = tmp;
    }
    
    template<typename ForwardIterator>
    inline void iter_swap(ForwardIterator a, ForwardIterator b)
    {
        __iter_swap(a, b, __value_type(a));
    }
    
    template<typename ForwardIterator>
    inline ForwardIterator swap_range(ForwardIterator first, ForwardIterator last, ForwardIterator start)
    {
        for(; first != last; ++first, ++start)
            iter_swap(first, start);
        return start;
    }
    
    template<typename T>
    inline void swap(T& a, T& b)
    {
        T tmp = a;
        a = b;
        b = tmp;
    }
    
    
    /////////////////////////////////////////
    // distance
    
    template<typename InputIterator, typename DistanceType>
    inline void __distance(InputIterator first, InputIterator last, DistanceType& n, input_iterator_tag)
    {
        while(first != last)
        {
            ++first;
            ++n;
        }
    }
    
    template<typename InputIterator, typename DistanceType>
    inline void __distance(InputIterator first, InputIterator last, DistanceType& n, random_access_iterator_tag)
    {
        n += (last - first);
    }
    
    template<typename InputIterator, typename DistanceType>
    inline void distance(InputIterator first, InputIterator last, DistanceType& n)
    {
        __distance(first, last, n, iterator_category(first));
    }
    
    template<typename InputIterator>
    inline typename iterator_traits<InputIterator>::difference_type __distance(InputIterator first, InputIterator last, input_iterator_tag)
    {
        typename iterator_traits<InputIterator>::difference_type n = 0;
        while(first != last)
        {
            ++first;
            ++n;
        }
        return n;
    }
    
    template<typename InputIterator>
    inline typename iterator_traits<InputIterator>::difference_type __distance(InputIterator first, InputIterator last, random_access_iterator_tag)
    {
        return last - first;
    }
    
    template<typename InputIterator>
    inline typename iterator_traits<InputIterator>::difference_type distance(InputIterator first, InputIterator last)
    {
        return __distance(first, last, iterator_category(first));
    }
    
    /////////////////////////////////////
    // copy_backward
    
    // [first, last), 复制到[end-(last - first), end)
    template<typename InputIterator, typename BackwardIterator>
    inline BackwardIterator copy_backward(InputIterator first, InputIterator last, BackwardIterator end)
    {
        for(; last != first;)
            *--end = *--last;
        return end;
    }
    
    
    
    
    /////////////////////////////////////
    // copy
    
    inline char* copy(const char* first, const char* last, char* start)
    {
        memmove(start, first, sizeof(char) * (last - first));
        return start + (last - first);
    }
    
    inline signed char* copy(const signed char* first, const signed char* last, signed char* start)
    {
        memmove(start, first, sizeof(signed char) * (last - first));
        return start + (last - first);
    }
    
    inline unsigned char* copy(const unsigned char* first, const unsigned char* last, unsigned char* start)
    {
        memmove(start, first, sizeof(unsigned char) * (last - first));
        return start + (last - first);
    }
    
    inline short* copy(const short* first, const short* last, short* start)
    {
        memmove(start, first, sizeof(short) * (last - first));
        return start + (last - first);
    }
    
    inline unsigned short* copy(const unsigned short* first, const unsigned short* last, unsigned short* start)
    {
        memmove(start, first, sizeof(unsigned short) * (last - first));
        return start + (last - first);
    }
    
    inline int* copy(const int* first, const int* last, int* start)
    {
        memmove(start, first, sizeof(int) * (last - first));
        return start + (last - first);
    }
    
    inline unsigned int* copy(const unsigned int* first, const unsigned int* last, unsigned int* start)
    {
        memmove(start, first, sizeof(unsigned int) * (last - first));
        return start + (last - first);
    }
    
    inline long* copy(const long* first, const long* last, long* start)
    {
        memmove(start, first, sizeof(long) * (last - first));
        return start + (last - first);
    }
    
    inline unsigned long* copy(const unsigned long* first, const unsigned long* last, unsigned long* start)
    {
        memmove(start, first, sizeof(unsigned long) * (last - first));
        return start + (last - first);
    }
    
    inline long long* copy(const long long* first, const long long* last, long long* start)
    {
        memmove(start, first, sizeof(long long) * (last - first));
        return start + (last - first);
    }
    
    inline unsigned long long* copy(const unsigned long long* first, const unsigned long long* last, unsigned long long* start)
    {
        memmove(start, first, sizeof(unsigned long long) * (last - first));
        return start + (last - first);
    }
    
    inline float* copy(const float* first, const float* last, float* start)
    {
        memmove(start, first, sizeof(float) * (last - first));
        return start + (last - first);
    }
    
    inline double* copy(const double* first, const double* last, double* start)
    {
        memmove(start, first, sizeof(double) * (last - first));
        return start + (last - first);
    }
    
    inline long double* copy(const long double* first, const long double* last, long double* start)
    {
        memmove(start, first, sizeof(long double) * (last - first));
        return start + (last - first);
    }
    
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
