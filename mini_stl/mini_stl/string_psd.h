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
#include <ctype.h>
#include <istream>
#include <ostream>

//note

//写substr函数的时候产生了一个疑问
//函数返回值是临时变量时，返回类一定会是隐式转换吗，要怎样改成显式转换
//本来我拷贝构造函数为explicit basic_string(const self& rhs);
//然而这样是substr函数的编译是通不过的，需要允许隐式的拷贝构造函数才行

//对operator>>的重载比较陌生，以后记得研究相应的源码

//char* str = "str"; 这样是错的，因为"strad"是常量字符串，而str不是，会发生类型转换。
//const char* str = "str"; 这样才对。

//拷贝构造函数的参数如果是与类可转换的数据类型，需要在拷贝构造函数前加上explicit，以免发生两者之间的隐式转换。

//函数参数中的pos都是指*this中的位置

namespace mini_stl
{
    class test
    {
    public:
        typedef reverse_iterator<char*> reve;
        
        reve te()
        {
            reve te(0);
            return te;
        }
    };
    
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
        
    public:
        static const size_type npos;
        
    private:
        typedef allocator<T, alloc> data_allocator;
        typedef basic_string<T> self;
        pointer start;
        pointer finish;
        pointer end_of_storage;
        
    private:
        static size_type length_of_values(const value_type* s);
        static int compare_aux(const value_type* str1, size_type len1, const value_type* str2, size_type len2);
        
    public:
        // constructor
        basic_string():start(0),finish(0),end_of_storage(0){}
        basic_string(const self& rhs);
        explicit basic_string(size_type count, value_type c);
        explicit basic_string(const value_type* first, const value_type* last);
        
        self& operator=(const value_type* s);
        self& operator=(const self& rhs);
        self& operator=(value_type c);
        
        // destructor
        ~basic_string();
        
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
        const value_type* c_str() const;
        const value_type* data() const;
        size_type copy(char* dest, size_type len, size_type pos = 0) const;
        
        // searching range: [pos, end)
        size_type find(const self& rhs, size_type pos = 0) const;
        size_type find(const value_type* s, size_type pos = 0) const;
        size_type find(const value_type* s, size_type pos, size_type len) const;
        size_type find(value_type c, size_type pos = 0) const;
        
        // searching range: [0, pos)
        size_type rfind(const self& rhs, size_type pos = npos) const;
        size_type rfind(const value_type* s, size_type pos = npos) const;
        size_type rfind(const value_type* s, size_type pos, size_type len) const;
        size_type rfind(value_type c, size_type pos = npos) const;
        
        // Finds the first character equal to one of the characters
        size_type find_first_of(const self& rhs, size_type pos = 0) const;
        size_type find_first_of(const value_type* s, size_type pos = 0) const;
        size_type find_first_of(const value_type* s, size_type pos, size_type len) const;
        size_type find_first_of(value_type c, size_type pos = 0) const;
        
        // Find character in string from the end
        size_type find_last_of(const self& rhs, size_type pos = npos) const;
        size_type find_last_of(const value_type* s, size_type pos = npos) const;
        size_type find_last_of(const value_type* s, size_type pos, size_type len) const;
        size_type find_last_of(value_type c, size_type pos = npos) const;
        
        size_type find_first_not_of(const self& rhs, size_type pos = 0) const;
        size_type find_first_not_of(const value_type* s, size_type pos = 0) const;
        size_type find_first_not_of(const value_type* s, size_type pos, size_type len) const;
        size_type find_first_not_of(value_type c, size_type pos = 0) const;
        
        size_type find_last_not_of(const self& rhs, size_type pos = npos) const;
        size_type find_last_not_of(const value_type* s, size_type pos = npos) const;
        size_type find_last_not_of(const value_type* s, size_type pos, size_type len) const;
        size_type find_last_not_of(value_type c, size_type pos = npos) const;
        
        self substr(size_type pos = 0, size_type len = npos) const;
        
        int compare(const self& rhs) const;
        int compare(size_type pos, size_type len, const self& rhs) const;
        int compare(size_type pos, size_type len, const self& rhs, size_type compare_pos, size_type compare_len) const;
        int compare(const value_type* s) const;
        int compare(size_type pos, size_type len, const value_type* s) const;
        int compare(size_type pos, size_type len, const value_type* s, size_type n) const;
    };
    
    template<typename T>
    const size_t basic_string<T>::npos = -1;
    
    template<typename T>
    typename basic_string<T>::size_type basic_string<T>::length_of_values(const value_type* s)
    {
        const value_type null = value_type();
        size_type count;
        for(count = 0; s[count] != null; ++count){}
        return count;
    }
    
    template<typename T>
    int basic_string<T>::compare_aux(const value_type* str1, size_type len1, const value_type* str2, size_type len2)
    {
        size_type len = min(len1, len2);
        for(size_type i = 0; i < len; ++i)
            if(str1[i] != str2[i])
                return str1[i] < str2[i] ? -1 : 1;
        return len1 == len2 ? 0 : (len1 < len2 ? -1 : 1);
    }
    
    template<typename T>
    basic_string<T>::basic_string(const self& rhs)
    {
        start = data_allocator::allocate(rhs.capacity());
        end_of_storage = start + rhs.capacity();
        finish = uninitialized_copy(rhs.cbegin(), rhs.cend(), start);
    }
    
    template<typename T>
    basic_string<T>::basic_string(size_type count, value_type c)
    {
        start = data_allocator::allocate(count);
        end_of_storage = start + count;
        finish = uninitialized_fill_n(start, count, c);
    }
    
    template<typename T>
    basic_string<T>::basic_string(const value_type* first, const value_type* last)
    {
        const size_type sz = last - first;
        start = data_allocator::allocate(sz);
        end_of_storage = start + sz;
        finish = uninitialized_copy(first, last, start);
    }
    
    template<typename T>
    typename basic_string<T>::self& basic_string<T>::operator=(const value_type* s)
    {
        return assign(s);
    }
    
    template<typename T>
    typename basic_string<T>::self& basic_string<T>::operator=(const self& rhs)
    {
        if(this != &rhs)
            return assign(rhs.begin(), rhs.end());
        return *this;
    }
    
    template<typename T>
    typename basic_string<T>::self& basic_string<T>::operator=(value_type c)
    {
        return assign(1, c);
    }
    
    template<typename T>
    basic_string<T>::~basic_string()
    {
        destroy(start, finish);
        data_allocator::deallocate(start, end_of_storage - start);
    }
    
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
        n = (n == 0 ? 1 : n);
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
        return append(s);
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
    
    
    
    
    
    
    
    
    ////////////////////////////////////
    // append
    
    template<typename T>
    typename basic_string<T>::self& basic_string<T>::append(const self& rhs)
    {
        return *this += rhs;
    }
    
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
        return append(s, length_of_values(s));
    }
    
    // const value_type* s表示s是一个指针，它指向const value_type
    template<typename T>
    typename basic_string<T>::self& basic_string<T>::append(const value_type* s, size_type n)
    {
        const size_type s_len = length_of_values(s);
        const_pointer last;
        if(n > s_len || n == npos)
            last = s + s_len;
        else
            last = s + n;
        return append(s, last);
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
        if(pos <= size())
            return insert(start + pos, rhs.start, rhs.finish);
        return *this;
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
        if(pos <= size())
            return insert(start + pos, s, s + length_of_values(s));
        return *this;
    }
    
    template<typename T>
    typename basic_string<T>::self& basic_string<T>::insert(size_type pos, const value_type* s, size_type len)
    {
        if(pos <= size())
        {
            const size_type s_len = length_of_values(s);
            const_pointer last;
            if(len > s_len || len == npos)
                last = s + s_len;
            else
                last = s + len;
            return insert(start + pos, s, last);
        }
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
        return start + pos;
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
            pointer position = mini_stl::copy(last, finish, first);
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
    const typename basic_string<T>::value_type* basic_string<T>::c_str() const
    {
        return start;
    }
    
    template<typename T>
    const typename basic_string<T>::value_type* basic_string<T>::data() const
    {
        return start;
    }
    
    template<typename T>
    typename basic_string<T>::size_type basic_string<T>::copy(char* dest, size_type len, size_type pos) const
    {
        const size_type sz = size();
        if(pos < sz)
        {
            for(size_type i = 0; i < len && i < sz; ++i)
                *(dest + i) = *(start + i);
        }
        return 0;
    }
    
    template<typename T>
    typename basic_string<T>::size_type basic_string<T>::find(const self& rhs, size_type pos) const
    {
        if(pos < size())
        {
            iterator result = search(start + pos, finish, rhs.start, rhs.finish);
            return (result == finish ? npos : result - start);
        }
        return npos;
    }
    
    template<typename T>
    typename basic_string<T>::size_type basic_string<T>::find(const value_type* s, size_type pos) const
    {
        return find(s, pos, length_of_values(s));
    }
    
    template<typename T>
    typename basic_string<T>::size_type basic_string<T>::find(const value_type* s, size_type pos, size_type len) const
    {
        if(pos < size())
        {
            iterator result = search(start + pos, finish, s, s + len);
            return result == finish ? npos : result - start;
        }
        return npos;
    }
    
    template<typename T>
    typename basic_string<T>::size_type basic_string<T>::find(value_type c, size_type pos) const
    {
        if(pos < size())
        {
            iterator result = mini_stl::find(start + pos, finish, c);
            return result == finish ? npos : result - start;
        }
        return npos;
    }
    
    template<typename T>
    typename basic_string<T>::size_type basic_string<T>::rfind(const self& rhs, size_type pos) const
    {
        if(pos != 0)
        {
            pointer last;
            if(pos > size() || pos == npos)
                last = finish;
            else
                last = start + pos;
            pointer result = find_end(start, last, rhs.start, rhs.finish);
            return result == finish ? npos : result - start;
        }
        return npos;
    }
    
    template<typename T>
    typename basic_string<T>::size_type basic_string<T>::rfind(const value_type* s, size_type pos) const
    {
        return rfind(s, pos, length_of_values(s));
    }
    
    template<typename T>
    typename basic_string<T>::size_type basic_string<T>::rfind(const value_type* s, size_type pos, size_type len) const
    {
        if(pos != 0)
        {
            pointer last1;
            if(pos > size() || pos == npos)
                last1 = finish;
            else
                last1 = start + pos;
            const_pointer last2;
            const size_type s_len = length_of_values(s);
            if(len > s_len)
                last2 = s + s_len;
            else
                last2 = s + len;
            pointer result = find_end(start, last1, s, last2);
            return result == finish ? npos : result - start;
        }
        return npos;
    }
    
    template<typename T>
    typename basic_string<T>::size_type basic_string<T>::rfind(value_type c, size_type pos) const
    {
        if(pos != 0)
        {
            pointer last;
            if(pos > size() || pos == npos)
                last = finish;
            else
                last = start + pos;
            reverse_iterator rlast = reverse_iterator(start);
            reverse_iterator r_result = mini_stl::find(reverse_iterator(last), rlast, c);
            if(r_result == rlast)
                return npos;
            else
                return (r_result.base() - 1) - start;
        }
        return npos;
    }
    
    template<typename T>
    typename basic_string<T>::size_type basic_string<T>::find_first_of(const self& rhs, size_type pos) const
    {
        if(pos < size())
        {
            pointer result = find_first_of(start + pos, finish, rhs.start, rhs.finish);
            return result == finish ? npos : result - start;
        }
        return npos;
    }
    
    template<typename T>
    typename basic_string<T>::size_type basic_string<T>::find_first_of(const value_type* s, size_type pos) const
    {
        return find_first_of(s, pos, length_of_values(s));
    }
    
    template<typename T>
    typename basic_string<T>::size_type basic_string<T>::find_first_of(const value_type* s, size_type pos, size_type len) const
    {
        if(pos < size())
        {
            const size_type s_len = length_of_values(s);
            const_pointer last;
            if(len > s_len || len == npos)
                last = s + s_len;
            else
                last = s + len;
            pointer result = find_first_of(start + pos, finish, s, last);
            return result == finish ? npos : result - start;
        }
        return npos;
    }
    
    template<typename T>
    typename basic_string<T>::size_type basic_string<T>::find_first_of(value_type c, size_type pos) const
    {
        return find(c, pos);
    }
    
    template<typename T>
    typename basic_string<T>::size_type basic_string<T>::find_last_of(const self& rhs, size_type pos) const
    {
        return find_last_of(rhs.start, pos, rhs.size());
    }
    
    template<typename T>
    typename basic_string<T>::size_type basic_string<T>::find_last_of(const value_type* s, size_type pos) const
    {
        return find_last_of(s, pos, length_of_values(s));
    }
    
    template<typename T>
    typename basic_string<T>::size_type basic_string<T>::find_last_of(const value_type* s, size_type pos, size_type len) const
    {
        if(pos != 0)
        {
            pointer last1;
            if(pos > size() || pos == npos)
                last1 = finish;
            else
                last1 = start + pos;
            const size_type s_len = length_of_values(s);
            const_pointer last2;
            if(len > s_len || len == npos)
                last2 = s + s_len;
            else
                last2 = s + len;
            reverse_iterator r_result = find_first_of(reverse_iterator(last1), rend(), s, last2);
            return r_result == rend() ? npos : (r_result.base() - 1) - start;
        }
        return npos;
    }
    
    template<typename T>
    typename basic_string<T>::size_type basic_string<T>::find_last_of(value_type c, size_type pos) const
    {
        return rfind(c, pos);
    }
    
    template<typename T>
    typename basic_string<T>::size_type basic_string<T>::find_first_not_of(const self& rhs, size_type pos) const
    {
        return find_first_not_of(rhs.start, pos, rhs.size());
    }
    
    template<typename T>
    typename basic_string<T>::size_type basic_string<T>::find_first_not_of(const value_type* s, size_type pos) const
    {
        return find_first_not_of(s, pos, length_of_values(s));
    }
    
    template<typename T>
    typename basic_string<T>::size_type basic_string<T>::find_first_not_of(const value_type* s, size_type pos, size_type len) const
    {
        if(pos < size())
        {
            const size_type s_len = length_of_values(s);
            const_pointer last;
            if(len > s_len || len == npos)
                last = s + s_len;
            else
                last = s + len;
            for(pointer itr1 = start; itr1 != finish; ++itr1)
            {
                const_pointer itr2;
                for(itr2 = s; itr2 != last; ++itr2)
                    if(*itr1 == *itr2)
                        break;
                if(itr2 == last)
                    return itr1 - start;
            }
        }
        return npos;
    }
    
    template<typename T>
    typename basic_string<T>::size_type basic_string<T>::find_first_not_of(value_type c, size_type pos) const
    {
        if(pos < size())
        {
            for(pointer itr = start + pos; itr != finish; ++itr)
                if(*itr != c)
                    return itr - start;
        }
        return npos;
    }
    
    template<typename T>
    typename basic_string<T>::size_type basic_string<T>::find_last_not_of(const self& rhs, size_type pos) const
    {
        return find_last_not_of(rhs.start, pos, rhs.size());
    }
    
    template<typename T>
    typename basic_string<T>::size_type basic_string<T>::find_last_not_of(const value_type* s, size_type pos) const
    {
        return find_last_not_of(s, pos, length_of_values(s));
    }
    
    template<typename T>
    typename basic_string<T>::size_type basic_string<T>::find_last_not_of(const value_type* s, size_type pos, size_type len) const
    {
        if(pos != 0)
        {
            pointer last1;
            if(pos > size() || pos == npos)
                last1 = finish;
            else
                last1 = start + pos;
            const size_type s_len = length_of_values(s);
            const_pointer last2;
            if(len > s_len || len == npos)
                last2 = s + s_len;
            else
                last2 = s + len;
            reverse_iterator rlast(start);
            for(reverse_iterator itr1(last1); itr1 != rlast; ++itr1)
            {
                const_pointer itr2;
                for(itr2 = s; itr2 != last2; ++itr2)
                    if(*itr1 == *itr2)
                        break;
                if(itr2 == last2)
                    return (itr1.base() - 1) - start;
            }
        }
        return npos;
    }
    
    template<typename T>
    typename basic_string<T>::size_type basic_string<T>::find_last_not_of(value_type c, size_type pos) const
    {
        if(pos != 0)
        {
            pointer last;
            if(pos > size() || pos == npos)
                last = finish;
            else
                last = start + pos;
            reverse_iterator rlast(start);
            for(reverse_iterator itr(last); itr != rlast; ++itr)
                if(*itr != c)
                    return (itr.base() - 1) - start;
        }
        return npos;
    }
    
    template<typename T>
    typename basic_string<T>::self basic_string<T>::substr(size_type pos, size_type len) const
    {
        if(pos < size())
        {
            pointer first = start + pos;
            pointer last;
            if(len > (finish - first) || len == npos)
                last = finish;
            else
                last = first + len;
            return self(first, last);
        }
        return self();
    }
    
    template<typename T>
    int basic_string<T>::compare(const self& rhs) const
    {
        return compare_aux(start, size(), rhs.start, rhs.size());
    }
    
    template<typename T>
    int basic_string<T>::compare(size_type pos, size_type len, const self& rhs, size_type compare_pos, size_type compare_len) const
    {
        if(pos < size() && compare_pos < rhs.size())
        {
            pointer first1 = start + pos;
            pointer first2 = rhs.start + compare_pos;
            if(len > (finish - first1))
                len = finish - first1;
            if(compare_len > (rhs.finish - first2))
                compare_len = rhs.finish - first2;
            return compare_aux(first1, len, first2, compare_len);
        }
        return 0;
    }
    
    template<typename T>
    int basic_string<T>::compare(const value_type* s) const
    {
        return compare_aux(start, size(), s, length_of_values(s));
    }
    
    template<typename T>
    int basic_string<T>::compare(size_type pos, size_type len, const value_type* s) const
    {
        return compare(pos, len, s, length_of_values(s));
    }
    
    template<typename T>
    int basic_string<T>::compare(size_type pos, size_type len, const value_type* s, size_type n) const
    {
        if(pos < size())
        {
            pointer first = start + pos;
            if(len > (finish - first))
                len = finish - first;
            const size_type s_len = length_of_values(s);
            if(n > s_len)
                n = s_len;
            return compare_aux(first, len, s, n);
        }
        return 0;
    }
    
    // Non-member function overloads
    template<typename T>
    inline basic_string<T> operator+(const basic_string<T>& str1, const basic_string<T>& str2)
    {
        basic_string<T> result;
        result.append(str1);
        result.append(str2);
        return result;
    }
    
    template<typename T>
    inline bool operator==(const basic_string<T>& lhs, const basic_string<T>& rhs)
    {
        return lhs.compare(rhs) == 0;
    }
    
    template<typename T>
    inline bool operator==(const char* lhs, const basic_string<T>& rhs)
    {
        return rhs.compare(lhs) == 0;
    }
    
    template<typename T>
    inline bool operator==(const basic_string<T>& lhs, const char* rhs)
    {
        return lhs.compare(rhs);
    }
    
    template<typename T>
    inline bool operator!=(const basic_string<T>& lhs, const basic_string<T>& rhs)
    {
        return !(lhs.compare(rhs) == 0);
    }
    
    template<typename T>
    inline bool operator!=(const char* lhs, const basic_string<T>& rhs)
    {
        return !(rhs.compare(lhs) == 0);
    }
    
    template<typename T>
    inline bool operator!=(const basic_string<T>& lhs, const char* rhs)
    {
        return !(lhs.compare(rhs) == 0);
    }
    
    template<typename T>
    inline bool operator<(const basic_string<T>& lhs, const basic_string<T>& rhs)
    {
        return lhs.compare(rhs) == -1;
    }
    
    template<typename T>
    inline bool operator<(const char* lhs, const basic_string<T>& rhs)
    {
        return rhs.compare(lhs) == 1;
    }
    
    template<typename T>
    inline bool operator<(const basic_string<T>& lhs, const char* rhs)
    {
        return lhs.compare(rhs) == -1;
    }
    
    template<typename T>
    inline bool operator>(const basic_string<T>& lhs, const basic_string<T>& rhs)
    {
        return lhs.compare(rhs) == 1;
    }
    
    template<typename T>
    inline bool operator>(const char* lhs, const basic_string<T>& rhs)
    {
        return rhs.compare(lhs) == -1;
    }
    
    template<typename T>
    inline bool operator>(const basic_string<T>& lhs, const char* rhs)
    {
        return lhs.compare(rhs) == 1;
    }
    
    template<typename T>
    inline bool operator<=(const basic_string<T>& lhs, const basic_string<T>& rhs)
    {
        return !(lhs > rhs);
    }
    
    template<typename T>
    inline bool operator<=(const char* lhs, const basic_string<T>& rhs)
    {
        return !(lhs > rhs);
    }
    
    template<typename T>
    inline bool operator<=(const basic_string<T>& lhs, const char* rhs)
    {
        return !(lhs > rhs);
    }
    
    template<typename T>
    inline bool operator>=(const basic_string<T>& lhs, const basic_string<T>& rhs)
    {
        return !(lhs < rhs);
    }
    
    template<typename T>
    inline bool operator>=(const char* lhs, const basic_string<T>& rhs)
    {
        return !(lhs < rhs);
    }
    
    template<typename T>
    inline bool operator>=(const basic_string<T>& lhs, const char* rhs)
    {
        return !(lhs < rhs);
    }
    
    template<typename T>
    inline void swap(basic_string<T>& lhs, basic_string<T>& rhs)
    {
        lhs.swap(rhs);
    }
    
    template<typename T>
    inline std::istream& operator>>(std::istream& is, basic_string<T>& rhs)
    {
        char ch;
        while(is.get(ch))
        {
            if(!(isblank(ch) || ch == '\n'))
                break;
        }
        is.putback(ch);
        rhs.clear();
        while(is.get(ch))
        {
            if(ch != EOF && !isblank(ch) && ch != '\n')
                rhs.push_back(ch);
            else
                break;
        }
        return is;
    }
    
    template<typename T>
    inline std::ostream& operator<<(std::ostream& os, const basic_string<T>& rhs)
    {
        typename basic_string<T>::const_iterator last = rhs.cend();
        for(typename basic_string<T>::const_iterator itr = rhs.cbegin(); itr != last; ++itr)
            os << *itr;
        return os;
    }
    
    template<typename T>
    inline std::istream& getline(std::istream& is, basic_string<T>& rhs, char delim)
    {
        char ch;
        rhs.clear();
        while(is.get(ch))
        {
            if(ch != delim)
                rhs.pushback(ch);
            else
                break;
        }
        return is;
    }
    
    template<typename T>
    inline std::istream& getline(std::istream& is, basic_string<T>& rhs)
    {
        return getline(is, rhs, '\n');
    }
    
    typedef basic_string<char> string;
}

#endif /* string_psd_h */
