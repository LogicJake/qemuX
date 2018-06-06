#include <stdio.h>
#include <string.h> 
#include <stdlib.h>   
#define POINTERS 2
#define REGION_SIZE (4000 + 1000*16)

struct block {
	void *address;			//the address in the region 
    struct block *next;
    int in_use;
    int size;
};

static char region[REGION_SIZE];    /* define a global buffer as heap, we use this buffer to test my_malloc/my_free */   
static block *g_chunk_head;                    /* the head chunk of this heap memory */  
enum {FREE = 0, USED};

void my_malloc_init()
{
	g_chunk_head = (block *)malloc(sizeof(block));
	g_chunk_head->address = region; 
	g_chunk_head->next = NULL;  
    g_chunk_head->in_use = FREE;
    g_chunk_head->size = REGION_SIZE;
}

void *my_malloc(int size)
{
	block *ptr = g_chunk_head;  
    block *free_chunk = (block *)malloc(sizeof(block));  
    /* find a free and big-enough chunk */  
    while(ptr)  
    {  
        if(ptr->in_use == FREE && ptr->size >= size)  
            break;  
        ptr = ptr->next;  
    }  
  
  
    /* create a new free mem control block */  
    free_chunk->address = ptr->address + size;
    free_chunk->next = NULL;  
    free_chunk->in_use = FREE;  
    free_chunk->size  = ptr->size - size;  
    free_chunk->next  = ptr->next;  
  
    ptr->in_use = USED;  
    ptr->size = size;  
    ptr->next = free_chunk;  
  
    return (void *)(ptr->address); 
}

void my_free(void *p)
{
	block *this_chunk = g_chunk_head;  
    block *pre_chunk = NULL;  
    
    while(this_chunk)  
    {  
    	if(this_chunk->next && this_chunk->next->address == p)
    		pre_chunk = this_chunk;
        if(this_chunk->address == p)  
            break;  
        this_chunk = this_chunk->next;  
    }

    block *next_chunk = this_chunk->next; 
    
    if(this_chunk->in_use == USED)  
    {  
        this_chunk->in_use = FREE;  
    }  
    
    /* merge right chunk*/  
    if(next_chunk && next_chunk->in_use == FREE)  
    {  
        this_chunk->next = next_chunk->next;  
        this_chunk->size = this_chunk->size + next_chunk->size;  
    }  
  
    if(pre_chunk && pre_chunk->in_use == FREE)  
    {  
        pre_chunk->next = this_chunk->next;  
        pre_chunk->size = pre_chunk->size + this_chunk->size;  
    }  
}

void print_free_memory()
{
	block *ptr = g_chunk_head;  
    int index = 0;  
    printf("---------------- chunk info start-----------------\n", index++);  
    while(ptr)  
    {  
        printf("\t-------- chunk[%d] ---------\n\n", index++);  
        printf("\t[address]:0x%p\n", ptr->address);  
        printf("\t[size(bytes)]:%d\n", ptr->size);  
        printf("\t[state]:%s\n", (ptr->in_use? "USED" : "FREE"));  
        ptr = ptr->next;  
    } 
	printf("---------------- chunk info end  -----------------\n\n", index++);  
 
}

void test0()
{
    int size;
    void *p1, *p2;

    puts("Test0");

    p1 = my_malloc(10);
    print_free_memory();

    p2 = my_malloc(20);
    print_free_memory();

    my_free(p1);
    print_free_memory();

    my_free(p2);
    print_free_memory();
}

void test1()
{
    void *array[POINTERS];
    int i;
    void *p;

    puts("Test1");
    for (i = 0; i < POINTERS; i++) {
        p = my_malloc(4);
        array[i] = p;
    }

    for (i = 0; i < POINTERS; i++) {
        p = array[i];
        my_free(p);
    }

    print_free_memory();
}

void test2()
{
    void *array[POINTERS];
    int i;
    void *p;

    puts("Test2");
    for (i = 0; i < POINTERS; i++) {
        p = my_malloc(4);
        array[i] = p;
    }

    for (i = POINTERS - 1; i >= 0; i--) {
        p = array[i];
        my_free(p);
    }

    print_free_memory();
}

void test3()
{
    void *array[POINTERS];
    int i;
    void *p;

    puts("Test3");
    for (i = 0; i < POINTERS; i++) {
        p = my_malloc(4);
        array[i] = p;
    }

    for (i = 0; i < POINTERS; i += 2) {
        p = array[i];
        my_free(p);
    }

    for (i = 1; i < POINTERS; i += 2) {
        p = array[i];
        my_free(p);
    }

    print_free_memory();
}

int main()
{
    my_malloc_init();
	test0();
    test1();
    test2();
    test3();
    puts("Finished");
    return 0;
}
