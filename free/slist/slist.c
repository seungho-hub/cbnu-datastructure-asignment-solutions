#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "slist.h"

slist_t *slist_create(int capacity, size_t usize, int (*cmp)(void *e1, void *e2))
{
	slist_t *h = malloc(sizeof(slist_t));
	h->arr = calloc(capacity, usize);
	h->capacity = capacity;
	h->size = 0;
	h->usize = usize;
	h->cmp = cmp;

	return h;
}

void slist_free(slist_t *h)
{
	free(h->arr);
	free(h);
}

int slist_size(slist_t *h)
{
	return h->size;
}

int cmp(slist_t *h, int a, int b)
{
	return h->cmp(h->arr + a * h->usize, h->arr + b * h->usize);
}

int slist_top(slist_t *h, void *buf)
{
	if (h->size == 0)
		return 0;

	memcpy(buf, h->arr, h->usize);
	return 1;
}

int
slist_pop (slist_t * h, void * buf)
{
	if (h->size == 0)
		return 0;


	memcpy(buf, h->arr, h->usize);

	int i = 0;

	// 단순하게 그냥 한칸씩 당기면 된다 즉, root 또한 덮어쓰면 된다.
	for (i = 0 ; i < h->size - 1; i++) {
		memcpy(	h->arr + i * h->usize, 
			h->arr + (i + 1) * h->usize,
			h->usize);
	}

	// size를 줄인다. 이 시점에 last index에는 last index의 앞 element와 동일한 value가 들어있다.
	h->size = h->size - 1;

	return 1 ;
}

// 얘는 heap과 다르게 0부터 채워지기 때문에 array의 last index가 비워져있는 상태다.
int
slist_push (slist_t * h, void * buf) 
{
	if (h->size == h->capacity) 
		return 0 ;
	
	int p = 0 ;

	// buf가 삽입될 자리를 찾는다.
	for (p = 0 ; p < h->size ; p++) {
		// buf의 priority가 더 높다면 break;
		if (0 < h->cmp(h->arr + p * h->usize, buf)) {
			break ;
		}
	}

	// buf가 들어갈 자리 부터 뒤에있는 원소들을 모두 1칸씩 뒤로민다.
	int i ;
	for (i = h->size ; i > p ; i--) {
		memcpy(	h->arr + i * h->usize, 
			h->arr + (i - 1) * h->usize,
			h->usize) ;
	}

	// 빈 자리에 buf를 삽입한다.
	memcpy(h->arr + p * h->usize, buf, h->usize) ;
	h->size++ ;

	return 1 ;
}
