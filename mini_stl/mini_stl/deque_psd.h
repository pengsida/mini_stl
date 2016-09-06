//
//  deque_psd.h
//  mini_stl
//
//  Created by pengsida on 16/9/6.
//  Copyright © 2016年 pengsida. All rights reserved.
//

#ifndef deque_psd_h
#define deque_psd_h

#include "allocator_psd.h"
#include "algorithm_psd.h"
#include "reverse_iterator_psd.h"

#define DefaultSize 512

namespace mini_stl
{
    template<typename size_type>
    inline size_type get_buffer_size(size_type n, size_type size_of_data)
    {
        return (n == 0 ? (size_of_data < DefaultSize ? 1 : size_type(DefaultSize/size_of_data)) : n);
    }
    
    // 这是一个random_access_iterator类型的迭代器
    template<typename T, typename Ref, typename Ptr, size_t BufSiz>
    struct iterator
    {
        typedef random_access_iterator_tag iterator_category;
        typedef T value_type;
        typedef Ref reference;
        typedef Ptr pointer;
        typedef size_t size_type;
        typedef ptrdiff_t difference_type;
        typedef T** map_pointer;
        
        static const size_type buffer_size;
        // 指向缓冲区地址的指针
        map_pointer node;
        // 用来指出缓冲区范围的指针
        pointer first;
        pointer last;
        // 指向数据地址的指针
        pointer cur_pos;
        
        iterator():node(NULL),first(NULL),cur_pos(NULL),last(NULL){}
        explicit iterator(map_pointer node, pointer pos):node(node),cur_pos(pos){}
        iterator(const iterator& rhs): node(rhs.node),first(rhs.first),cur_pos(rhs.cur_pos),last(rhs.last){}
        ~iterator()
        {
            node = NULL;
            first = NULL;
            last = NULL;
            cur_pos = NULL;
        }
        iterator& operator=(const iterator& rhs)
        {
            node = rhs.node;
            first = rhs.first;
            last = rhs.last;
            cur_pos = rhs.cur_pos;
            return *this;
        }
        
        void change_node(map_pointer new_node)
        {
            node = new_node;
            first = *node;
            last = first + difference_type(buffer_size);
        }
        
        reference operator*() const{return *cur_pos;}
        pointer operator->() const{return &(operator*());}
        
        iterator& operator++()
        {
            ++cur_pos;
            if(cur_pos == last)
            {
                change_node(node + 1);
                cur_pos = first;
            }
            return *this;
        }
        
        iterator operator++(int)
        {
            iterator old = *this;
            ++(*this);
            return old;
        }
        
        iterator& operator--()
        {
            if(cur_pos == first)
            {
                change_node(node - 1);
                cur_pos = last;
            }
            --cur_pos;
            return *this;
        }
        
        iterator operator--(int)
        {
            iterator old = *this;
            --(*this);
            return old;
        }
        
        iterator& operator+=(difference_type n)
        {
            //...
            return *this;
        }
        
        iterator operator+(difference_type n)
        {
            iterator result = *this;
            //...
            return result;
        }
        
        iterator& operator-=(difference_type n)
        {
            return (*this += (-n));
        }
        
        iterator operator-(difference_type n)
        {
            return (*this + (-n));
        }
        
        bool operator==(const iterator& rhs) const{return ((node == rhs.node) ? (cur_pos == rhs.cur_pos) : false);}
        bool operator!=(const iterator& rhs) const{return !(*this == rhs);}
        bool operator<(const iterator& rhs) const{return ((node == rhs.node) ? (cur_pos < rhs.cur_pos) : (node < rhs.node));}
        bool operator<=(const iterator& rhs) const{return !(rhs < *this);}
        bool operator>(const iterator& rhs) const{return rhs < *this;}
        bool operator>=(const iterator& rhs) const{return !(*this < rhs);}
        
        reference operator[](size_type index) const{return *(*this + index);}
    };
    
    template<typename T, typename Ref, typename Ptr, size_t BufSiz>
    const typename iterator<T, Ref, Ptr, BufSiz>::size_type iterator<T, Ref, Ptr, BufSiz>::buffer_size = get_buffer_size(BufSiz, sizeof(T));
    
    template<typename T, typename Alloc = alloc, size_t BufSiz = 8>
    class deque
    {
    public:
        typedef T value_type;
        typedef T& reference;
        typedef const T& const_reference;
        typedef T* pointer;
        typedef const T* const_pointer;
        typedef iterator<T, const T&, const T*, BufSiz> const_iterator;
        typedef iterator<T, T&, T*, BufSiz> iterator;
        typedef reverse_iterator<const_iterator> const_reverse_iterator;
        typedef reverse_iterator<iterator> reverse_iterator;
        typedef ptrdiff_t difference_type;
        typedef size_t size_type;
        
    private:
        typedef allocator<T, Alloc> data_allocator;
        
    public:
        // constructor
        deque(){}
        explicit deque(size_type n, const value_type& v = value_type());
        
        template<typename InputIterator>
        deque(InputIterator first, InputIterator last);
        
        deque(const deque& rhs);
        
        // destructor
        ~deque();
        
        // operator=
        deque& operator=(const deque& rhs);
        
        // Iterators
        iterator begin();
        const_iterator begin() const;
        iterator end();
        const_iterator end() const;
        reverse_iterator rbegin();
        const_reverse_iterator rbegin() const;
        reverse_iterator rend();
        const_reverse_iterator rend() const;
        const_iterator cbegin() const;
        const_iterator cend() const;
        const_reverse_iterator crbegin() const;
        const_reverse_iterator crend() const;
        
        // Capacity
        size_type size() const;
        size_type max_size() const;
        void resize(size_type n);
        void resize(size_type n, const value_type& v);
        bool empty() const;
        
        // Element access
        reference operator[](size_type index);
        const_reference operator[](size_type index) const;
        reference at(size_type index);
        const_reference at(size_type index) const;
        reference front();
        const_reference front() const;
        reference back();
        const_reference back() const;
        
        // Modifiers
        void assign(size_type n, const value_type& v);
        template<typename InputIterator>
        void assign(InputIterator first, InputIterator last);
        
        void push_back(const value_type& v);
        void push_front(const value_type& v);
        void pop_back();
        void pop_front();
        
        iterator insert(iterator position, const value_type& v);
        void insert(iterator position, size_type n, const value_type& v);
        template<typename InputIterator>
        void insert(iterator position, InputIterator first, InputIterator last);
        
        iterator erase(iterator position);
        iterator erase(iterator first, iterator last);
        
        void swap(deque& rhs);
        void clear();
    };
    
    // Non-member functions overloads
    template<typename T, typename Alloc>
    bool operator==(const deque<T,Alloc>& lhs, const deque<T,Alloc>& rhs)
    {
        //...
        return false;
    }
    
    template<typename T, typename Alloc>
    bool operator!=(const deque<T,Alloc>& lhs, const deque<T,Alloc>& rhs)
    {
        return !(lhs == rhs);
    }
    
    template<typename T, typename Alloc>
    bool operator<(const deque<T,Alloc>& lhs, const deque<T,Alloc>& rhs)
    {
        //...
        return false;
    }
    
    template<typename T, typename Alloc>
    bool operator<=(const deque<T,Alloc>& lhs, const deque<T,Alloc>& rhs)
    {
        return !(rhs < lhs);
    }
    
    template<typename T, typename Alloc>
    bool operator>(const deque<T,Alloc>& lhs, const deque<T,Alloc>& rhs)
    {
        return rhs < lhs;
    }
    
    template<typename T, typename Alloc>
    bool operator>=(const deque<T,Alloc>& lhs, const deque<T,Alloc>& rhs)
    {
        return !(lhs < rhs);
    }
    
    template<typename T, typename Alloc>
    void swap(deque<T,Alloc>& lhs, deque<T,Alloc>& rhs)
    {
        lhs.swap(rhs);
    }
}

#endif /* deque_psd_h */
