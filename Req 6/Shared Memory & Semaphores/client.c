#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>

struct shared_data {
    int client_id;
    int choice;
    char text[256];
    int result;
};

struct shared_data *data;

void SIG_HANDLER(int signum)
{
    if (data != (void *) -1)
        shmdt(data);

    exit(0);
}

int main()
{
    key_t shm_key = ftok("keyFile", 65);
    key_t sem_key = ftok("keyFile", 75);

    int shm_id = shmget(shm_key, sizeof(struct shared_data), 0666);
    int sem_id = semget(sem_key, 3, 0666);

    if (shm_id == -1 || sem_id == -1)
    {
        printf("Error in Connection\n");
        exit(1);
    }

    data = (struct shared_data *) shmat(shm_id, NULL, 0);

    while (1)
    {
        int choice;
        char sentence[256];
        printf("CHOOSE:");
        printf("\n1. Count Words\n2. Count Vowels\nChoice: ");
        scanf("%d", &choice);
        getchar();

        if (choice != 1 && choice != 2)
        {
            printf("Invalid choice\n");
            continue;
        }

        printf("Enter sentence: ");
        fgets(sentence, sizeof(sentence), stdin);
        sentence[strcspn(sentence, "\n")] = '\0';

        if (semctl(sem_id, 0, GETVAL) == -1)
        {
            printf("Failure in Communication\n");
            exit(1);
        }

        // lock mutex
        struct sembuf lock = {0, -1, SEM_UNDO};
        semop(sem_id, &lock, 1);

        // write request
        data->client_id = getpid();
        data->choice = choice;
        strcpy(data->text, sentence);

        // notify server
        struct sembuf signal_req = {1, 1, 0};
        semop(sem_id, &signal_req, 1);

        // wait for response
        struct sembuf wait_res = {2, -1, 0};
        if (semop(sem_id, &wait_res, 1) == -1)
        {
            perror("Waiting for response failed (server may be down)");
            exit(1);
        }

        // read result
        printf("Result = %d\n", data->result);

        // unlock mutex
        struct sembuf unlock = {0, 1, SEM_UNDO};
        semop(sem_id, &unlock, 1);
    }
}
