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
#include "pair_psd.h"

//note

//范型化时，调用类或结构体中的类型名称需要前面加个typename
//我有写个程序验证了一下
//vector<int>::iterator test()
//{
//    return vector<int>::iterator();
//}
//
//template<typename T>
//typename vector<T>::iterator tes()
//{
//    return vector<T>::iterator();
//}

// 红黑树
// 每个节点不是红色就是黑色
// 根节点是黑色
// 如果节点是红色，其子节点必须是黑色
// 任意节点到NULL节点的任何路径，所含的黑节点数必须相同
// {
//     这个规则要求，新增节点必须为红
//     结合上一规则，新增节点的父节点必须为黑
// }

namespace mini_stl
{
    template<typename Value>
    struct rbtree_node
    {
        typedef bool rbtree_color_type;
        typedef rbtree_node* rbtree_node_ptr;
        
        rbtree_node_ptr parent;
        rbtree_node_ptr left;
        rbtree_node_ptr right;
        rbtree_color_type node_color;
        Value value_field;
        
        static rbtree_node_ptr maximum(rbtree_node_ptr cur_node)
        {
            while(cur_node->right != NULL)
                cur_node = cur_node->right;
            return cur_node;
        }
        
        static rbtree_node_ptr minimum(rbtree_node_ptr cur_node)
        {
            while(cur_node->left != NULL)
                cur_node = cur_node->left;
            return cur_node;
        }
    };
    
    // rbtree_iterator是一个bidirectional_iterator的迭代器
    template<typename Value, typename Ref, typename Ptr>
    struct rbtree_iterator
    {
        typedef bidirectional_iterator_tag iterator_category;
        typedef Value value_type;
        typedef Ref reference;
        typedef Ptr pointer;
        typedef size_t size_type;
        typedef ptrdiff_t difference_type;
        typedef rbtree_node<Value> rbtree_node;
        typedef rbtree_node* rbtree_node_ptr;
        typedef typename rbtree_node::rbtree_color_type rbtree_color_type;
        
        rbtree_node_ptr node;
        static const rbtree_color_type rbtree_red = false;
        static const rbtree_color_type rbtree_black = true;
        
        rbtree_iterator():node(NULL){}
        explicit rbtree_iterator(rbtree_node_ptr cur_node):node(cur_node){}
        
        // 这么写是考虑了iterator向const_iterator转换
        rbtree_iterator(const rbtree_iterator<Value, Value&, Value*>& rhs)
        :node(rhs.node)
        {}
        
        rbtree_iterator& operator=(const rbtree_iterator& rhs)
        {
            node = rhs.node;
        }
        
        reference operator*() const{return node->value_field;}
        pointer operator->() const{return &(operator*());}
        
        // 正是operator++和operator--使得rbtree类看起来是有序的
        // operator++有两种情况
        // 1.node有右子节点时，返回右子树的最小值
        // 2.除了上两种情况，node回溯到它目前所处最小左子树根节点的parent。考虑到node可能是root，且此时rightmost()是root，我们需要防止node从header又跑到root
        rbtree_iterator& operator++()
        {
            if(node->right != NULL)
            {
                node = node->right;
                while(node->left != NULL)
                    node = node->left;
            }
            else
            {
                rbtree_node_ptr pa = node->parent;
                while(pa->right == node)
                {
                    node = pa;
                    pa = node->parent;
                }
                if(node->right != pa)
                    node = pa;
            }
            return *this;
        }
        
        rbtree_iterator operator++(int)
        {
            rbtree_iterator old = *this;
            ++(*this);
            return old;
        }
        
        rbtree_iterator& operator--()
        {
            rbtree_node_ptr tmp = node;
            if(node->parent->parent == node && node->node_color == rbtree_red)
                node = node->right;
            else if(node->left != NULL)
            {
                node = node->left;
                while(node->right != NULL)
                    node = node->right;
            }
            else
            {
                rbtree_node_ptr pa = node->parent;
                while(pa->left == node)
                {
                    node = pa;
                    pa = node->parent;
                }
                node = pa;
            }
            return *this;
        }
        
        rbtree_iterator operator--(int)
        {
            rbtree_iterator old = *this;
            --(*this);
            return old;
        }
        
        bool operator==(const rbtree_iterator& rhs) const{return node == rhs.node;}
        bool operator!=(const rbtree_iterator& rhs) const{return node != rhs.node;}
    };
    
    template<typename Value, typename Ref, typename Ptr>
    inline bidirectional_iterator_tag iterator_category(const rbtree_iterator<Value, Ref, Ptr>&)
    {
        return bidirectional_iterator_tag();
    }
    
    template<typename Value, typename Ref, typename Ptr>
    inline typename rbtree_iterator<Value, Ref, Ptr>::difference_type* distance_type(const rbtree_iterator<Value, Ref, Ptr>&)
    {
        return static_cast<typename rbtree_iterator<Value, Ref, Ptr>::difference_type*>(0);
    }
    
    template<typename Value, typename Ref, typename Ptr>
    inline typename rbtree_iterator<Value, Ref, Ptr>::value_type* __value_type(const rbtree_iterator<Value, Ref, Ptr>&)
    {
        return static_cast<typename rbtree_iterator<Value, Ref, Ptr>::value_type*>(0);
    }
    
    template<typename Key, typename Value, typename KeyOfValue, typename Compare, typename Alloc = alloc>
    class rbtree
    {
    public:
        typedef Key key_type;
        typedef Value value_type;
        typedef value_type& reference;
        typedef const value_type& const_reference;
        typedef value_type* pointer;
        typedef const value_type* const_pointer;
        typedef rbtree_iterator<Value, Value&, Value*> iterator;
        typedef rbtree_iterator<Value, const Value&, const Value*> const_iterator;
        typedef reverse_iterator<const_iterator> const_reverse_iterator;
        typedef reverse_iterator<iterator> reverse_iterator;
        typedef ptrdiff_t difference_type;
        typedef size_t size_type;
        typedef rbtree_node<Value> rbtree_node;
        typedef rbtree_node* rbtree_node_ptr;
        typedef typename rbtree_node::rbtree_color_type rbtree_color_type;
        
    public:
        static const rbtree_color_type rbtree_red;
        static const rbtree_color_type rbtree_black;
        
    private:
        typedef allocator<rbtree_node, Alloc> node_allocator;
        
        Compare key_compare;
        // 为了size()函数专门设立的
        size_type node_count;
        rbtree_node_ptr header;
        
    private:
        static rbtree_node_ptr construct_node(const value_type& v)
        {
            rbtree_node_ptr cur_node = node_allocator::allocate();
            construct(&(cur_node->value_field));
            return cur_node;
        }
        
        static rbtree_node_ptr construct_node()
        {
            rbtree_node_ptr cur_node = node_allocator::allocate();
            construct(&(cur_node->value_field), value_type());
            return cur_node;
        }
        
        static void destroy_node(rbtree_node_ptr cur_node)
        {
            destroy(&(cur_node->value_field));
            node_allocator::deallocate(cur_node);
        }
        
        static rbtree_node_ptr initial_header()
        {
            rbtree_node_ptr cur_node = construct_node();
            cur_node->parent = NULL;
            cur_node->left = cur_node;
            cur_node->right = cur_node;
            cur_node->node_color = rbtree_red;
            return cur_node;
        }
        
        rbtree_node_ptr& leftmost() const{return header->left;}
        rbtree_node_ptr& rightmost() const{return header->right;}
        rbtree_node_ptr& root() const{return header->parent;}
        
        static rbtree_node_ptr& left(rbtree_node_ptr cur_node){return cur_node->left;}
        static rbtree_node_ptr& right(rbtree_node_ptr cur_node){return cur_node->right;}
        static rbtree_node_ptr& parent(rbtree_node_ptr cur_node){return cur_node->parent;}
        static reference value(rbtree_node_ptr cur_node){return cur_node->value_field;}
        static const key_type& key(rbtree_node_ptr cur_node){return KeyOfValue()(value(cur_node));}
        static rbtree_color_type& color(rbtree_color_type cur_node){return cur_node->node_color;}
        
    public:
        // constructor
        rbtree(const Compare& comp = Compare())
        :key_compare(comp),node_count(0),header(initial_header())
        {}
        
        rbtree(const rbtree& rhs)
        :node_count(0),header(initial_header()),key_compare(rhs.key_compare)
        {
            // 我觉得好像有另一种效率更高的方法
            insert(rhs.begin(), rhs.end());
        }
        
        template<typename InputIterator>
        rbtree(InputIterator first, InputIterator last, const Compare& comp = Compare())
        :header(initial_header()),node_count(0),key_compare(comp)
        {
            insert(first, last);
        }
        
        // destructor
        ~rbtree()
        {
            clear();
            destroy_node(header);
        }
        
        // operator=
        rbtree& operator=(const rbtree& rhs)
        {
            if(this != &rhs)
            {
                clear();
                insert(rhs.begin(), rhs.end());
            }
            return *this;
        }
        
        // Iterators
        iterator begin(){return iterator(leftmost());}
        const_iterator begin() const{return const_iterator(leftmost());}
        iterator end(){return iterator(header);}
        const_iterator end() const{return const_iterator(header);}
        reverse_iterator rbegin(){return reverse_iterator(end());}
        const_reverse_iterator rbegin() const{return const_reverse_iterator(end());}
        reverse_iterator rend(){return reverse_iterator(begin());}
        const_reverse_iterator rend() const{return const_reverse_iterator(begin());}
        const_iterator cbegin() const{return const_iterator(leftmost());}
        const_iterator cend() const{return const_iterator(header);}
        const_reverse_iterator crbegin() const{return const_reverse_iterator(cend());}
        const_reverse_iterator crend() const{return const_reverse_iterator(cbegin());}
        
        // Capacity
        bool empty() const{return root() == header;}
        size_type size() const{return node_count;}
        size_type max_size() const{return size_type(-1) / sizeof(rbtree_node);}
        
        // Modifiers
//        pair<iterator, bool> insert(const value_type& v);
        iterator insert_unique(const value_type& v);
        
        template<typename InputIterator>
        void insert_unique(InputIterator first, InputIterator last)
        {
            for(; first != last; ++first)
                insert_unique(*first);
        }
        
        iterator insert_equal(const value_type& v);
        
        template<typename InputIterator>
        void insert_equal(InputIterator first, InputIterator last)
        {
            for(; first != last; ++first)
                insert_equal(*first);
        }
        
        iterator erase(iterator position);
        size_type erase(const value_type& v);
        
        void erase(iterator first, iterator last)
        {
            for(; first != last; ++first)
                erase(first);
        }
        
        void swap(rbtree& rhs)
        {
            mini_stl::swap(key_compare, rhs.key_compare);
            mini_stl::swap(node_count, rhs.node_count);
            mini_stl::swap(header, rhs.header);
        }
        
        void clear()
        {
            erase(begin(), end());
        }
        
        // Observers
//        key_compare key_comp() const;
//        value_compare value_comp() const;
        
        // Operations
        iterator find(const key_type& k);
        const_iterator find(const key_type& k) const;
        size_type count(const key_type& k) const;
        // 寻找大等于k的元素中最小的节点
        iterator lower_bound(const key_type& k);
        const_iterator lower_bound(const key_type& k) const;
        // 寻找大于k的元素中最小的节点
        iterator upper_bound(const key_type& k);
        const_iterator upper_bound(const key_type& k) const;
        
        pair<iterator, iterator> equal_range(const key_type& k)
        {
            return pair<iterator, iterator>(lower_bound(k), upper_bound(k));
        }
        
        pair<const_iterator, const_iterator> equal_range(const key_type& k) const
        {
            return pair<const_iterator, const_iterator>(lower_bound(k), upper_bound(k));
        }
    };
    
    template<typename Key, typename Value, typename KeyOfValue, typename Compare, typename Alloc>
    const typename rbtree<Key, Value, KeyOfValue, Compare, Alloc>::rbtree_color_type rbtree<Key, Value, KeyOfValue, Compare, Alloc>::rbtree_red = false;
    
    template<typename Key, typename Value, typename KeyOfValue, typename Compare, typename Alloc>
    const typename rbtree<Key, Value, KeyOfValue, Compare, Alloc>::rbtree_color_type rbtree<Key, Value, KeyOfValue, Compare, Alloc>::rbtree_black = true;
    
    template<typename Key, typename Value, typename KeyOfValue, typename Compare, typename Alloc>
    typename rbtree<Key, Value, KeyOfValue, Compare, Alloc>::iterator rbtree<Key, Value, KeyOfValue, Compare, Alloc>::find(const key_type& k)
    {
        rbtree_node_ptr last_node = header;
        rbtree_node_ptr cur_node = root();
        while(cur_node != NULL)
            if(!key_compare(key(cur_node), k)) // key(cur_node) >= k
            {
                last_node = cur_node;
                cur_node = cur_node->left;
            }
            else
                cur_node = cur_node->right;
        // 如果k小于key(cur_node)，说明没有相等的。因为last_node本身就是大等于k的元素中最小的节点
        if(key_compare(k, key(last_node))) return iterator(header);
        return iterator(last_node);
    }
    
    template<typename Key, typename Value, typename KeyOfValue, typename Compare, typename Alloc>
    typename rbtree<Key, Value, KeyOfValue, Compare, Alloc>::const_iterator rbtree<Key, Value, KeyOfValue, Compare, Alloc>::find(const key_type& k) const
    {
        rbtree_node_ptr last_node = header;
        rbtree_node_ptr cur_node = root();
        while(cur_node != NULL)
            if(!key_compare(key(cur_node), k)) // key(cur_node) >= k
            {
                last_node = cur_node;
                cur_node = cur_node->left;
            }
            else
                cur_node = cur_node->right;
        // 如果k小于key(cur_node)，说明没有相等的。因为last_node本身就是大等于k的元素中最小的节点
        if(key_compare(k, key(last_node))) return const_iterator(header);
        return const_iterator(last_node);
    }
    
    template<typename Key, typename Value, typename KeyOfValue, typename Compare, typename Alloc>
    typename rbtree<Key, Value, KeyOfValue, Compare, Alloc>::size_type rbtree<Key, Value, KeyOfValue, Compare, Alloc>::count(const key_type& k) const
    {
        size_type n = 0;
        rbtree_node_ptr cur_node = root();
        while(cur_node != NULL)
            if(!key_compare(key(cur_node), k))
            {
                if(!key_compare(k, key_compare(cur_node))) ++n;
                cur_node = cur_node->left;
            }
            else
                cur_node = cur_node->right;
        return n;
    }
    
    template<typename Key, typename Value, typename KeyOfValue, typename Compare, typename Alloc>
    typename rbtree<Key, Value, KeyOfValue, Compare, Alloc>::iterator rbtree<Key, Value, KeyOfValue, Compare, Alloc>::lower_bound(const key_type& k)
    {
        rbtree_node_ptr last_node = header;
        rbtree_node_ptr cur_node = root();
        while(cur_node != NULL)
            if(!key_compare(key(cur_node), k))
            {
                last_node = cur_node;
                cur_node = cur_node->left;
            }
            else
                cur_node = cur_node->right;
        return iterator(last_node);
    }
    
    template<typename Key, typename Value, typename KeyOfValue, typename Compare, typename Alloc>
    typename rbtree<Key, Value, KeyOfValue, Compare, Alloc>::const_iterator rbtree<Key, Value, KeyOfValue, Compare, Alloc>::lower_bound(const key_type& k) const
    {
        rbtree_node_ptr last_node = header;
        rbtree_node_ptr cur_node = root();
        while(cur_node != NULL)
            if(!key_compare(key(cur_node), k))
            {
                last_node = cur_node;
                cur_node = cur_node->left;
            }
            else
                cur_node = cur_node->right;
        return const_iterator(last_node);
    }
    
    template<typename Key, typename Value, typename KeyOfValue, typename Compare, typename Alloc>
    typename rbtree<Key, Value, KeyOfValue, Compare, Alloc>::iterator rbtree<Key, Value, KeyOfValue, Compare, Alloc>::upper_bound(const key_type& k)
    {
        rbtree_node_ptr last_node = header;
        rbtree_node_ptr cur_node = root();
        while(cur_node != NULL)
            if(key_compare(k, key(cur_node)))
            {
                last_node = cur_node;
                cur_node = cur_node->left;
            }
            else
                cur_node = cur_node->right;
        return iterator(last_node);
    }
    
    template<typename Key, typename Value, typename KeyOfValue, typename Compare, typename Alloc>
    typename rbtree<Key, Value, KeyOfValue, Compare, Alloc>::const_iterator rbtree<Key, Value, KeyOfValue, Compare, Alloc>::upper_bound(const key_type& k) const
    {
        rbtree_node_ptr last_node = header;
        rbtree_node_ptr cur_node = root();
        while(cur_node != NULL)
            if(key_compare(k, key(cur_node)))
            {
                last_node = cur_node;
                cur_node = cur_node->left;
            }
            else
                cur_node = cur_node->right;
        return const_iterator(last_node);
    }
    
    // Non-member functions
    template<typename Key, typename Value, typename KeyOfValue, typename Compare, typename Alloc>
    inline bool operator==(const rbtree<Key, Value, KeyOfValue, Compare, Alloc>& lhs, const rbtree<Key, Value, KeyOfValue, Compare, Alloc>& rhs);
    
    template<typename Key, typename Value, typename KeyOfValue, typename Compare, typename Alloc>
    inline bool operator!=(const rbtree<Key, Value, KeyOfValue, Compare, Alloc>& lhs, const rbtree<Key, Value, KeyOfValue, Compare, Alloc>& rhs)
    {
        return !(lhs == rhs);
    }
    
    template<typename Key, typename Value, typename KeyOfValue, typename Compare, typename Alloc>
    inline bool operator<(const rbtree<Key, Value, KeyOfValue, Compare>& lhs, const rbtree<Key, Value, KeyOfValue, Compare>& rhs)
    {
        typedef typename rbtree<Key, Value, KeyOfValue, Compare, Alloc>::const_iterator const_iterator;
        const_iterator first1 = lhs.begin(), last1 = lhs.end();
        const_iterator first2 = rhs.begin(), last2 = rhs.end();
        return mini_stl::less_compare(first1, last1, first2, last2);
    }
    
    template<typename Key, typename Value, typename KeyOfValue, typename Compare, typename Alloc>
    inline bool operator<=(const rbtree<Key, Value, KeyOfValue, Compare, Alloc>& lhs, const rbtree<Key, Value, KeyOfValue, Compare, Alloc>& rhs)
    {
        return !(rhs < lhs);
    }
    
    template<typename Key, typename Value, typename KeyOfValue, typename Compare, typename Alloc>
    inline bool operator>(const rbtree<Key, Value, KeyOfValue, Compare, Alloc>& lhs, const rbtree<Key, Value, KeyOfValue, Compare, Alloc>& rhs)
    {
        return rhs < lhs;
    }
    
    template<typename Key, typename Value, typename KeyOfValue, typename Compare, typename Alloc>
    inline bool operator>=(const rbtree<Key, Value, KeyOfValue, Compare, Alloc>& lhs, const rbtree<Key, Value, KeyOfValue, Compare, Alloc>& rhs)
    {
        return !(lhs < rhs);
    }
    
    template<typename Key, typename Value, typename KeyOfValue, typename Compare, typename Alloc>
    inline void swap(rbtree<Key, Value, KeyOfValue, Compare, Alloc>& lhs, rbtree<Key, Value, KeyOfValue, Compare, Alloc>& rhs)
    {
        lhs.swap(rhs);
    }
}

#endif /* rbtree_psd_h */
