#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

/* message buffer */
struct msgbuf {
    long mtype;
    int client_id;
    char mtext[256];
};

int WordsQueue_id;
int VowelsQueue_id;

/* SIGINT handler */
void SIG_HANDLER(int signum)
{
    msgctl(WordsQueue_id, IPC_RMID, NULL);
    msgctl(VowelsQueue_id, IPC_RMID, NULL);
    printf("\nServer terminated. Queues removed.\n");
    exit(0);
}

// counting functions
int count_vowels(const char *str)
{
    int count = 0;
    while (*str)
    {
        char c = *str;
        if (c=='a'||c=='e'||c=='i'||c=='o'||c=='u'||
            c=='A'||c=='E'||c=='I'||c=='O'||c=='U')
            count++;
        str++;
    }
    return count;
}

int count_words(const char *str)
{
    int count = 0, in_word = 0;
    while (*str)
    {
        if (*str != ' ' && in_word == 0)
        {
            count++;
            in_word = 1;
        }
        else if (*str == ' ')
        {
            in_word = 0;
        }
        str++;
    }
    return count;
}

int main()
{
    key_t Wkey = ftok("keyFile", 65);
    key_t Vkey = ftok("keyFile", 75);

    WordsQueue_id = msgget(Wkey, IPC_CREAT | 0666);
    VowelsQueue_id = msgget(Vkey, IPC_CREAT | 0666);

    if (WordsQueue_id == -1 || VowelsQueue_id == -1)
    {
        perror("Error creating queues");
        exit(1);
    }

    signal(SIGINT, SIG_HANDLER);

    printf("Server started... waiting 10 seconds\n");
    //sleep(25);
    sleep(10);

    struct msgbuf bufW, bufV;

    while (1)
    {
        // WORDS QUEUE
        if (msgrcv(WordsQueue_id, &bufW, sizeof(bufW) - sizeof(long), 1, IPC_NOWAIT) != -1)
        {
            int result = count_words(bufW.mtext);

            sprintf(bufW.mtext, "%d", result);
            bufW.mtype = bufW.client_id;

            msgsnd(WordsQueue_id, &bufW, sizeof(bufW) - sizeof(long), 0);

            //sleep(7);
            sleep(3);
        }

        // VOWELS QUEUE
        if (msgrcv(VowelsQueue_id, &bufV, sizeof(bufV) - sizeof(long), 1, IPC_NOWAIT) != -1)
        {
            int result = count_vowels(bufV.mtext);

            sprintf(bufV.mtext, "%d", result);
            bufV.mtype = bufV.client_id;

            msgsnd(VowelsQueue_id, &bufV, sizeof(bufV) - sizeof(long), 0);

            //sleep(7);
            sleep(3);
        }
    }

    return 0;
}