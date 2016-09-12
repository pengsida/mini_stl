//
//  rbtree_psd.h
//  mini_stl
//
//  Created by pengsida on 16/9/12.
//  Copyright © 2016年 pengsida. All rights reserved.
//

#ifndef rbtree_psd_h
#define rbtree_psd_h

#include "allocator_psd.h"
#include "algorithm_psd.h"
#include "reverse_iterator_psd.h"

namespace mini_stl
{
    template<typename T>
    struct tree_node
    {
        
    };
    
    template<typename T, typename Ref, typename Ptr>
    struct tree_iterator
    {
        typedef bidirectional_iterator_tag iterator_category;
        typedef T value_type;
        typedef Ref reference;
        typedef Ptr pointer;
        typedef size_t size_type;
        typedef ptrdiff_t difference_type;
    };
    
    template<typename T, typename Compare, typename Alloc = alloc>
    class rbtree
    {
    public:
        typedef T value_type;
        typedef T key_type;
        typedef Compare key_compare;
        typedef Compare value_compare;
        typedef T& reference;
        typedef const T& const_reference;
        typedef T* pointer;
        typedef const T* const_pointer;
        typedef tree_iterator<T, T&, T*> iterator;
        typedef tree_iterator<T, const T&, const T*> const_iterator;
        typedef reverse_iterator<const_iterator> const_reverse_iterator;
        typedef reverse_iterator<iterator> reverse_iterator;
        typedef ptrdiff_t difference_type;
        typedef size_t size_type;
        typedef tree_node<T> tree_node;
        
    private:
        typedef allocator<tree_node, Alloc> node_allocator;
        
    public:
        // constructor
        rbtree(){}
        rbtree(const rbtree& rhs);
        rbtree(size_type n, const value_type& v);
        template<typename InputIterator>
        rbtree(InputIterator first, InputIterator last);
        
        // destructor
        ~rbtree();
        
        // operator=
        rbtree& operator=(const rbtree& rhs);
        
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
        const_iterator crbegin() const;
        const_iterator crend() const;
        
        // Capacity
        bool empty() const;
        size_type size() const;
        size_type max_size() const;
        
        // Modifiers
//        pair<iterator, bool> insert(const value_type& v);
        iterator insert(const_iterator position, const value_type& v);
        template<typename InputIterator>
        void insert(InputIterator first, InputIterator last);
        iterator erase(const_iterator position);
        size_type erase(const value_type& v);
        iterator erase(const_iterator first, const_iterator last);
        void swap(rbtree& rhs);
        void clear();
        
        // Observers
        key_compare key_comp() const;
        value_compare value_comp() const;
        
        // Operations
        iterator find(const value_type& v);
        const_iterator find(const value_type& v) const;
        size_type count(const value_type& v) const;
        iterator lower_bound(const value_type& v);
        const_iterator lower_bound(const value_type& v) const;
        iterator upper_bound(const value_type& v);
        const_iterator upper_bound(const value_type& v) const;
//        pair<const_iterator, const_iterator> equal_range(const value_type& v) const;
//        pair<iterator, iterator> equal_range(const value_type& v);
    };
}

#endif /* rbtree_psd_h */
