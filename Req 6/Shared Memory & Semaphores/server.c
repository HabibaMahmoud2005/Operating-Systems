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

    union Semun semun;
    semun.val = 0;

    // initialize BOTH semaphores
    semctl(sem_id, 0, SETVAL, semun);
    semctl(sem_id, 1, SETVAL, semun);

    struct shared_data *data = (struct shared_data *) shmat(shm_id, NULL, 0);
    if (data == (void *) -1)
    {
        perror("Error in shmat");
        exit(-1);
    }

    while(1){
        sem_op(sem_id,0,-1);

        int result;
        if (data->choice == 1)
            result = count_words(data->text);
        else
            result = count_vowels(data->text);

        data->result = result;

        sem_op(sem_id,1,1);
    }
    
    return 0;
}