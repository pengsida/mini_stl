//
//  vector_psd.cpp
//  mini_stl
//
//  Created by pengsida on 16/8/30.
//  Copyright © 2016年 pengsida. All rights reserved.
//

#include "allocator_psd.h"
#include "reverse_iterator_psd.h"
#include "algorithm_psd.h"

namespace mini_stl
{
    template<typename T, typename Alloc = alloc>
    class vector
    {
    public:
        typedef T value_type;
        typedef T& reference;
        typedef const T& const_reference;
        typedef T* pointer;
        typedef const T* const_pointer;
        typedef T* iterator;
        typedef const T* const_iterator;
        typedef reverse_iterator<const_iterator> const_reverse_iterator;
        typedef reverse_iterator<iterator> reverse_iterator;
        typedef ptrdiff_t difference_type;
        typedef size_t size_type;
        
    private:
        typedef vector<T, Alloc> self;
        pointer start;
        pointer finish;
        pointer end_of_storage;
        typedef allocator<value_type, Alloc> data_allocator;
        
    private:
        void initialize(size_type count, const value_type& value);
        
    public:
        // constructor
        vector():start(0),finish(0),end_of_storage(0){}
        explicit vector(size_type count, const value_type& value);
        explicit vector(size_type count);
        explicit vector(const self& rhs);
        self& operator=(const self& rhs);
        
        // destructor
        ~vector();
        
        // Iterator
        iterator begin();
        iterator end();
        reverse_iterator rbegin();
        reverse_iterator rend();
        const_iterator cbegin() const;
        const_iterator cend() const;
        const_reverse_iterator crbegin() const;
        const_reverse_iterator crend() const;
        
        // Capacity
        size_type size() const;
        size_type max_size() const;
        void resize(size_type new_sz, const value_type& value = value_type());
        size_type capacity() const;
        bool empty() const;
        void reserve(size_type n);
        
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
        void push_back(const value_type& v);
        void pop_back();
        iterator insert(iterator position, const value_type& v);
        iterator insert(iterator position, const_iterator first, const_iterator last);
        iterator erase(iterator position);
        iterator erase(iterator first, iterator last);
        void swap();
        void clear();
    };
    
    //////////////////////////////////////
    // constructor
    
    template<typename T, typename Alloc>
    void vector<T,Alloc>::initialize(size_type count, const value_type& value)
    {
        start = data_allocator::allocate(count);
        end_of_storage = start + count;
        finish = uninitialized_fill_n(start, count, value);
    }
    
    template<typename T, typename Alloc>
    vector<T,Alloc>::vector(size_type count, const value_type& value)
    {
        initialize(count, value);
    }
    
    template<typename T, typename Alloc>
    vector<T,Alloc>::vector(size_type count)
    {
        initialize(count, value_type());
    }
    
    template<typename T, typename Alloc>
    vector<T,Alloc>::vector(const self& rhs)
    {
        start = data_allocator::allocate(rhs.size());
        finish = uninitialized_copy(rhs.begin(), rhs.end(), start);
    }
    
    template<typename T, typename Alloc>
    typename vector<T,Alloc>::self& vector<T,Alloc>::operator=(const self& rhs)
    {
        if(&rhs != this)
        {
            size_type rhs_size = rhs.size();
            if(rhs_size > capacity())
            {
                destroy(start, finish);
                data_allocator::deallocate(start, end_of_storage - start);
                start = data_allocator::allocate(rhs_size);
                uninitialized_copy(rhs.begin(), rhs.end(), start);
                end_of_storage = start + rhs_size;
            }
            else if(rhs_size > size())
                copy(rhs.begin(), rhs.end(), start);
            else
            {
                iterator pos = copy(rhs.begin(), rhs.end(), begin());
                destroy(pos, end());
            }
            finish = start + rhs_size;
        }
        return *this;
    }
    
    ///////////////////////////////////////
    // destructor
    
    template<typename T, typename Alloc>
    vector<T,Alloc>::~vector()
    {
        destroy(begin(), end());
        data_allocator::deallocate(begin(), end_of_storage - start);
    }
    
    ///////////////////////////////////////
    // Iterator
    
    template<typename T, typename Alloc>
    typename vector<T,Alloc>::iterator vector<T,Alloc>::begin()
    {
        return start;
    }
    
    template<typename T, typename Alloc>
    typename vector<T,Alloc>::iterator vector<T,Alloc>::end()
    {
        return finish;
    }
    
    template<typename T, typename Alloc>
    typename vector<T,Alloc>::reverse_iterator vector<T,Alloc>::rbegin()
    {
        return reverse_iterator(end());
    }
    
    template<typename T, typename Alloc>
    typename vector<T,Alloc>::reverse_iterator vector<T,Alloc>::rend()
    {
        return reverse_iterator(begin());
    }
    
    template<typename T, typename Alloc>
    typename vector<T,Alloc>::const_iterator vector<T,Alloc>::cbegin() const
    {
        return start;
    }
    
    template<typename T, typename Alloc>
    typename vector<T,Alloc>::const_iterator vector<T,Alloc>::cend() const
    {
        return finish;
    }
    
    template<typename T, typename Alloc>
    typename vector<T,Alloc>::const_reverse_iterator vector<T,Alloc>::crbegin() const
    {
        return const_reverse_iterator(cbegin());
    }
    
    template<typename T, typename Alloc>
    typename vector<T,Alloc>::const_reverse_iterator vector<T,Alloc>::crend() const
    {
        return const_reverse_iterator(cend());
    }
    
    template<typename T, typename Alloc>
    typename vector<T,Alloc>::size_type vector<T,Alloc>::size() const
    {
        return size_type(finish - start);
    }
    
    template<typename T, typename Alloc>
    typename vector<T,Alloc>::size_type vector<T,Alloc>::max_size() const
    {
        return size_type(-1) / sizeof(value_type);
    }
    
    template<typename T, typename Alloc>
    void vector<T,Alloc>::resize(size_type new_sz, const value_type& value)
    {
        if(new_sz > capacity())
            reserve();
        if(new_sz > size())
            uninitialized_fill_n(finish, new_sz - size(), value);
        else
            destroy(start + new_sz, finish);
        finish = start + new_sz;
    }
    
    template<typename T, typename Alloc>
    typename vector<T,Alloc>::size_type vector<T,Alloc>::capacity() const
    {
        return end_of_storage - start;
    }
    
    template<typename T, typename Alloc>
    bool vector<T,Alloc>::empty() const
    {
        return finish == start;
    }
    
    // 要求vector有n的空间大小
    template<typename T, typename Alloc>
    void vector<T,Alloc>::reserve(size_type n)
    {
        if(capacity() < n)
        {
            iterator new_start = data_allocator::allocate(n);
            iterator new_finish = uninitialized_copy(begin(), end(), new_start);
            destroy(begin(), end());
            data_allocator::deallocate(start, capacity());
            start = new_start;
            finish = new_finish;
            end_of_storage = start + n;
        }
    }
    
    template<typename T, typename Alloc>
    typename vector<T,Alloc>::reference vector<T,Alloc>::operator[](size_type index)
    {
        return *(begin() + index);
    }
    
    template<typename T, typename Alloc>
    typename vector<T,Alloc>::const_reference vector<T,Alloc>::operator[](size_type index) const
    {
        return *(begin() + index);
    }
    
    template<typename T, typename Alloc>
    typename vector<T,Alloc>::reference vector<T,Alloc>::at(size_type index)
    {
        return operator[](index);
    }
    
    template<typename T, typename Alloc>
    typename vector<T,Alloc>::const_reference vector<T,Alloc>::at(size_type index) const
    {
        return operator[](index);
    }
    
    template<typename T, typename Alloc>
    typename vector<T,Alloc>::reference vector<T,Alloc>::front()
    {
        return *begin();
    }
    
    template<typename T, typename Alloc>
    typename vector<T,Alloc>::const_reference vector<T,Alloc>::front() const
    {
        return *begin();
    }
    
    template<typename T, typename Alloc>
    typename vector<T,Alloc>::reference vector<T,Alloc>::back()
    {
        return *--end();
    }
    
    template<typename T, typename Alloc>
    typename vector<T,Alloc>::const_reference vector<T,Alloc>::back() const
    {
        return *--end();
    }
    
    template<typename T, typename Alloc>
    void vector<T,Alloc>::push_back(const value_type& v)
    {
        if(end_of_storage == finish)
            reserve(capacity() * 2);
        construct(finish, v);
        finish++;
    }
    
    template<typename T, typename Alloc>
    void vector<T,Alloc>::pop_back()
    {
        --finish;
        destroy(finish);
    }
    
    // insert会返回插入位置的迭代器
    template<typename T, typename Alloc>
    typename vector<T,Alloc>::iterator vector<T,Alloc>::insert(iterator position, const value_type& v)
    {
        if(position == finish)
            push_back(v);
        else
        {
            size_type pos = position - begin();
            if(finish == end_of_storage)
                reserve(capacity() * 2);
            position = begin() + pos;
            copy_backward(position - 1, end() - 1, end());
            construct(position, v);
            finish++;
        }
        return position;
    }
    
    // 在position处插入[first, last)的元素
    template<typename T, typename Alloc>
    typename vector<T,Alloc>::iterator vector<T,Alloc>::insert(iterator position, const_iterator first, const_iterator last)
    {
        
    }
    
    template<typename T, typename Alloc>
    typename vector<T,Alloc>::iterator vector<T,Alloc>::erase(iterator position)
    {
        if(position + 1 != end())
            copy(position + 1, end(), position);
        pop_back();
        return position;
    }
    
    template<typename T, typename Alloc>
    typename vector<T,Alloc>::iterator vector<T,Alloc>::erase(iterator first, iterator last)
    {
        size_type len = last - first;
        iterator position = copy(last, end(), first);
        destroy(position, end());
        finish = finish - len;
        return first;
    }
    
    template<typename T, typename Alloc>
    void vector<T,Alloc>::swap()
    {
        //...
    }
    
    template<typename T, typename Alloc>
    void vector<T,Alloc>::clear()
    {
        
    }
}
















































