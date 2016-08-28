//
//  alloc_psd.h
//  mini_stl
//
//  Created by pengsida on 16/8/27.
//  Copyright © 2016年 pengsida. All rights reserved.
//

#ifndef alloc_psd_h
#define alloc_psd_h

#include <cstdlib>

namespace mini_stl {
    
    class __malloc_alloc_template
    {
    private:
        static void* oom_malloc(size_t);
        static void* oom_realloc(void*, size_t);
        static void (*__malloc_alloc_oom_handler)();
    public:
        static void* allocate(size_t n)
        {
            void* result = malloc(n);
            if(0 == result) result = oom_malloc(n);
            return result;
        }
        static void deallocate(void* p, size_t)
        {
            free(p);
        }
        static void* reallocate(void* p, size_t, size_t new_sz)
        {
            void* result = realloc(p, new_sz);
            if(0 == result) result = oom_realloc(p, new_sz);
            return result;
        }
        static void (*set_malloc_handler(void(*f)()))()
        {
            void (*old)() = __malloc_alloc_oom_handler;
            __malloc_alloc_oom_handler = f;
            return old;
        }
    };
    
    void (* __malloc_alloc_template::__malloc_alloc_oom_handler)() = 0;
    
    void* __malloc_alloc_template::oom_malloc(size_t n)
    {
        void (*my_malloc_handler)();
        void* result;
        while(1)
        {
            my_malloc_handler = __malloc_alloc_oom_handler;
            if(0 == my_malloc_handler) exit(1);
            (*my_malloc_handler)();
            result = malloc(n);
            if(result) return result;
        }
    }
    
    void* __malloc_alloc_template::oom_realloc(void* p, size_t new_sz)
    {
        void (*my_malloc_handler)();
        void* result;
        while(1)
        {
            my_malloc_handler = __malloc_alloc_oom_handler;
            if(0 == my_malloc_handler) exit(1);
            (*my_malloc_handler)();
            result = oom_realloc(p, new_sz);
            if(result) return result;
        }
    }
    
    typedef __malloc_alloc_template malloc_alloc;
    
    template<typename data_type, typename Alloc = malloc_alloc>
    class simple_alloc
    {
    public:
        static data_type* allocate(size_t n)
        {
            return 0 == n ? 0 : (data_type*)Alloc::allocate(n * sizeof(data_type));
        }
        static data_type* allocate(void)
        {
            return (data_type*)Alloc::allocate(sizeof(data_type));
        }
        static void deallocate(data_type* p, size_t n)
        {
            if(0 == n) return;
            Alloc::deallocate(p,n*sizeof(data_type));
        }
        static void deallocate(data_type* p)
        {
            Alloc::deallocate(p,sizeof(data_type));
        }
    };
}


#endif /* alloc_psd_h */








































