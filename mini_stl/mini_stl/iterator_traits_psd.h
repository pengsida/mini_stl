//
//  iterator_traits_psd.h
//  mini_stl
//
//  Created by pengsida on 16/8/28.
//  Copyright © 2016年 pengsida. All rights reserved.
//

#ifndef iterator_traits_psd_h
#define iterator_traits_psd_h

#include <cstddef>

namespace mini_stl
{
    struct input_iterator_tag{};
    struct output_iterator_tag{};
    // forward_iterator_tag只能向前移动， 如 ++iterator
    struct forward_iterator_tag : public input_iterator_tag{};
    // bidirectional_iterator_tag可以双向移动，如 --iterator, ++iterator
    struct bidirectional_iterator_tag : public forward_iterator_tag{};
    // random_access_iterator_tag可以进行跳跃，如 iterator += n
    struct random_access_iterator_tag : public bidirectional_iterator_tag{};
    
    template<typename Iterator>
    struct iterator_traits
    {
        typedef typename Iterator::iterator_category iterator_category;
        typedef typename Iterator::value_type value_type;
        typedef typename Iterator::pointer pointer;
        typedef typename Iterator::reference reference;
        typedef typename Iterator::difference_type difference_type;
    };
    
    template<typename T>
    struct iterator_traits<T*>
    {
        typedef random_access_iterator_tag iterator_category;
        typedef T value_type;
        typedef T* pointer;
        typedef T& reference;
        typedef ptrdiff_t difference_type;
    };
    
    template<typename T>
    struct iterator_traits<const T*>
    {
        typedef random_access_iterator_tag iterator_category;
        typedef T value_type;
        typedef const T* pointer;
        typedef const T& reference;
        typedef ptrdiff_t difference_type;
    };
    
    template<typename Iterator>
    inline typename iterator_traits<Iterator>::difference_type* distance_type(const Iterator&)
    {
        return static_cast<typename iterator_traits<Iterator>::difference_type*>(0);
    }
    
    template<typename Iterator>
    inline typename iterator_traits<Iterator>::value_type* __value_type(const Iterator&)
    {
        return static_cast<typename iterator_traits<Iterator>::value_type*>(0);
    }
    
    template<typename Iterator>
    inline typename iterator_traits<Iterator>::iterator_category iterator_category(const Iterator&)
    {
        typedef typename iterator_traits<Iterator>::iterator_category category;
        return category();
    }
    
    template<typename InputIterator, typename difference_type>
    inline void __advance(InputIterator& itr, difference_type n, forward_iterator_tag)
    {
        while(n--)
            ++itr;
    }
    
    template<typename InputIterator, typename difference_type>
    inline void __advance(InputIterator& itr, difference_type n, bidirectional_iterator_tag)
    {
        if(n > 0)
            while(n--)
                ++itr;
        else
            while(n++)
                --itr;
    }
    
    template<typename InputIterator, typename difference_type>
    inline void __advance(InputIterator& itr, difference_type n, random_access_iterator_tag)
    {
        itr += n;
    }
    
    template<typename InputIterator, typename difference_type>
    inline void advance(InputIterator& itr, difference_type n)
    {
        __advance(itr, n, iterator_category(itr));
    }
}

#endif /* iterator_traits_psd_h */

















