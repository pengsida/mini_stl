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
    struct forward_iterator_tag : public input_iterator_tag{};
    struct bidirectional_iterator_tag : public forward_iterator_tag{};
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
    inline typename iterator_traits<Iterator>::value_type* __value_type(const Iterator&)
    {
        return static_cast<typename iterator_traits<Iterator>::value_type*>(0);
    }
}

#endif /* iterator_traits_psd_h */
