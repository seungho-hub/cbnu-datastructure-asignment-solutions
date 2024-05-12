#include <stdlib.h>
#include <stdio.h>
#include "tree.h"

tree_t *tree_create(char data)
{
  node_t *root;
  root = (node_t *)malloc(sizeof(node_t));
  root->data = data;
  root->down = NULL;
  root->next = NULL;
  return root;
}

tree_t *tree_add_child(tree_t *parent, tree_t *child)
{
  if (parent->down == NULL)
  {
    parent->down = child;
    return parent;
  }

  tree_t *last;

  for (last = parent->down; last->next != NULL; last = last->next)
    ;

  last->next = child;

  return parent;
}

tree_t *tree_search(tree_t *t, char key)
{
  if (t->data == key)
  {
    return t;
  }

  tree_t *i;

  for (i = t->down; i != NULL; i = i->next)
  {
    tree_t *r;
    r = tree_search(i, key);
    if (r)
      return r;
  }

  return NULL;
}

void tree_delete(tree_t *t)
{
  tree_t *i;
  for (i = t->down; i != NULL;)
  {
    tree_t *next;
    next = i->next;
    free(i);
    i = next;
  }

  free(t);
}

void tree_height(tree_t *t)
{
}

void tree_print(tree_t *t)
{
  printf("%c", t->data);

  if (t->down)
  {
    printf("(");
    tree_print(t->down);
    printf(")");
  }

  if (t->next)
  {
    printf("-");
    tree_print(t->next);
  }
}