#include "../lib/MLF.h"
#include <stdio.h>


static queue_object **MLF_queues;
static int level;
static int quantum;
static int current_tick;
//You can add more global variables here

void delete_from_queue3(queue_object *queue, process *process) {
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


process *highest_prio2(queue_object *queue) {
	if (queue == NULL || queue->next == NULL) {
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

unsigned int power_of_two(unsigned int exponent) {
    unsigned int result = 1;
    for (unsigned int i = 0; i < exponent; i++) {
        result *= 2;
    }
    return result;
}
void next_queue(queue_object *queue, process *process, int level1) {
	if(level1 < 3) {
		delete_from_queue3(queue, process);
		queue_add(process, MLF_queues[level1 + 1]);
	}
}

process *MLF_tick(process *running_process) {

	if(running_process == NULL) {
		
		for(int i = 0; i < 4; i++) {
			process *highest = highest_prio2(MLF_queues[i]);
			if(highest != NULL) {
				running_process = highest;
				quantum = power_of_two(i);
				current_tick = 0;
				level = i+1;
				if(level > 3) level = 3;
				next_queue(MLF_queues[i], running_process, i);
				break;
			}
			else {
				running_process = NULL;
			}
		}
	}
	else {
		if(current_tick == quantum && running_process->time_left > 0 && quantum != 8) {

			for(int i = 0; i < 4; i++) {
				process *highest = highest_prio2(MLF_queues[i]);
				if(highest != NULL) {
					running_process = highest;
					quantum = power_of_two(i);
					current_tick = 0;
					level = i+1;
					if(level > 3) level = 3;
					next_queue(MLF_queues[i], running_process, i);
					break;
				}
				else {
					running_process = NULL;
				}
			}
		}
		else if(running_process->time_left == 0) {
			delete_from_queue3(MLF_queues[level], running_process);

			for(int i = 0; i < 4; i++) {
				process *highest = highest_prio2(MLF_queues[i]);
				if(highest != NULL) {
					running_process = highest;
					quantum = power_of_two(i);
					current_tick = 0;
					level = i+1;
					if(level > 3) level = 3;
					next_queue(MLF_queues[i], running_process, i);
					break;
				}
				else {
					running_process = NULL;
				}
			}
		}
	}

	if (running_process != NULL) {
		running_process->time_left--;
		current_tick++;
	}
		

	return running_process;
}

int MLF_startup()
{
	MLF_queues = malloc(4 * sizeof(queue_object *));
    if (MLF_queues == NULL) {
		return 1;
	}
    for (int i = 0; i < 4; ++i) {
        MLF_queues[i] = new_queue();
    }
	if (MLF_queues[0]==NULL || MLF_queues[1]==NULL || MLF_queues[2]==NULL || MLF_queues[3]==NULL){
		return 1;
	}
    return 0;
		
}

process *MLF_new_arrival(process *arriving_process, process *running_process)
{
	if(arriving_process!=NULL){
		queue_add(arriving_process, MLF_queues[0]);
	}
	return running_process;

}

void MLF_finish()
{
	for (int i = 0; i < 4; ++i) {
        free_queue(MLF_queues[i]);
    }
    free(MLF_queues);
}

/*void moin(process *running_process) {
	for(int i = 0; i < 4; i++) {
		process *highest = highest_prio(MLF_queues[i]);
		if(highest != NULL) {
			running_process = highest;
			quantum = power_of_two(i);
			current_tick = 0;
			level = i+1;
			if(level > 3) level = 3;
				next_queue(MLF_queues[i], running_process, i);
				break;
			}
			else {
				running_process = NULL;
			}
		}
}*/