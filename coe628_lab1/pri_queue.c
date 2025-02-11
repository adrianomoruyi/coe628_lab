#include <stdlib.h>
#include <stdio.h>
#include "pri_queue.h"
/** @file pri_queue.c */
static Node_ptr_t head = NULL;
/**
 * Insert a Node into a priority queue.
 * @param priority
 * @param data
 * @author YOUR NAME
 */
void PQ_insert(int priority, char * data) {
    //New node is created
    Node_ptr_t new_node = (Node_ptr_t)malloc(sizeof(Node_t));
    new_node->priority = priority;
    new_node->data = data;
    new_node->next = NULL;

    //If the queue is empty or the new node has highest priority
    if (head == NULL || head->priority < priority) 
    {
        new_node->next = head;
        head = new_node;
    } 
    else //Find the right position in the list
    {
        Node_ptr_t current = head;
        while (current->next != NULL && current->next->priority >= priority) {
            current = current->next;
        }
        new_node->next = current->next;
        current->next = new_node;
    }
}
/**
 * Delete and return the node with the highest priority.
 * @return The highest priority Node.
 */
Node_ptr_t PQ_delete() {
    if (head == NULL) 
    {
        return NULL; //When the queue is empty, return null
    }
    Node_ptr_t highest_priority_node = head; //Remove head
    head = head->next; //Head is now next node
    return highest_priority_node; //Return removed node
}


/**
 * Do NOT modify this function.
 * @return A pointer to the head of the list.  (NULL if list is empty.)
 */
Node_ptr_t PQ_get_head() {
    return head;
}

/**
 * Do NOT modify this function.
 * @return the number of items in the queue
 */
int PQ_get_size() {
    int size = 0;
    Node_ptr_t tmp;
    for(tmp = head; tmp != NULL; tmp = tmp->next, size++)
        ;
    return size;
}


