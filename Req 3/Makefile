CC = gcc
CFLAGS = -Wall -Wextra -std=c11

TARGET = sub_tool
OBJ = main.o sub_tool.o

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

main.o: main.c sub_tool.h
	$(CC) $(CFLAGS) -c main.c

sub_tool.o: sub_tool.c sub_tool.h
	$(CC) $(CFLAGS) -c sub_tool.c

clean:
	rm -f $(TARGET) $(OBJ)
