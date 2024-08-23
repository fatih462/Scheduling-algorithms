#include "../lib/SRTN.h"

static queue_object *SRTN_queue;
//You can add more global variables here
void delete_from_queue1(queue_object *queue, process *process) {
	if (queue == NULL || queue->next == NULL) {
		return;
	}
	queue_object *temp = queue->next;
	if (temp->object == process) {
        queue->next = queue->next->next;
        free(temp);
        return;
    }
    
    queue_object *temp1;
    while(temp->next != NULL) {
        if (temp->next->object == process) {
			temp1 = temp->next;
            temp->next = temp->next->next;
            free(temp1);
            return;
        }
        temp = temp->next;
    }
}

process *shortest_job(queue_object *queue) {
	if(queue == NULL || queue->next == NULL) {
		return NULL;
	}

	queue_object *temp = queue->next;
    
    process *shortest = (process *)temp->object;
    while (temp != NULL) {
        process *current_process = (process *)temp->object;
        if (current_process->time_left <= shortest->time_left) {
            shortest = current_process;
        }
        temp = temp->next;
    }
    return shortest;
}

process *SRTN_tick(process *running_process)
{
	// TODO
	process *process = shortest_job(SRTN_queue);
	if(running_process != NULL && running_process->time_left == 0) {
		delete_from_queue1(SRTN_queue, running_process);
		running_process = NULL;
	}
	
	if(process != NULL && running_process != NULL && process->time_left < running_process->time_left) {
		running_process = process;
	}


	if (running_process == NULL) {
		process = shortest_job(SRTN_queue); //after deleting
		running_process = process;
	}
	if(running_process != NULL) {
		running_process->time_left--;
	}

	return running_process;
}

int SRTN_startup()
{
	// TODO
	SRTN_queue = new_queue();
	if (SRTN_queue == NULL) {
		return 1;
	}
	return 0;
}

process *SRTN_new_arrival(process *arriving_process, process *running_process)
{
	// TODO
	if (arriving_process != NULL) {
        queue_add(arriving_process, SRTN_queue);
    }

    return running_process;
}

void SRTN_finish()
{
	// TODO
	free_queue(SRTN_queue);
}