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
// 任意节点到叶节点的任何路径，所含的黑节点数必须相同(包括叶节点)
// {
//     这个规则要求，新增节点必须为红
//     结合上一规则，新增节点的父节点必须为黑
// }

/*
// rbtree的基本框架
template<typename Key, typename Value, typename KeyOfValue, typename Compare, typename Alloc>
class rbtree
{
public:
    typedef Key key_type;
    typedef Value value_type;
    typedef value_type* pointer;
    typedef const value_type* const_pointer;
    typedef value_type& reference;
    typedef const value_type& const_reference;
    typedef tree_node* tree_node_ptr;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
    typedef tree_iterator<value_type, reference, pointer> iterator;
    typedef tree_iterator<value_type, const_reference, const_pointer>
    const_iterator;
    typedef reverse_iterator<const_iterator> const_reverse_iterator;
    typedef reverse_iterator<iterator> reverse_iterator;
    
private:
    typedef allocator<tree_node, Alloc> node_allocator;
    
public:
    rbtree();
    rbtree(const Compare& comp);
    rbtree(const rbtree& rhs);
    ~rbtree();
    
    Compare key_compare() const;
    iterator begin();
    const_iterator begin() const;
    iterator end();
    const_iterator end() const;
    reverse_iterator rbegin();
    const_reverse_iterator rbegin() const;
    reverse_iterator rend();
    const_reverse_iterator rend() const;
    
    bool empty() const;
    size_type size() const;
    size_type max_size() const;
    void swap(rbtree& rhs);
    
    pair<iterator, bool> insert_unique(const value_type& v);
    iterator insert_equal(const value_type& v);
    iterator insert_unique(iterator position, const value_type& v);
    iterator insert_equal(iterator position, const value_type& v);
    
    template<typename InputIterator>
    void insert_unique(InputIterator first, InputIterator last);
    
    template<typename InputIterator>
    void insert_equal(InputIterator first, InputIterator last);
    
    void erase(iterator position);
    size_type erase(const key_type& k);
    void erase(iterator first, iterator last);
    void erase(const key_type* first, const key_type* last);
    void clear();
    
    iterator find(const key_type& k);
    const_iterator find(const key_type& k) const;
    size_type count(const key_type& k) const;
    iterator lower_bound(const key_type& k);
    const_iterator lower_bound(const key_type& k) const;
    iterator upper_bound(const key_type& k);
    const_iterator upper_bound(const key_type& k) const;
    pair<iterator, iterator> equal_range(const key_type& k);
    pair<const_iterator, const_iterator> equal_range(const key_type& k) const;
    
    bool rb_verify() const;
};
*/

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
        
        ~rbtree_node()
        {
            parent = NULL;
            left = NULL;
            right = NULL;
        }
        
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
            cur_node->left = NULL;
            cur_node->right = NULL;
            cur_node->parent = NULL;
            cur_node->node_color = rbtree_red;
            return cur_node;
        }
        
        static rbtree_node_ptr construct_node()
        {
            rbtree_node_ptr cur_node = node_allocator::allocate();
            construct(&(cur_node->value_field), value_type());
            cur_node->left = NULL;
            cur_node->right = NULL;
            cur_node->parent = NULL;
            cur_node->node_color = rbtree_red;
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
        
        static void right_rotation(rbtree_node_ptr cur_node, rbtree_node_ptr& root);
        static void left_rotation(rbtree_node_ptr cur_node, rbtree_node_ptr& root);
        static void rbtree_rebalance_after_insert(rbtree_node_ptr cur_node, rbtree_node_ptr& root);
        static rbtree_node_ptr rbtree_rebalance_for_erase(rbtree_node_ptr cur_node, rbtree_node_ptr& root, rbtree_node_ptr& left_most, rbtree_node_ptr& right_most);
        
        iterator insert_aux(rbtree_node_ptr pa_node, const value_type& v);
        
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
        pair<iterator, bool> insert_unique(const value_type& v);
        
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
        
        void erase(iterator position)
        {
            rbtree_node_ptr tmp = rbtree_rebalance_for_erase(position.node, root(), leftmost(), rightmost());
            destroy_node(tmp);
            --node_count;
        }
        
        void erase(iterator first, iterator last)
        {
            while(first != last)
                erase(*(first++));
        }
        
        size_type erase(const key_type& k)
        {
            pair<iterator, iterator> itr = equal_range(k);
            erase(itr.first, itr.second);
        }
        
        void erase(const key_type* first, const key_type* last)
        {
            for(; first != last; ++first)
                erase(*first);
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
    void rbtree<Key, Value, KeyOfValue, Compare, Alloc>::left_rotation(rbtree_node_ptr cur_node, rbtree_node_ptr& root)
    {
        rbtree_node_ptr child = cur_node->right;
        // 将cur_node的右子树嫁接到child上
        cur_node->right = child->left;
        if(child->left != NULL)
            child->left->parent = cur_node;
        // 考虑cur_node是root的情况
        if(cur_node == root)
            root = child;
        // child与cur_node相互嫁接
        child->parent = cur_node->parent;
        if(cur_node->parent->left == cur_node)
            cur_node->parent->left = child;
        else
            cur_node->parent->right = child;
        // chil与cur_node相互嫁接
        child->left = cur_node;
        cur_node->parent = child;
    }
    
    template<typename Key, typename Value, typename KeyOfValue, typename Compare, typename Alloc>
    void rbtree<Key, Value, KeyOfValue, Compare, Alloc>::right_rotation(rbtree_node_ptr cur_node, rbtree_node_ptr& root)
    {
        rbtree_node_ptr child = cur_node->left;
        cur_node->left = child->right;
        if(child->right != NULL)
            child->right->parent = cur_node;
        if(cur_node == root)
            root = child;
        child->parent = cur_node->parent;
        if(cur_node->parent->left == cur_node)
            cur_node->parent->left = child;
        else
            cur_node->parent->right = child;
        child->right = cur_node;
        cur_node->parent = child;
    }
    
    template<typename Key, typename Value, typename KeyOfValue, typename Compare, typename Alloc>
    void rbtree<Key, Value, KeyOfValue, Compare, Alloc>::rbtree_rebalance_after_insert(rbtree_node_ptr cur_node, rbtree_node_ptr& root)
    {
        while(cur_node != root && cur_node->parent != rbtree_red)
        {
            // cur_node在左子树的情况
            if(cur_node->parent == cur_node->parent->parent->left)
            {
                rbtree_node_ptr uncle = cur_node->parent->parent->right;
                // 当cur_node和parent都是红色的，根据第四原则，uncle要么是为空节点，要么是红色的。
                // uncle存在且为红时
                if(uncle && uncle->node_color == rbtree_red)
                {
                    uncle->node_color = rbtree_black;
                    cur_node->parent->node_color = rbtree_black;
                    cur_node->parent->parent = rbtree_red;
                    cur_node = cur_node->parent->parent;
                }
                else
                {
                    if(cur_node == cur_node->parent->right)
                    {
                        cur_node = cur_node->parent;
                        left_rotation(cur_node, root);
                    }
                    cur_node->parent->node_color = rbtree_black;
                    cur_node->parent->parent->node_color = rbtree_red;
                    right_rotation(cur_node->parent->parent, root);
                }
            }
            // cur_node在右子树的情况
            else
            {
                rbtree_node_ptr uncle = cur_node->parent->parent->left;
                if(uncle && uncle->node_color == rbtree_red)
                {
                    uncle->node_color = rbtree_black;
                    cur_node->parent->node_color = rbtree_black;
                    cur_node->parent->parent = rbtree_black;
                    cur_node = cur_node->parent->parent;
                }
                else
                {
                    if(cur_node == cur_node->parent->left)
                    {
                        cur_node = cur_node->parent;
                        right_rotation(cur_node, root);
                    }
                    cur_node->parent->node_color = rbtree_black;
                    cur_node->parent->parent->node_color = rbtree_red;
                    left_rotation(cur_node->parent->parent, root);
                }
            }
        }
        root->node_color = rbtree_black;
    }
    
    // 这段代码参考了源码
    template<typename Key, typename Value, typename KeyOfValue, typename Compare, typename Alloc>
    typename rbtree<Key, Value, KeyOfValue, Compare, Alloc>::rbtree_node_ptr rbtree<Key, Value, KeyOfValue, Compare, Alloc>::rbtree_rebalance_for_erase(rbtree_node_ptr cur_node, rbtree_node_ptr& root, rbtree_node_ptr& left_most, rbtree_node_ptr& right_most)
    {
        rbtree_node_ptr replace_node = cur_node;
        rbtree_node_ptr rebalance_node = NULL;
        rbtree_node_ptr rebalance_node_parent = NULL;
        if(replace_node->left == NULL)
            rebalance_node = replace_node->right;
        else if(replace_node->right == NULL)
            rebalance_node = replace_node->left;
        else
        {
            replace_node = replace_node->right;
            while(replace_node->left != NULL)
                replace_node = replace_node->left;
            rebalance_node = replace_node->right;
        }
        // 如果replace_node是cur_node的后继节点，则用replace_node替代cur_node
        if(replace_node != cur_node)
        {
            cur_node->left->parent = replace_node;
            replace_node->left = cur_node->left;
            if(replace_node != cur_node->right)
            {
                rebalance_node_parent = replace_node->parent;
                if(rebalance_node)
                    rebalance_node->parent = replace_node->parent;
                replace_node->parent->left = rebalance_node;
                replace_node->right = cur_node->right;
                cur_node->right->parent = replace_node;
            }
            else
                rebalance_node_parent = replace_node;
            if(root == cur_node)
                root = replace_node;
            else if(cur_node->parent->left == cur_node)
                cur_node->parent->left = replace_node;
            else
                cur_node->parent->right = replace_node;
            replace_node->parent = cur_node->parent;
            mini_stl::swap(replace_node->node_color, cur_node->node_color);
            replace_node = cur_node;
        }
        // 如果cur_node只有一个子节点或没有子节点，则需要处理cur_node与其子节点的关系
        else
        {
            rebalance_node_parent = cur_node->parent;
            if(rebalance_node)
                rebalance_node->parent = cur_node->parent;
            if(root == cur_node)
                root = rebalance_node;
            else
            {
                if(cur_node->parent->left == cur_node)
                    cur_node->parent->left = rebalance_node;
                else
                    cur_node->parent->right = rebalance_node;
            }
            if(left_most == cur_node)
            {
                if(cur_node->right == NULL)
                    left_most = cur_node->parent;
                else
                    left_most = rbtree_node::minimum(rebalance_node);
            }
            if(right_most == cur_node)
            {
                if(cur_node->left == NULL)
                    right_most = cur_node->parent;
                else
                    right_most = rbtree_node::maximum(rebalance_node);
            }
        }
        if(replace_node->node_color == rbtree_black)
        {
            while(rebalance_node != root && (rebalance_node == 0 || rebalance_node->node_color == rbtree_black))
            {
                if(rebalance_node == rebalance_node_parent->left)
                {
                    rbtree_node_ptr brother = rebalance_node_parent->right;
                    if(brother->node_color == rbtree_red)
                    {
                        brother->node_color = rbtree_black;
                        rebalance_node_parent->node_color = rbtree_red;
                        left_rotation(rebalance_node_parent, root);
                        brother = rebalance_node_parent->right;
                    }
                    if((brother->left == 0 || brother->left->node_color == rbtree_black) && (brother->right == 0 || brother->right->node_color == rbtree_black))
                    {
                        brother->node_color = rbtree_red;
                        rebalance_node = rebalance_node_parent;
                        rebalance_node_parent = rebalance_node_parent->parent;
                    }
                    else
                    {
                        if(brother->right == 0 || brother->right->node_color == rbtree_black)
                        {
                            if(brother->left)
                                brother->left->node_color = rbtree_black;
                            right_rotation(brother, root);
                            brother = rebalance_node_parent->right;
                        }
                        brother->node_color = rebalance_node_parent->node_color;
                        rebalance_node_parent->node_color = rbtree_black;
                        if(brother->right)
                            brother->right->node_color = rbtree_black;
                        left_rotation(rebalance_node_parent, root);
                        break;
                    }
                }
                else
                {
                    rbtree_node_ptr brother = rebalance_node_parent->left;
                    if(brother->node_color == rbtree_red)
                    {
                        brother->node_color = rbtree_black;
                        rebalance_node_parent->node_color = rbtree_red;
                        right_rotation(rebalance_node_parent, root);
                        brother = rebalance_node_parent->right;
                    }
                    if((brother->right == 0 || brother->right->node_color == rbtree_black) && (brother->left == 0 || brother->left->node_color == rbtree_black))
                    {
                        brother->node_color = rbtree_red;
                        rebalance_node = rebalance_node_parent;
                        rebalance_node_parent = rebalance_node_parent->parent;
                    }
                    else
                    {
                        if(brother->left == 0 || brother->left->node_color == rbtree_black)
                        {
                            if(brother->right)
                                brother->right->node_color = rbtree_black;
                            brother->node_color = rbtree_red;
                            left_rotation(brother, root);
                            brother = rebalance_node_parent->left;
                        }
                        brother->node_color = rebalance_node_parent->node_color;
                        rebalance_node_parent->node_color = rbtree_black;
                        if(brother->left)
                            brother->left->node_color = rbtree_black;
                        right_rotation(rebalance_node_parent, root);
                        break;
                    }
                }
                if(rebalance_node)
                    rebalance_node->node_color = rbtree_black;
            }
        }
        return replace_node;
    }
    
    template<typename Key, typename Value, typename KeyOfValue, typename Compare, typename Alloc>
    typename rbtree<Key, Value, KeyOfValue, Compare, Alloc>::iterator rbtree<Key, Value, KeyOfValue, Compare, Alloc>::insert_aux(rbtree_node_ptr pa_node, const value_type& v)
    {
        rbtree_node_ptr new_node = construct_node(v);
        // 如果pa_node是个header，说明整棵树为空，root()为空节点
        if(pa_node == header)
        {
            root() = new_node;
            leftmost() = new_node;
            rightmost() = new_node;
            parent(new_node) = header;
            color(new_node) = rbtree_black;
        }
        // 讨论如果v的键值小于pa_node键值的情况。此时新节点是pa_node的左节点
        else if(key_compare(v, KeyOfValue()(pa_node)))
        {
            left(pa_node) = new_node;
            parent(new_node) = pa_node;
            // 如果pa_node是最左节点的时候，树的leftmost()就需要改变
            if(pa_node == leftmost())
                leftmost() = new_node;
        }
        // 讨论如果v的键值大等于pa_node键值的情况。此时新节点是pa_node的右节点
        else
        {
            right(pa_node) = new_node;
            parent(new_node) = pa_node;
            // 如果pa_node是最右节点的时候，树的rightmost()就需要改变
            if(pa_node == rightmost())
                rightmost() = new_node;
        }
        rbtree_rebalance_after_insert(new_node, header->parent);
        ++node_count;
        return iterator(new_node);
    }
    
    template<typename Key, typename Value, typename KeyOfValue, typename Compare, typename Alloc>
    pair<typename rbtree<Key, Value, KeyOfValue, Compare, Alloc>::iterator, bool> rbtree<Key, Value, KeyOfValue, Compare, Alloc>::insert_unique(const value_type& v)
    {
        rbtree_node_ptr pa_node = header;
        rbtree_node_ptr cur_node = root();
        bool comp;
        while(cur_node != NULL)
        {
            pa_node = cur_node;
            comp = key_compare(KeyOfValue()(v), key(cur_node));
            cur_node = (comp ? cur_node->left : cur_node->right);
        }
        iterator result(pa_node);
        // v的键值比最后一个元素的键值还小的情况
        if(comp)
        {
            // 如果pa_node是最左节点，那么当前树中肯定没有和v的键值一样的节点了。
            // 因为如果有和v一样值的节点，v就会处在某个右子树中，这样pa_node就不会是leftmost()了
            if(pa_node == leftmost())
                return pair<iterator, bool>(insert_aux(pa_node, cur_node, v), true);
            // 如果pa_node不是最左节点，那么说明pa_node现在一定在某个右子树中
            // v的键值和该右子树根节点的parent比较后，只有大等于该节点的键值才可能进入右子树，所以唯一有可能和v的键值相等的是该最小右子树根节点的parent
            // 于是让result迭代器变成最小右子树节点的parent的迭代器
            else
                --result;
        }
        // 因为之前是v的键值大等于result中的node，现在如果得出result中的node也大等于v的键值这一结论，就能说明它们两者键值相等
        if(!key_compare(key(result.node), KeyOfValue(v)))
            return pair<iterator, bool>(result, false);
        // 否则现在就证明整棵树中没有与v的键值相等的节点
        return pair<iterator, bool>(insert_aux(pa_node, cur_node, v), true);
    }
    
    template<typename Key, typename Value, typename KeyOfValue, typename Compare, typename Alloc>
    typename rbtree<Key, Value, KeyOfValue, Compare, Alloc>::iterator rbtree<Key, Value, KeyOfValue, Compare, Alloc>::insert_equal(const value_type& v)
    {
        rbtree_node_ptr pa_node = header;
        rbtree_node_ptr cur_node = root();
        while(cur_node != NULL)
        {
            pa_node = cur_node;
            cur_node = (key_compare(KeyOfValue()(v), key(cur_node)) ? left(cur_node) : right(cur_node));
        }
        return insert_aux(pa_node, cur_node, v);
    }
    
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
