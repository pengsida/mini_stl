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
#define NodeToAdd 1

namespace mini_stl
{
    template<typename size_type>
    inline size_type get_buffer_size(size_type n, size_type size_of_data)
    {
        return (n == 0 ? (size_of_data < DefaultSize ? 1 : size_type(DefaultSize/size_of_data)) : n);
    }
    
    // 这是一个random_access_iterator类型的迭代器
    template<typename T, typename Ref, typename Ptr, size_t BufSiz>
    struct deque_iterator
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
        
        deque_iterator():node(NULL),first(NULL),cur_pos(NULL),last(NULL){}
        
        explicit deque_iterator(map_pointer node, pointer pos):node(node),cur_pos(pos)
        {
            first = *node;
            last = first + buffer_size;
        }
        
        deque_iterator(const deque_iterator& rhs): node(rhs.node),first(rhs.first),cur_pos(rhs.cur_pos),last(rhs.last){}
        // 用于iterator向const_iterator的类型转换
        deque_iterator(deque_iterator<T, T&, T*, BufSiz>& rhs): node(rhs.node),first(rhs.first),cur_pos(rhs.cur_pos),last(rhs.last){}
        ~deque_iterator()
        {
            node = NULL;
            first = NULL;
            last = NULL;
            cur_pos = NULL;
        }
        deque_iterator& operator=(const deque_iterator& rhs)
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
        
        deque_iterator& operator++()
        {
            ++cur_pos;
            if(cur_pos == last)
            {
                change_node(node + 1);
                cur_pos = first;
            }
            return *this;
        }
        
        deque_iterator operator++(int)
        {
            deque_iterator old = *this;
            ++(*this);
            return old;
        }
        
        deque_iterator& operator--()
        {
            if(cur_pos == first)
            {
                change_node(node - 1);
                cur_pos = last;
            }
            --cur_pos;
            return *this;
        }
        
        deque_iterator operator--(int)
        {
            deque_iterator old = *this;
            --(*this);
            return old;
        }
        
        deque_iterator& operator+=(difference_type n)
        {
            difference_type offset = n + (cur_pos - first);
            if(offset >= 0 && offset < difference_type(buffer_size))
                cur_pos += n;
            else
            {
                difference_type node_offset = (offset > 0) ? (offset / difference_type(buffer_size)) : -((offset - 1) / difference_type(buffer_size)) - 1;
                change_node(node + node_offset);
                cur_pos = first + (offset - difference_type(buffer_size) * node_offset);
            }
            return *this;
        }
        
        deque_iterator operator+(difference_type n)
        {
            deque_iterator result = *this;
            return (result += n);
        }
        
        deque_iterator& operator-=(difference_type n)
        {
            return (*this += (-n));
        }
        
        deque_iterator operator-(difference_type n)
        {
            return (*this + (-n));
        }
        
        difference_type operator-(const deque_iterator& rhs) const
        {
            return difference_type(buffer_size) * (node - rhs.node - 1) + (cur_pos - first) + (rhs.last - rhs.cur_pos);
        }
        
        bool operator==(const deque_iterator& rhs) const{return ((node == rhs.node) ? (cur_pos == rhs.cur_pos) : false);}
        bool operator!=(const deque_iterator& rhs) const{return !(*this == rhs);}
        bool operator<(const deque_iterator& rhs) const{return ((node == rhs.node) ? (cur_pos < rhs.cur_pos) : (node < rhs.node));}
        bool operator<=(const deque_iterator& rhs) const{return !(rhs < *this);}
        bool operator>(const deque_iterator& rhs) const{return rhs < *this;}
        bool operator>=(const deque_iterator& rhs) const{return !(*this < rhs);}
        
        reference operator[](size_type index) const{return *(*this + index);}
    };
    
    template<typename T, typename Ref, typename Ptr, size_t BufSiz>
    const typename deque_iterator<T, Ref, Ptr, BufSiz>::size_type deque_iterator<T, Ref, Ptr, BufSiz>::buffer_size = get_buffer_size(BufSiz, sizeof(T));
    
    template<typename T, typename Alloc = alloc, size_t BufSiz = 0>
    class deque
    {
    private:
        struct add_front{};
        struct add_back{};
        
    public:
        typedef T value_type;
        typedef T& reference;
        typedef const T& const_reference;
        typedef T* pointer;
        typedef const T* const_pointer;
        typedef deque_iterator<T, const T&, const T*, BufSiz> const_iterator;
        typedef deque_iterator<T, T&, T*, BufSiz> iterator;
        typedef reverse_iterator<const_iterator> const_reverse_iterator;
        typedef reverse_iterator<iterator> reverse_iterator;
        typedef ptrdiff_t difference_type;
        typedef size_t size_type;
        typedef T** map_pointer;
        
    private:
        typedef allocator<T, Alloc> node_allocator;
        typedef allocator<pointer, Alloc> map_allocator;
        
        map_pointer map;
        iterator start;
        iterator finish;
        size_type map_size;
        
    public:
        static const size_type buffer_size;
        
    private:
        pointer allocate_node()
        {
            return node_allocator::allocate(buffer_size);
        }
        
        void allocate_map(size_type n, const value_type& v);
        void reallocate_map(size_type nodes_to_add, add_front);
        void reallocate_map(size_type nodes_to_add, add_back);
        
        void reserve_front()
        {
            if(start.node == map)
                reallocate_map(NodeToAdd, add_front());
        }
        
        void reserve_back()
        {
            if(finish.node - map + 1 == map_size)
                reallocate_map(NodeToAdd, add_back());
        }
        
        void reserve_front_n_element(size_type n)
        {
            const size_type elem_left = size_type(start.node - map) * buffer_size + (start.cur_pos - start.first);
            if(elem_left < n)
                reallocate_map((n - elem_left)/buffer_size + 1, add_front());
        }
        
        void reserve_back_n_element(size_type n)
        {
            const size_type elem_left = (map_size - size_type(finish.node - map + 1)) * buffer_size + (finish.last - finish.cur_pos);
            if(elem_left < n)
                reallocate_map((n - elem_left)/buffer_size + 1, add_back());
        }
        
        template<typename InputIterator>
        void initial_aux(InputIterator first, InputIterator last, true_type)
        {
            allocate_map(size_type(first), last);
        }
        
        template<typename InputIterator>
        void initial_aux(InputIterator first, InputIterator last, false_type)
        {
            for(; first != last; ++first)
                push_back(*first);
        }
        
        template<typename InputIterator>
        void assign_aux(InputIterator first, InputIterator last, true_type)
        {
            assign(size_type(first), last);
        }
        
        template<typename InputIterator>
        void assign_aux(InputIterator first, InputIterator last, false_type)
        {
            size_type n = 0;
            distance(first, last, n);
            const size_type sz = size();
            if(n < sz)
            {
                copy(first, last, start);
                erase(start + n, finish);
            }
            else
            {
                copy(first, first + sz, start);
                uninitialized_copy(first + sz, last, finish);
            }
        }
        
        template<typename InputIterator>
        void insert_aux(iterator position, InputIterator first, InputIterator last, true_type)
        {
            insert(position, size_type(first), last);
        }
        
        template<typename InputIterator>
        void insert_aux(iterator position, InputIterator first, InputIterator last, false_type);
        
    public:
        // constructor
        deque():start(),finish(),map(NULL),map_size(0){}
        explicit deque(size_type n, const value_type& v = value_type())
        {
            allocate_map(n, v);
        }
        
        template<typename InputIterator>
        deque(InputIterator first, InputIterator last)
        :start(),finish(),map(NULL),map_size(0)
        {
            typedef typename is_integer<InputIterator>::_Integral _Integral;
            initial_aux(first, last, _Integral());
        }
        
        deque(const deque& rhs)
        {
            map_size = rhs.map_size;
            map = map_allocator::allocate(map_size);
            const size_type nodes_num = rhs.start.node - rhs.finish.node + 1;
            map_pointer start_node = map + (map_size - nodes_num) / 2;
            start.change_node(start_node);
            start.cur_pos = start.first;
            finish.change_node(start_node + nodes_num - 1);
            finish.cur_pos = finish.first + (rhs.size() % buffer_size);
            uninitialized_copy(rhs.start, rhs.finish, start);
        }
        
        // destructor
        ~deque()
        {
            clear();
            map_pointer cur_node = map;
            for(size_type i = 0; i < map_size; ++cur_node)
                node_allocator::deallocate(*cur_node, buffer_size);
            map_allocator::deallocate(map, map_size);
        }
        
        // operator=
        deque& operator=(const deque& rhs);
        
        // Iterators
        iterator begin(){return start;}
        const_iterator begin() const{return start;}
        iterator end(){return finish;}
        const_iterator end() const{return finish;}
        reverse_iterator rbegin(){return reverse_iterator(start);}
        const_reverse_iterator rbegin() const{return const_reverse_iterator(start);}
        reverse_iterator rend(){return reverse_iterator(finish);}
        const_reverse_iterator rend() const{return const_reverse_iterator(finish);}
        const_iterator cbegin() const{return start;}
        const_iterator cend() const{return finish;}
        const_reverse_iterator crbegin() const{return const_reverse_iterator(start);}
        const_reverse_iterator crend() const{return const_reverse_iterator(finish);}
        
        // Capacity
        size_type size() const{return finish - start;}
        size_type max_size() const{return size_type(-1) / sizeof(value_type);}
        void resize(size_type n);
        void resize(size_type n, const value_type& v);
        bool empty() const{return start == finish;}
        
        // Element access
        reference operator[](size_type index){return *(start + index);}
        const_reference operator[](size_type index) const{return *(start + index);}
        reference at(size_type index){return *(start + index);}
        const_reference at(size_type index) const{return *(start + index);}
        reference front(){return *start;}
        const_reference front() const{return *start;}
        reference back(){return *(finish - 1);}
        const_reference back() const{return *(finish - 1);}
        
        // Modifiers
        // 这些都要考虑边界问题
        void assign(size_type n, const value_type& v)
        {
            const size_type sz = size();
            if(sz < n)
            {
                fill(start, finish, v);
                insert(finish, n - sz, v);
            }
            else
            {
                fill(start, start + n, v);
                erase(start + n, finish);
            }
        }
        
        template<typename InputIterator>
        void assign(InputIterator first, InputIterator last)
        {
            typedef typename is_integer<InputIterator>::_Integral _Integral;
            assign_aux(first, last, _Integral());
        }
        
        void push_back(const value_type& v)
        {
            if(finish.curpos + 1 == finish.last)
                reserve_back();
            construct(finish.cur_pos, v);
            ++finish;
        }
        
        void push_front(const value_type& v)
        {
            if(start.cur_pos == start.first)
                reserve_front();
            --start;
            construct(start.cur_pos, v);
        }
        
        void pop_back()
        {
            --finish;
            destroy(finish.cur_pos);
        }
        
        void pop_front()
        {
            destroy(start.cur_pos);
            ++start;
        }
        
        iterator insert(iterator position, const value_type& v);
        void insert(iterator position, size_type n, const value_type& v);
        
        template<typename InputIterator>
        void insert(iterator position, InputIterator first, InputIterator last)
        {
            typedef typename is_integer<InputIterator>::_Integral _Integral;
            insert_aux(position, first, last, _Integral());
        }
        
        iterator erase(iterator position);
        iterator erase(iterator first, iterator last);
        
        void swap(deque& rhs)
        {
            mini_stl::swap(map, rhs.map);
            mini_stl::swap(start, rhs.start);
            mini_stl::swap(finish, rhs.finish);
            mini_stl::swap(map_size, rhs.map_size);
        }
        
        void clear()
        {
            destroy(start, finish);
            start.change_node(map + map_size / 2);
            finish.change_node(map + map_size / 2);
            start.cur_pos = start.first;
            finish.cur_pos = finish.first;
        }
    };
    
    template<typename T, typename Alloc, size_t BufSiz>
    const typename deque<T,Alloc,BufSiz>::size_type deque<T,Alloc,BufSiz>::buffer_size = get_buffer_size(BufSiz, sizeof(T));
    
    template<typename T, typename Alloc, size_t BufSiz>
    void deque<T,Alloc,BufSiz>::allocate_map(size_type n, const value_type& v)
    {
        size_type nodes_num = n / buffer_size + 1;
        map_size = mini_stl::max(size_type(8), nodes_num + 2);
        map = map_allocator::allocate(map_size);
        map_pointer start_node = map + (map_size - nodes_num) / 2;
        map_pointer finish_node = start_node + (nodes_num - 1);
        for(map_pointer cur_node = start_node; cur_node != finish_node; ++cur_node)
            *cur_node = allocate_node();
        start.change_node(start_node);
        finish.change_node(finish_node);
        start.cur_pos = start.first;
        finish.cur_pos = finish.first + (n % buffer_size);
    }
    
    template<typename T, typename Alloc, size_t BufSiz>
    void deque<T,Alloc,BufSiz>::reallocate_map(size_type nodes_to_add, add_front)
    {
        const size_type old_nodes_num = finish.node - start.node + 1;
        const size_type new_nodes_num = old_nodes_num + nodes_to_add;
        map_pointer new_start_node, new_finish_node;
        if(map_size > 2 * new_nodes_num)
        {
            new_start_node = map + (map_size - new_nodes_num) / 2 + nodes_to_add;
            new_finish_node = new_start_node + (old_nodes_num - 1);
            copy_backward(start.node, finish.node + 1, new_finish_node + 1);
        }
        else
        {
            const size_type new_map_size = map_size + mini_stl::max(map_size, nodes_to_add) + 2;
            map_pointer new_map = map_allocator::allocate(new_map_size);
            new_start_node = map + (new_map_size - new_nodes_num) / 2 + nodes_to_add;
            new_finish_node = new_start_node + (old_nodes_num - 1);
            uninitialized_copy(start.node, finish.node + 1, new_start_node);
            destroy(start.node, finish.node + 1);
            map_allocator::deallocate(map, map_size);
            map = new_map;
            map_size = new_map_size;
        }
        // 原先start.node和finish.node指向的缓存区地址都复制到了new_start_node和new_finish_node中
        start.change_node(new_start_node);
        finish.change_node(new_finish_node);
    }
    
    template<typename T, typename Alloc, size_t BufSiz>
    void deque<T,Alloc,BufSiz>::reallocate_map(size_type nodes_to_add, add_back)
    {
        const size_type old_nodes_num = finish.node - start.node + 1;
        const size_type new_nodes_num = old_nodes_num + nodes_to_add;
        map_pointer new_start_node, new_finish_node;
        if(map_size > 2 * new_nodes_num)
        {
            new_start_node = map + (map_size - new_nodes_num) / 2;
            new_finish_node = new_start_node + (old_nodes_num - 1);
            copy(start.node, finish.node + 1, new_start_node);
        }
        else
        {
            const size_type new_map_size = map_size + mini_stl::max(map_size, nodes_to_add) + 2;
            map_pointer new_map = map_allocator::allocate(new_map_size);
            new_start_node = new_map + (new_map_size - new_nodes_num) / 2;
            new_finish_node = new_start_node + (old_nodes_num - 1);
            uninitialized_copy(start.node, finish.node + 1, new_start_node);
            destroy(start.node, finish.node + 1);
            map_allocator::deallocate(map, map_size);
            map_size = new_map_size;
            map = new_map;
        }
        start.change_node(new_start_node);
        finish.change_node(new_finish_node);
    }
    
    template<typename T, typename Alloc, size_t BufSiz>
    void deque<T,Alloc,BufSiz>::resize(size_type n)
    {
        const size_type sz = size();
        if(sz < n)
            for(size_type tmp = sz; tmp <= n; ++tmp)
                push_back(value_type());
        else
        {
            iterator cur_position = begin();
            advance(cur_position, n);
            erase(cur_position, end());
        }
    }
    
    template<typename T, typename Alloc, size_t BufSiz>
    void deque<T,Alloc,BufSiz>::resize(size_type n, const value_type& v)
    {
        const size_type sz = size();
        if(sz < n)
            for(size_type tmp = sz; tmp <= n; ++tmp)
                push_back(v);
        else
        {
            iterator cur_position = begin();
            advance(cur_position, n);
            erase(cur_position, end());
        }
    }
    
    template<typename T, typename Alloc, size_t BufSiz>
    typename deque<T,Alloc,BufSiz>::iterator deque<T,Alloc,BufSiz>::insert(iterator position, const value_type& v)
    {
        if(position == finish)
        {
            push_back(v);
            return finish - 1;
        }
        else if(position == start)
        {
            push_front(v);
            return start;
        }
        const size_type index = size_type(position - start);
        if(index < size() / 2)
        {
            push_front(*start);
            position = start + index;
            copy(start + 2, position + 1, start + 1);
        }
        else
        {
            push_back(*(finish - 1));
            position = start + index;
            copy_backward(position, finish - 2, finish - 1);
        }
        *position = v;
        return position;
    }
    
    template<typename T, typename Alloc, size_t BufSiz>
    void deque<T,Alloc,BufSiz>::insert(iterator position, size_type n, const value_type& v)
    {
        const size_type index = size_type(position - start + 1);
        if(index < size() / 2)
        {
            reserve_front_n_element(n);
            // position是原position的后一个元素，是为了拷贝[start, 原position]范围内的元素
            position = start + index;
            if(index < n)
            {
                uninitialized_copy(start, position, start - n);
                uninitialized_fill_n(start - (n - index), n - index, v);
                fill(start, position, v);
            }
            else
            {
                uninitialized_copy(start, start + n, start - n);
                copy(start + n, position, start);
                fill(start + (index - n), position, v);
            }
            start -= n;
        }
        else
        {
            reserve_back_n_element(n);
            position = start + (index - 1);
            const size_type elem_after_pos = finish - position;
            if(elem_after_pos < n)
            {
                uninitialized_copy(position, finish, position + n);
                uninitialized_fill(finish, position + n, v);
                fill(position, finish, v);
            }
            else
            {
                uninitialized_copy(finish - n, finish, finish);
                copy_backward(position, finish - n, finish);
                fill(position, position + n, v);
            }
            finish += n;
        }
    }
    
    template<typename T, typename Alloc, size_t BufSiz>
    template<typename InputIterator>
    void deque<T,Alloc,BufSiz>::insert_aux(iterator position, InputIterator first, InputIterator last, false_type)
    {
        const size_type index = size_type(position - start + 1);
        size_type n = 0;
        distance(first, last, n);
        if(index < size() / 2)
        {
            reserve_front_n_element(n);
            position = start + index;
            if(index < n)
            {
                uninitialized_copy(start, position, start - n);
                uninitialized_copy(first, first + (n - index), start - (n - index));
                copy(first + (n - index), last, start);
            }
            else
            {
                uninitialized_copy(start, start + n, start - n);
                copy(start + n, position, start);
                copy(first, last, start + (index - n));
            }
            start -= n;
        }
        else
        {
            reserve_back_n_element(n);
            position = start + (index - 1);
            const size_type elem_after_pos = finish - position;
            if(elem_after_pos < n)
            {
                uninitialized_copy(position, finish, position + n);
                copy(first, first + elem_after_pos, position);
                uninitialized_copy(first + elem_after_pos, last, finish);
            }
            else
            {
                uninitialized_copy(finish - n, finish, finish);
                copy_backward(position, finish - n, finish);
                copy(first, last, position);
            }
            finish += n;
        }
    }
    
    template<typename T, typename Alloc, size_t BufSiz>
    typename deque<T,Alloc,BufSiz>::iterator deque<T,Alloc,BufSiz>::erase(iterator position)
    {
        if(position + 1 == finish)
        {
            pop_back();
            return finish - 1;
        }
        else if(position == start)
        {
            pop_front();
            return start;
        }
        const size_type index = size_type(position - start);
        if(index < size() / 2)
        {
            copy_backward(start, position, position + 1);
            destroy(start.cur_pos);
            ++start;
        }
        else
        {
            copy(position + 1, finish, position);
            --finish;
            destroy(finish.cur_pos);
        }
        return start + index;
    }
    
    template<typename T, typename Alloc, size_t BufSiz>
    typename deque<T,Alloc,BufSiz>::iterator deque<T,Alloc,BufSiz>::erase(iterator first, iterator last)
    {
        if(first != last)
        {
            if(first == start && last == finish)
            {
                clear();
                return finish;
            }
            else
            {
                difference_type n = last - first;
                difference_type elem_before_pos = first - start;
                if(elem_before_pos < ((difference_type(size()) - n) / 2))
                {
                    iterator tmp = copy_backward(start, first, last);
                    destroy(start, tmp);
                    start += n;
                }
                else
                {
                    iterator tmp = copy(last, finish, first);
                    destroy(tmp, finish);
                    finish -= n;
                }
            }
        }
        return last;
    }
    
    // Non-member functions overloads
    template<typename T, typename Alloc>
    bool operator==(const deque<T,Alloc>& lhs, const deque<T,Alloc>& rhs)
    {
        typedef typename deque<T,Alloc>::const_iterator const_iterator;
        const_iterator first1 = lhs.start, last1 = lhs.finish;
        const_iterator first2 = rhs.start, last2 = rhs.finish;
        while(first1 != last1 && first2 != last2)
        {
            if(*first1 != *first2)
                return false;
            ++first1;
            ++first2;
        }
        return ((first1 == last1) && (first2 == last2));
    }
    
    template<typename T, typename Alloc>
    bool operator!=(const deque<T,Alloc>& lhs, const deque<T,Alloc>& rhs)
    {
        return !(lhs == rhs);
    }
    
    template<typename T, typename Alloc>
    bool operator<(const deque<T,Alloc>& lhs, const deque<T,Alloc>& rhs)
    {
        return less_compare(lhs.start, lhs.finish, rhs.start, rhs.finish);
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
