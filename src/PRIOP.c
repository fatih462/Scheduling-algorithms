#include "../lib/PRIOP.h"
#include <stdio.h>

static queue_object *PRIOP_queue;
//You can add more global variables here
void delete_from_queue(queue_object *queue, process *process) {
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

process *highest_prio(queue_object *queue) {
	if(queue == NULL || queue->next == NULL) {
		return NULL;
	}
	queue_object *temp = queue->next;
    
    process *highest = (process *)temp->object;
    while (temp != NULL) {
        process *current_process = (process *)temp->object;
        if (current_process->priority >= highest->priority) {
            highest = current_process;
        }
        temp = temp->next;
    }
    return highest;
}

process *PRIOP_tick(process *running_process)
{
	// TODO
	process *process = highest_prio(PRIOP_queue);
	if(running_process != NULL && running_process->time_left == 0) {
		delete_from_queue(PRIOP_queue, running_process);
		running_process = NULL;
	}
	
	if(process != NULL && running_process != NULL && process->priority > running_process->priority) {
		running_process = process;
	}


	if (running_process == NULL) {
		process = highest_prio(PRIOP_queue); //after deleting
		running_process = process;
	}
	if(running_process != NULL) {
		running_process->time_left--;
	}
	
	return running_process;
}

int PRIOP_startup()
{
	// TODO
	PRIOP_queue = new_queue();
	if (PRIOP_queue == NULL) {
		return 1;
	}
	return 0;
}

process *PRIOP_new_arrival(process *arriving_process, process *running_process)
{
	// TODO
	if (arriving_process != NULL) {
        queue_add(arriving_process, PRIOP_queue);
    }

    return running_process;
}

void PRIOP_finish()
{
	// TODO
	free_queue(PRIOP_queue);
}
