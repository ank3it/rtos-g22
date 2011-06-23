#include process.h
#include queue.h

struct process * current_process;
void * receive_message(int * sender_ID)
{  
        // check mailbox of current process... block it if
        // it isn't currently blocked so that it's expecting a message
        if(current_process->mailbox.count == 0)
        {
			enqueue(blocked_queue, current_process->ID, current_process->priority);
			return NULL;
        }

        // else grab the first message and remove it from the mailbox
        else
        {
			//!!! change if this does not match to how mailbox is implemented in pcb struct
			struct List * mailbox = current_process->mailbox;
			//struct message_envelope * msg_envelope =  // grab first msg from mailbox... should it be a simple dequeue on the mailbox??
			return msg_envelope->sender_ID;
    }
}
//remove shitty code later
                /*
                UINT32 *temp_SP;
                struct process * next_process;
                next_process = get_proc(process_ID);               

                current_process->curr_SP = temp_SP;
                next_process = dequeue(ready_queue);
                next_process->state = RUNNING;
                */      