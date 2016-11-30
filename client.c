#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>      /* For I/O operations */
#include <string.h>

struct myMsg {
    long mtype;
    char mtext[128];
};

int main(int argc, char **argv) {

    if(argc != 2) {
        printf("You need to run this program with your username like:\n./client damian");
        return -1;
    }

    key_t key = ftok("/home/", 150);
    if(key < 0) {
        perror("key ");
    }
    /* get a System V message queue identifier */
    int queue_id = msgget(key, IPC_CREAT | 0640);
    if(queue_id < 0) {
        perror("queu_id ");
    }
    struct myMsg message;
    /* User input */
    char buf[128];

    while(1) {
        strcpy(message.mtext, argv[1]);
        fgets(buf, 128, stdin);
        strcat(message.mtext, ": ");
        strcat(message.mtext, buf);
        printf("message to be sent: %s \n", message.mtext);
        if(msgsnd(queue_id, &message, sizeof(message.mtext), 0) < 0) {
            perror("msgsnd ");
        }
        if(msgrcv(queue_id, &message, sizeof(message.mtext), 0, 0) < 0) {
            perror("msgrcv ");
        }

        printf("%s\n", message.mtext);
    }

    return 0;
}