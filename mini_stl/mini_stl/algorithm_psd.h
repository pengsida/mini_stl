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
#include "reverse_iterator_psd.h"
#include <cstring>

namespace mini_stl
{
    template<typename InputIterator1, typename InputIterator2, typename OutputIterator>
    OutputIterator merge(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, OutputIterator result)
    {
        while(first1 != last1 && first2 != last2)
            if(*first1 <= *first2)
                *(result++) = *(first1++);
            else
                *(result++) = *(first2++);
        return copy(first1, last1, copy(first2, last2, result));
    }
    
    template<typename InputIterator1, typename InputIterator2, typename OutputIterator, typename Compare>
    OutputIterator merge(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, OutputIterator result, Compare comp)
    {
        while(first1 != last1 && first2 != last2)
            if(comp(*first2, *first1))
                *(result++) = *(first2++);
            else
                *(result++) = *(first1++);
        return copy(first2, last2, copy(first1, last1, result));
    }
    
    
    
    
    ///////////////////////////////////////////
    // max and min
    template<typename T>
    inline T max(const T& x, const T& y)
    {
        return x > y ? x : y;
    }
    
    template<typename T>
    inline T min(const T& x, const T& y)
    {
        return x < y ? x : y;
    }
    
    ////////////////////////////////////////////
    // find
    
    template<typename SearchedIterator, typename InputIterator>
    inline SearchedIterator find_first_of(SearchedIterator first1, SearchedIterator last1, InputIterator first2, InputIterator last2)
    {
        for(;first1 != last1; ++first1)
            for(InputIterator itr = first2; itr != last2; ++itr)
                if(*itr == *first1)
                    return first1;
        return last1;
    }
    
    template<typename SearchedIterator, typename T>
    inline SearchedIterator find(SearchedIterator first, SearchedIterator last, const T& value)
    {
        while(first != last && !(*first == value))
            ++first;
        return first;
    }
    
    
    
    
    
    ///////////////////////////////////////////
    // search
    
    // 以下函数只适用于forward_iterator类型的迭代器
    // 要求SearchedIterator和InputIterator中的元素可以相比较
    template<typename SearchedIterator, typename InputIterator>
    SearchedIterator search(SearchedIterator first1, SearchedIterator last1, InputIterator first2, InputIterator last2)
    {
        if(first1 == last1 || first2 == last2)
            return first1;
        InputIterator tmp(first2);
        ++tmp;
        // 考虑所寻找的内容长度为1的情况
        if(tmp == last2)
            return find(first1, last1, *first2);
        InputIterator second_input(first2);
        ++second_input;
        InputIterator cur_input;
        SearchedIterator cur_search_pos(first1);
        while(first1 != last1)
        {
            first1 = find(first1, last1, *first2);
            if(first1 == last1)
                return last1;
            cur_input = second_input;
            cur_search_pos = first1;
            // 排除剩下的内容长度为1的情况
            if(++cur_search_pos == last1)
                return last1;
            while(*cur_search_pos == *cur_input)
            {
                if(++cur_input == last2)
                    return first1;
                if(++cur_search_pos == last1)
                    return last1;
            }
            ++first1;
        }
        return first1;
    }
    
    template<typename SearchedIterator, typename Size, typename T>
    SearchedIterator search_n(SearchedIterator first, SearchedIterator last, Size n, const T& value)
    {
        if(n <= 0)
            return first;
        else
        {
            first = find(first, last, value);
            while(first != last)
            {
                Size count = n - 1;
                SearchedIterator cur_pos(first);
                ++cur_pos;
                while(cur_pos != last && n != 0 && *cur_pos == value)
                {
                    ++cur_pos;
                    --count;
                }
                if(count == 0)
                    return first;
                else
                    first = find(cur_pos, last, value);
            }
            return last;
        }
    }
    
    
    
    
    
    
    /////////////////////////////////////////////
    // find_end
    
    template<typename SearchedIterator, typename InputIterator>
    inline SearchedIterator __find_end(SearchedIterator first1, SearchedIterator last1, InputIterator first2, InputIterator last2, forward_iterator_tag, forward_iterator_tag)
    {
        if(first1 == last1 || first2 == last2)
            return last1;
        else
        {
            SearchedIterator result = last1;
            while(1)
            {
                SearchedIterator new_result = search(first1, last1, first2, last2);
                if(new_result == last1)
                    return result;
                else
                {
                    result = new_result;
                    first1 = new_result;
                    ++first1;
                }
            }
        }
    }
    
    template<typename SearchedIterator, typename InputIterator>
    inline SearchedIterator __find_end(SearchedIterator first1, SearchedIterator last1, InputIterator first2, InputIterator last2, bidirectional_iterator_tag, bidirectional_iterator_tag)
    {
        typedef reverse_iterator<SearchedIterator> RevSearchedIterator;
        typedef reverse_iterator<InputIterator> RevInputIterator;
        RevSearchedIterator rlast1(first1);
        RevInputIterator rlast2(first2);
        RevSearchedIterator r_reuslt = search(RevSearchedIterator(last1), rlast1, RevInputIterator(last2), rlast2);
        if(r_reuslt == rlast1)
            return last1;
        else
        {
            SearchedIterator result = r_reuslt.base();
            advance(result, -(last2 - first2));
            return result;
        }
    }
    
    template<typename SearchedIterator, typename InputIterator>
    inline SearchedIterator find_end(SearchedIterator first1, SearchedIterator last1, InputIterator first2, InputIterator last2)
    {
        return __find_end(first1, last1, first2, last2, iterator_category(first1), iterator_category(first2));
    }
    
    
    /////////////////////////////////////////////
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
