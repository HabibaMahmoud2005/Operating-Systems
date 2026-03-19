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

int WordsQueue_id;
int VowelsQueue_id;

void SIG_HANDELR(int SIGNUM){
    msgctl(WordsQueue_id, IPC_RMID, NULL);
    msgctl(VowelsQueue_id, IPC_RMID, NULL);
    exit(0);
}

int count_vowels(const char *str)
{
    int count = 0;
    while (*str)
    {
        char c = *str;
        if (c=='a'||c=='e'||c=='i'||c=='o'||c=='u'||
            c=='A'||c=='E'||c=='I'||c=='O'||c=='U')
        {
            count++;
        }
        str++;
    }
    return count;
}

int count_words(const char *str)
{
    int count = 0;
    int in_word = 0;
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
    key_t Wkey_id = ftok("keyFile", 65);
    key_t Vkey_id = ftok("keyFile", 75);

    WordsQueue_id = msgget(Wkey_id, IPC_CREAT | 0666);
    VowelsQueue_id = msgget(Vkey_id, IPC_CREAT | 0666);

    signal(SIGINT, SIG_HANDELR);
    sleep(10);

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

    struct msgbuf bufW, bufV;

    while(1)
    {
        int RCVstatusW = msgrcv(WordsQueue_id, &bufW, sizeof(bufW.mtext), 0, IPC_NOWAIT);
        int RCVstatusV = msgrcv(VowelsQueue_id, &bufV, sizeof(bufV.mtext), 0, IPC_NOWAIT);

        if (RCVstatusW != -1)
        {
            int result = count_words(bufW.mtext);
            int client_id = bufW.mtype;

            sprintf(bufW.mtext, "%d", result);
            bufW.mtype = client_id;

            msgsnd(WordsQueue_id, &bufW, sizeof(bufW.mtext), !IPC_NOWAIT);
        }

        if (RCVstatusV != -1)
        {
            int result = count_vowels(bufV.mtext);
            int client_id = bufV.mtype;

            sprintf(bufV.mtext, "%d", result);
            bufV.mtype = client_id;

            msgsnd(VowelsQueue_id, &bufV, sizeof(bufV.mtext), !IPC_NOWAIT);
        }
    }
    return 0;
}