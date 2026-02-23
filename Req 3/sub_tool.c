#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sub_tool.h"


// HELPER FUNCTION
static int count_lines(const char *filename)
{
    FILE *fptr = fopen(filename, "r");
    if (fptr == NULL)
        return -1;

    char buffer[MAX_LINE_LENGTH];
    int count = 0;

    while (fgets(buffer, MAX_LINE_LENGTH, fptr) != NULL)
        count++;

    fclose(fptr);
    return count;
}

void view_all(const char *filename)
{
    FILE* fptr = fopen(filename,"r");
    if(fptr == NULL){
        fprintf(stderr, "Error: File does not exist.\n");
        exit(0);
    }
    char buffer[MAX_LINE_LENGTH];
    while(fgets(buffer,MAX_LINE_LENGTH,fptr) != NULL)
    {
        printf("%s",buffer);
    }
    fclose(fptr);
}

void view_specific_line(const char *filename, int line)
{
    int total = count_lines(filename);

    if (total == -1)
    {
        fprintf(stderr, "Error: File does not exist.\n");
        exit(0);
    }

    FILE *fptr = fopen(filename, "r");

    if (line <= 0 || line > total) 
    {
        fprintf(stderr, "Error: Line out of bounds.\n");
        fclose(fptr);
        exit(0);
    }

    char buffer[MAX_LINE_LENGTH];
    int current_line = 1;

    while (fgets(buffer, MAX_LINE_LENGTH, fptr) != NULL)
    {
        if (current_line == line)
        {
            printf("%s", buffer);
            fclose(fptr);
            return;
        }
        current_line++;
    }
    fclose(fptr);
}

void view_first_lines(const char *filename, int n)
{
    int total = count_lines(filename);

    if (total == -1)
    {
        fprintf(stderr, "Error: File does not exist.\n");
        exit(0);
    }

    // if (n < 1)
    // {
    //     fprintf(stderr, "Error: Line out of bounds.\n");
    //     exit(0);
    // }

    if (n > total)
    {
        fprintf(stderr, "Error: File limit exceeded.\n");
        exit(0);
    }

    FILE *fptr = fopen(filename, "r");

    char buffer[MAX_LINE_LENGTH];
    int count = 0;

    while (fgets(buffer, MAX_LINE_LENGTH, fptr) != NULL && count < n)
    {
        printf("%s", buffer);
        count++;
    }

    fclose(fptr);
}

void view_last_lines(const char *filename, int n)
{
    int total = count_lines(filename);

    if (total == -1)
    {
        fprintf(stderr, "Error: File does not exist.\n");
        exit(0);
    }

    // if (n < 1)
    // {
    //     fprintf(stderr, "Error: Line out of bounds.\n");
    //     exit(0);
    // }

    if (n > total)
    {
        fprintf(stderr, "Error: File limit exceeded.\n");
        exit(0);
    }

    FILE *fptr = fopen(filename, "r");

    char buffer[MAX_LINE_LENGTH];
    int current_line = 1;
    int start_line = total - n + 1;

    while (fgets(buffer, MAX_LINE_LENGTH, fptr) != NULL)
    {
        if (current_line >= start_line)
            printf("%s", buffer);

        current_line++;
    }

    fclose(fptr);
}

void insert_line(const char *filename,const char *text,const char *save_file)
{
    FILE *in = fopen(filename, "r");
    if (in == NULL)
    {
        fprintf(stderr, "Error: File does not exist.\n");
        exit(0);
    }

    const char *out_name = (save_file != NULL) ? save_file : "temp.txt";

    FILE *out = fopen(out_name, "w");
    if (out == NULL)
    {
        fclose(in);
        exit(0);
    }

    char buffer[MAX_LINE_LENGTH];

    while (fgets(buffer, MAX_LINE_LENGTH, in) != NULL)
        fputs(buffer, out);

    fprintf(out, "%s\n", text);

    fclose(in);
    fclose(out);

    if (save_file == NULL)
    {
        remove(filename);
        rename("temp.txt", filename);
    }
}

void edit_line(const char *filename,const char *text,int line,const char *save_file)
{
    int total = count_lines(filename);

    if (total == -1)
    {
        fprintf(stderr, "Error: File does not exist.\n");
        exit(0);
    }

    if (line < 1 || line > total)
    {
        fprintf(stderr, "Error: Line out of bounds.\n");
        exit(0);
    }

    FILE *in = fopen(filename, "r");
    const char *out_name = (save_file != NULL) ? save_file : "temp.txt";
    FILE *out = fopen(out_name, "w");

    char buffer[MAX_LINE_LENGTH];
    int current = 1;

    while (fgets(buffer, MAX_LINE_LENGTH, in) != NULL)
    {
        if (current == line)
            fprintf(out, "%s\n", text);
        else
            fputs(buffer, out);

        current++;
    }

    fclose(in);
    fclose(out);

    if (save_file == NULL)
    {
        remove(filename);
        rename("temp.txt", filename);
    }
}
