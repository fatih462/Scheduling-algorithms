#include "../lib/RR.h"

static queue_object *RR_queue;
static int RR_quantum;
static int current_tick;
/*typedef struct _RR_process {
	process *this_process;
	unsigned int time_in;
} RR_process;*/
//You can add more global variables

process *RR_tick(process *running_process) {	
	//TODO
	if(running_process == NULL) {
		running_process = queue_poll(RR_queue);
	}
	else {
		if(running_process->time_left > 0 && current_tick == RR_quantum) {
			queue_add(running_process, RR_queue);
			running_process = queue_poll(RR_queue);
			current_tick = 0;
		}
		else if(running_process->time_left == 0) {
			running_process = queue_poll(RR_queue);
			current_tick = 0;
		}

	}

	if (running_process != NULL) {
		running_process->time_left--;
		current_tick++;
		if (running_process->time_left == 0) {
			current_tick = RR_quantum; 
		}
	}

	return running_process;
}

int RR_startup(int quantum) {
	//TODO
	RR_queue = new_queue();
	if (RR_queue == NULL) {
		return 1;
	}
	RR_quantum = quantum;
	current_tick = 0;
	return 0;
}

process *RR_new_arrival(process *arriving_process, process *running_process) {
	//TODO
	if (arriving_process != NULL) {
		queue_add(arriving_process, RR_queue);
	}
	return running_process;
}

void RR_finish()
{
	//TODO
	free_queue(RR_queue);
}