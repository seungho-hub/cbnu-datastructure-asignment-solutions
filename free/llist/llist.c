#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _list_node_t
{
    char *data;
    struct _list_node_t *next;
} llist_node_t;

typedef struct
{
    llist_node_t *first;
    int size;
} llist_t;

llist_t *create_llist()
{
    llist_t *l = (llist_t *)malloc(sizeof(llist_t));
    l->first = NULL;
    l->size = 0;

    return l;
}

int insert_llist(llist_t *l, char *s)
{
    llist_node_t *n = (llist_node_t *)malloc(sizeof(llist_node_t));
    n->data = s;
    n->next = NULL;

    if (l->first == NULL)
    {
        l->first = n;
        l->size = 1;
        return 1;
    }

    llist_node_t *last = l->first;
    while (!(last->next == NULL))
    {
        last = last->next;
    }

    last->next = n;
    l->size++;

    return 1;
}

char *delete_llist(llist_t *l, char *s)
{
    llist_node_t *curr;
    llist_node_t *prev = NULL;

    for (curr = l->first; curr != NULL; curr = curr->next)
    {
        if (strcmp(curr->data, s) == 0)
            break;
        prev = curr;
    }

    if (curr == NULL)
        return NULL;

    char *r;
    r = curr->data;
    // first element라면 다르게 처리해줘야한다.
    if (curr == l->first)
    {
        l->first = curr->next;
    }
    // first element가 아니라면 prev와 target의 next를 연결해주면된다.
    else
    {
        prev->next = curr->next;
    }

    free(curr);

    return r;
}

void print_llist(llist_t *l)
{
    llist_node_t *curr = l->first;

    while (curr->next)
    {
        printf("%s ", curr->data);
        curr = curr->next;
    }
    printf("\n");
}

void free_llist(llist_t *l)
{
    llist_node_t *curr;
    llist_node_t *next;

    curr =  l->first;
    
    while (curr)
    {
        next = curr->next;
        free(curr);
        curr = next;
    }

    free(l);
}

int main()
{
    llist_t *l = create_llist();

    insert_llist(l, "BAT");
    insert_llist(l, "CAT");
    insert_llist(l, "EAT");
    insert_llist(l, "FAT");
    insert_llist(l, "GAT");
    insert_llist(l, "HAT");
    insert_llist(l, "VAT");
    insert_llist(l, "WAT");

    print_llist(l);

    delete_llist(l, "FAT");
    delete_llist(l, "VAT");

    free_llist(l);
}