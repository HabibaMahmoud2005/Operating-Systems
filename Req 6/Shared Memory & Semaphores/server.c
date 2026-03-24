#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <signal.h>

union Semun {
    int val;
};

struct shared_data {
    int client_id;
    int choice;
    char text[256];
    int result;
};

int shm_id, sem_id;
struct shared_data *data;

void SIG_HANDLER(int signum)
{
    shmctl(shm_id, IPC_RMID, NULL);
    semctl(sem_id, 0, IPC_RMID);
    printf("\nServer terminated. Resources cleaned.\n");
    exit(0);
}

int count_vowels(const char *str)
{
    int count = 0;
    while (*str)
    {
        if (strchr("aeiouAEIOU", *str))
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
        if (*str != ' ' && !in_word)
        {
            count++;
            in_word = 1;
        }
        else if (*str == ' ')
            in_word = 0;
        str++;
    }
    return count;
}

int main()
{
    key_t shm_key = ftok("keyFile", 65);
    key_t sem_key = ftok("keyFile", 75);

    shm_id = shmget(shm_key, sizeof(struct shared_data), IPC_CREAT | 0666);
    sem_id = semget(sem_key, 3, IPC_CREAT | 0666);

     if (shm_id == -1) {
        perror("shmget");
        exit(-1);
    }

    if (sem_id == -1) {
        perror("semget");
        exit(-1);
    }

    union Semun semun;

    // Initialize mutex
    semun.val = 1;
    semctl(sem_id, 0, SETVAL, semun);

    // Initialize request
    semun.val = 0;
    semctl(sem_id, 1, SETVAL, semun);

    // Initialize response
    semun.val = 0;
    semctl(sem_id, 2, SETVAL, semun);

    data = (struct shared_data *) shmat(shm_id, NULL, 0);
    if (data == (void *) -1) {
        perror("shmat");
        exit(-1);
    }


    signal(SIGINT, SIG_HANDLER);

    printf("Server started...\n");
    //sleep(25);
    sleep(10);

    while (1)
    {
        // wait for request
        struct sembuf wait_req = {1, -1, 0};
        semop(sem_id, &wait_req, 1);

        // process
        if (data->choice == 1)
            data->result = count_words(data->text);
        else
            data->result = count_vowels(data->text);

        // signal response
        struct sembuf signal_res = {2, 1, 0};
        semop(sem_id, &signal_res, 1);

        //sleep(7);
        sleep(3);
    }
}