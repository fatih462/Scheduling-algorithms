#include "../lib/queue.h"
#include <stdlib.h>
#include <stdio.h>

int queue_add(void *new_object, queue_object *queue)
{
	// TODO
	if(new_object == NULL || queue == NULL) {
		return 1;
	}

	queue_object *new = malloc(sizeof(queue_object));
	new->object = new_object;
    new->next = NULL;

    if (queue->next == NULL) {
        queue->next = new;
    } else {
        queue_object *temp = queue->next;
        queue->next = new;
        new->next = temp;
    }
	return 0;
}

void *queue_poll(queue_object *queue)
{
	// TODO
	if (queue == NULL || queue->next == NULL) {
        return NULL;
    }

	queue_object *temp1 = queue;
	queue_object *temp2;
	while(temp1->next != NULL) {
		temp2 = temp1;
		temp1 = temp1->next;
	}
	temp2->next = NULL;
	void *object = temp1->object;
	free(temp1);

	return object;
}

queue_object *new_queue()
{
	// TODO
	queue_object *queue = malloc(sizeof(queue_object));

    queue->object = NULL;
    queue->next = NULL;

    return queue;
	
}

void free_queue(queue_object *queue)
{
	// TODO
	if (queue == NULL) return;

	queue_object *temp1 = queue;
	queue_object *temp2;
	while(temp1 != NULL) {
		temp2 = temp1->next;
		temp1->object = NULL;
		temp1->next = NULL;
		free(temp1);
		temp1 = temp2;
	}
}

void *queue_peek(queue_object *queue)
{
	// TODO
	if (queue == NULL || queue->next == NULL) {
        return NULL;
    }
	
    return queue->next->object;
}
