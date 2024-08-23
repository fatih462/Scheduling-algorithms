#include "../lib/HRRN.h"

static queue_object *HRRN_queue;

//You can add more global variables and structs here
void delete_from_queue2(queue_object *queue, HRRN_process *process) {
    if (queue == NULL || queue->next == NULL) {
		return;
	}
	queue_object *temp = queue->next;
	if (temp->object == process) {
        queue->next = queue->next->next;
		free(process);
        free(temp);
        return;
    }
    
    queue_object *temp1;
    while(temp->next != NULL) {
        if (temp->next->object == process) {
			temp1 = temp->next;
            temp->next = temp->next->next;
			free(process);
            free(temp1);
            return;
        }
        temp = temp->next;
    }
}

float calculate_rr(unsigned int waiting_time, unsigned int duration) {
    return (float) (waiting_time + duration) / duration;
}

HRRN_process *find_highest_rr(queue_object *queue) {
	if (queue == NULL || queue->next == NULL) {
        return NULL;
    }
	queue_object *temp = HRRN_queue->next;
    HRRN_process *highest_rr = (HRRN_process *) temp->object;
    while (temp->next != NULL) {
        HRRN_process *hrrn_process = (HRRN_process *) temp->next->object;
        if (hrrn_process->rr >= highest_rr->rr) {
            highest_rr = hrrn_process;
        }
        temp = temp->next;
    }
    return highest_rr;
}

process *HRRN_tick(process *running_process)
{
	// TODO
	HRRN_process *highest_rr = find_highest_rr(HRRN_queue);
    
    if (running_process == NULL || running_process->time_left == 0) {
        if (highest_rr != NULL) {
            running_process = highest_rr->this_process;
            delete_from_queue2(HRRN_queue, highest_rr);
        }
        else {
            running_process = NULL;
        }
    }
    
    if (HRRN_queue != NULL) {
        queue_object *temp = HRRN_queue->next;
        while (temp != NULL) {
            HRRN_process *hrrn_process = (HRRN_process *)temp->object;
            hrrn_process->waiting_time++;
            hrrn_process->rr = calculate_rr(hrrn_process->waiting_time, hrrn_process->this_process->time_left);
            temp = temp->next;
        }
    }
    
    if (running_process != NULL) {
        running_process->time_left--;
    }
    
    return running_process;
}

int HRRN_startup()
{
	// TODO
	HRRN_queue = new_queue();
	if (HRRN_queue == NULL) {
		return 1;
	}
	return 0;
}

process *HRRN_new_arrival(process *arriving_process, process *running_process)
{
	// TODO

	if (arriving_process != NULL) {
		HRRN_process *hrrn_process = malloc(sizeof(HRRN_process));
		hrrn_process->this_process = arriving_process;
		hrrn_process->waiting_time = 0;
		hrrn_process->rr = 1; //duration / duration

		queue_add(hrrn_process, HRRN_queue);
    }

    return running_process;
}

void HRRN_finish()
{
	// TODO
	free_queue(HRRN_queue);
}
