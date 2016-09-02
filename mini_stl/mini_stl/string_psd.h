//
//  string_psd.h
//  mini_stl
//
//  Created by pengsida on 16/9/1.
//  Copyright © 2016年 pengsida. All rights reserved.
//

#ifndef string_psd_h
#define string_psd_h

#include "allocator_psd.h"
#include "reverse_iterator_psd.h"
#include "algorithm_psd.h"
#include <istream>
#include <ostream>

//因为reverse_iterator在编译期间就需要确认构造函数类型
//如果typedef reverse_iterator<char*> reverse_iterator的话，
//reverse_iterator是没有相应的构造函数的
//所以如果为了在string在使用reverse_iterator，就需要对string进行模板化
//为了合乎string针对字符串的一般用法，我先定义basic_string类
//然后 typedef basic_string<char> string

// 函数参数中的pos都是指*this中的位置

namespace mini_stl
{
    template<typename T>
    class basic_string
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
        typedef allocator<T, alloc> data_allocator;
        typedef basic_string<T> self;
        static const size_type npos;
        pointer start;
        pointer finish;
        pointer end_of_storage;
        
    public:
        // constructor
        basic_string():start(0),finish(0),end_of_storage(0){}
        explicit basic_string(const self& rhs);
        
        self& operator=(const self& rhs);
        self& operator=(const value_type* s);
        self& operator=(char c);
        
        // destructor
        ~basic_string(){}
        
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
        size_type length() const;
        size_type max_size() const;
        void resize(size_type new_sz, value_type c);
        void resize(size_type new_sz);
        size_type capacity() const;
        void reserve(size_type n);
        void clear();
        bool empty() const;
        
        // Element access
        reference operator[](size_type index);
        const_reference operator[](size_type index) const;
        reference at(size_type index);
        const_reference at(size_type index) const;
        reference back();
        const_reference back() const;
        reference front();
        const_reference front() const;
        
        ///////////////////////////////////////////////////
        // Modifiers
        
        self& operator+=(const self& rhs);
        self& operator+=(const value_type* s);
        self& operator+=(value_type c);
        
        self& append(const self& rhs);
        self& append(const self& rhs, size_type start_pos, size_type len = npos);
        self& append(const value_type* s);
        self& append(const value_type* s, size_type n);
        self& append(size_type n, value_type c);
        template<typename InputIterator>
        self& append(InputIterator first, InputIterator last);
        
        void push_back(value_type c);
        
        // Assigns a new value to the string, replacing its current contents.
        self& assign(const self& rhs);
        self& assign(const self& rhs, size_type start_pos, size_type len = npos);
        self& assign(const value_type* s);
        self& assign(const value_type* s, size_type n);
        self& assign(size_type n, value_type c);
        template<typename InputIterator>
        self& assign(InputIterator first, InputIterator last);
        
        self& insert(size_type pos, const self& rhs);
        self& insert(size_type pos, const self& rhs, size_type start_pos, size_type len = npos);
        self& insert(size_type pos, const value_type* s);
        self& insert(size_type pos, const value_type* s, size_type len);
        self& insert(size_type pos, size_type n, value_type c);
        self& insert(iterator position, size_type n, value_type c);
        self& insert(iterator position, value_type c);
        template<typename InputIterator>
        self& insert(iterator position, InputIterator first, InputIterator last);
        
        iterator erase(size_type pos, size_type len = npos);
        iterator erase(iterator position);
        iterator erase(iterator first, iterator last);
        
        // Replaces the portion of the string that begins at character pos and spans len characters (or the part of the string in the range between [first,last)) by new contents
        self& replace(size_type pos, size_type len, const self& rhs);
        self& replace(iterator first, iterator last, const self& rhs);
        self& replace(size_type pos, size_type len, const self& rhs, size_type start_pos, size_type replace_len);
        self& replace(size_type pos, size_type len, const value_type* s);
        self& replace(iterator first, iterator last, const value_type* s);
        self& replace(size_type pos, size_type len, const value_type* s, size_type replace_len);
        self& replace(iterator first, iterator last, const value_type* s, size_type replace_len);
        self& replace(size_type pos, size_type len, size_type replace_len, value_type c);
        self& replace(iterator first, iterator last, size_type replace_len, value_type c);
        template<typename InputIterator>
        self& replace(iterator first, iterator last, InputIterator replace_first, InputIterator replace_last);
        
        void swap(self& rhs);
        void pop_back();
        
        //////////////////////////////////////////////////////////////
        // String operations
        const char* c_str() const;
        const char* data() const;
        size_type copy(char* dest, size_type len, size_type pos = 0);
        
        // searching range: [pos, end)
        size_type find(const self& str, size_type pos) const;
        size_type find(const value_type* s, size_type pos) const;
        size_type find(const value_type* s, size_type pos, size_type len);
        size_type find(value_type c, size_type pos) const;
        
        // searching range: (0, pos]
        size_type rfind(const self& str, size_type pos) const;
        size_type rfind(const value_type* s, size_type pos) const;
        size_type rfind(const value_type* s, size_type pos, size_type len) const;
        size_type rfind(value_type c, size_type pos) const;
        
        size_type find_first_of(const self& str, size_type pos = 0) const;
        size_type find_first_of(const value_type* s, size_type pos = 0) const;
        size_type find_first_of(const value_type* s, size_type pos, size_type len) const;
        size_type find_first_of(char c, size_type pos = 0) const;
        
        // Find character in string from the end
        size_type find_last_of(const self& str, size_type pos = npos) const;
        size_type find_last_of(const value_type* s, size_type pos = npos) const;
        size_type find_last_of(const value_type* s, size_type pos, size_type len) const;
        size_type find_last_of(value_type c, size_type pos = npos) const;
        
        size_type find_first_not_of(const self& str, size_type pos = 0) const;
        size_type find_first_not_of(const value_type* s, size_type pos = 0) const;
        size_type find_first_not_of(const value_type* s, size_type pos, size_type len) const;
        size_type find_first_not_of(value_type c, size_type pos = 0) const;
        
        size_type find_last_not_of(const self& str, size_type pos = npos) const;
        size_type find_last_not_of(const value_type* s, size_type pos = npos) const;
        size_type find_last_not_of(const value_type* s, size_type pos, size_type len) const;
        size_type find_last_not_of(value_type c, size_type pos = npos) const;
        
        self substr(size_type pos = 0, size_type len = npos);
        
        int compare(const self& str) const;
        int compare(size_type pos, size_type len, const self& str) const;
        int compare(size_type pos, size_type len, const self& str, size_type compare_pos, size_type compare_len) const;
        int compare(const value_type* s) const;
        int compare(size_type pos, size_type len, const value_type* s) const;
        int compare(size_type pos, size_type len, const value_type* s, size_type n) const;
    };
    
    template<typename T>
    const size_t basic_string<T>::npos = -1;
    
    template<typename T>
    typename basic_string<T>::iterator basic_string<T>::begin()
    {
        return start;
    }
    
    template<typename T>
    typename basic_string<T>::iterator basic_string<T>::end()
    {
        return finish;
    }
    
    template<typename T>
    typename basic_string<T>::reverse_iterator basic_string<T>::rbegin()
    {
        return reverse_iterator(end());
    }
    
    template<typename T>
    typename basic_string<T>::reverse_iterator basic_string<T>::rend()
    {
        return reverse_iterator(begin());
    }
    
    template<typename T>
    typename basic_string<T>::const_iterator basic_string<T>::cbegin() const
    {
        return start;
    }
    
    template<typename T>
    typename basic_string<T>::const_iterator basic_string<T>::cend() const
    {
        return finish;
    }
    
    template<typename T>
    typename basic_string<T>::const_reverse_iterator basic_string<T>::crbegin() const
    {
        return const_reverse_iterator(cend());
    }
    
    template<typename T>
    typename basic_string<T>::const_reverse_iterator basic_string<T>::crend() const
    {
        return const_reverse_iterator(cbegin());
    }
    
    template<typename T>
    typename basic_string<T>::size_type basic_string<T>::size() const
    {
        return finish - start;
    }
    
    template<typename T>
    typename basic_string<T>::size_type basic_string<T>::length() const
    {
        return finish - start;
    }
    
    template<typename T>
    typename basic_string<T>::size_type basic_string<T>::max_size() const
    {
        return size_type(-1) / sizeof(T);
    }

    template<typename T>
    void basic_string<T>::resize(size_type new_sz, value_type c)
    {
        if(new_sz > capacity())
            reserve(new_sz);
        if(new_sz < size())
            destroy(start + new_sz, finish);
        else
            fill(finish, start + new_sz, c);
        finish = start + new_sz;
    }
    
    template<typename T>
    void basic_string<T>::resize(size_type new_sz)
    {
        resize(new_sz, value_type());
    }
    
    template<typename T>
    typename basic_string<T>::size_type basic_string<T>::capacity() const
    {
        return end_of_storage - start;
    }
    
    template<typename T>
    void basic_string<T>::reserve(size_type n)
    {
        if(end_of_storage - start < n)
        {
            const size_type old_capacity = capacity();
            const size_type new_capacity = old_capacity * 2 > n ? old_capacity * 2 : n;
            pointer new_start = data_allocator::allocate(new_capacity);
            pointer new_finish = uninitialized_copy(start, finish, new_start);
            destroy(start, finish);
            data_allocator::deallocate(start, end_of_storage - start);
            start = new_start;
            finish = new_finish;
            end_of_storage = start + new_capacity;
        }
    }
    
    template<typename T>
    void basic_string<T>::clear()
    {
        destroy(start, finish);
        finish = start;
    }
    
    template<typename T>
    bool basic_string<T>::empty() const
    {
        return finish == start;
    }
    
    template<typename T>
    typename basic_string<T>::reference basic_string<T>::operator[](size_type index)
    {
        return *(start + index);
    }
    
    template<typename T>
    typename basic_string<T>::const_reference basic_string<T>::operator[](size_type index) const
    {
        return *(start + index);
    }
    
    template<typename T>
    typename basic_string<T>::reference basic_string<T>::at(size_type index)
    {
        return *(start + index);
    }
    
    template<typename T>
    typename basic_string<T>::const_reference basic_string<T>::at(size_type index) const
    {
        return *(start + index);
    }
    
    template<typename T>
    typename basic_string<T>::reference basic_string<T>::back()
    {
        return *(finish - 1);
    }
    
    template<typename T>
    typename basic_string<T>::const_reference basic_string<T>::back() const
    {
        return *(finish - 1);
    }
    
    template<typename T>
    typename basic_string<T>::reference basic_string<T>::front()
    {
        return *start;
    }
    
    template<typename T>
    typename basic_string<T>::const_reference basic_string<T>::front() const
    {
        return *start;
    }
    
    
    
    
    //////////////////////////////////////////
    // operator+=()
    
    template<typename T>
    typename basic_string<T>::self& basic_string<T>::operator+=(const self& rhs)
    {
        const size_type num_add = rhs.size();
        if(end_of_storage - finish >= num_add)
            finish = uninitialized_copy(rhs.start, rhs.finish, finish);
        else
        {
            reserve(size() + num_add);
            finish = uninitialized_copy(rhs.start, rhs.finish, finish);
        }
        return *this;
    }
    
    template<typename T>
    typename basic_string<T>::self& basic_string<T>::operator+=(const value_type* s)
    {
        //...
        return *this;
    }
    
    template<typename T>
    typename basic_string<T>::self& basic_string<T>::operator+=(value_type c)
    {
        if(finish == end_of_storage)
            reserve(capacity() * 2);
        construct(finish, c);
        ++finish;
        return *this;
    }
    
    template<typename T>
    typename basic_string<T>::self& basic_string<T>::append(const self& rhs)
    {
        return *this += rhs;
    }
    
    
    
    
    ////////////////////////////////////
    // append
    
    // start_pos是rhs开始append的位置
    template<typename T>
    typename basic_string<T>::self& basic_string<T>::append(const self& rhs, size_type start_pos, size_type len)
    {
        if(start_pos < rhs.size())
        {
            pointer first = rhs.start + start_pos;
            pointer last;
            if(len > (rhs.finish - first) || len == npos)
                last = rhs.finish;
            else
                last = first + len;
            if((end_of_storage - finish) > (last - first))
                finish = uninitialized_copy(first, last, finish);
            else
            {
                reserve((end_of_storage - start) + (last - first));
                finish = uninitialized_copy(first, last, finish);
            }
        }
        return *this;
    }
    
    template<typename T>
    typename basic_string<T>::self& basic_string<T>::append(const value_type* s)
    {
        //...
        return *this;
    }
    
    // const value_type* s表示s是一个指针，它指向const value_type
    template<typename T>
    typename basic_string<T>::self& basic_string<T>::append(const value_type* s, size_type n)
    {
        //...
        return *this;
    }
    
    template<typename T>
    typename basic_string<T>::self& basic_string<T>::append(size_type n, value_type c)
    {
        if((end_of_storage - finish) >= n)
            finish = uninitialized_fill_n(finish, n, c);
        else
        {
            reserve((end_of_storage - start) + n);
            finish = uninitialized_fill_n(finish, n, c);
        }
        return *this;
    }
    
    // 以下函数实现方式要求InputIterator是random_access_iterator
    template<typename T>
    template<typename InputIterator>
    typename basic_string<T>::self& basic_string<T>::append(InputIterator first, InputIterator last)
    {
        if((end_of_storage - finish) >= (last - first))
            finish = uninitialized_copy(first, last, finish);
        else
        {
            reserve((end_of_storage - start) + (last - first));
            finish = uninitialized_copy(first, last, finish);
        }
        return *this;
    }
    
    template<typename T>
    void basic_string<T>::push_back(value_type c)
    {
        *this += c;
    }
    
    
    
    
    ///////////////////////////////////////
    // assign
    
    template<typename T>
    typename basic_string<T>::self& basic_string<T>::assign(const self& rhs)
    {
        clear();
        return append(rhs);
    }
    
    template<typename T>
    typename basic_string<T>::self& basic_string<T>::assign(const self& rhs, size_type start_pos, size_type len)
    {
        clear();
        return append(rhs, start_pos, len);
    }
    
    template<typename T>
    typename basic_string<T>::self& basic_string<T>::assign(const value_type* s)
    {
        clear();
        return append(s);
    }
    
    template<typename T>
    typename basic_string<T>::self& basic_string<T>::assign(const value_type* s, size_type n)
    {
        clear();
        return append(s, n);
    }
    
    template<typename T>
    typename basic_string<T>::self& basic_string<T>::assign(size_type n, value_type c)
    {
        clear();
        return append(n, c);
    }
    
    template<typename T>
    template<typename InputIterator>
    typename basic_string<T>::self& basic_string<T>::assign(InputIterator first, InputIterator last)
    {
        clear();
        return append(first, last);
    }
    
    
    
    
    //////////////////////////////////////////////
    // insert
    
    template<typename T>
    typename basic_string<T>::self& basic_string<T>::insert(size_type pos, const self& rhs)
    {
//        const size_type num_insert = rhs.size();
//        if((end_of_storage - finish) >= num_insert)
//        {
//            const size_type elem_after_pos = finish - (start + pos);
//            if(elem_after_pos > num_insert)
//            {
//                uninitialized_copy(finish - num_insert, finish, finish);
//                copy_backward(start + pos, finish - num_insert, finish);
//                finish += num_insert;
//                copy(rhs.start, rhs.finish, start + pos);
//            }
//            else
//            {
//                uninitialized_copy(start + pos, finish, start + pos + num_insert);
//                finish += num_insert;
//                copy(rhs.start, rhs.start + elem_after_pos, start + pos);
//                uninitialized_copy(rhs.start + elem_after_pos, rhs.finish, finish);
//            }
//        }
//        else
//        {
//            const size_type old_capacity = capacity();
//            const size_type new_capacity = old_capacity > num_insert ? old_capacity * 2 : old_capacity + num_insert;
//            pointer new_start = data_allocator::allocate(new_capacity);
//            pointer new_finish = uninitialized_copy(start, start + pos, new_start);
//            new_finish = uninitialized_copy(rhs.start, rhs.finish, new_finish);
//            new_finish = uninitialized_copy(start + pos, finish, new_finish);
//            destroy(start, finish);
//            data_allocator::deallocate(start, end_of_storage - start);
//            start = new_start;
//            finish = new_finish;
//            end_of_storage = start + new_capacity;
//        }
//        return *this;
        
        // 写完上面代码以后，我发现可以利用insert(iterator position, InputIterator first, InputIterator last)来实现，而且很多insert函数都可以利用这个函数实现
        return insert(start + pos, rhs.start, rhs.finish);
    }
    
    template<typename T>
    typename basic_string<T>::self& basic_string<T>::insert(size_type pos, const self& rhs, size_type start_pos, size_type len)
    {
        if(start_pos < rhs.size())
        {
            pointer first = rhs.start + start_pos;
            pointer last;
            if(len > (rhs.finish - first) || len == npos)
                last = rhs.finish;
            else
                last = first + len;
            return insert(start + pos, first, last);
        }
        return *this;
    }
    
    template<typename T>
    typename basic_string<T>::self& basic_string<T>::insert(size_type pos, const value_type* s)
    {
        //...
        return *this;
    }
    
    template<typename T>
    typename basic_string<T>::self& basic_string<T>::insert(size_type pos, const value_type* s, size_type len)
    {
        //...
        return *this;
    }
    
    template<typename T>
    typename basic_string<T>::self& basic_string<T>::insert(size_type pos, size_type n, value_type c)
    {
        return insert(start + pos, n, c);
    }
    
    template<typename T>
    typename basic_string<T>::self& basic_string<T>::insert(iterator position, size_type n, value_type c)
    {
        if(n != 0)
        {
            if((end_of_storage - finish) >= n)
            {
                const size_type elem_after_pos = finish - position;
                if(elem_after_pos > n)
                {
                    uninitialized_copy(finish - n, finish, finish);
                    copy_backward(position, finish - n, finish);
                    finish += n;
                    fill_n(position, n, c);
                }
                else
                {
                    uninitialized_copy(position, finish, position + n);
                    finish += n;
                    fill(position, finish, c);
                    uninitialized_fill(finish, position + n, c);
                }
            }
            else
            {
                const size_type old_capacity = capacity();
                const size_type new_capacity = old_capacity > n ? old_capacity * 2 : old_capacity + n;
                pointer new_start = data_allocator::allocate(new_capacity);
                pointer new_finish = uninitialized_copy(start, position, new_start);
                new_finish = uninitialized_fill_n(new_finish, n, c);
                new_finish = uninitialized_copy(position, finish, new_finish);
                destroy(start, finish);
                data_allocator::deallocate(start, end_of_storage - start);
                start = new_start;
                finish = new_finish;
                end_of_storage = start + new_capacity;
            }
        }
        return *this;
    }
    
    template<typename T>
    typename basic_string<T>::self& basic_string<T>::insert(iterator position, value_type c)
    {
        return insert(position, 1, c);
    }
    
    template<typename T>
    template<typename InputIterator>
    typename basic_string<T>::self& basic_string<T>::insert(iterator position, InputIterator first, InputIterator last)
    {
        if(last != first)
        {
            const size_type num_insert = last - first;
            if((end_of_storage - finish) >= num_insert)
            {
                const size_type elem_after_pos = finish - position;
                if(elem_after_pos > num_insert)
                {
                    uninitialized_copy(finish - num_insert, finish, finish);
                    copy_backward(position, finish - num_insert, finish);
                    finish += num_insert;
                    copy(first, last, position);
                }
                else
                {
                    uninitialized_copy(position, finish, position + num_insert);
                    finish += num_insert;
                    copy(first, first + elem_after_pos, position);
                    uninitialized_copy(first + elem_after_pos, last, finish);
                }
            }
            else
            {
                const size_type old_capacity = capacity();
                const size_type new_capacity = old_capacity > num_insert ? old_capacity * 2 : old_capacity + num_insert;
                pointer new_start = data_allocator::allocate(new_capacity);
                pointer new_finish = uninitialized_copy(start, position, new_start);
                new_finish = uninitialized_copy(first, last, new_finish);
                new_finish = uninitialized_copy(position, finish, new_finish);
                destroy(start, finish);
                data_allocator::deallocate(start, end_of_storage - start);
                start = new_start;
                finish = new_finish;
                end_of_storage = start + new_capacity;
            }
        }
        return *this;
    }
    
    
    
    
    
    ///////////////////////////////////////////
    // erase
    
    template<typename T>
    typename basic_string<T>::iterator basic_string<T>::erase(size_type pos, size_type len)
    {
        if(pos < size())
        {
            pointer first = start + pos;
            pointer last;
            if(len > (finish - first) || len == npos)
                last = finish;
            else
                last = first + len;
            return erase(first, last);
        }
        return *this;
    }
    
    template<typename T>
    typename basic_string<T>::iterator basic_string<T>::erase(iterator position)
    {
        return erase(position, position + 1);
    }
    
    template<typename T>
    typename basic_string<T>::iterator basic_string<T>::erase(iterator first, iterator last)
    {
        const size_type pos = first - start;
        if(last != first)
        {
            const size_type len = last - first;
            pointer position = copy(last, finish, first);
            destroy(position, finish);
            finish -= len;
        }
        return start + pos;
    }
    
    
    
    
    
    
    /////////////////////////////////////////////
    // replace
    
    // 以下replace函数的实现方式虽然重复利用了代码，可是没有充分考虑效率
    template<typename T>
    typename basic_string<T>::self& basic_string<T>::replace(size_type pos, size_type len, const self& rhs)
    {
        erase(pos, len);
        return insert(pos, rhs);
    }
    
    template<typename T>
    typename basic_string<T>::self& basic_string<T>::replace(iterator first, iterator last, const self& rhs)
    {
        iterator position = erase(first, last);
        return insert(position - start, rhs);
    }
    
    template<typename T>
    typename basic_string<T>::self& basic_string<T>::replace(size_type pos, size_type len, const self& rhs, size_type start_pos, size_type replace_len)
    {
        erase(pos, len);
        return insert(start + pos, rhs.start + start_pos, rhs.start + start_pos + replace_len);
    }
    
    template<typename T>
    typename basic_string<T>::self& basic_string<T>::replace(size_type pos, size_type len, const value_type* s)
    {
        erase(pos, len);
        return insert(pos, s);
    }
    
    template<typename T>
    typename basic_string<T>::self& basic_string<T>::replace(iterator first, iterator last, const value_type* s)
    {
        iterator position = erase(first, last);
        return insert(position - start, s);
    }
    
    template<typename T>
    typename basic_string<T>::self& basic_string<T>::replace(size_type pos, size_type len, const value_type* s, size_type replace_len)
    {
        erase(pos, len);
        return insert(pos, s, replace_len);
    }
    
    template<typename T>
    typename basic_string<T>::self& basic_string<T>::replace(iterator first, iterator last, const value_type* s, size_type replace_len)
    {
        iterator position = erase(first, last);
        return insert(position - start, s, replace_len);
    }
    
    template<typename T>
    typename basic_string<T>::self& basic_string<T>::replace(size_type pos, size_type len, size_type replace_len, value_type c)
    {
        erase(pos, len);
        return insert(pos, replace_len, c);
    }
    
    template<typename T>
    typename basic_string<T>::self& basic_string<T>::replace(iterator first, iterator last, size_type replace_len, value_type c)
    {
        iterator position = erase(first, last);
        return insert(position - start, replace_len, c);
    }
    
    template<typename T>
    template<typename InputIterator>
    typename basic_string<T>::self& basic_string<T>::replace(iterator first, iterator last, InputIterator replace_first, InputIterator replace_last)
    {
        iterator position = erase(first, last);
        return insert(position, replace_first, replace_last);
    }
    
    
    
    
    
    
    template<typename T>
    void basic_string<T>::swap(self& rhs)
    {
        swap(start, rhs.start);
        swap(finish, rhs.finish);
        swap(end_of_storage, rhs.end_of_storage);
    }
    
    template<typename T>
    void basic_string<T>::pop_back()
    {
        if(finish != start)
        {
            --finish;
            destroy(finish);
        }
    }
    
    
    
    
    ////////////////////////////////////////////
    // string operations
    
    template<typename T>
    typename basic_string<T>::size_type basic_string<T>::copy(char* dest, size_type len, size_type pos)
    {
        //...
        return len;
    }
    
    
    typedef basic_string<char> string;
    
    // Non-member function overloads
    inline string operator+(const string& str1, const string& str2);
    inline bool operator==(const string& lhs, const string& rhs);
    inline bool operator==(const char* lhs, const string& rhs);
    inline bool operator==(const string& lhs, const char* rhs);
    inline bool operator!=(const string& lhs, const string& rhs);
    inline bool operator!=(const char* lhs, const string& rhs);
    inline bool operator!=(const string& lhs, const char* rhs);
    inline bool operator<(const string& lhs, const char* rhs);
    inline bool operator<(const char* lhs, const string& rhs);
    inline bool operator<(const string& lhs, const char* rhs);
    inline bool operator<=(const string& lhs, const string& rhs);
    inline bool operator<=(const char* lhs, const string& rhs);
    inline bool operator<=(const string& lhs, const char* rhs);
    inline bool operator>(const string& lhs, const string& rhs);
    inline bool operator>(const char* lhs, const string& rhs);
    inline bool operator>(const string& lhs, const char* rhs);
    inline bool operator>=(const string& lhs, const string& rhs);
    inline bool operator>=(const char* lhs, const string& rhs);
    inline bool operator>=(const string& lhs, const char* rhs);
    inline void swap(string& lhs, string& rhs);
    inline std::istream& operator>>(std::istream& is, string& str);
    inline std::ostream& operator<<(std::ostream& os, const string& str);
    inline std::istream& getline(std::istream& is, string& str, char delim);
    inline std::istream& getline(std::istream& is, string& str);
}

#endif /* string_psd_h */
