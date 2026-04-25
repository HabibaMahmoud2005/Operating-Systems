// Simulate a system where a parent process (department) creates N child processes (TAs)

// Program inputs (via command line arguments):
// 1) File path containing student grades
// 2) N → number of TAs (child processes)
// 3) P → minimum passing total grade

// Input file format:
// - First line: number of students (S)
// - Next S lines: each contains two integers (midterm + final grades)

// Each TA (child process) is responsible for a subset of students:
// - Each TA gets floor(S/N) students
// - Remaining students go to the last TA

// Each child:
// - Computes how many students in its group passed (midterm + final >= P)
// - Sends result back to parent

// Parent process:
// - Creates N children using fork()
// - Collects results from all children (using wait / IPC)
// - Prints results in correct order (one line, space-separated)

// Output format (STRICT):
// Example: 2 1 2
// - No extra text, formatting, or new lines

// Constraints:
// - Do NOT use scanf/gets → use argv and file reading
// - Must use fork() for process creation
// - Do NOT assume order of process IDs (PIDs)

// Important process concepts:
// - Parent must wait for children to avoid zombie processes
// - Children may use exit() to send status (or use IPC if needed)

// Debug prints should be removed/commented before submission
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(int argc , char* argv[])
{
    if(argc != 4)
    {
        return 1;
    }

    const char *input_file = argv[1];
    int N = atoi(argv[2]);
    int P = atoi(argv[3]);

    FILE *fptr = fopen(input_file,"r");
    if(fptr == NULL)
    {
        return 1;
    }

    int S;
    fscanf(fptr,"%d",&S);

    // store total grades (mid + final)
    int *total = malloc(S * sizeof(int));
    if(total == NULL)
        return 1;

    for(int i = 0 ; i < S ; i++)
    {
        int mid , fin;
        fscanf(fptr,"%d %d",&mid,&fin);
        total[i] = mid + fin;
    }

    fclose(fptr);

    int base_batch_size = S / N;
    int remaining = S % N;

    int pids[N];
    int results[N];

    for (int i = 0 ; i < N ; i++)
    {
        int pid = fork();

        if(pid == -1)
        {
            return 1;
        }

        if(pid == 0)
        {
            // CHILD (TA)

            int start = i * base_batch_size;
            int end;

            if(i == N - 1)
                end = start + base_batch_size + remaining;
            else
                end = start + base_batch_size;

            int count = 0;

            for(int j = start; j < end; j++)
            {
                if(total[j] >= P)
                    count++;
            }

            free(total);
            exit(count);  // send result to parent
        }
        else
        {
            // PARENT
            pids[i] = pid;
        }
    }

    // collect results
    for(int i = 0; i < N; i++)
    {
        int status;
        int finished_pid = wait(&status);

        int result = WEXITSTATUS(status);

        // match result to correct TA
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

    printf("\n");

    free(total);

    return 0;
}