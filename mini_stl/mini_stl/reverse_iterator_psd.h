//
//  reverse_iterator_psd.h
//  mini_stl
//
//  Created by pengsida on 16/8/30.
//  Copyright © 2016年 pengsida. All rights reserved.
//

#ifndef reverse_iterator_psd_h
#define reverse_iterator_psd_h

#include "iterator_traits_psd.h"

namespace mini_stl
{
    template<typename Iterator>
    class reverse_iterator
    {
    private:
        Iterator cur_pos;
        
    public:
        typedef typename iterator_traits<Iterator>::iterator_category iterator_category;
        typedef typename iterator_traits<Iterator>::value_type value_type;
        typedef typename iterator_traits<Iterator>::difference_type difference_type;
        typedef typename iterator_traits<Iterator>::pointer pointer;
        typedef typename iterator_traits<Iterator>::reference reference;
        
    private:
        typedef reverse_iterator<Iterator> self;
        
    public:
        reverse_iterator(){}
        explicit reverse_iterator(const Iterator& rhs);
        explicit reverse_iterator(const self& rhs);
        
//        Returns a reference or pointer to the element previous to current.
        reference operator*() const;
        pointer operator->() const;
        
        // operator overload
        self& operator++();
        self operator++(int);
        self& operator--();
        self operator--(int);
        self operator+(difference_type n) const;
        self& operator+=(difference_type n);
        self operator-(difference_type n) const;
        self& operator-=(difference_type n);
        
        // index
        reference operator[](difference_type n) const;
        
        // compare
        bool operator==(const self& rhs);
        bool operator!=(const self& rhs);
        bool operator<(const self& rhs);
        bool operator>(const self& rhs);
        bool operator<=(const self& rhs);
        bool operator>=(const self& rhs);
        
        // subtract
        difference_type operator-(const self& rhs) const;
        
        // friend function
        friend Iterator operator+(difference_type n, const self& rhs);
    };
    
    template<typename Iterator>
    reverse_iterator<Iterator>::reverse_iterator(const Iterator& rhs)
    :cur_pos(rhs)
    {
    }

    template<typename Iterator>
    reverse_iterator<Iterator>::reverse_iterator(const self& rhs)
    :cur_pos(rhs.cur_pos)
    {
    }
    
    template<typename Iterator>
    typename reverse_iterator<Iterator>::reference reverse_iterator<Iterator>::operator*() const
    {
        // 返回当前元素前一元素的值
        // 比如应用于end()的情况
        Iterator tmp = cur_pos;
        return *--tmp;
    }
    
    template<typename Iterator>
    typename reverse_iterator<Iterator>::pointer reverse_iterator<Iterator>::operator->() const
    {
        return &(operator*());
    }
    
    template<typename Iterator>
    reverse_iterator<Iterator>& reverse_iterator<Iterator>::operator++()
    {
        --cur_pos;
        return *this;
    }
    
    template<typename Iterator>
    reverse_iterator<Iterator> reverse_iterator<Iterator>::operator++(int)
    {
        self tmp = *this;
        --cur_pos;
        return tmp;
    }
    
    template<typename Iterator>
    reverse_iterator<Iterator>& reverse_iterator<Iterator>::operator--()
    {
        ++cur_pos;
        return *this;
    }
    
    template<typename Iterator>
    reverse_iterator<Iterator> reverse_iterator<Iterator>::operator--(int)
    {
        self tmp = *this;
        ++cur_pos;
        return tmp;
    }
    
    template<typename Iterator>
    reverse_iterator<Iterator> reverse_iterator<Iterator>::operator+(difference_type n) const
    {
        return self(cur_pos - n);
    }
    
    template<typename Iterator>
    reverse_iterator<Iterator>& reverse_iterator<Iterator>::operator+=(difference_type n)
    {
        cur_pos -= n;
        return *this;
    }
    
    template<typename Iterator>
    reverse_iterator<Iterator> reverse_iterator<Iterator>::operator-(difference_type n) const
    {
        return self(cur_pos + n);
    }
    
    template<typename Iterator>
    reverse_iterator<Iterator>& reverse_iterator<Iterator>::operator-=(difference_type n)
    {
        cur_pos += n;
        return *this;
    }
    
    template<typename Iterator>
    typename reverse_iterator<Iterator>::reference reverse_iterator<Iterator>::operator[](difference_type n) const
    {
        return *(*this + n);
    }
    
    template<typename Iterator>
    bool reverse_iterator<Iterator>::operator==(const self& rhs)
    {
        return cur_pos == rhs.cur_pos;
    }
    
    template<typename Iterator>
    bool reverse_iterator<Iterator>::operator!=(const self& rhs)
    {
        return !(*this == rhs);
    }
    
    template<typename Iterator>
    bool reverse_iterator<Iterator>::operator<(const self& rhs)
    {
        return cur_pos < rhs.cur_pos;
    }
    
    template<typename Iterator>
    bool reverse_iterator<Iterator>::operator>(const self& rhs)
    {
        return rhs.cur_pos < cur_pos;
    }
    
    template<typename Iterator>
    bool reverse_iterator<Iterator>::operator<=(const self& rhs)
    {
        return !(rhs < *this);
    }
    
    template<typename Iterator>
    bool reverse_iterator<Iterator>::operator>=(const self& rhs)
    {
        return !(*this < rhs);
    }
    
    template<typename Iterator>
    typename reverse_iterator<Iterator>::difference_type reverse_iterator<Iterator>::operator-(const self& rhs) const
    {
        return cur_pos - rhs.cur_pos;
    }
    
    template<typename Iterator, typename DistanceType>
    reverse_iterator<Iterator> operator+(DistanceType n, const reverse_iterator<Iterator>& rhs)
    {
        return reverse_iterator<Iterator>(n + rhs.cur_pos);
    }
    
    template<typename RandomAccessIterator>
    inline random_access_iterator_tag iterator_category(const reverse_iterator<RandomAccessIterator>&)
    {
        return random_access_iterator_tag();
    }
    
    template<typename Iterator>
    inline typename iterator_traits<Iterator>::value_type* __value_type(const reverse_iterator<Iterator>&)
    {
        return static_cast<typename iterator_traits<Iterator>::value_type *>(0);
    }
    
    template<typename Iterator>
    inline typename iterator_traits<Iterator>::difference_type* distance_type(const reverse_iterator<Iterator>&)
    {
        return static_cast<typename iterator_traits<Iterator>::difference_type *>(0);
    }
}

#endif /* reverse_iterator_psd_h */
