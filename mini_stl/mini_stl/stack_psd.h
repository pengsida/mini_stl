//
//  stack_psd.h
//  mini_stl
//
//  Created by pengsida on 16/9/8.
//  Copyright © 2016年 pengsida. All rights reserved.
//

#ifndef stack_psd_h
#define stack_psd_h

#include "deque_psd.h"

namespace mini_stl
{
    template<typename T, typename Container = deque<T>>
    class stack
    {
    public:
        // Member types
        typedef Container container_type;
        typedef typename Container::value_type value_type;
        typedef typename Container::size_type size_type;
        typedef typename Container::reference reference;
        typedef typename Container::const_reference const_reference;
        
    private:
        container_type container;
        
    public:
        // constructor
        stack():container(){}
        stack(const stack& rhs):container(rhs.container){}
        
        // destructor
        // 使用默认析构函数
        // ~stack();
        
        // operator=
        stack& operator=(const stack& rhs)
        {
            container = rhs.container;
            return *this;
        }
        
        // Element access
        reference top(){return container.back();}
        const_reference top() const{return container.back();}
        
        // Capacity
        bool empty() const{return container.empty();}
        size_type size() const{return container.size();}
        
        // Modifiers
        void push(const value_type& v){container.push_back(v);}
        void pop(){container.pop_back();}
        void swap(stack& rhs){container.swap(rhs.container);}
        
        // friend functions
        template<typename T_type, typename Con>
        friend bool operator==(const stack<T_type, Con>& lhs, const stack<T_type, Con>& rhs);
        
        template<typename T_type, typename Con>
        friend bool operator<(const stack<T_type, Con>& lhs, const stack<T_type, Con>& rhs);
    };
    
    // Non-member functions
    template<typename T, typename Container>
    inline bool operator==(const stack<T,Container>& lhs, const stack<T,Container>& rhs)
    {
        return lhs.container == rhs.container;
    }
    
    template<typename T, typename Container>
    inline bool operator!=(const stack<T,Container>& lhs, const stack<T,Container>& rhs)
    {
        return !(lhs == rhs);
    }
    
    template<typename T, typename Container>
    inline bool operator<(const stack<T,Container>& lhs, const stack<T,Container>& rhs)
    {
        return lhs.container < rhs.container;
    }
    
    template<typename T, typename Container>
    inline bool operator<=(const stack<T,Container>& lhs, const stack<T,Container>& rhs)
    {
        return !(rhs < lhs);
    }
    
    template<typename T, typename Container>
    inline bool operator>(const stack<T,Container>& lhs, const stack<T,Container>& rhs)
    {
        return rhs < lhs;
    }
    
    template<typename T, typename Container>
    inline bool operator>=(const stack<T,Container>& lhs, const stack<T,Container>& rhs)
    {
        return !(lhs < rhs);
    }
    
    template<typename T, typename Container>
    inline void swap(const stack<T,Container>& lhs, const stack<T,Container>& rhs)
    {
        lhs.swap(rhs);
    }
}

#endif /* stack_psd_h */
