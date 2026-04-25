/*
This program implements a multi-process search using fork() and signals.

The parent process receives a target value and an array of integers from
the command line, initializes the array, prints its PID, then creates two
child processes. Each child is responsible for searching half of the array.

Each child further forks two secondary (grandchild) processes, dividing
its assigned half into two quarters. These grandchildren perform the
actual search on their respective segments.

If any process finds the target value, it sends a SIGUSR1 signal to the
parent and returns the position (1-based index) via exit status.

If not found, processes sleep for a specified time, print a termination
message, and exit normally.

The parent waits for all children and prints "Value Not Found" if no
signal is received.
*/
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>

int found = 0;

void signal_handler(int signum)
{
    found = 1;
}

int main(int argc, char *argv[])
{
    signal(SIGUSR1, signal_handler);

    if (argc != 22)
    {
        fprintf(stderr, "INVALID ARGUMENTS\n");
        return 1;
    }

    int target = atoi(argv[1]);

    int *array = malloc(sizeof(int) * 20);
    for (int i = 0; i < 20; i++)
    {
        array[i] = atoi(argv[i + 2]);
    }

    printf("I AM THE PARENT, PID: %d\n", getpid());

    int pid = fork();
    if (pid < 0)
    {
        return 1;
    }
    else if (pid == 0)
    {
        // ===== FIRST CHILD =====
        printf("I AM THE FIRST CHILD, PID: %d, PPID: %d\n", getpid(), getppid());

        int first_child = fork();
        if (first_child == 0)
        {
            // GRANDCHILD 1-1 (0 → 4)
            printf("I am child 1-1, PID: %d, PPID: %d\n", getpid(), getppid());

            for (int i = 0; i < 5; i++)
            {
                if (array[i] == target)
                {
                    kill(getppid(), SIGUSR1);
                    exit(i + 1);
                }
            }

            sleep(5);
            printf("Child 1-1 terminates\n");
            exit(0);
        }

        int second_child = fork();
        if (second_child == 0)
        {
            // GRANDCHILD 1-2 (5 → 9)
            printf("I am child 1-2, PID: %d, PPID: %d\n", getpid(), getppid());

            for (int i = 5; i < 10; i++)
            {
                if (array[i] == target)
                {
                    kill(getppid(), SIGUSR1);
                    exit(i + 1);
                }
            }

            sleep(5);
            printf("Child 1-2 terminates\n");
            exit(0);
        }

        // wait for grandchildren
        for (int i = 0; i < 2; i++)
        {
            int status;
            wait(&status);

            if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
            {
                kill(getppid(), SIGUSR1);
                exit(WEXITSTATUS(status));
            }
        }

        sleep(5);
        printf("Child 1 terminates\n");
        exit(0);
    }
    else
    {
        int pid2 = fork();
        if (pid2 == 0)
        {
            // ===== SECOND CHILD =====
            printf("I AM THE SECOND CHILD, PID: %d, PPID: %d\n", getpid(), getppid());

            int first_child2 = fork();
            if (first_child2 == 0)
            {
                // GRANDCHILD 2-1 (10 → 14)
                printf("I am child 2-1, PID: %d, PPID: %d\n", getpid(), getppid());

                for (int i = 10; i < 15; i++)
                {
                    if (array[i] == target)
                    {
                        kill(getppid(), SIGUSR1);
                        exit(i + 1);
                    }
                }

                sleep(5);
                printf("Child 2-1 terminates\n");
                exit(0);
            }

            int second_child2 = fork();
            if (second_child2 == 0)
            {
                // GRANDCHILD 2-2 (15 → 19)
                printf("I am child 2-2, PID: %d, PPID: %d\n", getpid(), getppid());

                for (int i = 15; i < 20; i++)
                {
                    if (array[i] == target)
                    {
                        kill(getppid(), SIGUSR1);
                        exit(i + 1);
                    }
                }

                sleep(5);
                printf("Child 2-2 terminates\n");
                exit(0);
            }

            // wait for grandchildren
            for (int i = 0; i < 2; i++)
            {
                int status;
                wait(&status);

                if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
                {
                    kill(getppid(), SIGUSR1);
                    exit(WEXITSTATUS(status));
                }
            }

            sleep(5);
            printf("Child 2 terminates\n");
            exit(0);
        }
        else
        {
            // ===== PARENT =====
            sleep(25);

            for (int i = 0; i < 2; i++)
                // wait for both children to terminate before it terminates itself
                wait(NULL);

            if (!found)
                printf("Value Not Found\n");
        }
    }

    free(array);
    return 0;
}