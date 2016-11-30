#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>      /* For I/O operations */
#include <signal.h>     /* For ending server loop with CTRL + C */
#include <string.h>

int queue_id;

struct myMsg {
    long mtype;
    char mtext[128];
};

struct sigaction old_action;

void sigint_handler(int sig_no)
{
    printf("Closing IPC message queue...\n");
    /* Destroy message queue */
    msgctl(queue_id, IPC_RMID, 0);

    sigaction(SIGINT, &old_action, NULL);
    kill(0, SIGINT);
}

int main(int argc, char **argv) {

    struct sigaction action;
    memset(&action, 0, sizeof(action));
    action.sa_handler = &sigint_handler;
    sigaction(SIGINT, &action, &old_action);

    key_t key = ftok("/home/", 150);
    if(key < 0) {
        perror("key: ");
    }
    /* get a System V message queue identifier */
    queue_id = msgget(key, IPC_CREAT | 0640);
    if(queue < 0) {
        perror("queue: ");
    }
    struct myMsg message;

    /* Server loop */
    while(1) {
        if(msgrcv(queue_id, &message, sizeof(message.mtext), 0, MSG_NOERROR) < 0) {
            perror("msgrcv ");
        }
        printf("%s\n", message.mtext);
        strcpy(message.mtext, "Hello dear client, what can I do for you?");
        if(msgsnd(queue_id, &message, sizeof(message.mtext), 0) < 0) {
            perror("msgsnd");
        }
    }


    return 0;
}