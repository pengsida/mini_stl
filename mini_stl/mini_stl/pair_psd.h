//
//  pair_psd.h
//  mini_stl
//
//  Created by pengsida on 16/9/14.
//  Copyright © 2016年 pengsida. All rights reserved.
//

#ifndef pair_psd_h
#define pair_psd_h

namespace mini_stl
{
    template<typename T1, typename T2>
    struct pair
    {
        typedef T1 first_type;
        typedef T2 second_type;
        
        T1 first;
        T2 second;
        
        pair(): first(T1()), second(T2()){}
        explicit pair(const T1& a, const T2& b): first(a), second(b){}
        
        template<typename U1, typename U2>
        pair(const pair<U1, U2>& rhs): first(rhs.first), second(rhs.second){}
    };
    
    template<typename T1, typename T2>
    inline bool operator==(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs)
    {
        return (lhs.first == rhs.first) && (lhs.second == rhs.second);
    }
    
    template<typename T1, typename T2>
    inline bool operator!=(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs)
    {
        return !(lhs == rhs);
    }
    
    template<typename T1, typename T2>
    inline bool operator<(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs)
    {
        return (lhs.first > rhs.first) ? false : ((lhs.first < rhs.first) ? true : (lhs.second < rhs.second));
    }
    
    template<typename T1, typename T2>
    inline bool operator<=(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs)
    {
        return !(rhs < lhs);
    }
    
    template<typename T1, typename T2>
    inline bool operator>(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs)
    {
        return rhs < lhs;
    }
    
    template<typename T1, typename T2>
    inline bool operator>=(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs)
    {
        return !(lhs < rhs);
    }
    
    template<typename T1, typename T2>
    inline pair<T1, T2> make_pair(const T1& a, const T2& b)
    {
        return pair<T1, T2>(a, b);
    }
}

#endif /* pair_psd_h */
