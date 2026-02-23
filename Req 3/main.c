#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sub_tool.h"

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        fprintf(stderr, "Invalid usage.\n");
        return 1;
    }

    const char *input_file = argv[1];

//================= VIEW =================
    if (strcmp(argv[2], "-v") == 0)
    {
        if (argc == 5 && strcmp(argv[3], "-n") == 0)
        {
            int line = atoi(argv[4]);
            view_specific_line(input_file, line);
        }
        else if (argc == 3)
        {
            view_all(input_file);
        }
        else
        {
            fprintf(stderr, "Invalid usage.\n");
            return 1;
        }
    }

//================= FIRST =================
    else if (strcmp(argv[2], "-f") == 0)
    {
        if (argc != 4)
        {
            fprintf(stderr, "Invalid usage.\n");
            return 1;
        }

        int n = atoi(argv[3]);
        view_first_lines(input_file, n);
    }

//================= LAST =================
    else if (strcmp(argv[2], "-l") == 0)
    {
        if (argc != 4)
        {
            fprintf(stderr, "Invalid usage.\n");
            return 1;
        }

        int n = atoi(argv[3]);
        view_last_lines(input_file, n);
    }

//================= INSERT =================
    else if (strcmp(argv[2], "-i") == 0)
    {
        const char *text = argv[3];
        const char *save_file = NULL;

        if (argc == 4)
        {
            insert_line(input_file, text, NULL);
        }
        else if (argc == 6 && strcmp(argv[4], "-s") == 0)
        {
            save_file = argv[5];
            insert_line(input_file, text, save_file);
        }
        else
        {
            fprintf(stderr, "Invalid usage.\n");
            return 1;
        }
    }

//================= EDIT =================
    else if (strcmp(argv[2], "-e") == 0)
    {
        if (argc < 6)
        {
            fprintf(stderr, "Invalid usage.\n");
            return 1;
        }

        const char *text = argv[3];

        if (strcmp(argv[4], "-n") != 0)
        {
            fprintf(stderr, "Invalid usage.\n");
            return 1;
        }

        int line = atoi(argv[5]);
        const char *save_file = NULL;

        if (argc == 6)
        {
            edit_line(input_file, text, line, NULL);
        }
        else if (argc == 8 && strcmp(argv[6], "-s") == 0)
        {
            save_file = argv[7];
            edit_line(input_file, text, line, save_file);
        }
        else
        {
            fprintf(stderr, "Invalid usage.\n");
            return 1;
        }
    }

    else
    {
        fprintf(stderr, "Invalid flag.\n");
        return 1;
    }

    return 0;
}
