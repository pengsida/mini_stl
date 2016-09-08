//
//  list_psd.h
//  mini_stl
//
//  Created by pengsida on 16/9/4.
//  Copyright © 2016年 pengsida. All rights reserved.
//

#ifndef list_psd_h
#define list_psd_h

#include "allocator_psd.h"
#include "algorithm_psd.h"
#include "reverse_iterator_psd.h"
#include <iostream>

//note

// const T& v; v是常量引用，也就是不能改变v的值

// 通过list的实现学习仿函数

// template<typename T, typename Ref, typename Ptr>
// struct iterator{//...};
// 之所以加上Ref和Ptr，是为了能定义const_iterator

namespace mini_stl
{
    template<typename T>
    struct node
    {
        node* prev;
        node* next;
        T data;
        
        node():prev(NULL), next(NULL), data(T()){}
        explicit node(const T& value): prev(NULL), next(NULL), data(value){}
    };
    
    // 这是一个bidirectional_iterator类型的迭代器
    template<typename T, typename Ref, typename Ptr>
    struct list_iterator
    {
        typedef bidirectional_iterator_tag iterator_category;
        typedef T value_type;
        typedef Ptr pointer;
        typedef Ref reference;
        typedef size_t size_type;
        typedef ptrdiff_t difference_type;
        
        typedef node<T> list_node;
        typedef list_node* list_node_ptr;
        
        // 用于完成iterator的任务
        list_node_ptr node;
        
        list_iterator():node(NULL){}
        explicit list_iterator(list_node_ptr node): node(node){}
        
        // 用于iterator向const_iterator的转换
        list_iterator(const list_iterator<T,T&,T*>& rhs): node(rhs.node){}
        
        ~list_iterator(){node = NULL;}
        list_iterator& operator=(const list_iterator& rhs)
        {
            node = rhs.node;
            return *this;
        }
        
        reference operator*() const{return node->data;}
        pointer operator->() const{return &(operator*());}
        
        bool operator==(const list_iterator& rhs){return node == rhs.node;}
        bool operator!=(const list_iterator& rhs){return node != rhs.node;}
        
        list_iterator& operator++()
        {
            node = node->next;
            return *this;
        }
        
        list_iterator operator++(int)
        {
            list_iterator old = *this;
            node = node->next;
            return old;
        }
        
        list_iterator& operator--()
        {
            node = node->prev;
            return *this;
        }
        
        list_iterator operator--(int)
        {
            list_iterator old = *this;
            node = node->prev;
            return old;
        }
    };
    
    template<typename T, typename Ref, typename Ptr>
    inline typename list_iterator<T, Ref, Ptr>::value_type* __value_type(const list_iterator<T, Ref, Ptr>&)
    {
        return static_cast<typename list_iterator<T, Ref, Ptr>::value_type*>(0);
    }
    
    template<typename T, typename Ref, typename Ptr>
    inline bidirectional_iterator_tag iterator_category(const list_iterator<T, Ref, Ptr>&)
    {
        return bidirectional_iterator_tag();
    }
    
    template<typename T, typename Alloc = alloc>
    class list
    {
    public:
        typedef T value_type;
        typedef T& reference;
        typedef const T& const_reference;
        typedef T* pointer;
        typedef const T* const_pointer;
        typedef list_iterator<T,const T&,const T*> const_iterator;
        typedef list_iterator<T,T&,T*> iterator;
        typedef reverse_iterator<const_iterator> const_reverse_iterator;
        typedef reverse_iterator<iterator> reverse_iterator;
        typedef ptrdiff_t difference_type;
        typedef size_t size_type;
        
    private:
        typedef node<T> list_node;
        typedef list_node* list_node_ptr;
        typedef allocator<list_node, Alloc> node_allocator;
        
        list_node_ptr tail;
        
    private:
        // 参考《imperfect C++》，初始化列表中的参数要函数初始化，为防止该函数提前使用未初始化的成员变量，将函数设为静态成员函数，相当于穿上《imperfect C++》中所说的苦行衣
        static list_node_ptr construct_node(const value_type& v)
        {
            list_node_ptr new_node = node_allocator::allocate();
            construct(new_node, v);
            return new_node;
        }
        
        void destroy_node(list_node_ptr cur_node)
        {
            destroy(cur_node);
            node_allocator::deallocate(cur_node);
        }
        
        void destroy_node(iterator first, iterator last)
        {
            for(; first != last; ++first)
                destroy_node(first.node);
        }
        
        template<typename InputIterator>
        void assign_aux(InputIterator first, InputIterator last, true_type)
        {
            assign(size_type(first), last);
        }
        
        template<typename InputIterator>
        void assign_aux(InputIterator first, InputIterator last, false_type)
        {
            clear();
            for(; first != last; ++first)
                push_back(*first);
        }
        
        template<typename InputIterator>
        void insert_aux(iterator position, InputIterator first, InputIterator last, true_type)
        {
            insert(position, size_type(first), last);
        }
        
        template<typename InputIterator>
        void insert_aux(iterator position, InputIterator first, InputIterator last, false_type)
        {
            for(; first != last; ++first)
                insert(position, *first);
        }
        
    public:
        // constructor
        list():tail(construct_node(value_type())){tail->next = tail; tail->prev = tail;}
        
        list(const list& rhs):tail(rhs.tail){}
        
        explicit list(size_type n, const value_type& v = value_type())
        :tail(construct_node(value_type()))
        {
            tail->next = tail;
            tail->prev = tail;
            for(size_type i = 0; i < n; ++i)
                push_back(v);
        }
        
        template<typename InputIterator>
        list(InputIterator first, InputIterator last)
        :tail(construct_node(value_type()))
        {
            tail->next = tail;
            tail->prev = tail;
            for(; first != last; ++first)
                push_back(*first);
        }
        
        // destructor
        ~list()
        {
            clear();
            destroy_node(tail);
        }
        
        // operator=
        list& operator=(const list& rhs)
        {
            if(this != &rhs)
            {
                clear();
                iterator last = rhs.end();
                for(iterator itr = rhs.begin(); itr != last; ++itr)
                    push_back(*itr);
            }
            return *this;
        }
        
        // Iterators
        iterator begin(){return iterator(tail->next);}
        const_iterator begin() const{return const_iterator(tail->next);}
        iterator end(){return iterator(tail);}
        const_iterator end() const{return const_iterator(tail);}
        reverse_iterator rbegin(){return reverse_iterator(end());}
        const_reverse_iterator rbegin() const{return const_reverse_iterator(end());}
        reverse_iterator rend(){return reverse_iterator(begin());}
        const_reverse_iterator rend() const{return const_reverse_iterator(end());}
        const_iterator cbegin() const{return const_iterator(tail->next);}
        const_iterator cend() const{return const_iterator(tail);}
        const_reverse_iterator rcbegin() const{return const_reverse_iterator(cend());}
        const_reverse_iterator rcend() const{return const_reverse_iterator(cbegin());}
        
        // Capacity
        bool empty() const{return tail->next == tail;}
        
        size_type size() const
        {
            size_type n = 0;
            distance(begin(), end(), n);
            return n;
        }
        
        size_type max_size() const{return size_type(-1) / sizeof(list_node);}
        
        // Element access
        reference front(){return *begin();}
        const_reference front() const{return *begin();}
        reference back(){return *(--end());}
        const_reference back() const{return *(--end());}
        
        // Modifiers
        
        // Assigns new contents to the list container, replacing its current contents, and modifying its size accordingly
        void assign(size_type n, const value_type& v)
        {
            clear();
            for(size_type i = 0; i < n; ++i)
                push_back(v);
        }
        
        template<typename InputIterator>
        void assign(InputIterator first, InputIterator last)
        {
            typedef typename is_integer<InputIterator>::_Integral _Integral;
            assign_aux(first, last, _Integral());
        }
    
        void push_front(const value_type& v)
        {
            list_node_ptr new_node = construct_node(v);
            tail->next->prev = new_node;
            new_node->next = tail->next;
            new_node->prev = tail;
            tail->next = new_node;
        }
        
        void pop_front()
        {
            list_node_ptr cur_node = tail->next;
            tail->next = cur_node->next;
            cur_node->next->prev = tail;
            destroy_node(cur_node);
        }
        
        void push_back(const value_type& v)
        {
            list_node_ptr new_node = construct_node(v);
            tail->prev->next = new_node;
            new_node->prev = tail->prev;
            new_node->next = tail;
            tail->prev = new_node;
        }
        
        void pop_back()
        {
            list_node_ptr cur_node = tail->prev;
            tail->prev = cur_node->prev;
            cur_node->prev->next = tail;
            destroy_node(cur_node);
        }
        
        iterator insert(iterator position, const value_type& v)
        {
            list_node_ptr new_node = construct_node(v);
            list_node_ptr cur_node = position.node;
            cur_node->prev->next = new_node;
            new_node->prev = cur_node->prev;
            new_node->next = cur_node;
            cur_node->prev = new_node;
            return iterator(new_node);
        }
        
        void insert(iterator position, size_type n, const value_type& v)
        {
            for(size_type i = 0; i < n; ++i)
                position = insert(position, v);
        }
        
        template<typename InputIterator>
        void insert(iterator position, InputIterator first, InputIterator last)
        {
            typedef typename is_integer<InputIterator>::_Integral _Integral;
            insert_aux(position, first, last, _Integral());
        }
        
        iterator erase(iterator position)
        {
            list_node_ptr cur_node = position.node;
            list_node_ptr tmp = cur_node->next;
            cur_node->prev->next = cur_node->next;
            cur_node->next->prev = cur_node->prev;
            destroy_node(cur_node);
            return iterator(tmp);
        }
        
        iterator erase(iterator first, iterator last)
        {
            list_node_ptr cur_node = first.node;
            list_node_ptr last_node = last.node;
            cur_node->prev->next = last_node;
            last_node->prev = cur_node->prev;
            destroy_node(first, last);
            return last;
        }
        
        void swap(list& rhs)
        {
            mini_stl::swap(tail, rhs.tail);
        }
        
        void resize(size_type n);
        void resize(size_type n, const value_type& v);
        
        void clear()
        {
            erase(begin(), end());
        }
        
        // Operations
        
        // Transfers elements from x into the container, inserting them at position
        void splice(iterator position, list& rhs)
        {
            splice(position, rhs, rhs.begin(), rhs.end());
        }
        
        void splice(iterator position, list& rhs, iterator rhs_position)
        {
            splice(position, rhs, rhs_position, ++rhs_position);
        }
        
        void splice(iterator position, list& rhs, iterator first, iterator last)
        {
            if(first != last)
            {
                list_node_ptr tmp = (last.node)->prev;
                (first.node)->prev->next = last.node;
                (last.node)->prev = (first.node)->prev;
                (position.node)->prev->next = first.node;
                (first.node)->prev = (position.node)->prev;
                tmp->next = position.node;
                (position.node)->prev = tmp;
            }
        }
        
        void remove(const value_type& v)
        {
            iterator last = end();
            for(iterator itr = begin(); itr != last;)
                if(*itr == v)
                    itr = erase(itr);
                else
                    ++itr;
        }
        
        template<typename Predicate>
        void remove_if(Predicate pred)
        {
            iterator last = end();
            for(iterator itr = begin(); itr != last;)
            {
                iterator next = itr;
                ++next;
                if(pred(*itr))
                    erase(itr);
                itr = next;
            }
        }
        
        void unique()
        {
            iterator prev = begin();
            iterator last = --end();
            while(prev != last)
            {
                iterator next = prev;
                ++next;
                if(*prev == *next)
                    erase(prev);
                prev = next;
            }
        }
        
        template<typename BinaryPredicate>
        void unique(BinaryPredicate binary_pred)
        {
            iterator prev = begin();
            iterator last = --end();
            while(prev != last)
            {
                iterator next = prev;
                ++next;
                if(binary_pred(*prev, *next))
                    erase(prev);
                prev = next;
            }
        }
        
        // This effectively removes all the elements in rhs (which becomes empty), and inserts them into their ordered position within container
        void merge(list& rhs);
        
        template<typename Compare>
        void merge(list& rhs, Compare comp);
        
        void sort();
        
        template<typename Compare>
        void sort(Compare comp);
        
        void reverse();
    };
    
    template<typename T, typename Alloc>
    void list<T,Alloc>::resize(size_type n)
    {
        const size_type sz = size();
        if(sz < n)
            for(size_type tmp_sz = sz; tmp_sz <= n; ++tmp_sz)
                push_back(value_type());
        else
        {
            iterator cur_pos = begin();
            advance(cur_pos, n);
            erase(cur_pos, end());
        }
    }
    
    template<typename T, typename Alloc>
    void list<T,Alloc>::resize(size_type n, const value_type& v)
    {
        const size_type sz = size();
        if(sz < n)
            for(size_type tmp_sz = sz; tmp_sz <= n; ++tmp_sz)
                push_back(v);
        else
        {
            iterator cur_pos = begin();
            advance(cur_pos, n);
            erase(cur_pos, end());
        }
    }
    
    template<typename T, typename Alloc>
    void list<T,Alloc>::merge(list<T,Alloc>& rhs)
    {
        iterator first1 = begin(), last1 = end();
        iterator first2 = rhs.begin(), last2 = rhs.end();
        while(first1 != last1 && first2 != last2)
            if(*first1 <= *first2)
                ++first1;
            else
            {
                iterator next = first2;
                ++next;
                splice(first1, rhs, first2);
                first2 = next;
            }
        splice(first1, rhs, first2, last2);
    }
    
    template<typename T, typename Alloc>
    template<typename Compare>
    void list<T,Alloc>::merge(list<T,Alloc>& rhs, Compare comp)
    {
        iterator first1 = begin(), last1 = end();
        iterator first2 = rhs.begin(), last2 = rhs.end();
        while(first1 != last1 && first2 != last2)
            if(comp(*first2, *first1))
            {
                iterator next = first2;
                ++next;
                splice(first1, rhs, first2);
                first2 = next;
            }
            else
                ++first1;
        splice(first1, rhs, first2, last2);
    }
    
    // 参考了源码的实现方式
    // 有点插入排序的思想
    // 时间复杂度为nlog(n)
//    template<typename T, typename Alloc>
//    void list<T,Alloc>::sort()
//    {
//        if(tail->next != tail && tail->next->next != tail)
//        {
//            list carry;
//            list counter[64];
//            int fill = 0;
//            while(tail->next != tail)
//            {
//                carry.splice(carry.begin(), *this, begin());
//                int index = 0;
//                while(index < fill && !counter[index].empty())
//                {
//                    counter[index].merge(carry);
//                    carry.swap(counter[index++]);
//                }
//                carry.swap(counter[index]);
//                if(index == fill) ++fill;
//            }
//            for(int index = 1; index < fill; ++index)
//                counter[index].merge(counter[index - 1]);
//            swap(counter[fill-1]);
//        }
//    }

    // 直接使用insertion sort
    // 时间复杂度为n^2
    template<typename T, typename Alloc>
    void list<T,Alloc>::sort()
    {
        if(tail->next != tail)
        {
            iterator i = ++begin();
            iterator last = end();
            while(i != last)
            {
                iterator first = begin();
                iterator next = i;
                ++next;
                iterator j = i;
                for(; j != first; --j)
                {
                    iterator prev = j;
                    --prev;
                    if(*i > *prev)
                        break;
                }
                if(j != i)
                    splice(j, *this, i);
                i = next;
            }
        }
    }
    
    template<typename T, typename Alloc>
    template<typename Compare>
    void list<T,Alloc>::sort(Compare comp)
    {
        if(tail->next != tail && tail->next->next != tail)
        {
            list carry;
            list counter[64];
            int fill = 0;
            while(tail->next != tail)
            {
                int index = 0;
                carry.splice(carry.begin(), *this, begin());
                while(index < fill && !counter[index].empty())
                {
                    counter[index].merge(carry, comp);
                    carry.swap(counter[index++]);
                }
                carry.swap(counter[index]);
                if(index == fill) ++fill;
            }
            for(int index = 1; index < fill; ++index)
                counter[index].merge(counter[index - 1], comp);
            swap(counter[fill-1]);
        }
    }
    
    template<typename T, typename Alloc>
    void list<T,Alloc>::reverse()
    {
        if(tail->next != tail)
        {
            iterator first = begin();
            iterator last = end();
            iterator cur_pos = first;
            ++cur_pos;
            while(cur_pos != last)
            {
                iterator next = cur_pos;
                ++next;
                splice(first, *this, cur_pos);
                first = cur_pos;
                cur_pos = next;
            }
        }
    }
    
    template<typename T, typename Alloc>
    inline bool operator==(const list<T,Alloc>& lhs, const list<T,Alloc>& rhs)
    {
        typedef typename list<T,Alloc>::const_iterator const_iterator;
        const_iterator first1 = lhs.begin(), last1 = lhs.end();
        const_iterator first2 = rhs.begin(), last2 = rhs.end();
        while(first1 != last1 && first2 != last2)
        {
            if(*first1 != *first2)
                break;
            ++first1;
            ++first2;
        }
        return ((first1 == last1) && (first2 == last2));
    }
    
    template<typename T, typename Alloc>
    inline bool operator!=(const list<T,Alloc>& lhs, const list<T,Alloc>& rhs)
    {
        return !(lhs == rhs);
    }
    
    template<typename T, typename Alloc>
    inline bool operator<(const list<T,Alloc>& lhs, const list<T,Alloc>& rhs)
    {
        typedef typename list<T,Alloc>::const_iterator const_iterator;
        const_iterator first1 = lhs.begin(), last1 = lhs.end();
        const_iterator first2 = rhs.begin(), last2 = rhs.end();
        while(first1 != last1 && first2 != last2)
        {
            if(*first1 < *first2)
                return true;
            if(*first2 < *first1)
                return false;
            ++first1;
            ++first2;
        }
        return ((first1 == last1) && (first2 != last2));
    }
    
    template<typename T, typename Alloc>
    inline bool operator<=(const list<T,Alloc>& lhs, const list<T,Alloc>& rhs)
    {
        return !(rhs < lhs);
    }
    
    template<typename T, typename Alloc>
    inline bool operator>(const list<T,Alloc>& lhs, const list<T,Alloc>& rhs)
    {
        return rhs < lhs;
    }
    
    template<typename T, typename Alloc>
    inline bool operator>=(const list<T,Alloc>& lhs, const list<T,Alloc>& rhs)
    {
        return !(lhs < rhs);
    }
    
    template<typename T, typename Alloc>
    inline void swap(list<T,Alloc>& lhs, list<T,Alloc>& rhs)
    {
        lhs.swap(rhs);
    }
}

#endif /* list_psd_h */
