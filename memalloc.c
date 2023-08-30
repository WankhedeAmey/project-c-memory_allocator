#include <unistd.h>
#include <string.h>
#include <pthread.h> 
#include <stdio.h>

pthread_mutex_t global_malloc_lock;

//header
typedef char ALLIGN[16];
union header {
    struct {
        size_t size;
        unsigned is_free;
        union header *next;
    } s;
    ALLIGN stub;
};

typedef union header header_t;

header_t *head, *tail;

header_t *get_free_block(size_t size) {
    header_t *ptr = head;

    while (ptr) {
        if (ptr -> s.is_free && ptr -> s.size >= size) {
            return ptr;
        }
        ptr = ptr -> s.next;
    }
    return NULL;
}

void *malloc(size_t size) {
    pthread_mutex_lock(&global_malloc_lock);
    if (!size) {
        pthread_mutex_unlock(&global_malloc_lock);
        return 
            NULL;
    }
    header_t *header;

    //printf("Calling malloc(%zu)\n", size);

    //get a free block
    header = get_free_block(size);

    if (header) {
        header -> s.is_free = 0;
        pthread_mutex_unlock(&global_malloc_lock);
        return 
            (void*) (header + 1);
    }

    //I no free block, create a new free block
    size_t total_size;

    total_size = size + sizeof(header_t);

    header_t *block = sbrk(total_size);
    if (block == (void*) -1) {
        return NULL;
    }
    header = block;
    header -> s.size    = size;
    header -> s.is_free = 0;
    header -> s.next    = NULL;

    if (!head) {
        head = header;
    }
    if (tail) {
        tail -> s.next = header;
    }
    tail = header;

    pthread_mutex_unlock(&global_malloc_lock);
    return 
        (void*) (header + 1);
}

void free(header_t *block) {
    //if  block is at the end of the heap, I'll release the memory back to OS, otherwise I'll mark the block as 'free'

    if (!block) {
        return;
    }

    header_t *header;
    header_t *programbreak;
    pthread_mutex_lock(&global_malloc_lock);

    header = (header_t*) block - 1;
    programbreak = sbrk(0);

    //topmost block to be freed
    if ((char *)block + header -> s.size == programbreak) {
        if (head == tail) {
            head = tail = NULL;
        }
        else {
            header_t *ptr = head;
            while (ptr) {
                if (ptr -> s.next == tail) {
                    ptr -> s.next = NULL;
                    tail = ptr;
                    break;
                }
                ptr = ptr -> s.next;
            }
        }
        sbrk(0 - sizeof(header_t) - header -> s.size);
        pthread_mutex_unlock(&global_malloc_lock);
        return;
    }

    header -> s.is_free = 1;
    pthread_mutex_unlock(&global_malloc_lock);
}

void *calloc(size_t num, size_t nsize) {
    size_t size;
    void *block;
    if (!num || !nsize) {
        return NULL;
    }

    size = num * nsize;
    //checking overflow
    if (nsize != size / num) {
        return NULL;
    }
    block = malloc(size);
    if (!block) 
        return NULL;
    memset(block, 0, size);
    return block;
}

void *realloc(void *block, size_t size) {
    header_t *header;
    void *new;
    if (!block || !size) {
        return malloc(size);
    }

    header = (header_t *)block - 1;

    //if I can accomodate the size in current block size
    if (header -> s.size >= size) 
        return block;
    
    //else new block is created and current one is freed
    new = malloc(size);

    if (new) {
        memcpy(new, block, header -> s.size);
        free(block);
    }
    return new;
}

//for debug purpose
void print_mem_list() {
    header_t *curr = head;

    printf("head = %p, tail = %p\n", (void *)head, (void *)tail);

    while (curr) {
        printf("addr = %p, size = %zu, is_free = %u, next = %p\n",
                (void *)curr, curr -> s.size, curr -> s.is_free, (void *)curr -> s.next);
        curr = curr -> s.next;
    }
}