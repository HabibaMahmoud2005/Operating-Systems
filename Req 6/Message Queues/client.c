#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

/* message buffer */
struct msgbuf {
    long mtype;
    int client_id;
    char mtext[256];
};

int main()
{
    key_t Wkey = ftok("keyFile", 65);
    key_t Vkey = ftok("keyFile", 75);

    int WordsQueue_id = msgget(Wkey,0666);
    int VowelsQueue_id = msgget(Vkey,0666);

    if (WordsQueue_id == -1 || VowelsQueue_id == -1)
    {
        perror("Error connecting to queues");
        exit(1);
    }

    while (1)
    {
        int choice;
        char sentence[256];
        printf("CHOOSE:");
        printf("\n1. Count Words\n2. Count Vowels\nChoice: ");
        scanf("%d", &choice);
        getchar(); // clear buffer

        if (choice != 1 && choice != 2)
        {
            printf("Invalid choice\n");
            continue;
        }

        printf("Enter sentence: ");
        fgets(sentence, sizeof(sentence), stdin);
        sentence[strcspn(sentence, "\n")] = '\0';

        struct msgbuf buf;
        int client_id = getpid() % 10000;

        buf.client_id = client_id;
        buf.mtype = 1;
        strcpy(buf.mtext, sentence);

        if (choice == 1)
        {
            int SENDstatus = msgsnd(WordsQueue_id, &buf, sizeof(buf) - sizeof(long), !IPC_NOWAIT);

            if (SENDstatus == -1)
            {
                perror("Sending Through Words Queue Failed");
                exit(1);
            }

            int RCVstatus = msgrcv(WordsQueue_id, &buf, sizeof(buf) - sizeof(long), client_id, !IPC_NOWAIT);

            if (RCVstatus == -1)
            {
                perror("Receiving Through Words Queue Failed");
                exit(1);
            }

            printf("Word Count: %s\n", buf.mtext);
        }
        else if (choice == 2)
        {
            int SENDstatus = msgsnd(VowelsQueue_id, &buf, sizeof(buf) - sizeof(long), !IPC_NOWAIT);

            if (SENDstatus == -1)
            {
                perror("Sending Through Vowels Queue Failed");
                exit(1);
            }

            int RCVstatus = msgrcv(VowelsQueue_id, &buf, sizeof(buf) - sizeof(long), client_id, !IPC_NOWAIT);

            if (RCVstatus == -1)
            {
                perror("Receiving Through Vowels Queue Failed");
                exit(1);
            }

            printf("Vowel Count: %s\n", buf.mtext);
        }
    }

    return 0;
}