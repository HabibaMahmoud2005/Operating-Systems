#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>

/* arg for semctl system calls. */
union Semun 
{
    int val;               /* Value for SETVAL */
    struct semid_ds *buf;  /* Buffer for IPC_STAT, IPC_SET */
    unsigned short *array; /* Array for GETALL, SETALL */
    struct seminfo *__buf; /* Buffer for IPC_INFO (Linux-specific) -> gives system-wide semaphore configuration / limits on Linux like max number of semaphores */
};

struct shared_data {
    int client_id;
    int choice;       // 1 = words, 2 = vowels
    char text[256];
    int result;
};

void sem_op(int sem_id, int sem_num, int op_val)
{
    struct sembuf op;
    op.sem_num = sem_num;
    op.sem_op = op_val;
    op.sem_flg = !IPC_NOWAIT;

    if (semop(sem_id, &op, 1) == -1)
    {
        perror("semop failed");
        exit(-1);
    }
}


int main()
{
    key_t shm_key = ftok("keyFile",65);
    key_t sem_key = ftok("keyFile",75);

    int shm_id = shmget(shm_key,sizeof(struct  shared_data),IPC_CREAT | 0666);
    if(shm_id == -1){
        perror("Error in Shared Memory\n");
        exit(-1);
    }

    int sem_id = semget(sem_key,2,IPC_CREAT | 0666);
    if(sem_id == -1){
        perror("Error in Semaphore\n");
    }

    struct shared_data *data = (struct shared_data *) shmat(shm_id, NULL, 0);
    if (data == (void *) -1)
    {
        perror("Error in shmat");
        exit(-1);
    }


    char sentence[sizeof(data->text)];
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

    data->client_id = getpid() % 10000;
    data->choice = choice;
    strcpy(data->text, sentence);

    sem_op(sem_id,0,1);
    sem_op(sem_id,1,-1);

    printf("Result = %d\n", data->result);

    return 0;
}