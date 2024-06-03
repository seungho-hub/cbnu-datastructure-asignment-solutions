#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "heap.h"

int string_cmp(void *e1, void *e2)
{
	char *s1 = *((char **)e1);
	char *s2 = *((char **)e2);

	return strcmp(s1, s2);
}

int main()
{
	char *inputs[7] = {"necessary", "correct", "absent", "type", "because", "further", 0x0};
	int i;

	heap_t *h = heap_create(6, sizeof(char *), string_cmp);

	for (i = 0; inputs[i] != 0x0; i++)
	{
		heap_push(h, &(inputs[i]));
		printf("push : %s\n", inputs[i]);
		printf("size : %d, capacity : %d\n", h->size, h->capacity);
	}

	char *doesNotExistString = "doesNotExistString";
	printf(heap_remove(h, &inputs[1]) ? "removed \"%s\"\n" : "can not found \"%s\"\n", inputs[1]);
	printf(heap_remove(h, &doesNotExistString) ? "removed \"%s\"\n" : "can not found \"%s\"\n", doesNotExistString);
	printf(heap_remove(h, &inputs[5]) ? "removed \"%s\"\n" : "can not found \"%s\"\n", inputs[5]);
	printf(heap_remove(h, &inputs[1]) ? "removed \"%s\"\n" : "can not found \"%s\"\n", inputs[1]);

	while (heap_size(h) > 0)
	{
		char *s = 0x0;
		heap_pop(h, &s);
		printf("popped : %s\n", s);
		printf("size : %d, capacity : %d\n", h->size, h->capacity);
	}

	heap_free(h);

	return 0;
}
