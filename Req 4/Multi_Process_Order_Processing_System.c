#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc , char* argv[])
{
    if(argc != 4)
    {
        fprintf(stderr,"Invalid Input\n");
        return 1;
    } 

    const char *input_file = argv[1];
    int N = atoi(argv[2]);
    int V = atoi(argv[3]);

    FILE *fptr = fopen(input_file,"r");
    if(fptr == NULL)
    {
        fprintf(stderr,"File doesn't exist\n");
        exit(1);
    }

    int orders;
    fscanf(fptr,"%d",&orders);

    int *total = malloc(orders * sizeof(int));
    if(total == NULL)
        exit(1);

    for(int i = 0 ; i < orders ; i++)
    {
        int items , price;
        fscanf(fptr,"%d %d",&items,&price);
        total[i] = items * price;
    }

    fclose(fptr);

    int base_batch_size = orders / N;
    int remaining = orders % N;

    int pids[N];
    int results[N];

    for (int i = 0 ; i < N ; i++)
    {
        int pid = fork();

        if(pid == -1)
        {
            perror("fork failed");
            exit(1);
        }

        if(pid == 0)
        {
            int start = i * base_batch_size;
            int end;

            if(i == N - 1)
                end = start + base_batch_size + remaining;
            else
                end = start + base_batch_size;

            int count = 0;

            for(int j = start; j < end; j++)
            {
                if(total[j] >= V)
                    count++;
            }

            free(total);
            exit(count);
        }
        else
        {
            pids[i] = pid;
        }
    }

    for(int i = 0; i < N; i++)
    {
        int status;
        int finished_pid = wait(&status);

        int result = WEXITSTATUS(status);

        for(int j = 0; j < N; j++)
        {
            if(pids[j] == finished_pid)
            {
                results[j] = result;
                break;
            }
        }
    }

    for(int i = 0; i < N; i++)
    {
        printf("%d", results[i]);
        if(i != N-1)
            printf(" ");
    }

    free(total);

    return 0;
}
