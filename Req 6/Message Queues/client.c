#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

/* message buffer for msgsnd and msgrcv calls */
struct msgbuf {
    long mtype;              /* type of message */
    char mtext[256];         /* message text */
};

int main()
{
    key_t Wkey_id = ftok("keyFile", 65);
    key_t Vkey_id = ftok("keyFile", 75);

    int WordsQueue_id = msgget(Wkey_id, IPC_CREAT | 0666);
    int VowelsQueue_id = msgget(Vkey_id, IPC_CREAT | 0666);

    if (WordsQueue_id == -1)
    {
        perror("Error in Words Queue");
        exit(-1);
    }

    if (VowelsQueue_id == -1)
    {
        perror("Error in Vowels Queue");
        exit(-1);
    }
while(1){
    char sentence[256];
    int choice;

    printf("Choose service:\n");
    printf("1. Count Words\n");
    printf("2. Count Vowels\n");
    printf("Enter choice: ");
    scanf("%d", &choice);

    getchar(); // clear buffer

    printf("Enter a sentence: ");
    fgets(sentence, sizeof(sentence), stdin);
    sentence[strcspn(sentence, "\n")] = '\0';

    if (choice != 1 && choice != 2)
    {
        printf("Invalid Input\n");
        exit(-1);
    }

    struct msgbuf buf;
    int client_id = getpid() % 10000;

    buf.mtype = client_id;
    strcpy(buf.mtext, sentence);

    if (choice == 1)
    {
        // send to WORDS queue
        int SENDstatus = msgsnd(WordsQueue_id, &buf, sizeof(buf.mtext), !IPC_NOWAIT);

        if (SENDstatus == -1)
        {
            perror("Sending Through Words Queue Failed\n");
            exit(-1);
        }

        // receive result
        int RCVstatus = msgrcv(WordsQueue_id, &buf, sizeof(buf.mtext), client_id, !IPC_NOWAIT);

        if (RCVstatus == -1)
        {
            perror("Receiving Through Words Queue Failed\n");
            exit(-1);
        }

        printf("Result: %s\n", buf.mtext);
    }
    else if (choice == 2)
    {
        // send to VOWELS queue
        int SENDstatus = msgsnd(VowelsQueue_id, &buf, sizeof(buf.mtext), !IPC_NOWAIT);

        if (SENDstatus == -1)
        {
            perror("Sending Through Vowels Queue Failed\n");
            exit(-1);
        }

        // receive result
        int RCVstatus = msgrcv(VowelsQueue_id, &buf, sizeof(buf.mtext), client_id, !IPC_NOWAIT);

        if (RCVstatus == -1)
        {
            perror("Receiving Through Vowels Queue Failed\n");
            exit(-1);
        }

        printf("Result: %s\n", buf.mtext);
    }
}

    return 0;
}