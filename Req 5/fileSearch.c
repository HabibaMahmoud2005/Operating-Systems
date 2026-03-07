#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <sys/wait.h>

pid_t pidA, pidB;

void loser_handler(int sig)
{
    printf("I am the child and I received from my parent that I am the loser.\n");
    fflush(stdout);
    exit(0);
}

void notfound_handler(int sig)
{
    printf("I am the child and I could not find the file.\n");
    fflush(stdout);
    exit(0);
}

void alarm_handler(int sig)
{
    kill(pidA, SIGUSR2);
    kill(pidB, SIGUSR2);
}

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        printf("Invalid Input\n");
        fflush(stdout);
        return 1;
    }

    int target_size = atoi(argv[1]);
    int num_files = argc - 2;

    int size1, size2;

    if (num_files % 2 == 0)
    {
        size1 = num_files / 2;
        size2 = num_files / 2;
    }
    else
    {
        size1 = num_files / 2 + 1;
        size2 = num_files / 2;
    }

    pidA = fork();

    if (pidA == 0)
    {
        // Child A

        signal(SIGUSR1, loser_handler);
        signal(SIGUSR2, notfound_handler);

        struct stat info;

        for (int i = 0; i < size1; i++)
        {
            if (stat(argv[2 + i], &info) == 0)
            {
                if (info.st_size == target_size)
                {
                    printf("I found the file at location %d.\n", i);
                    fflush(stdout);
                    exit(1); // WINNER
                }
            }
        }

        pause();
    }
    else
    {
        pidB = fork();

        if (pidB == 0)
        {
            // Child B

            signal(SIGUSR1, loser_handler);
            signal(SIGUSR2, notfound_handler);

            struct stat info;

            for (int i = 0; i < size2; i++)
            {
                if (stat(argv[2 + size1 + i], &info) == 0)
                {
                    if (info.st_size == target_size)
                    {
                        printf("I found the file at location %d.\n", size1 + i);
                        fflush(stdout);
                        exit(1); // WINNER
                    }
                }
            }

            pause();
        }
        else
        {
            // Parent

            signal(SIGALRM, alarm_handler);
            alarm(5);

            int status;
            pid_t finished = wait(&status);

            alarm(0);

            if (WIFEXITED(status) && WEXITSTATUS(status) == 1)
            {
                if (finished == pidA)
                {
                    printf("Parent: Child A found the file.\n");
                    fflush(stdout);
                    kill(pidB, SIGUSR1);
                }
                else if (finished == pidB)
                {
                    printf("Parent: Child B found the file.\n");
                    fflush(stdout);
                    kill(pidA, SIGUSR1);
                }
            }
            // To catch the loser process so it doesn't become zombie one
            wait(NULL);
        }
    }

    return 0;
}
