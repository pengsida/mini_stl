//
//  alloc_default_psd.h
//  mini_stl
//
//  Created by pengsida on 16/8/28.
//  Copyright © 2016年 pengsida. All rights reserved.
//

#ifndef alloc_default_psd_h
#define alloc_default_psd_h

#ifndef alloc_psd_h
#include "alloc_psd.h"
#endif

namespace mini_stl {
    class default_alloc_template
    {
    private:
        enum {ALIGN_BYTES = 8};
        enum {MAX_BYTES = 128};
        enum {NFREE_LISTS = 16};
        enum {NOBJS = 20};
        
    private:
        // 不是很懂为什么要用union
        union obj
        {
            union obj* next;
            char data[1];
        };
    
    private:
        static char* start_free;
        static char* end_free;
        static size_t heap_size;
    
    private:
        static obj* free_list[NFREE_LISTS];
        
        static size_t round_up(size_t bytes_size);
        static size_t freelist_index(size_t bytes_size);
        static void* refill(size_t n);
        static char* chunk_alloc(size_t size, int & nobjs);
        static void* allocate_from_pool(size_t n);
        static void deallocate_to_pool(void * p, size_t n);
        
    public:
        static void* allocate(size_t n);
        static void deallocate(void * p, size_t n);
        static void* reallocate(void * p, size_t old_sz, size_t new_sz);
    };
    
    char* default_alloc_template::start_free = 0;
    char* default_alloc_template::end_free = 0;
    size_t default_alloc_template::heap_size = 0;
    default_alloc_template::obj* default_alloc_template::free_list[NFREE_LISTS]
    = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    
    size_t default_alloc_template::round_up(size_t bytes_size)
    {
        return ((bytes_size + (size_t)ALIGN_BYTES - 1) & ~((size_t)ALIGN_BYTES - 1));
    }
    
    size_t default_alloc_template::freelist_index(size_t bytes_size)
    {
        return ((bytes_size + (size_t)ALIGN_BYTES - 1) / (size_t)ALIGN_BYTES - 1);
    }
    
    void* default_alloc_template::refill(size_t n)
    {
        int nobjs = NOBJS;
        char* chunk = chunk_alloc(n, nobjs);
        obj** my_free_list;
        obj* result;
        obj* current_obj;
        obj* next_obj;
        int i;
        if(1 == nobjs) return (void*)chunk;
        my_free_list = free_list + freelist_index(n);
        // 返回chunk的第一个节点作为分配的空间
        result = (obj*)chunk;
        // 接下来是将chunk_alloc新分配的节点放到my_free_list中
        *my_free_list = next_obj = (obj*)(chunk + n);
        for(i = 1; ; i++)
        {
            current_obj = next_obj;
            if(nobjs - 1 == i)
            {
                current_obj->next = 0;
                break;
            }
            next_obj = (obj*)((char*)next_obj + n);
            current_obj->next = next_obj;
        }
        return (void*)result;
    }
    
    char* default_alloc_template::chunk_alloc(size_t size, int & nobjs)
    {
        char* result;
        size_t total_bytes = size * nobjs;
        size_t bytes_left = end_free - start_free;
        if(bytes_left >= total_bytes)
        {
            result = start_free;
            start_free += total_bytes;
            return result;
        }
        else if(bytes_left >= size)
        {
            nobjs = (int)bytes_left / size;
            total_bytes = size * nobjs;
            result = start_free;
            start_free += total_bytes;
            return result;
        }
        else
        {
            size_t bytes_to_get = 2 * total_bytes + round_up(heap_size >> 4);
            if(bytes_left > 0)
            {
                obj** my_free_list = free_list + freelist_index(bytes_left);
                ((obj*)start_free)->next = *my_free_list;
                *my_free_list = (obj*)start_free;
            }
            start_free = (char*)malloc(bytes_to_get);
            if(0 == start_free)
            {
                int i;
                obj** my_free_list;
                obj* p;
                // 搜索free_list中可用的空间
                for(i = (int)size; i <= MAX_BYTES; i += ALIGN_BYTES)
                {
                    my_free_list = free_list + freelist_index(i);
                    p = *my_free_list;
                    if(0 != p)
                    {
                        *my_free_list = p->next;
                        start_free = (char*)p;
                        end_free = start_free + i;
                        return chunk_alloc(size, nobjs);
                    }
                }
                end_free = 0;
                // 使用simple_alloc分配空间
                start_free = (char*)malloc_alloc::allocate(bytes_left);
            }
            heap_size += bytes_left;
            end_free = start_free + bytes_to_get;
            return chunk_alloc(size, nobjs);
        }
    }
    
    void* default_alloc_template::allocate_from_pool(size_t n)
    {
        obj** my_free_list = free_list + freelist_index(n);
        obj* result = *my_free_list;
        if(result == 0)
        {
            void * re = refill(round_up(n));
            return re;
        }
        *my_free_list = result->next;
        return (void*)result;
    }
    
    void* default_alloc_template::allocate(size_t n)
    {
        if(n > (size_t)MAX_BYTES) return malloc_alloc::allocate(n);
        return allocate_from_pool(n);
    }
    
    void default_alloc_template::deallocate(void * p, size_t n)
    {
        if(n > MAX_BYTES)
            malloc_alloc::deallocate(p, n);
        else
        {
            obj * q = (obj*)p;
            obj** my_free_list = free_list + freelist_index(n);
            q->next = *my_free_list;
            *my_free_list = q;
        }
    }
    
    void* default_alloc_template::reallocate(void * p, size_t old_sz, size_t new_sz)
    {
        if(old_sz > MAX_BYTES && new_sz > MAX_BYTES)
            return malloc_alloc::reallocate(p, old_sz, new_sz);
        if((size_t)old_sz == (size_t)new_sz)
            return p;
        void * result = allocate(new_sz);
        size_t copy_sz = new_sz > old_sz ? old_sz : new_sz;
        memcpy(result, p, copy_sz);
        deallocate(p, old_sz);
        return result;
    }
    
    typedef default_alloc_template alloc;
}

#endif /* alloc_default_psd_h */























