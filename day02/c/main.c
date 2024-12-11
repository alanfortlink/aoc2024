#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

struct node;
struct list;

typedef struct node {
  int value;
  struct node *next;
  struct node *previous;
} node;

typedef struct list {
  node *head;
  node *tail;
} list;

void add_node(list *l, node *new_node) {
  if (l->head == NULL) {
    l->head = new_node;
    l->tail = new_node;
    return;
  }

  l->tail->next = new_node;
  new_node->previous = l->tail;
  l->tail = new_node;
}

void add_value(list *l, int value) {
  node *new_node = (node *)malloc(sizeof(node));
  new_node->value = value;
  new_node->next = NULL;
  new_node->previous = NULL;

  add_node(l, new_node);
}

void free_nodes(node* node){
  if (node == NULL) {
    return;
  }

  free_nodes(node->next);
  free(node);
}

void free_list(list* l) {
  free_nodes(l->head);
  free(l);
}

void add_after(list *l, node *prev, node *node) {
  if (prev == NULL) {
    add_node(l, node);
    return;
  }

  node->next = prev->next;
  node->previous = prev;

  if (prev->next != NULL) {
    prev->next->previous = node;
  }

  prev->next = node;
}

void remove_node(list *l, node *node) {
  if (l->head == node) {
    l->head->next->previous = NULL;
    l->head = l->head->next;
    return;
  }

  if (l->tail == node) {
    l->tail->previous->next = NULL;
    l->tail = l->tail->previous;
    return;
  }

  node->previous->next = node->next;
  node->next->previous = node->previous;
}

list *get_list(char *line) {
  list *l = (list *)malloc(sizeof(list));

  char buf[10];
  int index = 0;

  for (size_t i = 0; true; i++) {
    if (line[i] == ' ' || line[i] == '\0' || line[i] == '\n') {
      buf[index] = '\0';
      add_value(l, atoi(buf));
      index = 0;

      if (line[i] == '\n') {
        break;
      }

      continue;
    }

    buf[index++] = line[i];
  }

  return l;
}

bool is_safe(node *root, int min, int max) {
  if (root == NULL) {
    return true;
  }

  if (root->next == NULL) {
    return true;
  }

  node *next = root->next;
  int diff = next->value - root->value;

  if (diff < min || diff > max) {
    return false;
  }

  return is_safe(next, min, max);
}

bool q1_is_safe(node *root) {
  return is_safe(root, 1, 3) || is_safe(root, -3, -1);
}

void print_list(node *root) {
  if (root == NULL) {
    return;
  }

  printf(" %d", root->value);
  print_list(root->next);
}

bool q2_is_safe(list *l) {
  if (q1_is_safe(l->head)) {
    return true;
  }

  node *prev = NULL;
  node *node = l->head;
  for (; node != NULL; prev = node, node = node->next) {
    if (prev == NULL) {
      if (q1_is_safe(node->next)) {
        return true;
      }
      continue;
    }

    remove_node(l, node);
    if (q1_is_safe(l->head)) {
      return true;
    }
    add_after(l, prev, node);
  }

  return false;
}

int main(int argc, char **argv) {
  if (argc != 2) {
    printf("Usage: %s <file_location>\n", argv[0]);
    return 1;
  }

  FILE *file = fopen(argv[1], "r");
  // FILE *file = fopen("../example.txt", "r");
  if (file == NULL) {
    printf("Failed to open file %s\n", argv[1]);
    return 2;
  }

  char line[256];
  size_t safe = 0;

  while (fgets(line, sizeof(line), file)) {
    bool isQ1 = false;
    list *l = get_list(line);

    if (isQ1) {
      if (q1_is_safe(l->head)) {
        safe += 1;
      }
    } else {
      if (q2_is_safe(l)) {
        safe += 1;
      }
    }

    free_list(l);
  }

  printf("%zu\n", safe);
}
