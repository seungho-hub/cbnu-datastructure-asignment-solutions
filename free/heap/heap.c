#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "heap.h"

// unit에 따라 compare하는 기준이 달라질 수 있으니 compare function은 parameter로 전달받는다.
heap_t *
heap_create (int capacity, size_t usize, int (* cmp)(void *e1, void *e2)) 
{
	heap_t * h = malloc(sizeof(heap_t)) ;
	h->arr = calloc(capacity + 1, usize) ;
	h->capacity = capacity ;
	h->size = 0 ;
	h->usize = usize ;
	h->cmp = cmp ;
	return h ;
}

void heap_free(heap_t *h){
    free(h->arr);
    free(h);
}

int heap_size(heap_t *h){
    return h->size;
}

// i index의 parent index를 return한다.
int parent(int i){
    return i / 2;
}

int left(int i ){
    return i * 2;
}

int right(int i ){
    return i * 2 + 1;
}

// i index부터 arr의 pointer를 return한다.
void *arr (heap_t *h, int i){
    return h->arr + h->usize * i;
}

void swap (heap_t * h, int a, int b){
    // dynamic length string이기 때문에 unit size에 맞도록 할당해준다.
    char *tmp = (char *)malloc(h->usize);
    
    // src인 arr(h, a)가 a index부터의 array이기 때문에 h->usize로 한 개의 element만을 dest인 temp에 복사한다.
    memcpy(tmp, arr(h, a), h->usize);
    memcpy(arr(h, a), arr(h, b), h->usize);
    memcpy(arr(h, b), tmp, h->usize);
    free(tmp);
}

int cmp(heap_t *h, int a, int b){
    // element들로 heap의 compare function을 call한다.
    return h->cmp(h->arr + (a * h->usize), h->arr+ (b*h->usize));
}

int heap_top(heap_t *h, void *buf){
    if(h->size == 0) return 0;

    memcpy(buf, arr(h, 1), h->usize);
    return 1;
}

int heap_pop(heap_t *h, void *buf){
    if(h->size == 0) return 0;

    // buf에 root value를
    memcpy(buf, arr(h, 1), h->usize);
    
    // root value와 terminal node(가장 마지막 node)를 swap한 후 size를 줄여 
    // root value를 제거한다.
    swap(h, 1, h->size);
    h->size--;

    // root value의 priority는 terminal node였기 때문에 priority가 가장 낮을 
    // 수밖에 없다. 하지만 root에 존재하기 때문에 heap이 깨진상태. heapify를 해줘야한다.

    // 얘는 current node를 의미한다.
    int i = 1;

    // current node가 존재하고, left 혹은 right child보다 우선순위가 낮은 즉,
    // heapify가 필요한 상황이라면 루프를 반복한다.
    while((left(i) <= h->size && cmp(h, i, left(i)) > 0)
    || right(i) <= h->size && cmp(h, i, right(i)) > 0){
        // 아래에 비교연산이 있기 때문에 left로 먼저 박는 건 의미 없다.
        int r;

        // 
        if(right(i) <= h->size && cmp(h, left(i), right(i)) < 0){
            r = right(i);
        }else{
            r = left(i);
        }

        // current node와 결정된 child를 swap한다.
        swap(h, i, r);

        // swap되었기 때문에 swap한 child가 current node가 된다.
        i = r;
    }

    return 1;
}

int heap_push(heap_t *h, void *buf){
    if(h->size == h->capacity) return 0;
    
    // size를 늘린다.
    // 현재 arr의 size에 의하면 arr의 last는 쓰레기값이 들어있는 상태다.
    h->size += 1;

    // arr의 끝 pointer에 buf를 push한다.
    memcpy(arr(h, h->size), buf, h->usize);

    int i;

    // push된 element부터 parent로 타고올라가며 parent보다 priority가 높다면 swap한다.
    for(i=h->size; i > 1; i = parent(i)){
        if(cmp(h, parent(i), i) < 0) break;
        else swap(h, parent(i), i);
    }
    
    return 1;
}