#ifndef SUB_TOOL_H
#define SUB_TOOL_H

#include <stdio.h>
#define MAX_LINE_LENGTH 1024

void view_all(const char *filename);
void view_specific_line(const char *filename, int line);
void view_first_lines(const char *filename, int n);
void view_last_lines(const char *filename, int n);
void insert_line(const char *filename, const char *text, const char *save_file);
void edit_line(const char *filename, const char *text, int line, const char *save_file);

#endif
