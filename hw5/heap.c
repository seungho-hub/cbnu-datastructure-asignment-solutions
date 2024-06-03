#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "heap.h"

// capcity와 size의 관계 정의가 중요하다.
// heap의 capacity value 그 자체는 capacity가 맞다. 하지만 left, right child 연산을 위해 가장 앞 0 index는 비워둔다.
// heap의 size value는 element의 개수 그 자체다.
heap_t *
heap_create(int capacity, size_t usize, int (*cmp)(void *e1, void *e2))
{
	heap_t *h = malloc(sizeof(heap_t));
	h->arr = calloc(capacity + 1, usize);
	h->capacity = capacity;
	h->size = 0;
	h->usize = usize;
	h->cmp = cmp;
	return h;
}

void heap_free(heap_t *h)
{
	free(h->arr);
	free(h);
}

int heap_size(heap_t *h)
{
	return h->size;
}

int parent(int i)
{
	return i / 2;
}

int left(int i)
{
	return i * 2;
}

int right(int i)
{
	return i * 2 + 1;
}

void *
arr(heap_t *h, int i)
{
	return h->arr + h->usize * i;
}

void swap(heap_t *h, int a, int b)
{
	char *tmp = (char *)malloc(h->usize);
	memcpy(tmp, arr(h, a), h->usize);
	memcpy(arr(h, a), arr(h, b), h->usize);
	memcpy(arr(h, b), tmp, h->usize);
	free(tmp);
}

int cmp(heap_t *h, int a, int b)
{
	return h->cmp(h->arr + a * h->usize, h->arr + b * h->usize);
}

int heap_top(heap_t *h, void *buf)
{
	if (h->size == 0)
		return 0;
	memcpy(buf, arr(h, 1), h->usize);
	return 1;
}

int heap_pop(heap_t *h, void *buf)
{
	/* FIXME: fix this function for Task 1 */
	if (h->size == 0)
		return 0;

	memcpy(buf, arr(h, 1), h->usize);

	swap(h, 1, h->size);
	h->size--;

	int i = 1;
	while ((left(i) <= h->size && cmp(h, i, left(i)) > 0) ||
				 right(i) <= h->size && cmp(h, i, right(i)) > 0)
	{

		int r = i;
		if (left(i) <= h->size && cmp(h, r, left(i)) > 0)
		{
			r = left(i);
		}
		if (right(i) <= h->size && cmp(h, r, right(i)) > 0)
		{
			r = right(i);
		}

		swap(h, i, r);

		i = r;
	}

	// added
	// heap_pop 이 모두 수행된 후 남은 element의 수 즉, size가 capacity의 1/4보다 작다면 capacity를 절반으로 줄인다.
	if (h->size < (h->capacity) / 4)
	{
		h->capacity = h->capacity / 2;
		h->arr = realloc(h->arr, (h->capacity + 1) * h->usize);
	}
	return 1;
}

int heap_push(heap_t *h, void *buf)
{
	/* FIXME: fix this function for Task 1 */

	// removed
	// 이 시점에 heap의 size와 capacity가 동일할 일은 사라진다.

	h->size += 1;

	memcpy(arr(h, h->size), buf, h->usize);

	int i;
	for (i = h->size; i > 1; i = parent(i))
	{
		if (cmp(h, parent(i), i) < 0)
		{
			break;
		}
		else
		{
			swap(h, parent(i), i);
		}
	}

	// added
	// heap이 꽉찼다면, heap의 공간을 2배로 늘린다.
	if (h->size == h->capacity)
	{
		h->capacity = h->capacity * 2;

		// capacity는 2배가 되었다.
		h->arr = realloc(h->arr, (h->capacity + 1) * h->usize);
	}

	return 1;
}

int heap_remove(heap_t *h, void *buf)

// heap_remove eliminates an element in the heap
// if the element is equivalent to the element at buf.
// This function returns 1 if an element is removed.
// Otherwise, returns 0.

{
	/*TODO: Task 2*/
	if (h->size == 0)
		return 0;

	int targetIndex = 0;

	for (int i = 1; i < h->size; i++)
	{
		// buf와 동일한 element를 발견했다면
		if (h->cmp(arr(h, i), buf) == 0)
		{
			targetIndex = i;
			break;
		}
	}

	// heap에 buf와 동일한 element가 존재하지 않음.
	if (!targetIndex)
		return 0;

	// heap의 terminal node랑 swap한 후 size를 줄여 잃어버린다.
	swap(h, targetIndex, h->size);
	h->size--;

	int i = targetIndex;

	while ((left(i) <= h->size && cmp(h, i, left(i)) > 0) ||
				 right(i) <= h->size && cmp(h, i, right(i)) > 0)
	{
		int r = i;
		if (left(i) <= h->size && cmp(h, r, left(i)) > 0)
		{
			r = left(i);
		}
		if (right(i) <= h->size && cmp(h, r, right(i)) > 0)
		{
			r = right(i);
		}

		swap(h, i, r);

		i = r;
	}

	return 1;
}
