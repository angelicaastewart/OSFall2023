#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

int main() {
    list_t *mylist;

    // Test 1: Print an empty list
    printf("Test 1: Printing an empty list\n");
    mylist = list_alloc();
    list_print(mylist);
    list_free(mylist);

    // Test 2: Add elements to the front and print the list
    printf("\nTest 2: Add elements to the front and print the list\n");
    mylist = list_alloc();
    list_add_to_front(mylist, 10);
    list_add_to_front(mylist, 20);
    list_add_to_front(mylist, 30);
    list_print(mylist);
    if(strcmp("30->20->10->NULL",listToString(mylist)) != 0)
    {
         printf("list_add_to_front : FAILED\n");
    }
    list_free(mylist);

    // Test 3: Add elements to the back and print the list
    printf("\nTest 3: Add elements to the back and print the list\n");
    mylist = list_alloc();
    list_add_to_back(mylist, 10);
    list_add_to_back(mylist, 20);
    list_add_to_back(mylist, 30);
    list_print(mylist);
    if(strcmp("10->20->30->NULL",listToString(mylist)) != 0)
    {
         printf("list_add_to_back : FAILED\n");
    }
    list_add_to_front(mylist, 60);
    list_add_to_front(mylist, 50);
    list_add_to_front(mylist, 40);
    list_print(mylist);
    if(strcmp("40->50->60->10->20->30->NULL",listToString(mylist)) != 0)
    {
         printf("list_add_to_back : FAILED\n");
    }
    list_free(mylist);

    // Test 4: Add elements at specific indices and print the list
    printf("\nTest 4: Add elements at specific indices and print the list\n");
    mylist = list_alloc();
    list_add_at_index(mylist, 10, 0);
    list_add_at_index(mylist, 30, 1);
    list_add_at_index(mylist, 20, 1);
    list_print(mylist);
    if(strcmp("10->20->30->NULL",listToString(mylist)) != 0)
    {
         printf("list_add_at_index : FAILED\n");
    }
    list_free(mylist);

    // Test 5: Remove elements from the front and print the list
    printf("\nTest 5: Remove elements from the front and print the list\n");
    mylist = list_alloc();
    list_add_to_front(mylist, 10);
    list_add_to_front(mylist, 20);
    list_add_to_front(mylist, 30);
    list_remove_from_front(mylist);
    list_print(mylist);
    if(strcmp("20->10->NULL",listToString(mylist)) != 0)
    {
         printf("list_remove_from_front : FAILED\n");
    }
    list_free(mylist);

    // Test 6: Remove elements from the back and print the list
    printf("\nTest 6: Remove elements from the back and print the list\n");
    mylist = list_alloc();
    list_add_to_back(mylist, 10);
    list_add_to_back(mylist, 20);
    list_add_to_back(mylist, 30);
    list_remove_from_back(mylist);
    list_print(mylist);
    if(strcmp("10->20->NULL",listToString(mylist)) != 0)
    {
         printf("list_remove_from_back : FAILED\n");
    }
    list_free(mylist);

    // Test 7: Remove elements at specific indices and print the list
    printf("\nTest 7: Remove elements at specific indices and print the list\n");
    mylist = list_alloc();
    list_add_to_front(mylist, 10);
    list_add_to_front(mylist, 20);
    list_add_to_front(mylist, 30);
    list_remove_at_index(mylist, 1);
    if(strcmp("30->10->NULL",listToString(mylist)) != 0)
    {
         printf("list_remove_at_index : FAILED\n");
    }
    list_print(mylist);
    list_free(mylist);

    // Test 8: Check if an element is in the list
    printf("\nTest 8: Check if an element is in the list\n");
    mylist = list_alloc();
    list_add_to_back(mylist, 10);
    list_add_to_back(mylist, 20);
    list_add_to_back(mylist, 30);
    printf("Is 20 in the list? %d\n", list_is_in(mylist, 20));
    printf("Is 40 in the list? %d\n", list_is_in(mylist, 40));
    list_free(mylist);

    // Test 9: Get elements at specific indices
    printf("\nTest 9: Get elements at specific indices\n");
    mylist = list_alloc();
    list_add_to_back(mylist, 10);
    list_add_to_back(mylist, 20);
    list_add_to_back(mylist, 30);
    printf("Element at index 1: %d\n", list_get_elem_at(mylist, 1));
    printf("Element at index 3: %d\n", list_get_elem_at(mylist, 3));
    list_free(mylist);

    // Test 10: Get the index of a specific element
    printf("\nTest 10: Get the index of a specific element\n");
    mylist = list_alloc();
    list_add_to_back(mylist, 10);
    list_add_to_back(mylist, 20);
    list_add_to_back(mylist, 30);
    printf("Index of 20: %d\n", list_get_index_of(mylist, 20));
    printf("Index of 40: %d\n", list_get_index_of(mylist, 40));
    list_free(mylist);

    // Test 11: Edge case - Adding elements with negative indices
    printf("\nTest 11: Adding elements with negative indices\n");
    mylist = list_alloc();
    list_add_at_index(mylist, 10, -1);
    list_add_at_index(mylist, 20, -2);
    list_print(mylist);
    list_free(mylist);

    // Test 12: Edge case - Removing elements from an empty list
    printf("\nTest 12: Removing elements from an empty list\n");
    mylist = list_alloc();
    list_remove_from_front(mylist);
    list_remove_from_back(mylist);
    list_remove_at_index(mylist, 0);
    list_print(mylist);
    list_free(mylist);

    return 0;
}