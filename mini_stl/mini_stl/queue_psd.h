//
//  queue_psd.h
//  mini_stl
//
//  Created by pengsida on 16/9/8.
//  Copyright © 2016年 pengsida. All rights reserved.
//

#ifndef queue_psd_h
#define queue_psd_h

#include "deque_psd.h"

namespace mini_stl
{
    template<typename T, typename Container = deque<T>>
    class queue
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
        queue():container(){}
        queue(const queue& rhs):container(rhs.container){}
        
        // destrcutor
        // 使用默认析构函数
        // ~queue();
        
        // operator=
        queue& operator=(const queue& rhs)
        {
            container = rhs.container;
            return *this;
        }
        
        // Element access
        reference front(){return container.front();}
        const_reference front() const{return container.front();}
        reference back(){return container.back();}
        const_reference back() const{return container.back();}
        
        // Capacity
        bool empty() const{return container.empty();}
        size_type size() const{return container.size();}
        
        // Modifiers
        void push(const value_type& v){container.push_back(v);}
        void pop(){container.pop_front();}
        void swap(queue& rhs){container.swap(rhs.container);}
        
        // friend functions
        template<typename T_type, typename Con>
        friend bool operator==(const queue<T_type, Con>& lhs, const queue<T_type, Con>& rhs);
        
        template<typename T_type, typename Con>
        friend bool operator<(const queue<T_type, Con>& lhs, const queue<T_type, Con>& rhs);
    };
    
    // Non-member functions
    template<typename T, typename Container>
    inline bool operator==(const queue<T, Container>& lhs, const queue<T, Container>& rhs)
    {
        return lhs.container == rhs.container;
    }
    
    template<typename T, typename Container>
    inline bool operator!=(const queue<T, Container>& lhs, const queue<T, Container>& rhs)
    {
        return !(lhs == rhs);
    }
    
    template<typename T, typename Container>
    inline bool operator<(const queue<T, Container>& lhs, const queue<T, Container>& rhs)
    {
        return lhs.container < rhs.container;
    }
    
    template<typename T, typename Container>
    inline bool operator<=(const queue<T, Container>& lhs, const queue<T, Container>& rhs)
    {
        return !(rhs < lhs);
    }
    
    template<typename T, typename Container>
    inline bool operator>(const queue<T, Container>& lhs, const queue<T, Container>& rhs)
    {
        return rhs < lhs;
    }
    
    template<typename T, typename Container>
    inline bool operator>=(const queue<T, Container>& lhs, const queue<T, Container>& rhs)
    {
        return !(lhs < rhs);
    }
    
    template<typename T, typename Container>
    inline void swap(const queue<T, Container>& lhs, const queue<T, Container>& rhs)
    {
        lhs.swap(rhs);
    }
}

#endif /* queue_psd_h */
