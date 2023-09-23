#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"

// Define the structure for a single node in the linked list
// typedef struct node {
//     elem value;
//     struct node *next;
// } node_t;

// // Define the list structure, which contains a pointer to the first node
// struct list {
//     node_t *head;
// };

list_t *list_alloc() { 
    list_t *new_list = (list_t *)malloc(sizeof(list_t));
    if (new_list != NULL) {
        new_list->head = NULL;
    }
    return new_list;
}

void list_free(list_t *l) {
    if (l != NULL) {
        while (l->head != NULL) {
            node_t *temp = l->head;
            l->head = l->head->next;
            free(temp);
        }
        free(l);
        l = NULL; // Set l to NULL to make it clear that the list is freed
    }
}




// void list_print(list_t *l) {
//     if (l != NULL) {
//         node_t *current = l->head;
//         while (current != NULL) {
//             printf("%d -> ", current->value);
//             current = current->next;
//         }
//         printf("NULL\n");
//     }
// }

void list_print(list_t *l) {
    if (l != NULL) {
        node_t *current = l->head;
        if (current == NULL) {
            printf("Empty list\n");
            return; // No need to proceed further
        }
        while (current != NULL) {
            printf("%d -> ", current->value);
            current = current->next;
        }
        printf("NULL\n");
    } else {
        printf("Invalid list\n");
    }
}


int list_length(list_t *l) {
    int length = 0;
    if (l != NULL) {
        node_t *current = l->head;
        while (current != NULL) {
            length++;
            current = current->next;
        }
    }
    return length;
}

char *listToString(list_t *l) {
    char *buf = (char *)malloc(sizeof(char) * 1024);
    char tbuf[20];

    node_t *curr = l->head;
    while (curr != NULL) {
        sprintf(tbuf, "%d->", curr->value);
        curr = curr->next;
        strcat(buf, tbuf);
    }
    strcat(buf, "NULL");
    return buf;
}

void list_add_to_front(list_t *l, elem value) {
    if (l != NULL) {
        node_t *new_node = (node_t *)malloc(sizeof(node_t));
        if (new_node != NULL) {
            new_node->value = value;
            new_node->next = l->head;
            l->head = new_node;
        }
    }
}

void list_add_to_back(list_t *l, elem value) {
    if (l != NULL) {
        node_t *new_node = (node_t *)malloc(sizeof(node_t));
        if (new_node != NULL) {
            new_node->value = value;
            new_node->next = NULL;
            
            if (l->head == NULL) {
                l->head = new_node;
            } else {
                node_t *current = l->head;
                while (current->next != NULL) {
                    current = current->next;
                }
                current->next = new_node;
            }
        }
    }
}

void list_add_at_index(list_t *l, elem value, int index) {
    if (l != NULL && index >= 0) {
        if (index == 0) {
            list_add_to_front(l, value);
        } else {
            node_t *new_node = (node_t *)malloc(sizeof(node_t));
            if (new_node != NULL) {
                new_node->value = value;
                new_node->next = NULL;
                
                node_t *current = l->head;
                int i = 0;
                while (current != NULL && i < index - 1) {
                    current = current->next;
                    i++;
                }
                if (current != NULL) {
                    new_node->next = current->next;
                    current->next = new_node;
                } else {
                    free(new_node); // Invalid index, free the allocated memory
                }
            }
        }
    }
}

elem list_remove_from_back(list_t *l) {
    if (l != NULL && l->head != NULL) {
        if (l->head->next == NULL) {
            elem value = l->head->value;
            free(l->head);
            l->head = NULL;
            return value;
        } else {
            node_t *current = l->head;
            while (current->next->next != NULL) {
                current = current->next;
            }
            elem value = current->next->value;
            free(current->next);
            current->next = NULL;
            return value;
        }
    }
    return -1; // Return a default value for an empty list or NULL list
}

elem list_remove_from_front(list_t *l) {
    if (l != NULL && l->head != NULL) {
        node_t *temp = l->head;
        elem value = temp->value;
        l->head = l->head->next;
        free(temp);
        return value;
    }
    return -1; // Return a default value for an empty list or NULL list
}


elem list_remove_at_index(list_t *l, int index) {
    if (l != NULL && index >= 0 && l->head != NULL) {
        if (index == 0) {
            return list_remove_from_front(l);
        } else {
            node_t *current = l->head;
            int i = 0;
            while (current != NULL && i < index) {
                if (i == index - 1 && current->next != NULL) {
                    node_t *temp = current->next;
                    elem value = temp->value;
                    current->next = temp->next;
                    free(temp);
                    return value;
                }
                current = current->next;
                i++;
            }
        }
    }
    return -1; // Return a default value for an invalid index or empty list
}


bool list_is_in(list_t *l, elem value) {
    if (l != NULL) {
        node_t *current = l->head;
        while (current != NULL) {
            if (current->value == value) {
                return true; // Value found in the list
            }
            current = current->next;
        }
    }
    return false; // Value not found in the list or list is empty
}

elem list_get_elem_at(list_t *l, int index) {
    if (l != NULL && index >= 0) {
        node_t *current = l->head;
        int i = 0;
        while (current != NULL && i < index) {
            current = current->next;
            i++;
        }
        if (current != NULL) {
            return current->value; // Return the value at the specified index
        }
    }
    return -1; // Return a default value for an invalid index or empty list
}

int list_get_index_of(list_t *l, elem value) {
    if (l != NULL) {
        node_t *current = l->head;
        int index = 0;
        while (current != NULL) {
            if (current->value == value) {
                return index; // Return the index of the value
            }
            current = current->next;
            index++;
        }
    }
    return -1; // Value not found in the list or list is empty
}
