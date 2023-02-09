#include <stddef.h>
#include "ll_cycle.h"
#include<stdlib.h>
/*
This function walk the *head to next list and return the ptr
*/
node* next(node **head){
    *head = (*head)->next;
    return *head;
}

int ll_has_cycle(node *head) {
    node *fast_ptr = head;
    node *slow_ptr = head;

    while(fast_ptr && next(&fast_ptr) && next(&fast_ptr)){ 
        next(&slow_ptr);
        if(slow_ptr == fast_ptr){
            return 1;
        }
    }

    
    return 0;
}
